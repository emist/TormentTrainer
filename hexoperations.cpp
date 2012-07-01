//Credits to:
//http://www.daniweb.com/software-development/cpp/code/217330/hex-addition


#include "hexoperations.h"

DWORD str2dword(string address)
{
	stringstream ss;
	long addr;
	ss << hex << address;
	ss >> addr;
	return addr;
}

//credits http://www.codeproject.com/Articles/4804/Basic-concepts-on-Endianness
void endian_swap_long(unsigned long & nLongNumber)
{
   nLongNumber = (((nLongNumber&0x000000FF)<<24)+((nLongNumber&0x0000FF00)<<8)+
   ((nLongNumber&0x00FF0000)>>8)+((nLongNumber&0xFF000000)>>24));
}

void endian_swap(unsigned long long& x)
{
    x = (x>>56) | 
        ((x<<40) & 0x00FF000000000000) |
        ((x<<24) & 0x0000FF0000000000) |
        ((x<<8)  & 0x000000FF00000000) |
        ((x>>8)  & 0x00000000FF000000) |
        ((x>>24) & 0x0000000000FF0000) |
        ((x>>40) & 0x000000000000FF00) |
        (x<<56);
}

string hexdecadd(const string & hex1, const string & dec1)
{
	long n1, n2;
	n1 = hex2dec(hex1);
	n2 = str2dec(dec1);
	return dec2hex(n1+n2);
}

string hexdecsubstract(const string & hex1, const string & dec1)
{
	long n1, n2;
	n1 = hex2dec(hex1);
	n2 = str2dec(dec1);
	return dec2hex(n1-n2);
}



string hexadd(const string & hex1, const string & hex2)
{
	long n1, n2;
	n1 = hex2dec(hex1);
	n2 = hex2dec(hex2);
	return dec2hex(n1+n2);
}

string hexsubstract(const string & hex1, const string & hex2)
{
	long n1, n2;
	n1 = hex2dec(hex1);
	n2 = hex2dec(hex2);
	return dec2hex(n1-n2);
}

long str2dec(string dec)
{
	stringstream ss;
	long s;
	ss << dec;
	ss >> s;
	return s;
}

string dec2hex(long i)
{
	stringstream ss;
	string s;
	hex(ss);
	uppercase(ss);
	ss << i;
	ss >> s;
	return s;
}

long hex2dec(const string & hexstr)
{
	stringstream ss;
	long i = 0;
	hex(ss);
	ss << hexstr;
	ss.clear();
	ss >> i;
	return i;
}