/*-- Echse --*/

#strict 2

#include GRFN

public func GetAttackStrength() { return 16; }
public func GetBiteStrength() { return 22; }
public func GetMaxSpeed() { return 130; }
public func GetAcceleration() { return 350; }
public func GetAttackDelay() { return 100; }

public func AttackSound() { return Sound("Saurian"); }
public func ScreamSound() { return Sound("SaurianScream"); }

public func DeathConversion()
{
  // Verwandeln
  ChangeDef(SRND);
  SetPosition(GetX()+Sin(GetR(),23), GetY()+Cos(GetR(),23), this);
  SetAction("Dead");
  SetPhase(0);
  SetR(0);
  Call("ResetVertices");
  return 1;
}

private func UpdateRideVertex()
{
  var strAction = GetAction();
  var iPhase = GetPhase();
  var iDir = GetDir() * 2 - 1;
  
  // Wir müssen die Actions aus der ActMap der Echse, die teilweise nur
  // Ausschnitte der verschiedenen Basisaktionen sind, wieder auf die Basisaktion
  // inklusive der richtigen Basisphase mappen.
  if (strAction == "FlyFast") strAction = "Fly";
  if (strAction == "Glide2") strAction = "Glide";
  if (strAction == "TakeOff") { strAction = "Land"; iPhase = 10 - iPhase; }
  if (strAction == "FlyTurnBack") { strAction = "FlyTurn"; iPhase = 20 - iPhase; }
  if (strAction == "WalkTurnBack") { strAction = "WalkTurn"; iPhase = 20 - iPhase; }

  SetBaseRideVertex(strAction, iPhase, iDir);
  if(pRider) SetObjDrawTransform(1000, 0, 1000*GetVertex(0,0), 0, 1000, 1000*GetVertex(0,1), pRider);
}

private func Toogle()
{
 if(!Local())
	 SetAction("Glide");
 else SetAction("FlyBite");
 Local() = !Local();
}

private func SetBaseRideVertex(strAction, iPhase, iDir)
{
  var yAdjust=-10;
  var aVertsY, aVertsX;
  if (strAction == "Walk")
  {
    //         0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    aVertsY = [-5,-5,-6,-6,-7,-7,-6,-6,-5,-5, -5, -5, -5, -6, -6, -6, -6, -5, -5, -5, -5];
    SetVertexXY(0, 20*iDir, aVertsY[iPhase]);
    return 1;
  }
  if (strAction == "WalkTurn")
  {
    //           0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    aVertsX = [-16,-15,-13,-11,-11, -7, -4, -1,  1,  2,  2,  3,  4,  8, 12, 15, 16, 16, 17, 17, 17];
    aVertsY = [ -3, -3, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5];
    SetVertexXY(0, aVertsX[iPhase]*iDir, aVertsY[iPhase]);
    return 1;
  }
  if (strAction == "Attack")
  {
    //           0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    aVertsX = [ 22, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22];
    aVertsY = [ -4, -6,-10,-15,-18,-20,-20,-21,-21,-21,-21,-21,-20,-18,-17,-16,-13, -9, -6, -4, -4];
    SetVertexXY(0, aVertsX[iPhase]*iDir, aVertsY[iPhase]);
    return 1;
  }
  if (strAction == "Jump")
  {
    //           0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    aVertsX = [ 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 20, 20, 20, 20, 20, 20, 20, 20, 20];
    aVertsY = [ -6, -6, -5, -4, -3, -3, -3, -3, -3, -3, -4, -5, -5, -5, -6, -6, -6, -6, -5, -4, -4];
    SetVertexXY(0, aVertsX[iPhase]*iDir, aVertsY[iPhase]);
    return 1;
  }
  if (strAction == "Land")
  {
    SetVertexXY(0, 20*iDir, -4);
    return 1;
  }
  if (strAction == "Fly")
  {
    //           0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    aVertsX = [ 20, 20, 20, 20, 20, 20, 21, 22, 22, 22, 22, 22, 22, 22, 21, 21, 21, 20, 20, 20, 20];
    aVertsY = [ -5, -5, -5, -5, -6, -7, -7, -7, -7, -7, -7, -7, -6, -6, -6, -6, -6, -6, -6, -6, -6];
    SetVertexXY(0, aVertsX[iPhase]*iDir, aVertsY[iPhase]);
    return 1;
  }
  if (strAction == "FlyTurn")
  {
    //           0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    aVertsX = [-18,-15,-10, -7, -4, -3, -3, -2,  0,  2,  2,  5, 10, 15, 17, 20, 20, 20, 20, 20, 20];
    aVertsY = [ -3, -8,-12,-11,-10,-10,-10, -9, -9, -9, -9, -9, -9, -8, -8, -5, -5, -5, -4, -4, -4];
    SetVertexXY(0, aVertsX[iPhase]*iDir, aVertsY[iPhase]);
    return 1;
  }
  if (strAction == "FlyAttack")
  {
    //           0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    aVertsY = [ -4, -5, -6, -8,-10,-11,-12,-12,-12,-12,-12,-12,-12,-12,-12,-11,-10, -8, -6, -5, -5];
    SetVertexXY(0, 21*iDir, aVertsY[iPhase]);
    return 1;
  }
  if (strAction == "Glide")
  {
    SetVertexXY(0, 21*iDir, -4);
    return 1;
  }
  if (strAction == "FlyBite")
  {
    SetVertexXY(0, 21*iDir, -4);
    return 1;
  }
}

// Kann mit einem Ritual beschworen werden
public func IsRitual()   { return 1; }

// Nur für die Lichtkirche
public func NeedChurch() { return scBloodchurch; }

// Kann nicht in der Werkstatt produziert werden
public func RejectWorkshopConstruction() { return true; }
