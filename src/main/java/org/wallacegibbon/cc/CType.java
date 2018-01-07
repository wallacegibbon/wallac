package org.wallacegibbon.cc;

import java.util.List;

public class CType
{
  private boolean isVolatile = false;
  private boolean isExtern = false;
  private boolean isAuto = false;
  private boolean isConst = false;
  private boolean isRegister = false;

  public boolean isVolatile()
  {
    return isVolatile;
  }

  public void setVolatile(boolean aVolatile)
  {
    isVolatile = aVolatile;
  }

  public boolean isExtern()
  {
    return isExtern;
  }

  public void setExtern(boolean extern)
  {
    isExtern = extern;
  }

  public boolean isAuto()
  {
    return isAuto;
  }

  public void setAuto(boolean auto)
  {
    isAuto = auto;
  }

  public boolean isConst()
  {
    return isConst;
  }

  public void setConst(boolean aConst)
  {
    isConst = aConst;
  }

  public boolean isRegister()
  {
    return isRegister;
  }

  public void setRegister(boolean register)
  {
    isRegister = register;
  }
}

class CIntType extends CType
{
  private boolean isUnsigned = false;

  public boolean isUnsigned()
  {
    return isUnsigned;
  }

  public void setUnsigned(boolean unsigned)
  {
    isUnsigned = unsigned;
  }
}

class CChar extends CIntType
{
}

class CShort extends CIntType
{
}

class CInt extends CIntType
{
}

class CStruct extends CType
{
  private String name;
}