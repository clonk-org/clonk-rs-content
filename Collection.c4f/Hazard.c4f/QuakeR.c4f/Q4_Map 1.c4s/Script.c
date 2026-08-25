/* Quake Map 1 */
#strict

func Initialize()
{
	// Waffen
	PlaceSpawnpoint(Q_SG, 715,260, 300); // Shotgun
	PlaceSpawnpoint(Q_RG,  55,425, 500); // Railgun
	PlaceSpawnpoint(Q_GL, 230,210, 300); // Granatenwerfer
	
	// Munition
	PlaceSpawnpoint(Q_RL, 511,748, 600); // Squeeker (RL tmp.)
	PlaceSpawnpoint(QSGA, 465,195, 600); // Shotgun 
	PlaceSpawnpoint(QMGA, 125,275, 600); // MG
	PlaceSpawnpoint(QMGA, 795,320, 600); // MG
	PlaceSpawnpoint(QGLA, 460,442, 600); // Granatenwerfer
	PlaceSpawnpoint(QRGA, 610,320, 600); // Railgun
	
	// Medipacks
	PlaceMedikit(700,450, 300);
	PlaceMedikit(190,450, 300);
	PlaceMedikit(375,790, 300);
	PlaceMedikit(575,200, 300);
	PlaceMedikit(345,265, 300);
	
	// Boni
	PlaceBonusSpawnpoint(560,280);
	PlaceBonusSpawnpoint(510,690);
	
	// Blutsauger
	CreateObject(QBDS, 222,447, -1);
	CreateObject(QBDS, 464,447, -1);
	CreateObject(QBDS, 700,447, -1);
	CreateObject(QBDS, 500,672, -1);
	
	// Jumppads
	CreateObject(QJP1, 155,328, -1)->Set(+10,-80);
	CreateObject(QJP1, 795,342, -1)->Set(-10,-70);
	CreateObject(QJP3, 848,419, -1)->Set(-7,-65);
	CreateObject(QJP2, 175,180, -1)->Set(100,-40);
	CreateObject(QJP1, 545,425, -1)->Set(+10,-70);
	CreateObject(QJP1, 578,425, -1)->Set(-10,-70);
	//CreateObject(QJP2, 771,232, -1)->Set(-100,-50);

	// Teleporter
	CreateObject(AWRP, 875,340, -1)->Set(511,690+40); // Warp nach unten
	CreateObject(AWRP, 290,690, -1)->Set(465,190); // Warp nach oben
	CreateObject(AWRP, 732,690, -1)->Set(62,420); // Warp nach links/mitte

	// Screens
	CreateObject(QQSC, 464,433, -1);
	CreateObject(QQSC, 658,433, -1);
	
	// Wegpunkte
	PlaceWaypoints();
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
	var r = Random(10);
	if(!r--) { iX = RandomX(205,295); iY = 206;}
	if(!r--) { iX = RandomX(424,520); iY = 195;}
	if(!r--) { iX = RandomX(600,620); iY = 195;}
	if(!r--) { iX = RandomX(670,760); iY = 259;}
	if(!r--) { iX = 330; iY = 168;}
	if(!r--) { iX = RandomX(100,280); iY = 439;}
	if(!r--) { iX = RandomX(435,500); iY = 439;}
	if(!r--) { iX = RandomX(630,740); iY = 439;}
	if(!r--) { iX = 513; iY = 725;}
	if(!r--) { iX = RandomX(490,520); iY = 321;}
}

