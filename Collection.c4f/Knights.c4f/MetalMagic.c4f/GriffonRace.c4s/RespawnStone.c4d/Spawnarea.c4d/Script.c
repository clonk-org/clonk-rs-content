/*-- FoW-Generator --*/

#strict 2

local w,h; // Breite/Höhe des Objektes
local iX, iY;

protected func Initialize()
  {
  // Anderes Placement-Objekt gefunden? Dann platzieren
  var pOther;
  if (pOther = FindObject(GetID(), 0,0,0,0, 0, "Placement"))
  { ShadowInit(GetX(pOther), GetY(pOther), GetX(), GetY()); return RemoveObject(pOther); }
  // Ansonsten Platzierung starten
  return SetAction("Placement");
  }

public func ShadowInit(int x1, int y1, int x2, int y2)
  {
  // Positionieren
  w = Abs(x2-x1); h = Abs(y2-y1);
 // w=h=(w+h)/2; // Solange FoW immer quadratisch ist, besser gleich breit wie hoch...
  SetPosition((x1+x2)/2, (y1+y2)/2);
  SetShape(w/-2, h/-2, w,h);
  iX = GetX(); iY = GetY();
  // Starte deaktiviert und sichtbar
  SetAction("Active");
  MakeVisible();
  }
  
public func MakeVisible(bool fShowRange)
  {
  SetVisibility(VIS_All);
  SetShape(w/-2, h/-2, w,h);
    {
    SetObjDrawTransform(w*1000, 0,w*w*500, 0,h*1000,h*h*500);
    SetClrModulation(RGB(255,0,0));
    }
  }
  
protected func UpdateTransferZone()
  {
  if (GetVisibility() && GetVisibility() != VIS_None) SetVisibility(VIS_None);
  MakeInvisible();
  SetShape(w/-2, h/-2, w,h); // h4x
  }
  
public func MakeInvisible()
  {
  SetVisibility(VIS_None);
  SetClrModulation();
  SetShape(-1,-1,2,2);
  SetObjDrawTransform(1000, 0, 0, 0, 1000, 0);
  }

// Clonksuche zum Entfernen des FoW
protected func CheckClonk()
{
  if(!iX) iX = GetX(FindObject(_RSS, 0, 0, -1, -1));
  if(!iY) iY = GetY(FindObject(_RSS, 0, 0, -1, -1));
  var o; while (o = FindObject(0, w/-2, h/-2, w, h, OCF_CrewMember, 0,0, NoContainer(), o))
    if (GetController(o) >= 0)
      {
      if(GetRespawn(0, GetOwner(o))==iX && GetRespawn(1, GetOwner(o))==iY) continue;
      SetRespawn(iX, iY, GetOwner(o));
      FindObject(_RSS, 0, 0, -1, -1)->Activate(o);
      }
}

// Debug-Funktionen
global func SPW_Show()
  {
  var o; while (o = FindObject(_RPA, 0,0,0,0, 0, 0,0, 0, o))
    if (GetAction(o) != "Placement")
      o->MakeVisible();
  }
  
global func SPW_Hide()
  {
  var o; while (o = FindObject(_RPA, 0,0,0,0, 0, 0,0, 0, o))
    if (GetAction(o) != "Placement")
      o->MakeInvisible();
  }
