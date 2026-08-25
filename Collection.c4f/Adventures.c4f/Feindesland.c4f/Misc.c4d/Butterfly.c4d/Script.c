/*-- Schmetterling --*/

#strict

local x,y,ix,iy;

protected func Initialize()
  {
  x=GetX();
  y=GetY();
  SetAction("Fly");
  DoCon(-40);
  return(1);
  }
  
/* TimerCall */  
 
Timer:
//Bei Nacht verschwinden
  SetVar(9,100); //falls keine Zeit, dann = Tag
  SetVar(9,Local(2,FindObject(TIME))); //Zeit
  if(Var(9)>40)  SetClrModulation(RGBa(255,255,255,0), this());
  if(Var(9)==0)  SetClrModulation(RGBa(255,255,255,255), this());
// Unterwasser
  if (InLiquid()) return(SetComDir(COMD_Up()));
//Richtungskontrolle
  CheckDir();
//Zum Zielpunkt fliegen
  SetCommand(this(),"MoveTo",0,ix,iy);
//zuweit weg = teleportieren
  if(!Random(2))  if(Distance(GetX(),GetY(),x,y)>200)  return(ix=x,iy=y);
  if(Distance(GetX(),GetY(),x,y)>300)  return(SetPosition(x,y,this()),ix=0,iy=0);
//Nahgenug dran und kein Gegner in sicht = dumm rum fliegen
  if(!Random(3))  if(Distance(GetX(),GetY(),x,y)<200)  return(ix=GetX()-50+Random(101),iy=GetY()-50+Random(101) );
  return(1);

CheckDir:
  if(x-GetX()<0)  return(SetDir(DIR_Left()));
  if(x-GetX()>0)  return(SetDir(DIR_Right()));
  return(1);