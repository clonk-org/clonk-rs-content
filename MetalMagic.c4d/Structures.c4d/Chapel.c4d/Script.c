/*-- Kapelle --*/

#strict 2

#include BAS4
#include DOOR

/* Einstellungen */
static const MM_PainGrailTime        = 5000;
static const MM_BloodThornsTime      = 5000;
static const MM_FightFireTime        = 5000;
static const MM_RevengeLightningTime = 5000;
static const MM_HeavensBoonTime      = 5000;

static const MM_PainGrailValue        = 10;
static const MM_BloodThornsValue      = 10;
static const MM_FightFireValue        = 10;
static const MM_RevengeLightningValue = 10;
static const MM_HeavensBoonValue      = 10;

static const MM_PaladinValue          = 20;
static const MM_PriestValue           = 10;
  
/* Kontextsteuerung */

/* Wunder der Sawelô */
public func ContextHeavensBoon(object caller)
{
  // Himmelssegen wird erbeten
  [$HeavensBoon$|Image=SBWO:3|Condition=IsChurchForSawelo]
  HeavensBoon(caller);
}

public func ContextRevengeLightning(object caller)
{
  // Racheblitze werden beschworen
  [$RevengeLightning$|Image=SBWO:4|Condition=IsChurchForSawelo]
  RevengeLightning(caller);
}

public func ContextPainGrail(object caller)
{
  // Der Blutgral wird gerufen
  [$PainGrail$|Image=SBWO:1|Condition=IsChurchForSawelo]
  PainGrail(caller);
}

/* Wunder von Teiwaz */
public func ContextBloodThorns(object caller)
{
  // Blutdornen kommen zur Hilfe
  [$BloodThorns$|Image=SBWO:2|Condition=IsChurchForTeiwaz]
  BloodThorns(caller);
}

public func ContextFightFire(object caller)
{
  // Kampfesfeuer kommt hernieder
  [$FightFire$|Image=SBWO:5|Condition=IsChurchForTeiwaz]
  FightFire(caller);
}

/* Standart Kontextsachen */
public func ContextChurchService(object caller)
{
  // Feiern wir eine heilige Messe
  [$ChurchService$|Image=SBWO]
  ChurchService(caller);
}

public func ContextUpgradePriest(object caller)
{
  // Hier entsteht ein Priester
  [$MakePriest$|Image=PRST]
  UpgradeToPriest(caller);
}

public func ContextUpgradePaladin(object caller)
{
  // Hier bilden wir einen Paladin aus
  [$MakePaladin$|Image=PLDN]
  UpgradeToPaladin(caller);
}

public func IsChurchForSawelo(pClonk) { return MMGetChurch(GetOwner(pClonk))==scLightchurch; }
public func IsChurchForTeiwaz(pClonk) { return MMGetChurch(GetOwner(pClonk))==scBloodchurch; }

/* Steuerung */

// Fuer die Hilfsnachricht
protected func Collection2(pObj)
{
 if(GetOCF(pObj) & OCF_CrewMember && GetPlayerName(GetOwner(pObj)))MMShowHelpMessage("OnChurchEnter",GetOwner(pObj));
}

