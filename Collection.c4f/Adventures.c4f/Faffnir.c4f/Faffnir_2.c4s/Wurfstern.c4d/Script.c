/*-- Neues Objekt --*/

#strict

func Initialize() {
   
  return(1);
}

protected func Departure(pContainer) {
  if(GetID(pContainer) == ASAS && !Contained(pContainer) && GetAction(pContainer) ne "Push")
 {if(GetDir(pContainer) == 1) SetSpeed(80, -6, CreateObject(503B, 17, 0, NO_OWNER));
   else SetSpeed(-80, -6, CreateObject(503B, -17, 0, NO_OWNER)); 
  
 RemoveObject(); }
 return(1);
 }

//Wers blöd machen will, der gucke sich das an und sage!

//public func Activate(pByObj) {
// if(GetID(pByObj) == ASAS)
 // {if(GetDir(pByObj) == 1)
  //  Exit(0, 17, 0, 0, 7, -3,20);
   // else Exit(0, -17, 0, 0, -7, -3,20); }
  // return(1); }
   
   
// #appendto ASAS
 
 //protected func ControlThrow() {
// if(GetID(Contents(0)) == 503B) return(Activate()); 
 //return(inherited(...));
// }
 
// ERROR: inherited function not found, use _inherited to call failsafe (in ControlThrow, Faffnir.c4s\Wurfstern.c4d\Script.c:23:19)

