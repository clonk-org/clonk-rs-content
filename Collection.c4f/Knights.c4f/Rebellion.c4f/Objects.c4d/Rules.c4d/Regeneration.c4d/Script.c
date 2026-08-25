/*-- Erholung --*/

#strict

protected func Regenerate()
{
  var pObj;
  while(pObj = FindObject(,,,,, OCF_Alive || OCF_Living || OCF_CrewMember,,,, pObj))
  {
    if(!Random(4))
      DoEnergy(1, pObj);
    if(!Random(8))
      DoMagicEnergy(1, pObj);
  }
  return(1);
}

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}

