#strict

global func GetObjects(id ID)
{
  var obj, exp = Find_And(Find_Not(Find_Func("IsHUD")), Find_Not(Find_Func("IsLight")),Find_Not(Find_Category(C4D_Goal)),Find_Not(Find_Category(C4D_Rule)),Find_Not(Find_Category(64)));
  if(ID)
    exp = Find_And(Find_ID(ID),Find_Not(Find_Func("IsLight")),Find_Not(Find_Func("IsHUD")));
  var extra, extray;
  for(obj in FindObjects(exp))
  {
    extra = "";
    extray = 0;
    if(obj->GetR() != 0) extra = Format("->SetR(%d)", obj->GetR());
    if(obj->GetCon() != 100)
    {
      extra = Format("->SetCon(%d)", obj->GetCon());
      extray = -GetDefCoreVal("Offset", "DefCore", GetID(obj), 1) * (100-obj->GetCon()) / 100;
    }
    if(obj->GetID() == LADR || obj->GetID() == CRN1)
    {
      extra = Format("->Set(%d)", obj->LocalN("length"));
      if(obj->GetID() == LADR)
        extray = - (obj->LocalN("length")-1) * obj->LocalN("sizeY");
    }
    if(obj->GetID() == JMPD)
    {
      extra = Format("->Set(%d, %d)", obj->LocalN("top")->LocalN("strength"), obj->LocalN("top")->GetR());
      if(obj->GetR() != 0)
        extra = Format("->Set(%d, %d, %d)", obj->LocalN("top")->LocalN("strength"), obj->LocalN("top")->GetR(), obj->GetR());
    }
    if(obj->GetID() == SPNP)
    {
      if(!(obj->Contents())) continue;
      Log ("  PlaceSpawnpoint(%i, %d, %d);",
         Contents(0, obj)->GetID(),
         GetX(obj),
         GetY(obj));
      continue;
    }
    if(obj->~LastIndex()) extra = Format("->SetRail(%s)", iArray2String(obj->LocalN("parttype")));
    Log ("  CreateObject (%i, %d, %d, %d)%s;",
       GetID (obj),
       GetX(obj),
       GetY (obj) - GetDefCoreVal("Offset", "DefCore", GetID(obj), 1) - extray,
       GetOwner (obj),
       extra);
  }
}

global func iArray2String(aArray)
{
  var str = "[", i;
  for(var a in aArray)
  {
    str = Format("%s%d", str, a);
    i++;
    if(i < GetLength(aArray)) str = Format("%s,", str);
  }
  str = Format("%s]", str);
  return(str);
}