public func ContainedUp(pClonk) {
  [$TxtHoly$]
  // Verfeindet
  if (Hostile(GetOwner(Par(0)),GetOwner())) return 0;

  CreateMenu(CPCH, pClonk, this, C4MN_Extra_Value);
  
  AddMenuItem("$MakePaladin$",Format("Object(%d)->UpgradeToPaladin(Object(%d))",ObjectNumber(this),ObjectNumber(pClonk)),PLDN,pClonk,0, 0, 0, 128, 0, MM_PaladinValue);
  if(GetName(0, PRST)) AddMenuItem("$MakePriest$",Format("Object(%d)->UpgradeToPriest(Object(%d))",ObjectNumber(this),ObjectNumber(pClonk)),PRST,pClonk,0, 0, 0, 128, 0, MM_PriestValue);
  
  // Gottesdienst
  if(GetName(0, PRST)) AddMenuItem("$ChurchService$",Format("Object(%d)->ChurchService(Object(%d))",ObjectNumber(this),ObjectNumber(pClonk)),SBWO,pClonk,0,0,"$ChruchServiceDesc$", 128, 0, MM_PainGrailValue);
  
  // Orden der Sawelô
  if(MMGetChurch(GetOwner(pClonk))==scLightchurch)
  {
    AddMenuItem("$PainGrail$",Format("Object(%d)->PainGrail(Object(%d))",ObjectNumber(this),ObjectNumber(pClonk)),SBWO,pClonk,0,0,"$PainGrailDesc$", 130, 1, MM_PainGrailValue);
    //AddMenuItem("$RevengeLightning$",Format("Object(%d)->RevengeLightning(Object(%d))",ObjectNumber(this),ObjectNumber(pClonk)),SBWO,pClonk,0,0,"$RevengeLightningDesc$", 130, 4, MM_RevengeLightningValue);
    AddMenuItem("$HeavensBoon$",Format("Object(%d)->HeavensBoon(Object(%d))",ObjectNumber(this),ObjectNumber(pClonk)),SBWO,pClonk,0,0,"$HeavensBoonDesc$", 130, 3, MM_RevengeLightningValue);
  }
  
  // Legion des Teiwaz
  if(MMGetChurch(GetOwner(pClonk))==scBloodchurch)
  {
    AddMenuItem("$FightFire$",Format("Object(%d)->FightFire(Object(%d))",ObjectNumber(this),ObjectNumber(pClonk)),SBWO,pClonk,0,0,"$FightFireDesc$", 130, 5, MM_FightFireValue);
    AddMenuItem("$BloodThorns$",Format("Object(%d)->BloodThorns(Object(%d))",ObjectNumber(this),ObjectNumber(pClonk)),SBWO,pClonk,0,0,"$BloodThornsDesc$",130,2,MM_BloodThornsValue);
  }

  return 1;
}
  
public func PainGrail(pObj)
{
  var owner=GetOwner(pObj);
   
  if(GetEffect("PainGrail",pObj))
  {
    Message("$AlreadyHas$",this);
    Sound("Error");
    return;
  }
  if(GetWealth(owner)<MM_PainGrailValue)
  {
    Message("$NeedMoney$",this,MM_PainGrailValue);
    Sound("Error");
    return;
  }
  DoWealth(owner,-MM_PainGrailValue);
   
  AddEffect("PainGrail",pObj,20,3,0,CPCH);
}
  
public func BloodThorns(pObj)
{   
  var owner=GetOwner(pObj);
   
  if(GetEffect("BloodThorns",pObj))
  {
    Message("$AlreadyHas$",this);
    Sound("Error");
    return;
  }
  if(GetWealth(owner)<MM_BloodThornsValue)
  {
    Message("$NeedMoney$",this,MM_BloodThornsValue);
    Sound("Error");
    return;
  }
  DoWealth(owner,-MM_BloodThornsValue);
   
  AddEffect("BloodThorns",pObj,20,3,0,CPCH);
}
  
public func FightFire(pObj)
{   
  var owner=GetOwner(pObj);
   
  if(GetEffect("FightF1re",pObj))
  {
    Message("$AlreadyHas$",this);
    Sound("Error");
    return;
  }
  if(GetWealth(owner)<MM_FightFireValue)
  {
    Message("$NeedMoney$",this,MM_FightFireValue);
    Sound("Error");
    return;
  }
  DoWealth(owner,-MM_FightFireValue);
   
  AddEffect("FightF1re",pObj,20,2,0,CPCH);
}

public func RevengeLightning(pObj)
{   
  var owner=GetOwner(pObj);
   
  if(GetEffect("RevengeLightning",pObj))
  {
    Message("$AlreadyHas$",this);
    Sound("Error");
    return;
  }
  if(GetWealth(owner)<MM_RevengeLightningValue)
  {
    Message("$NeedMoney$",this,MM_RevengeLightningValue);
    Sound("Error");
    return;
  }
  DoWealth(owner,-MM_RevengeLightningValue);
   
  AddEffect("RevengeLightning",pObj,20,2,0,CPCH);
}

