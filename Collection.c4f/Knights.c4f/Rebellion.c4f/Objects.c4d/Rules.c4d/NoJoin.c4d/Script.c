/*-- Spielerbeitrittsverbot --*/

/* Info: Zeitspanne kann szenarienspezifisch durch eine Funktion
   public func NJ6C_GetTimeSpan() { return( ... ); }
   im Szenarienscript in Frames festgelegt werden.
*/                    

#strict

// Standard: ~20 Sekunden Zeit für Spielerbeitritte
static const NJ6C_TimeSpan = 740;  // 20 * 37

public func Initialize()
{
  // Zeitspanne durch Szenario vorgegeben?
  var iTimeSpan = GameCall("NJ6C_GetTimeSpan");
  // Wenn nicht, Standardwert verwenden
  if(iTimeSpan <= 0) iTimeSpan = NJ6C_TimeSpan;
  // Effekt starten
  AddEffect("IntNoPlayerJoin", this(), 1, iTimeSpan, this());
  // Logmeldung ausgeben (nur wenn Zeitspanne >= ~3 Sekunden)
  if(iTimeSpan >= 3*37) Log("$LogTime$");
}

protected func FxIntNoPlayerJoinTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  // Maximale Spieleranzahl auf 0 setzen
  SetMaxPlayer(0);
  // Logmeldung ausgeben
  Log("$LogNoJoin$");
  // Effekt löschen
  return(-1);
}

protected func Activate(iByPlr)
{
  if(GetEffect("IntNoPlayerJoin", this()))
    MessageWindow("$DescTime$", iByPlr);
  else
    MessageWindow("$DescNoJoin$", iByPlr);
  return(1);
}
