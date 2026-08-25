/*-- Geldsack --*/

#strict

local m_val;

public func Initialize()
{
	m_val = 5;
}

protected func RejectEntrance(object pContainer)
{
  if(GetPlayerType(GetOwner(pContainer))==C4PT_User)
  {
    Message(Format("{{MBAG}} %d", m_val), pContainer);
    DoWealth(GetOwner(pContainer), GetValue());
    Sound("Cash");
    return RemoveObject();
  }
}

// Shows the value
public func Activate(object pClonk)
{
  Message("{{MBAG}} %d", pClonk, m_val);
  return(1);
}
public func ControlDigDouble(object pClonk)
{ return(Activate(pClonk)); }

// Calculates the value
protected func CalcValue()
{
    return(m_val);
}
// Set the exact value
public func SetValue(int iVal)
{
    m_val = iVal;
}
// Add to the value
public func DoValue(int iVal)
{
    m_val += iVal;
}
