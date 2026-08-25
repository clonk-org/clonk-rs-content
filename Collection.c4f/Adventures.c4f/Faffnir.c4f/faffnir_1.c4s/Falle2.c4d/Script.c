/*-- Neues Objekt --*/

#strict

func Initialize() {
   
  return(1);
}

protected func DoofeClonksSuch() {
 if(GetEffect("Ausgelöst2")) SetClrModulation(RGB(240,240,240));
  else SetClrModulation(RGB(0,0,1));
 if(FindObject2(Find_ID(ASAS) ,Find_InRect(-100, 0, 100, 50)) && !GetEffect("Ausgelöst2"))
{ CastObjects(ROCK, 5, 50, 0,0); AddEffect("Ausgelöst2", 0,400,500); }
 }
