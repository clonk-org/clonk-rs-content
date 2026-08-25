/*---- Quake-Bot ----*/

#strict
#include HZCK

local TotalDamaged;
local Skin;

static Bot_Difficulty;

protected func RejectCollect(id idObj, object pObj)
{
  // Für die KI
  var effect;
  if(effect = GetEffect("CollectionException", pObj))
    if(EffectVar(0, pObj, effect) == this())
      return(1);
  // Spawnpunkt-Hack
  if(idObj == SPNP) return();
  // Munitionspaket?
  if(pObj ->~ IsAmmoPacket())
    // Davon kann man in jeden Fall _eines_ im Inventar haben
    if(!CustomContentsCount("IsAmmoPacket"))
      return(0);
  //Waffe?
  if(pObj ->~ IsWeapon()) {
    //Sonderbehandlung?
    if(!(pObj ->~ OnCollection(this()))) {
      //nein. Standardprozedur:
      //schon so eine Waffe im Inventar? Oder bereits 3 andere Waffen?
      if(ContentsCount(idObj))
      {
      	if(!ConvertToAmmo(pObj, idObj))
        	return(1);  //Ja, nicht aufnehmen
        else
        {
        	return(0); //Aufnehmen, auch wenns schon verwertet ist!
        } 
      }
      else
        return(0);
    }
    else
      return(0);
  }
  // Einsammellimit für Ausrüstung
  if(pObj ->~ IsEquipment() && ContentsCount(idObj) > 0)
    return(1);
  //Wieviel haben wir denn schon im inventar?
  if(ContentsCount() - CustomContentsCount("IsWeapon") >= 2)
    return(1);
  
  // nicht angelegte Ausrüstung nochmal aufsammeln
  for(var gear in aGear)
    if(gear)
      if(GetID(gear) == idObj)
        return(1);
  
  // Ok
  return(0);
}

func Initialize()
{
	inherited();
	DoEnergy( -GetPhysical("Energy") / 1000 + 1); //Noch 1 Energie
  SetPhysical("Energy", 100000, 2); //Physical ändern
  DoEnergy( GetPhysical("Energy") / 100 - 1); // Neu dazugeben
  
  var swim = GetDefCoreVal("Swim","Physical",GetID());
  if(GetPhysical("Swim") < swim)
  {
  	SetPhysical("Swim",swim,2);
  }
  
  DoAmmo(QGTS,1);
  
  //AddEffect("I_R_Uber",this(),1,0,this());
  ScheduleCall(this(),"Initialized",1);
}

func Initialized()
{
	// muss 1 Frame versetzt gemacht werden, da beim Relaunch temporär für
	// 1 Frame lang ein neuer Clonk erzeugt wird - der den Eintrag natürlich nicht hat.
	var str = GetSkin();
  if(str)
  {
  	SetGraphics(str, this(), GetID());
  }
  else if(GetPlayerType(GetOwner()) == C4PT_Script)
  	SetRandomSkin();
  																																																																				OnSkinChange();
}

func ControlDig()
{
  //Hack wegen Sekundärfeuer
  Control2Contents("ControlDig");  
	return(1);
}

func ControlDigSingle() { return(1); }
func ControlDigDouble() { return(1); }

protected func ControlSpecial()
{
	_inherited();
  Sound("change");
}

func OnDmg(int dmg, int type)
{
	if(type == DMG_Fire)
		return(60);
	return(20);
}

func OnHit(int iDmg, int iType, object pFrom)
{
	CastBlood(iDmg);
	
	if(GetBodyParts())
	{
		CastObjects(QBBP,1*GetBlood(), RandomX(20,30));
	}
	
	if(GetAction() S= "Dead")
	{
		if(iType == DMG_Explosion)
			Squash();
		
		TotalDamaged += iDmg;
		if(TotalDamaged > 10)
			Squash();
	}
}

func Damage(int iDmg, int byPlayer)
{
	if(GetAction() S= "Dead")
	{
		TotalDamaged += iDmg;
	
		if(TotalDamaged > 10)
			Squash();
	}
	
	return(_inherited(iDmg, byPlayer));
}

