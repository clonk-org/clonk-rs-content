/*-- Suizid --*/

#strict 2

protected func Activate(int iPlr)
{
  Log("$MsgSuicide$", GetPlayerName(iPlr));
  Kill(GetCursor(iPlr));
  return 1;
}
