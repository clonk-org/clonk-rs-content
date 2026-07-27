/*-- Paladin --*/

#strict 2 

#include KNIG // Ist ein Ritter und somit ein Clonk

static const PLDN_RingMenu=false; // Wenn false benutzt der Paladin ein ganz normales Auswahlmenü
static const PLDN_ComboMenu=true;
static const PLDN_SwordLayer=5; // Der Layer der Paladinschwertgrafik

local idCurSpell; // Auch! :O
local aSpells;    // Die Zauber, die man hat!
local alreadyInfo;

protected func Initialize()
{
  aSpells=[];
  SetSpells();
	DoKarmaEnergy(100);
  return _inherited(...);
}

protected func Recruitment()
{
  var clnk;
  var minimum_paladin_energy = 50000;
  
  if(!alreadyInfo)
  {
    alreadyInfo=1;
    clnk=CreateObject(KNIG,0,0,GetOwner());
    MakeCrewMember(clnk,GetOwner());
    var energy_ph = GetPhysical("Energy", 0, clnk);
    minimum_paladin_energy = Max(minimum_paladin_energy, energy_ph);
    
    GrabObjectInfo(clnk,this);
    RemoveObject(clnk);
    DoEnergy(100);
    // auf jeden Fall Hangeln können (gibt sonst manchmal Probleme)
    SetPhysical("CanHangle", 1, 2, this);
  }
  
  // workaround für HP bug (Paladine haben aus irgendeinem Grund manchmal nur 30HP während Ritter 50HP [kein faircrew bug])
  if(GetPhysical("Energy") < minimum_paladin_energy)
  {
  	SetPhysical("Energy", minimum_paladin_energy, PHYS_Permanent, this);
  	DoEnergy(100);
  }
  
  if (GetPlayerType(GetOwner()) == C4PT_User) Schedule(Format("StartChurchChoose(%d)", GetOwner()), 1);
  // Grafik nach Kirche
  SetGraphics("");
  if(MMGetChurch(GetOwner())==scBloodchurch) SetGraphics("Dark");
  var rval = _inherited(...);

  NewPortrait();
  return rval;
}

protected func NewPortrait()
{
  if(HasArmor())
  {
    if(MMGetChurch(GetOwner())==scBloodchurch) SetPortrait(Format("ArmoredDark%d",Random(StdPortraitCount())+1), this); 
    else SetPortrait(Format("Armored%d",Random(StdPortraitCount())+1), this); 
  }
  if(MMGetChurch(GetOwner())==scBloodchurch) SetPortrait(Format("Dark%d",Random(StdPortraitCount())+1), this); 
  else SetPortrait(Format("%d",Random(StdPortraitCount())+1), this); 
  return;
}

public func ChangedTo()
{
  // Grafik nach Kirche
  SetGraphics("");
  var iChurch = MMGetChurch(GetOwner());
  if(iChurch == scBloodchurch) SetGraphics("Dark");
  if(HasArmor()) {
    if(iChurch == scBloodchurch) SetGraphics("ArmoredDark", this, GetID());
    else SetGraphics("Armored", this, GetID()); }
  // Wunder
  aSpells=[];
  SetSpells();
  // Portrait
  NewPortrait();
}

public func OnChurchChange(iChurch)
{
  ChangedTo();
}

// Gibt Anzahl an Standard-Portraits an
public func StdPortraitCount() { return 3; }


/*--------------------------------------------- Status -------------------------------------------------*/

//! Welche Objekte kann dieser Clonk einsetzen?
public func CanUse(idObj)
{
  // Schwert
  if (idObj == SWOR) return 1;
  // Axt
  if (idObj == AXE1) return 1;  
  // Bogen 
  if (idObj == BOW1) return 1;   
  // Bogen 
  if (idObj == MUSK) return 1;   
  // Schild 
  if (idObj == SHIE) return 1;   
  // Speer 
  if (idObj == SPER) return 1;   
  // Lanze 
  if (idObj == LANC) return 1;   
  // Rüstung 
  if (idObj == ARMR) return 1;  
  // Schwarze Rüstung definitv nischt. Er ist ein strahlender Paladin und nicht der schwarze Ritter oder so..
  if (idObj == BRMR) return 0;  
  // Unbekannt 
  return 0; 
} 
 
//! Bevorzugte Waffentypen für selbständige Bewaffnung (zur Zeit werden genau zwei Waffentypen erwartet). 
public func GetFightWeaponType(index)
{
  if (index == 0) return SWOR;
  if (index == 1) return AXE1;
  return 0;
}

//! Dieser Clonk ist ein Ritter
public func IsKnight()
{
  return 1;
}
 
//! Zielt der Clonk gerade mit dem Bogen? 
public func IsAiming() 
{ 
  return (GetAction() == "AimBow" || GetAction() == "RideAimBow" 
  || GetAction() == "AimMusket" || GetAction() == "RideAimMusket"); 
} 
 
//! Lädt der Clonk gerade den Bogen nach? 
public func IsLoading() 
{ 
  return (GetAction() == "LoadBow" || GetAction() == "RideLoadBow" 
  || GetAction() == "LoadMusket" || GetAction() == "RideLoadMusket"); 
}
 
//! Trägt der Clonk einen Bogen und hat Pfeile dabei? 
private func IsBowEquipped() 
{ 
  // Bogen dabei? 
  if (!FindContents(BOW1)) return 0; 
  // Pfeile dabei? 
  if (!GetArrowCount()) return 0; 
  // Bogen und Pfeile dabei 
  return 1; 
} 
 
private func IsMusketEquipped() 
{ 
  // Muskete dabei? 
  if (!FindContents(MUSK)) return 0; 
  // Kugeln dabei? 
  if (!GetBulletCount()) return 0; 
  // Alles dabei 
  return 1; 
}
 
//! Kann der Clonk aus der aktuellen Aktion den Bogen oder eine Wurfwaffe einsetzen? (oder Muskete?)
public func ReadyToFire() 
{ 
  // Nur beim Laufen - und neu auch beim Reiten... 
  if (GetAction() == "Walk" || GetAction() == "Ride" || GetAction() == "RideStill") return 1; 
  return 0; 
} 
 
//! Der Ritter braucht einen Sattel zum Reiten. 
public func NeedSaddle()  
{  
  return 1;  
}
 
private func ExecuteAttackBow(pTarget) 
  { 
  // Zielposition ermitteln 
  var iTx = GetX(pTarget), iTy = GetY(pTarget); 
  // Ziel in Reichweite? 
  if (Abs(iTx-GetX())>300 || Abs(iTy-GetY())>300) 
    // Nicht? Mögliche Abschußposition einnehmen 
    return SetCommand(this,"MoveTo",0, BoundBy(GetX(),iTx-290,iTx+290),GetY()); 
  // Bogen auswählen 
  ShiftContents(0, 0, BOW1); 
  // Kann leider von hier aus nicht zielen 
  if (!IsAiming()) 
    if (!ReadyToFire()) 
      return 0; 
  // Ziel nicht im Freien 
  if (Contained(pTarget)) 
    return 0; 
  // Ziel von hier aus nicht in Sicht 
  if (!PathFree( GetX(),GetY(), iTx, iTy )) 
    if (!PathFree( GetX(),GetY()-20, iTy-20 )) 
      return 0; 
  // Schießen 
  FireBowAt(iTx,iTy, false); 
  // Weitermachen   
  return 1; 
  }
  
