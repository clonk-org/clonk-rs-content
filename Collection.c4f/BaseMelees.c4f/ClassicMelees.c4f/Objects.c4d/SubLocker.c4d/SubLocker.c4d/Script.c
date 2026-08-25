/*-- U-Boot-Schutz --*/

#strict

#appendto SUB1

protected func ActivateEntrance(object pByObject)
{
  if(FindObject(BL5P))
  {
    var i, contents;
    while(contents=Contents(i))
    {
      if(CrewMember(contents)) if(Hostile(GetOwner(contents),GetOwner(pByObject))) return(0);
      i++;
    }
  }
  return(_inherited(pByObject));
}