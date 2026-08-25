/* Quake Map Chinatown */
#strict

func Initialize()
{
	var w = LandscapeWidth();
	var d = 6;

	// Waffen
	PlaceSpawnpoint(Q_RG, 177, 161-d, 300);
	PlaceSpawnpoint(Q_SQ, 200, 572-d, 300);
	
	PlaceSpawnpoint(Q_RL, w-177, 161-d, 300);
	PlaceSpawnpoint(Q_GL, w-200, 572-d, 300);
	
	PlaceSpawnpoint(Q_SG, w/2, 715, 300);
	
	// Ammo
	PlaceSpawnpoint(QSGA, 387, 323-d, 300);
	PlaceSpawnpoint(QGLA, 550, 321-d, 300);
	PlaceSpawnpoint(QRLA, 200, 682-d, 300);
	
	PlaceSpawnpoint(QSGA, w-387, 323-d, 300);
	PlaceSpawnpoint(QMGA, w-550, 321-d, 300);
	PlaceSpawnpoint(QRGA, w-200, 682-d, 300);
	
	
	// Medikits
	PlaceMedikit(190,410, 300);
	PlaceMedikit(390,682, 300);
	
	PlaceMedikit(w-190,410, 300);
	PlaceMedikit(w-390,682, 300);
	
	// Boni
	PlaceBonusSpawnpoint(w/2, 160);


	// Jumppad
	CreateObject(QJP1, w/2, 445, -1)->Set(0,-60);
	
	CreateObject(QWRP, 205, 781, -1)->Set(550,435);
	CreateObject(QWRP, w-205, 781, -1)->Set(w-550,435);
	
	// Blutsauger
	CreateObject(QBDS, 333,718, -1);
	CreateObject(QBDS, w/2,762, -1);
	CreateObject(QBDS, w-333,718, -1);
	
	PlaceWP();
}

