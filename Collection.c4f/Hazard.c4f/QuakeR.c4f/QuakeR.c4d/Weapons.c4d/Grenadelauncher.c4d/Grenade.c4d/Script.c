#strict

#include GREN

func Launch(int xdir, int ydir, int iDmg)
{
	_inherited(xdir,ydir,iDmg);
	if(shooter)
		SetClrModulation(shooter->GetColorDw());
	else
		SetClrModulation(RGB(0,128,0));
}

// Aufprall (Quake4R-Physik!!) (Jetzt mit NF-Balancing!)
func Hit(oldX,oldY,c,d,e,f,g)
{
  oldX/=10;
  oldY/=10;
  if(GBackSolid(2,0) || GBackSolid(-2,0))
   SetXDir((oldX*-1)/2);
   
  if(GBackSolid(0,-2))
   SetYDir(-oldY*2/3);
  else
   if(GBackSolid(0,2))
    SetYDir((oldY*-1)/2);
  // Letztendlich hört man den Aufprall auch noch (und zwar Quakesound..!)
  Sound("GLHit*");
}

func FxGrenadeTimer(object target, int effect, int time)
{
  if(time > ExplodeDelay()) return(HitObject());
  
  if(GBackLiquid())
  {
  	Bubble(0,0);
  	return();
  }
  
  var vel=Abs(GetXDir())+Abs(GetYDir());
  var alpha=Max(0,100-vel);
  
  CreateParticle("Smoke2", -GetXDir()/6, -GetYDir()/6, RandomX(-15, 15), -5,
                       vel/2+RandomX(20, 60), RGBa(100,100,100,50+alpha)); 
}
