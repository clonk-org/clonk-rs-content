/*-- Respawnen & Co --*/

#strict 2

#appendto PLDN

protected func MenuQueryCancel()
{
  // Waffenauswahl nicht abbrechen
  if (GetMenu() == WIPF) return 1;
}

protected func Death(int iCausePlr)
{
  Sound("Die"); 
  if (Not(GetCrew(GetOwner())))
    GameCall("RelaunchPlayer",GetOwner(), iCausePlr);
  DeathAnnounce();

  ++g_No_Relaunch_Removal;
  _inherited(iCausePlr, ...);
  --g_No_Relaunch_Removal;
}
