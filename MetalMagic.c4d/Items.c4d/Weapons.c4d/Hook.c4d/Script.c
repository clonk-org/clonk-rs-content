/*-- Der Enterhaken --*/

#strict 2

local pRope, pRopeEnd;
local iX, iY;

public func IsHook() { return 1; }

public func GetMaxShootPower() { return 100; }

public func Launch(pClonk)
{
  SetAction ("InFlight");
  if (!(GetXDir()+GetYDir())) return;
  if (pRope) return;
  pRopeEnd = CreateObject (RPND, 0, 0, GetOwner());
  pRope = CreateObject (RPHP, 0, 0, GetOwner());
  pRope -> Activate (pRopeEnd, this, 1);
  ObjectCall( pRopeEnd, "SetRope", pRope, this);
  Enter(pClonk, pRopeEnd);
  return;
}

public func InFlight()
{
  SetR(Angle(0,0,GetXDir(), GetYDir()));
}

public func Hooking()
{
  SetR(Angle(GetVertex(GetVertexNum(pRope)-2,0,pRope),GetVertex(GetVertexNum(pRope)-2,1,pRope),GetVertex(GetVertexNum(pRope)-1,0,pRope),GetVertex(GetVertexNum(pRope)-1,1,pRope)));
  if(!GBackSolid(iX-GetX(), iY-GetY()))
  {
    SetAction("Idle");
  }
}

public func Entrance()
{
  SetAction("Idle");
  var i,pObj;
  if(pRopeEnd)
  {
    if(pRope) RemoveObject (pRope);
    if(pRopeEnd) RemoveObject (pRopeEnd);  
  }
  return;
}

public func RopeBreak()
{
  pRope = 0;                                          
  if(pRopeEnd) RemoveObject(pRopeEnd);
  SetAction("Idle");                               
  return;
}

public func Hit()
{
  Sound("MetalHit*");
  if(!pRope) return;
  var iAnlge = Angle(0,0,GetXDir(), GetYDir());
  SetPosition(GetX()+Sin(iAnlge,2),GetY()-Cos(iAnlge,2));
  SetXDir();
  SetYDir();
  ObjectSetAction(pRope,"ConnectSingle",pRopeEnd, this);
  ObjectCall( pRope, "SetLength", ObjectCall( pRope, "CalcLength"));
  SetAction("Hooking");
  var i, j;
  for(i=0; i<9; i++)
  {
    for(j=0; j<9; j++)
    {
      iX = GetX()+i;
      iY = GetY()+j;
      if(i>=5) iX = GetX()-i+4;
      if(j>=5) iY = GetY()-j+4;
      if(GBackSolid(iX-GetX(), iY-GetY()))
        break;
    }
    if(GBackSolid(iX-GetX(), iY-GetY()))
        break;
  }
  return;
}

public func RejectCollect()
{
  return GetAction()=="Hooking";
}
