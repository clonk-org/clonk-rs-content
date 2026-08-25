/*-- Skelett König --*/

#strict

#include KING
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

/* Kontextmenü */  
  
public func ContextMagic(pCaller)
{
  [$CtxMagicDesc$|Image=MCMS|Condition=ReadyToMagic]
  // Nicht bereit
  if (!ReadyToMagic()) return(0);
  // Clonk soll anhalten
  SetComDir(COMD_Stop(), pCaller);
  // Menü öffnen
  var iExtra;
  if(ObjectCount(ALCO)) { if(ObjectCount(NMGE)) iExtra=1; else iExtra=5; }
  else                  { if(ObjectCount(NMGE)) iExtra=0; else iExtra=3; }
  CreateMenu(MCMS, 0, 0, iExtra, "$MnuNoSpells$", GetMagicEnergy());
  var idSpell=[RUND,MDBT,_MBS,_GDG], i;
  while (idSpell[i]) 
    {
    AddMenuItem("$MnuCast$: %s", "DoMagic", idSpell[i]);
    i++;
    }
  // Letzten Spruch auswählen
  SelectMenuItem(iLastSpell);
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
  _inherited();
}

public func ControlUp()
{
  // Nach dem "Tot-Stellen" wieder aufstehen
  if(GetAction() eq "Dead") SetAction("UnDead");
  _inherited();
}

// Ist ein magischer Clonk
public func IsMagiclonk()
{
  return(1);
}