func Initialize()
{
	// Dekozeugs
	CreateObject (CRAT, 688, 182, -1);
	CreateObject (DEB1, 875, 182, -1)->SetDir(DIR_Right);
  CreateObject (CON1, 610, 151, -1)->SetPerspective(1);
  CreateObject (CON1, 591, 180, -1)->SetPerspective(3);
  CreateObject (CON1, 630, 180, -1)->SetPerspective(0);

	for(var obj in FindObjects(Find_ID(CON1)))
		obj->SetColorDw(RGB(220,220,170));

  CreateObject (PIPL, 586, 260, -1)->Left(2)->Down(6);
	CreateObject (PIPL, 549, 485, -1)->Left(3)->Down(2);

	for(var obj in FindObjects(Find_ID(PIPL)))
		obj->SetClrModulation();

  CreateObject (FENC, 1355, 242, -1);
  CreateObject (FENC, 1295, 242, -1);
  CreateObject (FENC, 1235, 242, -1);
  CreateObject (FENC, 1175, 242, -1);
  CreateObject (FENC, 1115, 242, -1);
  CreateObject (FENC, 1055, 242, -1);
  CreateObject (FENC, 995, 242, -1);
  CreateObject (FENC, 935, 242, -1);
  CreateObject (FENC, 871, 181, -1);
  CreateObject (FENC, 691, 181, -1);
  CreateObject (FENC, 631, 181, -1);
  CreateObject (FENC, 571, 181, -1);
  CreateObject (FENC, 506, 549, -1);
  CreateObject (FENC, 446, 549, -1);
  CreateObject (FENC, 386, 549, -1);
  CreateObject (FENC, 326, 549, -1);
  CreateObject (FENC, 266, 549, -1);
  CreateObject (FENC, 26, 549, -1);

	for(var obj in FindObjects(Find_Owner(-1), Find_Not(Find_Category(C4D_StaticBack))))
	{
		// alles in den Hintergrund
		obj->SetCategory(C4D_StaticBack | C4D_Background);
		// keine Solidmask
		obj->SetSolidMask();
		// anderer Layer!
		obj->SetObjectLayer(obj);
	}

	CreateObject(EGR2); //grass


  CreateObject (RADR, 661, 180, -1);
  CreateObject (DEB1, 654, 331, -1);
  CreateObject (VENT, 844, 560, -1)->SetCon(50);
 
  CreateObject (HNG2, 657, 551, -1)->Opened();
  CreateObject (HNG2, 228, 550, -1)->Opened();
  CreateObject (HNG2, 854, 180, -1)->Opened();
  CreateObject (HNG2, 717, 179, -1)->Opened();
  CreateObject (HNG2, 1135, 240, -1)->Opened();
  CreateObject (HNG2, 1296, 240, -1)->Opened();
  CreateObject (HNG2, 932, 281, -1)->Opened();
  CreateObject (HNG2, 64, 550, -1)->Opened();
  CreateObject (HNG2, 76, 480, -1)->Opened();
  CreateObject (HNG2, 217, 480, -1)->Opened();


  CreateObject (ALGH, 1297, 149, -1);
  CreateObject (ALGH, 1136, 151, -1);
  CreateObject (ALGH, 635, 270, -1)->SetR(90);
  CreateObject (ALGH, 734, 179, -1);
  CreateObject (ALGH, 765, 179, -1);
  CreateObject (ALGH, 672, 491, -1)->SetR(90);
  CreateObject (ALGH, 56, 483, -1)->SetR(-90);
  CreateObject (ALGH, 235, 486, -1)->SetR(90);



 CreateObject (QMED, 1026, 238, -1);
  CreateObject (QMED, 883, 619, -1);
  CreateObject (QMED, 388, 553, -1);  //medis

  PlaceBonusSpawnpoint(371,345);  //bonus!

  CreateObject(QQSC,949,546);
  CreateObject(QQSC,753,266);  //screens


  CreateObject (QJP3, 838, 620, -1)->Set(95,-53);
  CreateObject (QJP3, 924, 330, -1)->Set(-80,-60);
  CreateObject (QJP3, 1284, 160, -1)->Set(-60,-35);
  CreateObject (QJP2, 497, 190, -1)->Set(-29,5);
  CreateObject (QJP3, 735, 130, -1)->Set(80,-50);
  CreateObject (QJP2, 752, 327, -1)->Set(0,-90);
  CreateObject (QJP1, 150, 550, -1)->Set(0,-80);
  CreateObject (QJP2, 37, 384, -1)->Set(64,-140);
  CreateObject (QJP2, 518, 485, -1)->Set(-50,-75);  //jumpa

	  CreateObject (QWRP, 610, 259, -1)->Set(1119,525);
  var 	w=CreateObject (QWRP, 1156, 559, -1);
  	w->SetR(-90);
 	w->Set(655,270);  		//warp


  PlaceSpawnpoint(QMGA, 198, 536);
  PlaceSpawnpoint(QMGA, 1213, 229);
  PlaceSpawnpoint(QMGA, 954, 228);
  PlaceSpawnpoint(QMGA, 957, 498);
  PlaceSpawnpoint(Q_RG, 812, 120);
  PlaceSpawnpoint(Q_GL, 113, 468);
  PlaceSpawnpoint(QPGA, 540, 169);
  PlaceSpawnpoint(QRLA, 1263, 149);
  PlaceSpawnpoint(QGLA, 182, 468);
  PlaceSpawnpoint(Q_RL, 1173, 148);
  PlaceSpawnpoint(Q_SQ, 751, 204);
  PlaceSpawnpoint(Q_SG, 823, 318);
  PlaceSpawnpoint(Q_PG, 518, 169);
  PlaceSpawnpoint(QSGA, 800, 318);
  PlaceSpawnpoint(QRGA, 759, 119);
  PlaceSpawnpoint(QRGA, 926, 606);
  PlaceSpawnpoint(Q_RG, 983, 606);
  PlaceSpawnpoint(QSGA, 539, 537);
  PlaceSpawnpoint(Q_SG, 566, 537);
  PlaceSpawnpoint(Q_RL, 182, 409);
  PlaceSpawnpoint(QRLA, 108, 407);  //waffern



 
  CreateWaypoints();


  ScriptGo(true); 
  
    
  return(1);
}

