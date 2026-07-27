/*--- Banner ---*/

#strict 2

//Maximale übertragunsreichweite für Mana
protected func MaxManaDis(){return 200;}

protected func Initialize()
{
  SetAction("Fly");
  Wind2Fly();
  
  AddEffect("ManaSave",this,20,13+Random(4),this);
}

/* TimerCall */

private func Wind2Fly()
{
  SetDir(BoundBy(7 + GetWind() / 10, 0, 13));
}

/* Einpacken */

public func ControlDigDouble()
{
  [$MnuPack$]
  CreateObject(BANP, 0, +20, GetOwner());
  RemoveObject();
  Sound("TentPack");
  return 1;
}

/* Farbe setzen */

protected func OnOwnerChanged() 
{ 
  return UpdateColor(); 
}

public func Unpacked()
{
  return UpdateColor(); 
}  

public func UpdateColor()
{
  // Farbmodulation nach Besitzer setzen
  SetClrModulation(GetPlrColorDw(GetOwner()));
  // Fertig
  return 1;
}

//Die ganzen Blitzteileffekteundbla!
public func LightningStrike()
{
 //zOMG Manastrike!
 for(var cnt=50;cnt--;) DoMagicEnergy(1);
}

public func NextWizardStructure()
{
 var next;
  for(var obj;obj=FindObject(0,0,0,-1,-1,0,0,0,NoContainer(),obj);)
  {   
  if(~GetCategory(obj)&C4D_Structure && !obj->~CastlePartWidth())continue;
  
  if(!GetPhysical("Magic",0,obj))continue;
  
  next=obj;
  break;
 }
 
 return next;
}

protected func FxManaSaveTimer(pTarget, iEffectNumber)
{
 if(!this)return -1;
 if(ObjectCount(NMGE))return -1;
 if(!GetMagicEnergy())return 0;
 
 
 if(!Random(2))CastParticles("MSpark",GetMagicEnergy()/17,GetMagicEnergy()/5,0,-25,20,100,RGB(255,255,255),RGB(255,255,255));
 
 //Ein CLonk hat aua, wenn das banner unter Strom steht!1
 var clnk;
 if(GetMagicEnergy()>10)
 if(clnk=FindObject(0,0,0,0,0,OCF_CrewMember,"Push",this))
 {
  var strength=Random(GetMagicEnergy());
  DoMagicEnergy(-strength);
  Punch(clnk,strength/2);
  CastParticles("PxSpark",strength,strength/2,GetX(clnk)-GetX(),GetY(clnk)-GetY()-5,20,100,RGB(255,255,255),RGB(255,255,255));
 }
 
 //Mana übertrgseln!
 //Erstmal vll auf Magieraumteil? :O
 if(!GetEffect("TransferMana",this))
 for(var obj;obj=FindObject(0,0,0,-1,-1,0,0,0,NoContainer(),obj);)
 {
  if(obj==this)continue;
   
  if(ObjectDistance(this,obj)>MaxManaDis())break;
  if(~GetCategory(obj)&C4D_Structure && GetID(obj)!=GetID() && !obj->~CastlePartWidth())continue;
  
  if(GetID(obj)==GetID())
  if(ObjectDistance(obj->~NextWizardStructure(),obj) >= ObjectDistance(this,NextWizardStructure()))continue;
  
  if(!GetPhysical("Magic",0,obj)>=30)continue;
  if(GetMagicEnergy(obj)==GetPhysical("Magic",0,obj)/1000)continue;
  AddEffect("TransferMana",this,20,3,this,0,obj);
  break;
 }

 
 return 1;
}

//Nun das übertragen an sich
protected func FxTransferManaStart(object pTarget, int iEffectNumber, int iTemp, var1)
{
 //Ziel
 EffectVar(0, pTarget, iEffectNumber)=var1;
 //Abstand auf der linie
 EffectVar(1, pTarget, iEffectNumber)=0;
 //Schwankungen :D
 EffectVar(2, pTarget, iEffectNumber)=-10;
 //Ein buhl
 EffectVar(3, pTarget, iEffectNumber)=1;
 return 1;
}

