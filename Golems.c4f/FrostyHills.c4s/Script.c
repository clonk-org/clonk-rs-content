#strict

protected func Initialize() {
  SetSkyParallax(0, 20, 20);
  for (var pl_num = 0; pl_num < 25; pl_num++) PlaceInMaterial(GOLD, Material("Gold"));
  for (var pl_num = 0; pl_num < 3; pl_num++) PlaceInMaterial(BARL, Material("Lava"));
  for (var pl_num = 0; pl_num < 5; pl_num++) PlaceInMaterial(SFLN, Material("Crystal"));
  for (var pl_num = 0; pl_num < 3; pl_num++) PlaceInMaterial(EFLN, Material("Crystal"));
  for (var pl_num = 0; pl_num < 20; pl_num++) PlaceInMaterial(CNCR, Material("Rock"));
  for (var pl_num = 0; pl_num < 15; pl_num++) PlaceInMaterial(LIQC, Material("Granite"));
  for (var pl_num = 0; pl_num < 2; pl_num++) PlaceInMaterial(DFSH, Material("Ice"));
  for (var pl_num = 0; pl_num < 1; pl_num++) PlaceInMaterial(DSHK, Material("Ice"));
  for (var pl_num = 0; pl_num < 7; pl_num++) SetR(Random(360), PlaceInMaterial(BARL, Material("Earth")));
  for (var pl_num = 0; pl_num < 2; pl_num++) SetR(Random(360), PlaceInMaterial(LORY, Material("Earth")));    
  for (var pl_num = 0; pl_num < 7; pl_num++) SetR(Random(360), PlaceInMaterial(WOOD, Material("Snow")));
  for (var pl_num = 0; pl_num < 8; pl_num++) SetR(Random(360), PlaceInMaterial(BONE, Material("DuroLava")));
  for (var pl_num = 0; pl_num < 3; pl_num++) PlaceInMaterial(SKUL, Material("DuroLava"));    
  for (var pl_num = 0; pl_num < 3; pl_num++) PlaceVegetation(IDOL, Random(LandscapeWidth()));
  for (var pl_num = 0; pl_num < 2; pl_num++) PlaceVegetation(GIDL, Random(LandscapeWidth()));  
  for (var pl_num = 0; pl_num < 3; pl_num++) PlaceVegetation(TWR2, Random(LandscapeWidth()));
  PlaceInMaterial(SUBB, Material("DuroLava"));
}

protected func InitializePlayer(plr_num) {
  // Alle Zauberer (CrewMember) und...
  FillMagicEnergy(0, 0, 0, 0, 0, OCF_CrewMember(), 0, 0, 0, plr_num);
  // ...alle Zaubertürme starten mit voller Zauberenergie
  FillMagicEnergy(WZKP, 0, 0, 0, 0, 0, 0, 0, 0, plr_num);
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

public func ScriptAlgoDots(x, y, alpha, beta) {
  return(Random(100) < alpha);
}