func Squash()
{
	CastBlood(20);																																																								if(QEGGtf2) {CastParticles("Sproing",4,50,0,0,40,80,RGB(200,200,100),RGB(255,255,100)); CastParticles("Sproing2",2,50,0,0,30,60,RGB(200,200,100),RGB(255,255,100));}else
	if(GetBodyParts())
	{
		CreateObject(QBBP,6*(2*GetDir() -1), 5, GetOwner())->Set(GetSkin(), GetDir(), 0);
		CreateObject(QBBP,0, 0, GetOwner())->Set(GetSkin(), !GetDir(), 1);
		CreateObject(QBBP,0, 0, GetOwner())->Set(GetSkin(), GetDir(), 1);
		CreateObject(QBBP,0, 0, GetOwner())->Set(GetSkin(), !GetDir(), 2);
		CreateObject(QBBP,0, 0, GetOwner())->Set(GetSkin(), GetDir(), 2);
		
		CastObjects(QBBP,5*GetBlood(), RandomX(20,30));
	}
	
	Sound("Splatsch");
	
	RemoveObject();
}

global func CastBlood(int strength)
{
	var clr1 = RGB(180,0,0);
	var clr2 = RGB(255,0,0);
	var mat = "Blood";																																																															if(QEGGrainbow) clr1 = clr2 = HSL(Random(256),255,180);

	if(GetBlood(2))
		CastParticles("Blood", (Random(4)+1)*strength*GetBlood(),RandomX(30,50),0,0,10,30,clr1,clr2);
	else 
		CastPXS(mat, (Random(6)+1)*strength*GetBlood(), RandomX(20,40));
}

func StartPunch()
{
	if(!ReadyToFire())
		return();
	if(InLiquid())
		SetAction("SwimPunch");
	else
		SetAction("Punch");
}

func Punching()
{
	
	var enemies = FindObjects(Find_Or(Find_AtPoint(0,0), Find_AtPoint((GetDir()*2-1)*5,0)),Find_OCF(OCF_Alive), Find_Func("CheckEnemy", this()), Find_Exclude(this()));
	for(var enemy in enemies)
		DoDmg(15, DMG_Melee, enemy);
}

func ConvertToAmmo(object pObj, id idObj)
{
	var AmmoPackID = pObj->~GetAmmoPackID();
	if(!AmmoPackID || !AmmoPackID->~IsAmmoPacket())
		return();
	
	var Weapon = FindContents(idObj);
	var AmmoCount = AmmoPackID->AmmoCount();
	var AmmoID = AmmoPackID->AmmoID();
	var MaxAmmo = AmmoID->MaxAmmo();
	
	// Haben wir schon vollen Ammo?
	if(CheckAmmo(AmmoID, MaxAmmo, this(), Weapon))
		return();
	
	// Haben wir wenig Ammo? 
	if(!CheckAmmo(AmmoID, MaxAmmo/4, this(), Weapon))
		DoAmmo(AmmoID, AmmoCount, this());
	else
		DoAmmo(AmmoID, AmmoCount/4, this());
	
	if(GetAmmo(AmmoID, this()) > MaxAmmo)
		DoAmmo(AmmoID, GetAmmo(AmmoID, this())-MaxAmmo);
	
	Sound("CollectAmmo");
	
	RemoveObject(pObj);
	return(true);
}

func OnAmmoChange()
{
	for(var obj in FindObjects(Find_Container(this()), Find_Func("IsWeapon")))
	{
		obj->~AdjustOverlay(this());
	}
	UpdateCharge();
}

