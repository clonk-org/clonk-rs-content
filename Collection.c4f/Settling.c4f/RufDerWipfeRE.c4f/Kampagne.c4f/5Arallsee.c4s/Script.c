#strict

protected func Initialize()
{
  // Parallax
  SetSkyParallax(0, 15, 0, 0, 0, 0, -10);
  // Besonderheiten
  CreateObject(_Z9A,730,710,-1)->Set(1);
  CreateObject(_Z9A,60,1580,-1)->Set(2);
  // Rakete
  CreateObject(RAKT,65,1578,-1);

  // Spielziele
  var pGoal=FindObject2(Find_ID(RDWD));
  if(!pGoal) pGoal=CreateObject(RDWD,0,0,-1);
  pGoal->Set(_BAW);
  
  pGoal=FindObject2(Find_ID(CROB));
  if(!pGoal) pGoal=CreateObject(CROB,0,0,-1);
  pGoal->AddType(_W2A,1);
  
  Windstrom();
  return(1);
}

func Windstrom()
{
  Log("Winddaten werden eingelesen");
  WINDAUFLOESUNG=WindAufloesung();
  globalwestwinde=DoWhatIWWant();
  globalostwinde=DoWhatIWEant();
  horizont=DoWhatIWHant();
  return(1);
}

func LocalPlaceName(para)
{
  if(para==1) return("Himmelsinseln"); 
  if(para==2) return("Klotzberg");
  return("Komisches Unbekanntes Ding");
}

func OnGoalsFulfilled()
{
  GainMissionAccess("SoBeRdW5");
  return(0);
}

func ForcedTechLevel()
{
  return RdW_TL_Industry;
}

public func IsCampaignMission()
{
  return true;
}
