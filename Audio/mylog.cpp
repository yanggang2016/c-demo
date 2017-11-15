
#include "stdafx.h"
#include "mylog.h"
#include <string>

void  log(const char * str)
{
	printf("%s\nLength:%d\n", str, strlen(str));

}

void  log(const char* str, const int len) {
	//TODO
	log(str);
}


void print_hex(const char*p, int len)
{
	fflush(stdin);
	printf("\n");
	for (int i = 0; i < len; i++)
	{
		printf("%02x ", (unsigned char)p[i]);
	}
	printf("\n");
	fflush(stdin);
}
