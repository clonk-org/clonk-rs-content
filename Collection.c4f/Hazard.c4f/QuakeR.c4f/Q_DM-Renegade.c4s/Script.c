/* Renegade */
#strict

func Initialize()
{
    //Gamma & Sky
    SetGamma (RGB(25,0,0),RGB(150,128,128),RGB(255,234,230));
    SetSkyParallax(0,10,0,30,0);

    // Waffen
    PlaceSpawnpoint(Q_RL, 182,145, 200); // RocketLauncher
    PlaceSpawnpoint(Q_PG, 830, 64, 200); // Plasmagun
    PlaceSpawnpoint(Q_SG, 528,244, 200); // Shot Gun
    PlaceSpawnpoint(Q_SQ, 888,189, 250); // Squeeker
    
    // Munition
    PlaceSpawnpoint(QSGA, 638,142, 200); // Shrot
    PlaceSpawnpoint(QRLA, 183,258, 200); // Raketen
    PlaceSpawnpoint(QMGA, 765,265, 200); // MG
    //PlaceSpawnpoint(QPGA, 888,189, 220); // Plasma

    // Medipacks
    PlaceMedikit(630,270, 350); //Statue Gang
    PlaceMedikit(690,175, 350); //Platform oben
    PlaceMedikit(389,181, 450); //Jumper


		// Boni!
		PlaceBonusSpawnpoint(440,105);    
    // Blutsauger
    CreateObject(QBDS, 821,280, -1); // Statue
    CreateObject(QBDS, 260,270, -1); // Kammer rechts
    
    // Jumppads
    CreateObject(QJP1, 390,255, -1)->Set(+8,-90); // Speedpad
    CreateObject(QJP3, 874,282, -1)->Set(-50,-70); // Statue
    CreateObject(QJP3, 618,165, -1)->Set(+50,-65); // Plattform
    CreateObject(QJP2, 37,219, -1)->Set(+30,-65); // Rundung Links

    // Teleporter
    CreateObject(AWRP, 888,71, -1)->Set(180,145); // Warp Rechts

    // Deko
    CreateObject(QDST, 765,278,-1); //Statue
    CreateObject(QDAR, 390,238,-1); //Arrows
    CreateObject(QDAR, 390,228,-1); //Arrows
    CreateObject(QDAR, 390,218,-1); //Arrows
    CreateObject(QDLT, 92 ,225,-1); //Licht1L
    CreateObject(QDLT, 272,225,-1); //Licht1R
    CreateObject(QQSC, 526,224,-1); //Screen

  //Waypoints
	PlaceWP();
}

