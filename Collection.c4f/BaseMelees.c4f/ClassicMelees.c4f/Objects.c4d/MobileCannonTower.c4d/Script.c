/*--- Mobiler_Geschützturm ---*/

#strict

local cannon;

#include CXEC
#include _CDR

local fSignalState;

protected func RejectCollect(id idObject, object pObj)
{
  // Bei allen Objekten Hit auslösen (Sprengpfeil / Flint explodiert zB) wenn sie sich bewegen und nicht enthalten sind.
  if(Contained(pObj)) return(0);
  if(GetXDir(pObj)>10 || GetYDir(pObj)>10) ProtectedCall(pObj, "Hit");
  return(1);
}

/* Lore suchen und Inhalt nehmen */

private func CloseEntrance()
{
  GrabContents(FindContents(LORY));
  return(_inherited() );
}

/* Kanonensteuerung */

protected func ContainedLeft(object pObj)
{
  [$TxtRotatecannontothelef$|Image=CT01:0]
  var pEngine = FindObject(0,0,0,0,0,OCF_Grab(),"Push",this());
  if(pEngine) return(ControlLeft(pObj));
  if(!cannon) return(0);
  Sound("Click");
  return(cannon->ComLeft(pObj) );
}

protected func ContainedLeftReleased(object pObj)
{
  if(!cannon) return(0);
  Sound("CannonStop");
  return(cannon->ComStop(pObj));
}

protected func ContainedRight(object pObj)
{
  [$TxtRotatecannontotherig$|Image=CT01:2]
  var pEngine = FindObject(0,0,0,0,0,OCF_Grab(),"Push",this());
  if(pEngine) return(ControlRight(pObj));
  if(!cannon) return(0);
  Sound("Click");
  return(cannon->ComRight(pObj) );
}

protected func ContainedRightReleased(object pObj)
{
  if(!cannon) return(0);
  Sound("CannonStop");
  return(cannon->ComStop(pObj));
}

protected func ContainedUp(object pObj)
{
  [$TxtStopcannonrotation$|Method=Classic]
  if(!cannon) return(0);

  if(!GetPlrCoreJumpAndRunControl(pObj->GetController()))
  {
    Sound("Click");
    Sound("CannonStop");
    return(cannon->ComStop(pObj) );
  }
}

protected func ContainedUpDouble(object pObj)
{
  [$TxtDisconnectcannon$|Image=CT01:3]
  if(!cannon) return(0);
  return(ReleaseCannon() );
}

protected func ContainedDig(object pObj)
{
  [$TxtFire$|Image=CT01:1]
  if (!cannon) return(0);
  return (cannon->ComFire(pObj));
}

/* Kanone anschrauben */

protected func Collection2(object pObj)
{
  if(pObj->~IsCannon() )
  {
    if(!cannon)
      ConnectCannon(pObj);
  }
  else
  {
    if(cannon)
      cannon->~ComEnter(pObj);
  }
}

public func ConnectCannon(object pCannon)
{
  // Wir rufen die Connect-Funktion der Kanone auf. Somit kann die Kanone
  // eigene Dinge tun um sich anzubauen.
  pCannon->Connect(this() );
  SetR(45, pCannon);
  if(GetX() > LandscapeWidth() / 2) SetR(-45, pCannon);
  
  // neue Kategorie für die Kanone um sie in den Hintergrund zu kriegen
  SetCategory(2,pCannon);
  SetObjectOrder(pCannon);

  // Die neue Kanone über enthaltene Objekte informieren
  var i = ContentsCount() - 1, obj;
  while(obj = Contents(--i) )
    if(!obj->~IsCannon() )
      pCannon->~ComEnter(obj);
  
  cannon = pCannon;
  Sound("Connect");
}

// Diese Funktion können die Kanonen aufrufen, um sich an den Geschützturm anzuschrauben,
// wenn es darum geht, ihre Definition zu ändern.
public func ConnectCannonDef(object pCannon, id defChg, string szAction)
{
  if(!szAction) szAction = "Attaching";
  ChangeDef(defChg, pCannon);
  ObjectSetAction(pCannon, szAction, this() ); 
}

// Löst die Kanone vom Geschützturm
public func ReleaseCannon()
{
  if(!cannon) return(0);
  Sound("Connect");
  cannon->ComStopDouble();
  // Kategorie für die Kanone wiederherstellen
  SetCategory(GetDefCategory(GetID(cannon)),cannon);
  cannon = 0;
  
  // Neue Kanonen suchen
  var i, obj;
  while(obj = Contents(i++) )
  {
    if(obj->~IsCannon() )
    {
      ConnectCannon(obj);
      break;
    }
  }
  return(1);
}

/* Wird von den Kanonen als Turm erkannt */

