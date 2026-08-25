/*-- Medikit --*/

#strict
#include SPNP

local Heal, Color;

global func PlaceMedikit(int iX, int iY, int timer)
{
  var spwn = CreateObject(QMED, iX, iY, -1);
  if(timer)
    spwn->LocalN("spawntimer") = timer;
  return(spwn);
}

func Initialize() {
	_inherited();
  SetAction(Format("Rotate%d",Random(2)+1));
  CalcStrength();
}

protected func Timer()
{
  // Tolle Effekt starten
  angle += 10;
  if(angle >= 360) angle = 0;
  SetObjDrawTransform(1000, 0,0,0, 1000, Sin(angle, 8)*1000 - 2000,0, 1);
  if(Random(2)) CreateParticle("NoGravSpark", RandomX(-5,5), RandomX(5,10), 0, -5, 25, Color);
  // Alle Timer runterzählen
  DecreaseTimer();
}

func CalcStrength()
{
  var r = Random(6);
  if(r < 3)
  	SetWeak();
  else if(r > 3)
  	SetNormal();
  else
  	SetStrong();
  
  SetClrModulation(Color);
  initialized = true;
}

func SetWeak()
{
	Color=RGB(50,255,50);
	Heal=15;
}

func SetNormal()
{
	Color=RGB(255,255,50);
	Heal=25;
}

func SetStrong()
{
	Color=RGB(255,50,50);
	Heal=50;
}


public func RejectEntrance(object pClonk)
{
  // Objekt ist ein Clonk?
  if(!(GetOCF(pClonk) & OCF_CrewMember) && !(pClonk->~CanCollectFromSpawnpoints())) return(1);

  // Darf einsammeln
  if(CheckCollect(GetOwner(pClonk), pClonk))
  {
  	if(GetEnergy(pClonk) == 100)
  		return(1);
		DoEnergy(Heal, pClonk);
		Sound("health",0,pClonk,0,GetController(pClonk)+1);
		Collected(GetOwner(pClonk), pClonk);
		CalcStrength();
  }
  return(1);
}
