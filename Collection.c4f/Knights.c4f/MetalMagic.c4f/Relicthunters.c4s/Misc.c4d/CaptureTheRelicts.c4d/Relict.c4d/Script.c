/*-- Relikt --*/

#strict 2
local sElement;
func Initialize() {
   SetAction("Turn");
  return 1;
}

public func SetElement(newElement)
{
 sElement=newElement;
 
 SetGraphics(sElement);
}

protected func Entrance()
{
 if(!GetEffect("RelictEffect",this))
 AddEffect("RelictEffect",this,20,3,this);
}


protected func FxRelictEffectStart(pT,iEN)
{
 EffectVar(0,pT,iEN)=0;
 return 1;
}

protected func FxRelictEffectTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(!Contained()) return -1;
 var x=0;
 var y=0;
 var xdir,ydir;
 var front=1;
 //if(~GetOCF(Contained()) & OCF_CrewMember)return -1;
 
 if((~GetOCF(Contained()) & OCF_CrewMember) || Contained(Contained()))
 {
  EffectVar(0,pTarget,iEffectNumber)++;
  
  if(EffectVar(0,pTarget,iEffectNumber) >30)
  {
   if(Contained(Contained()))Enter(Contained(Contained()));
   else
   if(!GetCommand())SetCommand(this,"Exit");
  }
  else
  if(EffectVar(0,pTarget,iEffectNumber) >80)Exit();
 } 
 else
 if(EffectVar(0,pTarget,iEffectNumber))EffectVar(0,pTarget,iEffectNumber)=0;

 
 xdir=Cos(iEffectTime*5,20);
 ydir=Sin(iEffectTime*5,20);
 var color=RGBa(255,255,255,50);
 if(sElement == "Earth")color=RGBa(200,150,20,50);
 if(sElement == "Fire")color=RGBa(255,100,10,50);
 if(sElement == "Water")color=RGBa(10,10,255,50);
 
 CreateParticle("TriangleSpark",0+x,0+y,0+xdir,0+ydir,250,color,Contained(),front);
 
 
 return 1;
}
