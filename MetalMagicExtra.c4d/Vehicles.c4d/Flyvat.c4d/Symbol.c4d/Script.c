/*-- Symbol --*/

#strict 2

func Initialize() {
  SetVisibility(VIS_Owner | VIS_Allies | VIS_God); 
  return 1;
}

protected func AttachTargetLost()
{
 return RemoveObject();
}
