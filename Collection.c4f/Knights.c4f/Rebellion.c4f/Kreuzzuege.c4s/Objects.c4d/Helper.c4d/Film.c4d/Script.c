/*-- Film --*/

#strict

local pClonk, iReady, iPhase;

local pCamera;

public func StartFilm()
{
  // Kein Spielerbeitritt während dem Film!
  SetMaxPlayer(0);
  // Clonks deaktivieren
  for(var i=0; i<GetPlayerCount(); i++)
    for(var j=0; j<GetCrewCount(GetPlayerByIndex(i)); j++)
      SetCrewEnabled(false, GetCrew(GetPlayerByIndex(i), j));
  // Ersten Clonk von Spieler 1 wieder aktivieren und Menü erzeugen
  pCamera = CreateObject(_CAM,,,0);
  MakeCrewMember(pCamera, 0);
  pCamera->SetPosition(2950, 300);
  pCamera->SetCrewEnabled(true);
  SelectCrew(0, pCamera, true);
  CreateMenu(_F00, pCamera, this(), 0, "$Intro$", 0, 1);
  AddMenuItem("$Abort$", "AbortFilm", _F00, pCamera, 0, 0, "$AbortDesc$", 2, 2);
  AddMenuItem("$CloseMenu$", "CloseAbortMenu", _F00, pCamera, 0, 0, "$CloseMenuDesc$", 2, 1);
  // Film-Phase
  iPhase = 0;
  // Blitzlicht-Effekt
  CreateFlashlight(2);
  // Clonks erzeugen
  pClonk = CreateArray(3);
  pClonk[0]=CreateObject(CLNK,AbsX(2280+Random(50)),AbsY(438),-1);
  pClonk[1]=CreateObject(CLNK,AbsX(2280+Random(50)),AbsY(438),-1);
  pClonk[2]=CreateObject(CLNK,AbsX(2280+Random(50)),AbsY(438),-1);
  // Clonks färben
  for(var i=0; i<3; i++)
    CLFS->Colorize(pClonk[i]);
  // Sicht aller Spieler auf den Turm setzen
  SetPlrViewToTower();
  // Nächste Funktion
  ScheduleCall(this(), "Film1", 20);
}

public func AbortFilm()
{
  var pObj;

  // Goldstatue noch da?
  if(pObj=FindObject(GIDL)) 
  {
    // Entfernen
    RemoveObject(pObj);
    // Clonks bekommen Splitter
    for(var clonk in pClonk)
    {
      for(pObj in FindObjects(Find_Container(clonk)))
        RemoveObject(pObj);
      CreateContents(GDLS, clonk);
    }      
  }
  // Commands aller Clonks löschen
  for(pObj in FindObjects(Find_Func("IsClonk")))
  {
    SetCommand(pObj, "None");
    SetComDir(COMD_Stop, pObj);
  }
  // Blitzlicht-Effekt
  RemoveFlashlight();

  // Sonderbehandlung für einzelne Phasen
  if(iPhase == 2)
  {
    // Alte Splitter entfernen
    for(pObj in FindObjects(Find_ID(GDLS)))
      RemoveObject(pObj);
    // Clonks bekommen Splitter
    for(var clonk in pClonk)
    {
      for(pObj in FindObjects(Find_Container(clonk)))
        RemoveObject(pObj);
      CreateContents(GDLS, clonk);
    }      
  }

  // Film ist aus
  FilmEnd();
}

protected func CloseAbortMenu()
{
  SetCrewEnabled(false, pCamera);
  CloseMenu(pCamera);
  return(1);
}

protected func MenuQueryCancel(int iSelection, object pMenuObj)
{
  return(true);
}

private func SetPlrViewToTower()
{
  // Sicht aller Spieler auf den Turm setzen
  SetPlrViewRange(300, FindObject(TWR2));
  for(var i=0; i<GetPlayerCount(); i++)
  {
    SetPlrView(GetPlayerByIndex(i), FindObject(TWR2));
    SetFoW(true, GetPlayerByIndex(i));
  }
}

private func Film1()
{
  // Film-Phase
  iPhase = 1;
  // Sicht aller Spieler auf Turm setzen
  SetPlrViewToTower();
  // Clonks werfen Feuersteine auf Statue
  iReady = 0;
  for(var i=0; i<3; i++)
  {
    pClonk[i]->CreateContents(FLNT);
    SetCommand(pClonk[i], "Call", this(),,,, "Film2");
    AddCommand(pClonk[i], "Wait",,,,,, 18);
    AddCommand(pClonk[i], "Throw",, 2860+Random(21), 353,,,,5);
  }
}

