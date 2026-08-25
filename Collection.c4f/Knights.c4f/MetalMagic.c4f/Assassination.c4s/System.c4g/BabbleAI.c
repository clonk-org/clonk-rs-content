/*-- Blabla --*/

#strict 2

static const BAI_enemySight="Seen";
static const BAI_enemySightA=6;

static BAI_init;

static Seen1,Seen2,Seen3,Seen4,Seen5,Seen6;
static AlchemMage1,AlchemMage2,AlchemMage3,AlchemMage4,AlchemMage5,AlchemMage6;
static AlchemClonk1,AlchemClonk2,AlchemClonk3,AlchemClonk4,AlchemClonk5;
static LaborSorc1,LaborSorc2,LaborSorc3,LaborSorc4,LaborSorc5;
static King1,King2,King3,King4,King5;
static NextToKing1,NextToKing2,NextToKing3,NextToKing4,NextToKing5;
static ArmoryKnight1,ArmoryKnight2,ArmoryKnight3,ArmoryKnight4,ArmoryKnight5;
static KitchenSorc1,KitchenSorc2,KitchenSorc3,KitchenSorc4,KitchenSorc5;
static WaterClonk1,WaterClonk2,WaterClonk3,WaterClonk4,WaterClonk5,WaterClonk6;
static WorkshopClonk1,WorkshopClonk2,WorkshopClonk3,WorkshopClonk4,WorkshopClonk5;
static BedroomClonk1,BedroomClonk2,BedroomClonk3,BedroomClonk4,BedroomClonk5;
static WebClonk1,WebClonk2,WebClonk3,WebClonk4,WebClonk5;
static LibClonk1,LibClonk2,LibClonk3,LibClonk4,LibClonk5;
static WatchClonk1,WatchClonk2,WatchClonk3,WatchClonk4,WatchClonk5;
static MageDom1,MageDom2,MageDom3,MageDom4,MageDom5,MageDom6;
static ConfessFather1,ConfessFather2,ConfessFather3,ConfessFather4,ConfessFather5;

global func InitBAI()
{
 BAI_init=true;
 
 Seen1="$Seen1$";
 Seen2="$Seen2$";
 Seen3="$Seen3$";
 Seen4="$Seen4$";
 Seen5="$Seen5$";
 Seen6="$Seen6$";
 
 AlchemMage1="$AlchemMage1$";
 AlchemMage2="$AlchemMage2$";
 AlchemMage3="$AlchemMage3$";
 AlchemMage4="$AlchemMage4$";
 AlchemMage5="$AlchemMage5$";
 AlchemMage6="$AlchemMage6$";
 
 AlchemClonk1="$AlchemClonk1$";
 AlchemClonk2="$AlchemClonk2$";
 AlchemClonk3="$AlchemClonk3$";
 AlchemClonk4="$AlchemClonk4$";
 AlchemClonk5="$AlchemClonk5$";
 
 LaborSorc1="$LaborSorc1$";
 LaborSorc2="$LaborSorc2$";
 LaborSorc3="$LaborSorc3$";
 LaborSorc4="$LaborSorc4$";
 LaborSorc5="$LaborSorc5$";
 
 King1="$King1$";
 King2="$King2$";
 King3="$King3$";
 King4="$King4$";
 King5="$King5$";
 
 NextToKing1="$NextToKing1$";
 NextToKing2="$NextToKing2$";
 NextToKing3="$NextToKing3$";
 NextToKing4="$NextToKing4$";
 NextToKing5="$NextToKing5$";
 
 ArmoryKnight1="$ArmoryKnight1$";
 ArmoryKnight2="$ArmoryKnight2$";
 ArmoryKnight3="$ArmoryKnight3$";
 ArmoryKnight4="$ArmoryKnight4$";
 ArmoryKnight5="$ArmoryKnight5$";
 
 KitchenSorc1="$KitchenSorc1$";
 KitchenSorc2="$KitchenSorc2$";
 KitchenSorc3="$KitchenSorc3$";
 KitchenSorc4="$KitchenSorc4$";
 KitchenSorc5="$KitchenSorc5$";

 WaterClonk1="$WaterClonk1$";
 WaterClonk2="$WaterClonk2$";
 WaterClonk3="$WaterClonk3$";
 WaterClonk4="$WaterClonk4$";
 WaterClonk5="$WaterClonk5$";
 WaterClonk6="$WaterClonk6$";
 
 WorkshopClonk1="$WorkshopClonk1$";
 WorkshopClonk2="$WorkshopClonk2$";
 WorkshopClonk3="$WorkshopClonk3$";
 WorkshopClonk4="$WorkshopClonk4$";
 WorkshopClonk5="$WorkshopClonk5$";
 
 BedroomClonk1="$BedroomClonk1$";
 BedroomClonk2="$BedroomClonk2$";
 BedroomClonk3="$BedroomClonk3$";
 BedroomClonk4="$BedroomClonk4$";
 BedroomClonk5="$BedroomClonk5$";
 
 WebClonk1="$WebClonk1$";
 WebClonk2="$WebClonk2$";
 WebClonk3="$WebClonk3$";
 WebClonk4="$WebClonk4$";
 WebClonk5="$WebClonk5$";
 
 LibClonk1="$LibClonk1$";
 LibClonk2="$LibClonk2$";
 LibClonk3="$LibClonk3$";
 LibClonk4="$LibClonk4$";
 LibClonk5="$LibClonk5$";
 
 WatchClonk1="$WatchClonk1$";
 WatchClonk2="$WatchClonk2$";
 WatchClonk3="$WatchClonk3$";
 WatchClonk4="$WatchClonk4$";
 WatchClonk5="$WatchClonk5$";
 
 MageDom1="$MageDom1$";
 MageDom2="$MageDom2$";
 MageDom3="$MageDom3$";
 MageDom4="$MageDom4$";
 MageDom5="$MageDom5$";
 MageDom6="$MageDom6$";
 
 ConfessFather1="$ConfessFather1$";
 ConfessFather2="$ConfessFather2$";
 ConfessFather3="$ConfessFather3$";
 ConfessFather4="$ConfessFather4$";
 ConfessFather5="$ConfessFather5$";
}

