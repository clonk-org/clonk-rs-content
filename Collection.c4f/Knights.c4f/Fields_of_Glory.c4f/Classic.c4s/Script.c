/*-- FoG --*/

#strict

//Relaunch anzahl:
static const MaxRelaunch = 2;
static const AutoScoreboardShowTime = 5; // sekunden

// Globale Arrays
private func & Relaunches(int iPlr) { return (Global(iPlr*3)); }

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
  return(1);
}

func Initialize()
{
   Schedule("SetMaxPlayer(0)",100);
   relaunch = [];
   kill = [];
   iPotions = [PFIR,PHEA,PSTO,PMAN];
// Spark vorhanden?
// Das Spark-Objekt für Zufallsobjekte vom Himmel initialisieren
// RemoveAll(_SPK);
// CreateObject(_SPK, LandscapeWidth()/2 ,-5, -1)->SetAction("Wait");
  return(1);
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
				SetPlrMagic(killer, RUND);
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