/*-- Kleiner Ballon --*/

#strict

local pRope;
local alpha;
local pPully;

/* Keine Steuerung */

public func SetDirection(int comdir)
{
  if(comdir == COMD_Up || comdir == COMD_UpLeft || comdir == COMD_UpRight)
    SetComDir(COMD_Up);
  else if(comdir == COMD_Down || comdir == COMD_DownLeft || comdir == COMD_DownRight)
    SetComDir(COMD_Down);
  else
  {
    SetComDir(COMD_Stop);
  }
  CheckWindY();
}

/* Initialisierung */

func Initialize() {
 SetAction("Float");
 var pOwner=GetOwner();
 var pBaloon=this();
 pRope = CreateObject(CK5P);
 ScheduleCall(0,"Execute",1,0,pBaloon,pOwner);
 return(1);
 }

func Execute(pBaloon,pOwner) {
 // Seil bleibt beim Ballon
 if(Contained()) Enter(Contained(),pRope);
 pRope->NewConnect(0, this());
 pPully = GetActionTarget(1,pRope);
 //if(Contained() Enter(Contained(),pPully);
	
 // Alle Spieler überprüfen
 for(var iPlr=0;iPlr<GetPlayerCount();iPlr++)
  {
   if(!Hostile(iPlr,pOwner) || pOwner==-1)
     {
     var lgt=CreateObject(BVIW,0,0,iPlr);
     lgt->AttachTo(pBaloon);
     }
  }
 return(1);
}

/* Aktivität */

protected func Wind2Flag() { SetDir(BoundBy(GetWind()/10+4,0,8)); }

private func Wind2Float()
{
  //Seil noch dran? Nein, dann fliegt der Ballon davon
  if(GetActionTarget(0,pRope)!=this()) FadeMeOut();
  //Nach dem Wind drehen
  if(!GetContact(0, -1, 8)) SetXDir( BoundBy(GetWind()/4,-10,+10));
  CheckWindY();
  Wind2Flag();
  //Aufsteigen IMMER
  UpStream();
}

private func UpStream()
{
  SetDirection(COMD_Up);
}

private func CheckWindY()
{
  // Immer ein bisschen rauf- und runter geweht werden
  if(!GBackSolid(0, GetObjHeight()/2 + 2))
  {
    if(!GetEffect("IntWindYDir", this()))
      AddEffect("IntWindYDir", this(), 1, 10, this());
  }
  else
  {
    if(GetEffect("IntWindYDir", this()))
      RemoveEffect("IntWindYDir", this());
  }
}

protected func FxIntWindYDirTimer(object target, int number, int time)
{
  SetYDir(BoundBy(GetYDir() + RandomX(-GetYDir()) - 1 + Random(3), Max(GetYDir() - 1, -3), Min(GetYDir() + 1, 3)));
}

/* Trefferkontrolle */

protected func RejectCollect(id idObject, object pObj)
{
  // Pfeile vertragen sich nicht mit einem Ballon
  if (DefinitionCall(idObject,"IsArrow"))
   // Nur, wenn der Pfeil sich auch bewegt
   if(GetXDir(pObj) || GetYDir(pObj))
  {
    // bei brennendem Pfeil anzünden
    if(OnFire(pObj)) Incinerate();
    // ansonsten abstürzen
    else SetAction("DropOff");
    // in jedem Fall im Pfeil Hit auslösen (Sprengpfeil explodiert zB)
    ProtectedCall(pObj, "Hit");
    return(1);
  }
  return(1);
}

protected func Collection(object pObj)
  {
  var idObj = GetID(pObj);
  // bei brennendem Pfeil anzünden
  if(OnFire(pObj)) Incinerate();
  // ansonsten abstürzen
  else SetAction("DropOff");
  // in jedem Fall im Pfeil Hit auslösen (Sprengpfeil explodiert zB)
  ProtectedCall(pObj, "Hit");
  return(1);
  }

/* Externe Einwirkungen */

public func Poke()
{
  Sound("Rip");
  SetAction("DropOff");
}

/* Langsam verschwinden */
func FadeMeOut() {
  SetClrModulation(RGBa(255,255,255,alpha+=15));
	if(alpha==255) return(RemoveObject());
	return(1);
}