private func FindAvailableArrow()
{
  // Brandpfeile > Sprengpfeile > Pfeile
  var weapon = FindAvailableWeapon(FARP);
  if(!weapon) weapon = FindAvailableWeapon(XARP);
  if(!weapon) weapon = FindAvailableWeapon(ARWP);
  return weapon;
}
 
private func FindAvailableBullet()
{
  // Giftmunition > Schrot > normal
  var weapon = FindAvailableWeapon(PBLP);
  if(!weapon) weapon = FindAvailableWeapon(SBLP);
  if(!weapon) weapon = FindAvailableWeapon(BLTP);
  return weapon;
}

/* Der Hans kann zaubern!1------------------------------------------------------------ */  
 
local iKarma;

public func GetKarmaEnergy()
{
  return iKarma;
}

public func DoKarmaEnergy(iAmount)
{
	// Karma anpassen und zurueckgeben, ob die Anpassung vollstaendig war.
	var iNewKarma = iKarma+iAmount;
	iKarma = BoundBy(iNewKarma, 0, MaxKarmaEnergy());
  return iKarma == iNewKarma;
}

public func MaxKarmaEnergy()
{
  return 100;
}
 
protected func SetSpells()
{
  // Manaphysical setzen
  SetPhysical("Magic", 0, 2, this);
  
  // Spruchliste löschen
  aSpells = [];

  var iChurch = MMGetChurch(GetOwner());
  var i, idWonder;
  var aNewSpells = [];
  if(!(aNewSpells = GameCall("GetPaladinSpells", GetOwner())))
  {
		var aNewSpells = [];
		// Alle Definitionen durchsuchen
		while(idWonder = GetDefinition(i++,1))
		{
		  // Wunder rausfiltern
		  if(idWonder->~IsWonder())
		  {
		    // Nur bei passender Kirche aufnehmen
		    if(idWonder->~NeedChurch())
		    {
		      if(iChurch == idWonder->~NeedChurch()) aNewSpells[GetLength(aNewSpells)] = idWonder;
		    }
		    else aNewSpells[GetLength(aNewSpells)] = idWonder;
		  }
		}
  }
  aSpells = aNewSpells;
  // und noch sortieren
  SortSpells();
}

protected func SortSpells()
{
  var iPrio = 100;
  var iSpell, i;
  var aNewSpells = aSpells;
  aSpells = [];
  // Sortierschleife
  while(GetLength(aNewSpells))
  {
    iPrio = 100;
    iSpell = 0;
    i = 0;
    for(var idWonder in aNewSpells)
    {
      if(idWonder->~GetWonderPriority() < iPrio)
      {
        iPrio = idWonder->~GetWonderPriority();
        iSpell = i;
      }
      i++;
    }
    aSpells[GetLength(aSpells)] = aNewSpells[iSpell];
    DeleteArrayItem(iSpell, aNewSpells);
  }    
}

public func GetSpell(iIndex, idID)
{
  // ID fälschlicherweise als erstes angegeben?
  if(GetType(iIndex)==C4V_C4ID) idID = iIndex;
  // Entweder das Wunder nach Index oder die ID checken
  if(idID)
  {
    return (GetIndexOf(idID, aSpells)>=0);
  }
  if(iIndex < GetLength(aSpells)) return aSpells[iIndex];
  return 0;
}

public func LearnSpell(idID)
{
  if(GetSpell(idID)) return;
  aSpells[GetLength(aSpells)] = idID;
  SortSpells();
}

public func ClearSpells()
{
  aSpells = [];
}

// Der Paladin kann nicht immer zaubern
public func ReadyToConjure()  { 
  return !Contained() && GetLength(aSpells);
}
  
protected func DoMagic(idSpell)
{
  // Nicht bereit
  if (!ReadyToConjure()) return;

  // Noch ein Delay?
  if(GetEffect(Format("Int%iDelay", idSpell), this))
  {
    MMShowHelpMessage("OnHavingSpellDelay",GetOwner(this));
    Message("$MsgDelay$",this);
    return;
  }
  // Nicht genug Gunst
  if(GetKarmaEnergy() < GetValue(0,idSpell))
  {
    Sound("Error");
    Message("$MsgNotEnoughEnergy$",this);
    return;
  }

  ExecMagic(idSpell);
}

private func ExecMagic(idSpell)
{ 
  // Zauberobjekt erschaffen und aktivieren
  if (!CreateObject(idSpell,0,0,GetOwner())->~Activate(this))
    // Zauber fehlgeschlagen
    return;
    
  // Gunst abziehen
  var value=idSpell->~GetKarmaValue(this);
  if(!value)value=GetValue(0, idSpell);
  DoKarmaEnergy(-value);

  // Delay errechnen, Grundsätzlich mehr je teurer der Zauber war, und kürzer je besser die Gunst ist.
  var iDelay;
  if(!(iDelay = idSpell->~GetSpellDelay(this)))
  	iDelay=Value(idSpell)*36*1;

//  iDelay = iDelay-iDelay*GetKarmaEnergy()/110;
  // Wunderdelay setzen
  AddEffect(Format("Int%iDelay", idSpell), this, 1, iDelay);
  ScheduleCall(this, "DelayFinished", iDelay, 1, idSpell);

  // Fertig
  return 1;
}

public func DelayFinished(idSpell)
{
  CustomMessage(Format("$MsgDelayFinished$", GetName(0,idSpell)), this, GetOwner(), 0, 0, RGB(50,250,50));
  Sound ("Ding", 0, this, 0, GetOwner()+1); 
}
 
/*------------------------------------------- Steuerung ------------------------------------------------*/   
 
public func ContextPray(pCaller)
{
  [$CtxPrayDesc$|Image=PLDN|Condition=PrayingAllowed]
  
  if(GetAction() == "Walk")
  SetAction("Pray");
  else if(GetAction() == "Pray")
  SetAction("KneelUp");
  
  return 1;
}

protected func PrayingAllowed()
{
 return !ObjectCount(NPRY) && !Contained() && MMGetChurch(GetOwner());
}

public func GetCastableWonder(int i)
{
	if(i>=GetLength(aSpells))return 0;
	if(i<0)return 0;
	return aSpells[i];
}

public func CheckMagicRequirements(idThisSpell, val)
{
	if(GetEffect(Format("Int%iDelay", idThisSpell), this))return false;
	if(GetKarmaEnergy() < GetValue(0, idThisSpell))return false;
	return MCLK_UnlimitedCast;
}

