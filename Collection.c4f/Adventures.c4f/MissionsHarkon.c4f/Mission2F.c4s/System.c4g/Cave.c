/* Blimp */

#strict 2

#appendto CAVE

protected func ContainedUp(object pPlayer)
{
	var sText = GameCall(Format("MsgSignpost%d", 1));
  var szPortrait = Format("%i", GetID());
  CreateMenu(GetID(),pPlayer,this,0,Format("<c %x>%s:</c>", GetColorDw(), GetName()),0,C4MN_Style_Dialog);
  AddMenuItem(szPortrait,0,NONE,pPlayer,0,0,0,5);
  AddMenuItem(sText,0,NONE,pPlayer);
  AddMenuItem("$Cave01$", "DoTravel", LMM2, pPlayer, 0, pPlayer, 0, 2, 4);
  AddMenuItem("$Cave02$", "Stop", MCMX, pPlayer, 0, pPlayer);
}

public func Stop() { }

public func DoTravel(idID, pPlayer)
{
  if(!GameCall("IsFullfilled"))
  {
    var szPortrait = Format("%i", GetID());
    CreateMenu(GetID(),pPlayer,this,0,Format("<c %x>%s:</c>", GetColorDw(), GetName()),0,C4MN_Style_Dialog);
    AddMenuItem(szPortrait,0,NONE,pPlayer,0,0,0,5);
    AddMenuItem(GameCall("FullfillText"),0,NONE,pPlayer);
    AddMenuItem("$Cave02$", "Stop", MCMX, pPlayer);
    return;
  }
  GameCall("SignpostReached");
	var pStory = FindObject(_STY);
	if(pStory && pStory->~SignpostReached(pPlayer))
		return;
	FinishMission();
}