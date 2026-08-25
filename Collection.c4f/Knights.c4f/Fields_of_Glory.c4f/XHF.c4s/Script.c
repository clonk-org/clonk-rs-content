/*-- FoG --*/

#strict

//Relaunch anzahl:
static const MaxRelaunch = 10;
static const AutoScoreboardShowTime = 5; // sekunden

// Globale Arrays
private func & Relaunches(int iPlr) { return (Global(iPlr*3)); }

static team1;
static team2;
static relaunch;
static kill;
static iDeathCausedBy;
static iPotions;

protected func InitializePlayer(int player, X, Y, foo, iTeam)
{    
  relaunch[player] = MaxRelaunch;
  kill[player] = 0;
	// Den Spieler ausrüsten und platzieren
	CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->InitPlayer(player,iTeam);
  UpdateScoreBoard(player,false);
  if (iTeam == 1)
  {
  SetLength (team1, GetLength(team1));
  team1[GetLength(team1)] = player;
  }
  if (iTeam == 2)
  {
  SetLength (team2, GetLength(team2));
  team2[GetLength(team2)] = player;
  }
  return(1);
}

func Initialize()
{
   team1 = [];
   team2 = [];
   relaunch = [];
   kill = [];
   iPotions = [PFIR,PHEA,PSTO,PMAN];
// Spark vorhanden?
// Das Spark-Objekt für Zufallsobjekte vom Himmel initialisieren
// RemoveAll(_SPK);
// CreateObject(_SPK, LandscapeWidth()/2 ,-5, -1)->SetAction("Wait");
   // Zähler starten
   ScriptGo(1);
   return(1);
}

public func Script10()
{
  SetMaxPlayer(0);
  SetPlayerGrades();
  return(ScriptGo(0));
}

public func GiveKillCount(int killer,int killed)
{
	if(killer == -1) return(1);
	if (GetPlayerTeam(killer) == GetPlayerTeam(killed))
		{
		// Blitz erzeugen
		CreateObject(LGTS, 0, 0, killer)->Activate(GetX(GetCursor(killer))+GetVertex(0, VTX_X, GetCursor(killer)), GetY(GetCursor(killer))+GetVertex(0, VTX_Y, GetCursor(killer))-30, 0, 20, Sin(100, 10, 10));
		Sound("Thunder*");
		return(1);
		}
	if (GetPlayerTeam(killer) != GetPlayerTeam(killed))
		{
		kill[killer] ++;
		var j=0;
		if( FindObject2( Find_ID(KING), Find_OCF(OCF_Alive), Find_Allied(killer) ) ) j=1;
		CreateContents(iPotions[Random(GetLength(iPotions)-1+j)],GetCursor(killer));
		ShiftContents(GetCursor(killer));

		if (kill[killer] == 3)
			{
			CreateContents(SRL2,GetCursor(killer));
			}
		if (kill[killer] == 6)
			{
			//
			Sound("Promote");
			var IsPromoted = 0;
			var pClonk = GetCursor(killer);
			if( GetID(pClonk) == KING )
				{
				SetPlrMagic(killer, XCRS);
				IsPromoted = 1;
				}
			if( GetID(pClonk) == KNIG )
				{
				if (pClonk->HasArmor()) pClonk->SetArmored(1, BRMR);
				else
				pClonk->Redefine(KING);
				pClonk->SetPhysical("Magic", 80000, 2);
				if (pClonk->HasShield()) pClonk->SetGraphics("Shield", pClonk, KING, 10, GFXOV_MODE_ExtraGraphics);
				IsPromoted = 1;
				}
			if( IsPromoted != 1 )
			{
			pClonk->Redefine(_SKG);
			pClonk->SetPhysical("Magic", 80000, 2);
			if (pClonk->HasShield()) pClonk->SetGraphics("Shield", pClonk, _SKG, 10, GFXOV_MODE_ExtraGraphics);
			}
			//
			}
		}
	UpdateScoreBoard(killer,true);
}



public func OnClonkDeath(pClonk, int iMoerder){
iDeathCausedBy = iMoerder;
if( FindObject2( Find_Owner(GetOwner(pClonk)), Find_OCF (OCF_CrewMember)) )	GiveKillCount(iMoerder,GetOwner(pClonk)); 
return(1);
}

