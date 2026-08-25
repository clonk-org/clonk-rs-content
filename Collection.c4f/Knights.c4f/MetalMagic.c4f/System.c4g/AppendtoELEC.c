/*-- AppendtoELEC --*/

#strict 2
#appendto ELEC

protected func Activity()
{
  if(!Local(23))
    if(!(Local(23) = GetPlayerTeam(GetOwner())))
      return _inherited();
  if(GetOwner()==-1)
  {
    for(var i = 0; i < GetPlayerCount(); i++)
      if(GetPlayerTeam(GetPlayerByIndex(i)) == Local(23))
        return SetOwner(GetPlayerByIndex(i));
  }
  return _inherited();
}