func PlaceWaypoints()
{
	//Waypoints (autogen.)
  var wp1 = CreateWP(398,414);
  var wp2 = CreateWP(526,418);
  var wp3 = CreateWP(342,250);
  var wp4 = CreateWP(179,324);
  var wp5 = CreateWP(467,194);
  var wp6 = CreateWP(58,423);
  var wp7 = CreateWP(100,387);
  var wp8 = CreateWP(132,277);
  var wp9 = CreateWP(246,375);
  var wp10 = CreateWP(725,261);
  var wp11 = CreateWP(598,321);
  var wp12 = CreateWP(523,321);
  var wp13 = CreateWP(596,416);
  var wp14 = CreateWP(336,413);
  var wp15 = CreateWP(527,196);
  var wp16 = CreateWP(393,210);
  var wp17 = CreateWP(300,209);
  var wp18 = CreateWP(158,316);
  var wp19 = CreateWP(178,170);
  var wp20 = CreateWP(224,206);
  var wp21 = CreateWP(596,194);
  var wp22 = CreateWP(672,219);
  var wp23 = CreateWP(875,321);
  var wp24 = CreateWP(796,330);
  var wp25 = CreateWP(811,334);
  var wp26 = CreateWP(850,408);
  var wp27 = CreateWP(788,412);
  var wp28 = CreateWP(733,673);
  var wp29 = CreateWP(512,746);
  var wp30 = CreateWP(377,779);
  var wp31 = CreateWP(293,674);
  wp1 -> AddPath(wp2, Path_MoveTo, 1);
  wp1 -> AddPath(wp14, Path_Jump, -1);
  wp1 -> AddPath(wp30, Path_MoveTo, -1);
  wp2 -> AddPath(wp13, Path_Jump, 1);
  wp2 -> AddPath(wp11, Path_MoveTo, 1);
  wp2 -> AddPath(wp1, Path_MoveTo, -1);
  wp3 -> AddPath(wp17, Path_MoveTo, -1);
  wp3 -> AddPath(wp12, Path_MoveTo, 1);
  wp3 -> AddPath(wp16, Path_Jump, 1);
  wp4 -> AddPath(wp9, Path_MoveTo, 1);
  wp4 -> AddPath(wp8, Path_Jump, -1);
  wp4 -> AddPath(wp18, Path_MoveTo, -1);
  wp5 -> AddPath(wp15, Path_MoveTo, 1);
  wp5 -> AddPath(wp16, Path_MoveTo, -1);
  wp6 -> AddPath(wp7, Path_Jump, 1);
  wp6 -> AddPath(wp14, Path_MoveTo, 1);
  wp7 -> AddPath(wp6, Path_MoveTo, -1);
  wp7 -> AddPath(wp8, Path_Jump, 1);
  wp8 -> AddPath(wp4, Path_MoveTo, 1);
  wp8 -> AddPath(wp7, Path_MoveTo, -1);
  wp9 -> AddPath(wp14, Path_Jump, 1);
  wp9 -> AddPath(wp4, Path_MoveTo, -1);
  wp10 -> AddPath(wp11, Path_MoveTo, -1);
  wp10 -> AddPath(wp22, Path_Jump, -1);
  wp11 -> AddPath(wp10, Path_MoveTo, 1);
  wp11 -> AddPath(wp12, Path_Jump, -1);
  wp11 -> AddPath(wp2, Path_MoveTo, -1);
  wp12 -> AddPath(wp3, Path_MoveTo, -1);
  wp12 -> AddPath(wp13, Path_MoveTo, 1);
  wp12 -> AddPath(wp11, Path_Jump, 1);
  wp13 -> AddPath(wp12, Path_MoveTo, -1);
  wp13 -> AddPath(wp27, Path_MoveTo, 1);
  wp13 -> AddPath(wp2, Path_Jump, -1);
  wp14 -> AddPath(wp6, Path_MoveTo, -1);
  wp14 -> AddPath(wp9, Path_Jump, -1);
  wp14 -> AddPath(wp1, Path_Jump, 1);
  wp14 -> AddPath(wp29, Path_MoveTo, 1);
  wp14 -> AddPath(wp30, Path_MoveTo, 1);
  wp15 -> AddPath(wp11, Path_MoveTo, 1);
  wp15 -> AddPath(wp21, Path_Jump, 1);
  wp15 -> AddPath(wp5, Path_MoveTo, -1);
  wp16 -> AddPath(wp17, Path_Jump, -1);
  wp16 -> AddPath(wp5, Path_MoveTo, 1);
  wp17 -> AddPath(wp20, Path_MoveTo, -1);
  wp17 -> AddPath(wp3, Path_MoveTo, 1);
  wp18 -> AddPath(wp20, Path_MoveTo, 1);
  wp19 -> AddPath(wp21, Path_MoveTo, 1);
  wp20 -> AddPath(wp19, Path_Jump, -1);
  wp20 -> AddPath(wp17, Path_MoveTo, 1);
  wp21 -> AddPath(wp15, Path_Jump, -1);
  wp21 -> AddPath(wp12, Path_MoveTo, -1);
  wp21 -> AddPath(wp22, Path_MoveTo, 1);
  wp22 -> AddPath(wp10, Path_MoveTo, 1);
  wp22 -> AddPath(wp21, Path_MoveTo, -1);
  wp23 -> AddPath(wp29, Path_MoveTo, -1);
  wp24 -> AddPath(wp10, Path_MoveTo, -1);
  wp25 -> AddPath(wp24, Path_MoveTo, -1);
  wp25 -> AddPath(wp23, Path_Jump, 1);
  wp25 -> AddPath(wp27, Path_Jump, -1);
  wp26 -> AddPath(wp25, Path_MoveTo, -1);
  wp27 -> AddPath(wp13, Path_MoveTo, -1);
  wp27 -> AddPath(wp26, Path_MoveTo, 1);
  wp28 -> AddPath(wp6, Path_MoveTo, -1);
  wp29 -> AddPath(wp31, Path_MoveTo, -1);
  wp29 -> AddPath(wp30, Path_MoveTo, -1);
  wp29 -> AddPath(wp28, Path_MoveTo, 1);
  wp30 -> AddPath(wp31, Path_Jump, -1);
  wp30 -> AddPath(wp29, Path_Jump, 1);
  wp31 -> AddPath(wp5, Path_MoveTo, 1);
}
