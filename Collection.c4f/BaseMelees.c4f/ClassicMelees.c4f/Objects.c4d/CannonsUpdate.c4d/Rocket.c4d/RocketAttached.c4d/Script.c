/*--- Rakete (angeschraubt) ---*/

#strict

local FillLevel;
local iMaxFillLevel;
local hits;

protected func MaxFillLevel() { return(iMaxFillLevel); }
protected func RotationSpeed() { return(4); }
protected func CannonMobileID() { return(1RKT); }

protected func GetMass() {
	var Mass = 15+25; 
	return(Mass);
	}

protected func Hit()
{
	hits=1;
	CastPXS("Oil", FillLevel, 80);
	Explode(30,CreateObject(GUNP));
	Explode(30,CreateObject(GUNP));
	Explode(60);
}

protected func Damage()
{
  if (GreaterThan(GetDamage(),10) && !hits) Hit();
  return(1);
}

/* Turm weg und Treibstoffcheck */

public func ComEnter(object pAmmo)
{
  // Zu viel Öl in der Rakete? Dann ablassen
  if(FillLevel>MaxFillLevel())
	{
	var iLeak = FillLevel-MaxFillLevel();
	FillLevel = MaxFillLevel();
	// Materialtransfer
        for (var i = 0; i < iLeak; i++)  
          ObjectInsertMaterial(12, GetActionTarget()); // 12 für Öl
	}
  // Ölfasser in Munition umwandeln
  while(GetID(pAmmo) == OBRL && FillLevel<MaxFillLevel())
  {
    if(pAmmo->OBRL::GetAmount() < 10) break;
    Sound("Click");
    FillLevel += 10;
    pAmmo->OBRL::BarrelDoFill(-10);
    if(pAmmo->OBRL::GetAmount() == 0) ChangeDef(pAmmo->EmptyBarrelID(),pAmmo);
  }
}

public func ComFire(object pClonk)
{
  // Treibstoff vorhanden?
  if(FillLevel <= 0)	return(1);
  Sound("Click");
  SetRDir(0);
  SetAction("LAUNCH");

    // Controller setzen
    SetController(pClonk->GetController());
    
    // Ausstoß
    SetPlrView(GetOwner(pClonk),this());

}

public func Launch()
{
  SetCategory(GetDefCategory(GetID()));
  // Turm bescheid sagen und neue Rakete/Kanone suchen
  var Tower=GetActionTarget();
  LocalN("cannon",Tower)=0;

  SetAction("RUN");
  var i, obj;
  while(obj = Contents(i++,Tower) )
  {
    if(obj->~IsCannon() )
    {
      Tower->ConnectCannon(obj);
      break;
    }
  }
  return(1);
}

public func Accelerate()
{  
  // Schub aus Treibstoff erzeugen
  if(FillLevel <= 0)
	{
	SetAction("RUN1");
	return(1);
	}

  // Brenndauer hängt von Tankfüllung ab
  FillLevel -= 1;

  // In Normalrichtung fliegen
  var iXDir = Sin(GetR(),35);
  var iYDir = Cos(GetR(),35);

  // Wind hat Einfluss in X-Richtung
  var iXDirWind=GetWind()/20;

  SetXDir(+iXDir+iXDirWind, 0, 10);
  SetYDir(-iYDir, 0, 10);

  // Feuerburst ausführen
  var iAngle = GetR();
  if(iAngle>0) { CreateFireBurst(this(),10,Sin(iAngle,0),Cos(iAngle,0)+20,-Abs(iAngle)-270);}
  else CreateFireBurst(this(),10,Sin(iAngle,0),Cos(iAngle,0)+20,Abs(iAngle)-270);
  if(!Random(3)) Sound("FlameThrower");
}

// Fallen
public func Falling()
{
  // In Bewegungsrichtung drehen
  if (GetXDir()==0 && GetYDir()==0) return(SetR(180));
	if(GetYDir()>0)
	{
		if (GetXDir()>0)	SetR(180-ArcSin(GetXDir(),Sqrt(Pow(GetXDir(),2)+Pow(GetYDir(),2))));
		if (GetXDir()<0)	SetR(-180-ArcSin(GetXDir(),Sqrt(Pow(GetXDir(),2)+Pow(GetYDir(),2))));
	}
	if(GetYDir()<0)
	{
		if (GetXDir()>0)	SetR(90-ArcCos(GetXDir(),Sqrt(Pow(GetXDir(),2)+Pow(GetYDir(),2))));
		if (GetXDir()<0)	SetR(-90-ArcCos(GetXDir(),Sqrt(Pow(GetXDir(),2)+Pow(GetYDir(),2))));
	}
  // Langsam eindrehen
  /*if (Abs(GetR())==180 || GetR()==0) return();
  if (GetR()<0)	SetR(GetR()-2);
  if (GetR()>0)	SetR(GetR()+2);*/
  return(1);
}

//Taumeln
public func Tumble(iTumb)
{
var Tumb = 4;
if (iTumb) Tumb=iTumb;
SetR(GetR()+Random(Tumb+1)-Tumb/2);
return(1);
}

// Timer Call (Jedes 1 Frame)

public func CheckFlight()
{
// Flug zu Ende und nicht Explodiert?
if(GetAction() eq "RUN1")
	if (!GetXDir() || !GetXDir())	ChangeDef(CannonMobileID());
/* Ziel getroffen? */
   SetVar(0,Or(FindObject(SUB1,0,1,0,0,0, 0,0, NoContainer()),
   		FindObject2(Find_ID(_RTA),Find_AtPoint(0,1),Find_Not(Find_ActionTarget(GetActionTarget())), Find_NoContainer()),
		     FindObject(MCTW,0,1,0,0,0, 0,0, NoContainer()),
   		 	FindObject(_HEL,0,1,0,0,0, 0,0, NoContainer())));  
   if (Var(0)) return(Hit());
return(1);
}

/*--- Rakete (angeschraubt) ---*/

/* Turm weg? */

protected func AttachTargetLost()
{
  ComStopDouble();
}

/* Kommandos aus dem Turm */

public func ComLeft(object pClonk)
{
  SetAction("Rotating",GetActionTarget());
  SetRDir(-RotationSpeed() );
  return(1);
}

public func ComRight(object pClonk)
{
  SetAction("Rotating",GetActionTarget());
  SetRDir(RotationSpeed() );
  return(1);
}

public func ComStop(object pClonk)
{
  SetAction("Attaching",GetActionTarget());
  SetRDir(0);
  return(1);
}

public func ComStopDouble(object pClonk)
{
  var r = (GetR() + 270) % 360;
  SetR(r);
  ChangeDef(CannonMobileID() );
  return(1);
}

protected func RejectCollect(id idObject, object pObj)
{
  // Pfeile vertragen sich nicht mit einer Rakete
  if (DefinitionCall(idObject,"IsArrow") )
   // Nur, wenn der Pfeil sich auch bewegt
   if(GetXDir(pObj) || GetYDir(pObj))
  {
    // Bei Treffer trudeln
    Tumble(60);
    // Pfeil schlägt immer auch ein...
    ProtectedCall(pObj, "Hit");
    return(1);
  }
  // in jedem Fall bei allen Objekten Hit auslösen (Sprengpfeil / Flint explodiert zB)
  ProtectedCall(pObj, "Hit");
  return(1);
}

protected func Collection(object pObj)
  {
  var idObj = GetID(pObj);
  // abstürzen
   Tumble(60);
  // in jedem Fall im Object Hit auslösen (Sprengpfeil explodiert zB)
  ProtectedCall(pObj, "Hit");
  return(1);
  }