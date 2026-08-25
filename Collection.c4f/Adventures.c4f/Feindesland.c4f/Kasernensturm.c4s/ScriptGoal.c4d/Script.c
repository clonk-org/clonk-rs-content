/* Spielziel */

#strict

protected func Fertig()
{
 if(!FindObject(KING))
   {
    if(!FindObject(_BOK))
      {
       GainMissionAccess("Kaserne");
       GameOver();
       return();
      }
    return();
   }
 return();
}