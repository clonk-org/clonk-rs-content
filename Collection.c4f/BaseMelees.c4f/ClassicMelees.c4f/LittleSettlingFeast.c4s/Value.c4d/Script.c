/*-- Siedlungspunkte --*/

/* Umgebaut, so dass es mit den Team Siedlungspunkten zusammenarbeitet */

#strict
#include GOAL

// Punktzahl
local TargetScore;

protected func Initialize()
{
  // Anderes Objekt vorhanden? Selber dazuzählen
  var pGoal;
  if (pGoal = FindObject(GetID()))
  { 
    LocalN("TargetScore", pGoal) += 100;
    return(RemoveObject()); 
  }
  // Punktzahl setzen
  TargetScore = 100;
  return(inherited());
}
    
private func AllPlayersHaveValue(iValue)
{
  var iPlayers, i, iFactor = 1;
  var fUseTeamAccount = (FindObject2(Find_ID(TACC)) && !GameCall("TACC_BlockTeamExchange", TACC_TypeScore));
  for (iPlayers = GetPlayerCount(); iPlayers; i++)
    if (GetPlayerName(i))
    {
      iPlayers--;
      if(fUseTeamAccount)
	{
	 var iAccount = FindObjectOwner(ACNT, i);
	 if (iAccount) iFactor = iAccount->GetAlliancePlayerCount(1);
	}
      if (GetPlrValueGain(i) < ApplyFactor(iValue, iFactor)) return(0);
    }
  return(1);
}

private func ApplyFactor(iValue, iFactor)
{
  // Alleine muss der Spieler 100% schaffen, zu zweit 150% zu dritt 175% usw.
  iFactor = 2000 - 1000/(2**(iFactor-1));
  return iValue * iFactor / 1000;
}
    
public func IsFulfilled()
{
  if (!TargetScore) TargetScore = 100;
  return(AllPlayersHaveValue(TargetScore));
}

public func IsFulfilledforPlr(iPlr) 
{
  if(iPlr==-1) return();
  var iFactor = 1;
  if(FindObject2(Find_ID(TACC)) && !GameCall("TACC_BlockTeamExchange", TACC_TypeScore))
    iFactor = FindObjectOwner(ACNT, iPlr)->GetAlliancePlayerCount(1);
  return (GetPlrValueGain(iPlr) >= ApplyFactor(TargetScore, iFactor));
}

protected func Activate(iPlayer)
{
  var iFactor = 1;
  if(FindObject2(Find_ID(TACC)) && !GameCall("TACC_BlockTeamExchange", TACC_TypeScore))
    iFactor = FindObjectOwner(ACNT, iPlayer)->GetAlliancePlayerCount(1);
  if (IsFulfilled()) 
    return(MessageWindow(Format("$MsgGoalFulfilled$", ApplyFactor(TargetScore, iFactor)), iPlayer));
  MessageWindow(Format("$MsgGoalUnfulfilled$", GetPlrValueGain(iPlayer), ApplyFactor(TargetScore, iFactor)), iPlayer);
  return(1);
}

public func GetTargetScore(iPlayer)
{
  var iFactor = 1;
  if(FindObject2(Find_ID(TACC)) && !GameCall("TACC_BlockTeamExchange", TACC_TypeScore))
    iFactor = FindObjectOwner(ACNT, iPlayer)->GetAlliancePlayerCount(1);
  var SendScore = ApplyFactor(TargetScore, iFactor);
  return(SendScore);
}

public func RemovePlayer(iPlayer)
{
  // Beim Tod oder Leaven kleiner Ausgleich für das Team
  var iTeamStrength;
  var iTeam = GetPlayerTeam(iPlayer);
  var iPlayers, i;
  for (iPlayers = GetPlayerCount(); iPlayers; i++)
    if (GetPlayerName(i))
    {
      iPlayers--;
      if(GetPlayerTeam(i)==iTeam) iTeamStrength++;
    }
  var iScoreToDo = GetTargetScore(iPlayer) - GetPlrValueGain(iPlayer);
  var iNewFactor;
  if (iTeamStrength<=1) iNewFactor = 0;
  if (iTeamStrength==2) iNewFactor = 1000/3;
  if (iTeamStrength==3) iNewFactor = 1000/6;
  if (iTeamStrength==4) iNewFactor = 1000/12;
  if (iTeamStrength==5) iNewFactor = 1000/24;
  if (iTeamStrength>=6) iNewFactor = 1000/48;
  var iTeammate = GetOwner(FindObject2(Find_Not(Find_Owner(iPlayer)),Find_OCF(OCF_CrewMember),Find_Allied(iPlayer)));
  var iAddScore = iScoreToDo*iNewFactor/1000;
  DoScore(iTeammate, iAddScore);
}
