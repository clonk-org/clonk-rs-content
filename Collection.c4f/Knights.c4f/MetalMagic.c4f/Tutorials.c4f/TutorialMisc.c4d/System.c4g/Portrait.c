/*-- Portrait --*/

#appendto CLNK
#strict 2

static szHelpName;
static szHelpingText;

func GetMsgPortrait()
{
  if(Local(10)) if(GetType(Local(10)) == C4V_Int) if(Var() = GetPortraitByIndex(Local(10))) return(Var());// if(GetType(Local(10)) == C4V_String) return(Local(10));
  if(GetOwner()>=0) return GetPortrait();
}

func GetPortraitByIndex(iIndex)
{
  return ["1","2","3","4","5","Dark1","Dark2","Dark3","Armored1","Armored2","Armored3","ArmoredDark1","ArmoredDark2","ArmoredDark3"][iIndex];
}

func RejectCollect(idObj, pObj)
{
  if(FindObject(DG69, 0, 0, 0, 0, 0, 0, this)) return 1;
  return _inherited(idObj, pObj);
}

func ContextHelp()
{
  [Aufgabe|Image=CXIN]
  CreateMenu(CXIN, this, this, 0, szHelpName, 0, 3);
  AddMenuItem(szHelpingText, 0, NONE, this);
}
