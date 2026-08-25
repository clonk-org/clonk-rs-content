/*-- Lagerfeuer --*/

#appendto CPFR
#strict 2

private func Smoking() 
{
  if (!Random(3)) Smoke(Random(3), Random(3), Random(8) + 5);
}

protected func CheckContents() { }
