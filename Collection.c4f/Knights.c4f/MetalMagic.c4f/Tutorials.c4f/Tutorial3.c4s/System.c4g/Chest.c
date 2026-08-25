/*-- Kiste --*/

#strict 2

#appendto CHST

public func Initialize()
{
  CreateContents(SMGD);
  CreateContents(SPPR);
}

global func GetChestCount() { return 6; }
