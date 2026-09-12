/* The bow remembers a journey, not the places its targets have moved to. */
#strict 2
#include ARRW
local tt_path, tt_return, tt_index, tt_seen_out, tt_seen_back, tt_spent;

public func PackTo() { return 0; }
public func ArrowStrength() { return 20; }
public func ArrowTumbleRandom() { return 1; }
public func ArrowStopsOnHit() { return 0; }
public func Returning() { return tt_return; }
public func PathLength() { return GetLength(tt_path); }

public func Launch()
{
  tt_path = [[GetX(), GetY()]];
  tt_seen_out = []; tt_seen_back = [];
  tt_return = false; tt_spent = false;
  return _inherited();
}

protected func InFlight()
{
  if (tt_return) return;
  if (!tt_path) tt_path = [];
  var count = GetLength(tt_path);
  if (count && !Contained())
    TraceTargets(tt_path[count-1][0], tt_path[count-1][1], GetX(), GetY(), false);
  if (!GBackSolid() && count < 2048)
    tt_path[count] = [GetX(), GetY()];
  return _inherited();
}

private func TraceTargets(int x1, int y1, int x2, int y2, bool back)
{
  var seen = tt_seen_out;
  if (back) seen = tt_seen_back;
  for (var target in FindObjects(Find_OnLine(x1-GetX(), y1-GetY(), x2-GetX(), y2-GetY()), Find_Exclude(this())))
  {
    var already = false;
    for (var previous in seen) if (previous == target) already = true;
    if (already) continue;
    if (GetID(target) == TTMN)
    {
      seen[GetLength(seen)] = target;
      GameCall("TTHit", target->Key(), this(), back);
    }
    else if (back && GetAlive(target))
    {
      seen[GetLength(seen)] = target;
      DoEnergy(-20, target, true, 0, GetOwner());
      target->~CatchBlow(-20, this());
    }
  }
  if (back) tt_seen_back = seen; else tt_seen_out = seen;
}

public func Recall()
{
  if (Contained() || tt_return || tt_spent || GetLength(tt_path) < 2) return false;
  // The last sampled point is outside terrain even when the arrow is embedded.
  RemoveEffect("IntStickInMaterial", this());
  SetVertex(2, 0, 0, this(), 1);
  SetVertex(2, 1, 0, this(), 1);
  tt_index = GetLength(tt_path) - 1;
  tt_return = true;
  SetAction("Idle"); SetSpeed(); SetRDir();
  SetCategory(C4D_StaticBack);
  SetPosition(tt_path[tt_index][0], tt_path[tt_index][1]);
  AddEffect("TTReturn", this(), 10, 1, this());
  return true;
}

public func FxTTReturnTimer(object target, int effect, int time)
{
  if (tt_index <= 0) return EndReturn(true);
  var next = tt_path[tt_index-1];
  var x = GetX(), y = GetY();
  var length = Max(1, Distance(x, y, next[0], next[1]));
  SetR(Angle(x, y, next[0], next[1]));
  // Inspect every pixel; a new wall cannot be skipped by a fast returning arrow.
  for (var step = 1; step <= length; ++step)
  {
    var nx = x + (next[0]-x)*step/length, ny = y + (next[1]-y)*step/length;
    TraceTargets(GetX(), GetY(), nx, ny, true);
    if (GBackSolid(nx-GetX(), ny-GetY())) return EndReturn(false);
    SetPosition(nx, ny);
  }
  --tt_index;
  return 1;
}

private func EndReturn(bool finished)
{
  tt_return = false;
  tt_spent = finished;
  // On obstruction, keep only the untravelled path. Clear the obstruction to retry.
  SetLength(tt_path, tt_index + 1);
  SetCategory(C4D_Object); SetSpeed();
  return -1;
}

protected func Entrance(object container)
{
  RemoveEffect("TTReturn", this());
  tt_return = false; tt_path = []; tt_spent = false;
  return true;
}

public func JoinPack(object container, bool force) { return false; }
public func Pack(object container) { return false; }
protected func Activate(object actor) { return false; }
