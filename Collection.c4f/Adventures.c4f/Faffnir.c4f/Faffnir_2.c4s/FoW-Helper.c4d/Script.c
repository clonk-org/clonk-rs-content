/*-- Neues Objekt --*/

#strict

func Initialize() {
   
  return(1);
}

protected func ClonkSuch() {
if(FindObject2(Find_OCF(OCF_CrewMember), Find_InRect(-450, -100, 900, 200))) RemoveObject(); return(1); }