public func ContextMagic(pCaller)
{
  [$CtxMagicDesc$|Image=MCMS|Condition=ReadyToConjure]
  // Nicht bereit
  if (!ReadyToConjure()) return 0;
  // Clonk soll anhalten
  //SetComDir(COMD_Stop, pCaller);
  // Menü öffnen, wenn standard Menü an ist
  var iExtra, idSpell, i;
  if(PLDN_ComboMenu)
  {
  	var pMenu=CreatePaladinSpellMenu(this, this, 0, 1);
  	if(pMenu)
  	{
  		return 1;
  	}
  }
  if(!PLDN_RingMenu)
  {
    if(ObjectCount(NMGE)) iExtra=0; else iExtra=3; 
    CreateMenu(MCMS, 0, 0, iExtra, "$MnuNoSpells$", GetKarmaEnergy());
   
    for(var cnt=0;cnt<GetLength(aSpells);cnt++)
      AddMenuItem("$MnuCast$: %s", "DoMagic", aSpells[cnt]);

    // Einen Spruch auswählen
    SelectMenuItem(GetLength(aSpells)/2);
  }
  else
    InitSpellRingMenu();
  
  return 1;
}

public func OnComboMenuEnter(id idSpell)
{
	DoMagic(idSpell);
}

protected func InitSpellRingMenu()
{
  var menu = CreatePalaRingMenu(this);

  var index; 
  for(var cnt = 0; cnt < GetLength(aSpells); cnt++)
    index = menu->AddItem(aSpells[cnt],GetDesc(0,aSpells[cnt]),"DoMagic",aSpells[cnt],0,RGB(255,255,255));
  
  LocalN("iSymbolSize",menu)=100;
  menu->SelectID(aSpells[GetLength(aSpells)/2]);
  menu->Show(this);
}

protected func StartPraying()
{
  var iAmount = 8;
  if(FindObject2(Find_ID(SNHP), Find_AtPoint())) iAmount = 16;
  else if(FindObject2(Find_ID(ALTR), Find_AtPoint())) iAmount = 12;
  AddEffect("IntPraying", this, 1, 50/iAmount, this);
}

protected func FxIntPrayingTimer(object trg, int num, int fxtime)
{
  // Beten gibt Karmaenergie
  EffectVar(0, trg, num)++;
  if(EffectVar(0, trg, num)%4 == 1) DoKarmaEnergy(1);
  CreateParticle("PSpark", RandomX(-5,5),RandomX(-10,10),0, -3, 10, RGB(255,255,0), trg, 0);
  CreateParticle("PSpark", RandomX(-5,5),RandomX(-10,10),0, -3, 10, RGB(255,255,0), trg, 0);
  return FX_OK;
}

protected func StopPraying()
{
  if(GetAction()=="Pray") return;
  RemoveEffect("IntPraying", this);
  MMStopPraying();
}
 
// Aufhören mit dem Beten
protected func ControlUpDouble()
{
  if(GetAction() == "Pray")
  {SetAction("KneelUp");SetComDir(COMD_Down);return 1;}
   
  return _inherited(...);
}
 
protected func ControlLeftDouble()
{
  if(GetAction() == "Pray")
  {SetAction("KneelUp");SetComDir(COMD_Left);return 1;}
   
  return _inherited(...);
}
 
protected func ControlRightDouble()
{
  if(GetAction() == "Pray")
  {SetAction("KneelUp");SetComDir(COMD_Right);return 1;}
  
 return _inherited(...);
}
/*------------------------------------------ Kommandosteuerung ------------------------------------------------*/   
 
protected func ControlCommand(szCommand, pTarget, iTx, iTy, pTarget2, Data) 
{ 
  // Context -> Keine Sonderbehandlung 
  if(szCommand == "Context") return 0; 
  // Bisherige Befehle löschen  
  SetOrderNone();  
  // Kommando MoveTo an Pferd weiterleiten. 
  if (IsRiding()) 
    { 
    if (szCommand == "MoveTo")
    {
      if(!IsAiming())
        return ObjectCall(GetActionTarget(), "ControlCommand", szCommand, pTarget, iTx, iTy);
    }
    else
    { 
      // Anderes Kommando beim Reiten: absteigen 
      SetComDir(COMD_Stop, GetActionTarget()); 
      ObjectCall(GetActionTarget(), "ControlDownDouble", this);
    } 
  } 
  // Kommando MoveTo in Bogenschießen umwandeln. 
  if (szCommand == "MoveTo") 
    { 
    if (IsAiming()) 
      if(WildcardMatch(GetAction(),"*Bow*")){ FireBowAt(iTx, iTy, true); return 1; } 
      else if(WildcardMatch(GetAction(),"*Musket*")){ FireMusketAt(iTx, iTy, true); return 1; } 
    // Der Spieler war etwas schnell... 
    if (IsLoading()) 
      return 1; 
    } 
  // Kommando Attack abfangen und in eigenen OrderAttack umwandeln. 
  if (szCommand == "Attack") 
    return SetOrderAttack(pTarget); 
  // Kommando Throw mit Speer in OrderThrowSpear umwandeln. 
  if (szCommand == "Throw") 
    if (GetID(pTarget) == SPER || ObjectCall(pTarget, "IsSpear")) 
      return SetOrderThrowSpear(pTarget, iTx, iTy); 
  // Sonst nicht abfangen 
  return _inherited(szCommand, pTarget, iTx, iTy, pTarget2, Data, ...); 
}
 
/*-------------------------------------------- Pfeile / Pakete -----------------------------------------*/ 

public func StopLoading() 
{ 
  // Zielt gar nicht: nix machen 
  if (!IsLoading()) return 0; 
  // Stop 
  SetComDir(COMD_Stop); 
  // Basisaktion setzen 
  if (IsRiding()) 
    SetAction("Ride"); 
  else 
    SetAction("Walk"); 
  // Sound 
  Sound("Grab"); 
  return 1; 
} 

/* Kugel aus dem Inventar nehmen */ 
public func GetBullet() 
{ 
  // Einzelne Kugln suchen 
  var pObj; 
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~IsBullet()) 
      return pObj; 
  // Bei Bedarf aus Taschen nehmen 
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~IsBulletPack()) 
      // Kugel aus Paket verwenden
      return pObj->~GetItem();
  // Keine Kugeln gefunden 
  return 0; 
} 

/* Kugeln im Inventar zählen */ 
private func GetBulletCount() 
{ 
  return GetSpecialCount("IsBullet");
}
  
public func GetMaxSpecialCount(iIndex, fAmount)
{
  // Hier könnten Spezialbehandlungen von Itemgruppen definiert werden
  // wie z.B. zu dem Inventar noch 30 Pfeile aufnehmen (siehe auch Ritter)
  if(iIndex == 0) { if(fAmount) return 30; return "IsArrow"; }
  if(iIndex == 1) { if(fAmount) return 10; return "IsBullet"; }
}
 