func ContextSkin(object clonk)
{ [$ChangeSkin$|Image=QBOT]
	// darf nur der Clonk selbst!
	if(clonk != this())
		return();
	
	var temp = CreateObject(QDUM,0,0,NO_OWNER);
	SetGraphics(str,temp,GetID());
	temp->SetColorDw(GetColorDw());
	
	CreateMenu(QBOT, clonk, this(), 0, "$ChangeSkin$", 0, 0);
	SetMenuSize(5,0,clonk);
	AddMenuItem("Standard", "SetSkin", GetID(), this(), 0, 0, "Standard", 4, temp);
	RemoveObject(temp);
	
	var str, i;
	for(i = 1; str = GetSkinString(i); i++)
	{
		temp = CreateObject(QDUM,0,0,NO_OWNER);
		SetGraphics(str,temp,GetID());
		temp->SetColorDw(GetColorDw());
		
		AddMenuItem(str, "SetSkin", GetID(), this(), 0, i, str, 4, temp);
		RemoveObject(temp);
	}
	
	AddMenuItem("$Random$", "SetRandomSkin", QRND, this(), 0, i, "$Random$");
}

func SetSkin(dummy, int index)
{
	var Graphic = GetSkinString(index);
	SetGraphics(Graphic, this(), GetID());
	SetCrewExtraData(this(), "QuakeSkin", index);
	SetCrewExtraData(this(), "QuakeSkinSet", 1);																																												OnSkinChange();
}

func GetSkin()
{
	if(Skin)
		return(Skin);
	if(!GetCrewExtraData(this(), "QuakeSkinSet"))
		return();
	var index = GetCrewExtraData(this(), "QuakeSkin");
	var Graphic = GetSkinString(index);
	return(Graphic);
}

func GetSkinString(int index)
{
	if(index ==  0) return("");
	if(index ==  1) return("Future");
	if(index ==  2) return("Wooza");
	if(index ==  3) return("Spy");
	if(index ==  4) return("ABC");
	
	if(index ==  5) return("Normal");
	if(index ==  6) return("King");
	if(index ==  7) return("Knight");
	if(index ==  8) return("BlackKnight");
	if(index ==  9) return("Mage");
	
	if(index == 10) return("Bauer");
	if(index == 11) return("Priest");
	if(index == 12) return("DarkPriest");
	if(index == 13) return("Assassin");
	if(index == 14) return("Bones");
	
	if(index == 15) return("Agent");
	if(index == 16) return("Schutzanzug");
	if(index == 17) return("Hydro");
	if(index == 18) return("Banane");
	if(index == 19) return("Droid");
	
	if(index == 20) return("Dune");
	if(index == 21) return("Pinguin");
	if(index == 22) return("TheEye");
	if(index == 23) return("Pixel");
}

func SetRandomSkin()
{
	var i;
	for(i = 1; GetSkinString(i); i++);
	SetSkin(0,Random(i));
}


/* Teleporter */

func ContextTeleport(object clonk)
{ [Teleport|Image=QBTP|Condition=TeleReady]
	// darf nur der Clonk selbst!
	if(clonk != this())
		return();
	
	Sound("teleOn");
	
	var x,y;
	RelaunchPosition(x,y,GetPlayerTeam(GetOwner(clonk)));
	SetPosition(x,y,clonk);
	
	AddSpawnEffect(clonk, RGB(0,0,200));
	
	RemoveEffect("TeleportBonus",this());
}

func TeleReady()
{
	return(GetEffect("TeleportBonus",this()));
}

func FlatUp()
{
	SetXDir();
	SetYDir();
	SetAction("Walk");
}

// Bot-Zeug
local deleted;
public func DeleteMe()
{
	deleted = true;
	RemoveObject();
}

protected func Death(int killedby) {
	if(deleted)
		return();
	
	killedby = GetKiller(this());
	// Backup.
	if(Local(998))
	{
		killedby = Local(999);
	}

	// Skin speichern
	Skin = GetSkin();

  var plr = GetOwner();

  // Sound und Meldung
  Sound("Die");
  DeathAnnounce(GetOwner(),this(),killedby);

  // Letztes Mannschaftsmitglied func tot() { neuer Einsatz
  
  if(GetPlayerType(GetOwner()) == C4PT_Script)
    GameCallEx("RelaunchPlayer",GetOwner(),this(), killedby);
  else
  {
    var gotcrew;
    for(var i; i < GetCrewCount(plr); i++)
      if(GetOCF(GetCrew(plr,i)) & OCF_Alive)
        gotcrew = true;
  
    if(!gotcrew)
      GameCallEx("RelaunchPlayer",GetOwner(),this(), killedby);
  }

  if(ammobag)
    RemoveObject(ammobag);

//  if(GetEffect(0,this(),wpneffect))
  //  RemoveEffect(0,this(),wpneffect);

	// Info-Broadcasts für sterbende Clonks
  GameCallEx("OnClonkDeath", this(), killedby);
  
  if(GetAlive()) return(0);
	
	// toll rumfliegen
	if(!GetXDir() && !GetYDir())
	{
		SetXDir(30*(1-2*GetDir()));
		SetYDir(-30);
	}
	
	return(1);
}

