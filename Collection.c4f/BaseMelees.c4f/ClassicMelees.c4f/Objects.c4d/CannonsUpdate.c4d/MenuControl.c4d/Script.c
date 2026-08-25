/*-- Menü-Überwachung --*/

#strict

local object;      // Objekt, das das Menü enthält
local target;      // Objekt, an das die Aufrufe gehen
local menuID;      // ID des Menüsymbols
local infocount;   // Anzahl an gespeicherten Informationen
                   // die Informationen werden in den nummerierten Locals gespeichert


/* Initialisierung */

protected func Initialize()
{
  // irgendwohin verschieben, wo es nicht stört
  SetPosition(0,0);
}

public func Init(par_obj, par_target, removeothers)
{
  object = par_obj;
  target = par_target;
  menuID = GetMenu(object);
  // andere Menü-Überwachungen für das gleiche Objekt löschen
  if (!removeothers) return(1);
  var obj;
  while (obj = FindObject(_MEC, 0,0,0,0, 0,0,0,0, obj))
    if (obj->GetTarget() == target)
      RemoveObject(obj);
}

public func GetTarget()
{
  return(target);
}

/* Timer-Call */

protected func Check()
{
  // Menü abgebrochen?
  if (GetMenu(object) != menuID)
  {
    ObjectCall(target, "ClearHelpers");
    return(RemoveObject());
  }
}