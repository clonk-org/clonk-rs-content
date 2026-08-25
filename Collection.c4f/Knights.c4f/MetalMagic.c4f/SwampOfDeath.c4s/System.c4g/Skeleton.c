/*-- Skelette bringen Geld --*/

#strict 2
#appendto SKLT
#appendto _SSR
#appendto _ZMB
#appendto _SKN
#appendto BTRL

func Death() {
  if(GetID()==_SSR) DoWealth(GetPlayerByIndex(), 10);
  else if(GetID()==_ZMB) DoWealth(GetPlayerByIndex(), 10);
  else DoWealth(GetPlayerByIndex(), 10);
  if(GetWealth(GetPlayerByIndex())>500) SetWealth(GetPlayerByIndex(), 500);
  
  var c;
  if(GetPlayerName(GetKiller()))
  	if(c=GetCrew(GetKiller()))
  		c->~DoKarmaEnergy(10);
  return _inherited();
}

public func IsUndead() { return 1; }
