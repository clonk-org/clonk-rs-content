/*-- Giftschuss --*/

#strict 2

#include BLLT

/* Schusseigenschaften */
public func PackTo() { return PBLP; }
public func IsBullet() { return 1; }
public func BulletStrength(){return 5+Random(5);}


/* Treffer */

private func HitTarget(pObject)
{
  Sound("BulletHit*");

  //Punch(pObject,BulletStrength());
  //Punch simulieren, weil doof
  
  // Gifteffekt anhängen, bzw verlängern
  if(!GetEffect("BulletPoision",pObject))
    AddEffect("BulletPoision",pObject,20,3,0,GetID(),GetOwner());
  else
    EffectVar(2,pObject,GetEffect("BulletPoision",pObject))+=300;
  // Schaden machen und schleudern
  DoEnergy(-BulletStrength(),pObject);
  if(Random(4)) Fling(pObject,GetXDir()/80,GetYDir()/80-2);
  
  // Fertig
  RemoveObject();
}

protected func Tail()
{
   CreateParticle("BulletTail",0,0,GetXDir(),GetYDir(),BoundBy(GetActTime(),0,17)*10,RGBa(150,255,150,10),this);
}

protected func FxBulletPoisionStart(pTarget,iEffectNumber,temp,val1)
{
  // Bei temporären Aufrufen nichts machen
  if(temp) return;
  // Zeit setzen
  EffectVar(0,pTarget,iEffectNumber)=val1;
  EffectVar(2,pTarget,iEffectNumber)=60;
  // Giftpartikel
  CreateParticle("PoisionSpark",GetX(pTarget),GetY(pTarget)-15,0,-1,70,RGB(255,255,255),pTarget);
  return 1;
}

protected func FxBulletPoisionTimer(pTarget,iEffectNumber,iEffectTime)
{
  // Im Wasser auswaschbar
  if(InLiquid(pTarget))return -1;
  if(!GetAlive(pTarget))return -1;
 
  // Ab und zu den Partikeleffekt erneuern
  if(iEffectTime%(3*(39/3))==0)
    CreateParticle("PoisionSpark",GetX(pTarget),GetY(pTarget)-15,0,0,70,RGB(255,255,255),pTarget);

  // Mit der Zeit verschwinden
  if(iEffectTime-EffectVar(2,pTarget,iEffectNumber)>200-Random(50))return -1;
 
  // In bestimmten Zeiträumen Schaden machen
  if(iEffectTime%(3*6)==0)
  {
    // Der Cursor soll den Schaden verursachen, damit der Besitzer des Schadens richtige gesetzt ist
    if(GetCrew(EffectVar(0,pTarget,iEffectNumber)))
      GetCrew(EffectVar(0,pTarget,iEffectNumber))->DoEnergy(-1,pTarget);
    else
      DoEnergy(-1,pTarget);
  }
 
  return 1;
}
