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
  AddMenuItem("Level beenden", "DoTravel", LMM2, pPlayer, 0, pPlayer, 0, 2, 4);
  AddMenuItem("Abbrechen", "Stop", MCMX, pPlayer, 0, pPlayer);
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
    AddMenuItem("Abbrechen", "Stop", MCMX, pPlayer);
    return;
  }
  GameCall("SignpostReached");
	var pStory = FindObject(_STY);
	if(pStory && pStory->~SignpostReached(pPlayer))
		return;
	FinishMission();
}