/*-- Template für begehbare Gebäude --*/

#strict 2

#include DOOR

static g_AccessibleCopy;

/* Objektlokale Variablen */

// Variablen für die äußere Hütte

local fLeave;
local pInside;

local fOpen;

// Variablen für die innere Hütte
local aInterior;
local aPlayer;
local iBitMask;
local aGraphics;

local fIsDouble;

local fIsInside;
local fEnter;
local pOutside;

local fNoAdd;

local pDouble;

// Variablen für den Grafik Double
local iLastX;
local iLastY;

local pCaveReference;

local pTopFaceDouble;

// Charakterisierung (vom Haus überladbar)

public func GetDoubleOffset(iIndex) { return [0, GetDefHeight(GetID())][iIndex]; }

public func IsAccssible() { return 10; }

public func GetFadeTime() { return 10; }

public func IsInside() { return fIsInside; }

func IsCave() { return 0; }
func UseOutside() { return 1; }

global func PlayAccessibleHuts()
{
	for(var pObj in FindObjects(Find_Func("IsAccssible")))
		if(!pObj->LocalN("pInside"))
			pObj->PlayReady();
}

func PlayReady()
{
	var iXOff = GetLayerOffset(), iYOff = GetLayerOffset(1);
	if(pInside) return pInside->PlayReady();
	if(pDouble) return;
	if(fIsDouble) return;
	if(!fIsInside) return;
	g_AccessibleCopy = 1;
  var i = 1;
  while(i < last_count)
  {
    SetGraphics(0, this, 0, i, 0);
    i++;
  }
	pDouble = CreateObject(GetID());
	if(!pOutside && UseOutside())
	{
		pOutside = CreateObject(GetID());
//		pOutside->SetPosition(GetX()+GetLayerOffset(), GetY()+GetLayerOffset(1));
		if(IsCave()) pOutside->SetCategory(C4D_Background);
		SetGraphics("Black", pOutside, GetID(), 3, 1);
//		SetGraphics(0, pOutside, _VIW, 3, 1);
//		SetObjDrawTransform(1000*GetDefWidth(GetID())/2, 0, 0, 0, 1000*GetDefHeight(GetID())/2,  0, pOutside, 3);
		SetClrModulation(RGBa(0,0,0,100), pOutside, 3);
//		pOutside->LocalN("pInside") = this;
		pOutside->SetObjectLayer(pOutside);
		pOutside->SetSolidMask();
	}
	else if(pOutside->GetCategory() & C4D_Background)
	{		
		if(GetActMapVal("Length", "TopFace") == 1)
			{
				pTopFaceDouble = CreateObject(GetID());
				pTopFaceDouble->SetAction("TopFace");
				pTopFaceDouble->SetPosition(pOutside->GetX(), pOutside->GetY());
				pTopFaceDouble->SetObjectLayer(this);
				pTopFaceDouble->SetSolidMask();
			}
	}
	g_AccessibleCopy = 0;
	
	pDouble->LocalN("fIsDouble") = 1;
//	pDouble->SetPosition(pOutside->GetX(), pOutside->GetY());
	pDouble->SetGraphics("None", 0, TAHT);
	aInterior = [];
	aPlayer = [];
	aGraphics = [ [], [], [], [], [], [] ];
	var iX = GetX(), iY = GetY();
	AddObject(this);
	SetPosition(iX, iY);
	AddObject(pDouble);
	pDouble->SetPosition(iX+iXOff, iY+iYOff);
	if(pOutside)
		pOutside->SetPosition(iX+iXOff, iY+iYOff);
	pDouble->SetVisibility();
	pDouble->SetCategory(C4D_StaticBack | 8388608);
	SetVisibility(VIS_Local);
	SetClrModulation(RGBa(255,255,255,255));
	ScheduleCall(this, "DoPlayReady", 5, 1);
}

