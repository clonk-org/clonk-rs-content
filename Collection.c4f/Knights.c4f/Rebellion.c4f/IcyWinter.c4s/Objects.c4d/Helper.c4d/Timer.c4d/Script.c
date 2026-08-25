/*-- Königs-Auftau-Timer --*/

#strict

local pMsgObj;

protected func Initialize()
{
  // Category setzen
  SetCategory(C4D_StaticBack | C4D_Foreground | C4D_MouseIgnore | C4D_Parallax | C4D_IgnoreFoW);
  // Position setzen
  SetPosition(30, 100);
  // Anzeige
  pMsgObj = CreateObject(TIM1,,,-1);
  pMsgObj->SetCategory(C4D_StaticBack | C4D_Foreground | C4D_MouseIgnore | C4D_Parallax | C4D_IgnoreFoW);
  pMsgObj->SetPosition(60, 112);
  // Zeit anzeigen
  Check();
}

protected func Check()
{
  var iMaxTime = 3400*10/38;
  var iTime = iMaxTime - (ScriptCounter()*10/38);
  if(iTime<=0) return(RemoveObject());
  if(iTime<=120)
    Message("@<c ff0000>%.2d:%.2d</c>", pMsgObj, iTime / 60, iTime % 60);
  else
    Message("@%.2d:%.2d", pMsgObj, iTime / 60, iTime % 60);
}

protected func Destruction()
{
  if(pMsgObj) RemoveObject(pMsgObj);
}

