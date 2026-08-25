/*-- Schneller Schießen --*/

#strict 2

#appendto STHP

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
  AddEffect("IntReload", this, 1, 5, this);
  // Erfolgreich geschossen
  return 1;
}

func FxIntReloadTimer(pTarget, iNumber, iTime)
{
  Message("", pTarget);
  if(iTime >= GetReloadTime()) return -1;
  Message("%d", pTarget, GetReloadTime()-iTime);
}

public func GetReloadTime() { return 35; }