protected func GetObject2Drop(pCollectObj) 
{ 
  var iArrowPriority; 
  // Wenn ein Pfeil oder Pfeilpaket aufgenommen werden soll... 
  if (pCollectObj) if (ObjectCall(pCollectObj, "IsArrow") || ObjectCall(pCollectObj, "IsArrowPack")) 
    // Pfeilen und Pfeilpaketen eine niedrigere Priorität geben 
    iArrowPriority=-500; 
    //Nochmal für Kugelnz
    if (pCollectObj) if (ObjectCall(pCollectObj, "IsBullet") || ObjectCall(pCollectObj, "IsBulletPack")) 
    iArrowPriority=-499; 
    
  // Das schlechteste Objekt wegwerfen 
  var pBestDropObj, iBestDropPrio=1000; 
  var pObj, iPrio; 
  var idCollectObj = GetID(pCollectObj); 
  for(var i = 0; pObj = Contents(i); i++) 
    if ((iPrio = GetContentsPriority(pObj, iArrowPriority, idCollectObj)) < iBestDropPrio) 
      { 
      iBestDropPrio = iPrio; 
      pBestDropObj = pObj; 
      } 
  // Dieses Objekt zurückgeben 
  return pBestDropObj; 
} 
 
private func GetContentsPriority(pContents, iArrowPriority, idCollectObj) 
{ 
  // Pfeil? Normalerweise hohe Priorität, wenn nicht gerade ein anderes Pfeilpaket eingesammelt werden soll 
  if (pContents->~IsArrow()) return 302+iArrowPriority; 
  // Pfeilpakete sind etwas geringer brauchbar, damit sie eher abgelegt werden (geht schneller...) 
  if (pContents->~IsArrowPack()) return 301+iArrowPriority; 
  
  //Nochmal für Kugeln
  if (pContents->~IsBullet()) return 303+iArrowPriority; 
  if (pContents->~IsBulletPack()) return 302+iArrowPriority; 
  
  // Objekt ist eine Handwaffe? 
  var idCnt = GetID(pContents); 
  if (pContents->~IsFightWeapon()) 
    { 
    // Es soll eine Handwaffe eingesammelt werden 
    if (DefinitionCall(idCollectObj, "IsFightWeapon")) 
      // Hat schon eine Handwaffe 
      if (HasFightWeapon()) 
        // Sehr niedrige Priorität 
        return 1; 
    // Ansonsten relativ hohe Priorität 
    return 220; 
    } 
  // Bogen? 
  if (idCnt==BOW1) 
    { 
    // Noch ein Bogen vorhanden? Muss nicht sein 
    if (idCollectObj==BOW1 || ContentsCount(BOW1) > 1) return 1; 
    // Ansonsten fast so wichtig wie die Handwaffe (für die KI...) 
    return 216; 
    } 
  //Muskete?
  if (idCnt==MUSK) 
    { 
    // Noch ein Bogen vorhanden? Muss nicht sein 
    if (idCollectObj==MUSK || ContentsCount(MUSK) > 1) return 1; 
    // Eins mehr als Bogen
    return 217; 
    } 
  // Speer? 
  if (idCnt==SPER) return 212; 
  // Der Rest kann kaum gebraucht werden; ablegen nach Wert 
  return GetValue(pContents); 
} 

/* Rüstung anziehen */ 
public func WearArmor(pArmor) 
{ 
  // Kein Rüstungsobjekt übergeben? 
  if (!pArmor) return 0; 
  // Schon eine Rüstung an? 
  if (fArmored) return 0; 
  // Neue Grafik setzen 
  if (!SetGraphics(pArmor->~GetGraphics())) 
    // Grafik nicht vorhanden: Sicher ein abgeleitetes Objekt, das keine Rüstung tragen kann 
    return 0;
  // Schwarzer Paladin -> schwarze Rüstung
  if(MMGetChurch(GetOwner())==scBloodchurch) SetGraphics(Format("%sDark",pArmor->~GetGraphics()));
  // Mit dem Anziehen beginnen 
  SetAction("PutOnArmor");  
  SetComDir(COMD_None);  
  SetXDir(0); 
  // Rüstungstyp speichern und Rüstungsobjekt verbrauchen 
  idArmorType = GetID(pArmor); 
  RemoveObject(pArmor); 
  // Sound 
  Sound("Connect"); 
  return 1; 
}

/* Rüstungsstatus setzen */ 
public func SetArmored(fnArmored, idnArmorType) 
{ 
  var strArmorType, iPortraitCount, szDark;
  // Rüstung angelegt 
  if (fArmored = fnArmored) 
  {                   
    // Kein Rüstungstyp angegeben! 
    if (!idnArmorType) { fArmored = 0; return 0; } 
    // Rüstungstyp speichern 
    idArmorType = idnArmorType;     
    // Grafik setzen 
    strArmorType = DefinitionCall(idArmorType, "GetGraphics"); 
    SetGraphics(strArmorType);
    if(MMGetChurch(GetOwner())==scBloodchurch) SetGraphics(Format("%sDark",strArmorType));
    // Beschränkte Physicals ...könnte man noch von der jeweiligen Rüstung abhängig machen 
    SetPhysical("Jump", GetPhysical("Jump") * 3 / 4, 2); // Sprungkraft temporär vermindern 
    SetPhysical("Swim", GetPhysical("Swim") / 2, 2); // Schwimmen klappt auch nicht mehr so richtig 
    SetPhysical("CanScale", 0, 2);
    SetPhysical("CanHangle", 0, 2);
    // Ruestungsportrait 
    iPortraitCount = StdPortraitCount();
    szDark = "";
    if(MMGetChurch(GetOwner())==scBloodchurch) szDark = "Dark"; 
    SetPortrait(Format("%s%s%d", strArmorType, szDark, 1 + Random(iPortraitCount)), this, GetID(), 0, 0); 
  } 
  else 
  { 
    // Rüstungstyp löschen 
    idArmorType = 0;
    // Normale Grafik 
    SetGraphics("");
    if(MMGetChurch(GetOwner())==scBloodchurch) SetGraphics("Dark");
    // Normale Physicals 
    ResetPhysical(); 
    // Normales Portrait
    NewPortrait();
  } 
  // Ok 
  return 1; 
}    

 
// Erst 25 Frames nach dem ControlDown-Kommando 
private func StopFighting() 
{ 
  if(GetEffect("*Stun*", this)
  || GetEffect("*ForcesToFightWith*", GetActionTarget(0)))
  {
  	ScheduleCall(this, "StopFighting", 10);
  	return;
  }
  
  // ComDir auf NONE setzen, damit der Clonk nach dem Kampfabbruch nicht weiterlaeuft. 
  SetComDir(COMD_None); 
  // StopFighting-Effekt anfuegen, der den Kampf nach 25 Frames abbricht, wenn nicht schon vorhanden. 
  if(!GetEffect("IntKNIGStopFighting",this(),0,1) ) AddEffect("IntKNIGStopFighting", this(), 1, 25, this() ); 
} 
 
