/*-- Haubitze--*/

#strict 2
#appendto CANN

public func Fire(bool fAuto)
{
  // Schieﬂpulver und Projektil suchen
  var pProjectile=FindOtherContents(GUNP);
  // Projektil fehlt
  if (!pProjectile) 
  {
    Sound("Click");
    return 0;
  }

  // Austritt berechnen
  var iX = (GetPhase()+3)*(GetDir()*2-1);
  var iY = GetPhase()-14;
  var iAngle = BoundBy(GetPhase()*5-5,0,90);
  var iXDir = Sin(iAngle,32*(GetDir()*2-1));
  var iYDir = Cos(iAngle,-18);

  // Projektil abfeuern
  Exit(pProjectile,iX,iY,Random(360),iXDir,iYDir,+30);

  // Sicht verfolgen (wenn kein automatischer Schuss)
  if(!fAuto)
    if(GetPlrViewMode(GetController())!=2)
      SetPlrView(GetController(),pProjectile);
  // Sound
  Sound("Blast2");
  // Rauch
  for (var i = 0; i < 6; ++i) 
    Smoke(iX+RandomX(-5,+5),iY+RandomX(-5,+5),RandomX(5,12));
  // Erfolgreich geschossen
  return 1;
}
