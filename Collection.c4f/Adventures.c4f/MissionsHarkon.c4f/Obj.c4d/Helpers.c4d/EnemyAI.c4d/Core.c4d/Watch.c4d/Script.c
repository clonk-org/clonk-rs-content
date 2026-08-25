/*-- Umsehen --*/

#strict 2

protected func Initialize() { SetAlive(0); }

protected func AdjustDir() { SetDir(GetActionTarget()->GetDir()); }

public func IsWarning() { return 1; }