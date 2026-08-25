#strict

protected func Initialize()
{
  // Parallax
  SetSkyParallax(0, 20, 10, 0, 0, 0,0);
  // Besonderheiten
  CreateObject(_Z9A,240,2000,-1)->Set(1);
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

public func LocalPlaceName()
{
  var para=Par(0);
  if(para==1) return("Wüste"); 
  return("Komisches Unbekanntes Ding");
}
