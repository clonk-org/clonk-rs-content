/*-- Schild --*/

#strict

#appendto SHIE

protected func Activate(object pKnight)
{
  if(!_inherited(pKnight)) return(0);
  SetColorDw(GetColorDw(pKnight));
  return(1);
}

