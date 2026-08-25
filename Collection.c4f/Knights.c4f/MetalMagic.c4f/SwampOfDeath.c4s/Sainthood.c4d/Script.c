/*-- Heiligtum --*/

#strict 2

local fActivePlayer;
local fActiveUndead;
local iEnergy;
local iLightningSpeed;
local pNext;
local pLast;

static iLevel;

public func IsUndeadHome() { return fActiveUndead; }
public func IsPlayerHome() { return fActivePlayer; }

protected func Initialize()
{
  iEnergy = 10000;
  AddEffect("TeamEffect",CreateObject(SNHP,0,-50,-1),20,4,this,0);
  SetObjectBlitMode(2);
  SetClrModulation(RGB(20,20,20));
}

public func ControlUp(pClonk, selection)
{
  if(GetOwner()==-1) return;
  CreateMenu(SNHD,pClonk,this,C4MN_Extra_Value,"NothingToBuy",0,0,1);
 
  AddMenuItem("$Buy$: %s","Purchase",EFLN,pClonk,0,pClonk);
  AddMenuItem("$Buy$: %s","Purchase",SWOR,pClonk,0,pClonk);
  AddMenuItem("$Buy$: %s","Purchase",CNKT,pClonk,0,pClonk);
  AddMenuItem("$Buy$: %s","Purchase",FBMP,pClonk,0,pClonk);
  AddMenuItem("$Buy$: %s","Purchase",BHLG,pClonk,0,pClonk);

  AddMenuItem("$Buy$: %s","Purchase",ARMR,pClonk,0,pClonk);
  AddMenuItem("$Buy$: %s","Purchase",SHIE,pClonk,0,pClonk);
  AddMenuItem("$Buy$: %s","Purchase",BOW1,pClonk,0,pClonk);
  AddMenuItem("$Buy$: %s","Purchase",ARWP,pClonk,0,pClonk);
  AddMenuItem("$Buy$: %s","Purchase",FARP,pClonk,0,pClonk);
  AddMenuItem("$Buy$: %s","Purchase",XARP,pClonk,0,pClonk);
  
  AddMenuItem("$Buy$: %s","Purchase",BLTP,pClonk,0,pClonk);
  AddMenuItem("$Buy$: %s","Purchase",PBLP,pClonk,0,pClonk);
  AddMenuItem("$Buy$: %s","Purchase",SBLP,pClonk,0,pClonk);
  AddMenuItem("$Buy$: %s","Purchase",LFAM,pClonk,0,pClonk);
  AddMenuItem("$Buy$: %s","Purchase",BRED,pClonk,0,pClonk);
  
  if(GetID(pClonk)==KNIG)
  {
    AddMenuItem("$Train$: %s","Purchase",PLDN,pClonk,0,pClonk);
    AddMenuItem("$Train$: %s","Purchase",MAGE,pClonk,0,pClonk);
  }
  if(selection)
  SelectMenuItem(selection,pClonk);
}

public func Purchase(idID,pClonk)
{
  var owner=GetOwner(pClonk);
  var selection=GetMenuSelection(pClonk);

  if(GetWealth(owner) < GetValue(0,idID))
  {
    PlayerMessage(owner,"$NotEnoughMoney$",pClonk);
    return 1;
  }
 
  DoWealth(owner,-GetValue(0,idID));
 
   var object; 
  if(idID->~IsClonk())
  {
    pClonk->Redefine2(idID);
    if(idID == PLDN)
    {
      CreateContents(TSWD, pClonk);
      CreateContents(MUSK, pClonk);
      CreateContents(SBLP, pClonk);
      pClonk->ChangedTo();
    }
    if(idID == MAGE)
    {
      DoMagicEnergy(100, pClonk, 1);
      CreateContents(SCKZ, pClonk);
      SetPortrait("random", pClonk, MAGE);
      pClonk->Recruitment(GetOwner(pClonk));
    }
  }
  else
  {
    object = CreateObject(idID,0,14,owner);
    Collect(object,pClonk);
  }
  if(idID == PHNX)
  {
    object->~Summoned(pClonk);
  }
 
  ControlUp(pClonk,selection);
 
  Sound("Cash"); 
}

public func GetMaxUndead()
{
  if(iDifficulty == 1) return  20;
  if(iDifficulty == 2) return  50;
  if(iDifficulty == 3) return 100;
}

public func GetSpawnFactor()
{
  if(iDifficulty == 1) return 10-iLevel;
  if(iDifficulty == 2) return  8-iLevel;
  if(iDifficulty == 3) return  5-iLevel;
}