protected func FxTransferManaTimer(object pTarget, int iEffectNumber, int iTemp)
{
 if(!EffectVar(0, pTarget, iEffectNumber))return -1;
 if(!this)return -1;
 
 //Mana schon voll?
 if(GetMagicEnergy(EffectVar(0, pTarget, iEffectNumber))==GetPhysical("Magic",0,EffectVar(0, pTarget, iEffectNumber))/1000)return -1;
 //Mein mana leer? :C
 if(!GetMagicEnergy())return -1;
 
 //Ziel läuft weg :C
 if(ObjectDistance(this,EffectVar(0, pTarget, iEffectNumber))>MaxManaDis())return -1;
 
 //Erstmal variablen ändern und so
 //EffectVar(1, pTarget, iEffectNumber)=(EffectVar(1, pTarget, iEffectNumber)+1);//%ObjectDistance(this,EffectVar(0, pTarget, iEffectNumber))+1;
 
  EffectVar(1, pTarget, iEffectNumber)+=6;
  if(EffectVar(1, pTarget, iEffectNumber)>=ObjectDistance(this,EffectVar(0, pTarget, iEffectNumber)))
  {
   for(var cnt=20;DoMagicEnergy(-1,this)&& cnt>0;cnt--)
   if(!DoMagicEnergy(1,EffectVar(0, pTarget, iEffectNumber)))break;
   EffectVar(1, pTarget, iEffectNumber)=0;
   
   var x=0;
   var y=-23;
   
   if(GetID( EffectVar(0, pTarget, iEffectNumber)) == CPMR)
   {
    x=-20;
    y=0;
   } 
   
   DrawLightning(Abs(GetX()),Abs(GetY()-23),Abs(GetX( EffectVar(0, pTarget, iEffectNumber))+x),Abs(GetY( EffectVar(0, pTarget, iEffectNumber))+y));
     }
 //Schwankungen
 /*if(EffectVar(2, pTarget, iEffectNumber)==1 && EffectVar(3, pTarget, iEffectNumber)==1)
   EffectVar(3, pTarget, iEffectNumber)=0;
   
 if(EffectVar(2, pTarget, iEffectNumber)==-1 && EffectVar(3, pTarget, iEffectNumber)==0)
   EffectVar(3, pTarget, iEffectNumber)=1;
 if(EffectVar(3, pTarget, iEffectNumber))EffectVar(2, pTarget, iEffectNumber)++;
 else EffectVar(2, pTarget, iEffectNumber)--;*/
 
 //->var angle=Angle(GetX(),GetY(),GetX(EffectVar(0, pTarget, iEffectNumber)),GetY(EffectVar(0, pTarget, iEffectNumber))+15)-90;
 //sow, nun der Effekt
 /*var i=0;
 for(var cnt=30;cnt+i<ObjectDistance(this,EffectVar(0, pTarget, iEffectNumber));cnt+=BoundBy(EffectVar(1, pTarget, iEffectNumber)%30,1,10000))
 {
  var x=Cos(angle+EffectVar(2, pTarget, iEffectNumber)+RandomX(-1,1),i+cnt);
  var y=Sin(angle+EffectVar(2, pTarget, iEffectNumber)+RandomX(-1,1),i+cnt);
  
  CreateParticle("PSpark",x,y,0,0,60,RGB(255,255,255),0,0);
  
  i+=30;
 }*/
  /*var x=Cos(angle+EffectVar(2, pTarget, iEffectNumber)+RandomX(-0,0),EffectVar(1, pTarget, iEffectNumber));
  var y=Sin(angle+EffectVar(2, pTarget, iEffectNumber)+RandomX(-0,0),EffectVar(1, pTarget, iEffectNumber));
  CreateParticle("PSpark",x,y-15,x/20,y/20,60,RGB(255,255,255),0,0);*/
 return 1;
}