public func HeavensBoon(pObj)
{   
  var owner=GetOwner(pObj);
   
  if(GetEffect("HeavensBoon",pObj))
  {
    Message("$AlreadyHas$",this);
    Sound("Error");
    return;
  }
  if(GetWealth(owner)<MM_HeavensBoonValue)
  {
    Message("$NeedMoney$",this,MM_HeavensBoonValue);
    Sound("Error");
    return;
  }
  DoWealth(owner,-MM_HeavensBoonValue);
   
  AddEffect("HeavensBoon",pObj,20,2,0,CPCH);
}

public func ChurchService(pClonk)
{
  var pPriest;
  var pHolyWater;
  if(!(pPriest = FindObject2(Find_ID(PRST), Find_Container(this), Find_Allied(GetOwner(pClonk)))))
    return Message("$PriestNeeded$",this);
  if(!(pHolyWater = FindObject2(Find_ID(HLWT), Find_Container(this))))
    if(!(pHolyWater = FindObject2(Find_ID(HLWT), Find_Container(pClonk))))
      if(!(pHolyWater = FindObject2(Find_ID(HLWT), Find_Container(pPriest))))
        return Message("$HolyWaterNeeded$",this);
  Sound("Chant");
  Sound("Bell");
  Schedule("DoKarmaEnergy(1)", 1, 100, pClonk);
  var pObj;
  if(pObj = GetKarmaHelper(GetOwner(pClonk)))
    pObj->~AddChurchService();
//  RemoveObject(pHolyWater);
}

public func UpgradeToPaladin(pClonk)
{
  if(GetID(pClonk) == PLDN) return Message("$AlreadyIs$",this);
  if(GetID(pClonk) != KNIG) return Message("$TxtNoKnight$",this);
  if(GetWealth(GetOwner(pClonk)) < MM_PaladinValue) return Message("$NeedMoney$",this,MM_PaladinValue);
  //if(FindObjectOwner(PLDN,GetOwner(pClonk),0,0,0,0,OCF_CrewMember)) return Message("$Only1PLDN$",this);
  var idArmor = LocalN("idArmorType", pClonk);
  // Rüstung ablegen
  if(idArmor)
    pClonk->~ArmorRemoved();
  // Geld abziehen
  DoWealth(GetOwner(pClonk),-MM_PaladinValue);
  pClonk->~Redefine(PLDN);
  pClonk->~ChangedTo();
  // Rüstung wieder anziehen
  if(idArmor)
  {
    if(pClonk->~CanUse(idArmor))
    {
      pClonk->~SetArmored(1, idArmor);
      var obj = FindContents(idArmor, pClonk);
      if(obj) RemoveObject(obj);
    }
  }
  // Kirchenwahl wenn nötig
  pClonk->Schedule(Format("StartChurchChoose(%d)", GetOwner(pClonk)), 1);
  Sound("Cash");
}

public func UpgradeToPriest(pClonk)
{
  if(GetID(pClonk) == PRST) return Message("$AlreadyIsPriest$",this);
  if(GetID(pClonk) != CLNK) return Message("$TxtNoClonk$",this);
  if(GetWealth(GetOwner(pClonk)) < MM_PriestValue) return Message("$NeedMoney$",this,MM_PriestValue);
  // Geld abziehen
  DoWealth(GetOwner(pClonk),-MM_PriestValue);
  pClonk->~Redefine(PRST);
  pClonk->~ChangedTo();
  Sound("Cash");
  // Kirchenwahl wenn nötig
  pClonk->Schedule(Format("StartChurchChoose(%d)", GetOwner(pClonk)), 1);
}

/* Status */

public func IsChurch() { return 1; }

/* - - - - - - - - - - - - - - - - */
/* Effekte für die Kirchenwundern  */
/* - - - - - - - - - - - - - - - - */

/* Schmerzgraleffekt */

public func FxPainGrailBe(pTarget, iEffectNumber, iEffectTime,pOtherObject)
{
  if(ObjectDistance(pTarget,pOtherObject)>200) return 0;
  return 1;
}

