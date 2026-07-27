/*-- Magus Append --*/

#strict 2
#include MAGE
#appendto MAGE

local pStaff;
local pStaffPict;
local bNoStaff;
local fInitialized;

public func Initialize()
{
  // Falls der Magier weniger als den Minimalwert der Definition hat,
  // wird dieser angepasst
  if(GetPhysical("Magic") < GetDefCoreVal("Magic", "Physical", GetID()))
    SetPhysical("Magic", GetDefCoreVal("Magic", "Physical", GetID()));
  // Magus bekommt einen Stab geschenkt, aber nur einmal
  if(ObjectCount(FSTF) && !fInitialized)
   if(!FindContents(SCKZ)) CreateContents(SCKZ);
  fInitialized = 1;
  return _inherited();
}

public func ContextMagic(object pByObject)
{
  [$CtxMagicDesc$|Image=MCMS|Condition=NoComboReadyToMagic]
  // Nicht bereit
  if (!ReadyToMagic()) { DoCastError(Format("$MsgNotReadyToMagic$", GetName())); return 0; }
  // Clonk soll anhalten
  SetComDir(COMD_Stop, pByObject);
  // Menü öffnen
  var iExtra;
  if(ObjectCount(ALCO)) { if(ObjectCount(NMGE)) iExtra=1; else iExtra=5; }
  else                  { if(ObjectCount(NMGE)) iExtra=0; else iExtra=3; }
  CreateMenu(MCMS, 0, 0, iExtra, "$MnuNoSpells$", GetMagicEnergy());
  var idMagic, i = 0, iStaffSpellCount;
  // Stabsprüche
  if(HasStaff())
  while (idMagic = pStaff->~GetSpell(i++))
  {
    AddMenuItem("$MnuCast$: %s", "DoMagic", idMagic);
    if(idLastSpell == idMagic) SelectMenuItem(i-1);
    iStaffSpellCount++;
  }
  i = 0;
  // Normale Zauber
  while (idMagic = GetPlrMagic(GetOwner(), 0, i++)) 
  {
    AddMenuItem("$MnuCast$: %s", "DoMagic", idMagic);
    if(idLastSpell == idMagic) SelectMenuItem(i-1+iStaffSpellCount);
  }
  return 1;
}

protected func ControlSpecial(object pCaller, bool fNoStaffMenu)
{
  [$CtrlQuickspellDesc$|Image=MCMQ]
  // Stab? Stabzauber starten
  if(HasStaff() && !fNoStaffMenu)
  {
    // Combo oder normales Menü
    if(iCombo)
      return DoStaffCasting();
    else return SfaffMenu();
  }
  return _inherited(pCaller);
}

public func MaxQuickspells()
{
  if(pStaff) return 5;
  return _inherited(...);
}

public func SfaffMenu()
{
  // Nicht bereit
  if (!ReadyToMagic()) { DoCastError(Format("$MsgNotReadyToMagic$", GetName())); return 0; }
  // Menü öffnen
  var iExtra;
  if(ObjectCount(ALCO)) { if(ObjectCount(NMGE)) iExtra=1; else iExtra=5; }
  else                  { if(ObjectCount(NMGE)) iExtra=0; else iExtra=3; }
  CreateMenu(MCMS, 0, 0, iExtra, "$MnuNoSpells$", GetMagicEnergy());
  // Normales Schnellzaubermenü
  for(var i=0; i < MaxQuickspells(); ++i)
  {
    var idData = GetPlrExtraData(GetOwner(),Format("%s%d",MCLK_QSPrefix,i));
    if(IsQSID(idData))
      AddMenuItem("$MnuSlotQuickspell$: %s", "DoMagic", idData);
    else
      AddMenuItem("$MnuSlotQuickspell$: $MnuEmpty$", "Nothing", eval(Format("MCM%d", i%5+1)));
  }
  // in die Mitte
  SelectMenuItem(MaxQuickspells()/2);
  var idMagic, i = 0;
  // Alle Stabzauber einfüllen
  var iCount;
  while(pStaff->~GetSpell(i++)) iCount++;
  i = 0;
  while (idMagic = pStaff->~GetSpell(i++))
  {
    if( i == iCount-Mod(iCount,5)+1) {
      for(var j=0; j<(5-Mod(iCount,5))/2; j++)
        AddMenuItem("", "",FXU1);
    }
    AddMenuItem("$MnuCast$: %s", "DoMagic", idMagic);
  }
  return 1;
}

protected func DoStaffCasting()
{
  // Schon beim Zaubern? Abbrechen
  if (IsCasting()) return EndMagicAction();
  // Bereit?
  if (!ReadyToMagic()) { DoCastError(Format("$MsgNotReadyToMagic$", GetName())); return 0; }
  pComboMenu = CreateSpellMenu(this, this, GetID(pStaff), iCombo==2);
  if (!pComboMenu) return;
  // Clonk soll anhalten
  SetComDir(COMD_Stop);
  // Zauberaktivität setzen
  SetCastAction();
  // Auswahlmenü zurückgeben
  return pComboMenu;
}

