/*-- Neues Objekt --*/

#strict

func Initialize() {
  Local(1)=RandomX(1,2);
  Local(2)=0;
  Local(3)=0;
  Turn();
  Turn();
  return(1);
}

public func IsSchwarz() { if(Local(1) == 1) return(1); }

public func IsWeiss() { if(Local(1) == 2) return(1); }

public func Turn() {
  Local(2)=0;
  Local(3)=0;
  if(Local(1) == 2) {SetClrModulation(RGB(1,0,0)); Local(1)=1; }
   else {SetClrModulation(RGB(255,255,255)); Local(1)=2; }
  ScheduleCall(this(), "Checken", 1);
  return(1);
  }

public func Checken() {
   for(var SchwarzT in FindObjects(Find_ID(793B), Find_Func("IsSchwarz")))
   Local(2)++;
  for(var WeissT in FindObjects(Find_ID(793B), Find_Func("IsWeiss")))
   Local(3)++;
  if(Local(2) == 16)
   {Explode(25,FindObject(SFLN));
   CustomMessage("Nun kann ich weiter! Nun gehört das Artefakt mir!", 0, NO_OWNER, 100,100,0,DECO,"Portrait:ASAS::00ff00::1"); }
  if(Local(3) == 16) {Explode(25,FindObject(SFLN));
   CustomMessage("Nun kann ich weiter! Nun gehört das Artefakt mir!", 0, NO_OWNER, 100,100,0,DECO,"Portrait:ASAS::00ff00::1"); }
  return(1);
 }

public func ControlRight() {
  Turn();
  return(1);
}

public func ControlLeft() {
  Turn();
  return(1);
}