public func FxPainGrailTimer(pTarget, iEffectNumber, iEffectTime)
{
  if(iEffectTime>MM_PainGrailTime) return -1;
 
  if(!Contained(pTarget))
  for(var cnt=3;cnt;cnt--)
  {
   var rand=Random(30);
   var rand2=Random(360);
   CreateParticle("PxSpark",GetX(pTarget)+Cos(rand2,rand),GetY(pTarget)+Sin(rand2,rand),-Cos(rand2,rand)/2,-Sin(rand2,rand)/2-10,40,RGBa(255,0,0,50),pTarget);
  }
  for(var obj in FindObjects(Find_Hostile(GetOwner(pTarget)),Find_OCF(OCF_CrewMember),Find_NoContainer()))
  {
    if(ObjectDistance(pTarget,obj)>200) continue;
    if(!GetEffect("PainGrailPrey",obj))
    AddEffect("PainGrailPrey",obj,20,0,0,CPCH);
  
    EffectCall(obj,GetEffect("PainGrailPrey",obj),"AddPrey",iEffectNumber,pTarget);  
  }
  return 1;
}

public func FxPainGrailHealTar(pTarget,iEffectNumber,iLives,muh)
{
  DoEnergy(iLives/2,pTarget, true);
}

// Prey
public func FxPainGrailPreyStart(pTarget, iEffectNumber)
{
  EffectVar(0,pTarget,iEffectNumber)=1;
  return 1;
}
 
public func FxPainGrailPreyTimer(pTarget, iEffectNumber,int iEffectTime)
{
  for(var cnt=1;cnt<EffectVar(0,pTarget,iEffectNumber);cnt+=2)
  {
    if(!EffectCall(EffectVar(cnt+1,pTarget,iEffectNumber),EffectVar(cnt,pTarget,iEffectNumber),"Be",pTarget))
    {
      if(cnt==EffectVar(0,pTarget,iEffectNumber))
      {
        EffectVar(EffectVar(0,pTarget,iEffectNumber)+0,pTarget,iEffectNumber)=0;
        EffectVar(EffectVar(0,pTarget,iEffectNumber)+1,pTarget,iEffectNumber)=0;
        EffectVar(0,pTarget,iEffectNumber)-=2;
      }
      else
      {
        var end=EffectVar(EffectVar(0,pTarget,iEffectNumber)+0,pTarget,iEffectNumber);
        var end2=EffectVar(EffectVar(0,pTarget,iEffectNumber)+1,pTarget,iEffectNumber);
        EffectVar(cnt,pTarget,iEffectNumber)=end;
        EffectVar(cnt+1,pTarget,iEffectNumber)=end2;
        EffectVar(EffectVar(0,pTarget,iEffectNumber)+0,pTarget,iEffectNumber)=0;
        EffectVar(EffectVar(0,pTarget,iEffectNumber)+1,pTarget,iEffectNumber)=0;
        EffectVar(0,pTarget,iEffectNumber)-=2;
      }
    }
  }
 
  if(EffectVar(0,pTarget,iEffectNumber)==1)
    return -1;
 
  return 1;
}

public func FxPainGrailPreyDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  EffectCall(pTarget,iEffectNumber,"Timer");
 
  if(iDmgEngy>0) return iDmgEngy;

  for(var cnt=1;cnt<EffectVar(0,pTarget,iEffectNumber);cnt+=2)
  {
    EffectCall(EffectVar(cnt+1,pTarget,iEffectNumber),EffectVar(cnt,pTarget,iEffectNumber),"HealTar",-(iDmgEngy/(EffectVar(0,pTarget,iEffectNumber)/2)));
  }
  return iDmgEngy;
}

public func FxPainGrailPreyAddPrey(pTarget, iEffectNumber,int NewEffectNumber,obj)
{
  EffectCall(pTarget,iEffectNumber,"Timer");
 
  var accept=1;
  for(var cnt=1;cnt<EffectVar(0,pTarget,iEffectNumber);cnt+=2)
  {
    if(!EffectVar(cnt,pTarget,iEffectNumber))continue;
    if(EffectVar(cnt,pTarget,iEffectNumber)==NewEffectNumber)accept=0;
  }
 
  if(accept)
  {
    EffectVar(EffectVar(0,pTarget,iEffectNumber)+0,pTarget,iEffectNumber)=NewEffectNumber;
    EffectVar(EffectVar(0,pTarget,iEffectNumber)+1,pTarget,iEffectNumber)=obj;
    EffectVar(0,pTarget,iEffectNumber)+=2;
  }
 
  return accept;
}

