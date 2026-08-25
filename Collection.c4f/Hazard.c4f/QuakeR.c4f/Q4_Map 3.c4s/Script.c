/* Quake Map 3 */
#strict

func Initialize()
{
	var water_col = RGB(77,105,190);
	var tmp;
	SetSkyParallax(0, 20, 20);
	
	var d = 6; // Abstand von festem Material
	
	// Waffen
	PlaceSpawnpoint(Q_RL, 198, 336-d, 300); // Raketenwerfer
	PlaceSpawnpoint(Q_SG, 763, 593-d, 300); // Shotgun 
	PlaceSpawnpoint(Q_PG, 454, 672-d, 300); // Plasmagun
	
	// Ammo
	PlaceSpawnpoint(QSGA, 378, 114-d, 600); // Shotgun
	PlaceSpawnpoint(QPGA, 838, 114-d, 600); // Plasmagun
	PlaceSpawnpoint(QMGA, 371, 255-d, 600); // MG
	PlaceSpawnpoint(QMGA, 845, 255-d, 600); // MG
	PlaceSpawnpoint(QRLA, 350, 341, 600); // Raketen
	PlaceSpawnpoint(QGLA, 866, 341, 600); // Granaten
	PlaceSpawnpoint(QSGA, 455, 593-d, 600); // Shotgun
	PlaceSpawnpoint(QPGA, 762, 672-d, 600); // Plasmagun
	
	// Medikits
	PlaceMedikit(608,  52, 600); // Ganz oben
	PlaceMedikit( 82, 303, 600); // Mitte links
	PlaceMedikit(352, 495, 600); // Wasser links
	PlaceMedikit(608, 543, 600); // Unten Mitte
	PlaceMedikit(287, 639, 600); // Unten Links
	
	// Deko!
	CreateObject(VENT, 608, 352, -1)->SetClrModulation(water_col);
	tmp = CreateObject(MVNT, 352, 490, -1);
	tmp->SetClrModulation(water_col);
	tmp->SetCon(50);
	tmp = CreateObject(MVNT, 864, 490, -1);
	tmp->SetClrModulation(water_col);
	tmp->SetCon(50);
	CreateObject(BLGH, 416, 231, -1);
	CreateObject(BLGH, 800, 231, -1);
	CreateObject(BLGH, 416, 423, -1);
	CreateObject(BLGH, 800, 423, -1);
	
	for(var wlgh in FindObjects(Find_ID(BLGH)))
	{
		wlgh->SetClrModulation(water_col);
	}
	
	// Warp
	CreateObject(QWRP, 608, 480, -1)->Set(608*75/100, 535*75/100);
	
	// Jumppads
	CreateObject(QJP2, 256,  80, -1)->Set(60, -10);
	CreateObject(QJP1, 256, 281, -1)->Set(0,-120);
	CreateObject(QJP2, 215, 390, -1)->Set(-30,0);
	CreateObject(QJP2, 233, 390, -1)->Set(30,0);
	CreateObject(QJP1, 224, 641, -1)->Set(0,-120);
	
	// Deko
	tmp = CreateObject(ALGH, 608, 73, -1);
	tmp->SetR(180);
	tmp->TurnOn();
	tmp->ChangeLightColor(RGBa(20,20,255,20));
	
	CreateObject(CLGH, 608,560,-1);
	
	// Links
	CreateObject(QDAR, 225, 595, -1);
	CreateObject(QDAR, 225, 610, -1);
	CreateObject(QDAR, 225, 625, -1);
	
	CreateObject(QDAR, 258, 160, -1);
	CreateObject(QDAR, 258, 180, -1);
	CreateObject(QDAR, 258, 200, -1);
	
	tmp = CreateObject(PSTR, 380, 320, -1);
	tmp->Set(0,1);
	//tmp->DoCon(20);
	//CreateObject(FENC, 375, 115, -1);
	//CreateObject(FENC, 375+64, 115, -1);
	
	CreateObject(FLGH, 445, 100, -1)->SetRotation(-40);
	
	CreateObject(CLVM, 416,674,-1);
	CreateObject(CLGH, 488,610,-1);
	
	
	
	CreateObject(FLGH, 771, 100, -1)->SetRotation(40);
	
	CreateObject(SPVM, 800,674,-1);
	CreateObject(CLGH, 768,610,-1);
	
	
	for(var obj in FindObjects(Find_InRect(0,0,LandscapeWidth(), LandscapeHeight())))
	{
		SetPosition((GetX(obj)*75)/100, (GetY(obj)*75)/100, obj);
	}
	for(var obj in FindObjects(Find_ID(QJP1)))
	{
		SetPosition(GetX(obj), GetY(obj)-1, obj);
	}
	
		// Nochmal Deko, aber andere Seite!
	CreateObject(QDAR, 743, 595*75/100, -1);
	CreateObject(QDAR, 743, 610*75/100, -1);
	CreateObject(QDAR, 743, 625*75/100, -1);
	
	CreateObject(QDAR, 718, 160*75/100, -1);
	CreateObject(QDAR, 718, 180*75/100, -1);
	CreateObject(QDAR, 718, 200*75/100, -1);
	
	tmp = CreateObject(PSTR, 627, 320*75/100, -1);
	tmp->Set(0,2);
	//tmp->DoCon(20);
	//CreateObject(FENC, 841, 115, -1);
	//CreateObject(FENC, 841-64, 115, -1);
	
	
	PlaceMedikit(929*75/100, 639*75/100, 600); // Unten Rechts
	PlaceMedikit(1134*75/100,303*75/100, 600); // Mitte rechts
	
	CreateObject(QJP2, 960*75/100,  80*75/100+2, -1)->Set(-60, -10);
	CreateObject(QJP1, 960*75/100, 281*75/100+1, -1)->Set(0,-120);
	CreateObject(QJP2,1001*75/100, 390*75/100+2, -1)->Set(30,0);
	CreateObject(QJP2, 983*75/100, 390*75/100+2, -1)->Set(-30,0);
	CreateObject(QJP1, 992*75/100, 641*75/100+1, -1)->Set(0,-120);
	
	PlaceSpawnpoint(Q_GL,1018*75/100, 330*75/100, 300); // Granatenwerfer
	
	CreateObject(AWRP, 270,420, -1)->Set(675,50);
	CreateObject(AWRP, 912-270,420, -1)->Set(912-675,50);
	
	// Boni!
	PlaceBonusSpawnpoint(456, 72);
	PlaceBonusSpawnpoint(456,216);
	PlaceBonusSpawnpoint(456,435);
	
	PlaceSpawnpoint(Q_SQ, 864*75/100, 495*75/100, 500); // Wasser rechts - Squeeker
	
	PlaceWP();
}