public func IsCannonTower()
{
  return(1);
}

public func HasCannon()
{
  return (cannon != 0); 
}

/* Zerstörung */

protected func Destruction()
{
  if(cannon) ReleaseCannon();
  return(_inherited() );
}

/* Abfackeln */

protected func Incineration()
{
  if(cannon) ReleaseCannon();
  ChangeDef(CTW1);
  return(_inherited() );
}

protected func IncinerationEx()
{
  if(cannon) ReleaseCannon();
  ChangeDef(CTW1);
  return(_inherited() );
}

/* Dampfmaschinensteuerung */

protected func ContainedLeftDouble(object clonk)  { [$TxtLeft$|Image=STM1:1]  return(ControlLeftDouble(clonk));       }
protected func ContainedRightDouble(object clonk) { [$TxtRight$|Image=STM1:2] return(ControlRightDouble(clonk));      }
protected func ContainedDown(object clonk)  { [$TxtStop$|Method=None]  return(ControlDownSingle(clonk)); }
protected func ContainedUpdate(object clonk, int comdir, bool dig, bool throw) { return(ControlUpdate(clonk, comdir, dig, throw)); }

protected func ControlLeft(object clonk)
{
  [$TxtLeft$|Image=STMS:1]
  var pEngine = FindObject(0,0,0,0,0,OCF_Grab(),"Push",this());
  if(!pEngine) return(0);
  pEngine->~ControlLeft(clonk);
  return(1);
}

protected func ControlRight(object clonk)
{
  [$TxtRight$|Image=STMS:2]
  var pEngine = FindObject(0,0,0,0,0,OCF_Grab(),"Push",this());
  if(!pEngine) return(0);
  pEngine->~ControlRight(clonk);
  return(1);
}

protected func ControlLeftDouble(object clonk)
{
  [$TxtLeft$|Image=STM1:1]
  var pEngine = FindObject(0,0,0,0,0,OCF_Grab(),"Push",this());
  if(!pEngine) return(0);
  pEngine->~ControlLeft(clonk);
  return(1);
}

protected func ControlRightDouble(object clonk)
{
  [$TxtRight$|Image=STM1:2]
  var pEngine = FindObject(0,0,0,0,0,OCF_Grab(),"Push",this());
  if(!pEngine) return(0);
  pEngine->~ControlRight(clonk);
  return(1);
}

protected func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
  var pEngine = FindObject(0, 0,0,0,0, OCF_Grab, "Push",this());
  if(pEngine) pEngine->~ControlUpdate(clonk, comdir, dig, throw);
}

protected func ControlDownSingle(object pCaller)
{
  [$TxtStop$|Method=None]
  var pEngine = FindObject(0,0,0,0,0,OCF_Grab(),"Push",this());
  // No steam engine or engine has already stopped
  if (!pEngine || GetComDir(pEngine) == COMD_Stop()) 
  {
    // Caller is inside us: exit
    if (Contained(pCaller) == this())
    {
      ActivateEntrance();
      AddCommand(pCaller, "Exit");
    }
  } 
  // Forward control to steam engine
  else 
  {
    pEngine->~ControlDownSingle(pCaller);
    return(1);
  }
return(1);
}
  
/* Signalsteuerung */

public func SignalCall(object pSignal,bool fLoad)
{
  // Bei fLoad = 1 einladen, Status sichern
  if(fSignalState = fLoad) return(SignalLoad());
  // sonst ausladen
  else return(SignalUnload());
}

// Einladen
private func SignalLoad()
{
  var pObj;
  // Gegenstände, Fahrzeuge suchen...
  while(pObj = FindObject(0,-11,-6,20,20,OCF_Collectible()|OCF_Grab(),0,0,NoContainer(),pObj))
    // ...Zugmaschine und Waggon nicht...
    if(GetID(pObj)!=STMG && GetID(pObj)!=MCTW)
      // ...einladen
      SetCommand(pObj,"Enter",this());
  return(1);
}

// Ausladen
private func SignalUnload()
{
  // Nur bei Inhalt
  if(!ContentsCount()) return();
  // Alles raus
  for(var pContents,i=0; pContents = Contents(i); ++i)
    SetCommand(pContents,"Exit");
  return(1);
}

public func SignalDepart(object pSignal)
{
  // Beim Ausladen nix
  if(!fSignalState) return(1);
  // Einladen stoppen
  var pObj;
  while(pObj = FindObject(0,-20,-20,40,40,OCF_Collectible()|OCF_Grab()|OCF_CrewMember(),0,0,NoContainer(),pObj))
    if(GetCommand(pObj)S="Enter")
      SetCommand(pObj, "None");
  fSignalState = 0;
  return(1);
}

/* Forschung */

public func GetResearchBase() { return(STMG); }
