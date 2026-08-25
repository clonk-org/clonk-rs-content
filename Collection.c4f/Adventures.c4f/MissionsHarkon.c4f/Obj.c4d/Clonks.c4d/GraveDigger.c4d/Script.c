/*-- Gravedigger --*/

#strict 2

#include CLNK

local fGhost;

// Der totengräber will keinen krimskrams
func RejectCollect() { return 1; }

protected func FxLiveOnStop(object pTarget, int iEffectNumber, int iReason, bool fTmp)
  {
  if(fTmp) return;
  if(iReason!=4) return;
  // Leiche erzeugen
	var pCorps = CreateObject(GetID(), 0, 10, GetOwner());
	pCorps->SetName(GetName());
	pCorps->Kill();
//	pCorps->SetPhase(6);
	SetAlive(1);
	DoEnergy(100, this);
	SetObjectBlitMode(1);
	SetClrModulation(RGB(128,128,255));
	LocalN("fGhost") = 1;
	AddEffect("NoDamage", this, 1, 0);
	if (GetEffect("AI", this)) return;
  return AddEffect("AI", this, 1, 3, this);;
  }

global func FxNoDamageDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
	if(iDmgEngy > 0) return iDmgEngy;
	return 0;
}

global func FxNoDamageStop(object pTarget, int iEffectNumber, int iReason, bool fTmp)
  {
  if(fTmp) return;
  if(iReason!=4) return;
	SetAlive(1);
	DoEnergy(100, this);
	return -1;
  }

func Burried()
{
	var pClonk = GetActionTarget();
	SetPosition(GetX(pClonk), GetY(pClonk)+2, pClonk);
	if(GetActTime() >= 16*2 * 5)
	{
		GetActionTarget(1)->SetOccupied(pClonk);
		SetAction("Walk");
	}
}

public func CarryCorps(obj)
{
  if(obj)
	{
		SetAction("KneelDown");
		Enter(this, obj);
		AddEffect("CarryControlCorps", this, 1, 1, this, 0, obj);
	}
}

public func PutDownCorps()
{
  SetAction("CarryPutDown");
  EffectVar(2, this, GetEffect("CarryControlCorps", this))->SetAction("BeingPutDown", this);
}

func CarryPutDown()
{
	RemoveEffect("CarryControlCorps", this);
}

func FindCorps()
{
	if(GetEffect("CarryControlCorps", this)) return 0;
	return FindObject2(Find_Func("IsClonk"), Find_NoContainer(), Find_Action("Dead"), Find_AtPoint());
}

func IsCarryingCorps()
{
	if(GetEffect("CarryControlCorps", this)) return 1;
}

func GetCorps(fRemove)
{
	var iEff = GetEffect("CarryControlCorps", this);
	var obj = EffectVar(0, this, iEff);
	if(fRemove)
		RemoveEffect(0, this, iEff);
	return obj;
}