func PlaceWP()
{
  var wp1 = CreateWP(593,436);
  var wp2 = CreateWP(318,437);
  var wp3 = CreateWP(770,416);
  var wp4 = CreateWP(147,416);
  var wp5 = CreateWP(765,246);
  var wp6 = CreateWP(148,247);
  var wp7 = CreateWP(457,99);
  var wp8 = CreateWP(456,29);
  var wp9 = CreateWP(457,218);
  var wp10 = CreateWP(457,352);
  var wp11 = CreateWP(457,396);
  var wp12 = CreateWP(457,496);
  var wp13 = CreateWP(650,470);
  var wp14 = CreateWP(697,472);
  var wp15 = CreateWP(752,471);
  var wp16 = CreateWP(734,471);
  var wp17 = CreateWP(819,281);
  var wp18 = CreateWP(661,296);
  var wp19 = CreateWP(627,274);
  var wp20 = CreateWP(822,236);
  var wp21 = CreateWP(720,201);
  var wp22 = CreateWP(564,79);
  var wp23 = CreateWP(634,181);
  var wp24 = CreateWP(648,364);
  var wp25 = CreateWP(636,404);
  var wp26 = CreateWP(497,468);
  var wp27 = CreateWP(538,436);
  var wp28 = CreateWP(374,436);
  var wp29 = CreateWP(415,468);
  var wp30 = CreateWP(276,404);
  var wp31 = CreateWP(265,361);
  var wp32 = CreateWP(278,181);
  var wp33 = CreateWP(348,79);
  var wp34 = CreateWP(192,201);
  var wp35 = CreateWP(90,236);
  var wp36 = CreateWP(284,277);
  var wp37 = CreateWP(251,296);
  var wp38 = CreateWP(93,281);
  var wp39 = CreateWP(178,471);
  var wp40 = CreateWP(160,471);
  var wp41 = CreateWP(215,472);
  var wp42 = CreateWP(262,470);
  wp1 -> AddPath(wp27, Path_MoveTo, -1);
  wp1 -> AddPath(wp25, Path_Jump, 1);
  wp2 -> AddPath(wp28, Path_MoveTo, 1);
  wp2 -> AddPath(wp30, Path_Jump, -1);
  wp3 -> AddPath(wp14, Path_Jump, -1);
  wp4 -> AddPath(wp41, Path_Jump, 1);
  wp5 -> AddPath(wp20, Path_Jump, 1);
  wp5 -> AddPath(wp17, Path_MoveTo, 1);
  wp5 -> AddPath(wp18, Path_MoveTo, -1);
  wp5 -> AddPath(wp19, Path_MoveTo, -1);
  wp6 -> AddPath(wp35, Path_Jump, -1);
  wp6 -> AddPath(wp38, Path_MoveTo, -1);
  wp6 -> AddPath(wp37, Path_MoveTo, 1);
  wp6 -> AddPath(wp36, Path_MoveTo, 1);
  wp7 -> AddPath(wp22, Path_MoveTo, 1);
  wp7 -> AddPath(wp33, Path_MoveTo, -1);
  wp8 -> AddPath(wp33, Path_MoveTo, -1);
  wp8 -> AddPath(wp22, Path_MoveTo, 1);
  wp9 -> AddPath(wp31, Path_MoveTo, -1);
  wp9 -> AddPath(wp10, Path_MoveTo, 0);
  wp9 -> AddPath(wp24, Path_MoveTo, 1);
  wp9 -> AddPath(wp7, Path_MoveTo, 0);
  wp12 -> AddPath(wp26, Path_Jump, 1);
  wp12 -> AddPath(wp29, Path_Jump, -1);
  wp13 -> AddPath(wp14, Path_MoveTo, 1);
  wp13 -> AddPath(wp12, Path_MoveTo, -1);
  wp13 -> AddPath(wp1, Path_Jump, -1);
  wp14 -> AddPath(wp15, Path_Jump, 1);
  wp14 -> AddPath(wp16, Path_MoveTo, 1);
  wp14 -> AddPath(wp13, Path_MoveTo, -1);
  wp17 -> AddPath(wp5, Path_Jump, -1);
  wp17 -> AddPath(wp14, Path_MoveTo, -1);
  wp18 -> AddPath(wp5, Path_Jump, 1);
  wp18 -> AddPath(wp3, Path_MoveTo, 1);
  wp18 -> AddPath(wp19, Path_MoveTo, -1);
  wp19 -> AddPath(wp21, Path_Jump, 1);
  wp19 -> AddPath(wp18, Path_MoveTo, 1);
  wp20 -> AddPath(wp21, Path_Jump, -1);
  wp20 -> AddPath(wp5, Path_MoveTo, -1);
  wp21 -> AddPath(wp22, Path_Jump, -1);
  wp22 -> AddPath(wp9, Path_MoveTo, -1);
  wp22 -> AddPath(wp32, Path_MoveTo, -1);
  wp22 -> AddPath(wp5, Path_MoveTo, 1);
  wp22 -> AddPath(wp8, Path_Jump, -1);
  wp23 -> AddPath(wp7, Path_MoveTo, -1);
  wp23 -> AddPath(wp9, Path_MoveTo, -1);
  wp24 -> AddPath(wp9, Path_MoveTo, -1);
  wp24 -> AddPath(wp10, Path_MoveTo, -1);
  wp25 -> AddPath(wp33, Path_MoveTo, -1);
  wp26 -> AddPath(wp27, Path_MoveTo, 1);
  wp26 -> AddPath(wp29, Path_Jump, -1);
  wp26 -> AddPath(wp12, Path_MoveTo, -1);
  wp27 -> AddPath(wp1, Path_MoveTo, 1);
  wp27 -> AddPath(wp26, Path_MoveTo, -1);
  wp27 -> AddPath(wp11, Path_Jump, -1);
  wp28 -> AddPath(wp2, Path_MoveTo, -1);
  wp28 -> AddPath(wp29, Path_MoveTo, 1);
  wp28 -> AddPath(wp11, Path_Jump, 1);
  wp29 -> AddPath(wp28, Path_MoveTo, -1);
  wp29 -> AddPath(wp26, Path_Jump, 1);
  wp29 -> AddPath(wp12, Path_MoveTo, 1);
  wp30 -> AddPath(wp22, Path_MoveTo, 1);
  wp31 -> AddPath(wp9, Path_MoveTo, 1);
  wp31 -> AddPath(wp10, Path_MoveTo, 1);
  wp32 -> AddPath(wp9, Path_MoveTo, 1);
  wp32 -> AddPath(wp7, Path_MoveTo, 1);
  wp33 -> AddPath(wp6, Path_MoveTo, -1);
  wp33 -> AddPath(wp9, Path_MoveTo, 1);
  wp33 -> AddPath(wp23, Path_MoveTo, 1);
  wp33 -> AddPath(wp8, Path_Jump, 1);
  wp34 -> AddPath(wp33, Path_Jump, 1);
  wp35 -> AddPath(wp34, Path_Jump, 1);
  wp35 -> AddPath(wp6, Path_MoveTo, 1);
  wp36 -> AddPath(wp34, Path_Jump, -1);
  wp36 -> AddPath(wp37, Path_MoveTo, -1);
  wp37 -> AddPath(wp6, Path_Jump, -1);
  wp37 -> AddPath(wp36, Path_MoveTo, 1);
  wp37 -> AddPath(wp4, Path_MoveTo, -1);
  wp38 -> AddPath(wp6, Path_Jump, 1);
  wp38 -> AddPath(wp41, Path_MoveTo, 1);
  wp41 -> AddPath(wp39, Path_MoveTo, -1);
  wp41 -> AddPath(wp40, Path_Jump, -1);
  wp41 -> AddPath(wp42, Path_MoveTo, 1);
  wp42 -> AddPath(wp2, Path_Jump, 1);
  wp42 -> AddPath(wp12, Path_MoveTo, 1);
  wp42 -> AddPath(wp41, Path_MoveTo, -1);
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
		
		if(iX < 192 || iX > 720)
			continue;
		
		if(GBackSolid (iX, iY-10))
			continue;
		
		if(FindObject2(Find_Or(Find_ID(QJP1), Find_ID(QJP2), Find_ID(QJP3)), Find_Distance(20,iX,iY)))
			continue;
		
		ok = true;
	}
}
