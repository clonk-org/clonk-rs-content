/*-- Scoreboard --*/

#strict

static const AutoScoreboardShowTime = 5; // sekunden
static Account;
static Points;
static ScoreToDo;
static AccountToDo;

func Initialize()
{
// Arrays
Account = [];
Points = [];
ScoreToDo = [];
AccountToDo = [];
return(1);
}

public func Timer()
{
for(var i = 0; i < GetTeamCount(); i++)
	UpdateScoreBoard(GetTeamByIndex(i),false);
return(0);
}

global func GetTeamPlayer(iTeam)
{
var iPlr = -1;
for(var i = 0; i < GetPlayerCount(); i++)
	if (GetPlayerTeam(GetPlayerByIndex(i))==iTeam)
	{
	iPlr=GetPlayerByIndex(i);
	break;
	}
return(iPlr);
}

global func RemoveTeamScoreData(int iTeam)
{
	SetScoreboardData(iTeam,       1,       "{{SKUL}}",         1);
	SetScoreboardData(iTeam,       2,       "{{SKUL}}",         2);
	SortScoreboard(2, true);
	SortScoreboard(1, true);	
	return(1);
}

global func ShowTeamScore(iTeam)
{
for(var iPlr = 0; iPlr < GetPlayerCount(); iPlr++)
	if (GetPlayerTeam(iPlr)==iTeam)
		{
		DoScoreboardShow(1, iPlr+1);
		Schedule(Format("DoScoreboardShow(-1, %d)", iPlr+1), 38*AutoScoreboardShowTime);
		}
return(1);
}

global func UpdateScoreBoard(int iTeam, Show)	{

	var iPlr = GetTeamPlayer(iTeam);
	if(iPlr == -1)
		{
		if (Points[iTeam] != 0 && Account[iTeam] != 0) RemoveTeamScoreData(iTeam);
		return(0);
		}
	Account[iTeam] = GetWealth(iPlr);
	Points[iTeam] = GetPlrValueGain(iPlr);
	ScoreToDo[iTeam] = FindObject(VALG)->GetTargetScore(iPlr);
	AccountToDo[iTeam] = LocalN ("TargetWealth", FindObject(MONE)); 
	var iColID;

  // Überschrift
  SetScoreboardData(SBRD_Caption, SBRD_Caption, "Score",            SBRD_Caption);
  // Teamname
  SetScoreboardData(iTeam,        SBRD_Caption, Format("<c %x>%s", GetTeamColor(iTeam), GetTeamName(iTeam)), iTeam);
  // Punkte
  SetScoreboardData(SBRD_Caption, 1,       " {{GOAL}} ",                         1);
  SetScoreboardData(iTeam,       1,       Format("%d/%d", Points[iTeam], ScoreToDo[iTeam]),         Points[iTeam]);
  // Konto
  SetScoreboardData(SBRD_Caption, 2,       " {{GLDM}} ",                         2);
  SetScoreboardData(iTeam,       2,       Format("%d/%d", Account[iTeam], AccountToDo[iTeam]),         Account[iTeam]);
  SortScoreboard(1, true);

  if (Show) ShowTeamScore(iTeam);
}