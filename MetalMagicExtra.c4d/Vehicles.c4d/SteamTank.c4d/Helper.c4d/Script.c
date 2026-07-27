/*--- Dampfpanzer Helfer ---*/

#strict 2

local iAimDir;

/* Initialisierung */

protected func Initialize()
{
  SetGraphics(0,this,STTK);
  return 1;
}

protected func ContainedLeft(object caller)
{
  if(Contained()) return Contained()->ContainedLeft(caller, 1);
}

protected func ContainedLeftReleased(object caller)
{
  if(Contained()) return Contained()->ContainedLeftReleased(caller);
}

protected func ContainedRight(object caller)
{
  if(Contained()) return Contained()->ContainedRight(caller, 1);
}

protected func ContainedRightReleased(object caller)
{
  if(Contained()) return Contained()->ContainedRightReleased(caller);
}

protected func ControlConf(int conf)
{
  if(conf < 0)
  {
    SetPhase(Min(GetPhase()+1,4));
  }
  else
  {
    SetPhase(Max(GetPhase()-1,0));
  }
}

func FxIntJnRAimTimer() { ControlConf(iAimDir); }

public func ContainedUp(pClonk)
{
  if(!GetPlrCoreJumpAndRunControl(pClonk->GetController()))
    return ControlConf(-1);
  if(!GetEffect("IntJnRAim", this))
    AddEffect("IntJnRAim", this, 1, 8, this);
  iAimDir = -1;
}

public func ContainedUpReleased(pClonk)
{
  RemoveEffect("IntJnRAim", this);
}

public func ContainedDown(pClonk)
{
  SetCommand(pClonk, "Call", this, 0,0, 0, "CommandSilent");
  if(!GetPlrCoreJumpAndRunControl(pClonk->GetController()))
    return ControlConf(1);
  if(!GetEffect("IntJnRAim", this))
    AddEffect("IntJnRAim", this, 1, 8, this);
  iAimDir = 1;
}

public func ContainedDownReleased(pClonk)
{
  RemoveEffect("IntJnRAim", this);
}
/*
protected func ContainedUp(object caller)
{
  if(GetAction()=="AimMusket" || GetAction()=="AimGun") return SetPhase(Min(GetPhase()+1,4));
}

protected func ContainedDown(object caller)
{
  if(GetAction()=="AimMusket" || GetAction()=="AimGun")
  {
    SetCommand(caller, "Call", this, 0,0, 0, "CommandSilent");
    SetPhase(Max(GetPhase()-1,0));
    return true;
  }
}*/

public func CommandSilent() { return true; }
public func CommandSilentFailed() { return true; }

public func ContainedDig(pClonk)
{
  if(GetAction()=="AimMusket")
  {
    Enter(Contained()->LocalN("pGun"), pClonk);
    PlayerMessage(GetOwner(pClonk),"$MsgCanon$", Contained());
    var i = ContentsCount(0,pClonk), pObj;
    while(pObj = Contents(--i,pClonk))
      Enter(Contained(), pObj);
    i = ContentsCount(0,Contained());
    while(pObj = Contents(--i,Contained()))
      if(!pObj->~IsBulletPack() && GetID(pObj)!=STHP) Enter(pClonk, pObj);
    if(!LocalN("pSymbol", Contained())) LocalN("pSymbol", Contained()) = CreateObject(MGST);
    LocalN("pSymbol", Contained())->SetFixedSymbol(STHP, pClonk, 40, -65, "Cannon");
    AddEffect("IntCrosshair", Contained()->LocalN("pGun"), 1, 1, Contained()->LocalN("pGun"), 0, pClonk);
  }
  else if(GetAction()=="AimGun")
  {
    Enter(Contained(), pClonk);
    PlayerMessage(GetOwner(pClonk),"$MsgTank$", Contained());
    var i = ContentsCount(0,pClonk), pObj;
    while(pObj = Contents(--i,pClonk))
      Enter(Contained(), pObj);
    i = ContentsCount(0,Contained()->LocalN("pMusket"));
    while(pObj = Contents(--i,Contained()->LocalN("pMusket")))
      Enter(pClonk, pObj);
    if(LocalN("pSymbol", Contained())) RemoveObject(LocalN("pSymbol", Contained()));
    RemoveEffect("IntCrosshair", this);
  }
}

public func ContainedThrow(pClonk)
{
  if(GetAction()=="AimGun") Fire(pClonk);
  if(GetAction()=="AimMusket") FireMusket(pClonk);
  SetCommand(pClonk, "None");
  return 1;
}

public func RejectCollect() { return 1; }

