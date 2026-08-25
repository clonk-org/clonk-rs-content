/*-- Bruchteil einer Kristallkugel --*/

#strict
#include _BLP

local lastX,lastY,estX,estY;

func Activate()
{
  if (GetAction() eq "Activated") return(1);
  SetAction("Activated");
  return(1);
}

func Damage()
{
  if (GetDamage() < 1) return(1);
  SetAction("Activated2");
  return(1);
}

func Hit() { return(Sound("Crystal*")); }

/* Treffer */

private func HitTarget(pTarget)
{
 var iChange = - GetMagicEnergy (pTarget) * (4+Random(2))/16;
 if (iChange == 0) return(_inherited(pTarget));
 DoEnergy (iChange, pTarget);
 DoMagicEnergy(iChange, pTarget);
 Sound("Electric");
 CastParticles("MSpark", 5, 50, 0,0, 100, 200, RGBa(100,100,255,128), RGBa(0,0,255,0));
 CreateParticle("MSpark", 0,0, 1000, RGBa(50,50,255,150));
 RemoveObject(this());
 return(1);
}

/* Ziel pruefen */
private func InFlight()
{

 if(lastX || lastY)
 {
   // Wenn der Pfeil durch irgendwas gebeamt wurde oder Aehnliches muss eine Sicherheit her
   var bSkip=false;
   if(estX || estY)
   if(Distance(GetX(),GetY(),estX,estY) > Max(1,Max(Abs(GetXDir()),Abs(GetYDir())))*2){bSkip=true;}
   
   if(!bSkip)
   for(var obj in FindObjects(Find_OnLine(lastX-GetX(),lastY-GetY(),0,0),
                     Find_Exclude(this()),
                     Find_NoContainer(),
                                         Find_Layer(GetObjectLayer()),
                     Find_Or(Find_OCF(OCF_Alive()),Find_Func("IsArrowTarget",this())))){   
      // In der Trefferzone?
      if (obj->~IsOutsideArrowHitZone(this)) continue;
      
      HitTarget(obj);
      break;
    }
 }
 
 lastX=GetX();
 lastY=GetY();
 estX=GetX()+GetXDir();
 estY=GetY()+GetYDir();
 return(1);
}