/*-- Brückensegment aus Metal --*/

#strict
#include BRDG

public func MaxDamage() {return(150);}

protected func Damage() {
  // Bei mehr als maximalem Schaden
  if (GetDamage() > MaxDamage())
  {
    // Bestandsmaterial schleudern
    var component_id = GetComponent(0, 0, 0, GetID());
    var component_num = GetComponent(component_id, 0, 0, GetID());
    CastObjects(component_id, component_num, 35);
    // Zerstören
    RemoveObject();
  }
}