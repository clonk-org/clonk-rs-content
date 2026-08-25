/* Explosion */

#strict
#appendto BOOM

//Explode mit Effekt und tollen Extraparametern für performance-/effektmäßiges Zeug
global func Explode(int iLevel, object pObj, id idEffect, string szEffect, bool bNoShots, bool biggersmoke) {
  if(!pObj)
    if(!(pObj=this()))
      return();
  
  var x = AbsX(pObj->GetX()),
  	  y = AbsY(pObj->GetY());
  
  if(biggersmoke)
  	iLevel *= 2;
  
  var i=0, count = 3+iLevel/15, angle = Random(360);
  while(count > 0 && ++i < count*10) {
    angle += RandomX(40,80);
  
    //Rauch
    var smokex = +Sin(angle,RandomX(iLevel/4,iLevel/2));
    var smokey = -Cos(angle,RandomX(iLevel/4,iLevel/2));
    if(GBackSolid(x+smokex,y+smokey))
      continue;
    var level = iLevel + Random(iLevel/5);
    CreateSmokeTrail(level,angle,smokex,smokey,pObj);
    count--;
  }

  CreateBurnMark(x,y,iLevel,20+iLevel/2);

	if(biggersmoke)
		iLevel /= 2;

  if(IsDark()) {
    var iSize = iLevel*100;
    if(iLevel < 20) {
      iSize /= 2;
    }
    AddLightFlash(iSize/3, x, y, RGBa(255,220,64,15));
  }

  return(inherited(iLevel, pObj, idEffect, szEffect));
}

global func FxSmokeTrailTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  var iStrength = EffectVar(0, pTarget, iEffectNumber);
  var iAStr = EffectVar(1, pTarget, iEffectNumber);
  var iX = EffectVar(2, pTarget, iEffectNumber);
  var iY = EffectVar(3, pTarget, iEffectNumber);
  var iXDir = EffectVar(4, pTarget, iEffectNumber);
  var iYDir = EffectVar(5, pTarget, iEffectNumber);

  iAStr = Max(1,iAStr-iAStr/7+Random(2));
  iAStr--;
  // nicht ganz so starke gravitation!
  iYDir += GetGravity()/2;

  var xdir = iXDir*iAStr/iStrength;
  var ydir = iYDir*iAStr/iStrength;

	// Neuer: ohne Random
  // Neu: Random
  //iX += RandomX(-3,3);
  //iY += RandomX(-3,3);
  
  // zuerst zeichnen
  CreateParticle("Smoke3",iX,iY,0,0,20+iAStr*6,RGBa(175,165,155,55+35*iAStr/iStrength));
  //CreateParticle("Blast",iX,iY,0,0,10+iAStr*8,RGBa(250,100+Random(100),100,200));

  // dann nächste position berechnen
  iX += xdir/75;
  iY += ydir/75;
  
  if(GBackSemiSolid(iX,iY))
    return(-1);
  if(iAStr <= 1)
    return(-1);
    
  EffectVar(1, pTarget, iEffectNumber) = iAStr;
  EffectVar(2, pTarget, iEffectNumber) = iX;
  EffectVar(3, pTarget, iEffectNumber) = iY;
  EffectVar(5, pTarget, iEffectNumber) = iYDir;
}
