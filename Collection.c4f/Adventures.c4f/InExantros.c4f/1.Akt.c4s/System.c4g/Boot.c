#strict
#appendto SLBS

private func Wind2Sail()
{
  // Kein Steuermann
  if (!FindObject(0,0,0,0,0,0,"Push",this()))
    SetComDir(COMD_None());
  // Geschwindigkeit und Bewegung
  var wind_push = BoundBy(100 / 4, -10, +10);
  var xdir = 9 + wind_push / 5; /* windabhängige X-Dir */
  if (GetComDir() == COMD_None())
    SetXDir(wind_push);
  if (GetComDir() == COMD_Left())
    SetXDir(-xdir);
  if (GetComDir() == COMD_Right())
    SetXDir(+xdir);
}