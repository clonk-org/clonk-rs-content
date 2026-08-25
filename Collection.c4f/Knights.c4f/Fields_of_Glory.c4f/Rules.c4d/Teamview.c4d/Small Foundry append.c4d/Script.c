#strict
#appendto FDRS

protected func Burning()
{
  
  if(FindObject(RTVW)) SetPlrViewRange(10);
  // Effekte
  Smoke(0, 10, 6);
  // Noch nicht fertig
  if (GetActTime() < 150) return(1);
  // Aktion beenden
  SetAction("Idle");
  // Effekt
  CastParticles("PxSpark",6,10,0,10,15,30,RGB(255,223,127),RGB(255,223,127));
  // Erz in Metall umwandeln
  var pOre;
  if (pOre = FindContents(ORE1))
  {
    RemoveObject(pOre);
    Exit(CreateContents(METL), -19,10,0, -1);
  }
  Sound("Pshshsh");
  if(FindObject(RTVW)) SetPlrViewRange(0);
  // Fertig
  // Pr¸fen, ob der Ofen weiterlaufen kann
  return(ContentsCheck());
}

protected func ContentsCheck()
{
  // Unpassende Gegenst‰nde auswerfen
  var i, pContents, idContents;
  while (pContents = Contents(i++))
  {
    idContents = GetID(pContents);
    if (idContents != COAL && idContents != OBRL && idContents != WOOD && idContents != ORE1)
      Exit(pContents, -20,13,0, -1);
  }
  // Ofen l‰uft schon?
  if (!ActIdle()) return(1);
  // Nur starten, wenn Erz drin ist
  if (!FindContents(ORE1)) return(1);
  // Brennmaterial suchen
  var pBurnMat;
  if (ContentsCount(WOOD) >= 2)
  {
    RemoveObject(FindContents(WOOD));
    RemoveObject(FindContents(WOOD));
  }
  else if (pBurnMat = FindContents(COAL))
    RemoveObject(pBurnMat);
  else if (pBurnMat = FindContents(OBRL))
    ChangeDef(BARL,pBurnMat);
  else return(1);
  // Ofen anschmeiﬂen
  SetAction("Burning");
  return(1);
}