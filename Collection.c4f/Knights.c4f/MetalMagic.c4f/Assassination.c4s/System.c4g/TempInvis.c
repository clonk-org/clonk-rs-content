/*-- Buffbombe --*/

#strict 2

global func DoTempInvisibility(pTarget)
{
 if(!GetEffect("TempInvis",pTarget))
  AddEffect("TempInvis",pTarget,20,5,0);
}

global func FxTempInvisStart(pTarget,iEffectNumber,iTemp)
{
 if(iTemp)return;
 
 for(var cnt=0;cnt<10;cnt++)
 {
  CreateParticle("FSpark",GetX(pTarget),GetY(pTarget),RandomX(-5,5),RandomX(-15,-3),40+Random(40),RGB(255,255,255));
  CreateParticle("FSpark",GetX(pTarget),GetY(pTarget),RandomX(-5,5),RandomX(-5,0),20+Random(20),RGB(255,10,10));
 }
 
 Sound("Puff");
 
 EffectVar(0,pTarget,iEffectNumber)=GetVisibility(pTarget);
 SetVisibility(VIS_Owner | VIS_Allies | VIS_God,pTarget);
 
 EffectVar(1,pTarget,iEffectNumber)=GetClrModulation(pTarget);
  
 return 1;
}

global func FxTempInvisTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(iEffectTime > 35*6)
 {
  for(var cnt=0;cnt<10;cnt++)
   {
    CreateParticle("FSpark",GetX(pTarget),GetY(pTarget),RandomX(-5,5),RandomX(-15,-3),40+Random(40),RGB(255,255,255));
    CreateParticle("FSpark",GetX(pTarget),GetY(pTarget),RandomX(-5,5),RandomX(-5,0),20+Random(20),RGB(255,10,10));
   }
  Sound("Puff");
  
  SetVisibility(EffectVar(0,pTarget,iEffectNumber),pTarget);
  SetClrModulation(EffectVar(1,pTarget,iEffectNumber),pTarget);
  return -1;
 }
 
 SetClrModulation(HSLa(Abs(Cos(iEffectTime*2,255)),128,255,Abs(Cos((iEffectTime-90)*3,255))),pTarget);
 
 return 1;
}
