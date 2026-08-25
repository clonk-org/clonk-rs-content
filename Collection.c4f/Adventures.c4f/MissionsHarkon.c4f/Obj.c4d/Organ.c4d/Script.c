/*-- Orgel --*/

#strict

func Initialize() {
  //SoundLevel("Phantom of the Opera 1989", 0, this());
  //SoundLevel("Phantom of the Opera 1989", 100, this());
  return(1);
}

protected func Dudeldu()
{
 if(!Random(3))
  CreateParticle("Note", -3, -20, RandomX(-6,6), -8, 40, RGBa(100,100,155,0));
 if(!Random(4))
  CreateParticle("Note1", 3, -20, RandomX(-7,7), -8, 40, RGBa(100,100,155,0));
 if(!Random(2))
  CreateParticle("Note2", 0, -20, RandomX(-8,8), -8, 40, RGBa(100,100,155,0));
  return(1);
}

protected func Damage() 
{
  if (GetDamage() < 15) return(0);
  CreateContents(_ORB,this());
  RemoveObject(0,1);
  return(1);
}