protected func Check()
{
 if(!GetEffect("ChangeOwner",this) && !Random(3))
 {
  CheckConquerors();
  
  CheckEnemy();
  CheckHeal();
 }
 var iColor;
 if(fActiveUndead)
 {
   if(fActiveUndead>1) return fActiveUndead--;
   if(ObjectCount2(Find_Func("IsUndead"), Find_OCF(OCF_Alive))<GetMaxUndead())
     if(!Random(ObjectCount2(Find_Func("IsUndead"), Find_OCF(OCF_Alive))*(10-iLevel)))
   {
      SpawnEnemy();
   }
   var obj;
   if(obj = FindObject2(Find_ID(BHLG), Find_Distance(50)))
   {
     Sound("Poff");
     RemoveObject(obj);
     iEnergy -= 2000;
     Message("%d%", this, 100-iEnergy/100);
     iColor = 20 + 60*(10000-iEnergy)/10000;
     SetClrModulation(RGB(iColor, iColor,iColor));
     if(!iEnergy)
     {
       if(!pLast) iLevel++;
       // Prämie
       DoWealth(0, 100);
       pLast = FindObject2(Find_ID(SNHD), Find_Func("IsPlayerHome"));
       pLast->LocalN("fActivePlayer") = 0;
       fActivePlayer = 1;
       fActiveUndead = 0;
       iEnergy = 10000;
       SetOwner(GetPlayerByIndex());
       for(obj in FindObjects(Find_Func("IsUndead"))) Kill(obj);
       if(pNext)
         pNext->LocalN("fActiveUndead") = 100;
     }
   }
 }
 else if(fActivePlayer && !Random(3))
 {
   var iEmenys;
   if(iEmenys = ObjectCount2 (Find_ID(SKLT), Find_Distance(50)))
     iEnergy -= iEmenys*30;
   if(iEmenys = ObjectCount2 (Find_ID(_SSR), Find_NoContainer(), Find_Distance(50)))
     iEnergy -= iEmenys*60;
   if(iEmenys = ObjectCount2 (Find_ID(_ZMB), Find_Distance(50), Find_OCF(OCF_Alive)))
     iEnergy -= iEmenys*90;
   if(iEmenys = ObjectCount2 (Find_ID(BTRL), Find_Distance(50), Find_OCF(OCF_Alive)))
     iEnergy -= iEmenys*150;
   if(iEmenys = ObjectCount2 (Find_ID(_SKN), Find_Distance(50)))
     iEnergy -= iEmenys*60;
   if(iEnergy < 10000)
     iEnergy++;
   Message("%d%", this, iEnergy/100);
   iColor = 20 + 60*iEnergy/10000;
   SetClrModulation(RGB(iColor, iColor,iColor));
   if(iEnergy<=0)
   {
     if(pLast)
       pLast->LocalN("fActivePlayer") = 1;
     FindObject2(Find_ID(SNHD), Find_Func("IsUndeadHome"))->LocalN("fActiveUndead") = 0;
     fActivePlayer = 0;
     fActiveUndead = 1;
     iEnergy = 10000;
     SetOwner(-1);
   }
 }
}

protected func SpawnEnemy()
{
  for(var i = 0; i < iDifficulty; i++)
  {
    if(!Random(8) && iLevel>= 3) DoSpawnEnemy(BTRL,70);
    else if(!Random(6) && iLevel>= 2) DoSpawnEnemy(_ZMB,50);
    else if(!Random(3) && iLevel) DoSpawnEnemy(_SSR,10);
    else if(iLevel >= 3) DoSpawnEnemy(_SKN, 30);
    else DoSpawnEnemy(SKLT, 10);
  }
}

protected func DoSpawnEnemy(idID, iEnergy)
{
  var pObj = CreateObject(idID, RandomX(-50,50), 14, -1);
  DoEnergy(iEnergy-GetEnergy(pObj), pObj);
  pObj->SetPhysical("Energy", 1000*iEnergy, 2);
  DoMagicEnergy(100, pObj, 1);
  pObj->SetAI();
  pObj->SetAction("FlatUp");
  LocalN("ai_aSpells", pObj) = [CMFG, EXTG, MLGT, RUND, MBLS, MICS, MTSK, MGSW];//ABLA, GZ9Z
}

