/*-- Bewuchssteuerung --*/

#strict

/* Überladbare Konstanten */

private func ReproductionAreaSize() { return(1600); } // Größe des Bereichs, in dem neuer Bewuchs dieser Art entstehen kann
private func ReproductionRate()     { return(125); } // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet
private func MaxCoverCount()         { return(160); } // Maximale Bewuchsanzahl im Umkreis

/* Initialisierung */

protected func Construction()
{
  if (SetAction("Initialize"))
    {
    // Ausrichtung
    SetDir(Random(2));
    }
  return(0);
}

private func GetSolidOffset(int x, int y)
{
  var i;
  for (i = -15; GBackSolid(x, y - i) && (i < 15); i++);
  return(i);
}

/* Timer Call */

private func TimerCall()
{
CheckEarth();
Seed();
return(1);
}

/* Fortpflanzung */

private func Seed()
{
  // Je nach Fortpflanzungsrate mehr oder weniger oft...
  if (!Random(ReproductionRate()))
    // ...reproduzieren
    Reproduction();
}

public func Reproduction()
{
  // Ist noch Platz für Bewuchs?
  var iSize = ReproductionAreaSize();
  var iOffset = iSize / -2;
  if (ObjectCount2(Find_InRect(iOffset, iOffset, iSize, iSize),Find_Or(Find_ID(KFLR),Find_ID(GRAS)))<MaxCoverCount())
    // OK, hin damit
    return(PlaceVegetation(GetID(this()), iOffset, iOffset, iSize, iSize, 10) );
  // Kein Platz ;'(
  return(0);
}

private func MoveBehindTrees()
{
  var obj;
  while (obj = FindObject(0, 1,1, 0,0, OCF_Chop, 0,0, NoContainer(), obj))
    if (obj->~IsTree() && (obj->GetCategory() & C4D_StaticBack))
      SetObjectOrder(obj, this(), 1);
}

// Bei nahen Explosionen
public func OnShockwaveHit(iLevel,iX,iY)
{
 var con=(40*GetCon())/100;
 iLevel=40+iLevel/2;
 for(var cnt=0;cnt<15+Random(10);cnt++)
  CreateParticle("GrassBlade",RandomX(-con/2,con/2),-1,RandomX(-iLevel/3 ,iLevel/3),RandomX(-2*iLevel/3,-iLevel/3),30+Random(30),RGB(255,255,255),0,0);
 return(RemoveObject());
}

protected func CheckEarth(){
  //checkt ob es in der Luft hängt
  if(GetMaterial(0,10)!=Material("Earth"))
  DoCon(-10,this());
  return(1);
  }

// Kann immer von Schockwellen getroffen werden
public func CanBeHitByShockwaves(){return(true);}
public func BlastObjectsShockwaveCheck(){return(true);}
