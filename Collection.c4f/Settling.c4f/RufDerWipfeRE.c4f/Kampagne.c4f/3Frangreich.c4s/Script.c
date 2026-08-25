#strict

static const AI_Player=1;

static g_pDuChampagnje;

protected func Initialize()
{
  // Parallax
  SetSkyParallax(0, 15, 11, 0, 0, 0, -10);
  // Besonderheiten
  CreateObject(_Z9A,1420,800,-1)->Set(1);
  CreateObject(_Z9A,2090,436,-1)->Set(2);
  // Wipfenstein
  var pWipfstone=CreateObject(FEST,2090,436,AI_Player);
  // Die Besatzung...
  g_pDuChampagnje=CreateObject(CHAM,2090,436,AI_Player);g_pDuChampagnje->AddFightAI();Enter(pWipfstone,g_pDuChampagnje);
  var pClonk=CreateObject(CLNK,2030,436,AI_Player);pClonk->AddFightAI();Enter(pWipfstone,pClonk);
  pClonk=CreateObject(CLNK,2150,436,AI_Player);pClonk->AddFightAI();Enter(pWipfstone,pClonk);
  CreateObject(CLNK,2320,516,AI_Player)->AddFightAI();
  CreateObject(CLNK,2260,436,AI_Player)->AddFightAI();
  // ..., die Munition...
  CreateContents(EFLN,pWipfstone,1);
  CreateContents(SFLN,pWipfstone,3);
  CreateContents(FLNT,pWipfstone,8);
  CreateContents(ROCK,pWipfstone,3);
  // ...und das Torhaus davor
  pClonk=CreateObject(CLNK,1900,656,AI_Player);pClonk->AddFightAI();
  CreateObject(TWR2,1870,676,AI_Player)->ControlLeft(pClonk);
  // Spielziel
  var pGoal=FindObject2(Find_ID(RDWD));
  if(!pGoal) pGoal=CreateObject(RDWD,0,0,-1);
  pGoal->Set(_BAE);
  pGoal->SetMissionAccess("SoBeRdW3");
  return(1);
}

func LocalPlaceName(para)
{
  if(para==1) return("Plätscherbach");
  if(para==2) return("Burg Wipfenstein");
  return("Komisches Unbekanntes Ding");
}

func OnClonkDeath(pClonk)
{
  if(pClonk==g_pDuChampagnje)
    CreateObject(ELPL,GetX(pClonk),GetY(pClonk),0);
}

func InitializePlayer(iPlr)
{
  // Mit "Du Champagnje" verfeinden
  SetHostility(iPlr,AI_Player,1,true,true);
  SetHostility(AI_Player,iPlr,1,true,true);
  return(1);
}

func ForcedTechLevel()
{
  return RdW_TL_Knight;
}

public func IsCampaignMission()
{
  return true;
}
