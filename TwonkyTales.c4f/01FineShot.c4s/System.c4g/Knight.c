#strict 2
#appendto KNIG
protected func ContextTalk(object actor)
{
  [$Talk$]
  return GameCall("TTConversation", this(), actor);
}
public func GetAimingDeviation(object bow)
{
  if (bow == GameCall("TTGetBow")) return 0;
  return _inherited(bow);
}
public func GetBowOut(object bow, int angle, bool deviate, &x, &y, &rotation, &xdir, &ydir, &rdir)
{
  var result = _inherited(bow, angle, deviate, x, y, rotation, xdir, ydir, rdir);
  if (bow == GameCall("TTGetBow")) { xdir *= 3; ydir *= 3; }
  return result;
}
