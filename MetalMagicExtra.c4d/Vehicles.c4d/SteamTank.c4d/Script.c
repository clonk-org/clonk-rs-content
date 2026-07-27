/*--- Dampfpanzer ---*/

#strict 2

#include DOOR

local pWheel1;
local pWheel2;
local pTank;
local pMusket;
local pGun;
local pClonk;
local pSymbol;

local iAimDir;

/* Initialisierung */

protected func Initialize()
{
  SetAction("Walk");
  SetComDir(COMD_Stop);
  if(!pWheel1) pWheel1 = CreateContents(STHP);
  SetGraphics("Wheel1", pWheel1, STTK);
  pWheel1->SetAction("Wheel");
  if(!pTank) pTank = CreateContents(STHP);
  pTank->SetAction("WheelCap");
  if(!pWheel2) pWheel2 = CreateContents(STHP);
  SetGraphics("Wheel2", pWheel2, STTK);
  pWheel2->SetAction("Wheel");
  if(!pMusket) pMusket = CreateContents(STHP);
  pMusket->SetAction("AimMusket");
  pMusket->SetPhase(2);
  if(!pGun) pGun = CreateContents(STHP);
  pGun->SetAction("AimGun");
  pGun->SetPhase(2);
  EndTurn();
  SetWalkRotation();
}

protected func ActivateEntrance(pObj)
{
  if(GetPilote() && !Contained(pObj)) return Message("$MsgNotFree$", this);
  if (GetAction()=="Walk"||GetAction()=="Jump") SetAction("OpenDoor");
  return 1;
}

protected func Collection2(pObj)
{
  if(GetPilote() && GetPilote()!=pObj)
  {
    if(GetOCF(pObj) & OCF_CrewMember) SetCommand(pObj, "Exit");
    return;
  }
  if(GetOCF(pObj) & OCF_CrewMember) pClonk = pObj;
  AddEffect("IntCrosshair", pGun, 1, 1, pGun, 0, pClonk);
  SetEntrance(0);
}

protected func Ejection(pObj)
{
  if(pClonk == pObj) pClonk = 0;
  RemoveEffect("IntCrosshair", this);
}

public func GetPilote()
{
  if(pClonk)
    if(GetAlive(pClonk))
      return pClonk;
  pClonk = 0;
}

public func HostilePilote(pTo)
{
 var pPilote;
 if(!(pPilote=GetPilote()))return 0;
 if(Hostile(GetOwner(pTo),GetOwner(pPilote)))return 1;
 return 0;
}

public func Wheel()
{
  if(!Random(10))CreateParticle("Smoke",RandomX(-40,40),30,0,RandomX(-20,-80),RandomX(100,200),RGB(255,255,255));
  if(!pWheel1) return;
  if(GetComDir()==COMD_Stop) return;
  var iPhase = Mod(pWheel1->GetR()+360-20+GetDir()*40,360);
  pWheel1->SetR(iPhase);
  pWheel2->SetR(iPhase);
}

protected func EndTurn()
{
  if(GetAction()!="Turn")
  {
    SetGraphics(0, this, 0, 1, GFXOV_MODE_Object, 0, 0, pWheel1);
    SetGraphics(0, this, 0, 2, GFXOV_MODE_Object, 0, 0, pTank);
    SetGraphics(0, this, 0, 3, GFXOV_MODE_Object, 0, 0, pWheel2);
    SetGraphics(0, this, 0, 4, GFXOV_MODE_Object, 0, 0, pMusket);
    SetGraphics(0, this, 0, 5, GFXOV_MODE_Object, 0, 0, pGun);
    if(GetDir())
      pTank->SetObjDrawTransform(1000, 0,9000, 0, 1000,13000);
    else
      pTank->SetObjDrawTransform(1000, 0,-7000, 0, 1000, 13000);
  }
}

private func SetRotation(iR)
{
  var iR_old = iRot;
  var iDif = BoundBy(iR_old-iR, -3, 3);
  if(Abs(iDif)<=2) iDif = 0;
  if( (iR_old-iDif) == 0) iDif--;
  SetR(iR_old-iDif);
  if(Stuck())
  {
    SetPosition(GetX(), GetY()-1);
    if(Stuck())
    {
      SetPosition(GetX(), GetY()+1);
      SetR(iR_old);
      return 0;
    }
  }
  iRot = GetR();
  SetR(iRot, pTank); SetR(iRot, pMusket); SetR(iRot, pGun);
  return 1;
}

