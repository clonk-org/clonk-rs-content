/* FrostBite */
#strict

static const MAX_RelaunchPositionTries=1024;

func Initialize()
{
	//Gamma
	 SetGamma (RGB(0,0,25),RGB(128,128,150),RGB(230,234,255));

	// Waffen
	PlaceSpawnpoint(Q_SG, 59,45, 300); // Shotgun
	PlaceSpawnpoint(Q_RG, 630,45, 300); // Railgun
	PlaceSpawnpoint(Q_RL, 358,328, 900); // Rocket
	
	// Munition
	PlaceSpawnpoint(QSGA, 638,138, 200); // Shrot
	PlaceSpawnpoint(QRGA, 90,138, 200); // Slingshots
	PlaceSpawnpoint(QMGA, 359,50, 220); // MG
	PlaceSpawnpoint(QRLA, 208,287, 300); // Rockets
	PlaceSpawnpoint(QRLA, 517,287, 300); // Rockets

	// Medipacks
	PlaceMedikit(313,334, 350);
	PlaceMedikit(405,334, 350);
	PlaceMedikit(130,246, 450);
	PlaceMedikit(583,246, 450);
	
	// Blutsauger
	CreateObject(QBDS, 300,309, -1); // Kammer links
	CreateObject(QBDS, 400,309, -1); // Kammer rechts
	CreateObject(QBDS, 48, 244, -1); // Teleporter links
	CreateObject(QBDS, 660,244, -1); // Teleporter rechts
	
	// Jumppads
	CreateObject(QJP3, 30, 57, -1)->Set(+130,-30); // oben Links
	CreateObject(QJP3, 691,57, -1)->Set(-130,-30); // oben Rechts
	CreateObject(QJP2, 334,179, -1)->Set(-50,-75); //Mitte Links
	CreateObject(QJP2, 388,179, -1)->Set(+50,-75); //Mitte Rechts

	// Teleporter
	CreateObject(AWRP, 7,253, -1)->Set(359,50); // Warp Links
	CreateObject(AWRP, 700,253, -1)->Set(359,50); // Warp Links

	// Deko
	CreateObject(QQSC, 361,330, -1); //Screen
	
	CreateObject(D_WS, 165,241, -1)->SetR(-30);
	CreateObject(D_WS, 214,220, -1)->SetR(-5);
	CreateObject(D_WS, 289,190, -1)->SetR(-5);
	CreateObject(D_WS, 421,191, -1)->SetR(10);
	CreateObject(D_WS, 485,210, -1)->SetR(40);
	CreateObject(D_WS, 540,238, -1)->SetR(80);

	for(var wreck in FindObjects(Find_ID(D_WS)))
		wreck->SetCategory(wreck->GetCategory() | C4D_Background);

	// Deko
	CreateObject(Q_X1, 19,168, -1);
	CreateObject(Q_X1, 648,179, -1);
	
	PlaceVegetation(TRB1, 140,150, 130,100,RandomX(90000,100000));
	PlaceVegetation(TRB2, 140,150, 130,100,RandomX(90000,100000));
	PlaceVegetation(TRB1, 270,150, 90,100,RandomX(90000,100000));
	PlaceVegetation(TRB2, 270,150, 90,100,RandomX(90000,100000));
	
	PlaceVegetation(TRB1, 452,150, 130,100,RandomX(90000,100000));
	PlaceVegetation(TRB2, 452,150, 130,100,RandomX(90000,100000));
	PlaceVegetation(TRB1, 362,150, 90,100,RandomX(90000,100000));
	PlaceVegetation(TRB2, 322,150, 90,100,RandomX(90000,100000));
	
	for (var pTree in FindObjects(Find_Func("IsTree"))) pTree->SetObjectLayer(pTree);
	
	//Waypoints
  var wp1 = CreateWP(388,169);
  var wp2 = CreateWP(334,169);
  var wp3 = CreateWP(691,52);
  var wp4 = CreateWP(30,52);
  var wp5 = CreateWP(496,200);
  var wp6 = CreateWP(226,200);
  var wp7 = CreateWP(700,233);
  var wp8 = CreateWP(7,233);
  var wp9 = CreateWP(613,236);
  var wp10 = CreateWP(102,236);
  var wp11 = CreateWP(359,163);
  var wp12 = CreateWP(359,50);
  var wp13 = CreateWP(658,134);
  var wp14 = CreateWP(62,135);
  var wp15 = CreateWP(359,327);
  var wp16 = CreateWP(643,47);
  var wp17 = CreateWP(69,47);
  var wp18 = CreateWP(283,179);
  var wp19 = CreateWP(438,178);
  wp1 -> AddPath(wp16, Path_MoveTo, 1);
  wp2 -> AddPath(wp17, Path_MoveTo, -1);
  wp3 -> AddPath(wp14, Path_MoveTo, -1);
  wp4 -> AddPath(wp13, Path_MoveTo, 1);
  wp5 -> AddPath(wp19, Path_MoveTo, -1);
  wp5 -> AddPath(wp9, Path_Jump, 1);
  wp6 -> AddPath(wp18, Path_MoveTo, 1);
  wp6 -> AddPath(wp10, Path_Jump, -1);
  wp7 -> AddPath(wp12, Path_MoveTo, -1);
  wp8 -> AddPath(wp12, Path_MoveTo, 1);
  wp9 -> AddPath(wp5, Path_Jump, -1);
  wp9 -> AddPath(wp7, Path_MoveTo, 1);
  wp9 -> AddPath(wp15, Path_MoveTo, -1);
  wp10 -> AddPath(wp6, Path_Jump, 1);
  wp10 -> AddPath(wp8, Path_MoveTo, -1);
  wp10 -> AddPath(wp15, Path_MoveTo, 1);
  wp11 -> AddPath(wp15, Path_MoveTo, -1);
  wp12 -> AddPath(wp6, Path_Jump, -1);
  wp12 -> AddPath(wp5, Path_Jump, 1);
  wp13 -> AddPath(wp5, Path_Backflip, -1);
  wp13 -> AddPath(wp5, Path_Jump, -1);
  wp13 -> AddPath(wp5, Path_MoveTo, -1);
  wp14 -> AddPath(wp6, Path_Jump, 1);
  wp14 -> AddPath(wp6, Path_MoveTo, 1);
  wp14 -> AddPath(wp6, Path_Backflip, 1);
  wp15 -> AddPath(wp10, Path_MoveTo, -1);
  wp15 -> AddPath(wp9, Path_MoveTo, 1);
  wp16 -> AddPath(wp3, Path_MoveTo, 1);
  wp16 -> AddPath(wp5, Path_MoveTo, -1);
  wp16 -> AddPath(wp19, Path_Backflip, -1);
  wp16 -> AddPath(wp5, Path_Jump, -1);
  wp17 -> AddPath(wp4, Path_MoveTo, -1);
  wp17 -> AddPath(wp6, Path_MoveTo, 1);
  wp17 -> AddPath(wp18, Path_Backflip, 1);
  wp17 -> AddPath(wp6, Path_Jump, 1);
  wp18 -> AddPath(wp6, Path_MoveTo, -1);
  wp18 -> AddPath(wp11, Path_Jump, 1);
  wp19 -> AddPath(wp5, Path_MoveTo, 1);
  wp19 -> AddPath(wp1, Path_MoveTo, -1);
  wp19 -> AddPath(wp11, Path_Jump, -1);
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
																																																		global func CastBlood(int strength){if(!QEGGwinter)return(inherited(strength));	if(GetBlood(2))		CastParticles("Blood", (Random(4)+1)*strength*GetBlood(),RandomX(30,50),0,0,10,30,RGB(100,100,180),RGB(100,100,255));	else		CastPXS("Snow", (Random(6)+1)*strength*GetBlood(), RandomX(20,40));}
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
	var tries;
	while(!ok)
	{
		var obj = PlaceAnimal(WIPF);
		iX = obj->GetX();
		iY = obj->GetY();
		RemoveObject(obj);
		
		if (tries++>MAX_RelaunchPositionTries)
		{
			ErrorLog("Couldn't find a valid relaunch position.");
			break;
		}
		
		if(GBackSolid (iX, iY-10))
			continue;
		
		if(FindObject2(Find_Or(Find_ID(QJP1), Find_ID(QJP2), Find_ID(QJP3)), Find_Distance(20,iX,iY)))
			continue;
		
		ok = true;
	}
}
