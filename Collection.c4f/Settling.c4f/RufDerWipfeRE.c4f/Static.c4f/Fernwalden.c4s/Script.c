#strict

static globalwestwinde,globalostwinde;

protected func Initialize()
{
  // Parallax
  SetSkyParallax(0, 20, 10, 0, 0, 0,0);
  // Besonderheiten
  CreateObject(_Z9A,2050,1680,-1)->Set(1);
  CreateObject(_Z9A,3690,1920,-1)->Set(2);
  
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

func LocalPlaceName()
{
  var para=Par(0);
  if(para==1) return("Zentralgehügele"); 
  if(para==2) return("Küste");
  return("Komisches Unbekanntes Ding");
}
