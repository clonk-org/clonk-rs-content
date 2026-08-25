/*-- Edelsteinzahlaenderer --*/

#strict 2


public func Activate(iPlayer)
{
 if(GetPlayerByIndex(0) != iPlayer)
 {
  MessageWindow(GetDesc(),iPlayer);
  return 1;
 }
 if(NoMore())
 {
  MessageWindow("$NoMore$",iPlayer);
  return 1;
 }
 
 CreateMenu(GetID(),GetCursor(iPlayer),this,0,0,0,0,1);
 AddMenuItem("$Inc$","ChangeAmount(+1)",GMSN,GetCursor(iPlayer));
 AddMenuItem("$Dec$","ChangeAmount(-1)",_ASB,GetCursor(iPlayer));
 
 return 1;
}

func NoMore()
{
 if(ObjectCount2(Find_ID(GMSN)) < 18)return true;
 else
 return false;
}

func ChangeAmount(iVal)
{
 STONES_TO_STEAL=BoundBy(STONES_TO_STEAL+iVal,5,ObjectCount2(Find_ID(GMSN)));
 START_TIME=TIME_PER_GEM*STONES_TO_STEAL;
 Message("$Stat$",0,STONES_TO_STEAL,START_TIME/60,START_TIME%60);
 DoScoreboardRefresh();
}
