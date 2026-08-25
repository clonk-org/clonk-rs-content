/*-- Deckenkristall --*/

#strict

protected func Initialize()
{
  var rand=1;
  if(Random(2) == 1)rand=-1;
  SetObjDrawTransform(1000*rand+RandomX(-300,+300),0,0,0,1000+RandomX(-300,+300));
}

protected func Check()
{
  // Entweder einen kleinen, starken Blitzer...
  if(Random(3)) {
    CreateParticle("MaterialSpark", RandomX(-10,10),RandomX(-20,20), 0,0, 5*5+Random(5*5), RGBa(255,255,255,0));
  }
  // ...oder ein größeres, schwaches Leuchten
  else {
    CreateParticle("MaterialSpark", RandomX(-10,10),RandomX(-20,20), 0,0, 10*5+Random(40*5), RGBa(255,255,255,192+Random(64)));
  }
}
