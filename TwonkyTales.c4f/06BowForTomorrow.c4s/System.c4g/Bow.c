#strict 2
#appendto BOW1
protected func ContextRecall(object actor)
{
  [$Recall$]
  return GameCall("TTRecall", actor);
}
