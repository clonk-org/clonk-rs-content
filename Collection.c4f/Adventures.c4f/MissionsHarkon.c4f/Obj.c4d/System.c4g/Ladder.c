/*-- Leiter --*/

#appendto CLNK
#strict 2

/* Leiterklettern */

local LastLadder, iScaleDelay;

public func IsScalingLadder() { return GetAction() == "ScaleLadder"; }

private func GrabLadder(object pLadder, fDown)
{
	var pTargetLadder = pLadder->~GetTargetLadder();
	if(pTargetLadder)
	{
		pLadder = pTargetLadder;
		var pObj = pLadder->~GetLowerTarget();
		if(GetObjectLayer() != pObj)
		{
			if(GetObjectLayer()) GetObjectLayer()->~RemObject(this);
			if(pObj) pObj->~AddObject(this);
		}
	}
  // Kletteraktion initiieren
  SetAction("ScaleLadder", pLadder);
  SetComDir(COMD_Up);
	if(fDown) SetComDir(COMD_Down);
	SetDir(pLadder->GetLadderDir());
  // Richtung anpassen
//  if (GetX() < GetX(pLadder)) SetDir(DIR_Right); else SetDir(DIR_Left);
  AdjustLadderOffset(pLadder);
  // Klettereffekt
  AddEffect("ScalingLadder", this, 1, 1, this);
}

private func ReleaseLadder(int iXDir)
{
  // Letzte Leiter speichern
  LastLadder = GetActionTarget();
	// Leitern zwischen Gebäuden
	if(LastLadder && LastLadder->~GetUpperTarget() != LastLadder->~GetLowerTarget())
	{
		var pObj;
		if(!FindLadder(-20)) // Oberes Ende
		{
			pObj = LastLadder->~GetUpperTarget();
		}
		else
			pObj = LastLadder->~GetLowerTarget();
		
		if(GetObjectLayer() != pObj)
		{
			if(GetObjectLayer()) GetObjectLayer()->RemObject(this);
			if(pObj) pObj->AddObject(this);
		}
	}
  // Klettereffekt beenden
  RemoveEffect("ScalingLadder", this);
  // Fallen
  SetAction("Walk");
  SetXDir(iXDir);
}

protected func FxScalingLadderTimer()
{
  var pLadder;
  // Clonk hat aus irgendeinem Grund seine Kletteraktion verloren
  if (!(GetAction() == "ScaleLadder"))
    // Klettern beenden
    return ReleaseLadder(0);
  // Keine Leiter mehr: klettern beenden
	var iOffset = -10;
	if(GetComDir() == COMD_Down) iOffset = 10;
  if (!(pLadder = FindLadder(iOffset, -5*(1-2*GetDir()))))
  {
    // Oberes Ende: versuchen, die Wand zu erreichen
    if (GetComDir() == COMD_Up)
      return ReleaseLadder(0);
    // Unteres Ende: einfach fallen lassen
    else
      return ReleaseLadder(0);
  }
  // Ggf. neue Leiter speichern
  SetActionTargets(pLadder);
  // Abstand zur Leiter
  AdjustLadderOffset(pLadder);
  // Klettergeschwindigkeit 
  var iSpeed = BoundBy(GetPhysical("Scale") / 10000, 0, 3), iStep = 0;
  // Reduzierte Bewegung
  iScaleDelay += iSpeed;
  if (iScaleDelay >= 3) { iScaleDelay -= 3; iStep = 1; }
  // Kletterbewegung
  var iPosY = GetY(), iLastY = GetY();
  var iPhase = GetPhase();
  if (GetComDir() == COMD_Up) { iPosY -= iStep; iPhase += iStep; }
  if (GetComDir() == COMD_Down) { iPosY += iStep; iPhase -= iStep; }
  if (iPhase < 0) iPhase = 15; if (iPhase > 15) iPhase = 0;
  SetPhase(iPhase);
  SetPosition(GetX(), iPosY);
  SetXDir(0); SetYDir(0);
  // Nicht in festes Material klettern
//  if (Stuck()) { SetPosition(GetX(), iLastY); SetComDir(COMD_Stop); }
}

private func AdjustLadderOffset(object pLadder)
{
  // Abstand nach Kletterrichtung
  var iLastX = GetX();
  if (GetDir() == DIR_Left)
    SetPosition(GetX(pLadder) + 5, GetY());
  else
    SetPosition(GetX(pLadder) - 5, GetY());  
  // Nicht in festes Material klettern
/*  if (Stuck()) 
  {
    SetPosition(iLastX, GetY());
    return 0;
  }*/
  // Ausrichtung erfolgreich
  return 1;
}

private func FindLadder(iOffset, iXOffset)
{
  return FindObject(LADR, iXOffset, iOffset);
}  

private func ControlLadder(string strControl, int comdir)
{
  // Nicht an einer Leiter
  if (!IsScalingLadder())
	{
		if(strControl == "ControlUp" && GetProcedure() == "WALK")
		{
			// Nach einer Leiter Ausschau halten
			var pLadder; 
			if (pLadder = FindLadder(-20))
				GrabLadder(pLadder);
		}
		else if(strControl == "ControlDown" && GetProcedure() == "WALK")
		{
			// Nach einer Leiter Ausschau halten
			var pLadder; 
			if (pLadder = FindLadder(20))
				GrabLadder(pLadder, 1);
		}
		return 0; 
	}

  if(strControl == "ControlUpdate")
  {
    if(ComDirLike(comdir, COMD_Left)) comdir = ComDirTransform(comdir, COMD_Right);
    if(ComDirLike(comdir, COMD_Right)) comdir = ComDirTransform(comdir, COMD_Left);
    SetComDir(comdir);
  }
  // Rauf
  if (strControl == "ControlUp" && !GetPlrCoreJumpAndRunControl(GetController()))
    SetComDir(COMD_Up);
  // Runter
  if (strControl == "ControlDown" && !GetPlrCoreJumpAndRunControl(GetController()))
    SetComDir(COMD_Down);
  // Rechts
  if (strControl == "ControlRight" && !Stuck())
  {
    // Richtung wechseln
    if (GetDir() != DIR_Right)
			ReleaseLadder(+10);
  }
  // Links
  if (strControl == "ControlLeft" && !Stuck())
  {
    // Richtung wechseln
    if (GetDir() != DIR_Left)
      ReleaseLadder(-10);
  }
  
  // Werfen
  if (strControl == "ControlThrow")
  {
    // Ablegen
    SetComDir(COMD_Stop);
    SetCommand(0, "Drop");
  }
  
  // Steuerung überladen
  return 1; 
}

/* Bedienung an Waffen weitergeben, die gerade benutzt werden */

public func ControlLeft()
{
  if (ControlLadder("ControlLeft")) return 1;
  return _inherited(...);
}
public func ControlRight()
{
  if (ControlLadder("ControlRight")) return 1;
  return _inherited(...);
}
public func ControlUp()
{
  if (ControlLadder("ControlUp")) return 1;
  return _inherited(...);
}
public func ControlUpdate(object pByObject, int comdir, bool dig, bool throw)
{
  if(ControlLadder("ControlUpdate", comdir, dig, throw)) return 1;
  return _inherited(pByObject, comdir, dig, throw, ...);
}
public func ControlDown()  
{ 
  // Leiterkontrolle
  if (ControlLadder("ControlDown")) return 1; 
  return _inherited(...);
}
