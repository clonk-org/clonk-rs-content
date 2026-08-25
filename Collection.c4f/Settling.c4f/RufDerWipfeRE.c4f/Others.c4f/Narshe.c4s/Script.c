/*- Narshe -*/
#strict

protected func Initialize()
{
  // Parallax
  SetSkyParallax(0, 20, 20, 0, 0, 0,-300);
  // Besonderheiten
  CreateObject(_Z9A,0200,320,-1)->Set(1);
  CreateObject(_Z9A,1950,690,-1)->Set(2);
  // Bahnhof
  CreateObject(BAHN, 1904, 699, -1);
  // Eisenhütte
  CreateObject(_F1A, 174, 448, -1);
  // Fahrstuhl
  var pElevator=CreateObject(_H1A, 254, 410, -1);
  // Windrad
  var pWindmill=CreateObject(_G1A, 126, 255, -1);
  // Steinhütte
  CreateObject(_A2A, 190, 331, -1);
  // Krankenhaus
  CreateObject(_Q1A, 60, 257, -1);
  // Kohlekraftwerk
  var pPowerStation=CreateObject(_G2A, 412, 417, -1);
  // Schmiede
  CreateObject(_K1A, 326, 417, -1);
  // Kaserne
  CreateObject(_M1A, 826, 443, -1);
  // Kneipe
  CreateObject(_I1A, 612, 417, -1);
  // Wachtürme
  CreateObject(TWR2, 718, 415, -1)->~ControlRight();
  CreateObject(TWR2, 944, 466, -1)->~ControlRight();
  // Forschungslabor
  var pLab=CreateObject (_B3A, 502, 417, -1);
  // das Stromnetz
  ObjectSetAction(CreateObject(PWRL,126,255,-1), "Connect", pWindmill, pLab);
  ObjectSetAction(CreateObject(PWRL,126,255,-1), "Connect", pWindmill, pElevator);
  ObjectSetAction(CreateObject(PWRL,412,419,-1), "Connect", pPowerStation, pLab);
  ObjectSetAction(CreateObject(PWRL,502,427,-1), "Connect", pLab, pElevator);
  Windstrom();
  return(1);
}

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  // Imperiale Streitkräfte
  if(iTeam==2)
  {
    for(var pClnk in FindObjects(Find_ID(CLN2),Find_Owner(iPlr)))
    pClnk->MakeSoldier();
  }
}

func LocalPlaceName(para)
{
  if(para==1) return("Narshe"); 
  if(para==2) return("Imperialer Vorposten");
  return("Komisches Unbekanntes Ding");
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

// Gibt das forcierte Zeitalter des Szenarios zurück (für den Regelwähler)
func ForcedTechLevel()
{
  return RdW_TL_Industry;
}

public func IsCampaignMission()
{
  return true;
}