public func WeaponAt(&x, &y, &r)
{
  if(Contained()) return(0);

  var a = GetAction();
  var p = GetPhase();
  
  // x < 0: Richtung hinter den Clonk
  // x > 0: Richtung vor den Clonk
  x=0; y=0; r=0;

	if(a eq "JumpedArmed") {
    x=500-125*p;
    y=1000;
    return(1);
  }
  return(_inherited(x,y,r));
}

public func ReadyToFire() {     // nur aus bestimmten Aktionen feuern

   var a = GetAction();

   // In Gebäude
   if(Contained()) return(Contained()->~ReadyToFire());
   // Reitet
   if(IsRiding()) return(GetActionTarget()->~ReadyToFire());
   // Schiebt
   if(a S= "Push")
   	if(GetActionTarget()->~IsWeapon())
   		return(true);

   // Nur beim Laufen, Schwimmen, Springen oder Zielen
   if(!( a eq "WalkArmed"
      || a eq "SwimArmed"
      || a eq "JumpArmed"
      || a eq "JetpackFlight"
      || a eq "Aim"
      || a eq "AimLow"
      || a eq "AimSquat"
      || a eq "AimSquatLow"
      || a eq "JumpedArmed" )) 
     return(false);

   // Okay
   return(true);
}

// Fix
private func DolphinJump()
{
  // nur wenn an Meeresoberfläche
  if(!GBackLiquid(0,-1)) return(0);
  if(GBackSemiSolid(0,-2)) return(0);
  // Nicht wenn deaktiviert (z.B. Ohnmacht)
  if (GetActMapVal("ObjectDisabled", GetAction(), GetID())) return false;
  // herausspringen
  SetPosition(GetX(),GetY()-1);
  SetAction("Jump");
  SetSpeed(GetXDir(),-BoundBy(GetPhysical("Swim")/2500,24,38));
}


global func FxI_R_UberFMData(object pTarget, int iEffectNumber, int iData, value) {
  //Damage*2!
  if(iData == FM_Reload) return(value/2);
  return(value);
}

/********************** KI **********************/
local altfire;

// 0 = easy
// 1 = normal
global func SetBotDifficulty(int difficulty)
{
	Bot_Difficulty = BoundBy(difficulty,0,1);
	return(Bot_Difficulty);
}

global func GetBotDifficulty() { return(Bot_Difficulty); }

