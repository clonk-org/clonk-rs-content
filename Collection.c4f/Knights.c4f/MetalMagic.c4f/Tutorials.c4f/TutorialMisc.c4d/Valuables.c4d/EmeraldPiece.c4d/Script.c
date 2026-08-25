/* Smaragdsplitter */

#strict 2

protected func Hit()
{
  Sound("Crystal*");
  return 1;
}

public func IsValuable() { return 1; }