// Effektvariablen
// 0: Gespeicherter Alphawert
// 1: Hilfspobejekt zum anzeigen der Partikel
// 2: Winkel für die Partikel
// 3: aktueller Alphawert
// 4: Gespeicherte Kategorie
func FxCarryControlCorpsStart(pTarget, iNumber, fTmp, pCarry, pCarryHelper)
{
  // Walk Physical verringern
//  SetPhysical("Walk",GetPhysical("Walk", 0,pTarget)-20000,PHYS_StackTemporary,pTarget);
//	SetPhysical("Jump", GetPhysical("Jump") * 3 / 4, PHYS_StackTemporary); // Sprungkraft temporär vermindern
//  SetPhysical("CanScale", 0, PHYS_StackTemporary);
//  SetPhysical("CanHangle", 0, PHYS_StackTemporary);
	EffectVar(0, pTarget, iNumber) = pCarry;
	EffectVar(1, pTarget, iNumber) = pCarryHelper;
	if(!pCarryHelper)
	{
		EffectVar(1, pTarget, iNumber) = pCarry->CreateObject(_CYH, 0, 10, -1);
		EffectVar(0, pTarget, iNumber)->Enter(EffectVar(1, pTarget, iNumber));

		if(ObjectSetAction(EffectVar(0, pTarget, iNumber), "BeingTakenUp", 0, 0, 1))
		{
			EffectVar(2, pTarget, iNumber) = EffectVar(0, pTarget, iNumber);
		}
		else
		{
			EffectVar(2, pTarget, iNumber) = EffectVar(1, pTarget, iNumber);
			
			EffectVar(1, pTarget, iNumber)->SetAction("BeingTakenUp", this);
			EffectVar(1, pTarget, iNumber)->SetColorDw(pCarry->GetColorDw());
			if(!EffectVar(1, pTarget, iNumber)->SetGraphics(pCarry->GetName()))
				EffectVar(1, pTarget, iNumber)->SetGraphics(Format("%i", pCarry->GetID()));
		}
		EffectVar(1, pTarget, iNumber)->SetVisibility(VIS_OverlayOnly);
		SetAction("CarryTakeUp");
	}
	
	SetGraphics(0, this, 0, 11, GFXOV_MODE_Object, 0, 0, EffectVar(2, pTarget, iNumber));
	if(!fGhost) SetGraphics(0, this,GetID(), 20, GFXOV_MODE_ExtraGraphics);
}

func FxCarryControlCorpsTimer(pTarget, iNumber)
{
	if(GetAction() == "Walk") SetAction("WalkCarry");
	if(GetAction() == "Jump") SetAction("JumpCarry");
	if(GetAction() == "Dive") SetAction("JumpCarry");
	if(GetAction() == "WalkCarry")
	{
		EffectVar(2, pTarget, iNumber)->SetAction("GettingCarried", this);
		EffectVar(2, pTarget, iNumber)->SetPhase(GetPhase());
	}
	if(GetAction() == "JumpCarry")
	{
		EffectVar(2, pTarget, iNumber)->SetAction("GettingCarried", this);
		EffectVar(2, pTarget, iNumber)->SetPhase(0);
	}
	EffectVar(2, pTarget, iNumber)->SetDir(GetDir());
	EffectVar(1, pTarget, iNumber)->SetPosition(GetX(), GetY());
  // Erlaubte Aktionen
  if(GetAction() != "WalkCarry" && GetAction() != "JumpCarry" && GetAction() != "KneelUp" && GetAction() != "KneelDown" && GetAction() != "CarryTakeUp" && GetAction() != "CarryPutDown")
  {
		if(GetAction() == "Push")
		{
			var pPush = GetActionTarget();
			if(GetID(pPush) == CATA)
				pPush->~LoadCorps(EffectVar(0, pTarget, iNumber));
		}
    //return -1;
  }
}

func FxCarryControlCorpsStop(pTarget, iNumber, fTmp)
{
  // Walk zurücksetzten
//  ResetPhysical(pTarget, "Walk");
//  ResetPhysical(pTarget, "Jump");
//  ResetPhysical(pTarget, "CanScale");
//	ResetPhysical(pTarget, "CanHangle");
	EffectVar(0, pTarget, iNumber)->SetDir(GetDir());
	EffectVar(2, pTarget, iNumber)->SetAction("Dead");
	EffectVar(2, pTarget, iNumber)->SetPhase(5);
	if(Contained(EffectVar(0, pTarget, iNumber)) == EffectVar(1, pTarget, iNumber))
	{
		if(Contained()) EffectVar(0, pTarget, iNumber)->Enter(Contained());
		else Exit(EffectVar(0, pTarget, iNumber), 0, 10);
	}
	EffectVar(1, pTarget, iNumber)->RemoveObject();
	if(GetAction() == "WalkCarry") SetAction("Walk");
	if(GetAction() == "JumpCarry") SetAction("Jump");

	SetGraphics(0, this, 0, 11, 0);
	SetGraphics(0, this, 0, 20, 0);
}

