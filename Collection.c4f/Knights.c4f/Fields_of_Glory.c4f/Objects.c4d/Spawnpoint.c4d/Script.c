/*-- Spawnpoint --*/

#strict

local initialized, angle, player, spawntimer, spawns;

global func PlaceSpawnpoint(id idObj, int iX, int iY, int timer)
{
  var spwn = CreateObject(SPNP, iX, iY, -1);
  spwn->CreateContents(idObj);
  if(timer)
    spwn->LocalN("spawntimer") = timer;
  return(spwn);
}

protected func Initialize()
{
  // Spawnauswahl
  spawns = [XARP,SFLN,FLNT,COKI,METL];
  // Standardtimer = 1000 Frames
  spawntimer = 1000;
  SetClrModulation(RGBa(0,0,0,255));
}

protected func Timer()
{
   // Objekt als Layer auflegen
  SetGraphics(0, 0, GetID(Contents()), 1, 1);
  
  // Noch nicht initialisiert?
  if(!initialized)
    // Ein Inhaltsobjekt?
    if(Contents())
      // Initialisieren
      return(Initialized());
  // Kein Inhalt?
  if(!Contents())
    {
    // Nicht mehr initialisiert
    initialized = false;
    return();
    }
  // Tolle Effekt starten
  angle += 10;
  if(angle >= 360) angle = 0;
  SetObjDrawTransform(1000, 0,0,0, 1000, Sin(angle, 8)*1000 - 2000,0, 1);
  if(Random(2)) CreateParticle("VMSpark", RandomX(-5,5), RandomX(5,10), 0, -5, 25, RGBa(210, 210, 255, 100));
  // Alle Timer runterzählen
  DecreaseTimer();
}

private func Initialized()
{
  // Unsichtbar werden
  SetAction("Invis");
  // Objekt als Layer auflegen
  SetGraphics(0, 0, GetID(Contents()), 1, 1);
  // Transformation
  SetObjDrawTransform(1000, 0,0,0, 1000, -5000,0, 1);
  // Objekt(e) erzeugen
  // Waffen-Bleiben-Regel
  if(FindObject(WPST))
    {
    for(var i=0 ; i < GetPlayerCount() ; i++)
      Local() += 2**GetPlayerByIndex(i);
    SetVisibility(VIS_Local() | VIS_God());
    }
  else
    Local() = true;
  player = CreateArray();
  // Initialisiert
  initialized = true;
}

/* Timer runterzählen */

private func DecreaseTimer()
{
  // Waffen-Bleiben-Regel
  if(FindObject(WPST))
    {
    for(var i=0 ; i < GetPlayerCount() ; i++)
      // Spielertimer runterzählen
      {
      if(player[GetPlayerByIndex(i)])
        player[GetPlayerByIndex(i)] -= 5;
      if(player[GetPlayerByIndex(i)] <= 0 && !(Local() & 2**GetPlayerByIndex(i)))
        {
        // Spieler kann das Objekt wieder einsammeln
        player[GetPlayerByIndex(i)] = 0;
        Local() += 2**GetPlayerByIndex(i);
        }
      }
    }
  else
    {
    // Timer runterzählen
    player[0] -= 5;
    if(player[0] <= 0)
      {
      // Objekt ist wieder da
      player[0] = 0;
      Local() = true;
      SetVisibility(VIS_All());
      }
    }
}

public func RejectEntrance(object pClonk)
{
  // Objekt ist ein Clonk?
  if(!(GetOCF(pClonk) & OCF_CrewMember) && !(pClonk->~CanCollectFromSpawnpoints())) return(1);
  // Ich hab Contents?
  if(!Contents()) return(1);
  // Darf einsammeln
  if(CheckCollect(GetOwner(pClonk)))
  {
    var pObj = CreateContents(GetID(Contents()));
    // Kann der Clonk einsammeln?
if (!Contents() ->~ IsArrowPack()) 
{
    if(ObjectCount2(Find_Container(pClonk),Find_Not(Find_Func("IsArrowPack"),Find_Func("IsArrow"))) >= 3)
      return(RemoveObject(pObj));
}
else
{
    if(ObjectCount2(Find_Container(pClonk),Find_Func("IsArrowPack")) >= 3)
    return(RemoveObject(pObj));
}
    Collect(pObj, pClonk);
    if(Contained(pObj) == this()) RemoveObject(pObj);
    else
    {
      Sound("Grab", 0, pClonk, 0, GetOwner(pClonk)+1);
      Collected(GetOwner(pClonk));
    }
  }
  return(1);
}

private func CheckCollect(int iPlr) // Überprüft, ob ein Spieler das Objekt einsammeln darf
{
  // Waffen-Bleiben-Regel
  if(FindObject(WPST))
    return(Local() & 2**iPlr);
  else
    return(Local());
}

private func Collected(int iPlr) // Regelt, dass ein Spieler das Objekt einsammelt
{
  // Waffen-Bleiben-Regel
  if(FindObject(WPST))
    {
    // Spielertimer hochsetzen
    player[iPlr] = spawntimer;
    // Nicht mehr sichtbar für den Spieler
    Local() -= 2**iPlr;
    }
  else
    {
    // Timer hochsetzen
    player[0] = spawntimer;
    // Nicht mehr einsammelbar
    Local() = false;
    // Nicht mehr sichtbar
    SetVisibility(VIS_God());    
    // Neues Objekt als Inhalt erzeugen
    while (Contents()) RemoveObject(Contents());
    CreateContents(spawns[Random(GetLength(spawns))]);
    }
}
