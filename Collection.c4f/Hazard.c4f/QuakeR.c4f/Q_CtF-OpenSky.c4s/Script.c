/* OpenSky */
#strict
static Flags;


func Initialize()
{   
		Flags = CreateArray();

    // Munition
    PlaceSpawnpoint(QRLA, 270,250, 50); // Rockets L
    PlaceSpawnpoint(QRLA, 950,250, 50); // Rockets R
    PlaceSpawnpoint(QRGA, 280,250, 50); // Slings L
    PlaceSpawnpoint(QRGA, 940,250, 50); // Slings L
    PlaceSpawnpoint(QSGA, 290,250, 50); // Schrot
    PlaceSpawnpoint(QSGA, 930,250, 50); // Schrot

    // Medipacks
    PlaceMedikit(162,70, 300); //Platform_l
    PlaceMedikit(1062,70, 300); //Platform_r

		// Kein Haste und kein Teleport
		BonusList = [QBBF, QBRD, QBGD, QBQD, QBRF];

    // Boni!
    PlaceBonusSpawnpoint(615,170);    
    
    // Jumppads LINKS
    CreateObject(QJP2, 60,219, -1)->Set(+45,-40); // Flagge_l
    CreateObject(QJP2, 241,267, -1)->Set(-25,-89); // Platform_lL
    CreateObject(QJP2, 320,267, -1)->Set(+45,-55); // Platform_lR
    CreateObject(QJP2, 478,217, -1)->Set(-35,-45); // Mitte_lL
    CreateObject(QJP1, 514,210, -1)->Set(+8,-90); // Mitte_lR
    
    // Jumppads RECHTS
    CreateObject(QJP2, 1169,219, -1)->Set(-45,-40); // Flagge_r
    CreateObject(QJP2, 985,267, -1)->Set(+27,-89); // Platform_rL
    CreateObject(QJP2, 904,267, -1)->Set(-45,-55); // Platform_rR
    CreateObject(QJP2, 749,217, -1)->Set(+35,-45); // Mitte_rR
    CreateObject(QJP1, 716,210, -1)->Set(-8,-90); // Mitte_rL
    

    // Teleporter
    CreateObject(AWRP, 615,278, -1)->Set(603,120); // Warp boni



  //Waypoints platzieren! Icewing! K, done!
    var wp1 = CreateWP(589,158);
  var wp2 = CreateWP(514,200);
  var wp3 = CreateWP(478,207);
  var wp4 = CreateWP(320,257);
  var wp5 = CreateWP(241,257);
  var wp6 = CreateWP(280,250);
  var wp7 = CreateWP(60,209);
  var wp8 = CreateWP(26,202);
  var wp9 = CreateWP(950,250);
  var wp10 = CreateWP(904,257);
  var wp11 = CreateWP(749,207);
  var wp12 = CreateWP(716,200);
  var wp13 = CreateWP(615,260);
  var wp14 = CreateWP(641,158);
  var wp15 = CreateWP(1200,200);
  var wp16 = CreateWP(1169,209);
  var wp17 = CreateWP(985,257);
  wp1 -> AddPath(wp2, Path_MoveTo, -1);
  wp1 -> AddPath(wp3, Path_Jump, -1);
  wp1 -> AddPath(wp13, Path_MoveTo, 1);
  wp1 -> AddPath(wp14, Path_MoveTo, 1);
  wp1 -> AddPath(wp11, Path_Backflip, 1);
  wp2 -> AddPath(wp1, Path_MoveTo, 1);
  wp3 -> AddPath(wp6, Path_MoveTo, -1);
  wp4 -> AddPath(wp2, Path_MoveTo, 1);
  wp5 -> AddPath(wp8, Path_MoveTo, -1);
  wp6 -> AddPath(wp5, Path_MoveTo, -1);
  wp6 -> AddPath(wp4, Path_MoveTo, 1);
  wp7 -> AddPath(wp6, Path_MoveTo, 1);
  wp8 -> AddPath(wp7, Path_MoveTo, 1);
  wp9 -> AddPath(wp10, Path_MoveTo, -1);
  wp9 -> AddPath(wp17, Path_MoveTo, 1);
  wp10 -> AddPath(wp12, Path_MoveTo, -1);
  wp11 -> AddPath(wp9, Path_MoveTo, 1);
  wp12 -> AddPath(wp14, Path_MoveTo, -1);
  wp14 -> AddPath(wp1, Path_MoveTo, -1);
  wp14 -> AddPath(wp13, Path_MoveTo, -1);
  wp14 -> AddPath(wp12, Path_MoveTo, 1);
  wp14 -> AddPath(wp11, Path_Jump, 1);
  wp14 -> AddPath(wp3, Path_Backflip, -1);
  wp15 -> AddPath(wp16, Path_MoveTo, -1);
  wp16 -> AddPath(wp9, Path_MoveTo, -1);
  wp17 -> AddPath(wp15, Path_MoveTo, 1);
}

/* Flaggen */

protected func InitTeamFlag(int iTeam)
{
  var x,y,color;
  if(iTeam == 1)
  {
    x = 26;
    y = 212;
    color = RGB(126,93,57);
  }
  if(iTeam == 2)
  {
    x = 1200;
    y = 212;
    color = RGB(57,126,60);
  }
  CreateFlag(iTeam, x, y, color);
}




/* Relaunch */

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  // Flagge setzen (wenn nicht schon erledigt
  if(!Flags[iTeam])
  {
    Flags[iTeam] = true;
    InitTeamFlag(iTeam);
  }
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

 	pCrew->CreateContents(Q_RG);
 	pCrew->CreateContents(Q_RL);
 	pCrew->CreateContents(Q_SG);
 	CreateObject(QRGA)->Activate(pCrew);
 	CreateObject(QRLA)->Activate(pCrew);
 	CreateObject(QSGA)->Activate(pCrew);

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

global func RelaunchPosition(& iX, & iY, int iTeam)
{
  if(iTeam == 1)
  {
    iX = 20;
    iY = 198;
    if(!Random(3)) iY = 25;
  }
  if(iTeam == 2)
  {
    iX = 1200;
    iY = 198;
    if(!Random(3)) iY = 25;
  }
  }
