/* Schloss */

#strict 2

local iRefID;
local pTarget;
local iTargetDir;

protected func Initialize()
{
  SetAction("None");	
}

/* Steuerung */

public func ControlDig(pClonk)
{
  [$MsgTurnKey$]
  // Steckt ein Schlüssel?
  if (!Contents()) { ControlThrow(pClonk); if(!Contents()) { Message("$MsgNoKey$",this); Sound("Error"); return 1; } }
  // Passt der Schlüssel?
  if (LocalN("iRefID", Contents()) != iRefID) { Message("$MsgWrongKey$",this); Sound("Error"); return 1; }
  if(!pTarget) return Message("$MsgStuck$",this);
  // Schlüssel drehen
  SetDir(1-GetDir());
  // Angeschlossenes Objekt aktivieren
  if(iTargetDir == 0)
    pTarget->~Activate(this);
  if(iTargetDir == 1)
  {
    if(GetDir()) pTarget->~ControlLeft(this);
    else pTarget->~ControlRight(this);
  }
  if(iTargetDir == 2)
  {
    if(!GetDir()) pTarget->~ControlLeft(this);
    else pTarget->~ControlRight(this);
  }
  return 1;
}

public func IsSwitch() { return true; }
  
public func ControlThrow(pClonk)
{
  [Schlüssel reinstecken/rausnehmen]
  if(Contents())
  {
    Exit(Contents());
    SetDir(0);
    SetAction("None");
    return 1;
  }
  // Kann der Inhalt des Clonks angenommen werden?
  var pQuest = pClonk->~GetQuestStorage();
	var find = Find_Container(pClonk);
	if(pQuest) find = Find_Container(pQuest);
	for(var pObj in FindObjects(find, Find_Func("IsKey")))
			if(LocalN("iRefID", pObj) == iRefID)
			{
				if(!RejectCollect(GetID(pObj), pObj)) Enter(this, pObj);
				if(!GetClrModulation(pObj)) SetColorDw(RGB(255,255,255));
				else SetColorDw(GetClrModulation(pObj));
				return 1;
			}
  return 1;
}

/* Collection */

protected func RejectCollect(idObj, pObj)
{
  // Nur Schlüssel aufnehmen
  if (!pObj->~IsKey())  { Message("$MsgNoKey$",this); Sound("Error"); return 1; }
  // Nur, wenn sie einigermaßen passen
  if (LocalN("iRefID", pObj) != iRefID)  { Message("$MsgWrongKey$",this); Sound("Error"); return 1; }
  // Schlüssel passt
  SetAction("Key");
  return 0;
}

public func IsSwitch() { return true; }