public func FxAggroFire(object pTarget, int no)
{
  // Zusatzhack: BR-Bombe!
  if(GetID(Contents()) == GBRB)
    // Nichts tun :C
    return();
  // Nichts tun, wenn gerade verhindert
  if(!ReadyToFire()) return();
  var y = EffectVar(4, this(), no);
  var x = EffectVar(3, this(), no);
  var dist = EffectVar(2, this(), no);
  var target = EffectVar(1, this(), no);
  var level = EffectVar(0, this(), no);
  var pathfree = true;
  // Fahrzeugsteuerung
  if(Contained())
  {
    if(Contained()->~HandleAggro(this(), level, target, dist, x, y))
      return(1);
    else
      return(AddCommand(this(), "Exit", 0,0,0,0,0,0,0, C4CMD_SilentSub));
  }
  if(IsRiding())
  {
    if(GetActionTarget()->~HandleAggro(this(), level, target, dist))
      return(1);
    else
      return(SetAction("Walk"));
  }
  // Zu weit von der Wachposition entfernt?
  if(level == Aggro_Guard)
    if(Distance(GetX(), GetY(), x, y) > dist)
    {
      if(GetMacroCommand(1, 1) == target)
      {
        FinishMacroCommand(1,0,1);
        FinishMacroCommand(1);
      }
      AddMacroCommand(0, "MoveTo", 0, x,y, 0, level);
      EffectVar(1, this(), no) = 0;
//      Message("@Returning to guarded position", this());
      return();
    }
    
  var maxdist = dist;
  if(!PathFree(GetX(), GetY(), target->GetX(), target->GetY()))
  {
    if(EffectVar(0, this(), no) == 1) maxdist = 0;
    if(EffectVar(0, this(), no) >= 2) maxdist = dist/2;
    pathfree = false;
  }
  // Ziel irgendwie weg?
  if(ObjectDistance(target) > maxdist ||
      !(target->GetAlive()) ||
      !CheckEnemy(this(), target))
    {
      EffectVar(1, this(), no) = 0;
      if(EffectVar(0, this(), no) == 2)
        ClearMacroCommands();
      if(IsAiming())
        StopAiming();
      return();
    }
  // Ich hab nix? °-°
  if(!Contents()) return(); // Lauf, Forest, lauf!
  
  var result;
  if(!(result = SelectWeapon(level, target, true)))
  {
    // Bei Aggro_Follow können wir von unserem Pfade weg. D.h. eine Waffe und/oder Munition muss her
    if(GetAggroLevel() == Aggro_Follow)
    {
//      Message("@Searching for weapons / ammo", this());
      // Waffen auffrischen?
      if(CustomContentsCount("IsWeapon") <= 3)
        return(SearchWeapon(Aggro_Shoot));
      // Munition auffrischen
      return(SearchAmmo(Aggro_Shoot));
    }
    // ein Balrog, ein Feind gegen den ihr nichts ausrichten könnt...lauft!
    return();
  }
  else if(result == -1)
  {
  	if(IsAiming())
  		StopAiming();
  	if(GetBotDifficulty() == 0)
  		return();
  }
  // Stufe 1 - nur in die grobe Richtung ballern, lieber nicht anhalten oder sowas

  // Schaue ich in die richtige Richtung?
  if(GetX() < target->GetX())
  {
    if(GetDir() != DIR_Right)
    {
      SetDir(DIR_Right);
      if(GetBotDifficulty() == 0)
      	return();
    }
  }
  else
  {
    if(GetDir() != DIR_Left)
    {
      SetDir(DIR_Left);
      if(GetBotDifficulty() == 0)
      	return();
    }
  }
  // Gut. Feuern wir bereits?
  if(Contents()->IsRecharging() || Contents()->IsShooting()) return();

  // Zielen, muss auch mal sein
 if((!GetCommand() && !GetMacroCommand()) || level != 1 || IsAiming())
 {
  if(pathfree && Contents()->GetBotData(BOT_Range) > 30)
  {
    var angle = Angle(GetX(), GetY(), target->GetX(), target->GetY());
    if(((!Inside(angle, 70, 120) && !Inside(angle, 250, 290)) || Contents()->GetFMData(FM_Aim))
         && maxdist != 300 && ObjectDistance(target) < 300)
    {
      if(!IsAiming()) StartSquatAiming();
      if(IsAiming())
      {
        if(GetDir() == DIR_Left)
          angle = 360-angle;
        angle = BoundBy(angle, 0, AIM_Max);
        if(Contents()->GetBotData(BOT_Ballistic))
          angle = BoundBy(angle-10, 0, AIM_Max);
        crosshair->SetAngle(angle);
        UpdateAiming();
      }
    }
    else
      if(IsAiming())
        StopAiming();
   }
	 if((IsAiming() && !CheckAmmo(Contents()->GetFMData(FM_AmmoID, altfire), Contents()->GetFMData(FM_AmmoLoad, altfire), Contents(), this())) || !pathfree)
	 	StopAiming();
  }

  // Feuer!
  if(pathfree)
  {
  	if(altfire)
  		Control2Contents("ControlDig");
  	else
	  	Control2Contents("ControlThrow");
  }
//  Message("@My target: %s @%d/%d with level %d", this(), target->GetName(), target->GetX(), target->GetY(), level);
  // Stufe 2 - verfolgen!
  if(EffectVar(0, this(), no) >= 2)
    if(GetMacroCommand(1) ne "Follow" || GetMacroCommand(1, 1) != target)
      if(GetMacroCommand(0) ne "Follow" || GetMacroCommand(0,1) != target)
      {
        DebugLog("FxAggroFire - Adding Follow command","aggro");
        AddMacroCommand(0, "MoveTo", 0, GetX(),GetY(), 0, level);
        AddMacroCommand(0, "Follow", target, 0, 0, 0, level);
      }
}

