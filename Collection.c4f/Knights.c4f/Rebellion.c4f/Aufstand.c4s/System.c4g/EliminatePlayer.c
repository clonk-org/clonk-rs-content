
#strict

global func EliminatePlayer(int iPlr, b,c,d,e,f,g,h,i,j)
{
  // Team-Hud entfernen
  for(var obj in FindObjects(Find_ID(_THD), Find_Owner(iPlr)))
    RemoveObject(obj);
  // Überladene Funktion aufrufen
  return(inherited(iPlr,b,c,d,e,f,g,h,i,j));
}
