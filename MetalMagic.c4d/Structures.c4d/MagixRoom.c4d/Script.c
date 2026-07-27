/* Magieraum */

#strict 2

#include WTWR //magieee!
#include CPWK // Burgwerkstatt
#include DOOR // Türsteuerung

local direction;// DIR_Left/DIR_Right
local convert_more;
/* Initialisierung */

protected func Initialize()
{
  // Verzögerte Initialisierung
  ScheduleCall(this,"Initialize2",3,0);
  // Objekte im Umkreis ihre Überprüfungen durchführen lassen
//Farbe für Animation
SetColorDw(RGB(230,230,230));

CreateObject(WDSK,-10,30,GetOwner());
 // return _inherited() ;
 
 // In den Hintergrund versetzen
  SetCategory(C4D_StaticBack);
  // Bei Fertigstellung an anderen Teilen ausrichten
  CastlePartAdjust();
  // Objekte im Umkreis ihre Überprüfungen durchführen lassen
  CastlePartInitialize();
  // Fertig
  return 1;
}

protected func Initialize2()
{
 SetDir(DIR_Right, CreateObject(TORC, 30, 35)->Activate() );
 AddEffect("ReloadMagicEnergy",this,20,350,this);
}

protected func FxReloadMagicEnergyStart()
{
  return 1;
}

protected func FxReloadMagicEnergyTimer(pTarget)
{
 if(GetCon(pTarget) < 100)return 1;
 DoMagicEnergy(1,pTarget);
 return 1;
}

/* Kampfmagier ausbilden */
public func TrainBattleMage(idDummy, pClonk, fRight, iValue) {
  
  // nur Zauberclonks sind aufwertbar
  if(GetID(pClonk) != SCLK) {PlayerMessage(GetOwner(pClonk),"$NeedMagiClonk$",this); return;}
  // Ist überhaupt genug Geld da?
  if(GetWealth(GetOwner(pClonk)) < iValue) return PlayerMessage(GetOwner(pClonk),"$NeedGold$",this,iValue);
  // verwandeln
  pClonk->Redefine2(MAGE);
  // Das Portrait anpassen
  SetPortrait("random", pClonk, MAGE);
  // Geld abziehen
  DoWealth(GetOwner(pClonk),-iValue);
  // Magus bekommt einen Stab geschenkt.
  if(ObjectCount(FSTF)) CreateContents(SCKZ, pClonk);
  LocalN("fInitialized", pClonk) = 1;
  // Aktion setzen
  SetAction("Magic");
  Sound("Magic*");
  return 1;
  }
  
protected func InitTrainMenu(pCaller)
{
  if(Hostile(GetOwner(),GetOwner(pCaller)))return 0;
  CreateMenu(MAGE,pCaller,this, C4MN_Extra_Value);
  AddMenuItem("$TxtTrainBattlemage$","TrainBattleMage",MAGE,pCaller, 0, pCaller, 0, 128, 0, 20);
  return 1;
}

/* Kontext */
public func ContextTrainMage(object caller)
{
  [$TxtTrainBattlemage$|Image=MAGE]
  TrainBattleMage(MAGE,caller);
}

protected func PhaseCallEnergize()
{
var std_x=-27;
var std_y=15;

var x;
var y;

for(var cnt=0;cnt<360;cnt+=3+RandomX(-1,1))
{
x=Cos(cnt,3);
y=Sin(cnt,3);
CreateParticle("PSpark",std_x+x,std_y+y,x,y,20,RGB(0,0,255),this);
}
}

/* Steuerung */

protected func ContainedUp(pCaller) 
{
  [$TxtMagicItem$|Image=CXCN]
  return MenuProduction(pCaller);
}

protected func ContainedDig(pCaller)
{
 [$TxtTrainBattlemage$|Image=MAGE]
 return InitTrainMenu(pCaller);
}

/* Produkteigenschaften (überladbar) */
func ProductType() { return 0; }
func ProductCondition() { return "IsWizardTowerProduct"; }

/* Produktion */
private func MenuProduction(pCaller) {
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(CXCN,pCaller,this,1,"$NoPlrKnowledge$");
  for(var i=0,idKnowledge; idKnowledge=GetPlrKnowledge(GetOwner(pCaller),0,i,C4D_Object); ++i)
  if(IsProducerOf(pCaller,idKnowledge))  AddMenuItem("$TxtSelectProduction$: %s", "SelectProduction", idKnowledge, pCaller, 0, pCaller);
  return 1;
}


public func IsProducerOf(caller, def) 
{
  
  if (!IsBuilt ()) return (0);
  if (!GetPlrKnowledge (GetOwner (caller), def)) return (0);

    if (!DefinitionCall (def, ProductCondition()))
      return (0);
  return 1;
}

protected func IsBuilt() {
  return GetCon() >= 100;
}

/*Zauberturm*/

