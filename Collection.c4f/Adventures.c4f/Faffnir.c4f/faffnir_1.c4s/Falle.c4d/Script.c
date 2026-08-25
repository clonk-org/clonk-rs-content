/*-- Neues Objekt --*/

#strict

func Initialize() {
   
  return(1);
}

protected func DoofeClonksSuch() {
 if(FindObject2(Find_ID(ASAS) ,Find_InRect(-52, -15, 105, 30)) && !GetEffect("Ausgelöst"))
{ CastObjects(LIQG, 25, 0, 0,0); AddEffect("Ausgelöst", 0,400,500); }
 }
