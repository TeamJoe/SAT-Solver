#include "Functions.h"

char ToUpper(char c)
{
	if(c >= 'a' && c <= 'z')
	{
		return c - 32;
	}
	else
	{
		return c;
	}
}

char ToLower(char c)
{
	if(c >= 'A' && c <= 'Z')
	{
		return c + 32;
	}
	else
	{
		return c;
	}
}