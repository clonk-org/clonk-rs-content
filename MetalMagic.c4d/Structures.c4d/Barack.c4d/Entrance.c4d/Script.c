/*-- Niedergang --*/

#strict 2

local target, add_exit;

public func ActivateEntrance(pObj)
{
  if (!Hostile(GetOwner(), GetOwner(pObj)) && GetAction() != "Close")
  {
   SetEntrance(1);
  }
}

protected func AttachTargetLost()
{
 RemoveObject();
}

public func Activate(new_target, new_add_exit)
{
  target = new_target;
  add_exit = new_add_exit;
  SetAction("Open",target);
}

protected func Collection2(pObj)
{
  if (!target) return 0;

  Enter(target, pObj);
  if (add_exit) AddCommand(pObj, "Exit");
  SetEntrance(0);
}

protected func Check()
{
 SetOwner(GetOwner(GetActionTarget()));
 
 if(OnFire(GetActionTarget()) || GetID(GetActionTarget()) != BRCK)RemoveObject(0,1);
}

public func Switch()
{
 var phase=GetPhase();
 
 if(GetAction() == "Close")
        SetAction("Open",target);
 else   SetAction("Close",target);
 
 SetPhase(10-phase);
}










