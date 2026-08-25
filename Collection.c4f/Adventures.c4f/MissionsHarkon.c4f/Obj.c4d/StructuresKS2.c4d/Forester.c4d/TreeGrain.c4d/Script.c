/*-- Samenkorn --*/

#strict 2

protected func Activate(pCaller){
  //Checken ob Erde da ist
  if(GetMaterial(0, 10) != Material("Earth"))
  {
  return(false);
  }
  //Sähen
  pCaller -> SetAction("Seed");
  RemoveObject(this());
  //Baum
  var tree=CreateObject(TRE4,0,10);
  DoCon(-90,tree);
  return(1);
  }