protected func FxIntKNIGStopFightingStop() 
{ 
  // Kaempfen wir noch? 
  if(IsFighting() ) 
  { 
    // Letzten Gegner fuer weitere 25 Frames speichern 
    AddEffect("IntKNIGLastEnemy", this, 1, 25, this, 0, GetActionTarget() ); 
    // Kampf abbrechen 
    SetAction("Walk"); 
    
  //Leider soll der Paladin kämpfen. Also klauen wir ihm als Bestrafung Mana <- AUS!
 // if(!ObjectCount(NMNL))
 // if(!GetEffect("LoseMana",this))AddEffect("LoseMana",this,20,5,this,0,5*3);
  }
}

protected func FxIntKNIGLastEnemyStart(pTarget, iEffect, pEnemy) 
{ 
  EffectVar(0, pTarget, iEffect) = pEnemy; 
}

 
/* Während des Kämpfens */ 
private func Fighting(pEnemy) 
{ 
  // Lanze verlieren 
  var pLance; 
  if (pLance = HasLance())  
    pLance->LNCA::Unbuckle();   
  // Kampf mit Handwaffe 
  var pWeapon; 
  if (pWeapon = GetFightWeapon()) 
  { 
    // Erstmal Kampfaktion setzen 
    if (GetAction() != pWeapon->~GetFightAction()) 
      return (SetAction(pWeapon->~GetFightAction(), pEnemy)); 
    // Je nach Waffengeschwindigkeit Schlag durchführen 
    if (!Random(pWeapon->~GetStrikeTime())) 
      return (SetAction(pWeapon->~GetStrikeAction())); 
    // Weiterkämpfen 
    return 1; 
  } 
  // Keine Waffe: Gegner schlagen 
  if (!Random(4))  
    SetAction("Punch"); 
  // Weiterkämpfen 
  return 1; 
} 

protected func Death(int iKiller)
{
  // Angelegtes Zeugs ablegen
  var obj;
  while(obj = FindAttachedItems(obj) ) ProtectedCall(obj, "AttachTargetLost");
  // Rüstung ablegen
  ArmorRemoved(); while (Contents()) Contents()->Exit();
  // Basisfunktion aufrufen
  return _inherited(iKiller, ...);
} 
 
protected func CatchBlow(iLevel, pObj) 
{ 
  // Zusätzlicher Schutzfaktor: Weniger Energieverlusst 
  DoEnergy(-iLevel * IsShielded() / 100); 
  return _inherited(iLevel * (100 - IsShielded()) / 100, pObj, ...);
} 
 
/* Fallschaden */ 
protected func Hit3() 
{ 
  if (HasArmor()) 
  { 
    DoEnergy(-10); 
    Sound("ClonkHit*");   
  }     
  return _inherited(...); 
}
     
/* Suchfunktionen ======================================================== */ 
 
private func FindEnemyUnit() 
{ 
  var pObj; 
  // Nächstgelegene Mannschaftsobjekte überprüfen 
  while (pObj = FindObject(0, 0,0,-1,-1, OCF_CrewMember, 0, 0, 0, pObj)) 
    // Verfeindet? 
    if (Hostile(GetOwner(),GetOwner(pObj))) 
      // Feind gefunden 
      return pObj; 
  // Keine feindliche Einheit gefunden 
  return 0; 
} 
 
private func FindClosestBase() 
{ 
  // Nächstgelegene Heimatbasis suchen 
  var i, pObj, pClosestBase = 0; 
  for(var i = 0; pObj = FindBase(GetOwner(), i); i++) 
    if (!pClosestBase || ObjectDistance(pObj) < ObjectDistance(pClosestBase)) 
      pClosestBase = pObj; 
  return pClosestBase; 
}

protected func FindAttachedItems(object after)
{
  var obj = after;
  while(obj = FindObject(0, 0, 0, 0, 0, 0, 0, this, 0, obj) )
    if(GetProcedure(obj) == "ATTACH")
      // ein Ritterding
      if(obj->~UnbuckledID())
        return obj;
  return 0;
}

// Musketebschießen-----------------------------------------------------------------------

private func AimMusketToTarget(iXOffset, iYOffset) 
{ 
  // Richtung 
  if (iXOffset > 0) SetDir(DIR_Right); 
  if (iXOffset < 0) SetDir(DIR_Left); 
  // Zielrichtung 
  var iAngle = Angle(0,0,Abs(iXOffset),iYOffset); 
  SetPhase(BoundBy( iAngle / 23, 0, 7)); 
  // Höher zielen für größere Distanz 
  //if (GetPhase() > 1) 
   // SetPhase(BoundBy(GetPhase() - Abs(iXOffset) / 150, 1, 7)); 
  // Fertig 
  return 1; 
} 
 
private func AimMusketToAngle(iAngle) 
  { 
  // Winkel anpassen 
  while (iAngle > 180) iAngle -= 360; 
  // Richtung 
  if (iAngle > 0 ) SetDir(DIR_Right); else SetDir(DIR_Left); 
  // Zielrichtung 
  SetPhase(BoundBy( (Abs(iAngle)+10)/16, 0,9)); //8)/16,0,7
  // Fertig 
  return 1; 
  } 

private func SimMusket(int iAngle, int iTargetX)
{
  // Startwerte
  var iX, iY, iR, iXDir, iYDir, iRDir;
  GetMusketOut(0, iAngle, false, iX, iY, iR, iXDir, iYDir, iRDir);
  // Koordinaten umrechnen
  iX += GetX(); iY += GetY();
  // Zeit, die das Geschoss fliegen muss
  var iTime = (iTargetX - iX) * 10 / iXDir;
  // Simulieren
  return SimFlight(iX, iY, iXDir, iYDir, 1, 100, iTime);
}

private func FireMusketAt(int iX, int iY, bool fForce) 
{ 
  // Zielt gar nicht: erst mal laden 
  if (!IsAiming())
  {
    LoadMusket(FindContents(MUSK), 1);  
    return;  
  }
  // Zielen 
  
  var iAngle=Angle(GetX(),GetY(),iX,iY);
  AimMusketToAngle(iAngle); 
  // Feuern 
  if(GetEffect("SniperPotion",this) || GetEffect("*Sniper*",this))
    return Contents()->~FireAngle(iAngle);
  else
    return Contents()->~Fire(this);
} 

func HasFreeMusketShot()
{
	return GetEffect("HasFreeMusketShot", this);
}

/* Beginnt den Zielvorgang */  
public func AimMusket() 
{ 
  // Gibt den Rückgabewert von SetAction zurück 
  if (IsRiding()) 
    return SetAction("RideAimMusket"); 
  else 
    return SetAction("AimMusket");   
}  
  
