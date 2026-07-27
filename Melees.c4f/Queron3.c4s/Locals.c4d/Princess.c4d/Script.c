#strict 2

#include CLNK

protected func Timer() // Tick100
{
	// Vielleicht ist unser Held gestorben
	if (GetAction()=="Piggyback" && !GetAlive(GetActionTarget(0)))
	{
		Message("$MsgHeroDead$", this);
		SetAction("Jump");
	}
  if (!Random(30))
  {
  	var cage = FindObject(_CAG, 0,1);
  	if (cage && (cage->GetAction()!="Open"))
  		{
  		Sound("PrincessScream");
  		Message("$MsgHelp$", this);
  		}
  }
}

public func ControlDigDouble(object pCaller)
{
	// Nur wenn noch am Leben
	if (!GetAlive()) return 1;
	// Huckepack
	SetAction("Piggyback", pCaller);
	if (GetDir(pCaller) == DIR_Left)
	{
		SetDir(DIR_Left);
		SetActionData(256*2+4);
	}
	else
	{
		SetDir(DIR_Right);
		SetActionData(256*2+3);
	}
	// Left und Right abfangen und weiterleiten
	var nr = AddEffect("IntControlDir", pCaller, 1,, this);
	Message("$MsgHero$", this);
	return 1;
}

public func FxIntControlDirControlLeft(object pTarget, int iNr)
{
	SetDir(DIR_Left, this);
	SetActionData(256*2+4);
}

public func FxIntControlDirControlRight(object pTarget, int iNr)
{
	SetDir(DIR_Right, this);
	SetActionData(256*2+3);
}


protected func Death(int iKilledBy)
{
	// Effekt weg
	RemoveEffect("IntControlDir", GetActionTarget());
	if (GetPlayerName(iKilledBy))
		Message("$MsgCursed$", this, GetPlayerName(iKilledBy));
	return inherited(iKilledBy, ...);
}
