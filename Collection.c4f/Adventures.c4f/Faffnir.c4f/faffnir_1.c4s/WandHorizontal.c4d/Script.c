/*-- Neues Objekt --*/

#strict

func Initialize() {
   
  return(1);
}

public func ControlLeft() {
  Sound("Magic1", 1, FindObject(ASAS), 99);
  RemoveObject();
  return(1); }
  
  
  public func ControlRight() {
  Sound("Magic1", 1, FindObject(ASAS), 99);
  RemoveObject();
  return(1); }
