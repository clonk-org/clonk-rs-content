/*-- Umweltzerstörung --*/

#strict
#include GOAL

public IsFulfilled:
  // Umwelt kaputt?
  if(Fehl()>0) return(0); 
  // fertig?
  return(1);

private Fehl:
  return(50-ProzentVon(globalnature,globalpollution));
  

protected Activate: // int iPlayer
  if (IsFulfilled()) return(MessageWindow("$MsgDamaged$",Par()));
  return(MessageWindow(Format("$MsgMissing$",Fehl()),Par(0)));