/*-- Quake-Manager --*/

#strict 2

// Variablen:
local BloodCount; // Wieviel Blut: 0-2
local BloodStays; // Bleibt Blut da?
local ParticleBlood; // Partikelblut?
local BotCount;   // Wieviele KIs wir haben
local BodyParts; // Körperteile? 

static NoBots;

func InitializePlayer()
{
	if (FindObject2(Find_Func("IsMutableWinScoreGoal"))) //Hax?
	SetFrags(0,+3);
}

func RemovePlayer(int plr)
{
	var hasplayer;
	// checken ob es noch Spieler gibt, wenn nicht -> beenden
	for(var i = GetPlayerCount(); i; i--)
		if(GetPlayerType(GetPlayerByIndex(i)) == C4PT_User)
			hasplayer = true;
	
	if(!hasplayer && IsNetwork())
		GameOver();
}

func Initialize()
{
	BloodCount=1;
	BloodStays=false;
	ParticleBlood=false;
	BodyParts = false;
	ScheduleCall(this, "Initialized", 2);
}

func Initialized()
{
	// OMG nur 1 Spieler und netzwerk?
	if(IsNetwork())
		if(GetPlayerCount() < 2)
		{
			Log("$NoSinglePlayerInNetwork$");
			Message("$NoSinglePlayerInNetwork$");
			GameOver();
		}

	// omg keine Wegpunkte?!
	if(!FindObject(WAYP))
	{
		NoBots = true;
		Log("$NoBotsAvailable$");
		return;
	}

	var i, iReal,iTeam;
	//Nur ein Team da?
	for(i=1; i <= GetTeamCount(); i++)
		if(GetTeamPlrCount(i))
		{
			iReal++;
			iTeam = i;
		}
   
	if(iReal == 1)
	{
		AddAI(iTeam+1);
		ScheduleCall(this, "RecruitBots", 2,0,iTeam+1, GetTeamPlrCount(iTeam)-1);
	}
}

global func GetTeamPlrCount(int iTeam)
{
  var iRet;
  for(var i; i < GetPlayerCount(); i++)
	{
		var iPlr = GetPlayerByIndex(i);
			if(GetPlayerTeam(iPlr) == iTeam)
				iRet++;
	}
	return iRet;
}

public func RecruitBots(iTeam, icount)
{
   for(var i; i < icount; i++)
     AddBot(iTeam);
}

/* Statusfunktionen */
global func GetBlood(int type)
{
	// bei Liga vordefinierte Werte
	if(GetLeague())
	{
		if(type == 1)
			return false; // bleibt nicht da
		if(type == 2)
			return true; // immer Partikelblut
	}

	var manager = FindObject(QMNG);
	if(manager)
	{
		if(type == 1)
			return manager->GetBloodDuration();
		else if(type == 2)
			return manager->GetBloodType();
		else
			return manager->GetBloodCount();
	}
	else
		return 1;
}

global func GetBodyParts()
{
	var manager = FindObject(QMNG);
	if(manager)
		return manager->GetBodyPartsEnabled();
	else
		return 0;
}

func GetBloodCount() { return BloodCount; }
func GetBloodDuration() { return BloodStays; }
func GetBloodType() { return ParticleBlood; }

func GetBodyPartsEnabled() { return BodyParts; }


public func Activate(byPlr)
{
	var byClonk = GetCursor(byPlr);
	if(!byClonk)
		return;
	// Menü offen?
	if(GetMenu(byClonk))
	{
		// Schließen.
		CloseMenu(byClonk);
	}
	
	MakeInvincible(byClonk);
	
	AddEffect("GiveInfo", byClonk, 1, 1, this());
	
	CreateMenu(QMNG, byClonk, this(), 0, "$Options$");
	// Bots
	if(!GetLeague() && !NoBots)
		AddMenuItem("$ChangeBotCount$", "ChangeBots", QICO, byClonk,0,byClonk,"$ChangeBotInfo$", 2, 4);
	// Blut
	AddMenuItem("$ChangeBlood$", "ChangeBlood", QICO, byClonk,0,byClonk,"$ChangeBloodInfo$", 2, 6);
	// Frags
	if(CheckGoal() && !GetLeague())
		AddMenuItem("$ChangeFragCount$", "ChangeFrags", QGDM, byClonk,0,byClonk,"$ChangeFragsInfo$");
	
	BotsInfo(byClonk);
}

