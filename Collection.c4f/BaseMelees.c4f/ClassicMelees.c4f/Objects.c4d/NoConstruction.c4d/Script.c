/*-- Keine Baustellen --*/

#strict

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(),iByPlayer);
  return(1);
}

global func FxFireStart(object obj, int iNumber, int iTemp, int iCausedBy, bool fBlasted, object pIncineratingObject)
  {
  // Wenn die Regel aktiviert ist und ein Gebäude zu brennen beginnt
  if (FindObject(_NCS))
    if (GetCategory(obj) & C4D_Structure())
    {
    // Löschen aufrufen
    Schedule("Extinguish()", 10, 0, obj);
    }
  return(inherited(obj, iNumber, iTemp, iCausedBy, fBlasted, pIncineratingObject));
  }

global func FxFireStop(object obj, int iNumber, int iReason, bool fTemp)
  {
  // Wenn die Regel aktiviert ist und ein Gebäude gelöscht wird
  if (FindObject(_NCS))
    if (GetCategory(obj) & C4D_Structure())
    {
    // Verbranntes Gebäude vervollständigen
    SetCon (100, obj);
    }
  return(inherited(obj, iNumber, iReason, fTemp));
  }