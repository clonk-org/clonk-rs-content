/* Magier */

#strict

#include SCLK

protected func Initialize()
{
  // Wassereffekt anhängen
  AddEffect("SkeletonWaterCheck",this(),20,1,this());
  return(_inherited());
}

protected func Recruitment()
{
  // Portrait setzen
  SetPortrait("1");
  DoEnergy(100);
  SetAction("Dead");
  return(_inherited());
}

// Zur Sicherheit, damit Überladungen keinen Fehler beim Starten geben
protected func Scaling()
{
  var szDesiredAction;
  if (GetYDir()>0) szDesiredAction = "ScaleDown"; else szDesiredAction = "Scale";
  if (GetAction() ne szDesiredAction) SetAction(szDesiredAction);
  return(1);   
}

protected func ContactBottom()
  {
  if (SEqual(GetAction(), "Swim"))
    SetAction("Walk");
  return(1);
  }

protected func Death()
{
  // Ein Skelett zerfällt zu Knochen
  CastObjects(BONE,4,10);
 
  _inherited(Par(),Par(1),Par(2));
 
  if(this())
    RemoveObject(0,1);
  return(1);
}

/* Essen */  

public func Feed(iLevel)
{
  // Schon mal ein Skelett essen sehen?
  CastParticles ("DoughSlice", Random(3)+4, 13, 0,0, 30,35, RGB(170,115,60), RGB(145,95,50));
  Sound("WoodHit*");Sound("WoodHit*");
  return(1);
}

protected func FxSkeletonWaterCheckTimer(pTarget,iEffectNumber)
{
  // Im Wasser beim Springen...
  if(!InLiquid()) return(1);
  if(GetAction() ne "Jump" && GetAction() ne "Tumble") return(1);
  if(!GetXDir() && !GetYDir()) return(1);
  
  // ... wird das Skelett leicht abgebremst
  var xDir = GetXDir();
  if(GetXDir() > 16) xDir=(GetXDir()*7)/8;
  var yDir = GetYDir();
  if(GetYDir() > 16) yDir=(GetYDir()*7)/8;
  SetXDir(xDir);
  SetYDir(yDir);
  
  return(1);
}

// Böser Effektmissbrauch! :O
protected func FxSkeletonWaterCheckDamage(pTarget,iEffectNumber,iDmg,iCause)
{
  // Feuer ist einfach immer böse
  if(iCause == 35) return(iDmg);
  // Explosionen auch!
  if(iCause == 1)  return(iDmg);
  // Heilen sowiesoüberhauptundeh!
  if(iDmg > 0)return(iDmg);
 
  // Im Nahkampf nur etwas abschwächen
  if(iCause == 40) return((iDmg/10)*9);
 
  return((iDmg/10)*6);
}

public func ControlDown()
{
  // Skelett kann sich "tot" stellen
  if(GetPlrDownDouble(GetOwner()) && GetAction() eq "Walk") SetAction("Dead");
}

public func ControlUp()
{
  // Nach dem "Tot-Stellen" wieder aufstehen
  if(GetAction() eq "Dead") SetAction("UnDead");
}

/* Itemlimit */
public func MaxContentsCount() { return(1); }

// Ist ein magischer Clonk
public func IsMagiclonk()
{
  return(1);
}