/*-- Meuchler --*/

#appendto ASAS
#strict 2

func FxHideStop(pTarget, iNumber, fTmp)
{
  // Alpha auf alten Wert setzen
  var iAlpha = GetRGBaValue(GetClrModulation(), 0);
  SetClrModulation(DoRGBaValue(GetClrModulation(), EffectVar(0, pTarget, iNumber)-iAlpha, 0));
  // Walk zurücksetzten
  ResetPhysical(pTarget, "Walk");
  ResetPhysical(pTarget, "Scale");
  ResetPhysical(pTarget, "Hangle");
  ResetPhysical(pTarget, "Energy");
  SetPhysical("Energy", GetPhysical("Energy", 1, pTarget)/2, 2, pTarget);
  // Hilfsobjekt entfernen
	if(EffectVar(1, pTarget, iNumber))
		EffectVar(1, pTarget, iNumber)->RemoveObject();
  // Kategorie zurücksetzten
  SetCategory(EffectVar(4, pTarget, iNumber));
  fHide = 0;
}

// Den Clonk tarnen
public func Hide(fNoEnemyCheck, fInstantHide, fNoMessage)
{
  // Nie nach Gegnern checken
  return _inherited(1, fInstantHide, fNoMessage);
}

// Callback zum Kurzzeitigen Sichtbarmachen
public func DoShow(iAmount)
{
  return _inherited(iAmount);
}

public func Never() { return false; }

public func ContextUnbuckle()
{
  [Condition=Never]
}

public func ContextOrder(pClonk)
{
  [Condition=Never]
}
/*
public func ContextThrowCoin()
{
  [Muenze werfen|Image=_CIN]
  if(GetAction() == "Walk")
	{
		CreateContents(_CIN);
		AddCommand(this, "Throw");
		DoWealth(-1);
		Sound("Uncash");
	}
}*/

public func Chopping()
{
	DoShow(200);
	return _inherited(...);
}

public func Digging()
{
	DoShow(200);
	return _inherited(...);
}

protected func ControlCommand(szCommand)
{
  // Kommando MoveTo an Pferd weiterleiten
  if (szCommand == "Throw")
    if (Contents() && GetAction() != "Push")
      DoShow(200);
	return _inherited(szCommand, ...);
}

protected func ControlThrow()
{
	if(_inherited()) return 1;
//	if(!GetPlrDownDouble(GetOwner()) && Contents(0) && GetAction() != "Push")
  if(Contents(0) && GetAction() != "Push")
			DoShow(200);
  // Keine überladene Steuerung
  return 0;
}

func GetHideValue()
{
	var i;
  if(i = GetEffect("Hide", this))
  {
    return EffectVar(3, this, i);
  }
}

func SetHideValue(int iValue)
{
	var i;
	if(!GetEffect("Hide", this)) Hide(1,1,1);
  if(i = GetEffect("Hide", this))
  {
    EffectVar(3, this, i) = BoundBy(iValue, 0, 235);
    SetClrModulation(RGBa(255,255,255,EffectVar(3, this, i)));
  }
}