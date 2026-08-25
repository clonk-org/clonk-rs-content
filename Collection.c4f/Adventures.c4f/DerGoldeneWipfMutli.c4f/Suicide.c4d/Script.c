/*-- Suizid --*/

#strict

protected func Activate() {

  Kill(GetCursor(Par())); 
  
  return(1);
}
