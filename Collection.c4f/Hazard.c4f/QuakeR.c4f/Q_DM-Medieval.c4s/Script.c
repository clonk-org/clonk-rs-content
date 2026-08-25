/* Medieval */
#strict

func Initialize()
{
	var d = 6;

	PlaceDeko();

	// Waffen
	PlaceSpawnpoint(Q_RL, 486,127-d, 200); // Raketenwerfer
	PlaceSpawnpoint(Q_RG, 264,547-d, 200); // Railgun
	PlaceSpawnpoint(Q_PG, 555,363-d, 200); // Plasma
	PlaceSpawnpoint(Q_SG, 765,547-d, 200); // Shotgun
	PlaceSpawnpoint(Q_GL, 257,714-d, 200); // Granatenwerfer
	
	// Munition
	PlaceSpawnpoint(QRLA, 436,127-d, 200); // Raketenwerfer
	PlaceSpawnpoint(QRLA, 536,127-d, 200); // Raketenwerfer
	PlaceSpawnpoint(QGLA, 420,280-d, 200); // Granatenwerfer
	PlaceSpawnpoint(QPGA, 941,273-d, 200); // Plasma
	PlaceSpawnpoint(QRGA, 325,547-d, 200); // Railgun
	PlaceSpawnpoint(QMGA, 547,547-d, 200); // MG
	PlaceSpawnpoint(QSGA, 805,547-d, 200); // Shotgun
	PlaceSpawnpoint(QGLA, 318,714-d, 200); // Granatenwerfer
	
	// Medikits
	PlaceMedikit(490,361,350); // Turm
	PlaceMedikit(61,382,350);  // linkes haus
	PlaceMedikit(499,714,350); // Kerker

	// Boni!
	PlaceBonusSpawnpoint(487,235, 2000);
	PlaceBonusSpawnpoint(379,678, 2000);

	// Jumppads
	CreateObject(QJP2, 350, 290, -1)->Set(-50,-15); // Turm oben links
	CreateObject(QJP2, 622, 373, -1)->Set(+90,-50); // Turm oben rechts

	CreateObject(QJP1, 465, 557, -1)->Set(11,-94); // Turm unten links
	CreateObject(QJP1, 507, 557, -1)->Set(-9,-110); // Turm unten rechts

	CreateObject(QJP2, 15, 365, -1)->Set(50,-110); // Haus links
	CreateObject(QJP2, 985, 240, -1)->Set(-60,-90); // Haus rechts
	
	CreateObject(QJP3, 880, 547, -1)->Set(-60,-60); // Haus unten rechts

	//CreateObject(QJP1, 149, 701, -1)->Set(7,-87); // Kerker links
	CreateObject(QJP1, 610, 701, -1)->Set(-7,-87); // Kerker rechts
	
	// Warps
	CreateObject(QWRP, 560,448, -1)->Set(560,700); // oben auf dem kleinen Dach... Dingens
	CreateObject(QWRP, 379,715, -1)->Set(486,80); // im Kerker
	
	// Blutsauger
	CreateObject(QBDS, 410,713, -1);
	CreateObject(QBDS, 440,713, -1);
	
	PlaceWP();
}