func CreateWaypoints()
{
 
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
    var r = Random(9);
    if(!r) { iX = RandomX(570,900); iY = 162;}  //Hügel oben
    if(!r--) { iX = RandomX(100,1000); iY = 539;} //Gang, alles unten
    if(!r--) { iX = RandomX(100,1000); iY = 539;} //Gang, alles unten
    if(!r--) { iX = RandomX(100,1000); iY = 539;} //Gang, alles unten
    if(!r--) { iX = RandomX(100,1000); iY = 539;} //Gang, alles unten
    if(!r--) { iX = RandomX(100,1000); iY = 539;} //Gang, alles unten
    if(!r--) { iX = 1211; iY = 100;} //RL Platform
    if(!r--) { iX = 150; iY = 350;} //RL Plattform 2
    if(!r--) { iX = 150; iY = 520;}  //GetGL
    if(!r--) { iX = 800; iY = 300;}  //Shrot

}

func Script400()
{
	//Das bisschen Random lohnt nicht.
	FrayAttack();
	goto(1);
}

func FrayAttack() { AddEffect("FrayAttack",0,1,36); }

global func FxFrayAttackStart(object pTarget, int iEffectNumber, int iTemp)
{
	if (iTemp) return FX_OK;
	for ( var pLight in FindObjects(Find_ID(ALGH)))
	{
	pLight->TurnOn(); 
 	}
}

global func FxFrayAttackTimer (object pTarget, int iEffectNumber, int iEffectTime)
{
	//Warnungen
	var aMessages=$FrayAttackWarnings$;
	if(iEffectTime<36*48) Message(aMessages[Min(iEffectTime/36,11)-1]); //Ab der 10. Sec immer das Gleiche
	//Türen schließen
	if(iEffectTime==36*10)
	{
		for ( var pDoor in FindObjects(Find_ID(HNG2)))
		{
		 pDoor->Close();
		}
	}
	//Salven starten
	if(iEffectTime==36*12) FrayAttackSalve(0,  1, 160);
	if(iEffectTime==36*21) FrayAttackSalve(1, -1, 200);
	if(iEffectTime==36*30) FrayAttackSalve(0,  1, 160);
	if(iEffectTime==36*41) FrayAttackSalve(1, -1, 200);
	//Beenden
	if(iEffectTime==36*49) return -1;
}

global func FxFrayAttackStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
	if(iReason) return;
	for ( var pDoor in FindObjects(Find_ID(HNG2)))
	{
		pDoor->Open();
	}
	for ( var pLight in FindObjects(Find_ID(ALGH)))
	{
		pLight->TurnOff(); 
 	}
}

global func FrayAttackSalve(int dir, int start, int angle)
{
	return AddEffect("FraySalve",0,1,6,0,0,dir,start,angle);
}

global func FxFraySalveStart(object pTarget, int iEffectNumber, int iTemp, int dir, int start, int angle)
{
	if (iTemp) return FX_OK;
	//Winkel
	EffectVar(0, pTarget, iEffectNumber)=angle;
	//Schrittgröße
	EffectVar(1, pTarget, iEffectNumber)=start*LandscapeWidth()/40;
	//Momentane Position
	EffectVar(2, pTarget, iEffectNumber)=LandscapeWidth()*dir;
}

global func FxFraySalveTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	//Salve vorbei?
	if(iEffectTime>6*41) return -1;
	var pRocket=CreateObject(QRDM,EffectVar(2, pTarget, iEffectNumber),-30,NO_OWNER);
	pRocket->SetController(-1);
	pRocket->Launch(EffectVar(0, pTarget, iEffectNumber)+RandomX(-2,+2),40,50,300,0);
	//Ein Schritt weiter gehen.
	EffectVar(2, pTarget, iEffectNumber)+=EffectVar(1, pTarget, iEffectNumber);
}
