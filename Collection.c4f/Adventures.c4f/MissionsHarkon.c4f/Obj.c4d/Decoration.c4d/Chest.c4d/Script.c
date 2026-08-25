/*-- Truhe --*/

#strict

local fLocked;
local pCorps;

// Öffnen und Schließen

protected func Initialize()
{
  SetAction("Closed");
  return(1);
}

protected func Open()
{
  if(pCorps)
  {
    RemoveClonk(pCorps);
  }
  if (GetAction() ne "Closed") return();
  // Truhe ist abgeschlossen
  if (fLocked)
  {
    Message("$MsgLocked$", this());
    Sound("Click");
    return();
  }
  SetAction("Opening");
}
  
protected func Close()
{
  if (GetAction() ne "Open") return();  
  SetAction("Closing");	    
}

protected func Grabbed(object pClonk, bool fGrab)
{
  // Die Truhe wurde gerade angefasst oder losgelassen
  CheckGrab();
}

protected func GrabLost(object pClonk)
{
  // Ein anfassender Clonk hat seinen Halt verloren
  CheckGrab();
}

// Status überprüfen, wenn die Truhe...
// * angefasst oder losgelassen wird
// * sich fertig geöffnet oder geschlossen hat
// * auf- oder zugeschlossen wurde
// * ein anfassender Clonk seinen Halt verloren hat
// * bekanntes Problem: wird ein Clonk durch Schaden weggeschleudert, bekommt die Truhe das nicht mit (es gibt kein GrabLost)

private func CheckGrab()
{
  // Die Truhe soll sich selbst öffnen, wenn (mindestens) ein Clonk sie anfasst...
  var pClonk;
  if (pClonk = FindObject(0,0,0,0,0,OCF_CrewMember(),"Push",this()))
  {
    if(pClonk->~IsCarryingCorps())
    {
      if(pCorps)
      {
        RemoveClonk(pCorps);
      }
      Enter(this, pCorps = pClonk->~GetCorps(1));
      AddClonk(pCorps);
//      FinishCommand(pClonk, 1);
//      pClonk->SetAction("Walk");
      return 1;
    }
    Open();
  }
  // ...und schließen, wenn nicht
  else
    Close();
}

// Auf- und zuschließen

public func Lock()
{
  // Bereits abgeschlossen
  if (fLocked) return(1);
  // Abschließen    
  fLocked = true;
  Sound("Click");  
  CheckGrab();
  return(1);
}

public func Unlock()
{
  // Nicht abgeschlossen
  if (!fLocked) return(1);
  // Aufschließen
  fLocked = false;
  Sound("Click");  
  CheckGrab();
  return(1);
}

protected func RejectCollect(id idObj, object pObj)
{
  // Abgeschlossen: Hineinlegen nicht möglich
  if (fLocked)
  {
    Message("$MsgLocked$", this());
    Sound("Error");
    return(true);
  }
  // Nicht abgeschlossen: Hineinlegen okay
  return(false);
}

protected func RejectContents()
{
  // Abgeschlossen: Herausnehmen nicht möglich
  if (fLocked)
  {
    Message("$MsgLocked$", this());
    Sound("Error");
    return(true);
  }
  // Nicht abgeschlossen: Herausnehmen okay
  return(false);
}

// Sound-Effekte

protected func Opening() 
{
  Sound("ChestOpen"); 
}
  
protected func Closing() 
{
  Sound("ChestClose");
}

func AddClonk(pObj)
{
  Sound("ChestOpen"); 
  var iDir = -1+2*GetDir();
  SetGraphics("Clonk", this, GetID(), 10, 1);
  SetGraphics(0, this, GetID(pObj), 5, 2, "GettingCarried");
  SetObjDrawTransform(1000*iDir, 0,1000*5*iDir, 0, 1000, 1000*6, this, 5);
  SetObjDrawTransform(-1000*iDir, 0, 0, 0, 1000, 0, this, 10);
  SetColorDw(pObj->GetColorDw());
}

func RemoveClonk(pObj)
{
  SetGraphics(0, this, 0, 10, 0);
  SetGraphics(0, this, 0, 5, 0);
  Exit(pCorps);
  pCorps = 0;
}

// Steuerung

protected func ControlDig(object pClonk)
{
  // Herausnehmen per Graben: Holen-Menü öffnen
  pClonk->SetCommand(0, "Get", this(), 0,0, 0, 1);
}
