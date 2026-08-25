/*-- DeepDown --*/

#strict

func Initialize()
{
	var w = LandscapeWidth();
	var d = 6;  
  
  // Warps
  var warp = CreateObject (QWRP, 100, 550, -1);
  warp -> Set(530,400);
  warp -> SetR(90);
  
  var warp = CreateObject (QWRP, w-100, 550, -1);
  warp -> Set(w-530,400);
  warp -> SetR(-90);
  
  // Jumppads
  CreateObject (QJP2, 230, 552, -1) -> Set(-20,-92);
  CreateObject (QJP2, 358, 663, -1) -> Set(25,-55);
  CreateObject (QJP2,  30, 336, -1) -> Set(40,-90);
  CreateObject (QJP1, 290, 326, -1) -> Set(35,-85);
  
  CreateObject (QJP2, w-230, 552, -1) -> Set(20,-92);
  CreateObject (QJP2, w-358, 663, -1) -> Set(-25,-55);
  CreateObject (QJP2,  w-30, 336, -1) -> Set(-40,-90);
  CreateObject (QJP1, w-290, 326, -1) -> Set(-35,-85);
  
  // Waffen und Munition und Zeug
  // linke Seite
  PlaceSpawnpoint(QRGA, 270,159-d, 300);
  PlaceSpawnpoint(Q_RL,  55,372-d, 300);
  PlaceSpawnpoint(QRLA, 100,372-d, 300);
  PlaceSpawnpoint(QMGA, 325,326-d, 300);
  PlaceSpawnpoint(QPGA, 330,454-d, 300);
  PlaceSpawnpoint(QSGA, 250,592-d, 300);
  PlaceMedikit(150,592, 300);
  PlaceMedikit(200,150, 300);
  PlaceBonusSpawnpoint(235, 640);
  
  
  // rechte Seite
  PlaceSpawnpoint(QPGA, w-270,159-d, 300);
  PlaceSpawnpoint(Q_GL,  w-55,372-d, 300);
  PlaceSpawnpoint(QGLA, w-100,372-d, 300);
  PlaceSpawnpoint(QMGA, w-325,326-d, 300);
  PlaceSpawnpoint(QRGA, w-330,454-d, 300);
  PlaceSpawnpoint(QSGA, w-250,592-d, 300);
  PlaceMedikit(w-150,592, 300);
  PlaceMedikit(w-200,150, 300);
  PlaceBonusSpawnpoint(w-235, 640);
  
  
  // Mitte
  PlaceSpawnpoint(Q_RG, 400,120, 300);
  PlaceSpawnpoint(Q_PG, 400,415, 300);
  PlaceSpawnpoint(Q_SG, 400,555, 300);
  PlaceMedikit(400,180, 300);
  PlaceBonusSpawnpoint(400, 290);
  
  // Blutsauger
  CreateObject(QBDS, 361,592, -1);
  CreateObject(QBDS, 400,592, -1);
  CreateObject(QBDS, w-361,592, -1);

  
  // Effekte!
  CreateObject(PIPL, 400, 80, -1)->Solid()->Up(2,true);
  AddLavaEffects();
  
  CreateObject(SUCK, 400, 740, -1);
  CreateObject(QWSC, 400, 397+30, -1);
  CreateObject(QWSC, 183, 442+30, -1);
  CreateObject(QWSC, w-183, 442+30, -1);

  CreateObject(QDD2, 800-297,  257);
  CreateObject(QDD1, 722,  372);  
  CreateObject(QDD1, 78 ,  372);
  CreateObject(QDD1, 297 , 257 )->SetGraphics("2");

  var p = 52;

  CreateObject(QDD1, 503+1,  257-p)->SetGraphics("3");
  CreateObject(QDD1, 722+1,  372-p)->SetGraphics("3");  
  CreateObject(QDD1, 78+1,  372-p)->SetGraphics("3");
  CreateObject(QDD1, 297+1 , 257-p)->SetGraphics("3");


	for(var obj in FindObjects(Find_Func("IsJumppad")))
		obj->SetClrModulation(RGB(255,0,0));
  
	//PlaceWP();
  
  ScriptGo(true);
}