func CheckGoal()
{
	return FindObject2(Find_Func("IsTeamGoal"));
}

/** Bots **/

func ChangeBots(dummy, object byClonk)
{
	CreateMenu(QBOT, byClonk, this(), 0, "$ChangeBotCount$", 0, 1, true);
	
	var name, i;
	for(i = 1; name = GetTeamName(i); i++)
	{
		AddMenuItem(name,Format("AddBot(%d)",i), QICO, byClonk, 0, i, 0, 2,0);
		AddMenuItem(name,Format("RemoveBot(%d)",i), QICO, byClonk, 0, i, 0, 2,1); 
	}
	
	AddMenuItem("$NewTeam$", "NewTeam", QBOT, byClonk, 0, byClonk);
	
	// bots schwerer machen
	if(GetBotDifficulty() == 0)
	{
		AddMenuItem("$HarderBots$","SetBotDif",QBOT,byClonk,0,byClonk);
	}
	// bots leichter machen
	else
	{
		AddMenuItem("$EasierBots$","SetBotDif",QBOT,byClonk,0,byClonk);
	}
	
	CheckBotsInfo(byClonk);
}

func NewTeam(dummy, object byClonk)
{
	var sel = GetMenuSelection(byClonk);
	AddAI();
	byClonk->CloseMenu();
	ClearScheduleCall(this(), "CheckBotsInfo");
	ChangeBots(0, byClonk);
	SelectMenuItem(sel+2,byClonk);
}

func SetBotDif(dubby, object byClonk)
{
	var sel = GetMenuSelection(byClonk);
	SetBotDifficulty(!GetBotDifficulty());
	
	if(GetBotDifficulty())
		Log("$HarderBots$");
	else
		Log("$EasierBots$");
	
	byClonk->CloseMenu();
	ClearScheduleCall(this(), "CheckBotsInfo");
	ChangeBots(0, byClonk);
	SelectMenuItem(sel,byClonk);
}

func CheckBotsInfo(object byClonk)
{
	if(GetMenu(byClonk) != QBOT)
	{
		return;
	}
	
	ScheduleCall(this(),"CheckBotsInfo",1,0,byClonk);
	BotsInfo(byClonk);
}

func BotsInfo(object byClonk)
{
	var str = "$ActualSettings$:|";
	var name, i, players, bots, plr;
	players = CreateArray();
	bots = CreateArray();
	for(i = 0; i < GetPlayerCount(); i++)
	{
		plr = GetPlayerByIndex(i);
		// Scriptspieler?
		if(GetPlayerType(plr) == C4PT_Script)
			bots[GetPlayerTeam(plr)] += GetCrewCount(plr);
		else
			players[GetPlayerTeam(plr)] += GetCrewCount(plr);
	}
	for(i = 1; name = GetTeamName(i); i++)
	{
		str = Format("%s|%s: %d + %d Bots",str,name,players[i],bots[i]);
	}
	
	if(GetBotDifficulty())
		str = Format("%s||$HarderBots$",str);
	else
		str = Format("%s||$EasierBots$",str);
	
	PlayerMessage(GetController(byClonk), Format("@%s",str));
}

/** Blut **/