func DoPlayReady()
{
	var x = GetDefOffset(GetID(), 0)+GetX();
	var y = GetDefOffset(GetID(), 1)+GetY();
	var dx = GetDefWidth(GetID());
	var dy = GetDefHeight(GetID());
	var offx = -GetLayerOffset(0);
	var offy = -GetLayerOffset(1);

	for(var pObj in FindObjects(Find_InRect(x-GetX(), y-GetY(), dx, dy), Find_Not(Find_ID(_SUL))))
	{
		if(pObj->~IsAccssible()) continue;
		var iX = pObj->GetX(), iY = pObj->GetY();
    if(pObj->GetObjectLayer()) pObj->SetObjectLayer();
		AddObject(pObj, 1, 1);
//		pObj->SetPosition(iX, iY);
	}
}

protected func Initialize()
{
  if(g_AccessibleCopy) return;
  fIsInside = 1;
  SetCategory(C4D_StaticBack);
  // Außenraum erzeugen
  if(!IsCave())
  {
	  fNoAdd = 1;
	  g_AccessibleCopy = 1;
	  pOutside = CreateObject(GetID(), 0,-GetDefOffset(GetID(),1));
	  pOutside->SetPosition(GetX()-GetDoubleOffset(0), GetY()-GetDoubleOffset(1));
	  pOutside->LocalN("pInside") = this;
	  g_AccessibleCopy = 0;
	  fNoAdd = 0;
	  SetGraphics("Inside");
	  if(!pOutside) Log("Error");
  }
  // Funktion zum plazieren von Innereien aufrufen
  ScheduleCall(this, "PlaceInterior", 10, 1);
  return _inherited();
}

public func PlaceInterior()
{
  // Für spezielle Häuser überladbar
}

public func GetMain()
{
  if(pInside) return pInside;
  return this;
}

protected func Collection2(pObj, fNoMove)
{
	if(GetProcedure(pObj) == "ATTACH" && GetOCF(pObj->GetActionTarget()) & OCF_CrewMember && GetPlayerType(GetOwner(pObj->GetActionTarget())) == C4PT_User)
		return;
  // Innenraum
  if(fIsInside)
  {
    // Kommt er ein?
    if(fEnter) return;
    if(GetOCF(pObj) & OCF_CrewMember && GetPlayerType(GetOwner(pObj)) == C4PT_User)
		{
			FadeSightModulation(1, 0, 2, GetOwner(pObj), 0, 1);
			Schedule(Format("Collection3(Object(%d), %d)", ObjectNumber(pObj), fNoMove), GetFadeTime(), 1);
			return;
		}
    // ... oder will er raus?
    pOutside->~DoLeave(pObj);
    return;
  }
  // Verlässt er gerade?
  if(fLeave) return;
  if( GetOCF(pObj) & OCF_CrewMember && GetPlayerType(GetOwner(pObj)) == C4PT_User)
  {
		FadeSightModulation(1, 0, 2, GetOwner(pObj), 0, 1);
		Schedule(Format("Collection3(Object(%d), %d)", ObjectNumber(pObj), fNoMove), GetFadeTime(), 1);
		return;
	}
  // Oder will er in den Innenraum?
  pInside->AddObject(pObj);
  pInside->~DoEnter(pObj, fNoMove);
}

protected func Collection3(pObj, fNoMove)
{
  // Innenraum
  if(fIsInside)
  {
    // Kommt er ein?
    if(fEnter) return;
    // ... oder will er raus?
    pOutside->~DoLeave(pObj);
    return;
  }
  // Verlässt er gerade?
  if(fLeave) return;
  // Oder will er in den Innenraum?
  pInside->AddObject(pObj);
	if(pInside)
  pInside->~DoEnter(pObj, fNoMove);
}

//---------------------------------------------------//
//    Automatisches Raus und Reingehen (Balkon)      //
//---------------------------------------------------//

