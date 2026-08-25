/*-- Flaggenmast --*/

#strict 2

local basement;

func Initialize()
{
 basement=true; // Gegen den Wind-Pfeil-Combo
}

protected func RejectCollect(ID)
{
 if(ID != FLGP) return 1;
}

public func ControlDig(pClonk)
{
 if(Contents())
   Contents()->~SetDown(this);
 //if(!FindObject(POS1,0,0,0,0,0,"FlyBuilding",this))return;
 //ObjectSetAction(FindObject(POS1,0,0,0,0,0,"FlyBuilding",this),"HangDown",this);
 //Collect(Contents(),obj);
}

public func ControlThrow(pClonk)
{
  if(FindContents(FLGP, pClonk))
    Enter(this, FindContents(FLGP, pClonk));
  return 1;
}

public func ControlDown(pClonk)
{
  if(Contents())
    Contents()->~SetDown(this);
}

public func ControlUp(pClonk)
{
  if(FindContents(FLGP, pClonk))
    Enter(this, FindContents(FLGP, pClonk));
  return 1;
}

public func Down()
{
 var clnk=FindObject(0,0,0,0,0,OCF_CrewMember,"Push",this);
 if(clnk)
 {
  //var flag=FindContents(FLGP);
 // Exit(flag);
  Collect(FindContents(FLGP),clnk); 
 }
 
 if(FindContents(FLGP))
 FindContents(FLGP)->Enter(this); //ObjectSetAction(FindObject(POS1,0,0,0,0,0,"FlyBuilding",this),"Up",this);
} 