func ChangeBlood(dummy, object byClonk)
{
	var temp;
	BloodInfo(byClonk);

	CreateMenu(QBOT, byClonk, this(), 0, "$ChangeBlood$");
	SetMenuSize(3,0,byClonk);
	AddMenuItem("$NoBlood$", "SetBlood", QICO, byClonk, 0, 0, "$Blood1$", 2, 4);
	AddMenuItem("$NormalBlood$", "SetBlood", QICO, byClonk, 1, 1, "$Blood2$", 2, 5);
	AddMenuItem("$LotsOfBlood$", "SetBlood", QICO, byClonk, 2, 2, "$Blood3$", 2, 6);
	
	// Blut bleibt -> verschwinden Eintrag
	if(GetBloodDuration())
	{
		temp = CreateObject(QICO,0,0,-1);
		temp->SetPicture(384,0,64,64);
		SetGraphics("No",temp,QICO,1,GFXOV_MODE_Picture);
		SetObjDrawTransform(650,0,5000,0,650,5000, temp, 1);
		
		AddMenuItem("$BloodVanishes$", "SetBloodDuration", QBOT, byClonk, 0, 0, "$BloodDur1$", 4, temp);
		RemoveObject(temp);
	}
	else 	// Blut verschwindet -> bleiben Eintrag
	{
		temp = CreateObject(QICO,0,0,-1);
		temp->SetPicture(384,0,64,64);
		SetGraphics("Yes",temp,QICO,1,GFXOV_MODE_Picture);
		SetObjDrawTransform(650,0,5000,0,650,5000, temp, 1);
		
		AddMenuItem("$BloodStays$", "SetBloodDuration", QBOT, byClonk, 0, 1, "$BloodDur2$", 4, temp);
		RemoveObject(temp);
	}
	
	// Partikelblut an -> ausschalten?
	if(GetBloodType())
	{
		temp = CreateObject(QICO,0,0,-1);
		temp->SetPicture(448,0,64,64);
		SetGraphics("No",temp,QICO,1,GFXOV_MODE_Picture);
		SetObjDrawTransform(650,0,5000,0,650,5000, temp, 1);
		
		AddMenuItem("$DisableParticleBlood$", "SetBloodType", QBOT, byClonk, 0, 0, "$BloodParticle2$", 4, temp);
		RemoveObject(temp);
	}
	else 	// Partikelblut aus -> aktivieren?
	{
		temp = CreateObject(QICO,0,0,-1);
		temp->SetPicture(448,0,64,64);
		SetGraphics("Yes",temp,QICO,1,GFXOV_MODE_Picture);
		SetObjDrawTransform(650,0,5000,0,650,5000, temp, 1);
		
		AddMenuItem("$EnableParticleBlood$", "SetBloodType", QBOT, byClonk, 0, 1, "$BloodParticle1$", 4, temp);
		RemoveObject(temp);
	}
		
	// Leichenteile an -> aus?
	if(GetBodyPartsEnabled())
	{
		temp = CreateObject(QICO,0,0,-1);
		temp->SetPicture(448+64,0,64,64);
		SetGraphics("No",temp,QICO,1,GFXOV_MODE_Picture);
		SetObjDrawTransform(650,0,5000,0,650,5000, temp, 1);
		
		AddMenuItem("$DisableBodies$", "SetBodyParts", QBOT, byClonk, 0, false, "$BodyParts2$", 4, temp);
		RemoveObject(temp);
	}
	else 	// Leichenteile aus -> an?
	{
		temp = CreateObject(QICO,0,0,-1);
		temp->SetPicture(448+64,0,64,64);
		SetGraphics("Yes",temp,QICO,1,GFXOV_MODE_Picture);
		SetObjDrawTransform(650,0,5000,0,650,5000, temp, 1);
		
		AddMenuItem("$EnableBodies$", "SetBodyParts", QBOT, byClonk, 0, true, "$BodyParts1$", 4, temp);
		RemoveObject(temp);
	}
}

func SetBlood(dummy, int set)
{
	BloodCount = BoundBy(set,0,2);
	if(set == 0)
		Log("$BloodAmountSet$: $NoBlood$");
	if(set == 1)
		Log("$BloodAmountSet$: $NormalBlood$");
	if(set == 2)
		Log("$BloodAmountSet$: $LotsOfBlood$");
}

func SetBloodDuration(dummy, bool dur)
{
	if(GetLeague())
		return Log("Liga: $BloodVanishes$");

	BloodStays = dur;
	if(dur)
		Log("$BloodStays$");
	else
		Log("$BloodVanishes$");
}

