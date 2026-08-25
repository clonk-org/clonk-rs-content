/*-- Kältesteuerung im Clonk --*/

#strict

#appendto CLNK

local iWarmth, pColdWarning;

private func MaximumWarmth() { return(2500); }

protected func Initialize()
{
  _inherited();
  iWarmth = MaximumWarmth();
  AddEffect("IntCheckWarmth", this(), 1, 38, this());
}

protected func FxIntCheckWarmthTimer(object pTarget, int iEffectIndex, int iEffectTime)
{
  // Schon tot?
  if(!GetAlive()) return(-1);

  var iWarmthChange, obj;
  // Wärmeverlust/Aufwärmung je nach Temperatur
  iWarmthChange = -50; //BoundBy(GetTemperature()-10, -70, +20);
  // Im Tunnel ist es nicht so kalt
  if(GetMaterial(GetX(), GetY()) == Material("Tunnel")) iWarmthChange = iWarmthChange * 4/5;
  // Wurde eisiger Wind gezaubert?
  if(GetEffect("IcyWindCurse", 0)) iWarmthChange *= 4;
  if(!Contained())
  {
    // Aufwärmung durch Lagerfeuer
    while(obj=FindObject(CPFR,,,,,,"Burn",,,obj))
      iWarmthChange += Max(80-80*ObjectDistance(obj)/180, 0);
    // Aufwärmung durch Fackel
    while(obj=FindObject(TORC,,,,,,"Burning",,,obj))
      iWarmthChange += Max(40-40*ObjectDistance(obj)/80, 0);
    // Aufwärmung durch Esse
    while(obj=FindObject(FRGE,,,,,,,,,obj))
      if(GetAction(obj) eq "Burning" || GetAction(obj) eq "Melting")
        iWarmthChange += Max(40-40*ObjectDistance(obj)/40, 0);
    // Aufwärmung durch Kerze
    while(obj=FindObject(CNDL,,,,,,"On",,,obj))
      iWarmthChange += Max(10-10*ObjectDistance(obj)/10, 0);
    // Eisblock kühlt
    while(obj=FindObject(ICBL,,,,,,"Freeze",,,obj))
      iWarmthChange -= Max(10-10*ObjectDistance(obj)/10, 0);
    // Umliegende, brennende Objekte wärmen auch
    while(obj=FindObject(,-50,-50,100,100,OCF_OnFire,,,,obj))
      iWarmthChange += Max(20-20*ObjectDistance(obj)/30, 0);
  }
  // Aufwärmung vor Burgteilen (Palisade ist Burgteil!)
  if(!Contained())
    if(FindCastlePart(1,1))
      iWarmthChange += (49+Random(4));
  // Aufwärmung in Gebäuden
  if(Contained())
    if(Contained()->~GetWarmth())
      iWarmthChange=Contained()->~GetWarmth();
    else
      iWarmthChange=50;
  // Aufwärmung durch Schmiedeeisen
  if(FindContents(ANML))
    iWarmthChange+=60;
  // Brennt? Sehr stark aufwärmen!
  if(OnFire())
    iWarmthChange+=200;
  // Brennende Objekte im Inventar des Clonks wärmen auch
  while(obj=FindObject(,,,,,OCF_OnFire,,,this(),obj))
    iWarmthChange+=25;
  // Eis oder Schnee in der Hand? Brrrr...
  iWarmthChange -= 10*ContentsCount(ICE1);
  iWarmthChange -= 6*ContentsCount(SNWB);

  // Wärmewert verändern
  iWarmth=BoundBy(iWarmth+iWarmthChange, 0, MaximumWarmth());

  // Energie verändern
  if(!iWarmth) 
  {
    DoEnergy(-1);
    if(GetEnergy() < 20)
      if(!Random(10))
        Sound("Scream");
  }

  // Warnung anzeigen
  SetColdWarning(255*iWarmth/MaximumWarmth());

  // Informative Nachricht für den Test
//  Message("%d / %d / c=%d", this(), iWarmth, MaximumWarmth(), iWarmthChange);
}

protected func FxIntCheckWarmthStop()
{
  SetColdWarning(255); iWarmth = MaximumWarmth();
  if(pColdWarning) RemoveObject(pColdWarning);
}

private func SetColdWarning(int iAlpha)
{
  if(!pColdWarning) ObjectSetAction(pColdWarning=CreateObject(WNCL), "WarnCold", this());
  pColdWarning->SetClrModulation(RGBa(255,255,255,iAlpha));
}

