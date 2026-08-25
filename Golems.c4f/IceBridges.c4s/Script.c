#strict

protected func InitializePlayer(plr_num) {
  // Alle Zauberer (CrewMember) und...
  FillMagicEnergy(0, 0, 0, 0, 0, OCF_CrewMember(), 0, 0, 0, plr_num);
  // ...alle Zaubertürme starten mit voller Zauberenergie
  FillMagicEnergy(WZKP, 0, 0, 0, 0, 0, 0, 0, 0, plr_num);
}

protected func RemovePlayer(plr_num) {
  var flag;
  while (flag = FindObject(FLAG, 0, 0, 0, 0, 0, 0, 0, 0, flag))
    if (GetOwner(flag) == plr_num)
      SetOwner(-1, flag);
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