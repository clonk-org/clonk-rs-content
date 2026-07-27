/*-- CPB1 Appendto --*/

#strict 2


#appendto CPB1

private func CheckTarget()
{
  // Anderes Bauteil im Hintergrund
  if (Var()=FindCastlePart(0, +1))
    if(GetID(Var())!=ROFC)
    return RemoveObject();
  // Zugbrücke im Hintergrund
  if (FindObject(CPBL, 0, -1) || FindObject(CPBR, 0, -1))
    return RemoveObject();
  // Wand ist zersört worden
  if (!GetActionTarget())
    return RemoveObject();
  if (!(GetActionTarget()->GetOCF() & OCF_Fullcon) || !GetActionTarget()->~CastlePartWidth())
    return RemoveObject();
  return 1;
}