/* Beginnt den Ladevorgang */  
public func LoadMusket(pBow, fFirstTime) 
{ 
  var freeShot=HasFreeMusketShot();
  // Nur mit Pfeil! 
  if (!GetBulletCount() && !freeShot) 
  { 
    // Kein Pfeil fürs erste Zielen: Meldung ausgeben 
    if (fFirstTime) 
    { 
      Sound("KnightFailure"); 
      PlayerMessage(GetController(), "$MsgNoBullet$", this); // Kein Pfeil 
    } 
    return 1; 
  } 
  // Bogen lieber wieder auswählen (GetArrow wählt gerne ab...) 
  ShiftContents(0,0,MUSK); 
  // Schon beim Nachladen? 
  if (IsLoading()) return 1;
  // War gerade beim Zielen: letzten Winkel sichern
  if (IsAiming())
    iLastAimAngle = GetPhase();
  else
    iLastAimAngle = 6;
  // Ist die Muskete schon geladen? -> dann gleich zielen
  if(Contents(0, Contents())) return AimMusket();
  // Muss den Rückgabewert von SetAction zurückgeben, da das Setzen durch Überladungen fehlgeschlagen sein kann 
  if (IsRiding()) 
    return SetAction("RideLoadMusket"); 
  else 
    return SetAction("LoadMusket"); 
} 
 
protected func BeginAiming() { return SetPhase(iLastAimAngle); } 

/* Winkel, in den der Ritter momentan zielt */
public func GetMusketAimAngle()
{ 
  // Richtung ermitteln 
  var iOutDir; if (!GetDir()) iOutDir=-1; else iOutDir=1; 
  // Winkel (von oben) ermitteln
  return iOutDir*GetPhase()*150/10;
}

/* Zielabweichung des Ritters. Überladbar. */
public func GetMusketAimingDeviation(pBow)
{
  return 3;
}
  
/* Ausgangsgeschwindigkeit und -position, wenn der Ritter mit dem Bogen in angegebene Richtung schießt */
public func GetMusketOut(pBow, iAngle, fDeviate, &iOutX, &iOutY, &iOutR, &iOutXDir, &iOutYDir, &iOutRDir) 
{
  var iDeviation;
  // Zufallsabweichung
  if(fDeviate) 
    { 
    iDeviation = GetMusketAimingDeviation(pBow);
    iAngle += Random(iDeviation * 2 + 1) - iDeviation;
    }
  // Austrittsgeschwindigkeit ermitteln 
  iOutXDir = Sin(iAngle+RandomX(-2,2), 200);
  iOutYDir = -Cos(iAngle+RandomX(-2,2), 200);
  // Austrittsposition... 
  iOutX = Sin(iAngle, 25 * GetCon() / 100);
  iOutY = -Cos(iAngle, 25 * GetCon() / 100); 
  // Pfeildrehung 
  iOutR = iAngle; 
  if(iAngle > 0) iOutRDir = 3; else iOutRDir = -3;
  // Fertig 
  return 1;
} 

private func ChangeMusketAimAngle(iChange) 
{ 
  var iOldAngle; 
  // Derzeitigen Winkel ermitteln 
  if (IsAiming()) iOldAngle = GetPhase(); 
  if (IsLoading()) iOldAngle = iLastAimAngle; 
  // Am Anschlag? 
  if (!Inside(iOldAngle + iChange, 0, 9)) return; 
  // Neuen Winkel setzen 
  if (IsAiming()) SetPhase(iOldAngle + iChange); 
  if (IsLoading()) iLastAimAngle += iChange; 
} 
 
public func MusketControlUp(pBow, &rVal) 
{ 
  ChangeMusketAimAngle(-1); 
  // Funktion überladen 
  return 1; 
} 
 
public func MusketControlDown(pBow, &rVal) 
{ 
  ChangeMusketAimAngle(+1); 
  // Funktion überladen 
  return 1; 
} 
  
protected func MusketLoaded()
{
 ShiftContents(0,0,MUSK); 
 if(GetID(Contents(0)) != MUSK) return 0;
 
 var musk=Contents();
 if(Contents(0,musk)) return;
 var freeShot=HasFreeMusketShot();
 var bullet;
 if(freeShot)
 {
 	AddEffect("IntNoPacking", this, 1);
 	bullet = CreateContents(BLLT, this);
 	RemoveEffect("IntNoPacking", this);
 	if(GetEffect("HasFreeMusketShot", this))
 		RemoveEffect("HasFreeMusketShot", this);
 }
 else
 	bullet = GetBullet();
	
 if(!bullet) return;
 AddEffect("IntNoPacking", musk, 1);
 Enter(musk,bullet);
 RemoveEffect("IntNoPacking", musk);
}

/* Aufwertung: Nur erlauben, wenn der Clonk noch kein Ritter (z.B.: König) ist */

public func IsRevaluationBlocked(object target) { return target->~IsPaladin(); }

public func OnRevaluation()
{
  // Der Magier bekommt bei Aufwertung Mana - Der Ritter also Waffen
  var i = Random(3);
       if (!i--) { CreateContents(BOW1); CreateContents(ARWP); }
  else if (!i--) CreateContents(SWOR);
  else if (!i--) { CreateContents(AXE1); CreateContents(SHIE); }
}
  
//Effekt für Mana verlieren (beim Flüchten aus Kampf zb)

protected func FxLoseManaStart(object pTarget, int iEffectNumber, int iTemp, val1)
{
  EffectVar(0,pTarget,iEffectNumber) = val1;
}

protected func FxLoseManaTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(iEffectTime > EffectVar(0,pTarget,iEffectNumber)) return -1;
  if(!pTarget || !pTarget->GetKarmaEnergy()) return -1;
  pTarget->DoKarmaEnergy(-1);
  return 1;
}

public func GotSword(pSword)
{
  SetGraphics("Attached",this,TSWD,PLDN_SwordLayer,GFXOV_MODE_ExtraGraphics);
}

public func LostSword(pSword)
{
  if(!ContentsCount(GetID(pSword)))
    SetGraphics(0,this,0,PLDN_SwordLayer);
}

/* Die Kontextmenüsteuerung vom Ritter, verändert für Muskete und Schwert */

