/* Bloody Sun */
#strict

func Initialize()
{
	// Gamma & Sonne
	SetGamma (RGB(25,0,0),RGB(150,128,128),RGB(255,234,230));
 	 var s2 = CreateObject(SUN_, LandscapeWidth()/3, LandscapeHeight()/2, -1);
 	 var s1 = CreateObject(SUN_, LandscapeWidth()/3, LandscapeHeight()/2, -1);

 	 s1->SetClrModulation(RGBa(255,100,0,0));
 	 s2->SetObjectBlitMode(1);
  	 s2->SetClrModulation(RGBa(255,255,0,0));
  	 s2->SetRotationSpeed(12);
  	 s2->SetGrowPos(13000);

	// Waffen
	PlaceSpawnpoint(Q_SG, 735, 45, 300); // Shotgun
	PlaceSpawnpoint(Q_RG,  54, 64, 300); // Railgun
	PlaceSpawnpoint(Q_GL, 170,277, 300); // Granatenwerfer
	PlaceSpawnpoint(Q_RL, 247,390, 300); // Raketenwerfer
	PlaceSpawnpoint(Q_PG, 500,186, 300); // Plasmagun
	
	// Munition
	PlaceSpawnpoint(QSGA, 454,186, 500); // Shotgun 
	PlaceSpawnpoint(QMGA, 225,287, 600); // MG
	PlaceSpawnpoint(QGLA, 755,45, 600);  // Granatenwerfer
	PlaceSpawnpoint(QRGA, 538,186, 600); // Railgun
	PlaceSpawnpoint(QPGA, 538,186, 600); // Plasma
	//PlaceBonusSpawnpoint([Q_QD], 771,382, 4000);// QuadDamage
	PlaceBonusSpawnpoint(770,380,3000);
	
	// Medipacks
	PlaceMedikit(387,364, 300); // Meer links
	PlaceMedikit(769,417, 300); // Meer rechts
	PlaceMedikit(226,124, 300); // Platform links
	PlaceMedikit(155,384, 300); // Gang links
	PlaceMedikit(668,230, 300); // Platform rechts
	
	// Blutsauger
	CreateObject(QBDS, 215,395, -1); //Tunnel
	CreateObject(QBDS, 747,305, -1); //Meer rechts
	CreateObject(QBDS, 747,303, -1); //Meer rechts
	
	// Jumppads
	CreateObject(QJP3, 19,  74, -1)->Set(+90,-30); //oben links
	CreateObject(QJP3, 806, 57, -1)->Set(-120,-40);//oben rechts
	CreateObject(QJP3, 806,306, -1)->Set(-20,-60);//Wasser rechts
	CreateObject(QJP1,  33,291, -1)->Set(+23,-84);//unten links


	// Teleporter
	CreateObject(AWRP, 290,395, -1)->Set(771,387); //Warp nach QD
	CreateObject(AWRP, 286,274, -1)->Set(763, 48); //nach Platform oben rechts

	// Screens
	CreateObject(QQSC, 181,231, -1);

	//Waypoints
  var wp1 = CreateWP(578,296);
  var wp2 = CreateWP(676,47);
  var wp3 = CreateWP(806,47);
  var wp4 = CreateWP(742,47);
  var wp5 = CreateWP(286,256);
  var wp6 = CreateWP(806,296);
  var wp7 = CreateWP(686,296);
  var wp8 = CreateWP(387,349);
  var wp9 = CreateWP(229,281);
  var wp10 = CreateWP(123,281);
  var wp11 = CreateWP(63,281);
  var wp12 = CreateWP(36,281);
  var wp13 = CreateWP(95,378);
  var wp14 = CreateWP(248,390);
  var wp15 = CreateWP(290,390);
  var wp16 = CreateWP(771,416);
  var wp17 = CreateWP(214,119);
  var wp18 = CreateWP(331,146);
  var wp19 = CreateWP(114,64);
  var wp20 = CreateWP(155,119);
  var wp21 = CreateWP(54,64);
  var wp22 = CreateWP(19,64);
  var wp23 = CreateWP(463,183);
  var wp24 = CreateWP(409,183);
  var wp25 = CreateWP(274,118);
  var wp26 = CreateWP(567,183);
  var wp27 = CreateWP(651,218);
  var wp28 = CreateWP(725,218);
  wp1 -> AddPath(wp7, Path_MoveTo, 1);
  wp1 -> AddPath(wp8, Path_MoveTo, -1);
  wp1 -> AddPath(wp16, Path_MoveTo, 1);
  wp1 -> AddPath(wp9, Path_MoveTo, -1);
  wp2 -> AddPath(wp26, Path_Jump, -1);
  wp2 -> AddPath(wp4, Path_MoveTo, 1);
  wp3 -> AddPath(wp20, Path_MoveTo, -1);
  wp4 -> AddPath(wp2, Path_MoveTo, -1);
  wp4 -> AddPath(wp3, Path_MoveTo, 1);
  wp5 -> AddPath(wp4, Path_MoveTo, 1);
  wp6 -> AddPath(wp28, Path_MoveTo, -1);
  wp7 -> AddPath(wp8, Path_MoveTo, -1);
  wp7 -> AddPath(wp6, Path_MoveTo, 1);
  wp7 -> AddPath(wp1, Path_MoveTo, -1);
  wp8 -> AddPath(wp9, Path_MoveTo, -1);
  wp8 -> AddPath(wp16, Path_MoveTo, 1);
  wp8 -> AddPath(wp7, Path_MoveTo, 1);
  wp8 -> AddPath(wp1, Path_MoveTo, 1);
  wp9 -> AddPath(wp8, Path_MoveTo, 1);
  wp9 -> AddPath(wp10, Path_MoveTo, -1);
  wp9 -> AddPath(wp5, Path_Jump, 1);
  wp9 -> AddPath(wp1, Path_MoveTo, 1);
  wp10 -> AddPath(wp9, Path_MoveTo, 1);
  wp10 -> AddPath(wp11, Path_MoveTo, -1);
  wp11 -> AddPath(wp12, Path_MoveTo, -1);
  wp11 -> AddPath(wp10, Path_MoveTo, 1);
  wp11 -> AddPath(wp13, Path_MoveTo, 1);
  wp12 -> AddPath(wp20, Path_MoveTo, 1);
  wp13 -> AddPath(wp11, Path_MoveTo, -1);
  wp13 -> AddPath(wp14, Path_MoveTo, 1);
  wp14 -> AddPath(wp13, Path_MoveTo, -1);
  wp14 -> AddPath(wp15, Path_MoveTo, 1);
  wp15 -> AddPath(wp16, Path_MoveTo, 1);
  wp16 -> AddPath(wp8, Path_MoveTo, -1);
  wp16 -> AddPath(wp1, Path_MoveTo, -1);
  wp17 -> AddPath(wp25, Path_MoveTo, 1);
  wp17 -> AddPath(wp20, Path_MoveTo, -1);
  wp17 -> AddPath(wp11, Path_Jump, -1);
  wp18 -> AddPath(wp25, Path_MoveTo, -1);
  wp18 -> AddPath(wp24, Path_Jump, 1);
  wp19 -> AddPath(wp20, Path_MoveTo, 1);
  wp19 -> AddPath(wp21, Path_MoveTo, -1);
  wp20 -> AddPath(wp19, Path_Jump, -1);
  wp20 -> AddPath(wp17, Path_MoveTo, 1);
  wp21 -> AddPath(wp19, Path_MoveTo, 1);
  wp21 -> AddPath(wp22, Path_MoveTo, -1);
  wp22 -> AddPath(wp23, Path_MoveTo, 1);
  wp23 -> AddPath(wp24, Path_MoveTo, -1);
  wp23 -> AddPath(wp26, Path_MoveTo, 1);
  wp23 -> AddPath(wp5, Path_Backflip, -1);
  wp24 -> AddPath(wp23, Path_MoveTo, 1);
  wp24 -> AddPath(wp18, Path_Jump, -1);
  wp25 -> AddPath(wp18, Path_MoveTo, 1);
  wp25 -> AddPath(wp24, Path_Backflip, 1);
  wp25 -> AddPath(wp17, Path_MoveTo, -1);
  wp26 -> AddPath(wp23, Path_MoveTo, -1);
  wp26 -> AddPath(wp27, Path_Jump, 1);
  wp27 -> AddPath(wp26, Path_Jump, -1);
  wp27 -> AddPath(wp28, Path_MoveTo, 1);
  wp27 -> AddPath(wp1, Path_MoveTo, -1);
  wp28 -> AddPath(wp27, Path_MoveTo, -1);
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
		
		if (GBackSolid (iX, iY-10))
			continue;
		if (GBackSolid (iX-2, iY))
			continue;
	 	if (GBackSolid (iX+2, iY))
	 		continue;
	 	if(Inside(iX,150,210) && Inside(iY,190,240))
	 		continue;
		
		if(FindObject2(Find_Or(Find_ID(QJP1), Find_ID(QJP2), Find_ID(QJP3)), Find_Distance(20,iX,iY)))
			continue;
		
		ok = true;
	}
}