func PlaceWP()
{
  var wp1 = CreateWP(650,583);
  var wp2 = CreateWP(700,537);
  var wp3 = CreateWP(101,538);
  var wp4 = CreateWP(600,584);
  var wp5 = CreateWP(532,584);
  var wp6 = CreateWP(210,583);
  var wp7 = CreateWP(260,584);
  var wp8 = CreateWP(443,654);
  var wp9 = CreateWP(768,327);
  var wp10 = CreateWP(733,362);
  var wp11 = CreateWP(354,586);
  var wp12 = CreateWP(569,542);
  var wp13 = CreateWP(447,450);
  var wp14 = CreateWP(289,318);
  var wp15 = CreateWP(331,318);
  var wp16 = CreateWP(444,145);
  var wp17 = CreateWP(578,160);
  var wp18 = CreateWP(32,327);
  var wp19 = CreateWP(222,160);
  var wp20 = CreateWP(356,145);
  var wp21 = CreateWP(469,318);
  var wp22 = CreateWP(511,318);
  var wp23 = CreateWP(353,450);
  var wp24 = CreateWP(446,586);
  var wp25 = CreateWP(231,542);
  var wp26 = CreateWP(67,362);
  var wp27 = CreateWP(153,583);
  var wp28 = CreateWP(357,654);
  var wp29 = CreateWP(477,675);
  var wp30 = CreateWP(327,682);
  var wp31 = CreateWP(269,674);
  var wp32 = CreateWP(522,675);
  wp1 -> AddPath(wp4, Path_MoveTo, -1);
  wp1 -> AddPath(wp24, Path_MoveTo, -1);
  wp1 -> AddPath(wp2, Path_Jump, 1);
  wp2 -> AddPath(wp23, Path_MoveTo, -1);
  wp3 -> AddPath(wp13, Path_MoveTo, 1);
  wp4 -> AddPath(wp12, Path_Jump, -1);
  wp5 -> AddPath(wp12, Path_Jump, 1);
  wp6 -> AddPath(wp25, Path_Jump, 1);
  wp7 -> AddPath(wp25, Path_Jump, -1);
  wp8 -> AddPath(wp11, Path_MoveTo, -1);
  wp9 -> AddPath(wp17, Path_MoveTo, -1);
  wp10 -> AddPath(wp1, Path_MoveTo, -1);
  wp10 -> AddPath(wp24, Path_MoveTo, -1);
  wp10 -> AddPath(wp9, Path_Jump, 1);
  wp10 -> AddPath(wp24, Path_Backflip, -1);
  wp10 -> AddPath(wp5, Path_Backflip, -1);
  wp11 -> AddPath(wp7, Path_MoveTo, -1);
  wp11 -> AddPath(wp27, Path_MoveTo, -1);
  wp11 -> AddPath(wp24, Path_Jump, 1);
  wp11 -> AddPath(wp24, Path_Backflip, 1);
  wp11 -> AddPath(wp8, Path_MoveTo, 1);
  wp12 -> AddPath(wp10, Path_MoveTo, 1);
  wp13 -> AddPath(wp23, Path_Jump, -1);
  wp13 -> AddPath(wp11, Path_MoveTo, -1);
  wp14 -> AddPath(wp16, Path_MoveTo, 1);
  wp15 -> AddPath(wp14, Path_MoveTo, -1);
  wp15 -> AddPath(wp13, Path_Jump, 1);
  wp15 -> AddPath(wp24, Path_MoveTo, 1);
  wp16 -> AddPath(wp17, Path_MoveTo, 1);
  wp16 -> AddPath(wp15, Path_MoveTo, -1);
  wp16 -> AddPath(wp20, Path_Jump, -1);
  wp17 -> AddPath(wp1, Path_MoveTo, 1);
  wp17 -> AddPath(wp10, Path_Jump, 1);
  wp17 -> AddPath(wp16, Path_MoveTo, -1);
  wp18 -> AddPath(wp19, Path_MoveTo, 1);
  wp19 -> AddPath(wp27, Path_MoveTo, -1);
  wp19 -> AddPath(wp26, Path_Jump, -1);
  wp19 -> AddPath(wp20, Path_MoveTo, 1);
  wp20 -> AddPath(wp19, Path_MoveTo, -1);
  wp20 -> AddPath(wp21, Path_MoveTo, 1);
  wp20 -> AddPath(wp16, Path_Jump, 1);
  wp21 -> AddPath(wp22, Path_MoveTo, 1);
  wp21 -> AddPath(wp23, Path_Jump, -1);
  wp21 -> AddPath(wp11, Path_MoveTo, -1);
  wp22 -> AddPath(wp20, Path_MoveTo, -1);
  wp23 -> AddPath(wp13, Path_Jump, 1);
  wp23 -> AddPath(wp24, Path_MoveTo, 1);
  wp24 -> AddPath(wp1, Path_MoveTo, 1);
  wp24 -> AddPath(wp5, Path_MoveTo, 1);
  wp24 -> AddPath(wp11, Path_Jump, -1);
  wp24 -> AddPath(wp11, Path_Backflip, -1);
  wp24 -> AddPath(wp28, Path_MoveTo, -1);
  wp25 -> AddPath(wp26, Path_MoveTo, -1);
  wp26 -> AddPath(wp27, Path_MoveTo, 1);
  wp26 -> AddPath(wp11, Path_MoveTo, 1);
  wp26 -> AddPath(wp18, Path_Jump, -1);
  wp26 -> AddPath(wp11, Path_Backflip, 1);
  wp26 -> AddPath(wp7, Path_Backflip, 1);
  wp27 -> AddPath(wp11, Path_MoveTo, 1);
  wp27 -> AddPath(wp6, Path_MoveTo, 1);
  wp27 -> AddPath(wp3, Path_Jump, -1);
  wp28 -> AddPath(wp24, Path_MoveTo, 1);
  wp29 -> AddPath(wp8, Path_Jump, -1);
  wp29 -> AddPath(wp32, Path_MoveTo, 1);
  wp30 -> AddPath(wp28, Path_Jump, 1);
  wp30 -> AddPath(wp31, Path_MoveTo, -1);
  wp31 -> AddPath(wp28, Path_Jump, 1);
  wp31 -> AddPath(wp30, Path_MoveTo, 1);
  wp32 -> AddPath(wp8, Path_Jump, -1);
  wp32 -> AddPath(wp29, Path_MoveTo, -1);
}