private func ExecuteOrderArm() 
{ 
  // Schild dabei: anlegen 
  var pWeapon; 
  if (!HasShield()) 
    if (pWeapon=FindContents(SHIE)) 
      return ProtectedCall(pWeapon,"Activate",this);
    else 
      // Noch kein Schild: suchen 
      if (pWeapon=FindAvailableWeapon(SHIE)) 
        return SetCommand(this,"Get",pWeapon);
  var idWp1,idWp2;
  // Noch keine Handwaffe: suchen 
  if (!HasFightWeapon()) 
    { 
    if (Random(2))  
      { idWp1=GetFightWeaponType(0); idWp2=GetFightWeaponType(1); }  
    else  
      { idWp1=GetFightWeaponType(1); idWp2=GetFightWeaponType(0); } 
    pWeapon = FindAvailableWeapon(idWp1); 
    if (!pWeapon) pWeapon = FindAvailableWeapon(idWp2); 
    if (pWeapon) return SetCommand(this,"Get",pWeapon); 
    } 
  //M uskete wie immer höhere Priorität
  if (!FindContents(MUSK)) 
    { 
    if (pWeapon = FindAvailableWeapon(MUSK)) 
      return SetCommand(this,"Get",pWeapon); 
    } 
  else 
    // Hat eine Muskete: noch Kugeln dazu suchen 
    if (GetBulletCount()<10) 
      if (pWeapon=FindAvailableBullet())
        return SetCommand(this,"Get",pWeapon);
  
  // Wenn's geht noch einen Bogen suchen 
  if (!FindContents(BOW1)) 
    { 
    if (pWeapon=FindAvailableWeapon(BOW1)) 
      return SetCommand(this,"Get",pWeapon);
    } 
  else 
    // Hat einen Bogen: noch Pfeile dazu suchen 
    if (GetArrowCount()<21) 
      if (pWeapon=FindAvailableArrow())
        return SetCommand(this,"Get",pWeapon);
  // Wenn's geht noch einen Speer suchen 
  if (!FindContents(SPER)) 
    if (pWeapon=FindAvailableWeapon(SPER)) 
      return SetCommand(this,"Get",pWeapon); 
  // Fehlgeschlagen 
  if (!IsFullyArmed()) return OrderFailure("$MsgNoWeapons$"); // "Keine Waffen." 
  // Fertig   
  return OrderSuccess("$MsgArmed$"); // "Bewaffnet." 
} 

private func ExecuteOrderDefend() 
{ 
  // Distanz von der Verteidigungsposition ermitteln 
  var iDx=Abs(GetX()-ordrData1), iDy=Abs(GetY()-ordrData2);
  // Kein Ziel: zurück zur Verteidigungsposition 
  if (!pOrdrTarget) 
    if (iDx>20 || iDy>50) 
      return SetCommand(this,"MoveTo",0,ordrData1,ordrData2); 
  // Zu weit weg von Verteidigungsposition 
  if (iDx>150 || iDy>150) 
  {  pOrdrTarget=0; return 1; }
  // Keine feindliche Einheit in Reichweite 
  pOrdrTarget=FindEnemyUnit(); 
  if (!pOrdrTarget) return ExecuteWatch(); 
  if (ObjectDistance(pOrdrTarget) > 350) return ExecuteWatch();
  StopPray();
  //Muskete > Bogen
  //Muskete einsetzen (wenn möglich)
  if (IsMusketEquipped()) 
    if (ExecuteAttackMusket(pOrdrTarget)) 
      return 1;
  // Bogen einsetzen (wenn möglich) 
  if (IsBowEquipped()) 
    if (ExecuteAttackBow(pOrdrTarget)) 
      return 1; 
  // Speer einsetzen (wenn möglich) 
  if (FindSpearContents()) 
    if (ExecuteAttackSpear(pOrdrTarget)) 
      return 1; 
  // Direkter Angriff (mit kurzfristiger Laufzeit) 
  StopAiming(); 
  if(FindObject2(Find_Container(this), Find_Func("IsTwoHandedSword")) && (GetKarmaEnergy()>=2 || ObjectCount(NPRY)))
    if(!GetEffect("IntTHSwordAttack", this)) AddEffect("IntTHSwordAttack", this, 1, 1, this);
  return AddCommand(this,"Attack",pOrdrTarget,0,0,0,2);
  // Weiter 
  return 1; 
} 
   
private func ExecuteOrderGuard() 
{ 
  // Distanz von der Verteidigungsposition ermitteln 
  var iDx=Abs(GetX()-ordrData1), iDy=Abs(GetY()-ordrData2); 
  // Zurück zur Verteidigungsposition 
  if (iDx>10 || iDy>20) 
    return SetCommand(this,"MoveTo",0,ordrData1,ordrData2); 
  // Keine feindliche Einheit in Reichweite 
  pOrdrTarget=FindEnemyUnit(); 
  if (!pOrdrTarget) return ExecuteWatch(); 
  if (ObjectDistance(pOrdrTarget) > 350) return ExecuteWatch(); 
  StopPray();
  //Muskete > Bogen
  //Muskete einsetzen (wenn möglich)
  if (IsMusketEquipped()) 
    if (ExecuteAttackMusket(pOrdrTarget)) 
     return 1;
  // Bogen einsetzen (wenn möglich) 
  if (IsBowEquipped())
    if (ExecuteAttackBow(pOrdrTarget))
      return 1; 
  // Direkter Angriff (mit kruzfristiger Laufzeit) wenn Ziel ganz nah 
  StopAiming();
  if (ObjectDistance(pOrdrTarget) < 30)
  {
    if(FindObject2(Find_Container(this), Find_Func("IsTwoHandedSword")) && (GetKarmaEnergy()>=2 || ObjectCount(NPRY)))
      if(!GetEffect("IntTHSwordAttack", this)) AddEffect("IntTHSwordAttack", this, 1, 1, this);
    return AddCommand(this,"Attack",pOrdrTarget,0,0,0,2);
  }
  // Position halten 
  return 1; 
}

protected func FxIntTHSwordAttackTimer(pTarget, iNumber, iTime)
{
  if(iTime>=29) return -1;
//  if(GetCommand(this)!="Attack") return -1; 
  if(FindObject2(Find_Container(this), Find_Func("IsTwoHandedSword"))  && (GetKarmaEnergy()>=2 || ObjectCount(NPRY)))
  {
    if(FindObject2(Find_OCF(OCF_Alive),Find_Hostile(GetOwner()),Find_Layer(GetObjectLayer()),Find_Distance(20,-25,5),Find_NoContainer()))
    {
      SetDir();
      UseTwoHandSword();
    }
    else if(FindObject2(Find_OCF(OCF_Alive),Find_Hostile(GetOwner()),Find_Layer(GetObjectLayer()),Find_Distance(20,25,5),Find_NoContainer()))
    {
      SetDir(1);
      UseTwoHandSword();
    }
    return 1;
  }
  else return -1;
}
      
private func ExecuteOrderAttack() 
{ 
  // Kein Zielobjekt 
  if (!pOrdrTarget) return OrderFailure("$MsgNoEnemy$"); // "Kein Angriffsziel!" 
  // Ziel ist tot 
  if (!GetAlive(pOrdrTarget)) 
    return OrderSuccess("$MsgEnemyDeath$",GetName(pOrdrTarget)); // "%s erledigt." 
  // Ziel nicht mehr feindlich 
  if (!Hostile(GetOwner(pOrdrTarget),GetOwner())) 
    return OrderSuccess("$MsgEnemyHostChanged$",GetName(pOrdrTarget)); // "%s gehört|jetzt zu uns." 
  //Muskete > Bogen
  //Muskete einsetzen (wenn möglich)
  if (IsMusketEquipped()) 
    if (ExecuteAttackMusket(pOrdrTarget)) 
      return 1;
  // Bogen einsetzen (wenn möglich) 
  if (IsBowEquipped()) 
    if (ExecuteAttackBow(pOrdrTarget)) 
      return 1; 
  // Speer einsetzen (wenn möglich) 
  if (FindSpearContents()) 
    if (ExecuteAttackSpear(pOrdrTarget)) 
      return 1; 
  // Direkter Angriff (mit kruzfristiger Laufzeit) 
  StopAiming(); 
  AddCommand(this,"Attack",pOrdrTarget,0,0,0,2); 
  // Weiter 
  return 1; 
} 
 