public func RelaunchPlayer(int iPlr){
if (relaunch[iPlr] == 1)
	{
	GiveKillCount(iDeathCausedBy,iPlr);
	return(1);
	}
  relaunch[iPlr] --;

  var pObj = CreateObject(KNIG,0,0,iPlr); 
  SetOwner(iPlr,pObj);
  MakeCrewMember(pObj, iPlr);
  pObj->DoEnergy(100);
  SetCursor(iPlr, pObj);
  CreateObject(_PLO, LandscapeWidth() / 2, 50, iPlr)->RelaunchPlayer(iPlr);

  //wenn kein selbstmord dann dem töter den kill geben:
  if (iDeathCausedBy != iPlr) {
  if(iDeathCausedBy != -1) Log("%s starb ehrenvoll durch %s. Noch <c ff0000>%d</c> Relaunchs",GetTaggedPlayerName(iPlr),GetTaggedPlayerName(iDeathCausedBy),relaunch[iPlr],GiveKillCount(iDeathCausedBy,iPlr));
    UpdateScoreBoard(iPlr,true);  
    return(1);
  }
  //ansonsten selbstmord verkünden
  Log("%s starb ehrenvoll durch sich selbst. Noch <c ff0000>%d</c> Relaunchs",GetTaggedPlayerName(iPlr),relaunch[iPlr]);
  UpdateScoreBoard(iPlr,true);
  return(1);
}

global func UpdateScoreBoard(int iPlr, Show)	{
	if(iPlr == -1) return(0);
	
	var iPlrID = GetPlayerID(iPlr), iColID;
  // Spielername
  SetScoreboardData(SBRD_Caption, SBRD_Caption, "Kills / Relaunches",       SBRD_Caption);
  SetScoreboardData(iPlrID,       SBRD_Caption, GetTaggedPlayerName(iPlr), iPlrID);
  // Relaunches
  SetScoreboardData(SBRD_Caption, 1,       " {{KNIG}} ",                         1);
  SetScoreboardData(iPlrID,       1,       Format("%d", relaunch[iPlr]),         relaunch[iPlr]);
  // Kills
  SetScoreboardData(SBRD_Caption, 2,       " {{MGDW}} ",                         2);
  SetScoreboardData(iPlrID,       2,       Format("%d", kill[iPlr]),             kill[iPlr]);
  SortScoreboard(2, true);
  SortScoreboard(1, true);

    if (Show)
    {
    DoScoreboardShow(1, iPlr+1);
    Schedule(Format("DoScoreboardShow(-1, %d)", iPlr+1), 38*AutoScoreboardShowTime);
    }
}

protected func RemovePlayer(int iPlr)
{
	SetScoreboardData(GetPlayerID(iPlr),       1,       "{{SKUL}}",         1);
	SortScoreboard(1, true);
	return(1);
}


// König und Hauptman verteilen
protected func SetPlayerGrades()
{

	if (GetLength(team1) > 0)
	{
	var iNumbera = Random(GetLength(team1));
	var pKnig1 = FindObject2(Find_ID(KNIG),Find_Owner(team1[iNumbera]));
	CreateObject(SHIE)->Activate(pKnig1);
	pKnig1->SetArmored(1, ARMR);
	}

	if (GetLength(team1) > 1)
	{
	var iNumberb = Random(GetLength(team1));
	while (iNumberb == iNumbera)
	var iNumberb = Random(GetLength(team1));
	var pKing1 = FindObject2(Find_ID(KNIG),Find_Owner(team1[iNumberb]));
	pKing1->Redefine(KING);
	pKing1->SetPhysical("Magic", 80000, 2);
	CreateObject(SHIE)->Activate(pKing1);
	CreateContents(PMAN,pKing1);
	}

	if (GetLength(team2) > 0)
	{
	var iNumberc = Random(GetLength(team2));
	var pKnig2 = FindObject2(Find_ID(KNIG),Find_Owner(team2[iNumberc]));
	CreateObject(SHIE)->Activate(pKnig2);
	pKnig2->SetArmored(1, ARMR);
	}

	if (GetLength(team2) > 1)
	{
	var iNumberd = Random(GetLength(team2));
	while (iNumberd == iNumberc)
	var iNumberd = Random(GetLength(team2));
	var pKing2 = FindObject2(Find_ID(KNIG),Find_Owner(team2[iNumberd]));
	pKing2->Redefine(KING);
	pKing2->SetPhysical("Magic", 80000, 2);
	CreateObject(SHIE)->Activate(pKing2);
	CreateContents(PMAN,pKing2);
	}
	return(1);
}