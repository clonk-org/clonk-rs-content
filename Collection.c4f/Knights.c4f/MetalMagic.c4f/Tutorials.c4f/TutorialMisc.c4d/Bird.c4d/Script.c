/*-- Vogel --*/

#strict 2

#include ANIM

local Bait; // Verfolgter Köder
local pNest;
local pFix;

private func ReproductionRate()     { return 2000; }  // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet

public func IsPossessible() { return 1; }

/* Initialisierung */

protected func Initialize()
{
  SetAction("Fly");
  SetClrModulation(RGBa(255,255,255,255));
  SetGraphics(0, this, GetID(), 2, 6);
  SetColorDw(HSL(Random(255),255,128+RandomX(20,-20)), this);
  if (Random(2)) return SetComDir(COMD_Right);
  return SetComDir(COMD_Left);
}

public func AddItem(pObj)
{
  Enter(this, pObj);
  Contents()->SetColorDw(GetColorDw());
  SetGraphics(0, this, GetID(Contents()), 1,1);
  AddEffect("Item", this, 1, 1, this);
}

/* TimerCall mit KI-Steuerung */

protected func Activity()
{		
  // Geräusche machen
  if (!Random(25)) Sound("Raven*");

  // Special Vogel?
  if(pFix)
  {
    if(ObjectDistance(pFix)>100)
    {
      SetCommand(this, "MoveTo", pFix);
    }
    if(GetCommand() && ObjectDistance(pFix)<50)
      SetCommand(this, "None");
  }

  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this)) return;
  if(Contained()) return;
  if(GetCommand()) return;

  // Du wolltes dich doch vermehren!
  if(FindContents(BIRD)) Reproduction();
  
  // Köder fressen
  if(Bait)
    if(ObjectDistance(Bait, this) <= 25)
      if(GetAction() == "Fly")
        SetAction("Eat");

  var pBait;
  // Nur wenn der Vogel ausgewachsen ist
  if (GetCon()==100) 
   // Soll nicht zu einfach sein...
   if (!Random(5))
     // Ein Köder in der Nähe?
     while (pBait = FindObject( 0, -250,-250,500,500, 0,0,0,NoContainer(), pBait))
      // Köder?
      if(WildcardMatch(GetAction(pBait), "*MeatBait*"))
       // Je nach Köderqualität...
       if (Random(100) < pBait->~IsBait())
        {
         // ...hinfliegen
         SetCommand(this, "Follow", pBait);
         // Und Köder merken
         Bait = pBait;
        }

  // Nichts machen
  if (Random(2) || GetAction() != "Fly") return 1;

  // Nach oben oder unten fliegen
  SetComDir(COMD_Up);
  if (Random(2)) SetComDir(COMD_Down);

  // Nichts machen
  if (!Random(4)) return 1;
  
  // Ein Ei legen
  if (!Random(ReproductionRate())) 
   Reproduction();
  
  // Umdrehen
  if (Random(2)) return TurnRight();
  return TurnLeft();
}

// Nur im Fliegen
private func SpecialReprodCond()
{	
  if(Contained()) return;
  if(GetCommand()) return;
  return GetAction()=="Fly";
}

private func SpecialRepr()
{
//  if(FindContents(BIRD)) return 1;
	
  // Wenn das Nest nicht mehr am Baum hängt wird es verworfen
  if(pNest) if(GetAction(pNest) != "Be") pNest = 0;

  // Kein Nest da? Dann schnell eines bauenn
  if(!pNest)
  {
    var pTree = FindTree();	  
    if(pTree)
    {
      var id = GetID(pTree);	    
      SetCommand(this, "Call", this, pTree, 0, 0, "BuildNest");
      // Schönes Plätzchen in der Baumkrone suchen
      AddCommand(this, "MoveTo", 0, GetX(pTree)+GetDefOffset(id)+Random(GetDefWidth(id)-20)+10, GetY(pTree)+GetDefOffset(id, 1)+Random(GetDefHeight(id)-GetDefFireTop(id)-20)+10);
    }
    // Damit das Vermehrungssteuerungsobjekt schon den neuen Vogel sieht
    // und nicht ständig neue Nester bauen lässt
    if(!FindContents(BIRD)) CreateContents(BIRD);
    return 1;
  }
  
  // Manchmal im Nest ein Ei legen
  if(Random(100) && pNest && !FindObject(BEGG, 0, 0, 0, 0, 0, 0, pNest))
  {
    // Damit das Vermehrungssteuerungsobjekt schon den neuen Vogel sieht
    // und nicht ständig neue Nester bauen lässt
    if(!FindContents(BIRD)) CreateContents(BIRD);
    FlyToNest();	  
  }
  return 1;
}

