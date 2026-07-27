/* Mystiker: Zauber erweitert */

#strict 2

#appendto MYST

/* Ballonpack */

private func ActivateMysticBPCK(pClonk)
{
	// schon ein Ballon dran?
	if (FindObject2(Find_ID(BALO), Find_ActionTarget(pClonk)))
		{
		Sound("Error",, pClonk);
		Message("$MsgOnlyOneBalloon$", pClonk);
		return 0;
		}
	return CreateObject(BALO)->Lift(pClonk);
}

private func MysticTimeBPCK()
{
  return 36*10;
}

/* geänderter Super-T-Flint */

private func ActivateMysticSTFN(pClonk)
{
  return CreateObject(FGOB,,, GetOwner(pClonk))->Activate(pClonk);
}

/* Metall */

private func ActivateMysticMETL(object pClonk)
{
  return CreateObject(MSSH, 0, 0, GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeMETL()
{
  return 36 * 70;
}

/* Airpedo */

private func ActivateMysticARPD()
{
  return DoSpellAim(this);
}

private func ActivateAngleARPD(object pClonk, int iAngle)
{
  CreateObject(ARP2, Sin(iAngle, 15), 4 - Cos(iAngle, 15), GetOwner(pClonk))->~Launch(iAngle, pClonk);
}

private func IsAimerSpellARPD()
{
  return true;
}

private func MysticTimeARPD()
{
  return 36 * 35;
}

protected func QueryCatchBlow(object pObj)
{
  if (GetID(pObj) == ARP2 && pObj->~NoCatchBlow(this))
    return true;
  return _inherited(pObj, ...);
}

/* Schädel */

private func ActivateAngleSKUL(pCaller, iAimAngle)
{
  // Zauber
  Sound("Magic*");
  var id = BONE;
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir, pRock;
  var iAngle;
  for(var i=0; i<5; i++)
  {
    pRock = CreateContents(id, pCaller);
    // Masse drastisch reduzieren sonst wird das nix mit dem Balancing :X
    pRock->SetMass(4);
    iAngle = iAimAngle+RandomX(-10,10);
    if(id!=BIRK) AddEffect("BoneTumble",pRock,1,3,pCaller);

    // Austrittsgeschwindigkeit ermitteln 
    iOutXDir = Sin(iAngle, 80);
    iOutYDir = -Cos(iAngle, 80);
    // Austrittsposition... 
    iOutX = Sin(iAngle, 25 * GetCon() / 100);
    iOutY = -Cos(iAngle, 25 * GetCon() / 100); 
    // Drehung 
    iOutR = iAngle; 
    iOutRDir = 0;
  
    // Besitzer des Projektils setzen
    pRock->SetOwner(pCaller->GetOwner());
    // Stein abfeuern
    Exit(pRock, iOutX,iOutY,iOutR, 0,0,iOutRDir );
    SetR(Random(360), pRock);
    SetRDir(100, pRock);
    pRock->SetXDir(iOutXDir); pRock->SetYDir(iOutYDir);
    if(id->~IsArrow()) pRock->~Launch();
  }
  return 1;
}
