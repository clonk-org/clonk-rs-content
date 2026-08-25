/*-- Film --*/

#strict

local iPlr, iPlr2, iPlr3;

local pClonk1, pClonk2, pClonk3, pLorry, pCannon, pTent;

public func StartFilm()
{
  iPlr=-1; iPlr2=-1; iPlr3=-1;
  for(var i=0,p,c; i<GetPlayerCount(); i++)
    if(Global(p=GetPlayerByIndex(i)) == Position_Right)
    {
      if(iPlr==-1) iPlr=p;
      else if(iPlr2==-1) iPlr2=p;
      else if(iPlr3==-1) iPlr3=p;
      // FoW einschalten
      SetFoW(true, p);
    }
  // Kein Spieler? -> Kein Film!
  if(iPlr==-1) return();
  if(iPlr2==-1) iPlr2 = iPlr;
  if(iPlr3==-1) iPlr3 = iPlr2;
  // Clonks erzeugen
  pClonk1=CreateObject(KNIG,AbsX(2120),AbsY(1369),iPlr); pClonk1->SetDir(DIR_Left);
  pClonk2=CreateObject(CLNK,AbsX(2150),AbsY(1368),iPlr2); pClonk2->SetDir(DIR_Left);
  pClonk3=CreateObject(CLNK,AbsX(2184),AbsY(1368),iPlr3); pClonk3->SetDir(DIR_Left);
  // Zu deaktivierten CrewMembern machen
  MakeCrewMember(pClonk1, iPlr); SetCrewEnabled(false, pClonk1); SetPlrViewRange(50, pClonk1); DoEnergy(1000, pClonk1);
  MakeCrewMember(pClonk2, iPlr2); SetCrewEnabled(false, pClonk2); SetPlrViewRange(50, pClonk2); DoEnergy(1000, pClonk2);
  MakeCrewMember(pClonk3, iPlr3); SetCrewEnabled(false, pClonk3); SetPlrViewRange(50, pClonk3); DoEnergy(1000, pClonk3);
  // Lore und Haubitze
  pLorry=CreateObject(LORY,AbsX(2151),AbsY(1367),iPlr);
  pCannon=CreateObject(CANN,AbsX(2183),AbsY(1365),iPlr);
  // Objekte in Lore
  CreateContents(BANP, pLorry);
  for(var i=0; i<11; i++) CreateContents(WOOD, pLorry);
  // Clonks nehmen Fahrzeuge
  SetCommand(pClonk2, "Grab", pLorry);
  SetCommand(pClonk3, "Grab", pCannon);
  // Sicht aller Spieler auf Clonk 1 setzen
  SetPlrViewToClonk1();
  // Nächste Funktion
  ScheduleCall(this(), "Film1", 20);
}

private func SetPlrViewToClonk1()
{
  // Sicht aller Spieler auf Clonk 1 setzen
  for(var i=0,p; i<GetPlayerCount(); i++)
    if(Global(p=GetPlayerByIndex(i)) == Position_Right)
      SetPlrView(p, pClonk1);
}

private func Film1()
{
  // Sicht aller Spieler auf Clonk 1 setzen
  SetPlrViewToClonk1();
  // Nachricht
  FilmOutput("$film$", pClonk1);
  // Nach vorn!
  SetCommand(pClonk1, "Call", this(),,,,"Film2");
  AddCommand(pClonk1, "MoveTo",, 1422,1418);
  SetCommand(pClonk2, "Call", this(),,,,"Film2b");
  AddCommand(pClonk2, "MoveTo",, 1368,1430);
  AddCommand(pClonk2, "PushTo", pLorry, 1481,1418);
  SetCommand(pClonk3, "Call", this(),,,,"Film2c");
  AddCommand(pClonk3, "PushTo", pCannon, 1634,1329);
}

private func Film2()
{
  // Sicht aller Spieler auf Clonk 1 setzen
  SetPlrViewToClonk1();
  CreateContents(TENP, pClonk1)->Activate(pClonk1);
  pTent = FindObjectOwner(TENT, iPlr);
  ScheduleCall(this(), "Film3", 20);
}

private func Film2b()
{
  var pFire = CreateConstruction(CPFR, AbsX(1368), AbsY(1430+8), iPlr, 1, 1, 0);
  SetCommand(pClonk2, "Call", this(),,,, "Film3b");
  AddCommand(pClonk2, "Build", pFire);
}

private func Film2c()
{
  SetCommand(pClonk3, "Call", this(),,,, "Film2d");
  AddCommand(pClonk3, "MoveTo",, 1582, 1378);
  AddCommand(pClonk3, "UnGrab");
  AddCommand(pClonk3, "Get", FindContents(BANP, pLorry));
}

private func Film2d()
{
  // Banner aufbauen
  FindContents(BANP, pClonk3)->Activate(pClonk3);
  // Ins Zelt (oder bei Aufbau helfen)
  ScheduleCall(this(), "Film3c", 20);
}

private func Film3()
{
  // Sicht aller Spieler auf Clonk 1 setzen
  SetPlrViewToClonk1();
  // Kein Zelt? Noch eines bauen.
  if(!pTent)
  { if(FindContents(TENP, pClonk1)) RemoveObject(FindContents(TENP, pClonk1));
    return(Film2()); }
  // Zeltaufbau fertig?
  if(GetAction(pTent) ne "Idle") return(ScheduleCall(this(), "Film3", 15));
  // Ritter betritt Zelt und legt Fahne hinein
  CreateContents(FLAG, pClonk1);
  SetCommand(pClonk1, "Put", pTent);
  // Nächste Funktion
  ScheduleCall(this(), "Film4", 20);
}

