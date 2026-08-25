/*-- Wegweiser --*/

#strict 2

local sID;   // Nachrichten-ID
local sText; // Text string
local pNPC;

public func SetTextID(iNewID)
{
  sID = iNewID;
  UpdateTransferZone();
}

protected func UpdateTransferZone()
  {
  // Wegweisertext aktualisieren
  sText = GameCall(Format("MsgSignpost%d", sID));
  if (!sText) return Message("@GIMME TEXT (%d)!", this, sID);
  Message("", this);
  }

protected func ControlUp(object pPlayer)
  { [$CtrlRead$]
  // Spieler vorbereiten (Action halten, weil Push)
  var szPortrait = Format("Portrait:%i::%x::%s", GetID(), GetColorDw(), "1");
  CreateMenu(GetID(),pPlayer,this,0,Format("<c %x>%s:</c>", GetDlgMsgColor(), GetName()),0,C4MN_Style_Dialog);
  AddMenuItem(szPortrait,0,NONE,pPlayer,0,0,0,5);
  AddMenuItem(sText,0,NONE,pPlayer);
  AddMenuItem("Level beenden", "DoTravel", LMM2, pPlayer, 0, 0, 0, 2, 4);
  AddMenuItem("Abbrechen", "Stop", MCMX, pPlayer);
  return 1;
  }

public func Stop() { }

public func DoTravel()
{
  GameCall("SignpostReached");
  GameOver();
}

public func SetText(string sToText)
  {
  sText = sToText;
  return true;
  }
  
public func GetDlgMsgColor() { return 10778899; }