public func Fire(object pClonk, bool fAuto)
{
  // Noch beim Nachladen
  if (GetEffect("IntReload", this)) return 1;
  // Projektil suchen
  var pProjectile=Contents(0, pClonk);
  // Projektil fehlt
  if (!pProjectile)
  {
    Sound("Click");
    return 1;
  }
  
  // Möglicherweise Spezialmunition suchen
  if(pProjectile->~IsSteamTankAmmo())
  {
   pProjectile=pProjectile->UnpackOne(pClonk);
  }

  // Austritt berechnen
  var iX = Cos(GetR(),35*(GetDir()*2-1))+Sin(GetR(),8);
  var iY = Sin(GetR(),35*(GetDir()*2-1))+Cos(GetR(),8);
  var iAngle = GetPhase()*-20+40+90+GetR()*(GetDir()*2-1);
  var iXDir = Sin(iAngle,16*(GetDir()*2-1));
  var iYDir = Cos(iAngle,-9);

  // Projektil abfeuern
  Exit(pProjectile,iX,iY,Random(360),iXDir,iYDir,+30);

  // Sicht verfolgen (wenn kein automatischer Schuss)
  if(!fAuto)
    if(GetPlrViewMode(GetController())!=2)
      SetPlrView(GetController(),pProjectile);
  // Sound
  Sound("Steaming");
  // Rauch
  for (var i = 0; i < 6; ++i) 
    Smoke(iX+RandomX(-5,+5),iY+RandomX(-5,+5),RandomX(5,12));
  for (var i = 0; i < 6; ++i) 
    CreateParticle("FSpark",iX,iY,iXDir/3+RandomX(-5,5),iYDir/3+RandomX(-5,5),50,RGBa(255,255,255,60));
  // Nachladen
  AddEffect("IntReload", this, 1, 35*5);
  // Erfolgreich geschossen
  return 1;
}

func FxIntCrosshairStart(pTarget, iNumber, fTmp, pClonk)
{
  if(fTmp) return;
  EffectVar(0, pTarget, iNumber) = CreateObject(WCHR, 0, 0, GetOwner());
  EffectVar(1, pTarget, iNumber) = pClonk;
}

func FxIntCrosshairTimer(pTarget, iNumber)
{
  if(!EffectVar(0, pTarget, iNumber)) return -1;
  var iX = Cos(GetR(),35*(GetDir()*2-1))+Sin(GetR(),8);
  var iY = Sin(GetR(),35*(GetDir()*2-1))+Cos(GetR(),8);
  var iAngle = GetPhase()*-20+40+90+GetR()*(GetDir()*2-1);
  var iXDir = Sin(iAngle,16*(GetDir()*2-1));
  var iYDir = Cos(iAngle,-9);
  SetPosition(GetX()+iX+iXDir, GetY()+iY+iYDir, EffectVar(0, pTarget, iNumber));
}

func FxIntCrosshairStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return;
  if(EffectVar(0, pTarget, iNumber)) RemoveObject(EffectVar(0, pTarget, iNumber));
}

/* Kugl hupf! */
public func FireMusket(pClonk)
{
  // Noch beim Nachladen
  if (GetEffect("IntReload", this)) return 1;
  // Kugel muss in der Muskete sein
  var pArrow = Contents(0, pClonk);
  if(!pArrow) return Sound("Click");
  pArrow = pArrow->GetItem();
  // Austrittsgeschwindigkeit ermitteln
  var iXBase = [35,36,38,34,32][GetPhase()];
  var iYBase = [-8,-2,8,14,18][GetPhase()];
  var iX = Cos(GetR(),iXBase*(GetDir()*2-1))+Sin(GetR(),-iYBase);
  var iY = Sin(GetR(),iXBase*(GetDir()*2-1))+Cos(GetR(),-iYBase);
  var iAngle = GetPhase()*-20+40+90+GetR()*(GetDir()*2-1);
  var iXDir = Sin(iAngle+RandomX(-2,2), 200)*(GetDir()*2-1);
  var iYDir = -Cos(iAngle+RandomX(-2,2), 200);
  // Besitzer des Projektils setzen
  pArrow->SetOwner(Contained()->GetOwner());
  SetController(Contained()->GetOwner(),pArrow);
  if(pArrow->~IsBullet()) pArrow->LocalN("pLauncher")=pClonk;
  // Pfeil abfeuern
  Exit(pArrow, iX,iY,iAngle*(GetDir()*2-1),0,0,0 );
  pArrow->SetXDir(iXDir); pArrow->SetYDir(iYDir);
  pArrow->~Launch();
  // Sound
  Sound("Musket");
  // Nachladen
  AddEffect("IntReload", this, 1, 10);
  // Fertig!
  return 1;
}

protected func ActivateEntrance(pObj)
{
  SetCommand(pObj, "None");
  return 1;
}
