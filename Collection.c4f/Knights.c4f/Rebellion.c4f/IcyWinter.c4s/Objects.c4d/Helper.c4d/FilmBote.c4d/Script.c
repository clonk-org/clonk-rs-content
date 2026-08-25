/*-- Film --*/

#strict

local iPlr, iX, iY, iTX, iTY, fFilm2Called;

local pClonk1;

public func StartFilm()
{
  // Nächste Funktion
  ScheduleCall(this(), "Film1", 8000+Random(5000));
}

private func Film1()
{
  // Zufälligen Angreiferspieler
  iPlr = -1;
  for(var i=0,r; i<50; i++)
    if(Global(GetPlayerByIndex(r=Random(GetPlayerCount()))) != Position_Castle)
      { iPlr=GetPlayerByIndex(r); break; }
  // Noch kein Spieler?
  if(iPlr == -1)
    for(i=0; i<GetPlayerCount(); i++)
      if(Global(GetPlayerByIndex(i)) != Position_Castle)
        { iPlr=GetPlayerByIndex(r); break; }
  // Immer noch kein Spieler? Dann kommt kein Bote.
  if(iPlr == -1) return(RemoveObject());
  // Position
  if(Global(iPlr) == Position_Left)
  { iX = 18; iY = 1139; iTX = 614; iTY = 1216; }
  else
  { iX = 2184; iY = 1368; iTX = 1422; iTY = 1418; }
  // Boten erzeugen
  pClonk1 = CreateObject(KNIG, AbsX(iX), AbsY(iY), iPlr);
  // Inhalt erzeugen
  pClonk1->CreateContents(SWOR);
  pClonk1->CreateContents(WPPL);
  // Zur Basis des Spielers laufen
  SetCommand(pClonk1, "Call", this(),,,, "Film2");
  AddCommand(pClonk1, "MoveTo", 0, iTX, iTY);
  // Falls der Clonk den Weg nicht schafft
  ScheduleCall(this(), "Film2", 1500);
  // Sicht der Angreifer auf den Boten setzen
  for(var i=0,p; i<GetPlayerCount(); i++)
    if(Global(p=GetPlayerByIndex(i)) != Position_Castle)
      SetPlrView(p, pClonk1);  
  // Nachricht
  Log("$log$");
}

private func Film2()
{
  if(fFilm2Called) return();
  fFilm2Called = true;
  // Steuerbar
  SetCrewEnabled(true, pClonk1);
  MakeCrewMember(pClonk1, iPlr);
  // Nachricht
  FilmOutput("$film$", pClonk1);
  // Filmobjekt wird nicht mehr gebraucht
  Schedule("RemoveObject()", 500, 0, this());
}

