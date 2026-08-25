/*-- Sandwüste --*/

#strict

func Initialize()
{
  Message("Hier ist die Sandwüste also. |Das schöne Land von dem alle sprechen. |Los fangen wir an die Telegraphenmasten zu errichten.");
  //Zähler starten!
  ScriptGo(1);
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
  if(!--rand)return(FETH);
  if(!--rand)return(BMET);
  if(!--rand)return(FSHM);
  if(!--rand)return(RMET);
  if(!--rand)return(SMET);
  if(!--rand)return(WHKY);
  if(!--rand)return(LTHR);
  if(!--rand)return(ROPE);
  if(!--rand)return(WSKI);
  if(!--rand)return(BOWL);
  if(!--rand)return(AMBO);
  if(!--rand)return(ARPI);
  if(!--rand)return(WINC);
  return(COKI);
}