/*-- Wipfer --*/

#strict

protected func Hit()
{
  {
    var random=1+Random(3);
    if(random == 1) CastParticles("MSpark", 20,25, 0,0, 50, 45, RGB(Random(256), Random(256), Random(256)));
    if(random == 2) CastParticles("MSpark", 20,25, 0,0, 50, 45, RGB(Random(256), Random(256), Random(256)));
    if(random == 3) CastParticles("PxSpark", 20,25, 0,0, 50, 45, RGB(Random(256), Random(256), Random(256)));
    if(random == 4) CastParticles("PSpark", 20,25, 0,0, 50, 45, RGB(Random(256), Random(256), Random(256)));
  }
  return();
}

protected func Departure(thrower) {
  var proc = GetProcedure(thrower);
  var comd = GetComDir(thrower);
  if (GetPlrDownDouble(GetOwner(thrower)))
    if (proc ne "FLIGHT" || GetComDir(thrower) == COMD_None)
      return(0);
  
  if (proc eq "HANGLE" && comd == COMD_None)
    return(0);
  
  if (proc eq "SWIM" || proc eq "SCALE")
    return(0);

  var dir;
  if (comd == COMD_Left() || comd == COMD_Right()) {
    if (comd == COMD_Left())  dir = -1;
    if (comd == COMD_Right()) dir = +1;
  }
  else {
    if (GetDir(thrower) == DIR_Left()) dir = -1;
    else dir = +1;
  }

  var x_dir = dir * GetPhysical("Throw", 0, thrower)/1000 + GetXDir(thrower) / 3;
  var y_dir = -30;

  if (!x_dir) return(0);
  
  SetPosition(GetX() + x_dir / 5, GetY() - 2 + GetYDir(thrower) / 2);

  SetXDir(x_dir);
  SetYDir(y_dir);
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return(1); }

