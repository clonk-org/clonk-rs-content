/* Quake Map 2 */
#strict

func Initialize()
{
	// Jumppads 
	CreateObject(QJP2, 318, 192, -1)->Set(150, -20);
	CreateObject(QJP2, 635, 187, -1)->Set(-150, -20);
	CreateObject(QJP3, 484, 263, -1)->Set(50, -50);
	CreateObject(QJP3, 465, 263, -1)->Set(-50, -50);
	CreateObject(QJP2, 673, 397, -1)->Set(-150, -20);
	CreateObject(QJP2, 282, 395, -1)->Set(150, -20);
	CreateObject(QJP2, 302, 716, -1)->Set(150, -20);
	CreateObject(QJP2, 301, 608, -1)->Set(150, -20);
	CreateObject(QJP1, 475, 434, -1)->Set(0, -100);
	CreateObject(QJP2, 652, 716, -1)->Set(-150, -20);
	CreateObject(QJP2, 652, 609, -1)->Set(-150, -20);
	CreateObject(QJP3, 4, 597, -1)->Set(60, -65);
	CreateObject(QJP3, 946, 597, -1)->Set(-60, -65);
	CreateObject(QJP3, 946, 68, -1)->Set(-130, -20);
	CreateObject(QJP3, 4, 71, -1)->Set(130, -20);

	// Waffen
	PlaceSpawnpoint(Q_RL,465,63,200);
	PlaceSpawnpoint(Q_SG,562,245,200);
	PlaceSpawnpoint(Q_GL,560,423,200);
	PlaceSpawnpoint(Q_RG,476,778,200);

	// Muni
	PlaceSpawnpoint(QSGA, 77,116,200);
	PlaceSpawnpoint(QGLA,872,114,200);
	
	PlaceSpawnpoint(QGLA,346,510,200);
	PlaceSpawnpoint(QRGA,409,480,200);
	PlaceSpawnpoint(QRLA,476,510,200);
	PlaceSpawnpoint(QSGA,545,480,200);
	PlaceSpawnpoint(QMGA,607,510,200);
	
	PlaceSpawnpoint(QRGA,671,587,200);
	PlaceSpawnpoint(QRLA,282,697,200);
	PlaceSpawnpoint(QMGA,9,701,200);
	PlaceSpawnpoint(QMGA,943,700,200);
	
	// Medikits
	PlaceMedikit(272,100+7,200);
	PlaceMedikit(655,167+7,200);
	PlaceMedikit(292,240+7,200);
	PlaceMedikit(744,377+7,200);
	PlaceMedikit(328,424+7,200);
	PlaceMedikit(116,587+7,200);
	PlaceMedikit(816,697+7,200);
	
	// Warps
	CreateObject(QWRP, 101,353, -1)->Set(109,114);
	CreateObject(QWRP, 848,356, -1)->Set(470,770);
	
	PlaceWP();
}

