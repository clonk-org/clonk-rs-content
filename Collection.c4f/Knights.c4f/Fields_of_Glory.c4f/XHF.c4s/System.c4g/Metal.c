/*-- Metall --*/

#strict
#appendto METL

private func RuleNoStructure(clonk) {
  if (FindObject(NMTT)) {
    var obj;
    while (obj = FindObject(0, 0, 1, 0, 0, 0, 0, 0, NoContainer(), obj))
      if (GetCategory(obj) & (C4D_Structure() || C4D_StaticBack()))
        if (Hostile(GetOwner(obj), GetOwner(clonk))) {
          Message("$TxtNobridgeallowedhere$", this());
          Sound("Error");
          return(1);
        }
  }
  return(0);
}