func PlaceWP()
{
  var wp1 = CreateWP(538,540);
  var wp2 = CreateWP(437,540);
  var wp3 = CreateWP(510,548);
  var wp4 = CreateWP(462,548);
  var wp5 = CreateWP(621,358);
  var wp6 = CreateWP(906,296);
  var wp7 = CreateWP(608,691);
  var wp8 = CreateWP(576,695);
  var wp9 = CreateWP(412,707);
  var wp10 = CreateWP(346,709);
  var wp11 = CreateWP(147,696);
  var wp12 = CreateWP(264,537);
  var wp13 = CreateWP(130,401);
  var wp14 = CreateWP(353,275);
  var wp15 = CreateWP(18,355);
  var wp16 = CreateWP(565,437);
  var wp17 = CreateWP(639,448);
  var wp18 = CreateWP(878,539);
  var wp19 = CreateWP(761,537);
  var wp20 = CreateWP(522,356);
  var wp21 = CreateWP(452,272);
  var wp22 = CreateWP(591,123);
  var wp23 = CreateWP(381,123);
  var wp24 = CreateWP(537,122);
  var wp25 = CreateWP(441,121);
  var wp26 = CreateWP(981,231);
  var wp27 = CreateWP(632,540);
  var wp28 = CreateWP(566,539);
  var wp29 = CreateWP(379,273);
  var wp30 = CreateWP(589,355);
  var wp31 = CreateWP(380,704);
  wp1 -> AddPath(wp28, Path_MoveTo, 1);
  wp1 -> AddPath(wp2, Path_Jump, -1);
  wp1 -> AddPath(wp3, Path_MoveTo, -1);
  wp2 -> AddPath(wp12, Path_MoveTo, -1);
  wp2 -> AddPath(wp1, Path_Jump, 1);
  wp2 -> AddPath(wp4, Path_MoveTo, 1);
  wp3 -> AddPath(wp21, Path_MoveTo, -1);
  wp4 -> AddPath(wp20, Path_MoveTo, 1);
  wp5 -> AddPath(wp6, Path_MoveTo, 1);
  wp6 -> AddPath(wp26, Path_MoveTo, 1);
  wp6 -> AddPath(wp19, Path_MoveTo, -1);
  wp7 -> AddPath(wp28, Path_MoveTo, -1);
  wp8 -> AddPath(wp9, Path_MoveTo, -1);
  wp8 -> AddPath(wp7, Path_MoveTo, 1);
  wp9 -> AddPath(wp8, Path_MoveTo, 1);
  wp9 -> AddPath(wp10, Path_Jump, -1);
  wp9 -> AddPath(wp31, Path_MoveTo, -1);
  wp10 -> AddPath(wp11, Path_MoveTo, -1);
  wp10 -> AddPath(wp9, Path_Jump, 1);
  wp10 -> AddPath(wp31, Path_MoveTo, 1);
  wp11 -> AddPath(wp12, Path_Jump, 1);
  wp11 -> AddPath(wp10, Path_MoveTo, 1);
  wp12 -> AddPath(wp2, Path_MoveTo, 1);
  wp12 -> AddPath(wp11, Path_MoveTo, -1);
  wp13 -> AddPath(wp12, Path_MoveTo, 1);
  wp13 -> AddPath(wp15, Path_MoveTo, -1);
  wp14 -> AddPath(wp13, Path_MoveTo, -1);
  wp15 -> AddPath(wp25, Path_MoveTo, 1);
  wp16 -> AddPath(wp8, Path_MoveTo, 1);
  wp17 -> AddPath(wp19, Path_MoveTo, 1);
  wp17 -> AddPath(wp19, Path_Jump, 1);
  wp17 -> AddPath(wp16, Path_MoveTo, -1);
  wp18 -> AddPath(wp17, Path_MoveTo, -1);
  wp19 -> AddPath(wp27, Path_MoveTo, -1);
  wp19 -> AddPath(wp18, Path_MoveTo, 1);
  wp20 -> AddPath(wp30, Path_MoveTo, 1);
  wp20 -> AddPath(wp2, Path_MoveTo, -1);
  wp21 -> AddPath(wp29, Path_MoveTo, -1);
  wp21 -> AddPath(wp20, Path_MoveTo, 1);
  wp21 -> AddPath(wp20, Path_Jump, 1);
  wp22 -> AddPath(wp24, Path_MoveTo, -1);
  wp22 -> AddPath(wp18, Path_Backflip, 1);
  wp22 -> AddPath(wp19, Path_MoveTo, 1);
  wp22 -> AddPath(wp19, Path_Jump, 1);
  wp23 -> AddPath(wp25, Path_MoveTo, 1);
  wp23 -> AddPath(wp13, Path_Backflip, -1);
  wp23 -> AddPath(wp11, Path_Jump, -1);
  wp23 -> AddPath(wp12, Path_MoveTo, -1);
  wp24 -> AddPath(wp25, Path_MoveTo, -1);
  wp24 -> AddPath(wp22, Path_MoveTo, 1);
  wp25 -> AddPath(wp24, Path_MoveTo, 1);
  wp25 -> AddPath(wp23, Path_MoveTo, -1);
  wp25 -> AddPath(wp11, Path_Backflip, -1);
  wp26 -> AddPath(wp24, Path_MoveTo, -1);
  wp27 -> AddPath(wp19, Path_MoveTo, 1);
  wp27 -> AddPath(wp28, Path_Jump, -1);
  wp27 -> AddPath(wp8, Path_MoveTo, -1);
  wp28 -> AddPath(wp1, Path_MoveTo, -1);
  wp28 -> AddPath(wp27, Path_Jump, 1);
  wp29 -> AddPath(wp21, Path_MoveTo, 1);
  wp29 -> AddPath(wp14, Path_MoveTo, -1);
  wp29 -> AddPath(wp12, Path_Jump, -1);
  wp30 -> AddPath(wp20, Path_MoveTo, -1);
  wp30 -> AddPath(wp5, Path_MoveTo, 1);
  wp30 -> AddPath(wp19, Path_Jump, 1);
  wp31 -> AddPath(wp25, Path_MoveTo, 1);
}

