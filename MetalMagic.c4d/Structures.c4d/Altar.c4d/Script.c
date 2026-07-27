/*-- Altar --*/

#strict 2

protected func Completion()
{
 // Nach dem Bauen Clonks nach oben versetzen? 
 for(var clnk in FindObjects(Find_Distance(20,0,0),Find_Or(Find_OCF(OCF_Alive),Find_Category(16))))
 {
  var cnt=0;
  while(cnt < 10 && Stuck(clnk)){SetPosition(GetX(clnk),GetY(clnk)-1,clnk);cnt++;}
  // Nicht, dass es nachher in irgendeine Decke versetzt..
  if(Stuck(clnk))SetPosition(GetX(clnk),GetY(clnk)+cnt,clnk);
 }
}

protected func RejectCollect(ID)
{
  if(ID != BHLG)return 1;
  ScheduleCall(this,"RefreshAction",1);
}

protected func Ejection()
{
  RefreshAction();
}
public func RefreshAction()
{
  if(GetAction() != "Show")
    SetAction("Show");
 
  SetPhase(BoundBy(ContentsCount(BHLG),0,3));
}

public func ControlDig(pClonk)
{
 if(GetOwner() != -1)return;
 SetOwner(GetOwner(pClonk),this);
 return 1;
}

public func ControlUp(pClonk)
{
	if(!pClonk)return 1;
	MMShowHelpMessage("OnUseAltar", GetOwner(pClonk));
  if(!pClonk->~MaxKarmaEnergy()) return No("$NeedPaladin$", GetOwner(pClonk));
  var pHoligon = FindContents(BHLG);
  if(!pHoligon) return No("$NeedBar$", GetOwner(pClonk));
  if(GetEffect("HolyWater", pClonk))
  	return No("$AlreadyHave$");
  if(pClonk->~GetKarmaEnergy() == pClonk->~MaxKarmaEnergy()) return No("$AlreadyFull$");
  CreateParticle("PxSpark",0,-10,0,-10,70,RGB(255,255,255),this);
  RemoveObject(pHoligon);
  RefreshAction();
  Schedule("AddEffect(\"HolyWater\",this,210,10,0,HLWT,1000)",10,0,pClonk);
  Sound("Chant", 0, this, 0, GetOwner(pClonk));
  PlayerMessage(GetOwner(pClonk), "$Blessing$", pClonk);
  return 1;
}

func No(str, iPlr)
{
	PlayerMessage(iPlr, str, this);
	Sound("Error", 0, 0, 0, iPlr);
	return 1;
}

