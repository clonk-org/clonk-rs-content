/*-- Hilfsfunktionen --*/

#strict

global func CreateContents(id Id, object pObj, int iNumber)
{
  for(var i=1; i<iNumber; i++)
    _inherited(Id, pObj);
  return(_inherited(Id, pObj));
}
