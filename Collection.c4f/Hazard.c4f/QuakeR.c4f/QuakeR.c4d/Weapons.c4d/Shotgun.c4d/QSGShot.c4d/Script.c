#strict
#include QSHT

/* Schuss */

local iTime, lx, ly, pTrail, iDamage, iPrec;
local shooter; // Dingens/Clonk das den Schuss abgefeuert hat.

// Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  pTrail = CreateObject(TRAI,0,0,-1);
  if(pTrail) {
    pTrail->Set(iSize-2,iTrail,this());
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

private func HitObject(object pObject) {
  if(BulletStrike(pObject)) {
    Sparks(1,Color(GetActTime()));
    Remove()	;
  }
}

public func BulletStrike(object pObj) {
  if(pObj)
    DoDmg(iDamage,DMG_Projectile,pObj,iPrec);
  return(1);
}

private func Color(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(255,200-iPrg*2,200-iPrg*2,iPrg*2));
}

public func TrailColor(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(255-iPrg*2,200-iPrg*2,200-iPrg*2,iPrg*2));
}

public func GlowColor(int iATime) {
  return(RGBa(255,200,0,150));
}

// Dummy-func
func HitExclude()
{

}
