/*-- Eisklumpen --*/

#strict

#appendto ICE1

public func BridgeMaterial() 
{
  return(Material("Ice"));
}

public func Activate(caller)
{
  // Clonk soll anhalten
  SetComDir(COMD_Stop(), caller);
  // Menü öffnen
  CreateMenu(LMMS, caller, this());
  AddMenuItem("%s", "MenuBridge", LMM1, caller);
  AddMenuItem("%s", "MenuBridge", LMM2, caller);
  AddMenuItem("%s", "MenuBridge", LMM3, caller);
  AddMenuItem("%s", "MenuBridge", LMM4, caller);
  AddMenuItem("%s", "MenuBridge", LMM5, caller);
  // Vorauswahl Senkrecht
  SelectMenuItem(2, caller);
  return(1);
}

protected func MenuBridge(icon, dummy, enterall) 
{
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (GetAction(Contained()) ne "Walk") return(0);
  if (Contained(Contained())) return(0); 
  // Evtl. weitere Aktionen durchführen
  this()->~OnBridge();
  // Aktion setzen
  ObjectSetAction(Contained(), "Bridge");
  // Richtung setzen
  if (icon == LMM1) SetComDir(COMD_Left(), Contained());
  if (icon == LMM2) SetComDir(COMD_UpLeft(), Contained());
  if (icon == LMM3) SetComDir(COMD_Up(), Contained());
  if (icon == LMM4) SetComDir(COMD_UpRight(), Contained());
  if (icon == LMM5) SetComDir(COMD_Right(), Contained());
  // Brückenmaterial setzen
  SetActionData(BridgeMaterial(), Contained());
  // Ggf. Kommando zur Fortsetzung setzen
  if(enterall)
    SetContinuationCommand(icon);
  // Material verbrauchen
  RemoveObject(); 
  // Fertig
  return(1);
}       

private func SetContinuationCommand(idIcon)
{
  // weiterer Lehmklumpen zu verbrauchen?
  var pNext;
  if(pNext = Contents(1, Contained()))
    if(pNext != this())
      if(GetID(pNext) == GetID())
        SetCommand(Contained(), "Call", Contents(1, Contained()), idIcon, 0, 0, "ContinueBridge");
  // Kurze Pause vor nächster Bauetappe (Clonk muss ggf. aufstehen)
  AddCommand(Contained(), "Wait", 0, 0, 0, 0, 15);    
  // Bis zum Ende der gebauten Brücke laufen
  if(idIcon == LMM1)
    AddCommand(Contained(), "MoveTo", 0, GetX() - 23, GetY() + 8);
  if(idIcon == LMM2)
    AddCommand(Contained(), "MoveTo", 0, GetX() - 22, GetY() - 6);
  if(idIcon == LMM3)
    if(GetDir(Contained()) == DIR_Left())
      AddCommand(Contained(), "MoveTo", 0, GetX() - 9, GetY() - 15);
    else
      AddCommand(Contained(), "MoveTo", 0, GetX() + 8, GetY() - 15);
  if(idIcon == LMM4)
    AddCommand(Contained(), "MoveTo", 0, GetX() + 21, GetY() - 6);
  if(idIcon == LMM5)
    AddCommand(Contained(), "MoveTo", 0, GetX() + 22, GetY() + 8);
  // Warten, bis die Lehmbrücke fertig ist
  AddCommand(Contained(), "Wait", 0, 0, 0, 0, 100);  
}

protected func ContinueBridge(pByObject, idIcon)
{
  return(MenuBridge(idIcon, 0, 1));  
}