global func AddLavaEffects()
{
	AddEffect("MoreLava",0,200,2,0,0, 400,56,1);
	AddEffect("MoreLava",0,200,1,0,0, 400,700,20);
}

global func FxMoreLavaStart(object obj, int nr, tmp, int x, int y, int a)
{
	if(tmp)
		return(1);
	EffectVar(0,obj,nr) = x;
	EffectVar(1,obj,nr) = y;
	EffectVar(2,obj,nr) = a;
}

global func FxMoreLavaTimer(object obj, int nr)
{
	if(EffectVar(4,0,nr))
		return();
	if(EffectVar(3,0,nr))
	{
		EffectVar(3,0,nr)--;
		return();
	}
	CastPXS("DuroLava",EffectVar(2,obj,nr)+Random(2), Random(10)+3, EffectVar(0,obj,nr), EffectVar(1,obj,nr)); 
}

global func FxMoreLavaStop(object obj, int nr, int reason, bool tmp)
{
	if(tmp)
		return(1);
	return(-1);
}

func PlaceFog()
{
	var x,y;
	for(;;)
	{
		x = Random(LandscapeWidth());
		y = Random(LandscapeHeight());
		if(GBackSolid(x,y) || GBackLiquid(x,y))
			continue;
		CreateParticle("Fog",x,y,0,0,RandomX(1000,1500),RGB(255,200+Random(55)));
		break;
	}
}

func Script2()
{
	var w = LandscapeWidth();

	if(GBackLiquid(400,600))
		SuckOn();

	var lava;
	if(!Random(15))
		lava = CreateObject(LAVA, Random(40), 375, -1);
	else if(!Random(15))
		lava = CreateObject(LAVA, w-Random(40), 375, -1);
	if(lava)
	{
		lava->SetSpeed(RandomX(-20,20), -20-Random(40));
		lava->LocalN("mat") = "DuroLava";
	}

	goto(1);
}

func SuckOn()
{
	FindObject(SUCK)->TurnOn();
	
	for(var screen in FindObjects(Find_ID(QWSC)))
		screen->SetAction("Suck");
	
	var e,i;
	while(e = GetEffect("MoreLava",0,i++))
		EffectVar(4,0,e) = true;
}

func SuckOff()
{
	var r = Random(500);
	var e, i;
	while(e = GetEffect("MoreLava",0,i++))
	{
		EffectVar(3,0,e) = r+500;
		EffectVar(4,0,e) = false;
	}
	
	for(var screen in FindObjects(Find_ID(QWSC)))
	{
		screen->SetAction(Format("StandBy%d",Random(2)+1));
		Schedule("SetAction(\"Warning\")", r+500,0,screen);
	}
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
		if(Stuck(obj))
		{
			RemoveObject(obj);
			continue;
		}
		iX = obj->GetX();
		iY = obj->GetY();
		RemoveObject(obj);
		
		if(iX < 192 || iX > 720)
			continue;
		
		if(iY > 595)
			continue;
		
		if(GBackSolid (iX, iY-10))
			continue;
		
		if(FindObject2(Find_Or(Find_ID(QJP1), Find_ID(QJP2), Find_ID(QJP3)), Find_Distance(20,iX,iY)))
			continue;
		
		ok = true;
	}
}
