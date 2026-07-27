/*-- Klaue --*/

#strict 2

static const AFTF_Speed=40;

local pClonk;
local pTimer, iSpeed, iAngle;
local iDamage;

protected func Initialize() 
{
  // Geschwindigkeit setzen
  iSpeed=AFTF_Speed;
  iDamage = 18;
  return 1;
}

public func Activate(pCaster,iNewAngle)
{
  // Clonk und Winkel speichern
  pClonk=pCaster;
  iAngle=iNewAngle;
  
  // Geschwindigkeit passend setzen
  var iAdd=0;
  if(iAngle >= 0) iAdd=180;
  SetAction("Open");
  SetR(iAngle+90+iAdd);
  SetXDir(Cos(iAngle-90,AFTF_Speed));
  SetYDir(Sin(iAngle-90,AFTF_Speed));
 
  // Richtung anpassen
  if(iAngle < 0) { SetDir(DIR_Right); }
  else { SetDir(DIR_Left); }
}

protected func Check()
{
  // Die Zeit vorbei oder stehen geblieben
  if(++pTimer > 45 || iSpeed<=0)
  {
    // Ein letzer Effekt
    CastParticles("PSpark",10,10,0,0,60,160,RGB(150,150,240),RGB(210,210,255));
    RemoveObject();
  }

  // In Materie wird die Klaue langsamer
  if(GBackSolid())
  {
   iSpeed-=6;
   SetXDir(Cos(iAngle-90,iSpeed));
   SetYDir(Sin(iAngle-90,iSpeed));
  }

  if(!iDamage) return;

  // Clonks suchen
  for(var pClnk in FindObjects(Find_OCF(OCF_Alive),Find_Distance(23,0,0),Find_NoContainer()))
  {
    // Nur Feine, außer die Regel ist aktiviert
//    if(!ObjectCount(NTMG) && !Hostile(GetOwner(),GetOwner(pClnk))) continue;

    // Nicht in den ersten Frames den Zauberer selber treffen
    if(pClnk == pClonk && pTimer < 5) continue;

    // Clonk wegschleudern
    pClnk->SetAction("Tumble");
    SetPosition(GetX(pClnk),GetY(pClnk)-2,pClnk);
    SetXDir(GetXDir(pClnk)+RandomX(-10,10),pClnk);
    SetYDir(-20,pClnk);

    var iDmg = iSpeed/6;
    if(iDmg>iDamage) iDmg = iDamage;
    iDamage-=iDmg;
    // Und wehtun
    DoEnergy(-iDmg,pClnk);
  }
}