static QEGGtf2;
static QEGGrainbow;
static QEGGknifefight;
static QEGGwinter;

func OnSkinChange()
{
var knife = true, rainbow = CreateArray(), tf2 = CreateArray(), winter = true;
for(var clonk in FindObjects(Find_ID(QBOT), Find_OCF(OCF_Alive))){
var skin = clonk->GetSkin(); 
// auf tf2 checken
if(skin S= "" || skin S= "Future" || skin S= "Wooza" || skin S= "Spy" || skin S= "ABC"){if(!FindIn(tf2,skin)){tf2[GetLength(tf2)] = skin;}}
// auf winter checken
if(skin ne "Pinguin"){winter = false;}
// knifefight!
if(skin ne "Normal" && skin ne "Assassin" && skin ne "Knight" && skin ne "BlackKnight" && skin ne "King"){knife = false;}
// regenbogen!
if(skin S= "Banane" || skin S= "Bones" || skin S= "TheEye" || skin S= "Dune" || skin S= "Pixel"){if(!FindIn(rainbow,skin)){rainbow[GetLength(rainbow)] = skin;}}	}
// tf2 setzen?
if(GetLength(tf2) == 3){QEGGtf2 = true;}else{QEGGtf2 = false;}
// winter setzen
QEGGwinter = winter;	
// knifefight aktivieren?
if(knife && !QEGGknifefight){for(var gauntlet in FindObjects(Find_ID(Q_GT))){gauntlet->SetHax(true);}QEGGknifefight = true;}
// knifefight deaktivieren?
else if(!knife && QEGGknifefight){for(var gauntlet in FindObjects(Find_ID(Q_GT))){gauntlet->SetHax(false);}QEGGknifefight = false;}
// regenbogen setzen?
if(GetLength(rainbow) == 3){QEGGrainbow = true;}else{QEGGrainbow = false;}
}

func FindIn(array a, string thing)
{
	for(var b in a)
		if(b S= thing)
			return(true);
	
	return(false);
}

