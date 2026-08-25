/*-- Film --*/

#strict

local iPlr, iPlr2;

local pOffice1, pOffice2;
local pMage, pClonk1, pClonk2, pClonk3;

local fClonksCreated;

public func StartFilm()
{
  iPlr=-1; iPlr2=-1;
  for(var i=0,p,c; i<GetPlayerCount(); i++)
    if(Global(p=GetPlayerByIndex(i)) == Position_Castle)
    {
      if(iPlr==-1) iPlr=p;
      else if(iPlr2==-1) iPlr2=p;
      // FoW einschalten
      SetFoW(true, p);
    }
  // Kein Spieler? -> Kein Film!
  if(iPlr==-1) return();
  if(iPlr2==-1) iPlr2=iPlr;
  // Kontor
  pOffice1 = FindObject(CPOF,798,449,-1,-1);
  pOffice2 = FindObject(CPOF,798,630,-1,-1);
//  if(Random(2)) { var o=pOffice1; pOffice1=pOffice2; pOffice2=o; }
  // Burg soll Spieler iPlr gehören!
  var pObj;
  while(pObj = FindObject(0, 550, 350, 550, 650, 0,0,0,0, pObj))
    if(GetOwner(pObj) == -1)
      if(pObj != pKing)
        SetOwner(iPlr, pObj);
  // Schild des Königs soll aber richtig gefärbt sein!
  var pObj = FindObject(SHIE,,,,,,,pKing);
  if(pObj) SetColorDw(GetColorDw(pKing), pObj);
  // Clonks erzeugen
  pMage=CreateObject(MAGE,,,iPlr); pMage->SetDir(DIR_Left); pMage->Enter(FindObject(THRN)); FindObject(THRN)->~UpdateState(); pMage->SetName("Lorthemus");
  pClonk1=CreateObject(CLNK,AbsX(605),AbsY(596),iPlr); pClonk1->SetDir(DIR_Left);
  pClonk2=CreateObject(KNIG,,,iPlr); pClonk2->Enter(FindObject(CPR2));
  pClonk3=CreateObject(KNIG,,,iPlr); pClonk3->Enter(FindObject(CPR1));
  // Zu deaktivierten CrewMembern machen
  MakeCrewMember(pMage, iPlr); SetCrewEnabled(false, pMage); SetPlrViewRange(70, pMage); DoEnergy(1000, pMage);
  MakeCrewMember(pClonk1, iPlr); SetCrewEnabled(false, pClonk1); SetPlrViewRange(50, pClonk1); DoEnergy(1000, pClonk1);
  MakeCrewMember(pClonk2, iPlr); SetCrewEnabled(false, pClonk2); SetPlrViewRange(50, pClonk2); DoEnergy(1000, pClonk2);
  MakeCrewMember(pClonk3, iPlr); SetCrewEnabled(false, pClonk3); SetPlrViewRange(50, pClonk3); DoEnergy(1000, pClonk3);
  // Sicht aller Spieler auf Magus setzen
  SetPlrViewToClonk1();
  // Nächste Funktion
  ScheduleCall(this(), "Film1", 20);
}

private func SetPlrViewToClonk1()
{
  // Sicht aller Spieler auf das Bett setzen
  for(var i=0,p; i<GetPlayerCount(); i++)
    if(Global(p=GetPlayerByIndex(i)) == Position_Castle)
      SetPlrView(p, FindObject(BED1));
}

private func Film1()
{
  // Sicht aller Spieler auf Magus setzen
  SetPlrViewToClonk1();
  // Nachricht
  FilmOutput("$film1$", pMage);
  Sound("ClonkMunch", false, pMage);
  // Der Clonk geht zum Magus
  SetCommand(pClonk1, "Call", this(),,,, "Film2");
  AddCommand(pClonk1, "MoveTo",, 834, 537);
  AddCommand(pClonk1, "Wait",,,,,,15);
}

private func Film2()
{
  // Sicht aller Spieler auf Clonk 1 setzen
  SetPlrViewToClonk1();
  // Gespräch
  var iDur = FilmOutput("$film2$", pClonk1);
  iDur += FilmOutput("$film3$", pMage, iDur);
  iDur += FilmOutput("$film4$", pClonk1, iDur);
  Schedule(Format("SetCommand(Object(%d), \"Exit\")", ObjectNumber(pMage)), iDur);
  iDur += FilmOutput("$film5$", pMage, iDur);
  iDur += FilmOutput("$film6$", pClonk1, iDur);
  Schedule(Format("Sound(\"CommandFailure1\", false, Object(%d))", ObjectNumber(pMage)), iDur);
  iDur += FilmOutput("Hmmm...", pMage, iDur);
  iDur += FilmOutput("$film7$", pMage, iDur+2);
  // Nächste Funktion aufrufen
  ScheduleCall(this(), "Film3", iDur+10);
}

