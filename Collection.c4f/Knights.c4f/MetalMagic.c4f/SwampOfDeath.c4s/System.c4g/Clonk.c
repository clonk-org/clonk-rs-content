/*-- Bauen mit Maus --*/

#strict 2
#appendto CLNK

// no construction!
func HasConstructMenu(){ return false; }

protected func MenuQueryCancel()
{
 // Die Relaunchmessage verschwindet nach dem Schließen des Relaunchmenüs
 if (GetMenu() == WIPF) return(1);
}

protected func ControlCommand(szCommand, pTarget, iTx, iTy, pTarget2, Data) 
{ 
  // Kommando Construct in Bauen mit Geld umwandeln. 
  if (szCommand == "Construct") 
  { 
    AddCommand(this, "Call", FindContents(CNKT), Data, ObjectNumber(this), 0, 0, "MouseCreateConstructionSite");
    AddCommand(this, "MoveTo", 0, iTx, iTy); 
    return 1; 
  }
  // Sonst nicht abfangen 
  return _inherited(szCommand, pTarget, iTx, iTy, pTarget2, Data) ; 
} 
