/*--Relauncher --*/

#strict 2

local iClonk;
local idClonks;
local iAngle;
local iWantedAngle;

static lastPlayerAngles;

func Initialize() {
  if(!lastPlayerAngles)lastPlayerAngles=[];
  
  iClonk=0;
  iAngle=1;
  iWantedAngle=0;
  idClonks=[KNIG,MAGE,ASAS,PLDN];
  
  SetAction("Do");
  Phase();
  
  AdjustiClonk();
  
  AddEffect("ShowSymbols",this,1,1,this);
  SetCategory(1 | C4D_Foreground);
  return 1;
}

func Phase()
{
 var iSec=15-GetPhase();
 PlayerMessage(GetOwner(),"$TimeTo$",0,iSec);
}

func End()
{
 SetAction("Idle");
 
 var pFriend=[];
 for(var obj in FindObjects(Find_ID(SNHD)))
 {
  if(GetPlayerTeam(GetOwner(obj)) == GetPlayerTeam(GetOwner()))
   pFriend[GetLength(pFriend)]=obj;
 }
 
 if(!GetLength(pFriend))
 {
  Message("$NoSainthood$",this);
  if(!GetEffect("CheckSainthood",this))
   AddEffect("CheckSainthood",this,1,1,this);
  return;
 }
 
 var rand=Random(GetLength(pFriend));
  
 SetPosition(GetX(pFriend[rand])+RandomX(-15,15),GetY(pFriend[rand])+Random(10),this);
 
 var crew=FindObject(0,0,0,0,0,OCF_CrewMember,0,0,this);
 if(!crew) RemoveObject(0,1);
 
 var newCrew=CreateContents(idClonks[iClonk]);
 MakeCrewMember(newCrew,GetOwner());
 RemoveObject(crew,0);
 SelectCrew(GetOwner(),newCrew,1);
 SetPlrView(GetOwner(),newCrew);
 
 EquipClonk(newCrew);
 
 lastPlayerAngles[GetOwner()]=iAngle;
 
 RemoveObject(0,1);
}

func EquipClonk(pClonk)
{
 pClonk->DoEnergy(100);
 if(GetID(pClonk) == MAGE)
 {
  CreateContents(PMAN,pClonk);
  //CreateContents(PMAN,pClonk);
  while(DoMagicEnergy(1,pClonk));
  CreateContents(SCKZ, pClonk);
  ScheduleCall(FindContents(SCKZ, pClonk), "Activate", 1, 0, pClonk);
  return 1;
 }
 
 if(GetID(pClonk) == KNIG)
 {
  CreateContents(SWOR,pClonk);
  CreateContents(BRED,pClonk);
  CreateObject(SHIE,0,0,GetOwner(pClonk))->Activate(pClonk);
  CreateContents(BOW1,pClonk);
  CreateContents(ARWP,pClonk);
  return 1;
 }
 
 if(GetID(pClonk) == KAND)
 {
  CreateContents(SPER,pClonk);
  CreateObject(SHI2,0,0,GetOwner(pClonk))->Activate(pClonk);
  CreateContents(PHEA,pClonk);
  CreateContents(SFLN,pClonk);
  return 1;
 }
 
 if(GetID(pClonk) == PLDN)
 {
  CreateContents(MUSK,pClonk);
  CreateContents(PBLP,pClonk);
  CreateObject(SHIE,0,0,GetOwner(pClonk))->Activate(pClonk);
  CreateContents(AXE1,pClonk);
  CreateContents(TSWD,pClonk);
  return 1;
 }
 
 if(GetID(pClonk) == ASAS)
 {
  CreateContents(CRBW,pClonk);
  CreateContents(BOTP,pClonk);
  CreateContents(KNFP,pClonk);
  CreateContents(METL,pClonk);
  return 1;
 }
 return 0;
}

protected func FxCheckSainthoodStart(pTarget,iEffectNumber)
{
 
}