private func FindTree()
{
   var obj, pTree;
   while(obj = FindObject(0, 0, 0, -1, -1, OCF_Chop, 0, 0, 0, obj))
   {
      if(ObjectDistance(obj)>500) return 0;
      if(OnFire(obj)) continue;
      if(GetCategory(obj) & C4D_Vehicle) continue;
      if(GetCon(obj)<100) continue;
      return obj;
   }
}

public func BuildNest(pThis, pTree)
{
  if(!pTree) return 0;
  if(GetX(pTree)<GetX()) SetDir(0);
  else SetDir(1);
  pNest = CreateConstruction(NEST, -12+24*GetDir(), +5, -1, 50);
  LocalN("pTree", pNest) = pTree;
  ObjectSetAction(pNest, "Be", pTree);
  SetAction("BuildNest");
  SetComDir(COMD_Stop);
  SetXDir(0);
  SetYDir(0);
}

protected func Build()
{
  // Nest bauen	
  if(!pNest || OnFire(pNest)) { SetAction("Fly"); return Activity(); }
  DoCon(10, pNest);
  if(GetCon(pNest)>=100) { SetAction("Fly"); FlyToNest(); }
}

private func FlyToNest()
{
  // Ins Nest fliegen und dort das Brüten anfangen	
  SetCommand(this, "Call", this, 0, 0, 0, "StartBreeding");
//  AddCommand(this, "Enter", pNest);
  AddCommand(this, "MoveTo", pNest); 
}

protected func StartBreeding()
{
  Enter(pNest);
  // Nest benachrichtigen	
  ObjectSetAction(pNest, "Bird");	
}

protected func LayEgg()
{
  // Ein Ei legen
  var pEgg = CreateObject(BEGG);
  pEgg->SetAction("Nest", pNest);
  SetPosition(GetX(pNest), GetY(pNest)-1, pEgg);
  if(FindContents(BIRD))
    Enter(pEgg, FindContents(BIRD));
  Exit();
  SetAction("Fly");
  Activity();
}

/* Geburt */

public func Birth()
{
  SetAction("Fly");
  if (Random(2)) SetComDir(COMD_Left);
  else SetComDir(COMD_Right);
  return 1;
}

protected func HitCheck()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this)) return;

  // Einen Ballon oder ein Luftschiff zum attackieren suchen
  return Attack();
}

public func Attack(object pAttack)
{
  // Nur während des Fliegens kann gebissen werden
  if (GetAction() != "Fly") return;

  // Einen Ballon oder ein Luftschiff suchen
  if (!pAttack) pAttack = FindObject(BALN, -20,0,40,30, 0, 0,0, NoContainer());
  if (!pAttack) pAttack = FindObject(BLMP, -30,0,60,40, 0, 0,0, NoContainer());
  if (!pAttack) return;

  // Reinbeißen!
  pAttack->~Poke();
  SetAction("Attack");
  Sound("Raven*");
}

protected func Survive()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this)) return;

  // Nicht in Flüssigkeiten oder brennende Objekte fliegen
  if (InLiquid() || GBackLiquid(GetXDir()*2, GetYDir()*2) || GBackLiquid(GetXDir()*3, GetYDir()*3) || FindObject(0, -20 + GetXDir() * 2, -20 + GetYDir() * 2, 40, 40, OCF_OnFire))
   SetComDir(COMD_Up);
}

/* Kontakte */

protected func ContactLeft()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this)) return;

  return TurnRight();
}

protected func ContactRight()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this)) return;

  return TurnLeft();
}

protected func ContactTop()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this)) return;

  return SetComDir(COMD_Down);
}

protected func ContactBottom()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this)) return;

  return SetComDir(COMD_Up);
}

/* Aktionen */

public func TurnRight()
{
  if (Stuck() || GetAction() != "Fly") return;
  if (GetXDir() < 0) SetXDir(0);
  SetDir(DIR_Right);
  SetComDir(COMD_Right); 
  return 1;
}

public func TurnLeft()
{
  if (Stuck() || GetAction() != "Fly") return;
  if (GetXDir() > 0) SetXDir(0);
  SetDir(DIR_Left);
  SetComDir(COMD_Left); 
  return 1;
}

/* Einwirkungen */

protected func CatchBlow(iLevel, pObject)
{
  if (pObject->~IsArrow()) SetAction("Tumble");
  if (!Random(3)) Sound("RavenHurt");
  return 1;
}

