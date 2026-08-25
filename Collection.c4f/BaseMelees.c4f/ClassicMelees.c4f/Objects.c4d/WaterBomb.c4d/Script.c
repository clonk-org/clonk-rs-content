/*-- Wasserbombe --*/

#strict

local iFuse, iCount;
local iWaterAmount, iDeep;

func Initialize() 
{
  iFuse=1;
  iCount=0;
  SetAction("Loop");
  return(1);
}

func Hit()
{
  if(!InLiquid()) Sound("MetalHit*");
  return(1);
}

private func Sink()
{ 
  if(InLiquid() && !Contained())
  {
    SetXDir(GetXDir()-GetXDir()/10);
    if(GetXDir()>0) SetXDir(GetXDir()-1);
    if(GetXDir()<0) SetXDir(GetXDir()+1);
    SetYDir(23);
    ++iCount;
    if((iCount%4)==0) Bubble(0,-3); 
    if((iCount%35)==0) --iFuse;  
    if(iFuse<=0 && !Contained())
    {
      var obj;
      while(obj=FindObject(0,-100,-100,200,200,OCF_CrewMember,0,0,0,obj)) if(InLiquid(obj) && ObjectDistance(obj)<100) DoEnergy(-(100-ObjectDistance(obj)),obj);
      while(obj=FindObject(FISH,-150,-150,300,300,0,0,0,0,obj)) if(ObjectDistance(obj)<150) Kill(obj,1);
      while(obj=FindObject(SHRK,-75,-75,150,150,0,0,0,0,obj)) if(ObjectDistance(obj)<75) Kill(obj,1);
      while(obj=FindObject(SUB1,-150,-150,300,300,0,0,0,0,obj))
      { 
        var damage=(200-(ObjectDistance(obj)**(2))/115); 
        if(damage<0) damage=0;
        DoDamage(damage,obj);
        if(GetDamage(obj)<150) CreateObject(BI5P)->Activate(obj,3);
      }

      // Wasserfontäne
      while(GBackSemiSolid(0,-iDeep))
      {
        ++iDeep;
        if(iDeep>450) break;
      }
      var k=CastInt(iDeep/10);
      if(k!=0)
      {
        if(k<38) CastObjects(AL5P,4,5,RandomX(-3,3),-iDeep+4);
        if(k<31) CastObjects(AL5P,14,5,RandomX(-3,3),-iDeep+4);
        if(k<24) CastObjects(AL5P,17,5,RandomX(-3,3),-iDeep+4);
        if(k<17) CastObjects(AL5P,20,5,RandomX(-3,3),-iDeep+4);
        if(k<10) CastObjects(AL5P,23,5,RandomX(-3,3),-iDeep+4);
      }
      else CastObjects(AL5P,60,5,RandomX(-6,6),-iDeep+4);
      Explode(30);
      for(var i=1;i<=40;++i)
      {
        var x=Random(40)-20;
        var y=Random(40)-20;
        if(Distance(GetX()+x,GetY()+y,GetX(),GetY())<20) Bubble(x,y);
      }
      for(var i=1;i<=10;++i)
      {
        var x=Random(80)-40;
        var y=Random(80)-40;
        if(Distance(GetX()+x,GetY()+y,GetX(),GetY())<40) Bubble(x,y);
      }
      for(var i=1;i<=10;++i)
      {
        var x=Random(150)-75;
        var y=Random(150)-75;
        if(Distance(GetX()+x,GetY()+y,GetX(),GetY())<75) Bubble(x,y);
      }
    }
  }
}

public func Activate(pCaller)
{
  [$TxtChangeFuse$]
  ++iFuse;
  if(iFuse>6) iFuse=0;
  PlayerMessage(GetOwner(pCaller),Format("$TxtFuse$",iFuse),pCaller);
  return(1);
}

public func IsChemicalProduct() 
{
   return(1);
}