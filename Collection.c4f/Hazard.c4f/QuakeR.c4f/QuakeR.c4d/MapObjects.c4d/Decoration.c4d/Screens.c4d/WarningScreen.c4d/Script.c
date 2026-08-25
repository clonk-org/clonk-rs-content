/*-- Warning-Screen --*/

#strict

func Initialize() {
  SetAction("Warning");
  AddFrame();
  return(1);
}

func StandbyChoose() {
if (!(WildcardMatch(GetAction(),"StandBy*"))) return false;
if (Random(2)) SetAction("StandBy1"); else SetAction("StandBy2");
return();
}

func Wait() {
ScheduleCall(this(),"StandbyChoose",RandomX(30,120));
return(); 
}





