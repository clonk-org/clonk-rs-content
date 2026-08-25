#strict

protected func Initialize() {
  // Steine erzeugen
  var num;
  num = RandomX(20,25); while(--num) PlaceInMaterial(RO13, Material("Earth"));
  num = RandomX(15,20); while(--num) PlaceInMaterial(ER13, Material("Earth"));
  num = RandomX(10,15); while(--num) PlaceVegetation(RO13, 0, 0, LandscapeWidth(), LandscapeHeight(), 100000);
  num = RandomX( 5,10); while(--num) PlaceVegetation(ER13, 0, 0, LandscapeWidth(), LandscapeHeight(), 100000);
  num = RandomX(20,25); while(--num) PlaceStalagtit();
  ScriptGo(1);
}

protected func PlaceStalagtit() {
  var id = _STA;
  var x = Random(LandscapeWidth());
  var y = Random(LandscapeHeight());
  while (GBackSolid(x, y)) {
    x = Random(LandscapeWidth());
    y = Random(LandscapeHeight());
  }
  while (!GBackSolid(x, y) && y > 0) y -= 3;
  if (!GBackSolid(x, y)) return(0);
  CreateObject(id, x, y + 35, -1);
  return(1);
}

public func Script400() {
  var zap = PlaceInMaterial(ZAP1, Material("Tunnel"));
  CastObjects(ZAP1, RandomX(3, 4), 10, GetX(zap), GetY(zap));
  goto(0);
}

public func InitializePlayer(plr_num) {
  // Nach der ersten Spielminute keine Beitritte mehr
  if (FrameCounter() > 60 * 30) EliminatePlayer(plr_num);
  
  // Alle Zauberer (CrewMember) und...
  FillMagicEnergy(0, 0, 0, 0, 0, OCF_CrewMember(), 0, 0, 0, plr_num);
  // ...alle Zaubertürme starten mit voller Zauberenergie
  FillMagicEnergy(WZKP, 0, 0, 0, 0, 0, 0, 0, 0, plr_num);
  
  var tower = FindObjectOwner(WZKP, plr_num % 2);
  for (var crew_num = 0; crew_num < GetCrewCount(plr_num); crew_num++)
    Enter(tower, GetCrew(plr_num, crew_num));

  if (plr_num == 1) {
    var enemy_tower = FindObjectOwner(WZKP, 0);
    Var(0) = LandscapeWidth() - GetX(enemy_tower);
    Var(1) = GetY(enemy_tower);
    if (!FindConstructionSite(WZKP, 0, 1)) {
      Var(0) += RandomX(-200, +200);
      DigFree(Var(0), Var(1), 50);
    } else {
      Var(1) -= 41;
    }

    var basement = LocalN("basement", tower);
    var offx = GetX(tower) - GetX(basement);
    var offy = GetY(tower) - GetY(basement);
    SetPosition(Var(0), Var(1), tower);
    SetPosition(Var(0) - offx, Var(1) - offy, basement);    
  }
}

protected func FillMagicEnergy(id, x, y, wdt, hgt, ocf, action, act_target, container, owner) {
  // Alle passende Objekte durchgehen
  var fill_obj;
  while (fill_obj = FindObject(id, x, y, wdt, hgt, ocf, action, act_target, container, fill_obj)) {
    if (GetOwner(fill_obj) == owner) {
      // Auffüllen
      while (DoMagicEnergy(1, fill_obj)) {};
      DoMagicEnergy(1, fill_obj);
    }
  }
}

// Vulkan soll DuroLava verwenden
public func VolcanoMaterial() { return(Material("DuroLava")); }