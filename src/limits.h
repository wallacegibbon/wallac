#ifndef __LIMIT_H__
#define __LIMIT_H__


#define MAX_CSTR_LENGTH 256
#define MAX_IDENT_LENGTH 64
#define MAX_INT_LENGTH 20
#define MAX_DECIMAL_STRING "2147483647"
#define MAX_OCTAL_STRING "37777777777"
#define MAX_HEX_STRING "ffffffff"

#define MAX_FILENAME_SIZE 60
#define IS_VALID_FILENAME(f) (strlen(f) < MAX_FILENAME_SIZE-3)

#define CHAR_MIN -127
#define CHAR_MAX +127
#define UCHAR_MAX 255

#define SHRT_MIN -32767
#define SHRT_MAX +32767
#define USHRT_MAX 65535

#define LONG_MIN -2147483647
#define LONG_MAX +2147483647
#define ULONG_MAX 4294967295

#define INT_MIN LONG_MIN
#define INT_MAX LONG_MAX
#define UINT_MAX ULONG_MAX

#define CHAR_BIT 8
#define SHORT_BIT 16
#define INT_BIT 32

#ifdef __LP64__
#define LONG_BIT 64
#else
#define LONG_BIT 32
#endif


#endif