private func Film2()
{
  iReady++;
  if(iReady<3) return();
  // Film-Phase
  iPhase = 2;
  // Sicht aller Spieler auf Turm setzen
  SetPlrViewToTower();
  // Statue zerspringt
  CastObjects(GDLS, 3, 10+Random(10), GetX(FindObject(GIDL)), GetY(FindObject(GIDL)));
  RemoveObject(FindObject(GIDL));
  // Jeder Clonk sammelt einen Splitter ein
  var pSplinter; iReady = 0;
  for(var i=0; i<3; i++)
  {
    pSplinter = FindObject(GDLS,,,,,,,,,pSplinter);
    pClonk[i]->CreateContents(TIM1);
    SetCommand(pClonk[i], "Call", this(),,,, "Film3");
    AddCommand(pClonk[i], "MoveTo",, AbsX(2280), AbsY(438),,,,5);
    AddCommand(pClonk[i], "Get", pSplinter,,,,,, 5);
  }
}

private func Film3()
{
  iReady++;
  if(iReady<3) return();
  // Film-Phase
  iPhase = 3;
  // Blitzlicht-Effekt
  RemoveFlashlight();
  // Spielerclonks laufen dorthin, wo die Statue war
  iReady = 0;
  for(var i=0,c,p; i<GetPlayerCount(); i++)
  {
    p = GetPlayerByIndex(i);
    c = FindObject2(Find_Owner(p), Find_Func("IsClonk"));
    SetPlrView(p, c);
    SetCommand(c, "Call", this(),,,, "Film4");
    AddCommand(c, "MoveTo",, 2840+Random(40), 350,,,, 5);
    AddCommand(c, "Wait",,,,,, 20+Random(20));
  }
}

private func Film4()
{
  iReady++;
  if(iReady<GetPlayerCount()) return();
  // Film-Phase
  iPhase = 4;
  // Gamma-Bugfix (hoffentlich reicht das...)
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 0);
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 2);
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 4);
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 5);
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 6);
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 7);
  // Nachricht
  Message("<c %x>$Statue$", GetCrew(GetPlayerByIndex(0)), GetPlrColorDw(GetPlayerByIndex(0)));
  // Nächste Funktion
  ScheduleCall(this(), "Film5", 70);
}

private func Film5()
{
  // Film-Phase
  iPhase = 5;
  // Nachricht
  Message("<c %x>$Statue2$", GetCrew(GetPlayerByIndex(0)), GetPlrColorDw(GetPlayerByIndex(0)));
  // Film ist zu Ende
  FilmEnd();
}

private func FilmEnd()
{
  // Menü schließen und Kameraobjekt löschen
  iPhase = -1;
  CloseMenu(pCamera);
  if(pCamera) RemoveObject(pCamera);
  // Gamma-Bugfix (besser nochmal)
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 0);
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 2);
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 4);
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 5);
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 6);
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 7);
  // Diebesclonks an ihre Endpositionen setzen (dort, wo sie ihren Splitter verteidigen)
  var aClonkPos = [[3090, 707],
                   [568, 788],
                   [348, 360],
                   [51, 290],
                   [927, 438],
                   [1463, 295],
                   [2137, 390]], p;
  for(var i=0; i<3; i++)
  {
    while(!aClonkPos[p=Random(GetLength(aClonkPos))]); // Leerlaufschleife beabsichtigt!
    pClonk[i]->SetPosition(aClonkPos[p][0], aClonkPos[p][1]);
    aClonkPos[p] = 0;
    pClonk[i]->SetAI(0, 50);
  }
  // Spielerclonks wieder aktivieren
  for(var i=0,p; i<GetPlayerCount(); i++)
  {
    p = GetPlayerByIndex(i);
    for(var j=0; j<GetCrewCount(p); j++)
      SetCrewEnabled(true, GetCrew(p, j));
    SelectCrew(p, GetCrew(p), true);
  }
  // Spielerbeitritt wieder erlauben
  SetMaxPlayer(6);
  // ScriptCounter läuft weiter
  ScriptGo(1);
  // Filmobjekt entfernen
  RemoveObject();
}