// Waffe auswählen!
public func SelectWeapon(int iLevel, object pTarget, bool fFireModes)
{
  // Entfernung zum Ziel
  var dist = ObjectDistance(pTarget);
  // Keine Waffen in Inventar?
  if(!CustomContentsCount("IsWeapon")) return();
  
  // Waffen die wir haben, nach Priorität sortiert - stärkste zuerst
  var weapons = FindObjects(Find_Container(this()), Find_Func("IsWeapon"), Sort_Reverse(Sort_Func("GetBotData",BOT_Priority)));
	var favorit;
  
	for(var weapon in weapons)
	{
		// erstmal ohne altfire
  	altfire = false;
		// haben wir nur 1 Feuermodus?
		if(! weapon->GetFMData(FM_Name,2))
		{
			// Distanz zu groß?
			if(dist > weapon->GetBotData(BOT_Range,1))
				// passt, nächste!
				continue;
		}
		// nein, wir haben 2 den besseren auswählen
		else
		{
			// ist der 2. Modus stärker als der 1. und passt die Distanz?
			if(weapon->GetBotData(BOT_Power, 2) > weapon->GetBotData(BOT_Power, 1) &&
			   weapon->GetBotData(BOT_Range, 2) >= dist)
			  // 2. feuermodus verwenden
				altfire = true;
			// Distanz bei beiden Feuermodi ausreichend, und Stärke gleich?
			else if(weapon->GetBotData(BOT_Power, 2) == weapon->GetBotData(BOT_Power, 1) &&
		  	      weapon->GetBotData(BOT_Range, 2) >= dist && weapon->GetBotData(BOT_Range, 1) >= dist)
				// Per Random einen auswählen
				altfire = Random(2);
			// bleibt nur noch der 1.
			else
				// Passt die Distanz des 1. Feuermodus??
				if(dist > weapon->GetBotData(BOT_Range, 1))
					// kthx, nächste Waffe
					continue;
		}
		// haben wir überhaupt genug Munition?
		if(!CheckAmmo(weapon->GetFMData(FM_AmmoID), weapon->GetFMData(FM_AmmoUsage, altfire+1), weapon, this()))
		{
			if(! weapon->GetFMData(FM_Name,2))
				continue;
			altfire = !altfire;
			// kommt der andere Feuermodus in Frage und hat der genug Ammo?
			if(weapon->GetBotData(BOT_Range, altfire+1) >= dist && 
			   !CheckAmmo(weapon->GetFMData(FM_AmmoID), weapon->GetFMData(FM_AmmoUsage, altfire+1), weapon, this()))
				// nächste Waffe
				continue;
		}
		
		// Okay. Wir haben unsere Waffe gefunden!
		favorit = weapon;
		break;
	}

	// keine Waffe gefunden?
	if(!favorit)
	{
		favorit = FindContents(Q_MG); // MG auswählen
		// und ammo suchen oder so
		return(0);
	} 
	
	//return(ShiftContents(0,0,favorit->GetID()) && favorit);
	
	
	// ob wir Waffe gewechselt haben
	var shift = (Contents(0) != favorit);
	if(shift)
		ShiftContents(0,0,favorit->GetID()); 
	
	// Wir haben Waffe gewechselt -> nicht gleich feuern
	if(shift)
  	return(-1);
  
  return(true);
  
}

// Sucht nach Waffen und läuft dorthin
public func SearchWeapon(int iAggro)
{
  // Nächstbeste Spawnpunkte abklappern
  for(var pSpawn in FindObjects(Find_ID(SPNP), Sort_Random()))
    // Da ist eine Waffe drin?
    if(pSpawn -> Contents() ->~ IsWeapon())
      // Die haben wir auch noch nicht?
      if(!FindContents(pSpawn->Contents()->GetID()))
      	// Einsammelbar?
    		if(pSpawn->CheckCollect(GetOwner(),this()))
        	// Hinlaufen
        	return(SetMacroCommand(0, "MoveTo", pSpawn, 0,0,0, iAggro));
}

// Sucht nach Munition und läuft dorthin
public func SearchAmmo(int iAggro)
{
  // Nächstbeste Spawnpunkte abklappern
  for(var pSpawn in FindObjects(Find_ID(SPNP), Sort_Random()))
    // Da ist Munition drin?
    if(pSpawn -> Contents() ->~ IsAmmo())
    	// Einsammelbar?
    	if(pSpawn->CheckCollect(GetOwner(),this()))
      	// Hinlaufen (wir sind gutgläubig und denken, dass wir die auch brauchen)
      	return(SetMacroCommand(0, "MoveTo", pSpawn, 0,0,0, iAggro));
}

// Sucht nach einem Medikit und läuft dorthin
public func SearchMedikit(int iAggro)
{
  // Medikits suchen
  for(var pSpawn in FindObjects(Find_ID(QMED), Sort_Distance(0,0)))
    // einsammelbar?
		if(pSpawn->CheckCollect(GetOwner(),this()))
			// Hinlaufen
			return(SetMacroCommand(0, "MoveTo", pSpawn, 0,0,0, iAggro));
}

// Checken ob wir Munition brauchen
public func CheckMyAmmo()
{
	var totalshots;
	// erstmal checken ob wir genug Ammo für unsere vorhandenen Waffen haben
	var weapons = FindObjects(Find_Container(this()), Find_Func("IsWeapon"));
	for(var weapon in weapons)
		totalshots += GetAmmo(weapon->GetFMData(FM_AmmoID),this())*10/weapon->GetFMData(FM_AmmoLoad);
	
	// weniger als 10 Schuss? D:
	return(totalshots > 100);
}
