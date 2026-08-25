/*-- Die Welt endet mit Dir --*/

#strict

func Initialize()
{
  Message("Ich sollte mich beeilen mit der Rakete fertig zu werden. | Sonst werde ich hier auch noch verrecken.");
  //Zähler starten!
  // Spielziele setzen
  RemoveAll(CROB);
  var goal = CreateObject(CROB, 50,50,-1);
  if (goal)
  {
    goal->AddType(RKT_, 1);
  }
  ScriptGo(1);
  return;
}

protected func InitializePlayer(int iPlr)
{
  CreateContents(FLAG, GetHiRank(iPlr));
  SetPosition(7, 400, GetCrew(iPlr));
}

func Script2()
{
  // Truhen füllen
  for(var chest in FindObjects(Find_ID(_CST)))
  {
    while(Contents(0,chest))RemoveObject(Contents(0,chest));
    FillChest(chest);
  }
  return(1);
}

protected func FillChest(object pChest)
{
  var count=6+Random(6);
  while(count--)
  CreateContents(RandChestID(),pChest);
}

protected func RandChestID()
{
  var rand=Random(14);
  if(!--rand)return(TFLN);
  if(!--rand)return(SFLN);
  if(!--rand)return(FLNT);
  if(!--rand)return(WOOD);
  if(!--rand)return(ROCK);
  if(!--rand)return(METL);
  if(!--rand)return(SPHR);
  if(!--rand)return(ORE1);
  if(!--rand)return(GOLD);
  if(!--rand)return(CRYS);
  if(!--rand)return(COAL);
  if(!--rand)return(CNKT);
  if(!--rand)return(LNKT);
  return();
}

// Vulkan soll DuroLava verwenden
public func VolcanoMaterial() { return(Material("DuroLava")); }