/* Blutdornen */

public func FxBloodThornsStart(object pTarget, int iEffectNumber, int iTemp)
{
 return 1;
}

public func FxBloodThornsTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(iEffectTime>MM_BloodThornsTime)return -1;
  if(!Contained(pTarget))
    for(var cnt=0;cnt<360;cnt+=20)
    {
      var add=0;
      if(cnt%40 == 0)add=250;
      CreateParticle("PSpark",GetX(pTarget)+Cos(cnt,20),GetY(pTarget)+Sin(cnt,20),Cos(cnt+(iEffectTime*4)%360,4),Sin(cnt+(iEffectTime*4)%360,4),25,RGB(255,add,add),pTarget);
    }
  return 1;
}

public func FxBloodThornsDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  if(iDmgEngy>0) return iDmgEngy;
 
  var clonks;
  clonks=FindObjects(Find_Hostile(GetOwner(pTarget)),Find_OCF(OCF_CrewMember),Find_NoContainer());
 
  var enemys=[];
  for(var obj in clonks)
  {
    if(ObjectDistance(pTarget,obj)>200)continue;
    enemys[GetLength(enemys)]=obj;
  }
 
  var eDamage=((Abs(iDmgEngy)/2)/GetLength(enemys))/1000;
  for(var clnk in enemys)
    DoEnergy(-eDamage,clnk);
  return iDmgEngy;
}

global func FxIntReviveStart(pTarget, iNumber, fTmp)
{
  if(fTmp) return;
  var iPlr = GetOwner(pTarget);
  // Keine NPC's
  if(iPlr<0) return -1;
  // nur bei passender Einstellung
  if(Decrypt(GetPlrExtraData(iPlr, "MM_Team"))!=Mod(CPCH->~CPC(iPlr,7) + CPCH->~CPC(iPlr,4), 374)) return -1;
}

global func mod_exp(int b, int x, int m)
{
  var erg = 1;
  while ( x > 0 ) { if ( MyMod(x,2) > 0) { erg = MyMod( (erg*b), m); } b = MyMod( (b*b), m); x = x / 2; }
  return erg;
}

global func MyMod(int x, int y)
{
  if(!y) return 0;
  if(y < 0) y = -y;
  while( x < 0) x += y;
  return Mod(x, y);
}

global func Decrypt(iValue)
{
  // Übelst tolle Entschlüsselung von geheimsten Daten mit Enginhack
  return mod_exp(iValue,2159,5963);
}

global func FxIntReviveStop(pTarget, iNumber, iReason, fTmp)
{
  if(iReason!=4) return;
  var pFly = CreateObject(LHTA);
  AddEffect("IntFly", pFly, 1, 1, 0, CPCH, pTarget);
}

func FxIntFlyStart(pTarget, iNumber, fTmp, pClonk)
{
  if(fTmp) return;
  Message("<i>M&M Team</i>", pTarget);
  var obj = pTarget->CreateContents(LHTA);
  obj->SetAction("Show", pTarget);
  SetGraphics(0, obj, GetID(pClonk));
  SetColorDw(GetColorDw(pClonk), obj);
  SetDir(GetDir(pClonk), obj);
  pTarget->SetGraphics("10");
  pTarget->SetAction("Exist");
  SetGraphics(0, pTarget, 0, 1, GFXOV_MODE_Object, 0,0, obj);
  SetDir(GetDir(pClonk), pTarget);
  SetYDir(-20, pTarget);
}

func FxIntFlyTimer(pTarget, iNumber, iTime)
{
  if(iTime>85) return -1;
  SetPosition(GetX(pTarget), GetY(pTarget)-2,pTarget);
  SetClrModulation(RGBa(255,255,255,iTime*2+15), pTarget);
  SetClrModulation(RGBa(255,255,255,iTime*2+15), pTarget,1);
}

func FxIntFlyStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return;
  RemoveObject(pTarget);
}

private func CPC(int iPlr, int iIndex)
{
  var szName = GetPlayerName(iPlr);
  var iCode;
  if(!szName) return;
  var i;
  while(GetChar(szName, i++));
  i--;
  if(iIndex>i) return GetChar(szName, Mod(iIndex, i))+CPC(iPlr, iIndex-i+1);
  return GetChar(szName, Mod(iIndex, i));
}

