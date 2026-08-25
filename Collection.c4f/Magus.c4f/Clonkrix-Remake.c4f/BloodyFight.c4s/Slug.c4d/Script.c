/*-- Neues Objekt --*/

#strict

func Initialize() {
   SetAction("Be");
   SetPhase(Random(9));
   SetClrModulation(RGB(150, 150, 150));
   AddVertex(0,0,this);
  return(1);
}

