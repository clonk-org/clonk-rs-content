/*-- Ballon --*/

#strict 2

local pLoad;

func Initialize()
	{
	SetAction("Fly"); // Standart
	}
	
public func Poff()
	{
	Sound("Poff");
  CastParticles("FSpark", 30, 28, 0, 0, 70, 80, RGB(255,255,255),RGB(255,255,255));
  return RemoveObject();
	}

public func Lift(object pClonk)
	{
	pLoad = pClonk;
	Sound("Inflate");
	SetAction("Inflate", pClonk);
	SetActionData(256*3+1);
	return 1;
	}

public func FreeBallon()
{
	if (GetAction() == "Lift")
		{
		SetAction("Fly");
		RemoveEffect("IntLift", pLoad); // pLoad muss ja gesetzt sein
		// bis jetzt hatte ja nur der Clonk Geschwindigkeit->übernehmen
		SetYDir(GetYDir(pLoad));
		// löschen, damit wir nicht mit Find_ActionTarget gefunden werden
		SetActionTargets(0);
		}
	pLoad = 0;
	return 1;
}

// EndCall von Inflate
protected func LiftOff()
	{
	if (pLoad)
		{
		AddEffect("IntLift", pLoad, 1, 1, this);
		AddEffect("IntControlJump", pLoad, 1,, this);
		}
	else
		{
		SetAction("Fly");
		SetActionTargets(0); // löschen
		}
	SetComDir(COMD_Up);
	AddEffect("IntStuckCheck", this, 1, 10, this);
	}


/* IntStuckCheck */

public func FxIntStuckCheckTimer(object pTarget, int iNr, int iTime)
	{
	if (Stuck()) Poff();
	if (iTime > 500) Poff();
	}

/* IntLift */

public func FxIntLiftTimer(object pTarget, int iNr, int iTime)
	{
	// Abheben nur wenn er nicht noch fest auf dem Boden steht
	if (GetAction(pTarget)!= "Walk" && GetAction(pTarget)!="Jump") pTarget->SetAction("Jump");
	if (GetAction(pTarget)=="Jump")
		{
		// Herumwedeln
		if (pTarget->GetPhase() > 3) pTarget->SetPhase(0);
		// beschleunigen nach oben, seitliche Geschw. abbremsen
		pTarget->SetYDir(BoundBy(GetYDir(pTarget)-5, -20, GetYDir(pTarget)));
		if (iTime%4) pTarget->SetXDir(((GetXDir(pTarget)>0)*2-1)*(Abs(GetXDir(pTarget))-1));
		}
	// Ein bisschen schneller werden
	//pTarget->SetYDir(BoundBy(-iTime/2, -20, -5), pTarget);
	}

/* IntControlJump */

protected func FxIntControlJumpControlLeft(object pTarget, int iNr)
	{
	// Richtungswechsel in der Luft erlaubt
	if (GetDir(pTarget) != DIR_Left)
		pTarget->SetDir(DIR_Left);
	else pTarget->SetXDir(GetXDir(pTarget)-30);
	}

protected func FxIntControlJumpControlRight(object pTarget, int iNr)
	{
	if (GetDir(pTarget) != DIR_Right)
		pTarget->SetDir(DIR_Right);
	else pTarget->SetXDir(GetXDir(pTarget)+30);
	}

protected func FxIntControlJumpControlLeftDouble(object pTarget, int iNr)
	{
	FreeBallon();
	// nach links Springen
	pTarget->SetAction("Jump");
	pTarget->SetXDir(-30);
	return RemoveEffect(, pTarget, iNr); // Fertig
	}

protected func FxIntControlJumpControlRightDouble(object pTarget, int iNr)
	{
	FreeBallon();
	// nach rechts Springen
	pTarget->SetAction("Jump");
	pTarget->SetXDir(+30);
	return RemoveEffect(, pTarget, iNr); // Fertig
	}
	
protected func FxIntControlJumpControlDig(object pTarget, int iNr)
	{
	FreeBallon();
	// Abspringen
	pTarget->SetAction("Jump");
	//pTarget->SetXDir(+30);
	return RemoveEffect(, pTarget, iNr); // Fertig
	}
