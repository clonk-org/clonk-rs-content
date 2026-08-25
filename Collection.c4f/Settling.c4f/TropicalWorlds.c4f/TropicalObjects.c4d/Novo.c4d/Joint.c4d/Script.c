/*-- Joint --*/

#strict

public func Activate(object pClonk)
{
  [$TxtEat$]
  Kiff(pClonk);
  return(1);
}

public func Kiff(object pClonk)
{
  pClonk->~Feed(20);
  DoAlcohol(pClonk,1900);
  CreateParticle("Smoke",0,0,0,-5,420,RGBa(120,120,120,64));
  CreateParticle("Smoke",-2,0,0,-5,420,RGBa(120,120,120,64));
  CreateParticle("Smoke",+2,0,0,-5,420,RGBa(120,120,120,64));
  CreateParticle("Smoke",0,-8,0,-5,420,RGBa(120,120,120,64));    
  RemoveObject();
  return(1);
}
protected func Hit(){
   Sound("WoodHit*");
  return(1);
}
func Incineration()
{
  SetClrModulation (RGB(48, 32, 32));
  CastParticles("MaterialParticle2",25,20,0,0,20,100,RGB(255,120,00),RGB(200,120,0));
  Explode(12);
}