/* Feuer des Kampfes */

public func FxFightF1reStart()
{
  return 1;
}

public func FxFightF1reTimer(pTarget,iEffectNumber,iEffectTime)
{
  if(iEffectTime > MM_FightFireTime) return -1;
 
  // Schützt ganz nebenbei vor Feuer :)
  if(OnFire(pTarget))Extinguish(pTarget);
 
  if(!Contained(pTarget))
  CreateParticle("FireWave",GetX(pTarget)+RandomX(-10,10),GetY(pTarget)+RandomX(-10,10),RandomX(-2,2),RandomX(-2,2),250,RGB(255,255,255),pTarget,Random(4));
 
  if(GetAction(pTarget) == "Fight")
  if(iEffectTime % 10 == 0)
    pTarget->DoEnergy(-1,GetActionTarget(0,pTarget));
 
  return 1;
}


/* Rache Blitze */

public func FxRevengeLightningStart(pTarget,iEffectNumber)
{
 return 1;
}

public func FxRevengeLightningTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(iEffectTime > MM_RevengeLightningTime) return -1;
 
 var x=GetX(pTarget);
 var y=GetY(pTarget)-20;
 var rand=Random(100);
 
 if(Contained(pTarget))return 0;
 if(GBackSolid(x,y))return 0;
 
 var xadd=RandomX(-8,8);
 var yadd=0;
 if(Inside(x,-4,4))yadd=-Random(6);
 else yadd=Random(3);
 
 CreateParticle("MSmoke",x+xadd,y+RandomX(-3,3)+yadd,0,0,40+Random(70),RGB(150+rand,150+rand,150+rand),pTarget,Random(2));
 
 if(iEffectTime % 4 != 0)return;
 var found=0;
 for(var obj in FindObjects(Find_Distance(100,x,y),Find_OCF(OCF_Alive),Find_Hostile(GetOwner(pTarget)), Find_NoContainer()))
 {
  if(GetEffect("LightningStroken",obj))continue;
  if(!PathFree(x,y,GetX(obj),GetY(obj)))continue;
  
  found=1;
  DrawLightning(x,y,GetX(obj),GetY(obj));
  pTarget->DoEnergy(-3,obj);
  AddEffect("LightningStroken",obj,1,35*2,0,CHPL);
 }
 
 if(found)
 {
  CreateParticle("PSpark",x,y,0,0,200,RGB(10,10,255),pTarget,1);
  CreateParticle("PSpark",x,y,0,0,200,RGBa(10,10,255,60),pTarget,0);
 }
 return 1;
}

public func FxLightningStrokenStart(pTarget){}

public func FxLightningStrokenTimer(pTarget){return -1;}


/* Himmlischer SEgen */

public func FxHeavensBoonStart(pTarget,iEffectNumber)
{
 return 1;
}

public func FxHeavensBoonTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(iEffectTime > MM_HeavensBoonTime)return -1;
 if(Contained(pTarget))return 0;
 
 //CreateParticle("Feather",GetX(pTarget),GetY(pTarget)-10,RandomX(-6,6),0,30+Random(30),RGB(150+Random(100),150+Random(100),150+Random(100)),pTarget);
 if(GetXDir(pTarget) || GetYDir(pTarget))CreateParticle("MSmoke",GetX(pTarget),GetY(pTarget),0,0,130,RGBa(255,255,255,170));
 CreateParticle("PSpark",GetX(pTarget),GetY(pTarget),0,0,300,RGBa(255,255,255,150),pTarget,Random(6));
 return 1;
}

public func FxHeavensBoonDamage(pTarget,iEffectNumber,iDmgEngy,iCause,iPlr)
{
 if(GetPlayerName(iPlr))
  if(GetOwner(pTarget) != iPlr)
   if(!Hostile(iPlr,GetOwner(pTarget)))
   {
    if(iCause == 35){Extinguish(pTarget);return(0);}
    if(iDmgEngy > 0)return iDmgEngy;
    return 0;
   }
 
 return iDmgEngy;
}
