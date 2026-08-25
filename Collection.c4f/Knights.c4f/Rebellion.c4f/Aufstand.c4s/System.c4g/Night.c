/*-- Ewige Nacht --*/

#strict

global func InitNight()
{
  // Wenn Zeitobjekt gefunden...
  if(FindObject(TIME))
  {
    // Tageszeit auf 50 setzen (Mitternacht)
    FindObject(TIME)->SetTime(50);
    // Alle TIME-Objekte löschen
    RemoveAll(TIME);
  }
  // Gammakorrektur auf Rampe 3
  SetGamma(0, RGB(78, 78, 128), 16777215, 3);
  // Himmel -> schwarz
  SetSkyAdjust(0, GetSkyAdjust(1));
}

/* Status */

global func IsDay()
{
  return(0);
}
  
global func IsNight()
{
  return(1);
}

