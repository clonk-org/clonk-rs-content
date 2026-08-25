#strict
#include LASR

/* intern */

private func HitCheck() {

  // calculate length
  var mx=-Sin(r,iMaxDist),
      my=+Cos(r,iMaxDist),
      ml=0;

	var tempx = GetX(),
	    tempy = GetY(),
	    tempw = LandscapeWidth(),
	    temph = LandscapeHeight();

  while(++ml)
  { 
  	var tempx2 = mx*ml/iMaxDist + tempx,
  	    tempy2 = my*ml/iMaxDist + tempy;
    if(tempx2 < 0 && tempx2 > tempw) break;
    if(tempy2 < 0 && tempy2 > temph) break;
    if(ml>=iMaxDist) break;
  }
  // graphic
  SetLaser(ml == iMaxDist);

  l = 1000*ml/GetActMapVal("Facet",GetAction(),0,3);
  LaserEnd(mx,my); 

  // no hit check
  if(!pDmgCallback)
	return();

  // clonk = user of the weapon that fires (the shooter). Will be given with clonk
  var clonk;
  clonk = pDmgCallback->~GetUser();
  if(!clonk) clonk = pDmgCallback;
	
  // search points on the line for clonks etc.
  // With Find_OnLine!
  var pObj;
  // there may be several objects at that position
  //DrawParticleLine("PSpark",0,0,mx*ml/iMaxDist,my*ml/iMaxDist,5,25,RGB(0,255),RGB(255));
    for(pObj in FindObjects(Find_OnLine(0, 0, mx, my),
              Find_Exclude(this()),
              Find_Exclude(pAttach),
              Find_Exclude(pDmgCallback),
              Find_Exclude(Contained(pDmgCallback)),
              Find_NoContainer(),
              Find_Or(
              	Find_And(
              		Find_Or(
		              	Find_Func("IsBulletTarget",GetID(),this(),clonk),
              			Find_OCF(OCF_Alive)),
              		Find_Func("CheckEnemy",this())),
              	Find_Action("Dead")
              )
              ))
  {
  	pDmgCallback ->~ LaserStrike(pObj,c);
  }

}