global func AddBabbleAI(pTarget,sKind,iAmount)
{
 if(!BAI_init)InitBAI();
 AddEffect("BabbleSchedule",pTarget,20,6*(1+Random(20)),0,0,sKind,iAmount);
 
 if(sKind == "AlchemMage")
  AddEffect("MageMushroom",pTarget,20,25,0,0,0);
  
 return pTarget;
}

global func FxBabbleScheduleStart(pTarget,iEffectNumber,iTemp,var1,var2)
{
 if(iTemp)return;
 EffectVar(0,pTarget,iEffectNumber)=var1;
 EffectVar(1,pTarget,iEffectNumber)=var2;
}

global func FxBabbleScheduleTimer(pTarget,iEffectNumber)
{
 AddEffect("BabbleAI",pTarget,20,30,0,0,EffectVar(0,pTarget,iEffectNumber),EffectVar(1,pTarget,iEffectNumber));
 return -1;
}

global func FxBabbleAIStart(object pTarget, int iEffectNumber, int iTemp, var1, var2)
{
 if(iTemp)return;
 //Art
 EffectVar(0,pTarget,iEffectNumber)=var1;
 //Anzahl
 EffectVar(1,pTarget,iEffectNumber)=var2;
 //Timer
 EffectVar(2,pTarget,iEffectNumber)=0;
}

global func FxBabbleAITimer(pTarget,iEffectNumber,iEffectTime)
{ 

 for(var clnk in FindObjects(Find_Distance(150,GetX(pTarget),GetY(pTarget)),Find_OCF(OCF_CrewMember),Find_NoContainer()))
 {
  if(!GetPlayerName(GetOwner(clnk)))continue;
  
  //Fuer Sichtverfolgung
  var dir=DIR_Right;
  if(GetX(clnk) < GetX(pTarget))dir=DIR_Left;
  SetDir(dir,pTarget);
  
  if(iEffectTime-EffectVar(2,pTarget,iEffectNumber) < 35*8 && EffectVar(0,pTarget,iEffectNumber) != 0)return 0;
  
  //Rest
  if(!PathFree(GetX(pTarget),GetY(pTarget),GetX(clnk),GetY(clnk)))continue;
  if(GetPlayerTeam(GetOwner(clnk)) == 1)DoBabble(pTarget,EffectVar(0,pTarget,iEffectNumber),EffectVar(1,pTarget,iEffectNumber));
  else
  if(GetPlayerTeam(GetOwner(clnk)) == 2)DoBabble(pTarget,BAI_enemySight,BAI_enemySightA);
    
  if(!Random(4))
  {
   ObjectSetAction(pTarget,"Jump");
   SetPosition(GetX(pTarget),GetY(pTarget)-1,pTarget);
   SetXDir(0,pTarget);
   SetYDir(-(20+Random(10)),pTarget);
  }
  
  EffectVar(2,pTarget,iEffectNumber)=iEffectTime;
  
  break;
 }
 
 return 1;
}


global func DoBabble(pTarget,sKind,iAm)
{
 var iColor=GetColorDw(pTarget);
 BrightenColor(iColor);
 Message("<c %x><%s> %s",pTarget,iColor,GetName(pTarget),GlobalN(Format("%s%d",sKind,Random(iAm)+1)));
 return 1;
}

global func BrightenColor(&iColor)
{
 var r,g,b,a;
 SplitRGBaValue(iColor,r,g,b,a);
 if(r+g+b > 400 && !Max(Max(r,g),b) >= 210)return;
 
 var iMax=Max(Max(r,g),b);
  
 if(iMax == r)
 {
  var iFac1=(r*1000)/g;
  var iFac2=(r*1000)/b;
  r=210;
  g=(r*1000)/iFac1;
  b=(r*1000)/iFac2;
  iColor=RGBa(r,g,b,a);
  return 1;
 }
 
 if(iMax == g)
 {
  var iFac1=(r*1000)/r;
  var iFac2=(r*1000)/b;
  g=210;
  r=(g*1000)/iFac1;
  b=(g*1000)/iFac2;
  iColor=RGBa(r,g,b,a);
  return 1;
 }
 
 if(iMax == b)
 {
  var iFac1=(r*1000)/r;
  var iFac2=(r*1000)/g;
  b=210;
  r=(b*1000)/iFac1;
  g=(b*1000)/iFac2;
  iColor=RGBa(r,g,b,a);
  return 1;
 }
}

global func FxMageMushroomStart()
{

}

global func FxMageMushroomTimer(pTarget,iEffectNumber,iEffectTime)
{
 var mush=0;
 if(!GetEffect("MageSmoke",pTarget))
 if(mush=FindObject2(Find_Container(pTarget),Find_ID(MUSH)))
 {
  AddEffect("MageSmoke",pTarget,20,1,0,0);
  if(mush)
   RemoveObject(mush);
 }
}

global func FxMageSmokeTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(iEffectTime > 90)return -1;
 Smoke(GetX(pTarget),GetY(pTarget)-8,5+Random(4));
 Message("<c %x><%s> HIHIHI",pTarget,GetColorDw(pTarget),GetName(pTarget));
}

