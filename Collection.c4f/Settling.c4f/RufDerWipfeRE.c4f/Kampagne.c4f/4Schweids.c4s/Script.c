#strict

static g_pMine;

protected func Initialize()
{
  // Parallax
  SetSkyParallax(0, 30, 20, 0, 0, 0, -150);
  // Besonderheiten
  CreateObject(_Z9A,4940,570,-1)->Set(1);
  CreateObject(_Z9A,2570,340,-1)->Set(2);
  CreateObject(_Z9A,20,980,-1)->Set(3);
  // Strommästen,Umspannwerk und das Bergwerk
  CreateObject(_G6A,110,1025);
  ObjectSetAction(CreateObject(PWRL),"Connect",
	CreateObject(_G6A,4740,560),
	g_pMine=CreateObject(_L1A,4850,560));
  CreateObject(_A1A,4790,560);
  CreateObject(UMSP,50,1025,-1);
  // Seilbahn
  var soben,sunten,kabel,kabine;
  soben=CreateObject(SBST,3160,260,-1);
  ObjectCall(soben,"TurnLeft");
  sunten=CreateObject(SBST,2050,770,-1);
  kabine=CreateObject(SBKA,10,10,-1);
  ObjectCall(kabine,"Travel",soben,sunten);
  // Das wichtigste, ein Schneemann
  CreateObject(SCHN,1390,1280,-1);
  return(1);
}

func UBRLTarget()
{
  return g_pMine;
}

func LocalPlaceName(para)
{
  if(para==1) return("Bergwerk"); 
  if(para==2) return("Jodelübahn");
  if(para==3) return("Wipfenstein-Leitung");
  return("Komisches Unbekanntes Ding");
}

// hier intern weil es ja 2 Spielziele sind
func OnGoalsFulfilled()
{
  GainMissionAccess("SoBeRdW4");
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
