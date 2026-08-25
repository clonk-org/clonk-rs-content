/*-- Tageszeiten --*/

#strict

#appendto TIME

private func Initialized()
{
  // Damit das TIME-Objekt nicht gleich gelöscht wird
  Local(1) = 500;
  // Überladene Funktion aufrufen
   _inherited();
  // Geschwindigkeit auf 0!
  Local() = 0;
  // Zu Beginn ist es Nacht!
  SetTime(60);
  // Fertig
  return(1);  
}

private func Timing()
{
  // Nach Sonnenaufgang bleibt es Tag
  if(Local(1) < 100) return(RemoveObject());
  // Fertig
  return(_inherited());
}

public func Sunrise()
{
  Local(0) = 50;
}

