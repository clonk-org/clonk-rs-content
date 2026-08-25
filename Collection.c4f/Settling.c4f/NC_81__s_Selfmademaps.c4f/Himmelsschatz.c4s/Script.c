#strict 2

/*-- Schatzsuche --*/

func Initialize()
{
  // Spielziel setzen
  FindObject(RMOB)->AddType(_SKL, 0);
  FindObject(RMOB)->SetMissionAccess("SkullGone");
  // Schädel erzeugen
  CreateObject(_SKL,  442, 1105, -1);
  CreateObject(_SKL, 6, 672, -1);
  CreateObject(_SKL, 131, 135, -1);
  CreateObject(_SKL, 1094, 366, -1);
  CreateObject(_SKL, 1603, 600, -1);
  CreateObject(_SKL, 1515, 80, -1);
  return 1;
}

static g_fInitializedPlayers;

func InitializePlayer(iPlr)
{
  GetCrew(iPlr)->SetPosition(195, 888);
  CreateContents(WOOD,GetCrew(iPlr,0));
  CreateContents(WOOD,GetCrew(iPlr,0));
  CreateContents(WOOD,GetCrew(iPlr,0));
  CreateContents(WOOD,GetCrew(iPlr,0));
  CreateContents(ROCK,GetCrew(iPlr,0));
  CreateContents(FLAG,GetCrew(iPlr,0));
  CreateContents(CNKT,GetCrew(iPlr,0));

}

func TACC_BlockFirstAllianceAdd() { return 1; }
