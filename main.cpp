#include "MemoryUtility\Modifier.h"
#include "MemoryUtility\HexOperations.h"
#include "MemoryUtility\PaySegment.h"
#include "MemoryUtility\Payload.h";
#include "MemoryUtility\PayManager.h"
#include "MemoryUtility\Logger.h"

#include <stdio.h>
#include <iomanip>
#include <sstream>
#include <math.h>

using namespace MemUtil;

PayManager * tormentManager = NULL;

//not working, 19 address??
void change_xp(HANDLE proc, string offset)
{
	DWORD address = HexOperations::str2dword(HexOperations::hexadd(offset, "19"));
	
	cout << "CHANGE XP Address: " << HexOperations::hexadd(offset, "19") << endl;

	PaySegment xpSegment;

	Payload xppay;
	xppay.BuildPayload("01C28B45D881C200300000"); //add edx, eax; mov eax, [ebp-28]; add edx, 00010000
	xppay.BuildPayload("E9BE550C00"); //jmp Torment.exe+C55E1
	xpSegment.AddPayload(address, xppay);
	xppay.ClearPayload();

	address = HexOperations::str2dword(HexOperations::hexadd(offset, "C55E2"));
	xppay.BuildPayload("E932AAF3FF"); //jmp Torment.exe+19
	xpSegment.AddPayload(address, xppay);
	tormentManager->AddSegment(xpSegment);
	tormentManager->Inject();
}


void hpstatic(HANDLE proc, string offset)
{
	DWORD address = HexOperations::str2dword(HexOperations::hexadd(offset, "333E3C"));
	PaySegment hpSegment;

	Payload hppay;
	hppay.BuildPayload("0FBF880C050000"); //movsx ecx, word ptr[eax+50c]
	hppay.BuildPayload("50050C05000083F914"); //push eax; add eax, 50c; cmp ecx, 14
	hppay.BuildPayload("7E078905809A9600"); //jle Torment.exe+333E55; //mov [Torment.exe+569A80], eax
	hppay.BuildPayload("58E9190D0100"); //pop eax ; //jmp Torment.exe+344B73
	hpSegment.AddPayload(address, hppay);

	hppay.ClearPayload();
	hppay.BuildPayload("E9C9F2FEFF9090"); //jmp Torment.exe+344B73
	
	address = (DWORD)HexOperations::str2dword(HexOperations::hexadd(offset, "344B6E"));
	hpSegment.AddPayload(address, hppay);
	
	tormentManager->AddSegment(hpSegment);
	tormentManager->Inject();
}

void godmode(HANDLE proc, string offset)
{
		DWORD address = HexOperations::str2dword(HexOperations::hexadd(offset, "26937C"));
		PaySegment godSegment;
		cout << "godmode Address: " << HexOperations::hexadd(offset, "26937C") << endl;

		Payload godpay;
		godpay.BuildPayload("518B0D809A9600"); //push ecx; mov ecx, [Torment.exe+569A80]
		godpay.BuildPayload("8B9538FCFFFF"); //mov edx, [ebp-3c8]
		godpay.BuildPayload("39CA597403"); //cmp edx, ecx; pop ecx ; //je Torment.exe+26939B
		godpay.BuildPayload("66890AE97CA8E4FF"); //mv [edx], cx; jmp Torment.exe+B3C12
		godSegment.AddPayload(address, godpay);
		
		address = (DWORD)HexOperations::str2dword(HexOperations::hexadd(offset, "B3C0D"));
		cout << "godmode Address: " << HexOperations::hexadd(offset, "B3C0D") << endl;
		Payload godpayTwo("E96A571B0090909090"); //jmp Torment.exe+344B73
		godSegment.AddPayload(address, godpayTwo);
		
		tormentManager->AddSegment(godSegment);
		tormentManager->Inject();
}


void revert_instakill(HANDLE proc, string offset)
{
		DWORD address = HexOperations::str2dword(HexOperations::hexadd(offset, "B3C0D"));
		PaySegment instaSegment;
		Payload instapay;
		instapay.BuildPayload("8B9538FCFFFF"); //noping sub edx, 01
		instaSegment.AddPayload(address, instapay);
		
		Payload instapayTwo;
		instapayTwo.BuildPayload("66890A"); //jmp Torment.exe+344B73
	
		address = HexOperations::str2dword(HexOperations::hexadd(offset, "B3C0D"));
		instaSegment.AddPayload(address, instapayTwo);

		tormentManager->AddSegment(instaSegment);
		tormentManager->Inject();
}

void instakill(HANDLE proc, string offset)
{
		DWORD address = HexOperations::str2dword(HexOperations::hexadd(offset, "26937C"));
		PaySegment instaSegment;
		Payload instapay;
		instapay.BuildPayload("51"); //push ecx
		instapay.BuildPayload("8B0D809A9600"); //mov ecx, [Torment.exe+569A80]
		instapay.BuildPayload("8B9538FCFFFF"); //mov edx, [ebp-3c8]
		instapay.BuildPayload("39CA"); //cmp edx, ecx
		instapay.BuildPayload("59"); //pop ecx
		instapay.BuildPayload("7406"); //je Torment.exe+269394
		instapay.BuildPayload("C70200000000"); //mv [edx], 000
		instapay.BuildPayload("E97CA8E4FF90"); //jmp Torment.exe+B3C12
		instaSegment.AddPayload(address, instapay);
		
		Payload instapayTwo;
		instapayTwo.BuildPayload("E96A571B0090909090"); //jmp Torment.exe+344B73
	
		address = HexOperations::str2dword(HexOperations::hexadd(offset, "B3C0D"));
		instaSegment.AddPayload(address, instapayTwo);
		tormentManager->AddSegment(instaSegment);
		tormentManager->Inject();
}