private func GetHorizonHeight( int iX)
{
  var height;  
  while ( (height<LandscapeHeight()) && !(GBackSemiSolid(iX,height)) )
  {
    height = height+2;
    if(height > 52) break;
  }
  return height;
}
local pParticle;
local iHeight;
local iRot;
local iROld;
local iHit;
protected func SetWalkRotation()
{
  if(!pWheel1) return;
  var iWPos1 = -7, iWPos2 = 23;
  if(GetDir()) { iWPos1 = -23; iWPos2 = 9; }
  var iH0 = GetHorizonHeight(  0);
  var iH1 = GetHorizonHeight(iWPos1);
  var iH2 = GetHorizonHeight( iWPos2);
  var iA0 = Angle(iWPos1, iH1,iWPos2, iH2)-90;
  var iA1 = Angle(iWPos1, iH1, 0, iH0)-90;
  var iA2 = Angle(  0, iH0,iWPos2, iH2)-90;
  if(iH1 > 52)  {    SetRotation(iA2);}
  else if(iH2 > 52){ SetRotation(iA1);}
  else {         SetRotation(iA0);}

  SetRDir();

  var iHeight_old = iHeight;
  var iWhish = 0;
  if(GetAction()=="Jump") { iWhish = 2; if(GetYDir()+GetXDir()>30) iHit=1; }
  else if(iHeight && GetActTime()<7) { iWhish = -10; if(iHit) Sound("ClonkHit*",iHit=0); }
  if(iHeight <  iWhish) iHeight++;
  if(iHeight > iWhish ) iHeight--;
  if(iHeight > iWhish ) iHeight--;
  if(iHeight > iWhish ) iHeight--;
  if(iHeight_old != iHeight)
  {
    SetVertex(1,0,0,0,2);
    SetVertex(1,1,iHeight+23,0,2);
    if(iHeight>iHeight_old) SetPosition(GetX(), GetY()-1);
  }
  var iR = GetR()-GetR(pWheel1);// SetR();
  if(GetDir())
  {
    pWheel1->SetObjDrawTransform(1000, 0, Cos(+iR,9000)+Sin(-iR,19000+iHeight*500), 0, 1000,
            -Sin(-iR,9000)+Cos(+iR,19000+iHeight*500));
    pWheel2->SetObjDrawTransform(1000, 0, Cos(+iR,-22000)+Sin(-iR,19000+iHeight*500), 0, 1000,
            -Sin(-iR,-22000)+Cos(+iR,19000+iHeight*500));
  }
  else
  {
    pWheel1->SetObjDrawTransform(1000, 0, Cos(+iR,-7000)+Sin(-iR,19000+iHeight*500), 0, 1000
            ,-Sin(-iR,-7000)+Cos(+iR,19000+iHeight*500));
    pWheel2->SetObjDrawTransform(1000, 0, Cos(+iR,23000)+Sin(-iR,19000+iHeight*500), 0, 1000,
            -Sin(-iR,23000)+Cos(+iR,19000+iHeight*500));
  }
//  SetR(iR);
}

/* Effekte */

protected func Puff()
{
  {
    Sound("Chuff");
    Smoke(26 -52*GetDir(), -20, 5 + Random(4));
  }
}

/* Aktionen */

public func TurnRight()
{
  if (Stuck() || (GetAction() != "Walk") || iHeight) return;
  SetAction("Turn");
  if (GetXDir() < 0) SetXDir(0);
  if(GetComDir()!=COMD_Stop) SetComDir(COMD_Right);
  SetDir(DIR_Right);
}


public func TurnLeft()
{
  if (Stuck() || (GetAction() != "Walk") || iHeight) return;
  SetAction("Turn");
  if (GetXDir() > 0) SetXDir(0);
  if(GetComDir()!=COMD_Stop) SetComDir(COMD_Left);
  SetDir(DIR_Left);
}
  
public func Turning()
{
  var iDir = !GetDir();
  SetGraphics(0, this, 0, 1);
  SetGraphics(0, this, 0, 2);
  SetGraphics(0, this, 0, 3);
  SetGraphics(0, this, 0, 4);
  SetGraphics(0, this, 0, 5);
  SetDir(iDir, pMusket);SetDir(iDir, pGun);SetDir(iDir, pTank);
  return 1;
}

