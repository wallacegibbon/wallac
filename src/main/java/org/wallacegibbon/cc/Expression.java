package org.wallacegibbon.cc;

import java.util.List;

public class Expression
{
}

class VarExpression extends Expression
{
  private String varName;
}

class FunctionCall extends Expression
{
  private String funName;
  private List<CVar> args;
}

class AssignExpression extends Expression
{
  private String varName;
  private Expression value;
}

class Op1Expression extends Expression
{
  private Op operator;
  private Expression operand;
}

class Op2Expression extends Expression
{
  private Op operator;
  private Expression operand1;
  private Expression operand2;
}

class Op3Expression extends Expression
{
  private Op operator;
  private Expression operand1;
  private Expression operand2;
  private Expression operand3;
}
