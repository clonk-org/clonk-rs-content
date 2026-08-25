/*-- Ritter --*/

#strict

#appendto KNIG

private func GetObject2Drop(object pCollObj)
{
  return(GetObject2DropCore(pCollObj));
}

private func GetObject2Drop_Knight(object pCollectObj)
{
  var iArrowPriority; 
  // Wenn ein Pfeil oder Pfeilpaket aufgenommen werden soll...
  if (pCollectObj) if (ObjectCall(pCollectObj, "IsArrow") || ObjectCall(pCollectObj, "IsArrowPack"))
    // Pfeilen und Pfeilpaketen eine niedrigere Priorität geben
    iArrowPriority=-500;
  // Das schlechteste Objekt wegwerfen
  var pBestDropObj, iBestDropPrio=1000;
  var pObj, iPrio;
  var idCollectObj = GetID(pCollectObj);
  for(var i = 0; pObj = Contents(i); i++)
    if ((iPrio = GetContentsPriority(pObj, iArrowPriority, idCollectObj)) < iBestDropPrio)
      {
      iBestDropPrio = iPrio;
      pBestDropObj = pObj;
      }
  // Dieses Objekt zurückgeben 
  return(pBestDropObj); 
}

private func ExecuteAttackBow(pTarget) 
  { 
  // Zielposition ermitteln 
  var iTx = GetX(pTarget), iTy = GetY(pTarget); 
  // Ziel in Reichweite?
  var iRange;
  if(GetOwner() == NO_OWNER)
    iRange = 120*(iDifficulty+1);
  else
    iRange = 300; // Spielerstärke soll nicht von Schwierigkeit abhängen!
  if (Abs(iTx-GetX())>iRange || Abs(iTy-GetY())>iRange)
    // Nicht? Mögliche Abschußposition einnehmen 
    return( SetCommand(this(),"MoveTo",0, BoundBy(GetX(),iTx-(iRange-10),iTx+(iRange-10)),GetY()) );
  // Bogen auswählen 
  ShiftContents(0, 0, BOW1); 
  // Kann leider von hier aus nicht zielen 
  if (!IsAiming()) 
    if (!this()->ReadyToFire()) 
      return(0); 
  // Ziel nicht im Freien 
  if (Contained(pTarget)) 
    return(0); 
  // Ziel von hier aus nicht in Sicht 
  if (!PathFree( GetX()-16+32*GetDir(),GetY(), iTx, iTy )) 
    if (!PathFree( GetX()-16+32*GetDir(),GetY()-20, iTx, iTy-20 )) 
      return(0);
  // Schießen 
  this()->FireBowAt(iTx,iTy, false); 
  // Weitermachen   
  return(1); 
  } 

// Gibt das Vorzeichen zurück (Signumfunktion)
global func Sgn(int number)
{
  if(!number) return(0);
  return(number/Abs(number));
}

/* Schlag mit Handwaffe */ 
private func Striking() 
{ 
  // Waffe bestimmen 
  var pWeapon; 
  if (!(pWeapon = GetFightWeapon())) return(0); 
  // Sound 
  pWeapon->~StrikeSound(); 
  // Kein Schaden 
  if (!Random(2)) return(1); 
  // Schaden ausrechnen 
  var pEnemy = GetActionTarget(); 
  var iDamage = pWeapon->~GetStrikeDamage(); 
  iDamage = iDamage * (100 - pEnemy->~IsShielded()) / 100;
  // Monstertrank?
  if(GetEffect("MonsterPotion", this()))
    iDamage += iDamage * (3+Random(5)) / 10;
  // Schlagen! 
  Punch(pEnemy, iDamage); 
  return(1); 
} 


