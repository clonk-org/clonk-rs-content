/*-- CaptureTheRelicts --*/

#strict 2

private func Execute()
{
  // Alle Heiligtümer
  var sainthoods=FindObjects(Find_ID(SNHD));
  for(var obj in sainthoods)
  {
   var relicts=0;
   relicts=ObjectCount(MRLC,AbsX(GetX(obj))-50,AbsY(GetY(obj))-25,100,50,0,0,0,NoContainer());
   
   if(relicts)
    if(GetPlayerName(GetOwner(obj)))
     DoWealth(GetOwner(obj),3);
     
   if(relicts >= 4)
    if(GetPlayerName(GetOwner(obj)))
     EliminateOpponentTeam(GetOwner(obj));
  }
}    

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}

protected func EliminateOpponentTeam(iPlr)
{
 for(var cnt=0;cnt<GetPlayerCount();cnt++)
 {
  var plr=GetPlayerByIndex(cnt);
  if(Hostile(iPlr,plr))EliminatePlayer(plr);
 }
}