public func AddObject(pObj, fNoSight, fNoMove)
{
  // Das regelt alles der Chef!
  if(pInside) return pInside->AddObject(pObj, fNoSight);
	if(GetID(pObj) == _MUS) return;
	if(fNoAdd) return;

 	for(var pTest in aInterior) if(pTest == pObj) return;
  // Schon in was anderem "drin"?
  if(GetObjectLayer(pObj) && GetObjectLayer(pObj) != this)
  {
    // Dann erst aus dem entfernen
    var pBuilding = GetObjectLayer(pObj);
    pBuilding->RemObject(pObj, 1);
    fNoSight = 1;
  }
  if(pObj != this)
	{
		var i=-1;
		while(Contents(++i, pObj)) AddObject(Contents(i, pObj));
		for(var pAttachObj in FindObjects(Find_ActionTarget(pObj))) if(GetProcedure(pAttachObj) == "ATTACH") AddObject(pAttachObj);
		if(pObj->~GetQuestStorage()) AddObject(pObj->~GetQuestStorage());
	}
	
  // Layer und Kategorie setzen
  SetObjectLayer(this, pObj);
//  SetCategory(8388608, pObj);pObj->Schedule("SetCategory(8388608)", 1, 1);
	if(pObj != this || pObj != pDouble)
;//		SetVisibility(VIS_OverlayOnly, pObj);
  // Platz im Array finden und dort einnisten
  var i=-1;
  while(aInterior[++i]);
  aInterior[i] = pObj;
	var j=-1;
	var i=-1;
	var aCategorys = [C4D_StaticBack, C4D_Structure, C4D_Vehicle, C4D_Living, C4D_Object];
	while(!(pObj->GetCategory() & aCategorys[++j]));
	if(pObj->GetCategory() & 8388608) j = 5; // Foreground
	while(aGraphics[j][++i]);
  aGraphics[j][i] = pObj;
	
  // Und an die "SolidMask" anpassen
	if(!(GetCategory(pObj) & C4D_Parallax))
		ShiftVertices(pObj, 1, fNoMove);
  // Sicht setzen
  if(GetOCF(pObj) & OCF_CrewMember && GetPlayerType(GetOwner(pObj)) == C4PT_User)
  {
    aPlayer[GetOwner(pObj)]++;
    if(!fNoSight)
		{
			SetPlrSightModulation(RGBa(1,1,1,128), 0, 1, GetOwner(pObj));
//			FadeSightModulation(1, 0, 1, GetOwner(pObj));
			FadeSightModulation(1, 0, 2, GetOwner(pObj), 1, 1);
		}
    CalcBitMask();
  }
}

func FxDoAirTimer(pTarget)
{
	var Max = pTarget->GetPhysical("Breath", 2)/1000;
	pTarget->DoBreath(1000);
}

public func RemObject(pObj, fNoSight)
{
  // Nur der liebe Main verwaltet alles
  if(pInside) return pInside->RemObject(pObj, fNoSight);
	if(GetID(pObj) == _MUS) return;

  // Sicherheit
  if(pObj == this) return;
	// Inventar
  var i=-1;
  while(Contents(++i, pObj)) RemObject(Contents(i, pObj));
	// Attachment wieder raus
	for(var pAttachObj in FindObjects(Find_ActionTarget(pObj))) if(GetProcedure(pAttachObj) == "ATTACH") RemObject(pAttachObj);
	if(pObj->~GetQuestStorage()) RemObject(pObj->~GetQuestStorage());
  // Layer und Kategorie zurücksetzen
  SetObjectLayer(0, pObj);
//  SetCategory(GetCategory(0, GetID(pObj)), pObj);
//	SetVisibility(VIS_All, pObj);
  // Aus der Liste löschen
  var o, i = 0;
  for(o in aInterior)
  {
    if(o == pObj)
    {
      aInterior[i] = 0;
      break;
    }
    i++;
  }
  var j=-1;
	var i=-1;
	var aCategorys = [C4D_StaticBack, C4D_Structure, C4D_Vehicle, C4D_Living, C4D_Object];
	while(!(pObj->GetCategory() & aCategorys[++j]));
	if(pObj->GetCategory() & 8388608) j = 5; // Foreground
	var o, i = 0;
  for(o in aGraphics[j])
  {
    if(o == pObj)
    {
      aGraphics[j][i] = 0;
      break;
    }
    i++;
  }
	
  // Rückanpassung von der SolidMask
  if(!(GetCategory(pObj) & C4D_Parallax))
		ShiftVertices(pObj, -1);
  if(GetEffect("IntUpdateInteriorTransform", pObj))
    RemoveEffect("IntUpdateInteriorTransform", pObj);
//  pObj->SetObjDrawTransform();
  // War er vorher in einem anderen Haus?
  if(pOutside && pOutside->GetObjectLayer() && !IsCave())
  {
    // Dann wieder da rein.
    var pBuilding = pOutside->GetObjectLayer();
    pBuilding->AddObject(pObj, 1);
    fNoSight = 1;
  }
  // Sicht setzen
  if(GetOCF(pObj) & OCF_CrewMember && GetPlayerType(GetOwner(pObj)) == C4PT_User)
  {
    aPlayer[GetOwner(pObj)]--;
    if(!fNoSight)
		{
			SetPlrSightModulation(0, 0, 1, GetOwner(pObj));
//			FadeSightModulation(1, 0, 1, GetOwner(pObj));
			FadeSightModulation(1, 0, 2, GetOwner(pObj), 1, 1);
		}
    CalcBitMask();
    if(!iBitMask) RemoveEffect("Inside", pInside);
  }
}

