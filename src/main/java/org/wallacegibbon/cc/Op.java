package org.wallacegibbon.cc;

public enum Op
{
  SADD,
  SMINUS,
  SMUL,
  SDIV,
  SMOD,

  UADD,
  UMINUS,
  UMUL,
  UDIV,
  UMOD,

  FADD,
  FMINUS,
  FMUL,
  FDIV,

  REF,
  UNREF,
  GETFIELD,
  GETPFIELD,
  GETINDEX,
  FUNCALL,
}
