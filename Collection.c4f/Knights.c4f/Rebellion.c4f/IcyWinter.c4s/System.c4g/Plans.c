/*-- Waffenbaupläne --*/

#strict

#appendto WPPL

public func SetKnowledge(iPlayer)
{
  SetPlrKnowledge(iPlayer, BRMR);
  return(_inherited());
}

