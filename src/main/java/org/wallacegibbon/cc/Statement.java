package org.wallacegibbon.cc;

public class Statement
{
}

class IfStatement extends Statement
{
  private Expression cond;
  private Statement thenBody;
  private Statement elseBody;

  public Expression getCond()
  {
    return cond;
  }

  public void setCond(Expression cond)
  {
    this.cond = cond;
  }

  public Statement getThenBody()
  {
    return thenBody;
  }

  public void setThenBody(Statement thenBody)
  {
    this.thenBody = thenBody;
  }

  public Statement getElseBody()
  {
    return elseBody;
  }

  public void setElseBody(Statement elseBody)
  {
    this.elseBody = elseBody;
  }
}

class SwitchStatement extends Statement
{
  private Expression cond;
  private Statement cases;

  public Expression getCond()
  {
    return cond;
  }

  public void setCond(Expression cond)
  {
    this.cond = cond;
  }

  public Statement getCases()
  {
    return cases;
  }

  public void setCases(Statement cases)
  {
    this.cases = cases;
  }
}

class WhileStatement extends Statement
{
  private Expression cond;
  private Statement body;

  public Expression getCond()
  {
    return cond;
  }

  public void setCond(Expression cond)
  {
    this.cond = cond;
  }

  public Statement getBody()
  {
    return body;
  }

  public void setBody(Statement body)
  {
    this.body = body;
  }
}

class DoWhileStatement extends Statement
{
  private Expression cond;
  private Statement body;

  public Expression getCond()
  {
    return cond;
  }

  public void setCond(Expression cond)
  {
    this.cond = cond;
  }

  public Statement getBody()
  {
    return body;
  }

  public void setBody(Statement body)
  {
    this.body = body;
  }
}

class ForStatement extends Statement
{
  private Statement init;
  private Statement cond;
  private Statement step;
  private Statement body;

  public Statement getInit()
  {
    return init;
  }

  public void setInit(Statement init)
  {
    this.init = init;
  }

  public Statement getCond()
  {
    return cond;
  }

  public void setCond(Statement cond)
  {
    this.cond = cond;
  }

  public Statement getStep()
  {
    return step;
  }

  public void setStep(Statement step)
  {
    this.step = step;
  }

  public Statement getBody()
  {
    return body;
  }

  public void setBody(Statement body)
  {
    this.body = body;
  }
}

class ExpressionStatement extends Statement
{
  private Expression expr;

  public Expression getExpr()
  {
    return expr;
  }

  public void setExpr(Expression expr)
  {
    this.expr = expr;
  }
}

class CompoundStatement extends Statement
{
}