// Inhalt prüfen: Kristalle verarbeiten
protected func ContentsCheck() 
{
  CheckBuild();

  for (var i=0,pContent; pContent=Contents(i); ++i)
    if (DefinitionCall(GetID(pContent),"IsLorry"))
      GrabContents(pContent);
  /*if (FindContents(CRYS)) 
    ConvertCrystal();*/
  return 1;
}
public func IsConvertReady() { return ActIdle() || GetAction() == "Ready"; }

public func SelectSpell(caller, int direction) 
{
  // Forschung abbrechen
  CancelResearch();
  // Nicht bereit
  //if (!ActIdle() && GetAction() != "Ready") return 0;
	//if(ActIdle()) SetAction("Ready");
  // Alchemieregel vorhanden?
  var iExtra;
  if (ObjectCount(ALCO)) 
    { if(ObjectCount(NMGE)) iExtra=1; else iExtra=5; }
  else                  
    { if(ObjectCount(NMGE)) iExtra=0; else iExtra=3; }
  // Menü erzeugen und mit den Zaubersprüchen des Turmbesitzers füllen
  CreateMenu(MCMS, caller, this, iExtra, "$TxtNoSpells$", GetMagicEnergy());  
  var spell_num;
  var spell_id;
  var iPlr = GetOwner(); if (!GetPlayerName(iPlr)) iPlr = GetOwner(caller);
  while (spell_id = GetPlrMagic(iPlr, 0, spell_num++)) 
    if(!spell_id->~NoTowerMagic() )
		{
			var strCommand = Format("Object(%d)->DoMagic(%i,Object(%d),%d)", ObjectNumber(this()), spell_id, ObjectNumber(caller), direction);
      AddMenuItem("$TxtDoMagic$", strCommand, spell_id, caller, 0, caller);
		}
  // Zauber erforschbar
  if (ObjectCount(WTRS) && (caller->Contained() == this)) 
  {
    // Neue Forschung starten
    AddMenuItem("$TxtResearchs$", "ResearchMenu", RSR6, caller, 0, caller);
    // Laufende Forschung fortsetzen
    if (target) AddMenuItem("$TxtContinuedevelopement$", "SelectResearch", target, caller, 0, caller);
  }
  AddMenuItem("$TxtConvertCrystal$","MenuConvertCrystal",CRYS,caller,0,caller,"$TxtSpecial2$");
  return 1;
}

protected func MenuConvertCrystal(ID,caller,special2)
{
if(!FindContents(CRYS))
{
PlayerMessage(GetOwner(caller),"$TxtNoCrystal$",this);
Sound("Error");
return;
}
ConvertCrystal();
if(special2)convert_more=1;
}

protected func ConvertNextCrystal()
{

if(convert_more && FindContents(CRYS) && GetMagicEnergy()!=100)return ConvertCrystal();
else{ convert_more=0;SetAction("Idle");}
}

public func ContainedRight(clnk)
{

if(GetDir()!=DIR_Right)
{SetDir(DIR_Right);Sound("Energize");}
if(ActIdle() || GetAction() == "Ready")CreateParticle("Arrow",-22,15,5,0,100,RGB(255,255,255),this);
if(ActIdle() || GetAction() == "Ready")SelectSpell(clnk, 1);
return 1;
}

public func ContainedLeft(clnk)
{

if(GetDir()!=DIR_Left)
{SetDir(DIR_Left);Sound("Energize");}
if(ActIdle() || GetAction() == "Ready") CreateParticle("Arrow",-22,14,-5,0,100,RGB(255,255,255),this);
if(ActIdle() || GetAction() == "Ready") SelectSpell(clnk, 0);
return 1;
}

// Fuer die Hilfsnachricht
protected func Collection2(pObj)
{
 if(GetOCF(pObj) & OCF_CrewMember && GetPlayerName(GetOwner(pObj)))MMShowHelpMessage("OnMagicRoomEnter",GetOwner(pObj));
}

public func SetDir(dir)
{
direction=dir;
return (direction);
}
public func GetDir()
{
return (direction);
}
/* Türsteuerung */

private func SoundOpenDoor()
{
  Sound("MagicElementEarth");
}

private func SoundCloseDoor()
{
  Sound("MagicElementAir");
}

/* Forschung */

public func GetResearchBase() { return CPW2; }


/*globales überladen von Funktionen :S*/

global func GetDir(obj)
{
if(GetID(obj)==CPMR)return LocalN("direction",obj);
else return _inherited(obj,Par(1),Par(2),Par(3));
}
global func SetDir(dir,obj)
{
if(GetID(obj)==CPMR){return LocalN("direction",obj)=dir;}
else return _inherited(dir,obj,Par(2),Par(3),Par(4));
}

private func CloseEntrance()
{
  if(GetAction() != "DoorOpen")
    SetEntrance(0);
}
