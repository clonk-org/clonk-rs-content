/*-- Statue --*/

#strict 2

public func MaxDamage() {
  return 30*5;
}

protected func Damage() {
  
  // Szenario benachrichtigen
  GameCall("StatueDamage",this);
  
  // Bei mehr als maximalem Schaden
  if (GetDamage() < MaxDamage()) return;
  
  Sound("Earthquake");
  Sound("Blast3");
  
  
  for(var cnt=0;cnt<30;cnt++)
  {
   var rand=100+Random(150);
   CreateParticle("MSmoke",RandomX(-61,61),RandomX(-67,67),0,0,500+Random(400),RGB(rand,rand,rand));
   Smoke(RandomX(-61,61),RandomX(-67,67),10);
   CreateParticle("Blast",RandomX(-61,61),RandomX(-67,67),0,0,400,RGB(255,255,255));
  }
  GameCall("StatueDestroyed");
  // Zerstören
  RemoveObject();
  
}
