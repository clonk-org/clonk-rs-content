#strict

Initialize:
  SetXDir(-20+Random(41));
  SetYDir(-90-Random(21));
  SetRDir(-50+Random(101));
  return(1);

Timer:
  SetLocal(0,Local(0)+1);
  if(Local(0)>100)  return(RemoveObject());

  if(!Random(2))  CreateParticle("DBFire",0,0,0,0,RandomX(50,100),RGB(255,105,105));

  var pObject;
  while(pObject=FindObject(0,-15,-15,30,30,OCF_Prey(),0,0,NoContainer(),pObject))  if(GetAlive(pObject))

{
  CreateParticle("Blast", 0,0,0,0, 200);
  Sound("Blast*");

  var iDamage=Random(30)+(GetPlayerCount()*4);
  if(LocalN("pSchutz",pObject)>iDamage-1)  return(DoEnergy(-Random(2),pObject),RemoveObject());
  iDamage=iDamage-LocalN("pSchutz",pObject);
  Punch(pObject, iDamage);
  Sound("Travel*");

  RemoveObject();
}
  return(1);