public func DoLeave(pObj, fNoMove)
{
  fLeave = 1;
  if(!fNoMove) Enter(this, pObj);
  fLeave = 0;
  RemObject(pObj);
  SetCommand(pObj, "Exit");
}

public func DoEnter(pObj, fNoMove)
{
  fEnter = 1;
  if(!fNoMove) Enter(this(), pObj);
  fEnter = 0;
  SetCommand(pObj, "Exit");
}

private func CalcBitMask()
{
  if(pInside) return pInside->CalcBitMask();
  iBitMask = 0;
  var iPlr;
  for(var fActive in aPlayer)
  {
    if(fActive) iBitMask += 2**iPlr;
    iPlr++;
  }
  Local(0) = iBitMask;
}

//---------------------------------------------------//
//    Automatisches Raus und Reingehen (Balkon)      //
//---------------------------------------------------//

// muss dafür als TimerCall vorhanden sein
protected func CheckEntrance()
{
  for(var i = 0; i < GetEntranceZoneCount(); i++) {
    var aEntrance = GetEntranceZone(fIsInside, i);
    for(var pObj in FindObjects(aEntrance))
    {
//      if(GetObjectLayer(pObj)==GetObjectLayer())
      {
        var iDir = GetEntranceDir(fIsInside, i);
        if(iDir)
        {
          if(iDir == COMD_Up && pObj->GetYDir() >= 0) continue;
          if(iDir == COMD_Down && pObj->GetYDir() <= 0) continue;
          if(iDir == COMD_Right && pObj->GetXDir() <= 0) continue;
          if(iDir == COMD_Left && pObj->GetXDir() >= 0) continue;
        }
        if(fIsInside) RemObject(pObj);
        else if(pInside) pInside->~AddObject(pObj);
      }
    }
  }
}

public func GetEntranceZone(fInside,iIndex)
{
  if(fInside) return Find_InRect(0,0,0,0); // Jeweils Offset und Größe
  return Find_InRect(0,0,0,0);
}

public func GetEntranceZoneCount() { return 0; }

public func GetEntranceDir(iInside,iIndex) { return 0; }

//---------------------------------------------------//
//    Hilfsfunktion Fixieren                         //
//---------------------------------------------------//

func Fix()
{
	AddEffect("IntFix", this, 1, 1, this);
}

func Loose()
{
	RemoveEffect("IntFix", this);
}

func FxIntFixStart(pTarget, iNumber, fTmp)
{
	if(fTmp) return;
	EffectVar(0, pTarget, iNumber) = GetX();
	EffectVar(1, pTarget, iNumber) = GetY();
}

func FxIntFixTimer(pTarget, iNumber)
{
	SetPosition(EffectVar(0, pTarget, iNumber), EffectVar(1, pTarget, iNumber));
}

//---------------------------------------------------//
//    Zerstörung                                     //
//---------------------------------------------------//
  
protected func Destruction()
{
  // Haus ausleeren
  if(aInterior)
  for(var pObj in aInterior)
    if(pObj) RemObject(pObj);
	// Fertig
  return(1);
}

protected func Incineration()
{
  // Haus ausleeren
  if(aInterior)
  for(var pObj in aInterior)
    if(pObj) RemObject(pObj);
}

//---------------------------------------------------//
//    Grafik Innenraum                               //
//---------------------------------------------------//