func PlaceWP()
{
  var wp1 = CreateWP(710,256);
  var wp2 = CreateWP(489,257);
  var wp3 = CreateWP(207,768);
  var wp4 = CreateWP(115,644);
  var wp5 = CreateWP(330,589);
  var wp6 = CreateWP(125,534);
  var wp7 = CreateWP(335,689);
  var wp8 = CreateWP(177,151);
  var wp9 = CreateWP(118,413);
  var wp10 = CreateWP(379,499);
  var wp11 = CreateWP(535,313);
  var wp12 = CreateWP(387,313);
  var wp13 = CreateWP(560,757);
  var wp14 = CreateWP(666,313);
  var wp15 = CreateWP(813,313);
  var wp16 = CreateWP(1023,151);
  var wp17 = CreateWP(1082,413);
  var wp18 = CreateWP(821,499);
  var wp19 = CreateWP(1075,533);
  var wp20 = CreateWP(870,589);
  var wp21 = CreateWP(865,689);
  var wp22 = CreateWP(636,755);
  var wp23 = CreateWP(1085,644);
  var wp24 = CreateWP(995,770);
  var wp25 = CreateWP(599,165);
  var wp26 = CreateWP(600,437);
  var wp27 = CreateWP(544,437);
  var wp28 = CreateWP(658,437);
  var wp29 = CreateWP(486,463);
  var wp30 = CreateWP(714,463);
  var wp31 = CreateWP(773,672);
  var wp32 = CreateWP(431,672);
  wp1 -> AddPath(wp25, Path_MoveTo, -1);
  wp1 -> AddPath(wp15, Path_Jump, 1);
  wp2 -> AddPath(wp25, Path_MoveTo, 1);
  wp2 -> AddPath(wp12, Path_Jump, -1);
  wp3 -> AddPath(wp27, Path_MoveTo, 1);
  wp4 -> AddPath(wp5, Path_Jump, 1);
  wp4 -> AddPath(wp32, Path_MoveTo, 1);
  wp4 -> AddPath(wp7, Path_MoveTo, 1);
  wp5 -> AddPath(wp6, Path_Jump, -1);
  wp5 -> AddPath(wp4, Path_MoveTo, -1);
  wp6 -> AddPath(wp10, Path_Jump, 1);
  wp6 -> AddPath(wp5, Path_MoveTo, 1);
  wp7 -> AddPath(wp3, Path_MoveTo, -1);
  wp8 -> AddPath(wp12, Path_MoveTo, 1);
  wp9 -> AddPath(wp10, Path_MoveTo, 1);
  wp9 -> AddPath(wp12, Path_Jump, 1);
  wp10 -> AddPath(wp9, Path_Jump, -1);
  wp10 -> AddPath(wp29, Path_Jump, 1);
  wp10 -> AddPath(wp6, Path_MoveTo, -1);
  wp10 -> AddPath(wp13, Path_MoveTo, 1);
  wp11 -> AddPath(wp10, Path_MoveTo, -1);
  wp11 -> AddPath(wp14, Path_MoveTo, 1);
  wp11 -> AddPath(wp28, Path_MoveTo, 1);
  wp12 -> AddPath(wp9, Path_MoveTo, -1);
  wp12 -> AddPath(wp29, Path_MoveTo, 1);
  wp12 -> AddPath(wp27, Path_MoveTo, 1);
  wp12 -> AddPath(wp8, Path_Jump, -1);
  wp12 -> AddPath(wp2, Path_Jump, 1);
  wp13 -> AddPath(wp22, Path_MoveTo, 1);
  wp13 -> AddPath(wp22, Path_Jump, 1);
  wp13 -> AddPath(wp31, Path_Jump, 1);
  wp13 -> AddPath(wp32, Path_MoveTo, -1);
  wp14 -> AddPath(wp11, Path_MoveTo, -1);
  wp14 -> AddPath(wp18, Path_MoveTo, 1);
  wp14 -> AddPath(wp27, Path_MoveTo, -1);
  wp15 -> AddPath(wp17, Path_MoveTo, 1);
  wp15 -> AddPath(wp30, Path_MoveTo, -1);
  wp15 -> AddPath(wp28, Path_MoveTo, -1);
  wp15 -> AddPath(wp16, Path_Jump, 1);
  wp15 -> AddPath(wp1, Path_Jump, -1);
  wp16 -> AddPath(wp15, Path_MoveTo, -1);
  wp17 -> AddPath(wp15, Path_Jump, -1);
  wp17 -> AddPath(wp18, Path_MoveTo, -1);
  wp18 -> AddPath(wp17, Path_Jump, 1);
  wp18 -> AddPath(wp30, Path_Jump, -1);
  wp18 -> AddPath(wp19, Path_MoveTo, 1);
  wp18 -> AddPath(wp22, Path_MoveTo, -1);
  wp19 -> AddPath(wp18, Path_Jump, -1);
  wp19 -> AddPath(wp20, Path_MoveTo, -1);
  wp20 -> AddPath(wp19, Path_Jump, 1);
  wp20 -> AddPath(wp23, Path_MoveTo, 1);
  wp21 -> AddPath(wp24, Path_MoveTo, 1);
  wp22 -> AddPath(wp13, Path_MoveTo, -1);
  wp22 -> AddPath(wp13, Path_Jump, -1);
  wp22 -> AddPath(wp32, Path_Jump, -1);
  wp22 -> AddPath(wp31, Path_MoveTo, 1);
  wp23 -> AddPath(wp20, Path_Jump, -1);
  wp23 -> AddPath(wp31, Path_MoveTo, -1);
  wp23 -> AddPath(wp21, Path_MoveTo, -1);
  wp24 -> AddPath(wp28, Path_MoveTo, -1);
  wp25 -> AddPath(wp2, Path_MoveTo, -1);
  wp25 -> AddPath(wp1, Path_MoveTo, 1);
  wp26 -> AddPath(wp11, Path_MoveTo, -1);
  wp26 -> AddPath(wp14, Path_MoveTo, 1);
  wp27 -> AddPath(wp29, Path_MoveTo, -1);
  wp27 -> AddPath(wp26, Path_MoveTo, 1);
  wp27 -> AddPath(wp28, Path_Jump, 1);
  wp28 -> AddPath(wp30, Path_MoveTo, 1);
  wp28 -> AddPath(wp26, Path_MoveTo, -1);
  wp28 -> AddPath(wp27, Path_Jump, -1);
  wp29 -> AddPath(wp27, Path_MoveTo, 1);
  wp29 -> AddPath(wp10, Path_MoveTo, -1);
  wp29 -> AddPath(wp12, Path_Jump, -1);
  wp30 -> AddPath(wp28, Path_MoveTo, -1);
  wp30 -> AddPath(wp15, Path_Jump, 1);
  wp30 -> AddPath(wp18, Path_MoveTo, 1);
  wp31 -> AddPath(wp13, Path_Jump, -1);
  wp31 -> AddPath(wp22, Path_MoveTo, -1);
  wp31 -> AddPath(wp21, Path_MoveTo, 1);
  wp31 -> AddPath(wp23, Path_MoveTo, 1);
  wp31 -> AddPath(wp18, Path_Jump, 1);
  wp32 -> AddPath(wp22, Path_Jump, 1);
  wp32 -> AddPath(wp13, Path_MoveTo, 1);
  wp32 -> AddPath(wp7, Path_MoveTo, -1);
  wp32 -> AddPath(wp4, Path_MoveTo, -1);
  wp32 -> AddPath(wp10, Path_Jump, -1);
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