public func ContainedThrow(pClonk)
{
  var pObj = Contents(0, pClonk);
  if(!pObj) return 1;
  if(pObj->~IsBulletPack()) FireMusket(pClonk);
  else Fire(pClonk);
  SetCommand(pClonk, "None");
  return 1;
}

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
  var iAngle = GetPhase(pGun)*-20+40+90+GetR()*(GetDir()*2-1);
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
  var iAngle = GetPhase(pMusket)*-20+40+90+GetR()*(GetDir()*2-1);
  var iXDir = Sin(iAngle+RandomX(-2,2), 200)*(GetDir()*2-1);
  var iYDir = -Cos(iAngle+RandomX(-2,2), 200);
  // Besitzer des Projektils setzen
  pArrow->SetOwner(pClonk->GetOwner());
  SetController(pClonk->GetOwner(),pArrow);
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


protected func ContainedLeft(object caller)
{
  if(Contained()) return Contained()->ContainedLeft(caller);
  SetCommand(this, "None");

  if(GetDir()) {
    if(GetComDir()!=COMD_Stop) return SetComDir(COMD_Stop);
    else TurnLeft();
  }
  SetComDir(COMD_Left);
  return 1;
}

protected func ContainedLeftReleased() { SetComDir(COMD_Stop); }

protected func ContainedRight(object caller)
{
  if(Contained()) return Contained()->ContainedRight(caller);
  SetCommand(this, "None");
  if(!GetDir())
  {
    if(GetComDir()!=COMD_Stop) return SetComDir(COMD_Stop);
    else TurnRight();
  }
  SetComDir(COMD_Right);
  return 1;
}

protected func ContainedRightReleased() { SetComDir(COMD_Stop); }

protected func ControlLeft(object caller)
{
  if(GetPilote()) return;
  return ContainedLeft(caller);
}

protected func ControlRight(object caller)
{
  if(GetPilote()) return;
  return ContainedRight(caller);
}

protected func ControlDown(object caller)
{
  if(GetPilote()) return;
  return ContainedDown(caller);
}

protected func ControlUp(object caller)
{
  if(GetPilote()) return 1;
  return;
}

protected func ContainedUpOld(object caller)
{
  if(Contained()) return Contained()->ContainedUp(caller);
  var pObj;
  if(pObj = FindObject2(Find_Func("IsStaircase"), Find_AtPoint(0,23)))
    return Enter(pObj);
 if(GetAction()=="Walk") DoJump();
 return 1;
}

public func DoJump()
{
  if(GetActTime()<30) return;
  SetComDir(COMD_Stop);
  AddEffect("IntSteam", this, 1, 1, this);
  SetAction("StartJump");
}

protected func ContainedDownOld(object caller)
{
  if(Contained())
  {
    Contained()->ContainedDown(caller);
    return 1;
  }
  if(GetComDir()==COMD_Stop && GetAction() != "Turn") return 0;
  SetComDir(COMD_Stop);
 return 1;
}

func FxIntSteamTimer(pTarget, iNumber, iTime)
{
  if(iTime==5)
  {
    SetAction("Walk");
    Jump();
  }
  if(iTime>=15) return -1;
  DoSteam();
}

public func DoSteam()
{
  for(var cnt=0;cnt<20;cnt++)
    CreateParticle("Smoke",RandomX(-40,40),30,0,RandomX(-20,-80),RandomX(100,200),RGB(255,255,255));
}

public func ContainedDig(pClonk)
{return ContainedUpOld();
  Enter(pMusket, pClonk);
  LocalN("pClonk") = pClonk;
  PlayerMessage(GetOwner(pClonk),"$MsgGun$", this);
  var i = ContentsCount(0,pClonk), pObj;
  while(pObj = Contents(--i,pClonk))
    Enter(pMusket, pObj);
  i = ContentsCount();
  while(pObj = Contents(--i,this))
    if(pObj->~IsBulletPack() && GetID(pObj)!=STHP && pObj!=GetPilote()) Enter(pClonk, pObj);
  pSymbol = CreateObject(MGST, 0, 0, GetOwner(pClonk));
  pSymbol->SetFixedSymbol(STHP, pClonk, 40, -65);
}

public func ContextExit(pClonk)
{
  [Ausstieg|Image=SSY1:5]
  SetCommand(pClonk, "Exit");
}

/* Schaden */    
    