local last_count;
func FxIntUpdateTransformTimer(pTarget, iNumber)
{
  if(!pDouble) return;
	var iXOffset = GetLayerOffset();
	var iYOffset = GetLayerOffset(1);
//	SetObjDrawTransform(1000, 0, iXOffset*1000, 0, 1000, 1000*iYOffset, this);
	var o, i = 1;
/*	SetGraphics(0, pDouble, _VIW, i, 1);
	SetObjDrawTransform(1000*1024, 0, 0, 0, 1000*1024/2,  0, pDouble, i);
	SetClrModulation(RGBa(0,0,0,150), pDouble, i);
	i++;*/
	for(var j = 0; j < 6; j++)
	{
		for(o in aGraphics[j])
		{
			if(!o || o->Contained() || GetID(o) == _VIW) continue;
			if(o == pDouble) continue;
			if(o->GetVisibility() & VIS_OverlayOnly) continue;
			SetGraphics(0, pDouble, 0, i, GFXOV_MODE_Object, 0, 0, o);
			SetObjDrawTransform(1000, 0, (GetX(o)-GetX())*1000, 0, 1000, (GetY(o)-GetY())*1000, pDouble, i);
			if(o->GetClrModulation() && o != this)
				SetClrModulation(o->GetClrModulation(), pDouble, i);
			else
				SetClrModulation(RGB(255,255,255), pDouble, i);
			i++;
		}
		if(j == 4)
			if(GetActMapVal("Length", "TopFace") == 1)
			{
				SetGraphics("Inside", pDouble, GetID(), i, GFXOV_MODE_Action, "TopFace");
				SetObjDrawTransform(1000, 0, 0, 0, 1000, 0, pDouble, i);
				i++;
			}
	}
	while(i < last_count)
	{
		SetGraphics(0, pDouble, 0, i, 0);
		i++;
	}
	if(pOutside)
	{
		if(IsCave())
		{
			SetGraphics(0, pOutside, GetID(), 1, 1);
			SetGraphics(0, pOutside, 0, 2, GFXOV_MODE_Object, 0, 0, pDouble);
		}
		else
		{
			SetGraphics("Inside", pOutside, GetID(), 1, 1);
			SetGraphics(0, pOutside, 0, 2, GFXOV_MODE_Object, 0, 0, pDouble);
			SetGraphics("", pOutside, GetID(), 3, 1);
      if(GetClrModulation(pOutside))
        SetClrModulation(GetClrModulation(pOutside), pOutside, 3);
		}
	}
	last_count = i;
}

public func ShiftVertices(pObj,iDir,fNoMove)
{
  if(!pObj) if(pObj != this) return;
	if(!GetEffect("IntUpdateTransform", this))
      AddEffect("IntUpdateTransform", this, 1, 1, this);
	var iXOffset = GetLayerOffset();
	var iYOffset = GetLayerOffset(1);
	if(!fNoMove)
		SetPosition(GetX(pObj)-iXOffset*iDir, GetY(pObj)-iYOffset*iDir, pObj);
	if( (GetOCF(pObj) & OCF_CrewMember) && (GetPlayerType(pObj->GetOwner()) == C4PT_User ))
	{
		var pView = FindObject2(Find_ID(_VIW), Find_ActionTarget(pObj));
		if(!pView)
		{
			pView = CreateObject(_VIW, 0, 0, GetOwner(pObj));
			pView->SetAction("Attach", pObj);
    }
    pView->SetCategory(1);
    pView->SetPlrViewRange(50);
    pObj->SetPlrViewRange(0);
		pView->DoAttach();
	}
	return;
}

public func GetLayerOffset(fY)
{
	if(!pOutside)
	{
		if(pCaveReference && pCaveReference->LocalN("pLink"))
		{
			if(fY) return -pCaveReference->GetY()+pCaveReference->LocalN("pLink")->GetY();
			return -pCaveReference->GetX()+pCaveReference->LocalN("pLink")->GetX();
		}
		if(fY) return GetDefHeight(GetID());
		else return 0;
	}
  if(fY) return -GetY()+GetY(pOutside);
  return -GetX()+GetX(pOutside);
}

public func IsAccesible() { return 1; }