func PlaceWP()
{
  var wp1 = CreateWP(509,429);
  var wp2 = CreateWP(440,429);
  var wp3 = CreateWP(673,387);
  var wp4 = CreateWP(283,382);
  var wp5 = CreateWP(682,590);
  var wp6 = CreateWP(268,590);
  var wp7 = CreateWP(683,699);
  var wp8 = CreateWP(272,697);
  var wp9 = CreateWP(482,774);
  var wp10 = CreateWP(467,70);
  var wp11 = CreateWP(329,424);
  var wp12 = CreateWP(469,428);
  var wp13 = CreateWP(7,66);
  var wp14 = CreateWP(198,170);
  var wp15 = CreateWP(320,183);
  var wp16 = CreateWP(466,257);
  var wp17 = CreateWP(337,228);
  var wp18 = CreateWP(239,375);
  var wp19 = CreateWP(61,616);
  var wp20 = CreateWP(303,706);
  var wp21 = CreateWP(306,593);
  var wp22 = CreateWP(7,588);
  var wp23 = CreateWP(258,478);
  var wp24 = CreateWP(11,699);
  var wp25 = CreateWP(326,737);
  var wp26 = CreateWP(102,344);
  var wp27 = CreateWP(850,344);
  var wp28 = CreateWP(626,737);
  var wp29 = CreateWP(941,699);
  var wp30 = CreateWP(694,478);
  var wp31 = CreateWP(943,588);
  var wp32 = CreateWP(650,593);
  var wp33 = CreateWP(649,706);
  var wp34 = CreateWP(891,616);
  var wp35 = CreateWP(713,375);
  var wp36 = CreateWP(615,228);
  var wp37 = CreateWP(486,257);
  var wp38 = CreateWP(632,183);
  var wp39 = CreateWP(754,170);
  var wp40 = CreateWP(945,66);
  var wp41 = CreateWP(479,428);
  var wp42 = CreateWP(623,424);
  wp1 -> AddPath(wp42, Path_MoveTo, 1);
  wp1 -> AddPath(wp2, Path_Jump, -1);
  wp1 -> AddPath(wp41, Path_MoveTo, -1);
  wp2 -> AddPath(wp1, Path_Jump, 1);
  wp2 -> AddPath(wp11, Path_MoveTo, -1);
  wp2 -> AddPath(wp12, Path_MoveTo, 1);
  wp3 -> AddPath(wp18, Path_MoveTo, -1);
  wp4 -> AddPath(wp35, Path_MoveTo, 1);
  wp5 -> AddPath(wp9, Path_Jump, -1);
  wp5 -> AddPath(wp32, Path_MoveTo, -1);
  wp5 -> AddPath(wp34, Path_MoveTo, 1);
  wp6 -> AddPath(wp21, Path_MoveTo, 1);
  wp6 -> AddPath(wp19, Path_MoveTo, -1);
  wp6 -> AddPath(wp9, Path_Jump, 1);
  wp7 -> AddPath(wp33, Path_MoveTo, -1);
  wp7 -> AddPath(wp34, Path_MoveTo, 1);
  wp7 -> AddPath(wp9, Path_Jump, -1);
  wp8 -> AddPath(wp20, Path_MoveTo, 1);
  wp8 -> AddPath(wp19, Path_MoveTo, -1);
  wp8 -> AddPath(wp9, Path_Jump, 1);
  wp9 -> AddPath(wp28, Path_MoveTo, 1);
  wp9 -> AddPath(wp25, Path_MoveTo, -1);
  wp10 -> AddPath(wp14, Path_MoveTo, -1);
  wp10 -> AddPath(wp39, Path_MoveTo, 1);
  wp11 -> AddPath(wp23, Path_Jump, -1);
  wp11 -> AddPath(wp2, Path_MoveTo, 1);
  wp11 -> AddPath(wp19, Path_MoveTo, -1);
  wp12 -> AddPath(wp14, Path_MoveTo, -1);
  wp13 -> AddPath(wp10, Path_MoveTo, 1);
  wp14 -> AddPath(wp13, Path_MoveTo, -1);
  wp14 -> AddPath(wp15, Path_MoveTo, 1);
  wp14 -> AddPath(wp10, Path_Jump, 1);
  wp15 -> AddPath(wp39, Path_MoveTo, 1);
  wp16 -> AddPath(wp15, Path_MoveTo, -1);
  wp17 -> AddPath(wp15, Path_Jump, -1);
  wp17 -> AddPath(wp16, Path_MoveTo, 1);
  wp17 -> AddPath(wp18, Path_MoveTo, -1);
  wp18 -> AddPath(wp26, Path_MoveTo, -1);
  wp18 -> AddPath(wp4, Path_MoveTo, 1);
  wp18 -> AddPath(wp11, Path_Jump, 1);
  wp19 -> AddPath(wp22, Path_MoveTo, -1);
  wp19 -> AddPath(wp8, Path_MoveTo, 1);
  wp19 -> AddPath(wp6, Path_Jump, 1);
  wp20 -> AddPath(wp7, Path_MoveTo, 1);
  wp21 -> AddPath(wp5, Path_MoveTo, 1);
  wp21 -> AddPath(wp34, Path_MoveTo, 1);
  wp22 -> AddPath(wp11, Path_MoveTo, 1);
  wp22 -> AddPath(wp2, Path_MoveTo, 1);
  wp23 -> AddPath(wp6, Path_Jump, -1);
  wp23 -> AddPath(wp30, Path_MoveTo, 1);
  wp23 -> AddPath(wp11, Path_Jump, 1);
  wp24 -> AddPath(wp25, Path_MoveTo, 1);
  wp25 -> AddPath(wp9, Path_MoveTo, 1);
  wp25 -> AddPath(wp8, Path_Jump, -1);
  wp25 -> AddPath(wp24, Path_MoveTo, -1);
  wp26 -> AddPath(wp10, Path_MoveTo, 1);
  wp27 -> AddPath(wp9, Path_MoveTo, -1);
  wp28 -> AddPath(wp9, Path_MoveTo, -1);
  wp28 -> AddPath(wp29, Path_MoveTo, 1);
  wp28 -> AddPath(wp7, Path_Jump, 1);
  wp29 -> AddPath(wp28, Path_MoveTo, -1);
  wp30 -> AddPath(wp23, Path_MoveTo, -1);
  wp30 -> AddPath(wp5, Path_Jump, 1);
  wp30 -> AddPath(wp42, Path_Jump, -1);
  wp31 -> AddPath(wp42, Path_MoveTo, -1);
  wp31 -> AddPath(wp1, Path_MoveTo, -1);
  wp32 -> AddPath(wp6, Path_MoveTo, -1);
  wp32 -> AddPath(wp19, Path_MoveTo, -1);
  wp33 -> AddPath(wp8, Path_MoveTo, -1);
  wp34 -> AddPath(wp31, Path_MoveTo, 1);
  wp34 -> AddPath(wp7, Path_MoveTo, -1);
  wp34 -> AddPath(wp5, Path_Jump, -1);
  wp35 -> AddPath(wp27, Path_MoveTo, 1);
  wp35 -> AddPath(wp3, Path_MoveTo, -1);
  wp35 -> AddPath(wp42, Path_Jump, -1);
  wp36 -> AddPath(wp38, Path_Jump, 1);
  wp36 -> AddPath(wp37, Path_MoveTo, -1);
  wp36 -> AddPath(wp35, Path_MoveTo, 1);
  wp37 -> AddPath(wp38, Path_MoveTo, 1);
  wp38 -> AddPath(wp14, Path_MoveTo, -1);
  wp39 -> AddPath(wp10, Path_Jump, -1);
  wp39 -> AddPath(wp40, Path_MoveTo, 1);
  wp39 -> AddPath(wp38, Path_MoveTo, -1);
  wp40 -> AddPath(wp10, Path_MoveTo, -1);
  wp41 -> AddPath(wp39, Path_MoveTo, 1);
  wp42 -> AddPath(wp30, Path_Jump, 1);
  wp42 -> AddPath(wp1, Path_MoveTo, -1);
  wp42 -> AddPath(wp34, Path_MoveTo, 1);
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

  // Lecker Waffen
  if(!FindObject(IGIB))
  {
  	pCrew->CreateContents(Q_MG);
  	pCrew->CreateContents(Q_GT);
  	CreateObject(QMGA)->Activate(pCrew);
  }

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
	var ok;
	while(!ok)
	{
		var obj = PlaceAnimal(WIPF);
		iX = obj->GetX();
		iY = obj->GetY();
		RemoveObject(obj);
		
		if(GBackSolid (iX, iY-10))
			continue;
		
		if(FindObject2(Find_Or(Find_ID(QJP1), Find_ID(QJP2), Find_ID(QJP3)), Find_Distance(20,iX,iY)))
			continue;
		
		ok = true;
	}
}

