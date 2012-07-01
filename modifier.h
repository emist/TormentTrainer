#ifndef MODIFIER
#define MODIFIER

#include <iostream>
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

struct opcode
{
	BYTE payload[20];
	int size;
};

DWORD FindProcessId(std::string processName);
HANDLE AttachToProcess(DWORD ProcessId, bool killonexit);
DWORD GetOffset(std::string exename);
DWORD GetModuleBase(LPCSTR lpModuleName, DWORD dwProcessId);
//unsigned long ReadMemory(HANDLE proc, DWORD address,SIZE_T size, SIZE_T * bytesRead);
LPVOID ReadMemory(HANDLE proc, DWORD address,SIZE_T size, SIZE_T * bytesRead);
BOOL WriteMemory(HANDLE proc, LPVOID address, LPCVOID buffer, SIZE_T size, SIZE_T * byteswritten);
BOOL patch(HANDLE proc, struct opcode op[], int bufflen, unsigned long * address);

#endif