protected func CheckEnemy()
{
// if(GetOwner() == -1)return;
 // Wenn keine Gegner da sind gibt FindObjects mit Find_Hostile 0 statt [] zurück... -> Enginbug!
 var crews;
 if(fActivePlayer || GetOwner()>=0) crews = FindObjects(Find_ID(SKLT),Find_Owner(-1),Find_Distance(LightningDis(),0,0));
 else crews = FindObjects(Find_OCF(OCF_CrewMember),Find_Not(Find_Owner(-1)),Find_Distance(LightningDis(),0,0));
 if(GetLength(crews))
 for(var clnk in crews)
 {
  //if(ObjectDistance(clnk,this)>LightningDis())continue;
  if(!GetAlive(clnk))continue;
  //if(!Hostile(GetOwner(),GetOwner(clnk)))continue;
  if(GetEffect("StrikeEnemy",clnk))continue;
  if(!PathFree(GetX(),GetY()-50,GetX(clnk),GetY(clnk)))continue;
  AddEffect("StrikeEnemy",clnk,20,33*5-iLightningSpeed,this);
 }
}

protected func ConvertGold()
{

 if(GetOwner() == -1)return;
 
 var nGold=3;
 var goldchunks=FindObjects(Find_ID(GOLD),Find_Distance(MaxGoldDis(),0,0));
 //überhaupt zu wenige da und so?
 if(GetLength(goldchunks)<nGold)return;
 
 //Gegner mögen wir nicht.
  for( var enem in FindObjects(Find_OCF(OCF_CrewMember)))
  if(Hostile(GetOwner(),GetOwner(enem)))
  if(ObjectDistance(enem,this)<MaxConquerorDis())
  return;
  
  
  for(var cnt=0;cnt<3;cnt++)
  {
   var gold=goldchunks[cnt];
   var x=GetX(gold)-GetX();
   var y=GetY(gold)-GetY();
   CastParticles("FSpark",4,10,x,y,50,100,RGB(200,200,20),RGB(255,255,200));
   CreateParticle("PSpark",x,y,0,0,60,RGB(255,255,50));
   Sound("Puff");
   RemoveObject(gold);
  } 
  
   CastParticles("FSpark",4,3,0,13,50,100,RGB(200,200,20),RGB(255,255,200));
   CreateParticle("PSpark",0,13,0,0,60,RGB(255,255,50));
   var ID;
   var IDcollection=[];
   //for(var cnt=0;ID=GetDefinition(cnt++, C4D_Object);cnt++)
   //IDcollection[GetLength(IDcollection)]=ID;
   
   IDcollection=GetConvertIDs();
   
   ID=IDcollection[Random(GetLength(IDcollection))];
   Schedule(Format("CreateObject(%i,0,16,GetOwner())",ID),5,0,this);
}

protected func GetConvertIDs()
{
 var aID=[BHLG,BHLG,LFAM,ALC_,BOOK,GBLT,MUSH,FLOU,ARMR,FARP];
 return aID;
}

protected func CheckConquerors()
{
 var found=-1;
 var hostile=-1;
 
 var crews=FindObjects(Find_OCF(OCF_CrewMember));
 for(var clnk in crews)// FindObjects(Find_OCF(OCF_CrewMember)))
 {
 
  if(Contained(clnk))continue;
  if(GetOwner()==GetOwner(clnk))continue;
  if(ObjectDistance(this,clnk)>MaxConquerorDis())continue;
  if(!PathFree(GetX(),GetY(),GetX(clnk),GetY(clnk)) 
  && !PathFree(GetX(),GetY()-5,GetX(clnk),GetY(clnk)) 
  && !PathFree(GetX()-15,GetY()-12,GetX(clnk),GetY(clnk)) 
  && !PathFree(GetX()+15,GetY()-12,GetX(clnk),GetY(clnk)))continue;
  if(GetPlayerName(GetOwner()))if(!Hostile(GetOwner(),GetOwner(clnk)))continue;
   if(!GetPlayerName(GetOwner(clnk)))continue;
   
   /*if(hostile != -1)
   if(Hostile(hostile,GetOwner(clnk))){found=-1;break;}
   hostile=GetOwner(clnk);*/
   
  for( var enem in crews)
  {
  
    if(!PathFree(GetX(),GetY(),GetX(enem),GetY(enem)) 
  && !PathFree(GetX(),GetY()-5,GetX(enem),GetY(enem)) 
  && !PathFree(GetX()-15,GetY()-12,GetX(enem),GetY(enem)) 
  && !PathFree(GetX()+15,GetY()-12,GetX(enem),GetY(enem)))continue;
  
  if(Hostile(GetOwner(clnk),GetOwner(enem)))
  if(ObjectDistance(enem,this)<MaxConquerorDis())
  {found=-1;return;}
  }
  if(found != -1)
  if(!Hostile(found,GetOwner(clnk))){continue;}
  
 
  found=GetOwner(clnk);
 
 }
 
 if(found != -1)
 AddEffect("ChangeOwner",this,20,2,this,0,FindObjectOwner(0,found,0,0,-1,-1,OCF_CrewMember));
 return 1;
}