private func Film3()
{
  // Sicht aller Spieler auf Clonk 1 setzen
  SetPlrViewToClonk1();
  // Fahnen im Clonk erzeugen
  if(iPlr!=iPlr2) CreateContents(FLAG, pClonk1)->SetOwner(iPlr2);
  CreateContents(FLAG, pClonk1)->SetOwner(iPlr);
  // Clonk legt Fahne in einen Kontor
  SetCommand(pClonk1, "Call", this(),,,, "Film4");
  if(iPlr!=iPlr2) AddCommand(pClonk1, "Put", pOffice2);
  AddCommand(pClonk1, "Put", pOffice1);
  AddCommand(pClonk1, "Enter",FindObject(CPT2,736,537,-1,-1));
  AddCommand(pClonk1, "MoveTo",, 736, 537);
  // Wenn Clonk zu dumm ist, Funktion trotzdem aufrufen
  ScheduleCall(this(), "Film4", 35*60);
  // Ritter rüstet sich mit schwarzer Rüstung aus
  SetCommand(pClonk2, "Call", this(),,,, "Film4b");
  AddCommand(pClonk2, "Exit");
  AddCommand(pClonk2, "Get", FindContents(BRMR, FindObject(CPR2)));
  // Anderer Ritter holt sich Schild und Schwert
  SetCommand(pClonk3, "MoveTo",, 603, 596);
  AddCommand(pClonk3, "Call", FindObject(CPT2, 618, 637, -1, -1),,,,, "ControlLeft");
  AddCommand(pClonk3, "Grab", FindObject(CPT2, 618, 637, -1, -1));
  AddCommand(pClonk3, "UnGrab");
  AddCommand(pClonk3, "Get", Contents(0, FindObject(SWRC)));
  AddCommand(pClonk3, "Call", this(),,,,, "Film4c");
  AddCommand(pClonk3, "UnGrab");
  AddCommand(pClonk3, "Get", Contents(0, FindObject(SHRC)));
}

private func Film4()
{
  if(fClonksCreated) return();
  fClonksCreated = true;

  // Sonne geht jetzt auf
  FindObject(TIME)->Sunrise();

  // ScriptCounter läuft weiter (Thanathos soll ja irgendwann auftauen)
  ScriptGo(true);

  for(var i=0,p,c; i<GetPlayerCount(); i++)
    if(Global(p=GetPlayerByIndex(i)) == Position_Castle)
    {
      // Baupläne
      SetPlrKnowledge(p, AXE1);
      SetPlrKnowledge(p, BANP);
      SetPlrKnowledge(p, CNKT);
      SetPlrKnowledge(p, BOW1);
      SetPlrKnowledge(p, FBMP);
      SetPlrKnowledge(p, FARP);
      SetPlrKnowledge(p, BRED);
      SetPlrKnowledge(p, DOGH);
      SetPlrKnowledge(p, FLNT);
      SetPlrKnowledge(p, COKI);
      SetPlrKnowledge(p, COOD);
      SetPlrKnowledge(p, ARWP);
      SetPlrKnowledge(p, ARMR);
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
      SetPlrKnowledge(p, XBOW);
      SetPlrKnowledge(p, DPOT);
      SetPlrKnowledge(p, BALN);
      SetPlrKnowledge(p, BED1);
      SetPlrKnowledge(p, BWRC);
      SetPlrKnowledge(p, WODC);
      SetPlrKnowledge(p, CATA);
      SetPlrKnowledge(p, CNDL);
      SetPlrKnowledge(p, LORY);
      SetPlrKnowledge(p, SHRC);
      SetPlrKnowledge(p, SWRC);
      SetPlrKnowledge(p, SPRC);
      SetPlrKnowledge(p, TABL);
      SetPlrKnowledge(p, ARCH);
      SetPlrKnowledge(p, OVEN);
      SetPlrKnowledge(p, FRGE);
      SetPlrKnowledge(p, CPEL);
      SetPlrKnowledge(p, CPOF);
      SetPlrKnowledge(p, CPT1);
      // Burgbaupläne kaufbar
      DoHomebaseMaterial(p, CPPL, +1);
      // Steine können in der Burg nicht gekauft werden!
      DoHomebaseMaterial(p, ROCK, -GetHomebaseMaterial(p, ROCK));
      // Spielerobjekt löschen
      if(c=FindObjectOwner(_CAM, p)) RemoveObject(c);
      // Clonks aktivieren
      while(c=FindObjectOwner(0,p,,,,,OCF_CrewMember | OCF_Alive,,,c))
        SetCrewEnabled(true, c);
      // Hat noch keine Crewmitglieder?
      if(!FindObjectOwner(MAGE, p))
      {
        (c=CreateObject(CLNK,,,p))->Enter(pOffice2);
        MakeCrewMember(c, p);
        DoEnergy(1000, c);
        SetPlrViewRange(50, c);
        (c=CreateObject(KNIG,,,p))->Enter(pOffice2);
        MakeCrewMember(c, p);
        DoEnergy(1000, c);
        SetPlrViewRange(50, c);
        (c=CreateObject(KNIG,,,p))->Enter(pOffice2);
        MakeCrewMember(c, p);
        DoEnergy(1000, c);
        SetPlrViewRange(50, c);
      }
      // Einen Clonk aktivieren
      UnselectCrew(p);
      c=FindObjectOwner(MAGE,p); if(!c) c=FindObjectOwner(KNIG,p);
      SelectCrew(p, c, true);
      // FoW einschalten
      SetFoW(true, p);
    }
  // Filmobjekt wird nicht mehr gebraucht
  Schedule("RemoveObject()", 500, 0, this());
}

private func Film4b()
{
  // Rüstung anziehen
  FindContents(BRMR, pClonk2)->Activate(pClonk2);
  // Dann noch ein Schwert nehmen und Stellung beziehen
  SetCommand(pClonk2, "MoveTo",, 993, 596);
  AddCommand(pClonk2, "Call", FindObject(CPT2, 978,637,-1,-1),,,,, "ControlRight");
  AddCommand(pClonk2, "Grab", FindObject(CPT2, 978,637,-1,-1));
  AddCommand(pClonk2, "UnGrab");
  AddCommand(pClonk2, "Get", Contents(1, FindObject(SWRC)));
}

private func Film4c()
{
  // Ritter legt Schild an
  FindContents(SHIE, pClonk3)->Activate(pClonk3);
}