func PlaceDeko()
{
	var tmp;
	
	// Deko. n' Stuff.
	CreateObject(TORC, 488,455, -1);
	CreateObject(TORC, 488,300, -1);
	
	CreateObject(TORC, 200,522, -1);
	CreateObject(TORC, 350,522, -1);
	CreateObject(TORC, 599,533, -1);
	CreateObject(TORC, 772,522, -1);
	
	CreateObject(TORC, 260,690, -1);
	CreateObject(TORC, 379,690, -1);
	CreateObject(TORC, 500,690, -1);
	
	CreateObject(BANR, 379,130, -1);
	CreateObject(BANR, 432,130, -1);
	CreateObject(BANR, 486,130, -1);
	CreateObject(BANR, 540,130, -1);
	CreateObject(BANR, 594,130, -1);
	
	CreateObject(BANR, 362,282, -1);
	CreateObject(BANR, 610,364, -1);
	
	
	// Linkes Haus - Küche
	LeftHouse();
	// Rechtes Haus - Schmiede und Ställe
	RightHouse();
	
	var h = 548;
	// Linker Platz:
	CreateObject(BWRC, 230,h, -1)->SetPhase(3);
	CreateObject(SHIE, 230,512, -1)->SetCategory(C4D_StaticBack);
	tmp = CreateObject(SPER, 264,510, -1);
	tmp->SetR(120);
	tmp->SetCategory(C4D_StaticBack);
	tmp = CreateObject(SPER, 264,510, -1);
	tmp->SetR(60);
	tmp->SetCategory(C4D_StaticBack);
	CreateObject(SHIE, 310,512, -1)->SetCategory(C4D_StaticBack);
	CreateObject(RAIL, 295,h, -1);
	CreateObject(CPFR, 325,h, -1)->SetAction("Idle");
	CreateObject(RAIL, 355,h, -1);
	CreateObject(SPRC, 395,h, -1)->SetPhase(5);
	
	for(var i = 0; i < 5; i++)
	{
		tmp = CreateObject(LANC, 167 + i*42,510, -1);
		tmp->SetR(90);
		tmp->SetCategory(C4D_StaticBack);
		tmp->SetClrModulation(RGB(150,150,150));
	}
	tmp = CreateObject(LANC, 395,509, -1);
	tmp->SetR(-90);
	tmp->SetCategory(C4D_StaticBack);
	tmp->SetClrModulation(RGB(150,150,150));
	
	// Rechter Platz:
	tmp = CreateObject(SHIE, 673,551, -1);
	tmp->SetR(68);
	
	tmp = CreateObject(SHIE, 670,512, -1);
	tmp->SetCategory(C4D_StaticBack);
	//tmp->SetObjectBlitMode(1);
	tmp->SetClrModulation(RGBa(0,0,0,100));
	
	
	tmp = CreateObject(SWOR, 710,510, -1);
	tmp->SetR(-90);
	tmp->SetCategory(C4D_StaticBack);
	tmp = CreateObject(SWOR, 711,512, -1);
	tmp->SetR(-5);
	tmp->SetCategory(C4D_StaticBack);
	CreateObject(SHIE, 750,512, -1)->SetCategory(C4D_StaticBack);
	
	CreateObject(SWRC, 730,h, -1)->SetPhase(5);
	tmp = CreateObject(SHRC, 790,h, -1);
	tmp->SetPhase(3);
	tmp->SetColorDw(RGB(200));
	CreateObject(TENT, 830,h, -1)->SetClrModulation(RGB(100));
	tmp = CreateObject(FLAG,0,0,-1);
	tmp->SetColorDw(RGB(200));
	tmp->SetAction("FlyBase",FindObject(TENT));
	
	for(var i = 0; i < 5; i++)
	{
		tmp = CreateObject(LANC, 688 + i*42,510, -1);
		tmp->SetR(90);
		tmp->SetCategory(C4D_StaticBack);
		tmp->SetClrModulation(RGB(150,150,150));
	}
	//tmp = CreateObject(LANC, 395,509, -1);
	tmp->SetR(-90);
	tmp->SetCategory(C4D_StaticBack);
	tmp->SetClrModulation(RGB(150,150,150));
	
	var obj;
	for(obj in FindObjects(Find_Owner(-1), Find_Not(Find_Or(Find_ID(QJP1), Find_ID(QJP2), Find_ID(QJP3), Find_ID(QWRP)))))
		obj->SetObjectLayer(obj);
	
	for(obj in FindObjects(Find_ID(WOOD)))
		RemoveObject(obj);
	
	for(obj in FindObjects(Find_ID(BANR)))
		obj->SetClrModulation(RGB(200));
	for(obj in FindObjects(Find_ID(SHIE)))
		obj->SetColorDw(RGB(200));
}