void revert_godmode(HANDLE proc, string offset)
{
	revert_instakill(proc, offset);
}


void unlimited_points(HANDLE proc, string offset)
{
	DWORD address = HexOperations::str2dword(HexOperations::hexadd(offset, "215BDE"));
	PaySegment unlimitedSegment;
	Payload unlimitedpay;
	unlimitedpay.BuildPayload("909090"); //noping sub edx, 01
	unlimitedSegment.AddPayload(address, unlimitedpay);
	tormentManager->AddSegment(unlimitedSegment);
	tormentManager->Inject();
}

void revert_unlimited_points(HANDLE proc, string offset)
{
	DWORD address = HexOperations::str2dword(HexOperations::hexadd(offset, "215BDE"));
	PaySegment unlimitedSegment;
	Payload unlimitedpay;
	unlimitedpay.BuildPayload("83EA01"); //sub edx, 01
	unlimitedSegment.AddPayload(address, unlimitedpay);
	tormentManager->AddSegment(unlimitedSegment);
	tormentManager->Inject();
}

void set_maxhp(HANDLE proc, string offset)
{
	DWORD address = HexOperations::str2dword(HexOperations::hexadd(offset, "44CAD"));
	PaySegment maxHpSegment;
	Payload maxhppay;
	maxhppay.BuildPayload("66B89979"); //sub edx, 01
	maxHpSegment.AddPayload(address, maxhppay);
	tormentManager->AddSegment(maxHpSegment);
	tormentManager->Inject();
}

void maxregen(HANDLE proc, string offset)
{
	DWORD address = HexOperations::str2dword(HexOperations::hexadd(offset, "B8305"));
	PaySegment maxRegenSegment;
	Payload maxregenpay;
	maxregenpay.BuildPayload("6683C179"); //add cx, 79
	maxRegenSegment.AddPayload(address, maxregenpay);
	tormentManager->AddSegment(maxRegenSegment);
	tormentManager->Inject();
}

void revert_maxregen(HANDLE proc, string offset)
{
	DWORD address = HexOperations::str2dword(HexOperations::hexadd(offset, "B8305"));
	PaySegment maxRegenSegment;
	Payload maxregenpay;
	maxregenpay.BuildPayload("66034808"); //add cx, [eax+08]
	maxRegenSegment.AddPayload(address, maxregenpay);
	tormentManager->AddSegment(maxRegenSegment);	
	tormentManager->Inject();
}

void reverse_maxhp(HANDLE proc, string offset)
{
	DWORD address = HexOperations::str2dword(HexOperations::hexadd(offset, "44CAD"));
	PaySegment maxHpSegment;
	Payload maxhppay;
	maxhppay.BuildPayload("668B45F8"); //sub edx, 01
	maxHpSegment.AddPayload(address, maxhppay);
	tormentManager->AddSegment(maxHpSegment);
	tormentManager->Inject();
}

void revert_xp(HANDLE proc, string offset)
{
	DWORD address = HexOperations::str2dword(HexOperations::hexadd(offset, "C55E2"));
	PaySegment xpSegment;
	Payload xpppay;
	xpppay.BuildPayload("03D08B45D8");
	xpSegment.AddPayload(address, xpppay);
	tormentManager->AddSegment(xpSegment);
	tormentManager->Inject();
}

using namespace std;
using namespace MemUtil;

void process_args(string argumentval, HANDLE proc, string offset)
{
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
		cout<< "usage: " << "TormentTrainer.exe" << " <option>" << endl;
		exit(0);
	}
}

int main ( int argc, char *argv[] )
{
	Logger::logging = true;
	if ( argc != 2 ) 
	{
		cout<< "usage: " << argv[0] << " <option>" << endl;
		exit(0);
	}
    
	std::string processName("Torment.exe");
	
	DWORD processID = MemoryUtility::FindProcessId(processName);

	if ( processID == 0 )
		std::wcout << "Could not find " << processName.c_str() << std::endl;
	else
		std::wcout << "Process ID is " << processID << std::endl;
	
	stringstream ss;
	string offset;
	ss << hex << MemoryUtility::GetModuleBase(processName.c_str(), processID);
	ss >> hex >> offset;

	HANDLE proc = MemoryUtility::AttachToProcess(processID, false);
	if(proc == NULL)
	{
		cout << "Could not get handle to process" << endl;
		system("PAUSE");
	}

	PayManager manager(proc);
	tormentManager = &manager;

	//hpstatic(proc, offset);
	string argumentval(argv[1]);
	process_args(argumentval, proc, offset);
	return 0;
}
