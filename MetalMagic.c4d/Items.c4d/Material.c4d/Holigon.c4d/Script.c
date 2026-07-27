/*-- Holigonerz --*/

#strict 2

func Initialize() {
   
  return 1;
}

 protected func Hit()
{
  Sound("Crystal*");
  return 1;
}

func IsAlchemContainer() { return true; }
func AlchemProcessTime() { return 200; }
