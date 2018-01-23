#ifndef __CEXPRS_H__
#define __CEXPRS_H__


#define U_SELF_INC_A 0x01
#define U_SELF_INC_B 0x02
#define U_SELF_DEC_A 0x03
#define U_SELF_DEC_B 0x04
#define U_SIZEOF 0x05
#define U_ADDR_REF 0x06
#define U_ADDR_DEREF 0x07
#define U_ACTIVE 0x08
#define U_NEGATIVE 0x09
#define U_LOGIC_NOT 0x0a
#define U_BIT_NOT 0x0b


#define B_FCALL 0x21
#define B_GET_FIELD 0x22
#define B_PGET_FIELD 0x23
#define B_MUL 0x24
#define B_DIV 0x25
#define B_MOD 0x26
#define B_ADD 0x27
#define B_SUB 0x28
#define B_BIT_L_SHIFT 0x29
#define B_BIT_R_SHIFT 0x2a
#define B_GT 0x2b
#define B_GEQ 0x2c
#define B_LT 0x2d
#define B_LEQ 0x2e
#define B_EQ 0x2f
#define B_NEQ 0x30
#define B_BIT_AND 0x31
#define B_BIT_XOR 0x32
#define B_BIT_OR 0x33
#define B_LOGIC_AND 0x34
#define B_LOGIC_OR 0x35
#define B_COMMA 0x36

#define GVAR_REF 0x51
#define PARAM_REF 0x52
#define LVAR_REF 0x53
#define FIELD_REF 0x54
#define INT_LITERAL 0x55
#define STR_LITERAL 0x56

struct expr { int type; struct expr *op1, *op2; };


#endif

