#include "modifier.h"
#include "hexoperations.h"
#include <stdio.h>
#include <iomanip>
#include <sstream>
#include <math.h>

void change_xp(HANDLE proc, string offset)
{
	long address = (DWORD)str2dword(hexadd(offset, "19"));

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

void hpstatic(HANDLE proc, string offset)
{
	long address = (DWORD)str2dword(hexadd(offset, "333E3C"));

	struct opcode * ops = new opcode[8];

	ops[0].payload[0] = 0x0F; //movsx ecx, word ptr[eax+50c]
	ops[0].payload[1] = 0xBF;
	ops[0].payload[2] = 0x88;
	ops[0].payload[3] = 0x0C;
	ops[0].payload[4] = 0x05;
	ops[0].payload[5] = 0x00;
	ops[0].payload[6] = 0x00;
	ops[0].size = 7;

	ops[1].payload[0] = 0x50; //push eax
	ops[1].size = 1;

	ops[2].payload[0] = 0x05; //add eax, 50c
	ops[2].payload[1] = 0x0c;
	ops[2].payload[2] = 0x05;
	ops[2].payload[3] = 0x00;
	ops[2].payload[4] = 0x00;
	ops[2].size = 5;

	ops[3].payload[0] = 0x83; //cmp ecx, 14
	ops[3].payload[1] = 0xF9;
	ops[3].payload[2] = 0x14;
	ops[3].size = 3;

	ops[4].payload[0] = 0x7E; //jle Torment.exe+333E55
	ops[4].payload[1] = 0x07;
	ops[4].size = 2;

	ops[5].payload[0] = 0x89; //mov [Torment.exe+569A80], eax
	ops[5].payload[1] = 0x05;
	ops[5].payload[2] = 0x80;
	ops[5].payload[3] = 0x9A;
	ops[5].payload[4] = 0x96;
	ops[5].payload[5] = 0x00;
	ops[5].size = 6;

	ops[6].payload[0] = 0x58; //pop eax
	ops[6].size = 1;

	ops[7].payload[0] = 0xE9; //jmp Torment.exe+344B73
	ops[7].payload[1] = 0x19;
	ops[7].payload[2] = 0x0D;
	ops[7].payload[3] = 0x01;
	ops[7].payload[4] = 0x00;
	ops[7].size = 5;

	patch(proc, ops, 8, (unsigned long *)address);

	delete ops;

	ops = new opcode[1];

	ops[0].payload[0] = 0xE9; //jmp Torment.exe+344B73
	ops[0].payload[1] = 0xC9;
	ops[0].payload[2] = 0xF2;
	ops[0].payload[3] = 0xFE;
	ops[0].payload[4] = 0xFF;
	ops[0].payload[5] = 0x90;
	ops[0].payload[6] = 0x90;
	ops[0].size = 7;

	address = (DWORD)str2dword(hexadd(offset, "344B6E"));

	patch(proc, ops, 1, (unsigned long *)address);
	
	delete ops;
}

void godmode(HANDLE proc, string offset)
{
	long address = (DWORD)str2dword(hexadd(offset, "26937C"));

	struct opcode * ops = new opcode[8];

	ops[0].payload[0] = 0x51; //push ecx
	ops[0].size = 1;

	ops[1].payload[0] = 0x8B; //mov ecx, [Torment.exe+569A80]
	ops[1].payload[1] = 0x0D;
	ops[1].payload[2] = 0x80;
	ops[1].payload[3] = 0x9A;
	ops[1].payload[4] = 0x96;
	ops[1].payload[5] = 0x00;
	ops[1].size = 6;

	ops[2].payload[0] = 0x8B; //mov edx, [ebp-3c8]
	ops[2].payload[1] = 0x95;
	ops[2].payload[2] = 0x38;
	ops[2].payload[3] = 0xFC;
	ops[2].payload[4] = 0xFF;
	ops[2].payload[5] = 0xFF;
	ops[2].size = 6;

	ops[3].payload[0] = 0x39; //cmp edx, ecx
	ops[3].payload[1] = 0xCA;
	ops[3].size = 2;

	ops[4].payload[0] = 0x59; //pop ecx
	ops[4].size = 1;

	ops[5].payload[0] = 0x74; //je Torment.exe+26939B
	ops[5].payload[1] = 0x03;
	ops[5].size = 2;

	ops[6].payload[0] = 0x66; //mv [edx], cx
	ops[6].payload[1] = 0x89;
	ops[6].payload[2] = 0x0A;
	ops[6].size = 3;

	ops[7].payload[0] = 0xE9; //jmp Torment.exe+B3C12
	ops[7].payload[1] = 0x7C;
	ops[7].payload[2] = 0xA8;
	ops[7].payload[3] = 0xE4;
	ops[7].payload[4] = 0xFF;
	ops[7].size = 5;

	patch(proc, ops, 8, (unsigned long *)address);

	delete ops;
	
	ops = new opcode[1];

	ops[0].payload[0] = 0xE9; //jmp Torment.exe+344B73
	ops[0].payload[1] = 0x6A;
	ops[0].payload[2] = 0x57;
	ops[0].payload[3] = 0x1B;
	ops[0].payload[4] = 0x00;
	ops[0].payload[5] = 0x90;
	ops[0].payload[6] = 0x90;
	ops[0].payload[7] = 0x90;
	ops[0].payload[8] = 0x90;
	ops[0].size = 9;

	address = (DWORD)str2dword(hexadd(offset, "B3C0D"));

	patch(proc, ops, 1, (unsigned long *)address);
	
	delete ops;
	
}

void revert_instakill(HANDLE proc, string offset)
{
	long address = (DWORD)str2dword(hexadd(offset, "B3C0D"));

	struct opcode * ops = new opcode[2];

	//noping sub edx, 01
	ops[0].payload[0] = 0x8B; 
	ops[0].payload[1] = 0x95;
	ops[0].payload[2] = 0x38;
	ops[0].payload[3] = 0xFC;
	ops[0].payload[4] = 0xFF;
	ops[0].payload[5] = 0xFF;
	ops[0].size = 6;


	ops[1].payload[0] = 0x66; 
	ops[1].payload[1] = 0x89;
	ops[1].payload[2] = 0x0A;
	ops[1].size = 3;

	patch(proc, ops, 2, (unsigned long *)address);

	delete ops;
}

void instakill(HANDLE proc, string offset)
{
	long address = (DWORD)str2dword(hexadd(offset, "26937C"));

	struct opcode * ops = new opcode[8];

	ops[0].payload[0] = 0x51; //push ecx
	ops[0].size = 1;

	ops[1].payload[0] = 0x8B; //mov ecx, [Torment.exe+569A80]
	ops[1].payload[1] = 0x0D;
	ops[1].payload[2] = 0x80;
	ops[1].payload[3] = 0x9A;
	ops[1].payload[4] = 0x96;
	ops[1].payload[5] = 0x00;
	ops[1].size = 6;

	ops[2].payload[0] = 0x8B; //mov edx, [ebp-3c8]
	ops[2].payload[1] = 0x95;
	ops[2].payload[2] = 0x38;
	ops[2].payload[3] = 0xFC;
	ops[2].payload[4] = 0xFF;
	ops[2].payload[5] = 0xFF;
	ops[2].size = 6;

	ops[3].payload[0] = 0x39; //cmp edx, ecx
	ops[3].payload[1] = 0xCA;
	ops[3].size = 2;

	ops[4].payload[0] = 0x59; //pop ecx
	ops[4].size = 1;

	ops[5].payload[0] = 0x74; //je Torment.exe+269394
	ops[5].payload[1] = 0x06;
	ops[5].size = 2;

	ops[6].payload[0] = 0xC7; //mv [edx], 000
	ops[6].payload[1] = 0x02;
	ops[6].payload[2] = 0x00;
	ops[6].payload[3] = 0x00;
	ops[6].payload[4] = 0x00;
	ops[6].payload[5] = 0x00;
	ops[6].size = 6;

	ops[7].payload[0] = 0xE9; //jmp Torment.exe+B3C12
	ops[7].payload[1] = 0x7C;
	ops[7].payload[2] = 0xA8;
	ops[7].payload[3] = 0xE4;
	ops[7].payload[4] = 0xFF;
	ops[7].payload[5] = 0x90;
	ops[7].size = 5;

	patch(proc, ops, 8, (unsigned long *)address);

	delete ops;
	
	ops = new opcode[1];

	ops[0].payload[0] = 0xE9; //jmp Torment.exe+344B73
	ops[0].payload[1] = 0x6A;
	ops[0].payload[2] = 0x57;
	ops[0].payload[3] = 0x1B;
	ops[0].payload[4] = 0x00;
	ops[0].payload[5] = 0x90;
	ops[0].payload[6] = 0x90;
	ops[0].payload[7] = 0x90;
	ops[0].payload[8] = 0x90;
	ops[0].size = 9;

	address = (DWORD)str2dword(hexadd(offset, "B3C0D"));

	patch(proc, ops, 1, (unsigned long *)address);
	
	delete ops;
	
}


void revert_godmode(HANDLE proc, string offset)
{
	revert_instakill(proc, offset);
}


void unlimited_points(HANDLE proc, string offset)
{
	long address = (DWORD)str2dword(hexadd(offset, "215BDE"));

	struct opcode * ops = new opcode[1];

	//noping sub edx, 01
	ops[0].payload[0] = 0x90; 
	ops[0].payload[1] = 0x90;
	ops[0].payload[2] = 0x90;
	ops[0].size = 3;

	patch(proc, ops, 1, (unsigned long *)address);
}

void revert_unlimited_points(HANDLE proc, string offset)
{
	long address = (DWORD)str2dword(hexadd(offset, "215BDE"));

	struct opcode * ops = new opcode[1];

	//sub edx, 01
	ops[0].payload[0] = 0x83; 
	ops[0].payload[1] = 0xEA;
	ops[0].payload[2] = 0x01;
	ops[0].size = 3;

	patch(proc, ops, 1, (unsigned long *)address);
}

void set_maxhp(HANDLE proc, string offset)
{
	long address = (DWORD)str2dword(hexadd(offset, "44CAD"));

	struct opcode * ops = new opcode[1];

	//sub edx, 01
	ops[0].payload[0] = 0x66; 
	ops[0].payload[1] = 0xB8;
	ops[0].payload[2] = 0x99;
	ops[0].payload[3] = 0x79;
	ops[0].size = 4;

	patch(proc, ops, 1, (unsigned long *)address);
}

void maxregen(HANDLE proc, string offset)
{
	long address = (DWORD)str2dword(hexadd(offset, "B8305"));

	struct opcode * ops = new opcode[1];

	//add cx, 79
	ops[0].payload[0] = 0x66; 
	ops[0].payload[1] = 0x83;
	ops[0].payload[2] = 0xC1;
	ops[0].payload[3] = 0x79;
	ops[0].size = 4;

	patch(proc, ops, 1, (unsigned long *)address);
}

void revert_maxregen(HANDLE proc, string offset)
{
	long address = (DWORD)str2dword(hexadd(offset, "B8305"));

	struct opcode * ops = new opcode[1];

	//add cx, [eax+08]
	ops[0].payload[0] = 0x66; 
	ops[0].payload[1] = 0x03;
	ops[0].payload[2] = 0x48;
	ops[0].payload[3] = 0x08;
	ops[0].size = 4;

	patch(proc, ops, 1, (unsigned long *)address);
}

void reverse_maxhp(HANDLE proc, string offset)
{
	long address = (DWORD)str2dword(hexadd(offset, "44CAD"));

	struct opcode * ops = new opcode[1];

	//sub edx, 01
	ops[0].payload[0] = 0x66; 
	ops[0].payload[1] = 0x8B;
	ops[0].payload[2] = 0x45;
	ops[0].payload[3] = 0xF8;
	ops[0].size = 4;

	patch(proc, ops, 1, (unsigned long *)address);
}


void revert_xp(HANDLE proc, string offset)
{
	//0x03D0
	//0x8B45D8
	
	long address = (DWORD)str2dword(hexadd(offset, "C55E2"));

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

	hpstatic(proc, offset);

	string argumentval(argv[1]);
	if(argumentval.compare("-xp=on") == 0)
	{
		change_xp(proc, offset);
	}
	else if(argumentval.compare("-xp=off") == 0)
	{
		revert_xp(proc, offset);
	}
	else if(argumentval.compare("-points=on") == 0)
	{
		unlimited_points(proc, offset);
	}
	else if(argumentval.compare("-points=off") == 0)
	{
		revert_unlimited_points(proc, offset);
	}
	else if(argumentval.compare("-maxhp=on") == 0)
	{
		set_maxhp(proc, offset);
	}
	else if(argumentval.compare("-maxhp=off") == 0)
	{
		reverse_maxhp(proc, offset);
	}
	else if(argumentval.compare("-maxregen=on") == 0)
	{
		maxregen(proc, offset);
	}
	else if(argumentval.compare("-maxregen=off") == 0)
	{
		revert_maxregen(proc, offset);
	}
	else if(argumentval.compare("-godmode=on") == 0)
	{
		godmode(proc, offset);
	}
	else if(argumentval.compare("-godmode=off") == 0)
	{
		revert_godmode(proc, offset);
	}
	else if(argumentval.compare("-instakill=on") == 0)
	{
		instakill(proc, offset);
	}
	else if(argumentval.compare("-instakill=off") == 0)
	{
		revert_instakill(proc, offset);
	}
	else
	{
		cout<< "usage: " << argv[0] << " <option>" << endl;
		exit(0);
	}
	return 0;
}
