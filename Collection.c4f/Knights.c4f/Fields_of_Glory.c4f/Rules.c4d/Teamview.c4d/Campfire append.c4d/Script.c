/*-- Licht --*/

#strict
#appendto CPFR

//#appendto TORC
//#appendto FK9Z
//#appendto CPFR


protected func CheckContents() 
{
  if(GetActTime()>5000) {
    SetAction("Idle");
    if(FindObject(RTVW)) SetPlrViewRange(0);  	
	}
  if(ActIdle())
    if(ContentsCount(WOOD))
      return(BurnWood());
}

private func BurnWood() 
{
  var pWood;
  if(pWood=FindContents(WOOD)) RemoveObject(pWood);
  Sound("Inflame");
  SetAction("Burn");
  if(FindObject(RTVW)) SetPlrViewRange(25);  
}