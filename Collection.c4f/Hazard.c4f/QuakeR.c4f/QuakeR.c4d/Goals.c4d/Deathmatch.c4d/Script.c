/*-- Deathmatch --*/

#strict 2
#include GTDM

local scoreleft;

func Initialize()
{
	scoreleft = 3;
	_inherited();
}

public func ScoreChange(int iTeam, int iChange)
{
	var oldleader = GetLeadingTeam();
  aPoints[iTeam] += iChange;
  var newleader = GetLeadingTeam();
  
  // wir hatten einen leader, jemand hat aber aufgeholt
	if(newleader == -1 && oldleader != -1)
	{
		// alter leader hat Führung verloren
		PlayTeamSound("lostlead", oldleader);
		// neuer leader hat aufgeholt
		PlayTeamSound("tiedlead", iTeam);
	}
	// wir hatten keinen leader, aber jetzt haben wir einen
	else if(newleader != -1 && oldleader == -1)
		PlayTeamSound("takenlead", iTeam);
	
  
  // 3 Frags left!
  if(iWinScore - aPoints[iTeam] == 3 && scoreleft == 3)
  {
  	Sound("3_frags",true);
  	scoreleft = 2;
  }
  // 2 Frags left!
  else if(iWinScore - aPoints[iTeam] == 2 && scoreleft == 2)
  {
  	Sound("2_frags",true);
  	scoreleft = 1;
  }
  // 1 Frags left!
  else if(iWinScore - aPoints[iTeam] == 1 && scoreleft == 1)
  {
  	Sound("1_frag",true);
  	scoreleft = 0;
  }
  
  UpdateScoreboard(iTeam);
  CheckTime();
}

func GetLeadingTeam()
{
	var max, leader;
	var i;
	for(i = 0; i < GetLength(aPoints); i++)
	{
		if(aPoints[i] == max)
			leader = -1;
		if(aPoints[i] > max)
		{
		 	leader = i;
		 	max = aPoints[i];
		}
	}
	
	return leader;
}

func PlayTeamSound(string snd, int team)
{
	var plr;
	for(var i = 0; i < GetPlayerCount(); i++)
		if(GetPlayerTeam(plr = GetPlayerByIndex(i)) == team)
			Sound(snd, true, 0,0,plr+1);
}


public func WinScoreChange(int iNewScore)
{
  iWinScore = iNewScore;
  SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("%d Frags",iWinScore), SBRD_Caption);
}




/* KI */
func OnMacroCommandSuccess(object byClonk)
{
	AITactic(0, byClonk);
}

func OnMacroCommandFailed(object clonk, int reason)
{
	// debug.
	//Log("%s(%d) Failed: %d", GetName(clonk),GetOwner(clonk),reason);
}

public func AITactic(object pAIPlayer, object pCrew2)
{
	var owner, pCrew, single, gotcommand;
	if(!pAIPlayer && pCrew2)
	{
  	single = true;
  }
  else if(pAIPlayer && !pCrew2)
  	owner = pAIPlayer->GetOwner();
  else
  	return;

  for(var i = 0; pCrew = GetCrew(owner,i) ; i++)
  {
  	gotcommand = false;
  	if(single)
  		pCrew = pCrew2;
  	// wir sind wo drin?
    if(pCrew->Contained())
    	continue;
    
    // tun wir bereits was?
    if(pCrew->GetMacroCommand())
    	continue;
    
    // Energiemangel?
    if(pCrew->GetEnergy() < 75)
    	if(pCrew->SearchMedikit(Aggro_Shoot))
				gotcommand = true;
    
    // Wieviele Waffen haben wir?
    if(!gotcommand && pCrew->CustomContentsCount("IsWeapon") < 3+Random(2))
    	if(pCrew->SearchWeapon(Aggro_Shoot))
    		gotcommand = true;
    
    // brauchen wir Ammo?
    if(!gotcommand && !pCrew->CheckMyAmmo())
    	if(pCrew->SearchAmmo(Aggro_Shoot))
    		gotcommand = true;
    
    // haben wir Teamkollegen?
    var colleagues = FindObjects(Find_ID(GetID(pCrew)),Find_Not(Find_Hostile(GetOwner(pCrew))), Find_Exclude(pCrew));
    // zu einem Laufen!
    if(GetLength(colleagues) >= 1 && Random(2))
    	pCrew->SetMacroCommand(this(), "Follow", colleagues[Random(GetLength(colleagues))], 0,0,0, Aggro_Follow);
    
    // k, Gegner suchen!
    if(!gotcommand && Random(2))
    	pCrew->SetMacroCommand(this(), "MoveTo", FindObject2(Find_ID(WAYP), Sort_Random()), 0,0,0, Aggro_Follow);
    	
    if(single)
    	break;
  }
  return(true);
}

public func IsMutableWinScoreGoal()	{ return true; }