func PlaceWP()
{
  var wp1 = CreateWP(185,144);
  var wp2 = CreateWP(187,257);
  var wp3 = CreateWP(529,240);
  var wp4 = CreateWP(390,249);
  var wp5 = CreateWP(438,106);
  var wp6 = CreateWP(833,66);
  var wp7 = CreateWP(888,64);
  var wp8 = CreateWP(846,273);
  var wp9 = CreateWP(622,156);
  var wp10 = CreateWP(647,156);
  var wp11 = CreateWP(40,205);
  var wp12 = CreateWP(81,256);
  var wp13 = CreateWP(766,264);
  var wp14 = CreateWP(876,277);
  var wp15 = CreateWP(891,187);
  var wp16 = CreateWP(354,243);
  var wp17 = CreateWP(427,245);
  wp1 -> AddPath(wp12, Path_MoveTo, -1);
  wp1 -> AddPath(wp16, Path_MoveTo, 1);
  wp2 -> AddPath(wp12, Path_MoveTo, -1);
  wp2 -> AddPath(wp16, Path_MoveTo, 1);
  wp3 -> AddPath(wp13, Path_MoveTo, 1);
  wp3 -> AddPath(wp17, Path_MoveTo, -1);
  wp4 -> AddPath(wp5, Path_MoveTo, 1);
  wp5 -> AddPath(wp16, Path_MoveTo, -1);
  wp6 -> AddPath(wp10, Path_MoveTo, -1);
  wp6 -> AddPath(wp7, Path_MoveTo, 1);
  wp7 -> AddPath(wp1, Path_MoveTo, -1);
  wp8 -> AddPath(wp15, Path_Jump, 1);
  wp8 -> AddPath(wp14, Path_MoveTo, 1);
  wp8 -> AddPath(wp13, Path_MoveTo, -1);
  wp9 -> AddPath(wp6, Path_MoveTo, 1);
  wp10 -> AddPath(wp13, Path_MoveTo, 1);
  wp10 -> AddPath(wp9, Path_MoveTo, -1);
  wp11 -> AddPath(wp1, Path_MoveTo, 1);
  wp12 -> AddPath(wp11, Path_MoveTo, -1);
  wp12 -> AddPath(wp11, Path_Jump, -1);
  wp12 -> AddPath(wp2, Path_MoveTo, 1);
  wp13 -> AddPath(wp3, Path_MoveTo, -1);
  wp13 -> AddPath(wp8, Path_MoveTo, 1);
  wp14 -> AddPath(wp10, Path_MoveTo, -1);
  wp15 -> AddPath(wp13, Path_MoveTo, -1);
  wp15 -> AddPath(wp8, Path_MoveTo, -1);
  wp16 -> AddPath(wp17, Path_Jump, 1);
  wp16 -> AddPath(wp4, Path_MoveTo, 1);
  wp16 -> AddPath(wp2, Path_MoveTo, -1);
  wp17 -> AddPath(wp3, Path_MoveTo, 1);
  wp17 -> AddPath(wp4, Path_MoveTo, -1);
  wp17 -> AddPath(wp16, Path_Jump, -1);
}

/* Relaunch */

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  for(var i=0, pCrew ; pCrew = GetCrew(iPlr, i) ; i++)
    RelaunchPlayer(iPlr, pCrew, 0, iTeam);
}

public func RelaunchPlayer(int iPlr, object pCrew, object pKiller, int iTeam)
{
  // Kein ordentlicher Spieler?
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER)
    return();
  // Kein Team
  if(!iTeam) iTeam = GetPlayerTeam(iPlr);
  // Clonk tot?
  if(!GetAlive(pCrew))
    pCrew = RelaunchClonk(iPlr, pCrew);
  // Zufallsposition
  var iX, iY;
  RelaunchPosition(iX, iY, iTeam);

    pCrew->CreateContents(Q_MG);
    pCrew->CreateContents(Q_GT);
    CreateObject(QMGA)->Activate(pCrew);

  if(Contained(pCrew))
    SetPosition(iX, iY, Contained(pCrew));
  else
    SetPosition(iX, iY, pCrew);
  // Fertig gerelauncht
}

public func RelaunchClonk(int iPlr, object pCursor)
{
  var pClonk = CreateObject(QBOT, 10, 10, iPlr);
  if(pCursor)
    GrabObjectInfo(pCursor, pClonk);
  else
    MakeCrewMember(pClonk, iPlr);

  DoEnergy(+150, pClonk);
  SetCursor(iPlr, pClonk);
  SetPlrView(iPlr, pClonk);

  // Wegstecken
  var tim = CreateObject(TIM2, 10, 10, -1);
  pClonk->Enter(tim);
  PlayerMessage(iPlr, Format("@%s", GetName(pClonk)), tim);

  return(pClonk);
}


global func RelaunchPosition(&iX, &iY, int iTeam)
{
    var r = Random(7);
    if(!r) { iX = RandomX(90,290); iY = 265;}  //Gang Links
    if(!r--) { iX = RandomX(420,450); iY = 102;} //Gang
    if(!r--) { iX = RandomX(140,227); iY = 148;} //RL Platform
    if(!r--) { iX = RandomX(418,590); iY = 242;} //Plattform Speedpad
    if(!r--) { iX = 888; iY = 190;}  //PG Muni
    if(!r--) { iX = 647; iY = 150;}  //Shrot Muni
    if(!r--) { iX = RandomX(747,870); iY = 60;}  //PG
    if(!r--) { iX = RandomX(675,855); iY = 265;} //Statue
}