protected func OnComboMenuSpecial(pMenu)
{
  var id = LocalN("idSelectedElement", pMenu);
  // Wenn wir einen Stab haben, nach dem Stabmenü das andere Menü anzeigen
  if(id) if(id->~IsStaff())
  {
    pMenu->Close();
//    ScheduleCall(this, "DoComboCasting", 1, iCombo==2);
    DoComboCasting(iCombo==2);
  }
}

public func RejectCollect(idObject, pObject)
{
  // Wenn noch kein Stab da, auf jeden Fall einpacken
  if(!HasStaff() && idObject->~IsStaff())
    return 0;
  // Keinen zweiten Stab aufnehmen
  if(HasStaff() && idObject->~IsStaff())
    return 1;
  // Ansonsten ist alles erlaubt
  return _inherited(idObject, pObject);
}

public func ControlThrow()
{
  // Wenn der Clonk einen Stab hat...
  if(HasStaff() && !Contents())
  // ...und wenn er den Stab gerade werfen kann
  if(GetAction()=="Walk" || GetAction()=="Ride" || GetAction()=="RideStill"
  || GetAction()=="Scale" || GetAction()=="ScaleDown" || GetAction()=="Hangle" || GetAction()=="Swim"
  || (GetAction()=="Jump" && GetPlrDownDouble(GetOwner())) || (GetAction()=="Tumble" && GetPlrDownDouble(GetOwner())))
  {
    // Beim Materialjump Stab einen Zurückkomm-Effekt geben
    if((GetAction()=="Jump" && GetPlrDownDouble(GetOwner())) || (GetAction()=="Tumble" && GetPlrDownDouble(GetOwner())))
      ScheduleCall(pStaff, "DepatureX", 15, 1, this);
    // Nur ablegen, wenn es die Regel zulässt
    else if(FindObject(FXST) && GetPlrDownDouble(GetOwner())) return _inherited();
    // Stab im Inventar erzeugen
    bNoStaff = 1;
    Enter(this, pStaff);
    bNoStaff = 0;
    pStaff = 0;
    SetGraphics(0, this, 0, 0);
    SetGraphics(0, this, 0, 1);
    if(pStaffPict) RemoveObject(pStaffPict);
  }
  return _inherited();
}

protected func Death(a,b,c,d,e,f)
{
  // Beim Tod den Stab ausgeben
  if(HasStaff())
  {
    bNoStaff = 1;
    if(FindObject(FXST)) RemoveObject(pStaff);
    else Enter(this, pStaff);
    pStaff = 0;
    SetGraphics(0, this, 0, 0);
    SetGraphics(0, this, 0, 1);
    if(pStaffPict) RemoveObject(pStaffPict);
  }
  return _inherited(a,b,c,d,e,f);
}

public func Collection2(object pObj)
{
  if(bNoStaff) return 0;
  // Stäbe werden beim einsammeln "geschluckt"
  if(pObj->~IsStaff())
  {     
    // Der Stab wird aufgenomen und ein Bild für das Inventar gesetzt
    pStaff = pObj;
    pStaffPict = CreateObject(MGST, 0, 0, GetOwner());
    pStaffPict->SetStaff(pStaff, this);
    SetGraphics("NoStaff", this, AMGE);
    SetGraphics("Wear", this, GetID(pStaff), 1, GFXOV_MODE_ExtraGraphics);
    return;
  }
  // Stabbild anpassen
  if(pStaffPict) SetPosition(60,-25, pStaffPict);
}

public func HasStaff()
{
  return pStaff;
}

// "Auswerfen" des Stabes
protected func EjectStaff()
{
  // Hat keinen Stab?
  if(!HasStaff()) return false;
  // Stab erschaffen
  bNoStaff = 1;
  Exit(pStaff);
  Collect(pStaff,this);
  bNoStaff = 0;
  pStaff = 0;
  SetGraphics(0, this, 0, 0);
  SetGraphics(0, this, 0, 1);
  if(pStaffPict) RemoveObject(pStaffPict);
}

// Der Magier kann den Stab verwenden
public func CanUse(idObj)
{
  if(idObj == SCKZ) return true;
 
  return false;
}

// Damit Aufwertung mit einem Stab in der Hand funktioniert
public func Redefine2(p1,p2,p3,p4)
{
  if(!FindObject(FXST)) EjectStaff();
  return _inherited(p1,p2,p3,p4);
}

public func Redefine(p1,p2,p3,p4)
{
  if(!FindObject(FXST)) EjectStaff();
  return _inherited(p1,p2,p3,p4);
}