protected func Death()
{
  SetClrModulation();
  if(Contents()) Exit(Contents());
  SetGraphics(0, this, 0, 1);
  SetGraphics(0, this, 0, 2);
  ChangeDef(DBRD);
  SetAction("Dead");
  return 1;
}

/* Steuerung durch Besessenheit */

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 // Bewegungskommando
 if (szCommand == "MoveTo")
  return SetCommand(this,szCommand, pTarget, iTx, iTy);
 return 0;
}

protected func ContainedLeft(object caller)
{
  [$TxtMovement$]
  SetCommand(this, "None");
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
    TurnLeft();
  return 1;
}

protected func ContainedRight(object caller)
{
  [$TxtMovement$]
  SetCommand(this, "None");
  if(!GetPlrCoreJumpAndRunControl(caller->GetController())) 
    TurnRight();
  return 1;
}

protected func ContainedUp(object caller)
{
  [$TxtMovement$]
  SetCommand(this, "None");
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
    SetComDir(COMD_Up);
  return 1;
}

protected func ContainedDown(object caller)
{
  [$TxtMovement$]
  SetCommand(this, "None");
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
    SetComDir(COMD_Down);
  return 1;
}

/* JumpAndRun-Steuerung */

private func ClearDir(bool fX)
{
  if(fX && GetXDir())
  {
    if(GetXDir() > 0) SetXDir(Max(GetXDir() - 2, 0));
    else SetXDir(Min(GetXDir() + 2, 0));
  }
  if(!fX && GetYDir())
  {
    if(GetYDir() > 0) SetYDir(Max(GetYDir() - 2, 0));
    else SetYDir(Min(GetYDir() + 2, 0));
  }
}

protected func ContainedUpdate(object controller, int comdir)
{
  SetComDir(comdir);
  ClearScheduleCall(this, "ClearDir");
  if(comdir == COMD_Up || comdir == COMD_Down || comdir == COMD_Stop)
    ScheduleCall(this, "ClearDir", 1, (Abs(GetXDir())+1)/2, true);
  if(comdir == COMD_Left || comdir == COMD_Right || comdir == COMD_Stop)
    ScheduleCall(this, "ClearDir", 1, (Abs(GetYDir())+1)/2, false);

  if(comdir == COMD_UpRight || comdir == COMD_Right || comdir == COMD_DownRight)
    SetDir(DIR_Right);
  if(comdir == COMD_UpLeft || comdir == COMD_Left || comdir == COMD_DownLeft)
    SetDir(DIR_Left);

  return 1;
}

protected func ContainedDig()
{
 [$TxtAttack$]
 Attack();
 return 1;
}

protected func ContainedThrow() { return 1; }

protected func ContainedDigDouble()
{
 [$TxtLeave$]
 RemoveEffect("PossessionSpell", this);
 return 1;
}

/* Köder fressen */

protected func Eating()
{
  // Köder weg
  if(!Bait) return;
  // Köder fressen
  RemoveObject(Bait);
  // Schmeckt gut
  Sound("Raven*");
}

public func DoSetItemXY(iX, iY, iPhase)
{
  SetPhase(iPhase);
  SetItemXY(iX, iY);
}

public func SetItemXY(iX, iY)
{
  var iDir = -1+2*GetDir();
  SetObjDrawTransform(1000, 0, iX*1000*iDir, 0, 1000, iY*1000, this, 1);
}

public func FxItemTimer(pTarget, iNumber)
{
  var aVertsX, aVertsY;
  var iPhase = GetPhase();
  if(GetAction()=="Fly")
  {
    //           0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    aVertsX = [ -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -6, -5, -5, -5, -6, -6, -7, -7];
    aVertsY = [  5,  6,  7,  8,  9,  9,  9,  8,  7,  6,  5,  4,  4,  4,  3,  2,  3,  3,  4,  5,  5];
    SetItemXY(aVertsX[iPhase], aVertsY[iPhase]);
  }
  if(GetAction()=="Turn")
  {
    //           0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    aVertsX = [  7,  7,  6,  5,  4,  2,  0, -2, -4, -7, -8, -9, -9, -8, -6, -5, -5, -6, -6, -7, -7];
    aVertsY = [  5,  5,  6,  8,  8,  8,  8,  8,  8,  8,  8,  8,  6,  5,  4,  3,  3,  3,  4,  5,  5];
    SetItemXY(aVertsX[iPhase], aVertsY[iPhase]);
  }
}