private func ExecuteAttackMusket(pTarget) 
{ 
  // Zielposition ermitteln 
  var iTx = GetX(pTarget), iTy = GetY(pTarget); 
  // Ziel sichtbar?
  if(!PathFree(GetX(),GetY(),iTx,iTy))
    // Nicht? Mögliche Abschußposition einnehmen 
    return SetCommand(this,"MoveTo",0, BoundBy(GetX(),iTx-290,iTx+290),GetY()); 
  // Bogen auswählen 
  ShiftContents(0, 0, MUSK); 
  // Kann leider von hier aus nicht zielen 
  if (!IsAiming()) 
    if (!ReadyToFire()) 
      return 0; 
  // Ziel nicht im Freien 
  if (Contained(pTarget)) 
    return 0; 
  // Ziel von hier aus nicht in Sicht 
  //if (!PathFree( GetX(),GetY(), iTx, iTy )) 
  //  if (!PathFree( GetX(),GetY()-20, iTy-20 )) 
  //    return 0; 
  // Schießen 
  FireMusketAt(iTx,iTy, false); 
  // Weitermachen   
  return 1; 
}

private func ExecuteWatch() 
{
  if(FindContents(MUSK) &&  (GetAction() == "Walk" || IsAiming()))
  {
    if(!Contents(0, FindContents(MUSK)) && GetBulletCount()) LoadMusket(FindContents(MUSK));
  }
  if ( (GetAction() == "Walk" || IsAiming()) && PrayingAllowed())
  {
    if(GetKarmaEnergy() < MaxKarmaEnergy())
      SetAction("Pray");
  }
  if (Random(3)) return 1; 
  if (GetAction() == "Walk") SetDir(Random(2));
  var now, state;
  if (!Random(500)) { 
    now = Random(24); 
    if (!Random(500)) now = 42; 
    state = "$MsgWatchStateWell$"; 
    if (GetEnergy() < 10) 
      state = "$MsgWatchStateBad$";
    Message("$MsgWatch$", this, now, state);
  }
  return 1;
}

private func StopPray()
{
  if(GetAction() == "Pray")
  { SetAction("KneelUp");
    SetComDir(COMD_Down);
    return 1;
  }
}
  
public func UseTwoHandSword(pSword)
{ 
   if(!ObjectCount(NPRY))
   {
   if(GetKarmaEnergy() < 2)
     {
      PlayerMessage(GetOwner(this),"$MsgNotEnoughEnergy$",this);
      return 1;
     }
   }

   if(GetAction() == "Walk")
     SetAction("TwoHandSwordStrike");

   if(GetAction() == "Jump")
     SetAction("TwoHandSwordStrikeJump");

   return 1;
}
  
protected func TwoHandSwordPunchEnemies()
{   
  if(!ObjectCount(NPRY))
  {
    DoKarmaEnergy(-2);
  }
  
  var obj=FindObject2(Find_Container(this), Find_Func("IsTwoHandedSword"));
  if(obj)
  	if(obj->~CustomStrike(this))return 1;
  
  var mul=-1;
  if(GetDir() == DIR_Right) mul=1;
   
  for(var clnk in FindObjects(Find_OCF(OCF_Alive),Find_Distance(20,25*mul,5),Find_Layer(GetObjectLayer()),Find_NoContainer()))
  {
    Punch(clnk,20+RandomX(0,10));
  }
   
  for(var obj in FindObjects(Find_Category(16),Find_Distance(20,25*mul,5),Find_Layer(GetObjectLayer()),Find_NoContainer()))
  {
    if(!Stuck(obj))
    SetYDir(GetYDir(obj)-15-Random(10),obj);  
  }
   
  var y=15;
  for(;y>-10;y--)
  if(!GBackSolid(20*mul,y) && !GBackLiquid(20*mul,y))break;
   
  if(y <= -10)y=10;
   
  for(var cnt=0;cnt < 30;cnt++)
    CreateParticle("PxSpark",25*mul,y-Random(2),RandomX(-10,10),RandomX(-15,-5),30+Random(30),RGB(10,100,200));
   
  CreateParticle("Terashock",25*mul,y,RandomX(-6,6),RandomX(-6,6),280,RGB(10,100,200));
   
  ShakeFree(GetX()+25*mul,GetY()+y,20);
   
  Sound("TwoHandedSwordStrike",0,this);
}
  
protected func StartSwordstrike()
{
   if(!GetEffect("PaladinSwordstrike",this))
   AddEffect("PaladinSwordstrike",this,20,1,this);
   
   SetComDir(COMD_Stop);
   
   Sound("TwoHandedSwordGet*",0,this);
   
   SetGraphics(0,this,0,PLDN_SwordLayer);
}
  
protected func FxPaladinSwordstrikeStart(object pTarget, int iEffectNumber, int iTemp)
{
   if(iTemp)return 0;
   if(HasArmor()) {
     SetGraphics("ArmoredSwordstrike");
     if(MMGetChurch(GetOwner())==scBloodchurch) SetGraphics("ArmoredDarkSwordstrike"); }
   else {
   SetGraphics("Swordstrike");
   if(MMGetChurch(GetOwner())==scBloodchurch) SetGraphics("DarkSwordstrike"); }
   
   // Ein Schild der neuen Art wird unsichtbar gemacht
   if(LocalN("idShield",this))
   {
    EffectVar(5,pTarget,iEffectNumber)=GetClrModulation(this,KNIG_ShieldLayer);
    SetClrModulation(RGBa(255,255,255,255),this,KNIG_ShieldLayer);
   }
}
  
protected func FxPaladinSwordstrikeTimer(pTarget,iEffectNumber)
{
   if(!WildcardMatch(GetAction(),"*TwoHandSword*")) return -1;
   
   return 1;
}
  
protected func FxPaladinSwordstrikeStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
   if(fTemp)return 0;
   if(HasArmor()) {
     SetGraphics("Armored");
     if(MMGetChurch(GetOwner())==scBloodchurch) SetGraphics("ArmoredDark"); }
   else {
     SetGraphics();
     if(MMGetChurch(GetOwner())==scBloodchurch) SetGraphics("Dark"); }
   
   if(FindObject2(Find_Container(this), Find_Func("IsTwoHandedSword")) )
   SetGraphics("Attached",this,TSWD,PLDN_SwordLayer,GFXOV_MODE_ExtraGraphics);
   
   // Schild wiederherstellen?
   if(LocalN("idShield",this))
   {
    SetClrModulation(EffectVar(5,pTarget,iEffectNumber),this,KNIG_ShieldLayer);
   }
}

// Status
public func IsPaladin()     { return 1;    }
