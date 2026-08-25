/*-- Geld --*/

#strict 2

global func FxMakeMoneyStart()
{
 
}

global func FxMakeMoneyTimer()
{
 var aGiven=[];
 for(var cnt=0;cnt<GetPlayerCount();cnt++)
 {
  var iPlr=GetPlayerByIndex(cnt);
  var iTeam=GetPlayerTeam(iPlr);
  if(GetArrayItemPosition(iTeam,aGiven) != -1)continue;
  PushBack(iTeam,aGiven);
  DoWealth(iPlr,1);
 }
}