protected func Damage()
{
  Message("<c %x>%d%</c>",this, RGB(255*GetDamage()/150, 255-255*GetDamage()/150), 100*GetDamage()/150);
  // Wenn der Panzer zu viel Schaden bekommen hat
  if (GetDamage() > 150)
  {
    var pObj;
    // Alles aus Geschütz und Kanone einsammeln
    if(pMusket)
     while(pObj = Contents(0,pMusket))
      Enter(this, pObj);
    if(pGun) 
     while(pObj = Contents(0,pGun))
      Enter(this, pObj);
    // Pilot fliegt raus!
    Exit(pClonk);
    if(LocalN("pSymbol", pClonk)) RemoveObject(LocalN("pSymbol", pClonk));
    // Muskete, Kanone und Räder entfernen
    if(pMusket) RemoveObject(pMusket,1);
    if(pGun) RemoveObject(pGun,1);
    if(pWheel1) RemoveObject(pWheel1,1);
    if(pWheel2) RemoveObject(pWheel2,1);
    if(pTank) RemoveObject(pTank,1);
    // Alles muss raus!
    var iBulletCount, pBullet, iAngle;
    while(pObj = Contents())
    {
      if(pObj->~IsBulletPack())
      {
        iBulletCount = Random(pObj->~PackCount());
	for(var i=0; i<iBulletCount; i++)
	{
          iAngle = Random(360);
	  pBullet = CreateObject(pObj->UnpackTo());
	  pBullet->SetXDir(Sin(iAngle, 200)); pBullet->SetYDir(-Cos(iAngle, 200));
          pBullet->~Launch();
	}
	RemoveObject(pObj);
	continue;
      }
      if(pObj->~IsSteamTankAmmo())
      {
	while(pObj)
	{
          iAngle = Random(360);
	  pBullet = pObj->~UnpackOne(this);
	  if(pBullet){
	  pBullet->SetXDir(Sin(iAngle, 100)); pBullet->SetYDir(-Cos(iAngle, 100));
          pBullet->~Launch();}
	}
	continue;
      }
      iAngle = Random(360);
      Exit(pObj, 0, 0, Random(360), Sin(iAngle, 16), Cos(iAngle, 16));
      pObj->~Launch();
    }
    // und explodieren!
    Explode(40);
  }
  return 1;
}

protected func RejectCollect(id def, object obj)
{
  // Bewegt sich das Objekt?
  if( (obj->GetOCF() & OCF_HitSpeed1) && (~obj->GetCategory() & C4D_StaticBack) )
  {
    // TODO: Checken, ob in einem gewissen Bereich
    // TODO: Sachen abprallen lassen?
    // Bewegungsvektor
    var ang = Angle(0, 0, obj->GetXDir(), obj->GetYDir() );
    // Staerke abhaengig von der Geschwindigkeit und Masse
    var strength = (obj->GetXDir()**2 + obj->GetYDir()**2) * obj->GetMass()/20;
    // Bewegungsfaktor
    var x = Sin(ang, strength), y = -Cos(ang, strength);
    // Schaden verursachen
    var damage = Sqrt(strength) / 25;
    DoDamage(damage);
    // Von Gegenstand getroffen
    if (GetOCF(obj) & OCF_Collectible)
      // Kollision im Gegenstand aufrufen
      PrivateCall(obj, "Hit");
  }

  // Drachen nehmen nichts auf.
  return true;
}

/* ------ Zielen ----- */

protected func ControlConf(int conf)
{
  if(conf < 0)
  {
    SetPhase(Min(GetPhase(pGun)+1,4), pGun);
    SetPhase(Min(GetPhase(pMusket)+1,4), pMusket);
  }
  else
  {
    SetPhase(Max(GetPhase(pGun)-1,0), pGun);
    SetPhase(Max(GetPhase(pMusket)-1,0), pMusket);
  }
  return 1;
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
//  SetCommand(pClonk, "Call", this, 0,0, 0, "CommandSilent");
  if(!GetPlrCoreJumpAndRunControl(pClonk->GetController()))
    return ControlConf(1);
  if(!GetEffect("IntJnRAim", this))
    AddEffect("IntJnRAim", this, 1, 8, this);
  iAimDir = 1;
  return 1;
}

public func ContainedDownReleased(pClonk)
{
  RemoveEffect("IntJnRAim", this);
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

public func CommandSilent() { return true; }
public func CommandSilentFailed() { return true; }
