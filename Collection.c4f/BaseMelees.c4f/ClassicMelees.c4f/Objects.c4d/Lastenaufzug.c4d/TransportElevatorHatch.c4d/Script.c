/*-- Neues Objekt --*/

#strict

func Initialize() {
  SetAction("Closed");
  SetPosition(GetX(), GetY()+6);
  DigFreeRect(GetX()-66, GetY()-26, 132, 18);
//CreateObject(BAS1, -53, 0);
//CreateObject(BAS1, 53, 0);
  return(1);
}

func BeginMask()	{
	SetSolidMask(0,0,132,8);
}

func EndMask()	{
	SetSolidMask(0,0,0,0);
}

private func UpdateSolidMask()
{
  if (GetAction() eq "Open")
    SetSolidMask(GetPhase() * 132, 0, 132, 8, 0, 0);
  if (GetAction() eq "Close")
    SetSolidMask(GetDefCoreVal("Width", "DefCore", GetID()) - GetPhase() * 132, 0, 132, 8, 0, 0);
}  

func Timer()	{
	if(GetAction() eq "Opened")
		{
		EndMask();
		}
	if(GetAction() eq "Closed")
		{
		BeginMask();
		}

	if(FindObject(LC0S, -40, -45, 80, 85))
		{
		if(GetAction() eq "Closed")
			{
			SetAction("Open");
			Sound("SteelGate2");
			}
		}
	else
		{
		if(GetAction() eq "Opened")
			{
			SetAction("Close");
			Sound("SteelGate2");
			}
		}
}
