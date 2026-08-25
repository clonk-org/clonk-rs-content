/*-- Oil lamp --*/

#strict

local wonderlamp,x,y;

protected func Initialize()
{
  wonderlamp= 0;
  SetCategory(C4D_Vehicle());
  SetAction("On");
}

protected func ControlDigDouble()
{
  if(GetAction() S= "On") return(SetOff());
  SetAction("On");
}

public func SetOff()
{
  SetAction("Off");
  Sound("Pshshsh");
  if (wonderlamp==1)
   {
     while(Var(0)=FindObject(0, -7, -20, 14, 20, OCF_Living(), 0, 0, NoContainer(), Var(0)))
      if(GetAlive(Var(0)))
       return(BeamTarget(Var(0)));
   }
  return(1);
}

protected func BeamTarget()
{
  SetPosition(x, y, Par(0)); //Zielkoordinaten des Beamvorgangs
  DrawParticleLine("MSpark", 0, 0, GetX(Par(0))-GetX(), GetY(Par(0))-GetY(), 20, 100, RGBa(255,50,50,50), RGBa(50,255,50,100), -10);
  CastParticles("MSpark", 5, 50, GetX(Par(0))-GetX(),GetY(Par(0))-GetY(), 100, 200, RGBa(100,100,255,128), RGBa(0,0,255,0));
  CreateParticle("MSpark", GetX(Par(0))-GetX(),GetY(Par(0))-GetY(), 1000, RGBa(50,50,255,150));
  Sound("Magic1");
  Message("$Welcome$",Par(0));
  return(1);
}

protected func SetWonderlamp(int xAxis, int yAxis)
{
  wonderlamp= 1;
  x= xAxis;
  y= yAxis;
  return(1);
}