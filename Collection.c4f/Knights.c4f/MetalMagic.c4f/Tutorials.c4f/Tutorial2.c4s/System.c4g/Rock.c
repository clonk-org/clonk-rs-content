/* Raucheffekte des Steines im Berg */
#strict 2
#appendto ROCK

func Hit()
{
  if(Local(0)) {
      Sound("RockHit*", 0, 0, 40);
      CastParticles("FSpark", 5,10, 0,0, 50, 75);
      RemoveObject();
      return;
    }
  _inherited();
    return 1;
}    