protected func CheckHeal()
{
 if(GetOwner() == -1)return;
 var crews=FindObjects(Find_OCF(OCF_CrewMember),Find_Not(Find_Owner(-1)));
 
 for(var clnk in crews)
 {
  //if(Hostile(GetOwner(),GetOwner(clnk)))continue;
  if(GetEffect("SainthoodHealing",clnk))continue;
  if(GetEnergy(clnk) == GetPhysical("Energy",0,clnk)/1000)continue;
  AddEffect("SainthoodHealing",clnk,20,3,this,0,this);
 }
}

//Funktionen!
public func LightningDis(){return MaxConquerorDis();}
public func MaxGoldDis(){return 40;}
public func MaxHealDis(){return 100;}
public func MaxConquerorDis(){return 50;}
public func ConquerorTime(){return 1000;}

//Effekte!

//Gegner aua machen!
protected func FxStrikeEnemyTimer(object pTarget, int iEffectNumber,int iEffectTime)
{
  //Nicht weglaufen!1
 if(ObjectDistance(pTarget,this)>LightningDis())return -1;
 //Nicht weg sein!
 if(!pTarget || !GetAlive(pTarget))return -1;
 //Nicht verstecken!
 if(Contained(pTarget))return -1;
 //Nicht froindlich sein!
 if(GetOwner(this)==-1 && GetOwner(pTarget)==-1) return -1;
 if(GetOwner(this)>=0 && GetOwner(pTarget)>=0) return -1;
 
 //aua machen!
 for(var cnt=1+Random(3);cnt;cnt--)
 DrawLightning(GetX(),GetY()-50,GetX(pTarget)+RandomX(-4,4),GetY(pTarget)-4+RandomX(-4,4));
 Punch(pTarget,8+Random(3));
 //CastParticles("PxSpark",10,5,GetX(pTarget)-GetX(),GetY(pTarget)-GetY()-4,20,50,RGB(200,200,100),RGB(255,255,255));
 CreateParticle("PSpark",0,-50,0,0,250,RGBa(230,230,255,100));

 return 1;
}
//Teamanzeigen!
protected func FxTeamEffectTimer(object pTarget, int iEffectNumber,int iEffectTime)
{
 //tests
 if(!pTarget || !this)return -1;
 if(GetOwner(this) == -1)return 0;
 //Besitzer wechseln?
 if(GetOwner(this) != GetOwner(pTarget))
 SetOwner(GetOwner(this),pTarget);
 /*
 //Effekt
 // for(var cnt=0;cnt<2;cnt++)
 CreateParticle("PSpark",Cos(iEffectTime*2,10),Sin(iEffectTime*2,10)-50,0,0,25,RGBa(255,255,50,50),pTarget);
 //for(var cnt=0;cnt<2;cnt++)
 CreateParticle("PSpark",Cos(iEffectTime*2,10),-Sin(iEffectTime*2,10)-50,0,0,25,RGBa(255,255,50,50),pTarget);
 
 // for(var cnt=0;cnt<2;cnt++)
 CreateParticle("PSpark",Cos(iEffectTime*2-90,10),Sin(iEffectTime*2-90,10)-50,0,0,25,RGBa(255,255,50,50),pTarget);
// for(var cnt=0;cnt<2;cnt++)
 CreateParticle("PSpark",Cos(iEffectTime*2+90,10),-Sin(iEffectTime*2+90,10)-50,0,0,25,RGBa(255,255,50,50),pTarget);
 
 //  for(var cnt=0;cnt<2;cnt++)
 CreateParticle("PSpark",Cos(iEffectTime*2,10),Sin(iEffectTime/2,10)-50,0,0,25,RGBa(255,255,50,50),pTarget);
 //for(var cnt=0;cnt<2;cnt++)
 CreateParticle("PSpark",Cos(iEffectTime/2,10),-Sin(iEffectTime*2,10)-50,0,0,25,RGBa(255,255,50,50),pTarget);*/
 
 for(var cnt=0;cnt<360;cnt+=90)
 {
  CreateParticle("PSpark",Cos(cnt+iEffectTime%360,10),-50+Sin(cnt+iEffectTime%360,3),0,0,25,RGBa(255,255,50,100),pTarget);
  CreateParticle("PSpark",Cos(cnt+90+iEffectTime%360,3),-50+Sin(cnt+90+iEffectTime%360,10),0,0,25,RGBa(255,255,50,100),pTarget);
 }

 return 1;
}
protected func FxTeamEffectStop(object pTarget, int iEffectNumber)
 {
 if(pTarget)
 RemoveObject(pTarget);
}
//Heilen!
protected func FxSainthoodHealingStart(object pTarget, int iEffectNumber, int iTemp,  var1)
{
 EffectVar(0, pTarget, iEffectNumber)=var1;
}