func SetBloodType(dummy, bool enabled)
{
	if(GetLeague())
		return Log("Liga: $ParticleBloodEnabled$");

	ParticleBlood = enabled;
	if(enabled)
		Log("$ParticleBloodEnabled$");
	else
		Log("$ParticleBloodDisabled$");
}

func SetBodyParts(dummy, bool enabled)
{
	BodyParts = enabled;
	if(enabled)
		Log("$BodyPartsEnabled$");
	else
		Log("$BodyPartsDisabled$");
}

func BloodInfo(object byClonk)
{
	var str = "$ActualSettings$:";
	var countstr;
	if(BloodCount == 0)
		countstr = "$NoBlood$";
	if(BloodCount == 1)
		countstr = "$NormalBlood$";
	if(BloodCount == 2)
		countstr = "$LotsOfBlood$";

	var staystr;
	if(GetBloodDuration())
		staystr = "$BloodStays$";
	else
		staystr = "$BloodVanishes$";

	var typestr;
	if(GetBloodType())
		typestr = "$ParticleBloodEnabled$";
	else
		typestr = "$ParticleBloodDisabled$";
	
	var bodystr;
	if(GetBodyPartsEnabled())
		bodystr = "$BodyPartsEnabled$";
	else
		bodystr = "$BodyPartsDisabled$";

	PlayerMessage(GetController(byClonk), Format("@%s|%s|%s|%s|%s",str,countstr,staystr,typestr,bodystr)); 
}

/** Frags **/

func ChangeFrags(dummy, object byClonk)
{
	CreateMenu(QGDM, byClonk, this(), 0, "$ChangeFragCount$", 0, 0, true);
	AddMenuItem("1 $More$", "SetFrags", QICO, byClonk, 1, +1, 0, );
	AddMenuItem("5 $More$", "SetFrags", QICO, byClonk, 5, +5, 0, );
	AddMenuItem("1 $Less$", "SetFrags", QICO, byClonk, 1, -1, 0, 2,1);
	AddMenuItem("5 $Less$", "SetFrags", QICO, byClonk, 5, -5, 0, 2,1);

	CheckFragsInfo(byClonk);
}

func SetFrags(dummy, int change)
{
	var goal = CheckGoal();
	var score = Max(1,GetWinScore(goal) + change);
	SetWinScore(score, goal);
} 

func CheckFragsInfo(object byClonk)
{
	if(GetMenu(byClonk) != QGDM)
	{
		Log("$FragsSet$: %d $Points$",GetWinScore(CheckGoal()));
		return;
	}
		
	ScheduleCall(this(),"CheckFragsInfo",1,0,byClonk);
	FragsInfo(byClonk);
}

func FragsInfo(object byClonk)
{
	var str = "$ActualSettings$:";
	
	PlayerMessage(GetController(byClonk), Format("@%s|%d $Points$",str,GetWinScore(CheckGoal())));
}


/** Restliches Zeug **/

func OnMenuSelection(int index, object byClonk)
{
	if(GetMenu(byClonk) != QMNG)
		return;
		
	if(index == 0)
		BotsInfo(byClonk);
	if(index == 1)
		BloodInfo(byClonk);
	if(index == 2)
		FragsInfo(byClonk);
}

func MenuQueryCancel(int sel, object obj)
{
	PlayerMessage(GetController(obj),"");
}

func FxGiveInfoTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	var ID = GetMenu(pTarget);
	if(!(ID == QMNG || ID == QBOT || ID == QICO || ID == QGDM))
		return(-1);
}

func FxGiveInfoStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
	PlayerMessage(GetController(pTarget),"");
	StopInvincibility(pTarget);
}


