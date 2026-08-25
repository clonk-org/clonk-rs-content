/*-- Buch --*/

#strict 2

local sReadFunc; // Funktion beim Lesen
local sText; // Buchtext
local sID;   // Buchtext-ID (aus Szenarienscript)

protected func Initialize()
  {
  SetColorDw(HSL(Random(256), 255, Random(128)+128));
  }
  
protected func Activate(object pPlayer)
  { [$CtrlRead$]
  // Spieler vorbereiten
  StopClonkEx(pPlayer);
  if (sReadFunc) return (Call(sReadFunc, pPlayer, sText));
  var sShowText = sText;
  var pSpeaker;
  if (!sShowText) { sShowText = "$MsgCantRead$"; pSpeaker = pPlayer; }
//  if (!pPlayer->GetSkill(BO69)) { sShowText = "$ErrNoRead$"; pSpeaker = pPlayer; }
//  return MsgBox(pPlayer, sShowText, pSpeaker);
  }

public func SetTextID(string sToText)
  {
  sID = sToText;
  UpdateTransferZone();
  return true;
  }
  
public func SetReadFunc(string sToFunc)
  {
  sReadFunc = sToFunc;
  return true;
  }

protected func UpdateTransferZone()
  {
  // Buchtext aktualisieren
  var sText2;
  if (sID) sText2 = GameCall(Format("MsgBook%s", sID));
  if (sText2) sText = sText2;
  else if (!sText && sID) return Message("@GIMME TEXT (%s)!", this, sID);
  Message("", this);
  }
  
