/*-- Ritter --*/

#strict

#include KARH

public func GetMaxSpecialCount(iIndex, fAmount) { return 0; }

public func GetStrikeAction() { return "Strike"; } /* Schlagaktion */

protected func Striking()
{     
  var aRect = Find_InRect(-25,-10,25,20);
  if(GetDir() == DIR_Right) aRect = Find_InRect(0,-10,25,20);
   
  var pClonk = FindObject2(Find_OCF(OCF_Alive),Find_Hostile(GetOwner()),aRect,Find_Layer(GetObjectLayer()),Find_NoContainer());
  if(pClonk)
    Punch(pClonk,20+RandomX(0,5));
}