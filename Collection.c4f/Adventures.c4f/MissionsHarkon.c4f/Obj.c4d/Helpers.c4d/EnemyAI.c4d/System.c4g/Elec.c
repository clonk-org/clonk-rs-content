/* Objekte, denen die KI ausweichen sollte */

#strict 2

#appendto ELEC

local fNoFix;

protected func UpdateTransferZone()
{
  return SetTransferZone();
}

private func FindWaitingClonk()
{
  var clnk, best, proc;
  while(clnk = FindObject(0, -30, RangeTop - GetY(), 60, 6000, OCF_CrewMember, 0, 0, NoContainer(), clnk) )
  {
		if(GetPlayerType(GetOwner(clnk)) == C4PT_Script) continue; // Scriptplayer fordern es sich selber an! (somit blockieren sie sich auch nicht gegenseitig)
    proc = GetProcedure(clnk);
    if(GetComDir(clnk) == COMD_Stop || (proc == "SWIM" && Inside(GetXDir(clnk), -5, 5)) )
    {
      if(proc == "WALK" || proc == "PUSH" || proc == "SCALE" || proc == "HANGLE" || proc == "SWIM")
        if(!Hostile(GetController(clnk), GetOwner()) )
        {
          // Nicht erreichbar?
          if(GetY(clnk) < GetY() - 7) if(!PathFree(GetX(), GetY(), GetX(), GetY(clnk)) ) continue;
          if(GetY(clnk) > GetY() + 7) if(!PathFree(GetX(), GetY() + 16, GetX(), GetY(clnk)) ) continue;
          // Priorität prüfen (Cursor hat Priorität, sonst nächster Clonk)
          if(!best)
            best = clnk;
          else if(GetCursor(GetController(clnk)) == clnk)
            best = clnk;
          else if(GetCursor(GetController(best)) != best)
            if(ObjectDistance(clnk) < ObjectDistance(best))
              best = clnk;
        }
    }
  }
  return best;
}

local iDestination;

protected func Timer() 
{
  _inherited();
  if(iDestination)
  {
    if(Abs(GetY()-iDestination)>2)
    {
      if(GetY() > iDestination)
        SetComDir(COMD_Up);
      else
        SetComDir(COMD_Down);
    }
    else
    { SetComDir(COMD_Stop); SetYDir(0); SetAction("Wait"); SetPosition(GetX(), iDestination); iDestination = 0; }
  }
}

private func SetMoveTo(iPos)
{
  if (NoEnergy()) return(1);
  SetAction("Ride"); 
  var iDir = 0;
  if (iPos > GetY()) iDir = +3;
  if (iPos < GetY()) iDir = -3;
  iDir = 0;
//  SetCommand(this(),"MoveTo", 0, GetX(), Max(iPos, RangeTop/10*10) + iDir);
  if(fNoFix)
    iDestination = Max(iPos, RangeTop)-1;
  else
    iDestination = Max(iPos, (RangeTop+5)/10*10)-1;
//  SetYDir(0);
  return(1);
}

protected func Activity()
{
  UpdateTransferZone();
  // Legacy
  if (Local(8) && !RangeTop) ImportSettings();
  // Fahrstuhl zerstört
  // if (!Elevator() || OnFire(Elevator())) { SetAction("Idle"); Incinerate(); return(); }
  // Zu wartenden Clonks fahren
  if (AutoMode == 0 && !Passenger() && !GetCommand())
    MoveToWaitingClonk();
  // Sicherheitscheck: Bewegung ohne Kommando nur bei Schachtbohrung
//  if (GetComDir())
//    if (!GetCommand() && GetAction() ne "Drill")
//      Halt();
  // Fertig
  return(1);
}

func UpdateVert(v)
{
  if(v==0)
    v = 8;
SetVertexXY(0, -11, -10);
SetVertexXY(1, 9, -10);
SetVertexXY(2,-11, v);
SetVertexXY(3, 9, v);
}
