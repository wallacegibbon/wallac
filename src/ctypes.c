#include "ctypes.h"

char *ctype_tostr(int type)
{
	if (type == CT_UCHAR)
		return "unsigned char";
	if (type == CT_USHORT)
		return "unsigned short";
	if (type == CT_UINT)
		return "unsigned int";
	if (type == CT_ULONG)
		return "unsigned long";
	if (type == CT_CHAR)
		return "char";
	if (type == CT_SHORT)
		return "short";
	if (type == CT_INT)
		return "int";
	if (type == CT_LONG)
		return "long";
	if (type == CT_VOID)
		return "void";
	if (type == CT_STRUCT)
		return "struct";
	return "CT_UNKNOWN";
}
