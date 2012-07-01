#include "modifier.h"
#include "hexoperations.h"
#include <stdio.h>
#include <iomanip>
#include <sstream>
#include <math.h>

void change_xp(HANDLE proc, string offset)
{
	long address = (DWORD)str2dword(hexadd(offset, "19"));

	SIZE_T writen = 0;

	struct opcode * ops = new opcode[4];

	ops[0].payload[0] = 0x01; //add edx, eax
	ops[0].payload[1] = 0xc2;
	ops[0].size = 2;

	ops[1].payload[0] = 0x8B; //mov eax, [ebp-28]
	ops[1].payload[1] = 0x45;
	ops[1].payload[2] = 0xD8;
	ops[1].size = 3;

	ops[2].payload[0] = 0x81; //add edx, 00010000
	ops[2].payload[1] = 0xC2;
	ops[2].payload[2] = 0x00;
	ops[2].payload[3] = 0x30;
	ops[2].payload[4] = 0x00;
	ops[2].payload[5] = 0x00;
	ops[2].size = 6;

	ops[3].payload[0] = 0xE9; //jmp Torment.exe+C55E1
	ops[3].payload[1] = 0xBE;
	ops[3].payload[2] = 0x55;
	ops[3].payload[3] = 0x0C;
	ops[3].payload[4] = 0x00;
	ops[3].size = 5;

	patch(proc, ops, 4, (unsigned long *)address);

	delete ops;

	address = (DWORD)str2dword(hexadd(offset, "C55E2"));
	ops = new opcode[2];
	ops[0].payload[0] = 0xE9;  //jmp Torment.exe+19
	ops[0].payload[1] = 0x32;
	ops[0].payload[2] = 0xAA;
	ops[0].payload[3] = 0xF3;
	ops[0].payload[4] = 0xFF;
	ops[0].size = 5;
	
	patch(proc, ops, 1, (unsigned long *)address);

	delete ops;
}

void revert_xp(HANDLE proc, string offset)
{
	//0x03D0
	//0x8B45D8
	
	long address = (DWORD)str2dword(hexadd(offset, "C55E2"));

	SIZE_T writen = 0;

	struct opcode * ops = new opcode[2];

	//This is two opcodes, can be re-written in two structs instead of one
	ops[0].payload[0] = 0x03; 
	ops[0].payload[1] = 0xD0;
	ops[0].payload[2] = 0x8B;
	ops[0].payload[3] = 0x45;
	ops[0].payload[4] = 0xD8;
	ops[0].size = 5;

	patch(proc, ops, 1, (unsigned long *)address);
}

using namespace std;

int main ( int argc, char *argv[] )
{

	if ( argc != 2 ) 
	{
		cout<< "usage: " << argv[0] << " <option>" << endl;
		exit(0);
	}
    
	std::string processName("Torment.exe");
	
	DWORD processID = FindProcessId(processName);

	if ( processID == 0 )
		std::wcout << "Could not find " << processName.c_str() << std::endl;
	else
		std::wcout << "Process ID is " << processID << std::endl;
	
	stringstream ss;
	string offset;
	ss << hex << GetModuleBase(processName.c_str(), processID);
	ss >> hex >> offset;

	HANDLE proc = AttachToProcess(processID, false);
	if(proc == NULL)
	{
		cout << "Could not get handle to process" << endl;
		system("PAUSE");
	}

	string argumentval(argv[1]);
	if(argumentval.compare("-xp=on") == 0)
	{
		change_xp(proc, offset);
	}
	else if(argumentval.compare("-xp=off") == 0)
	{
		revert_xp(proc, offset);
	}
	else
	{
		cout<< "usage: " << argv[0] << " <option>" << endl;
		exit(0);
	}
	return 0;
}