protected func FxCheckSainthoodTimer(pTarget,iEffectNumber,iEffectTime)
{
 
 var x=Cos(iEffectTime*5,Sin(iEffectTime*2,5)+10);
 var y=Sin(iEffectTime*5,Sin(iEffectTime*2,5)+10);
 
 CreateParticle("PSpark",x,y,0,0,50,RGB(255,10,10),this);
 CreateParticle("PSpark",-x,-y,0,0,50,RGB(10,10,255),this);
 
 if(iEffectTime % 30 == 0)
   End();
 return 1;
}

func Init(iPlr)
{
 for(var i=0;i<GetCrewCount(iPlr);i++)
  Enter(this,GetCrew(iPlr,i));
 SetOwner(iPlr);
 SetVisibility(VIS_Owner);
 
 iAngle=lastPlayerAngles[GetOwner()];
 iWantedAngle=iAngle;
 if(!iAngle)iAngle=1;
 else iAngle+=1;
 AdjustiClonk();
}

func RejectCollect()
{
 return 1;
}

public func ContainedLeft(pCaller)
{
 iWantedAngle-=(360/GetLength(idClonks));
// while(iWantedAngle < -360) { iWantedAngle+=360; iAngle+=360; }
 while(Abs(iWantedAngle-iAngle) > 360)
 {
   if(iWantedAngle>iAngle) iWantedAngle -= 360;
   else iWantedAngle += 360;
 }
 //if(iWantedAngle < 0)iWantedAngle=360-iWantedAngle;
 
 AdjustiClonk();
 
 return 1;
}

func AdjustiClonk()
{
  var fakeAng=iWantedAngle % 360;
  if(fakeAng < 0)fakeAng=360+fakeAng;
  else
  if(fakeAng > 360)fakeAng=(fakeAng-360);
 
  for(var i=0;i < GetLength(idClonks);i++)
  {
   if(fakeAng > (360/GetLength(idClonks))*i)continue;
   //if(Inside(fakeAng,(360/GetLength(idClonks))*i,(360/GetLength(idClonks))*(i+1)))
   iClonk=(GetLength(idClonks)-1)-i;
   break;
  }
  
  Message("@%s",this,GetName(0,idClonks[iClonk]));
}

public func ContainedRight(pCaller)
{
 iWantedAngle+=(360/GetLength(idClonks));
// while(iWantedAngle > 360) { iWantedAngle-=360; iAngle-=360; }
 while(Abs(iWantedAngle-iAngle) > 360)
 {
   if(iWantedAngle>iAngle) iWantedAngle -= 360;
   else iWantedAngle += 360;
 }
 //if(iWantedAngle > 360)iWantedAngle=(iWantedAngle-360);
 
 AdjustiClonk();
 
 return 1;
}

protected func FxShowSymbolsStart(pTarget,iEffectNumber,iTemp)
{
 if(iTemp)return;
 var counter=1;
 for(var id in idClonks)
 {
  SetGraphics("",this,id,counter,GFXOV_MODE_Base,0,0,0);
  counter++;
 }
 EffectCall(pTarget,iEffectNumber,"Timer");
}

protected func FxShowSymbolsTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(iAngle == iWantedAngle && iEffectTime)return 1;
 iAngle=BoundBy(iWantedAngle,iAngle-5,iAngle+5);
 
 for(var i=1;i<GetLength(idClonks)+1;i++)
 {
  var iFac=900;
  if(i-1 == iClonk) iFac=1200;
  var x=Cos(iAngle+((360/GetLength(idClonks)*(i-1))),50);
  var y=Sin(iAngle+((360/GetLength(idClonks)*(i-1))),25);
  var y2=Sin(iAngle+((360/GetLength(idClonks)*(i-1))),100);
  SetObjDrawTransform(iFac,0,x*1000,0,iFac,(y+25)*1000,this,i);
 }
 
 return 1;
}


