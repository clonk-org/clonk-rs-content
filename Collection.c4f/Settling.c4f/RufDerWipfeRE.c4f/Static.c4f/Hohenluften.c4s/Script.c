#strict

protected func Initialize()
{
  // Parallax
  SetSkyParallax(0, 20, 10, 0, 0, 0,0);
  // Besonderheiten
  CreateObject(_Z9A,120,1540,-1)->Set(1);
  CreateObject(_Z9A,800,2230,-1)->Set(2);
  CreateObject(_Z9A,3460,1960,-1)->Set(3);
  Windstrom();
  return(1);
}

func Windstrom()
{
  Log("Winddaten werden eingelesen");
  WINDAUFLOESUNG=WindAufloesung();
  globalwestwinde=DoWhatIWWant();
  globalostwinde=DoWhatIWEant();
  horizont=DoWhatIWHant();
  return(1);
}

func LocalPlaceName(para)
{
  if(para==1) return("Donnerklippe"); 
  if(para==2) return("Schlucht");
  if(para==3) return("Einöde");
  return("Komisches Unbekanntes Ding");
}
