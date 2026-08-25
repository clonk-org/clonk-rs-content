#strict

local pCase;

protected Initialize:
  pCase = CreateObject(LC0S,+0,+27,GetOwner(this()));
  SetObjectOrder(this(), pCase);
  SetAction("LiftCase",pCase);
  return(1);

protected CheckCase:
  if (Equal(GetCon(),100))
    if (Not(Equal(GetID(GetActionTarget()),LC0S)))
      And( SetAction("Idle"), DoCon(-1) );
  return(1);

protected func Incineration() { if(pCase) RemoveObject(pCase); }
protected func IncinerationEx() { if(pCase) RemoveObject(pCase); }
protected func Destruction() { if(pCase) RemoveObject(pCase); return(_inherited()); }