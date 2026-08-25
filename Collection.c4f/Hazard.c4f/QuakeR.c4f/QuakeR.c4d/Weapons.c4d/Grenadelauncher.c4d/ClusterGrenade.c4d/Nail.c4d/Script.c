#strict
#include QSHT

func Initialize()
{
	if(GBackSolid())
		Remove();
	_inherited();
}

// Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  pTrail = CreateObject(TRAI,0,0,-1);
  if(pTrail) {
    pTrail->Set(iSize-2,iTrail,this());
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

func Hit()
{
	SetAction("Stuck");
}

func Stick()
{
	if(pTrail) {
    pTrail->SetPosition(GetX(),GetY());
    pTrail->Remove();
  }
  SetComDir(COMD_None);
  SetXDir();
  SetYDir();
}

private func HitObject(object pObject) {
	if(GetAction() S= "Travel")
  	BulletStrike(pObject);
}

public func BulletStrike(object pObj) {
  if(pObj)
    DoDmg(iDamage,DMG_Projectile,pObj,iPrec);
}

private func Color(int iATime) {
  return(GetPlrColorDw(GetOwner()));
}

public func TrailColor(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(180-iPrg,180-iPrg,180-iPrg,50+iPrg));
}

public func GlowColor(int iATime) {
  return(RGBa(0,0,0,255));
}

// Dummy-func
func HitExclude()
{

}
