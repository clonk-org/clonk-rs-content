#strict

protected func Initialize()
{
  // Besonderheiten
  CreateObject(_Z9A,1000,2560,-1)->Set(1);
  CreateObject(_Z9A,550,1270,-1)->Set(2);
  CreateObject(_Z9A,90,380,-1)->Set(3);
  // Spielziel
  var pGoal=FindObject2(Find_ID(CROB));
  if(!pGoal) pGoal=CreateObject(CROB,0,0,-1);
  pGoal->AddType(_W3A, 5);
  pGoal->AddType(_Y6B, 20);
  pGoal->AddType(_W2A, 1);
  pGoal->SetMissionAccess("SoBeRdW6");
  
  //Windstrom();
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
  if(para==1) return("Eishöhlen"); 
  if(para==2) return("Stiefelhöhe");
  if(para==3) return("Himmelsinseln");
  return("Komisches Unbekanntes Ding");
}

func OnGoalsFulfilled()
{
  GainMissionAccess("SoBeRdW6");
  return(0);
}

func InitializePlayer(iPlr)
{
  SetPlrKnowledge(iPlr,_W3A);
  return(1);
}

public func IsCampaignMission()
{
  return true;
}

// Gibt das forcierte Zeitalter des Szenarios zurück (für den Regelwähler)
func ForcedTechLevel()
{
  return RdW_TL_Modern;
}
