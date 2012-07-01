#include "modifier.h"
#include "hexoperations.h"

using namespace std;

BOOL SetPrivilege(HANDLE hToken,LPCTSTR lpszPrivilege,BOOL bEnablePrivilege) 
{
        TOKEN_PRIVILEGES tp;
        LUID luid;

        if ( !LookupPrivilegeValue(NULL,lpszPrivilege,&luid ) )// receives LUID of privilege
        {
                printf("LookupPrivilegeValue error: %u\n", GetLastError() ); 
                return FALSE; 
        }

        tp.PrivilegeCount = 1;
        tp.Privileges[0].Luid = luid;
        if (bEnablePrivilege)
                tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        else
                tp.Privileges[0].Attributes = 0;

        // Enable the privilege or disable all privileges.

        if (!AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(TOKEN_PRIVILEGES),(PTOKEN_PRIVILEGES) NULL,(PDWORD) NULL) )
        { 
                printf("AdjustTokenPrivileges error: %u\n", GetLastError() ); 
                return FALSE; 
        } 

        if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
        {
                printf("The token does not have the specified privilege. \n");
                return FALSE;
        } 

        return TRUE;
}

int iGetDebugPrivilege ( void ) 
{     
	HANDLE hToken;
	TOKEN_PRIVILEGES CurrentTPriv;
	LUID luidVal;
	
	if ( OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) == FALSE )
		return 0;    

	if ( LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luidVal) == FALSE )     
	{    
		CloseHandle( hToken );
		return 0; 
	}     
	
	CurrentTPriv.PrivilegeCount = 1; 
	CurrentTPriv.Privileges[0].Attributes  = SE_PRIVILEGE_ENABLED;
	CurrentTPriv.Privileges[0].Luid = luidVal; 
	int iRet = AdjustTokenPrivileges(hToken, FALSE, &CurrentTPriv, sizeof( TOKEN_PRIVILEGES ), NULL, NULL);
	CloseHandle(hToken); 
	return iRet; 
}

HANDLE AttachToProcess(DWORD ProcessId, bool killonexit)
{
	int err = 0;
	PHANDLE TokenHandle;

	if(!(err = DebugActiveProcess(ProcessId)))
	{
		cerr << "Could not attach to process, exiting" << endl;
		
	}

	if(!iGetDebugPrivilege())
	{
		cout << "Failed to set debug privileges" << endl;
	}
	
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD, FALSE, ProcessId);
	if(hProcess == NULL)
	{
		cout << "Cannot OpenProcess" << endl;
	}
	
	if(!(err = DebugSetProcessKillOnExit(killonexit)))
	{
		cerr << "Failed to set debug to not kill process on exit, process will die when this program exits" << endl;
	}

	return hProcess;
}

LPVOID ReadMemory(HANDLE proc, DWORD address,SIZE_T size, SIZE_T * bytesRead)
{
	BYTE * buf = new BYTE[size];

	for(int i = 0; i < size; i++)
	{
		cout <<" READMEM ADDRESS" << hex << (address+i) << endl;
		BOOL couldread = ReadProcessMemory(proc, (void*) (address+i), &buf[i], 1, bytesRead);
		//printf("%x\n", buf[i]);
		if(!couldread)
		{
			cout << "Error Reading Memory" << endl;
		}
	}

	return buf;
}

BOOL WriteOpcode(HANDLE proc, struct opcode op, unsigned long * address)
{
	stringstream ss;
	string addrstr;
	string offsetstr;
	bool ret;
	unsigned long addr = 0;
	addr = (unsigned long) address;
	
	for(int i = 0; i < op.size; i++)
	{
		ret = WriteMemory(proc, (LPVOID)addr, &op.payload[i], 1, NULL);
		
		ss.clear();
		ss << hex << addr;
		ss >> hex >> addrstr;
		
		ss.clear();	

		addr = str2dword(hexdecadd(addrstr, "1"));
	}
	return ret;
}

BOOL patch(HANDLE proc, struct opcode op[], int bufflen, unsigned long * address)
{
	unsigned long value = 0;
	unsigned long addr = 0;
	int offset = 0;
	stringstream ss;
	string hextest;
	string dectest;
	addr = (unsigned long)address;
	for(int i = 0; i < bufflen; i++)
	{
		unsigned long val = 0xc201;

		WriteOpcode(proc, op[i], (unsigned long *)addr);
		offset+=op[i].size;

		ss.clear();
		ss << address;
		ss >> hextest;
		ss.clear();
		ss << offset;
		ss >> dectest;

		addr = str2dword(hexdecadd(hextest, dectest));
	}
	return true;
}

BOOL WriteMemory(HANDLE proc, LPVOID address, LPCVOID buffer, SIZE_T size, SIZE_T * byteswritten)
{
	BOOL wrote = false;
	unsigned long hold = NULL;
	
    VirtualProtectEx(proc,(void*)address,size, PAGE_EXECUTE_READWRITE, &hold);
	if(!(wrote = WriteProcessMemory(proc, address, buffer, size, byteswritten)))
	{
		cout << "Memory Write Failed" << endl;
		cout << GetLastError() << endl;
	}
	VirtualProtectEx(proc,(void*)address, size, hold, NULL);

	return wrote;
}
/*
unsigned long ReadMemory(HANDLE proc, DWORD address,SIZE_T size, SIZE_T * bytesRead)
{
	unsigned long buffer = 0;
	//BOOL couldread = ReadProcessMemory(proc, (void *)0x4C55DA , &buffer, size, bytesRead);
	BOOL couldread = ReadProcessMemory(proc, (void*) address, &buffer, size, bytesRead);
	if(!couldread)
	{
		cout << "Error Reading Memory" << endl;
	}

	return buffer;
}
*/

DWORD GetModuleBase(LPCSTR lpModuleName, DWORD dwProcessId)
{
	MODULEENTRY32 lpModuleEntry = {0}; 

	HANDLE hSnapShot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwProcessId ); 

	if(!hSnapShot)
		return NULL; 

	lpModuleEntry.dwSize = sizeof(lpModuleEntry);

	BOOL bModule = Module32First( hSnapShot, &lpModuleEntry ); 

	while(bModule)
	{
		if(!strcmp( lpModuleEntry.szModule, lpModuleName ) )
		{
			CloseHandle( hSnapShot ); 
			
			return (DWORD)lpModuleEntry.modBaseAddr; 
		}

		bModule = Module32Next( hSnapShot, &lpModuleEntry );
	}
		
	CloseHandle( hSnapShot ); 

	return NULL; 
}

DWORD FindProcessId(std::string processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if ( processesSnapshot == INVALID_HANDLE_VALUE )
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if ( !processName.compare(processInfo.szExeFile) )
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while ( Process32Next(processesSnapshot, &processInfo) )
	{
		if ( !processName.compare(processInfo.szExeFile) )
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}
	
	CloseHandle(processesSnapshot);
	return 0;
}