#strict 2

global func LaserLight(int iSize, int iRGB, int iX, int iY, int iIterations)
{
  if(!iIterations) iIterations = 1;

  for(var i = 0; i < iIterations; ++i) {
    var random = Random(360);
    CreateParticle("LsrSprk",iX,iY,Sin(random,10),-Cos(random,10),iSize*5,iRGB);
  }
  
  //if(!IsDark())
    return;
 /* 
  if(!iRGB)
    iRGB = RGB(255,255,255);

  var r,g,b,a;
  SplitRGBaValue(iRGB,r,g,b,a);
  iRGB = RGBa(r,g,b,Min(a+65,255));

 // AddLightFlash(iSize*14, iX, iY, iRGB);
*/
}

global func Chance(int chance) { // chance from 0 to 100%
	return Random(100) < chance;
}

global func Normalize(int iAngle, int iStart, int iPrecision) {
  if(!iPrecision) iPrecision = 1;
  var iEnd = iPrecision*360+iStart;
  
  while(iAngle<iStart) iAngle += iPrecision*360;
  while(iAngle>=iEnd) iAngle -= iPrecision*360;

  return iAngle;
}

global func CheckVisibility(object Obj1, object Obj2) {	// if Owner of Obj2 can see Obj1
	if(!Obj2)
		Obj2 = this;
		
	var vis = GetVisibility(Obj1);
	if(vis != VIS_All) {
		var own1 = GetOwner(Obj1);
		var own2 = GetOwner(Obj2);
	
		// VIS Owner
		if(vis & VIS_Owner)
			if(own1 == own2)
				return true;
		// VIS Enemies
		if(vis & VIS_Enemies)
			if(Hostile(own1,own2))
				return true;
		// VIS Allies
		if(vis & VIS_Allies)
			if(!Hostile(own1,own2))
				return true;
		// VIS Local
		if(vis & VIS_Local) {
			var lo = own2/32;
			var bit = 1 << own2%32;
			// man, kompliziert...
			if(bit & Local(lo,Obj1))
				return true;
		}
		// sonst gibt es keine Modi
		return false;
	}
	return true;
}

