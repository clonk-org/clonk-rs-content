/*-- Waffenpläne --*/

#strict

#appendto WPPL

public func SetKnowledge(iPlayer)
{
  // Belagerungswaffen baubar
  SetPlrKnowledge(iPlayer, MS6C);
  return(_inherited());
}