// Hazard-Changes
global func AddBot(int iTeam)
{
	if(NoBots)
		return;

  // Spieler suchen
  for(var i=0, found = -1 ; i < GetPlayerCount(C4PT_Script) ; i++)
    if(GetPlayerTeam(GetPlayerByIndex(i, C4PT_Script)) == iTeam)
    {
      found = GetPlayerByIndex(i, C4PT_Script);
      break;
    }
  // Spieler vorhanden?
  if(found == -1)
  {
  	AddAI(iTeam);
  	return;
  }
  // Clonk erstellen
  var bot = CreateObject(QBOT, 0,0, found); // wir wollen einen Quakebot!
  MakeCrewMember(bot, found);
  bot->SetRandomSkin();
  GameCall("RelaunchPlayer", found, bot, -1);
  return bot;
}

global func RemoveBot(int iTeam)
{
  // Spieler suchen
  for(var i=0, found = -1 ; i < GetPlayerCount(C4PT_Script) ; i++)
    if(GetPlayerTeam(GetPlayerByIndex(i, C4PT_Script)) == iTeam)
    {
      found = GetPlayerByIndex(i, C4PT_Script);
      break;
    }
  // Spieler vorhanden?
  if(found == -1) return;
  // Clonk aus der Crew entfernen
  var bot = GetCrew(found);
  if(bot)
  {
  	if(bot->Contained())
  	{
    	if(bot->Contained()->GetID() == TIM1 || bot->Contained()->GetID() == TIM2)
    	{
    		bot->Contained()->RemoveObject();
    		bot->DeleteMe();
	    }
	  }
    else
      bot->DeleteMe();
  }
}


/**** Unverwundbarkeits - Effekt ****/
func MakeInvincible(object obj)
{
	// nicht wahrend liga
	if(GetLeague())
		return;

	AddEffect("ManagerInvincibility",obj,300,1,this,QMNG,45);
	AddEffect("ManagerInvincibility",obj,300,1,this,QMNG,120+45);
	AddEffect("ManagerInvincibility",obj,300,1,this,QMNG,240+45);
	
	var dummy = obj->CreateObject(QDUM,0,10,obj->GetOwner());
	dummy->Local(0) = GetXDir(obj);
	dummy->Local(1) = GetYDir(obj);
	dummy->Local(2) = GetAction(obj);
	Enter(dummy, obj);
	dummy->SetGraphics(0,dummy,obj->GetID(), 1, GFXOV_MODE_Object, 0, 0, obj);
}

func StopInvincibility(object obj)
{
	var e;
	while(e = GetEffect("ManagerInvincibility",obj))
		RemoveEffect(0,obj,e);
	
	var dummy = Contained(obj);
	if(!dummy)
		return; //wth ist hier passiert? :S
	
	dummy->Exit(obj, 0, 10);
	obj->SetAction(dummy->Local(2));
	obj->SetXDir(dummy->Local(0));
	obj->SetYDir(dummy->Local(1));
	RemoveObject(dummy);
}

func FxManagerInvincibilityStart(object pTarget, int iEffectNumber, int iTemp, int offset)
{
	if(iTemp)
		return(0);
	EffectVar(0, pTarget, iEffectNumber) = offset;
	EffectVar(1, pTarget, iEffectNumber) = offset*7/360;
}


func FxManagerInvincibilityTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	var maxtime = 20;

	var time = EffectVar(1, pTarget, iEffectNumber)++;

	var a;
	var x,y;
	a = (EffectVar(0, pTarget, iEffectNumber) + time*10)%360;
	x = Sin(a, Sin(time*(180/maxtime),maxtime-time));
	y = 10-time;
	
	var color = HSL(iEffectTime%256, 200, 150);
	
	pTarget->CreateParticle("PSpark",x,y, 0,-3, 20, color,pTarget, a > 90 && a <= 270);
	
	if(time > maxtime)
	{
		EffectVar(1, pTarget, iEffectNumber) = 0;
	}
}

func FxManagerInvincibilityDamage(object pTarget, int iEffectNumber, int dmg, int cause)
{
	return(0);
}


global func IsInvincible(object obj)
{
	if(!obj)
		obj = this();
	
	return(GetEffect("ManagerInvincibility",obj));
}