protected func FxSainthoodHealingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
 //Nicht weglaufen!1
 if(ObjectDistance(pTarget,this)>MaxHealDis())return -1;
 //Nicht weg sein!
 if(!pTarget || !GetAlive(pTarget))return -1;
 //Nicht verstecken!
 if(Contained(pTarget))return -1;
 //Nicht kämpfen!
 if(GetProcedure(pTarget)=="FIGHT")return -1;
 //Nicht heil sein!
 if(GetEnergy(pTarget) == GetPhysical("Energy",0,pTarget)/1000)return -1;
 //Nicht feindlich sein!
 if(Hostile(GetOwner(this),GetOwner(pTarget)))return -1;
 //wee
 if(iEffectTime%21==0 && Random(10))DoEnergy(1,pTarget);
 
 //PArticlez!
 for(var cnt=5;cnt;cnt--)
 CreateParticle("PSpark",
 GetX(pTarget)-GetX()+RandomX(-6,6),GetY(pTarget)-GetY()+RandomX(-5,5),
 0,-1-Random(2),10,RGB(255,255,50),pTarget,Random(2));
 CreateParticle("PSpark",
 GetX(pTarget)-GetX(),GetY(pTarget)-GetY(),
 0,0,300,RGBa(255,255,50,150),EffectVar(0, pTarget, iEffectNumber));
 
 return 1;
}

//Einnehmen
protected func FxChangeOwnerStart(object pTarget, int iEffectNumber, int iTemp,  var1)
{
//Message("%d%s",0,GetOwner(var1),GetName(var1));
 EffectVar(0, pTarget, iEffectNumber)=var1;
 EffectVar(1, pTarget, iEffectNumber)=0;
 EffectVar(2, pTarget, iEffectNumber)=0;
}

protected func FxChangeOwnerTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
 
 //Bei zu grosser Entfernung einen kleinen Buffer gewaehren
 var tooFar=false;
 if(ObjectDistance(EffectVar(0, pTarget, iEffectNumber),this)>MaxConquerorDis())
 {
  if(!EffectVar(2, pTarget, iEffectNumber))
   EffectVar(2, pTarget, iEffectNumber) = iEffectTime;
  else
  {
   var iMaxTime=GameCall("SainthoodBufferTime");
   if(!iMaxTime)
    iMaxTime=35*5;
    
   if(iEffectTime - EffectVar(2, pTarget, iEffectNumber) > iMaxTime)
     {tooFar=true;}
  }
 }
 else
  if(EffectVar(2, pTarget, iEffectNumber))
   EffectVar(2, pTarget, iEffectNumber)=0;
  
 //Der Clonk darf nciht weglaufen
 if(!GetAlive(EffectVar(0, pTarget, iEffectNumber)) || Contained(EffectVar(0, pTarget, iEffectNumber)) || tooFar)
 {//und wenn, dann hat er sicher Froinde!
  var found=0;
  for(var clnk in FindObjects(Find_Allied(GetOwner(EffectVar(0, pTarget, iEffectNumber)))))
  {
   if(!(GetOCF(clnk)&OCF_CrewMember))continue;
   if(Contained(clnk))continue;
   if(ObjectDistance(this,clnk)<MaxConquerorDis()){EffectVar(0, pTarget, iEffectNumber)=clnk;found=1;break;}
  }
  //Nicht weitermachen, keine Froinde :C
  if(!found)
  return -1;
  }
 
 for(var cnt=Random(iEffectTime/50);cnt;cnt--)
 CreateParticle("PSpark",RandomX(-25,25),RandomX(-5,10),0,-1-Random(2),10,RGB(255,255,50),this);
 
 if(iEffectTime>ConquerorTime())
 {
  SetOwner(GetOwner(EffectVar(0, pTarget, iEffectNumber)),this);
  //CastParticles("PSpark",40,40,0,0,30,70,RGB(150,150,200),RGB(200,200,255),this);
  
   for(var cnt=Random(iEffectTime/20);cnt;cnt--)
 CreateParticle("PSpark",RandomX(-25,25),RandomX(-5,10),0,-1-Random(3),18,RGB(255,255,50),this);
  return -1;
 }
 
 //alles toll!
 return 1;
}

protected func FxChangeOwnerStop (object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
 //Wui effekt!1
}