private func Film3b()
{
  // Kein Zelt?
  if(!pTent) return(ScheduleCall(this(), "Film3b", 15));
  // Noch nicht fertig?
  if(GetAction(pTent) ne "Idle")
  {
    // Mithelfen
    SetCommand(pClonk2, "Grab", pTent);
    return(ScheduleCall(this(), "Film3b", 15));
  }
  // Betreten
  SetCommand(pClonk2, "Enter", pTent);
}

private func Film3c()
{
  // Kein Zelt?
  if(!pTent) return(ScheduleCall(this(), "Film3c", 15));
  // Noch nicht fertig?
  if(GetAction(pTent) ne "Idle")
  {
    // Mithelfen
    SetCommand(pClonk3, "Grab", pTent);
    return(ScheduleCall(this(), "Film3c", 15));
  }
  // Betreten
  SetCommand(pClonk3, "Enter", pTent);
}

private func Film4()
{
  // Objekte in Lore
  CreateContents(TENP, pLorry);
  for(var i=0; i<5; i++) CreateContents(FLNT, pLorry);
  for(var i=0; i<3; i++) CreateContents(SFLN, pLorry);
  for(var i=0; i<2; i++) CreateContents(EFLN, pLorry);
  for(var i=0; i<7; i++) CreateContents(METL, pLorry);
  for(var i=0; i<2; i++) CreateContents(CNKT, pLorry);
  for(var i=0; i<5; i++) CreateContents(SWOR, pLorry);
  for(var i=0; i<1; i++) CreateContents(AXE1, pLorry);
  for(var i=0; i<5; i++) CreateContents(SHIE, pLorry);
  CreateContents(ARMR, pLorry); CreateContents(BRMR, pLorry);
  CreateContents(_MSC, pLorry);
  // Schießpulver in Zelt
  for(var i=0; i<15; i++) CreateContents(GUNP, pTent);
  // Spieler aktivieren
  for(var i=0,p,c; i<GetPlayerCount(); i++)
    if(Global(p=GetPlayerByIndex(i)) == Position_Right)
    {
      // Baupläne
      SetPlrKnowledge(p, AXE1);
      SetPlrKnowledge(p, BANP);
      SetPlrKnowledge(p, CNKT);
      SetPlrKnowledge(p, BOW1);
      SetPlrKnowledge(p, FBMP);
      SetPlrKnowledge(p, FARP);
      SetPlrKnowledge(p, FLNT);
      SetPlrKnowledge(p, ARWP);
      SetPlrKnowledge(p, GUNP);
      SetPlrKnowledge(p, SHIE);
      SetPlrKnowledge(p, SWOR);
      SetPlrKnowledge(p, SPER);
      SetPlrKnowledge(p, XARP);
      SetPlrKnowledge(p, SFLN);
      SetPlrKnowledge(p, EFLN);
      SetPlrKnowledge(p, STFN);
      SetPlrKnowledge(p, TFLN);
      SetPlrKnowledge(p, DCO3);
      SetPlrKnowledge(p, DCO4);
      SetPlrKnowledge(p, TENP);
      SetPlrKnowledge(p, BALN);
      SetPlrKnowledge(p, BWRC);
      SetPlrKnowledge(p, WODC);
      SetPlrKnowledge(p, LORY);
      SetPlrKnowledge(p, SHRC);
      SetPlrKnowledge(p, SWRC);
      SetPlrKnowledge(p, SPRC);
      SetPlrKnowledge(p, TABL);
      SetPlrKnowledge(p, ARCH);
      SetPlrKnowledge(p, ANVL);
      SetPlrKnowledge(p, CHEM);
      SetPlrKnowledge(p, FRGE);
      SetPlrKnowledge(p, ELEV);
      SetPlrKnowledge(p, HUT2);
      SetPlrKnowledge(p, HUT3);
      SetPlrKnowledge(p, FDRS);
      SetPlrKnowledge(p, CPFR);
      SetPlrKnowledge(p, PALS);
      SetPlrKnowledge(p, PAL2);
      SetPlrKnowledge(p, SAWM);
      SetPlrKnowledge(p, WGTW);
      SetPlrKnowledge(p, WRKS);
      SetPlrKnowledge(p, WTWR);
      SetPlrKnowledge(p, BAS7);
      // Verteidiger können Aufwertung zaubern!
      SetPlrMagic(p, RVLT);
      // Kamera löschen
      if(c=FindObjectOwner(_CAM, p)) RemoveObject(c);
      // Clonks aktivieren
      while(c=FindObjectOwner(0,p,,,,,OCF_CrewMember | OCF_Alive,,,c))
        SetCrewEnabled(true, c);
      // Hat noch keine Crewmitglieder?
      if(!FindObjectOwner(KNIG, p))
      {
        (c=CreateObject(KNIG,,,p))->Enter(pTent);
        MakeCrewMember(c, p);
        DoEnergy(1000, c);
        SetPlrViewRange(50, c);
      }
      if(!FindObjectOwner(CLNK, p))
      {
        (c=CreateObject(CLNK,,,p))->Enter(pTent);
        MakeCrewMember(c, p);
        DoEnergy(1000, c);
        SetPlrViewRange(50, c);
      }
      if(!FindObjectOwner(CLNK, p,,,,,,,,FindObjectOwner(CLNK, p)))
      {
        (c=CreateObject(CLNK,,,p))->Enter(pTent);
        MakeCrewMember(c, p);
        DoEnergy(1000, c);
        SetPlrViewRange(50, c);
      }
      // Einen Ritter aktivieren
      UnselectCrew(p);
      SelectCrew(p, FindObjectOwner(KNIG, p), true);
      // FoW einschalten
      SetFoW(true, p);
    }
  // Filmobjekt wird nicht mehr gebraucht
  Schedule("RemoveObject()", 500, 0, this());
}
