#strict
#include QSHT

/* Schuss */


// Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  pTrail = CreateObject(TRAI,0,0,-1);
  if(pTrail) {
    pTrail->Set(iSize-2,iTrail,this());
    pTrail->SetAction("Travel2");
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

private func Traveling()
{
	_inherited();
	CreateParticle("Blast",0,0,0,0,100, RGBa(0,0,255,20));
}

public func BulletStrike(object pObj) {
  if(pObj)
    DoDmg(iDamage,DMG_Energy,pObj,iPrec);
  return(1);
}

private func Color(int iATime) {
	var iPrg = 100*iATime/iTime;
  return(RGBa(100,150,220,iPrg*2));
}

private func GlowColor(int iATime) {
  return(RGBa(100,150,220));
}

private func HitObject(object pObject) {
  if(BulletStrike(pObject)) {
  var rand = 200;
  	CreateParticle("XSpark",0,0,0,0,rand,
                 	RGBa(100,200,255,100),pObject);
		AddLightFlash(rand, 0, 0, RGBa(100,200,255,10), pObject);
		//CreateParticle("XSpark",0,0,0,0,rand+50,
    	//             RGBa(180,255,220,200),pObject);
    Remove();
  }
}

func TrailColor() { return(RGBa(0,0,0,255)); }

//public func TrailColor(int iATime) { return(Color(iATime)); }
