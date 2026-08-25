/*-- Seemine --*/

#strict

local iYDir;
local iTrigger;

public func Initialize() 
{
   SetAction("Wait");
   return(1);
}

private func CheckLiquid()
{
  if(!GetXDir() && !GetYDir()) SetRDir(0);
  if(InLiquid() && !GBackLiquid(0,-5) && (GetYDir()<0))
  {
    iYDir=0;
    SetAction("Activated");
  }
  if(InLiquid() && GBackLiquid(0,-5))
  {
    if(iYDir==1) iYDir=GetYDir();
    if(iYDir>(-70)) --iYDir;
    SetYDir(GetYDir(this(),20)+iYDir,this(),20);
    SetXDir(GetXDir(this(),100)*95,this(),10000);
  }
  if(InLiquid() && iTrigger>40)
  {
    iYDir=0;
    SetAction("Activated");
  }
  if(InLiquid() && (GetYDir()<0)) ++iTrigger;
}

private func Check()
{
  var obj;
  if(!Contained()) while(obj = FindObject(0,-100,-100,200,200,0,0,0,NoContainer(),obj))
  if(((GetX()<(GetX(obj)+GetDefCoreVal("Width",0,GetID(obj))/2)) && (GetX()>(GetX(obj)-GetDefCoreVal("Width",0,GetID(obj))/2))) && ((GetY()<(GetY(obj)+5+GetDefCoreVal("Height",0,GetID(obj))/2)) && (GetY()>(GetY(obj)-GetDefCoreVal("Height",0,GetID(obj))/2)))) {Explode(30,CreateObject(GUNP)); Explode(30,CreateObject(GUNP)); Explode(60);}
  if(!InLiquid()) SetAction("Wait");
  return(1);
}

private func IsChemicalProduct()
{
  return(1);
}