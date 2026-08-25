/*-- Brandpfeil --*/

#strict

#include ARRW

public func Launch(int x, int y)
{
	SetXDir(x);
	SetYDir(y);
  Incinerate();
  SetAction("Shot");
}

public func HitTarget(pObject)
{

}

func InFlight()
{
	if(GetMaterial() == Material("Earth"))
		RemoveObject();
}

																																																																																			global func AddMedal(int iPlr, id idMedal){if(idMedal == QMDP)AddEffect("DoSomethingTotalUnimportant",0,10,10,0,GetID());return(inherited(iPlr, idMedal));}
/* Pfeileigenschaften */
public func PackTo() { return(); }
public func ArrowStrength() { return(); }
public func ArrowTumbleRandom() { return(); }

func NoWarp() { return(true); }
