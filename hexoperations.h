#ifndef HEXOPERATIONS
#define HEXOPERATIONS


#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
using namespace std;


string hexadd(const string & hex1, const string & hex2);
string hexdecadd(const string & hex1, const string & dec1);
string hexdecsubstract(const string & hex1, const string & dec1);
string hexsubstract(const string & hex1, const string & hex2);
string dec2hex(long i);
long hex2dec(const string & hexstr);
long str2dec(string dec);
DWORD str2dword(string address);
void endian_swap_long(unsigned long & nLongNumber);
void endian_swap(unsigned __int64& x);
#endif