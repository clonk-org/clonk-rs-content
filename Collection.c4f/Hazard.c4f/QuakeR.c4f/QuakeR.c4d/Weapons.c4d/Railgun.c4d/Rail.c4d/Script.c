#strict
#include LASR

static const RAIL_MaxBounce = 5;

local ref;

func SetReflection(int iReflection) { ref = iReflection; }

/* intern */

private func HitCheck() {

  // calculate length
  var mx=-Sin(r,iMaxDist),
      my=+Cos(r,iMaxDist),
      ml=0;

	while(++ml) {
    if(GBackSolid(mx*ml/iMaxDist,my*ml/iMaxDist)) break;
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

	if(ml != iMaxDist && ref < RAIL_MaxBounce)
		CheckGround(ml);
}

func CheckGround(int length)
{
	var x,y;
	LaserEnd(x,y);
	
	var r = Normalize(LocalN("r"),0);
	var angle1, angle2; // Winkel des Materials
	var angle1set, angle2set;
	var rout; //ausgangswinkel
	var i;
	
//	Log("Position: %d, %d", GetX()+x, GetY()+y);
//	Log("Einfallswinkel: %d", Normalize(r+180,0));


	var oldx = x, oldy = y;
	for(i = 0; oldx == x && oldy == y; i++)
	{
		x = oldx+Sin(r, i);
		y = oldy-Cos(r, i);
//		SetLandscapePixel(x,y,RGB(255,255));
	}
	
	// wir suchen jetzt nach dem Winkel..!
	for(i = 0; i < 180; i++)
	{
		if(GBackSolid(x+Sin(r+i,5), y-Cos(r+i,5)) && !angle1set)
		{
			angle1set = true;
//			Log("Abweichung1: %d",i);
		}
		if(GBackSolid(x+Sin(r-i,5), y-Cos(r-i,5)) && !angle2set)
		{
			angle2set = true;
//			Log("Abweichung2: %d",i);
		}	
		if(!angle2set)
		{
//			SetLandscapePixel(x+Sin(r-i,5), y-Cos(r-i,5),RGB(0,0,255));
			angle1 = r-i;
		}
		if(!angle1set)
		{
//			SetLandscapePixel(x+Sin(r+i,5), y-Cos(r+i,5),RGB(0,255,0));
			angle2 = r+i;
		}
	}
	
	var angle = Normalize(Angle(Sin(angle1,5),-Cos(angle1,5),Sin(angle2,5),-Cos(angle2,5)),0);
	
//	Log("Wand: %d", angle);
	
	rout = Normalize(angle*2 - (r+180),0);
	
//	Log("Ausfallswinkel: %d", rout);
	
//	SetLandscapePixel(x,y,RGB(255));
//	Log("Solid: %d (%d/%d)",GBackSolid(x,y), GetX()+x, GetY()+y);
	oldx = x; oldy = y;
	var count;
	for(i = 0; GBackSolid(x,y); i++)
	{
		x = oldx+Sin(rout, i);
		y = oldy-Cos(rout, i);
		if(count++ > 10)
			return();
//		SetLandscapePixel(x,y,RGB(255,255));
	}
//	Log("Solid: %d (%d/%d)",GBackSolid(x,y), GetX()+x, GetY()+y);
//	Log("------");
	
	var newlaser = CreateObject(QRGR, x,y, GetOwner());
	newlaser->SetClrModulation(GetClrModulation());
	newlaser->SetReflection(ref+1);
	newlaser->Set(rout, 2, iMaxDist-length, iTime, pDmgCallback, pAttach);
//	Log("done");
}

func NoWarp() { return(true); }
