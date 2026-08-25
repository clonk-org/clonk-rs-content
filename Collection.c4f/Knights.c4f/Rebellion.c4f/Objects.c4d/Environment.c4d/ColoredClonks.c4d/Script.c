/*-- Bunte Clonks --*/

#strict

protected func Initialize() 
{
  // Alle Clonks anfangs umfärben (auch Leichen)
  var pClonk;
  while(pClonk=FindObject(0,0,0,0,0,OCF_CrewMember,0,0,0,pClonk))
  {
//    Log("Colorizing: %s", GetName(pClonk));
    CLFS->Colorize(pClonk);
    LocalN("Color_006C", pClonk) = GetColorDw(pClonk);
  }
}