func LeftHouse()
{
	// Oben:
	CreateObject(FLOU, 10,475, -1);
	CreateObject(FLOU, 20,475, -1);
	CreateObject(FLOU, 30,475, -1);
	CreateObject(FLOU, 40,475, -1);
	
	CreateObject(WDWB,  95,475, -1);
	CreateObject(WDWB, 100,475, -1);
	CreateObject(WDWB, 105,475, -1);
	CreateObject(WDWB, 110,475, -1);
	CreateObject(WDWB, 115,475, -1);
	// Unten:
	CreateObject(DPOT, 17,550, -1);
	CreateObject(CLD2, 60,538, -1);
	CreateObject(CPFR, 60,550, -1)->SetAction("Idle");
	CreateObject(OVEN, 95,550, -1);
}

func RightHouse()
{
	// Oben:
	CreateObject(FRGE, 920,440, -1);
	CreateObject(ANVL, 960,440, -1);
	CreateObject(CHST, 995,440, -1);
	CreateObject(CHST, 985,440, -1);
	
	// Unten:
	CreateObject(HORS, 925,550, -1)->SetDir(DIR_Left);
	CreateObject(HORS, 950,550, -1)->SetDir(DIR_Left);
	CreateObject(HORS, 975,550, -1)->SetDir(DIR_Left);
	//CreateObject(HORS, 985,550, -1)->SetDir(DIR_Left);
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
		
		// nicht links unten in den deko-kammern spawnen
		if(Inside(iX, 0,50) || (iX < 140 && iY > 400))
			continue;

		// rechts auch nicht
		if(Inside(iX, 960, 1000) || (iX > 885 && iY > 340))
			continue;

		// nicht an den ecken der Dächer spawnen, da bleibt man stecken
		if((Inside(iX, 110,160) && iY < 540)
		|| (Inside(iX, 860,900) && iY < 400))
			continue;
		
		// Nicht auf jumppads spawnen
		if(FindObject2(Find_Or(Find_ID(QJP1), Find_ID(QJP2), Find_ID(QJP3)), Find_Distance(20,iX,iY)))
			continue;
		
		ok = true;
	}
}


func Script50()
{
	goto(1);

	if(Random(3))
		return();
	
	// Pfeilsalve!
	var x = Random(LandscapeWidth()*6/8) + LandscapeWidth()/8;
	var y = RandomX(470,550);
	var angle = RandomX(-45,45);
	var xspeed = Sin(angle,100+Random(30));
	var yspeed =-Cos(angle,100+Random(40));

	var i,j;
	for(i = Random(3)+7; i; i--)
	{
		var arrow = CreateObject(FFRW, x + RandomX(-30,30),y + RandomX(-30,30), -1);
		//CreateParticle("PSpark",  x + j*5 + i*xoff,y + i*5 + j*yoff,0,0,25,RGB(255));
		arrow->Launch(xspeed,yspeed);
		arrow->SetObjectLayer(arrow);
	}
		
}