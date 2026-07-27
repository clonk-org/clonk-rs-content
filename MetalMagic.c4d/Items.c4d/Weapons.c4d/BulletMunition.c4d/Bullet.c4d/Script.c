/*-- Kugel --*/

#strict 2

#include ARRW

local bLaunched, pLauncher;

local oldX, oldY;

/* Pfeileigenschaften */
public func PackTo() { return BLTP; }
public func IsBullet() { return 1; }
public func IsArrow() { return 0; }
public func BulletStrength() { return 16+Random(6); }

/* Produkteigenschaften */

public func IsDangerousToAirTravel() { return 1; }

func RejectEntrance()
{
	var i=inherited(...);
	if(i) return i;
	if(GetAction() == "Down") return true;
	return false;
}

/* Wird abgefeuert */
public func Launch(bQuiet)
{
  bLaunched=1;
  if(!bQuiet)
  {
   // Macht Rauch usw
   for(var cnt = 0; cnt < 3; cnt++)
     CreateParticle("FSpark",-GetXDir()/15,-GetYDir()/15,GetXDir()/50+RandomX(-2,2),GetYDir()/50+RandomX(-2,2),90,RGB(155,155,150,0));
   for(var cnt = 0; cnt < 3; cnt++)
     CreateParticle("FSpark",-GetXDir()/15,-GetYDir()/15,GetXDir()/25+RandomX(-2,2),GetYDir()/25+RandomX(-2,2),70,RGB(155,155,150,0));
 
   for(var cnt = 10; cnt < 10; cnt++)
   {
     var rand = Random(100);
     CreateParticle("MSmoke",-GetXDir()/15,-GetYDir()/15,GetXDir()/(40+Random(15))+RandomX(-2,2),GetYDir()/(40+Random(15))+RandomX(-2,2),200,RGBa(205-rand,205-rand,205-rand,50));
   }
 
   for(var cnt=0;cnt<6;cnt++)
     Smoke(-GetXDir()/20,-GetYDir()/20,10+Random(10));
 
   for(var cnt=0;cnt<6;cnt++)
     CreateParticle("Fire",-GetXDir()/15,-GetYDir()/15,GetXDir()/60+RandomX(-3,3),GetYDir()/60+RandomX(-3,3),70,RGBa(255,255,50,100));
   for(var cnt=0;cnt<6;cnt++)
     CreateParticle("Fire2",-GetXDir()/15,-GetYDir()/15,GetXDir()/60+RandomX(-3,3),GetYDir()/60+RandomX(-3,3),70,RGBa(255,255,50,100));
  }
 
  SetClrModulation(RGBa(255,255,255,90));

  oldX=GetX();
  oldY=GetY();

  SetAction("Shot");
	
	if(pLauncher)
		pLauncher->~Control2Effect("ShootProjectile", this);
	
  // Es gibt jetzt eine extra Trefferpruefung
  SetCategory(C4D_Vehicle,this);
}

/* Aufschlag */
protected func Hit()
{
  if(!bLaunched) return;

  InFlight();
  Sound("BulletHit*");

  var iColor=RGB(255,255,255);

  var iMat = -1;
  for(var cnt = -1; cnt <= 1; cnt++)
   for(var cnt2 = -1; cnt2 <= 1; cnt2++)
   {
     iMat = GetMaterial(cnt,cnt2);
     if(iMat != -1) break;
   }

   if(iMat == -1)
     for(var cnt = -5; cnt <= 5; cnt++)
       for(var cnt2 = -5; cnt2 <= 5; cnt2++)
       {
         iMat = GetMaterial(cnt,cnt2);
         if(iMat != -1) break;
       }
   
  if(iMat != -1)
    iColor=RGB(GetMaterialVal("Color","Material",iMat,0),
               GetMaterialVal("Color","Material",iMat,1),
               GetMaterialVal("Color","Material",iMat,2));

  CastParticles("MaterialParticle",20,15,0,0,20,40,iColor,iColor);

  RemoveObject();
}

/* Ziel prüfen */
private func InFlight()
{
  Tail();

  var target = FindTarget();
  if(target)
    return HitTarget(target);
        
  // Nicht unendlich fliegen usw
  if(GetActTime() > 30 && GetAction() == "Shot") Stop();
}

protected func FindTarget()
{ 
  var x = GetX()-oldX;
  var y = GetY()-oldY;

  // Optimierung mit Sort_Distance moeglich
  for(var obj in FindObjects(Find_OnLine(oldX-GetX(),oldY-GetY(),0,0),Find_Layer(GetObjectLayer()),Find_NoContainer(),Find_OCF(OCF_Alive, OCF_Living)))
  {
    if(obj != pLauncher){return obj;break;}
  }
  
    // Osterei!
  for(flnt in FindObjects(Find_OnLine(oldX-GetX(),oldY-GetY()),Find_Layer(GetObjectLayer()),Find_NoContainer()))
    if(flnt->~ExplodeSize())
      if(ActIdle(flnt))
      { flnt->Hit(); Hit(); return;}
 
  oldX=GetX();
  oldY=GetY();
 
  return 0;
}

protected func Tail()
{
  CreateParticle("BulletTail",0,0,GetXDir(),GetYDir(),BoundBy(GetActTime(),0,17)*10,RGBa(255,255,255,10),this);
}

protected func Stop()
{
  var blt = CreateObject(GetID(),0,0,GetOwner());
  blt->Launch(true);
  blt->SetAction("Down");
  SetXDir(GetXDir()/2,blt);
  SetYDir(GetYDir()/2,blt);
  blt->SetClrModulation(RGBa(255,255,255,130));
  RemoveObject();
}
/* Treffer */

private func HitTarget(pObject)
{
  Sound("BulletHit*");
  
  DoEnergy(-BulletStrength(),pObject);
  if(Random(3))Fling(pObject,GetXDir()/80,GetYDir()/80-2);
  
  RemoveObject();
}

