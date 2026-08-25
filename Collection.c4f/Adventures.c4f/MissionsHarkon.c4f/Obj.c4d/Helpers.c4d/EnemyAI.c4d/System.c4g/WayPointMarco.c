/*---- Wegfindung ----*/

#strict 2
#appendto CLNK
#appendto BIRD

// Logs an error message
global func ErrorLog(string msg) {
  var out = Format(msg,Par(1),Par(2),Par(3),Par(4),Par(5),Par(6),Par(7),Par(8),Par(9));
  var out = Format("ERROR: %s",out);
  Log(out);
  if(this)
    Log(" in: %s @(%d,%d)",GetName(),GetX(),GetY());
}

// Logs if debug mode is on
global func DebugLog(string msg, name) {
  if(Debug(name))
    Log(msg,Par(2),Par(3),Par(4),Par(5),Par(6),Par(7),Par(8),Par(9));
}

// Displays the message if the debug mode is on
// always per-object
global func DebugMessage(string msg,  name) {
  if(Debug(name))
    Message(msg,this,Par(2),Par(3),Par(4),Par(5),Par(6),Par(7),Par(8),Par(9));
}

// Checks wether the debug mode is on
global func Debug(name) {

  // this namespace on
  if(g_DebugMode == name) return true;

  // generally on
  if(g_DebugMode == "all") return true;

  return false;
}

static g_DebugMode;

global func DebugMode(bool switch, name) {

  if(!name) name = "all";

  // on
  if(switch) {
    g_DebugMode = name;
  }
  // off
  else {
    g_DebugMode = 0;
  }
}

local aMacroCommandList;
// Wegpunktearray
local aPath;
local iActiveMacro;
local iMacroBreaktime;
local fMacroTest; // Ohne Breaktime

local pShowHelper;

global func ResetAI()
{
	for(var obj in FindObjects(Find_OCF(OCF_CrewMember)))
		obj->LocalN("aMacroCommandList") = [];
}

global func GetMacroPathLength(iX, iY, iX2, iY2)
{
	if( !iX2 && !iY2)
	{
		iX2 = GetX();
		iY2 = GetY();
	}
  var start, end;
  start = FindWaypoint(iX2, iY2);
  end = FindWaypoint(iX, iY);
	if(!start || !end) return Distance(iX, iY, iX2, iY2);
	return start->GetLengthToTarget(end);
}

public func SetMacroCommand(string szCommand, pTarget, int iX, int iY, object pLayer)
{
	if(g_showMacros) Log("SetMacroCommand %v", szCommand);
	while(AbortMacroCommand(0));
	if(szCommand == "None") return;
	if(!GetEffect("ExecMacroCommand", this))
		AddEffect("ExecMacroCommand", this, 300, 1, this);
	
//	if(GetAction() == "Scale") SetAction("Jump");

	aMacroCommandList =  [ ["MacroMoveTo", pTarget, iX, iY, szCommand, pLayer] ];
	iMacroBreaktime = 100;
}

func Death()
{
	SetMacroCommand("None");
	return _inherited(...);
}

local g_showMacros;

func FxExecMacroCommandTimer(pTarget, iNumber, iTime)
{
	if(!aMacroCommandList || !aMacroCommandList[0]) return -1;
	if(iMacroBreaktime-- <= 0 && !fMacroTest) { iMacroBreaktime = 0; MacroComSuccessFailed(); }
	var aMacroCommand = aMacroCommandList[0];
	iActiveMacro = 0;
	if(aMacroCommand)
	{if(g_showMacros) Log("      %v", aMacroCommand);
		var res = 0;
		if(aMacroCommand[0] == "MacroMoveTo")
			res = MacroComMoveTo(aMacroCommand[1],aMacroCommand[2],aMacroCommand[3], aMacroCommand[4],aMacroCommand[5]);
		else if(aMacroCommand[0] == "MoveToStep")
			res = MacroComMoveToStep(aMacroCommand[1],aMacroCommand[2],aMacroCommand[3]);
		else if(aMacroCommand[0] == "Command")
			res = MacroCommand(aMacroCommand[1], aMacroCommand[2], aMacroCommand[3], aMacroCommand[4]);
		else if(aMacroCommand[0] == "MoveTo" && GetProcedure() == "FLOAT")
			res = MacroFly(aMacroCommand[1],aMacroCommand[2],aMacroCommand[3]);
		else if(aMacroCommand[0] == "MoveTo")
			res = MacroMoveTo(aMacroCommand[1],aMacroCommand[2],aMacroCommand[3]);
		else if(aMacroCommand[0] == "Get")
			res = MacroMoveTo(aMacroCommand[1],aMacroCommand[2],aMacroCommand[3]);
		else if(aMacroCommand[0] == "Attack")
			res = MacroMoveTo(aMacroCommand[1],aMacroCommand[2],aMacroCommand[3]);
		else if(aMacroCommand[0] == "Jump")
			res = MacroJump(aMacroCommand[1],aMacroCommand[2],aMacroCommand[3]);
		else if(aMacroCommand[0] == "Enter")
			res = MacroEnter(aMacroCommand[1]);
		else if(aMacroCommand[0] == "Grab")
			res = MacroGrab(aMacroCommand[1]);
		else if(aMacroCommand[0] == "Wait")
			res = MacroWait(aMacroCommand[1], 0);
		else if(aMacroCommand[0] == "Call")
		{
			FinishMacroCommand(iActiveMacro);
			ProtectedCall(aMacroCommand[1], aMacroCommand[2], aMacroCommand[3], aMacroCommand[4], aMacroCommand[5]);
		}
		else res = 1;
		if(res)
			FinishMacroCommand(iActiveMacro);
	}
	if(g_showMacros)
	{
		if(!pShowHelper)
			pShowHelper = CreateObject(VIPU);
		pShowHelper->Set2(this,aMacroCommand);
		var szComm = "";
		for(var com in aMacroCommandList)
			if(com) szComm = Format("%s|%v %v", szComm, com[0], com[1]);
		Message(szComm, this);
		Message("%d", this, iMacroBreaktime);
	}
	else if(pShowHelper) RemoveObject(pShowHelper);
}

func MacroCallAbort(pObj, szName, p1, p2)
{
	if(szName == "KIOpen" || szName == "KIClose")
		ProtectedCall(pObj, szName, p1, p2);
}

func MacroWait(iTime, iIndex)
{if(g_showMacros) Log("%d (%d)", aMacroCommandList[iIndex][2], iTime);
	if(aMacroCommandList[iIndex][2]++ >= iTime) return 1;
}

func MacroFly(pTarget, iX, iY)
{
	if(pTarget && ObjectDistance(pTarget) < 10) return 1;
	if(Distance(GetX(), GetY(), iX, iY) < 10) return 1;
	if(!GetCommand())
		SetCommand(this, "MoveTo", pTarget, iX, iY);
}

func MacroCommand(pTarget, iX, iY, szCommand)
{
	if(pTarget) { iX = iY = 0; }
	if(pTarget && ObjectDistance(pTarget) < 10) return 1;
	if(Distance(GetX(), GetY(), iX, iY) < 10) return 1;
	if(!GetCommand())
		SetCommand(this, szCommand, pTarget, iX, iY);
}

func MacroMoveTo(pTarget, iX, iY)
{
  var iExtra = 0;
  if(iY == -1)
  {
    iExtra = iX;
    iX = 0;
    iY = 0;
  }
	if(GetAction() == "ScaleLadder") return;
	if(GetProcedure() != "WALK")
	{
		// Zielaktion stoppen
		this->~StopAiming();
		if(GetAction() == "LookAround") SetAction("Walk");
		if(GetAction() == "Push") SetAction("Walk");
    if(GetProcedure() == "FIGHT" && GetActionTarget()->~IsTrainingPuppet()) SetAction("Walk");
	}
	if(Contained()) MacroExit();
	if(pTarget) { iX = GetX(pTarget); iY = GetY(pTarget); }
	if(!iX && !iY && !iExtra) return 1;
	if(Abs(iX-GetX()) < 5 && Abs(iY-GetY()) < 20 && GetContact(this, -1))
	{
		if(pTarget && pTarget->Contained()) AddMacroCommand("Enter", pTarget->Contained());
		SetComDir(COMD_Stop);
		return 1;
	}
	else if(iExtra)// && GetObjectLayer() != pTarget->GetObjectLayer())
  {
    SetComDir(iExtra);
  }
	else if(iX < GetX()) SetComDir(COMD_Left);
	else if(iX > GetX()) SetComDir(COMD_Right);

	if(GetProcedure() == "SCALE")
		if(GetDir() != (GetComDir() == COMD_Right))
		{
			SetComDir(COMD_Down);
			// Fallen lassen
			if(!GBackSolid(-5+10*GetDir(),4))
				SetAction("Jump");
		}
		
	if(GetProcedure() == "SWIM")
	{
		if(GetComDir() == COMD_Right) SetComDir(COMD_UpRight);
		if(GetComDir() == COMD_Left ) SetComDir(COMD_UpLeft);
	}
}

func MacroMoveToAbort() { SetComDir(COMD_Stop); }

func MacroJump(iDir)
{//Log("%d %d %v %v %d(%d)", GetX(), GetY(), GetAction(), GetPhase(), iDir, GetDir());
  if(g_showMacros == 1) Log("Jump");
	if(GetProcedure() != "WALK" || !(GetContact(this, -1) & CNAT_Bottom)) return 0;
	SetDir(iDir == 1);
	Jump();
	return 1;
}

func MacroGrab(pTarget)
{
	var iX = GetX(pTarget);
	var iY = GetY(pTarget);
	if(Abs(GetX()-iX) < 5 && Abs(GetY()-iY) < 10 && GetContact(this, -1))
	{
		DoGrab(pTarget); if(g_showMacros) Log("DoGrab %v", GetActionTarget());
		return 1;
	}if(g_showMacros) Log("GrabMoveTo %v", GetActionTarget());
	AddMacroCommand("MoveTo", pTarget);
}

func DoGrab(obj)
{
	SetComDir(COMD_Stop);
	SetCommand(this, "None");
	SetAction("Push", obj);
	obj->~Grabbed(this, 1);
}

func MacroExit()
{
	var pTarget = Contained();
	if(!pTarget) return 1;
	var iXOffset = GetDefCoreVal("Entrance", "DefCore", GetID(pTarget), 0);
	var iYOffset = GetDefCoreVal("Entrance", "DefCore", GetID(pTarget), 1);
	var iWidth   = GetDefCoreVal("Entrance", "DefCore", GetID(pTarget), 2);
	var iHeight  = GetDefCoreVal("Entrance", "DefCore", GetID(pTarget), 3);
	var iX = iXOffset+iWidth/2;
	var iY = iYOffset+iHeight;
	if(pTarget->GetEntrance())
	{
		Exit(0, iX, iY);
		return 1;
	}
	pTarget->~ActivateEntrance(this);
	if(!Contained()) return 1;
	return 0;
}

func MacroEnter(pTarget)
{
	var iXOffset = GetDefCoreVal("Entrance", "DefCore", GetID(pTarget), 0);
	var iYOffset = GetDefCoreVal("Entrance", "DefCore", GetID(pTarget), 1);
	var iWidth   = GetDefCoreVal("Entrance", "DefCore", GetID(pTarget), 2);
	var iHeight  = GetDefCoreVal("Entrance", "DefCore", GetID(pTarget), 3);
	var iX = GetX(pTarget)+iXOffset+iWidth/2;
	var iY = GetY(pTarget)+iYOffset+iHeight/2;
	if(Abs(GetX()-iX) < 5 && Abs(GetY()-iY) < 10)
	{
		if(pTarget->GetEntrance())
		{
			Enter(pTarget);
			return 1;
		}
		pTarget->~ActivateEntrance(this);
		if(Contained() == pTarget) return 1;
		return 0;
	}
	AddMacroCommand("MoveTo", 0, iX, iY);
}

func AddMacroCommand(szCommand, pTarget, iX, iY, iZ, iZ2)
{if(g_showMacros)Log("AddMacro %v", aMacroCommandList);
	iActiveMacro++;
	if(!aMacroCommandList) aMacroCommandList = CreateArray();
	PushFront([szCommand, pTarget, iX, iY, iZ, iZ2], aMacroCommandList);if(g_showMacros)Log("      AddMacro %v", aMacroCommandList);
}

func AppendMacroCommand(szCommand, pTarget, iX, iY, iZ)
{
	if(!aMacroCommandList) aMacroCommandList = CreateArray();
	PushBack([szCommand, pTarget, iX, iY, iZ], aMacroCommandList);
}

func GetMacroCommand()
{
	if(aMacroCommandList && GetLength(aMacroCommandList) && GetLength(aMacroCommandList[0])) return aMacroCommandList[0][0];
}

func FinishMacroCommand(iIndex)
{if(g_showMacros)Log("FinishMacroCommand %v", aMacroCommandList);
	DeleteArrayItem(iIndex, aMacroCommandList);if(g_showMacros)Log("FinishMacroCommand %v", aMacroCommandList);
}

func AbortMacroCommand(iIndex)
{
	if(GetLength(aMacroCommandList) <= iIndex) return 0;
	var aMacroCommand = aMacroCommandList[iIndex];
	if(!aMacroCommand) return 0;
	ObjectCall(this, Format("Macro%sAbort", aMacroCommand[0]), aMacroCommand[1], aMacroCommand[2], aMacroCommand[3], aMacroCommand[4]);
	FinishMacroCommand(iIndex);
	return 1;
}

// MoveTo
protected func MacroComMoveTo(pTarget, iX, iY, szCommand, pLayer)
{
  // Erstes Kommando ist gar nicht MoveTo?
//  if(GetMacroCommand(0,0) != "MoveTo") return;
  // Kein Ziel? (bewirkt, dass der Clonk nicht nach 0,0 laufen kann!)
  if(!iX && !iY)
    if(!pTarget)
      return Log("ERROR") && 0;//MacroComSuccessFailed(0, Macro_MoveToNoTarget);
	if(pTarget)
	{
		iX = GetX(pTarget);
		iY = GetY(pTarget);
		pLayer = GetObjectLayer(pTarget);
	}
  // Pfad suchen
  aPath = CreateArray();
  var start, end;
  start = FindWaypoint(GetX(), GetY(), GetObjectLayer());
  end = FindWaypoint(iX, iY, pLayer);
  if(g_showMacros) Log("Start %v End %v", start, end);
	if(start == end)
	{if(g_showMacros) Log("Same: Start %v End %v", start, end);
		if(pTarget)
			iMacroBreaktime = Max(ObjectDistance(pTarget) / 100 * 100, 100);
		else
			iMacroBreaktime = Max(Distance(GetX(), GetY(), iX, iY) / 100 * 100, 100);
		if(start == 0 || start->~IsUsingCommands())
			AddMacroCommand("Command", pTarget, iX, iY, szCommand);
		AddMacroCommand(szCommand, pTarget, iX, iY);
		return 1;
	}
  if(!start || !end)
	{/*if(g_showMacros) Log("Not Found Start %v End %v", start, end);
		if(pTarget)
			iMacroBreaktime = Max(ObjectDistance(pTarget) / 100 * 100, 100);
		else
			iMacroBreaktime = Max(Distance(GetX(), GetY(), iX, iY) / 100 * 100, 100);
		AddMacroCommand("Command", pTarget, iX, iY, szCommand);*/
		iMacroBreaktime = 1;
		return 1;
	}
	// Find a shorter way?
	var start2 = start->GetNextWaypoint(end);
	var flag = start->GetPathFlag(start->GetPathToTarget(start2));
//	var type = GetPathFlag
  if( (GetX(start) > GetX()) != (GetX(start2) > GetX())  && flag == Path_MoveTo && Abs(GetX()-GetX(start)) >= 5)
	{
		if(g_showMacros) Log("Shorter Start %v %v %d", start, start2, flag);
		start = start2;
	}
	if(start != end)
	{
    var end2 = start, end_tmp = start;
    while(1)
    {if(g_showMacros == 1) Log("%v %v %v", end2, end_tmp, end);
      end2 = end_tmp;
      end_tmp = end2->GetNextWaypoint(end);
      if(g_showMacros == 1) Log("%v %v %v", end2, end_tmp, end);
      if (end_tmp == end) break;
    }
		flag = end2->GetPathFlag(end2->GetPathToTarget(end));
		if( (GetX(end) > iX) != (GetX(end2) > iX) && flag == Path_MoveTo && Abs(iX-GetX(end)) >= 5)
		{
			if(g_showMacros) Log("Shorter End %v %v %d", end, end2, flag);
			end = end2;
		}
	}
	
	if(start == end)
	{
		if(pTarget)
			iMacroBreaktime = Max(ObjectDistance(pTarget) / 100 * 200 * 5, 100);
		else
			iMacroBreaktime = Max(Distance(GetX(), GetY(), iX, iY) / 100 * 200 * 5, 100);
		if(start->~IsUsingCommands())
			AddMacroCommand("Command", pTarget, iX, iY, szCommand);
		AddMacroCommand(szCommand, pTarget, iX, iY);
		return 1;
	}

	if(g_showMacros) Log("---Start %v End %v", start, end);
  aPath = FindPath(start, end, true);
	if(g_showMacros)Log("%v", aPath);
//	PushFront(start, aPath);
  // Kein Pfad konnte gefunden werden?
  if(!aPath) return 0;//MacroComSuccessFailed(0, Macro_NoPath);

	AddMacroCommand("MoveToStep", 1);
	if(Abs(GetX()-GetX(aPath[0])) > 5)
	{
		AddMacroCommand("MoveTo", aPath[0]);
//		if(GetHorse()) SetCommand(GetHorse(), "MoveTo", aPath[0]);
	}
}

protected func MacroComMoveToStep(int iStep) // MoveTo-Schritt
{
  // Wird haben das Ende des Pfades erreicht?
  if(GetLength(aPath) == iStep)
    return 1;
  // Aus irgendeinem Grund ist kein Wegpunkt da?
  if(!aPath[iStep]) return Log("1!") && 0;
  if(!aPath[iStep-1]) return Log("2!") && 0;
	// Rausfinden, welchen Weg wir gehen müssen
	var path = aPath[iStep-1]->GetPathToTarget(aPath[iStep]);
  // Kein Weg? -> Fehler
  if(path < 0) return Log("3! %v %v", aPath[iStep-1], aPath[iStep]) && 0;
  // Nächsten Wegpunkt begehen
	AddMacroCommand("MoveToStep", iStep+1);
  MoveAlongPath(aPath[iStep-1], aPath[iStep], iStep+1, path);
  // Automatischer Abbruch
	iMacroBreaktime = aPath[iStep-1]->GetPathTime(path) * 150 / 100; // Normale Zeit plus 50% gratis!
	if(aPath[iStep-1]->GetPathFlag(path) == Path_Lift) iMacroBreaktime *= 2; // Wenn der Aufzug gerade nicht da ist kann es schon mal doppelt so
	iMacroBreaktime = Max(iMacroBreaktime, 100);
	if(g_showMacros) Log("iMacroBreaktime %v", iMacroBreaktime);
	DebugLog(Format("%s #%d: breaktime: %d (%d)", GetName(), ObjectNumber(), iMacroBreaktime), "AI_ExecMovement");
  return 1;
}

// Makrobefehl an der Stelle iCommand ist fehlgeschlagen
protected func MacroComSuccessFailed(int iCommand, int iReason)
{
	while(AbortMacroCommand(0));
	aMacroCommandList = [];
	if(g_showMacros) Log("Faill!! %v", this);
  return 1;
}

// Lässt den Clonk von einem Wegpunkt zum anderen laufen, wobei er die jeweilige Pfadeigenschaft beachten muss
private func MoveAlongPath(object pCurrentWp, object pNextWp, int iNextStep, int path)
{
  // Flag rausfinden und entsprechend agieren
  var flag = pCurrentWp->GetPathFlag(path);
  if(flag == Path_MoveTo)
  {
    // Hinlaufen
    AddMacroCommand("MoveTo", pNextWp, pCurrentWp->GetPathExtra(path), -1);
    return 1;
  }
  if(flag == Path_Jump)
  {
    // Springen und dann laufen
    var dir = pCurrentWp->GetPathDir(path);
		AddMacroCommand("MoveTo", pNextWp);
		AddMacroCommand("Jump", dir);
		AddMacroCommand("Wait", 1);
    return 1;
  }
  if(flag == Path_Ladder)
  {
    // Springen und dann laufen
    if(pNextWp->GetY() < GetY())
			ControlLadder("ControlUp");
		else
			ControlLadder("ControlDown");
		AddMacroCommand("MoveTo", pNextWp);
		AddMacroCommand("Wait", 35);
    return 1;
  }
  if(flag == Path_Scale) {
    var pHelper = pCurrentWp->GetPathExtra(path);
		var dir = pCurrentWp->GetPathDir(path);
		AddMacroCommand("MoveTo", pNextWp);
		AddMacroCommand("Call", this, "ReleaseScale");
		AddMacroCommand("MoveTo", pHelper);
		AddMacroCommand("Jump", dir);
		AddMacroCommand("Wait", 1);
    return 1;
  }
  if(flag == Path_Lift)
  {
    // Wir suchen den Lift und warten oder betreten ihn
    LiftControl(pCurrentWp, pNextWp, path);
    return 1;
  }
  if(flag == Path_Door)
  {
    // Wir suchen die Tür und öffnen sie
    DoorControl(pCurrentWp, pNextWp, path);
    return 1;
  }
  if(flag == Path_Stairs)
	{
		// Wir suchen Treppenaufgänge und gehen rein
    StairsControl(pCurrentWp, pNextWp, path);
    return 1;
	}
  // Unbekanntes flag, MoveTo versuchen
  AddMacroCommand("MoveTo", pNextWp);
  return 0;
}

private func ReleaseScale()
{
  // Fallen
	SetPosition(GetX()+1-2*GetDir(), GetY());
  SetXDir(+10-20*GetDir());
  SetAction("Jump");
}

func ForceDir(pDummy, iDir) { SetDir(iDir); }

protected func StairsControl(object pCurrentWp, object pNextWp, int path)
{
  // Treppenhaus suchen
	var stairs = pCurrentWp->GetPathExtra(path);
	if(!stairs) AddMacroCommand("MoveTo", pNextWp);
	else
	{
    AddMacroCommand("MoveTo", pNextWp);
    if(stairs[1]==1) stairs[1] = "ContainedUp";
    else if(stairs[1]==2) stairs[1] = "ContainedDown";
    else
    {
      if(GetY(pNextWp) < GetY()) stairs[1] = "ContainedUp";
      else stairs[1] = "ContainedDown";
    }
		if(stairs[1]) AddMacroCommand("Call", stairs[0], stairs[1], this);
		if(stairs[2]) AddMacroCommand("Wait", stairs[2]);
		AddMacroCommand("Enter", stairs[0]);
	}
  return 1;
}

protected func LiftControl(object pCurrentWp, object pNextWp, int path)
{
  // Fässt den Lift schon an?
  if(GetAction() == "Push")
  {
    var lift = GetActionTarget();
    if(!lift) return;
    // Nah genug am Ziel? Absteigen
    if(Abs(GetY()-(GetY(pNextWp)+5)/10*10) < 1)
		{
      return AddMacroCommand("MoveTo", pNextWp);
		}
    // Liftplatten befehligen
		if(!lift->GetCommand())
			lift->~SetMoveTo(pNextWp->GetY()/10*10);
    // Warten
    AddMacroCommand("Call", this, "LiftControl", pCurrentWp, pNextWp, path);
    AddMacroCommand("Wait", 15);
    return 1;
  }
  // Liftplatte suchen
  var lift = pCurrentWp->GetPathExtra(path);
  var pOtherControler = FindObject2(Find_OCF(OCF_CrewMember), Find_Action("Push"), Find_ActionTarget(lift), Find_Exclude(this));
  // Kein Lift? -> Fehlschlag
  if(!lift) return 0;
  // Lift nah genug? -> Einsteigen
  if(Abs(GetY()-GetY(lift)) < 5)
  {
    AddMacroCommand("Call", this, "LiftControl", pCurrentWp, pNextWp, path);
    AddMacroCommand("Grab", lift);
    return 1;
  }
  else if(!lift->GetCommand() && !pOtherControler)
		lift->~SetMoveTo((pCurrentWp->GetY()+5)/10*10);
  // Warten
  AddMacroCommand("Call", this, "LiftControl", pCurrentWp, pNextWp, path);

  AddMacroCommand("Wait", 15);
  return 1;
}

protected func DoorControl(object pCurrentWp, object pNextWp, int path)
{
	var door = pCurrentWp->GetPathExtra(path);
	if(door)
	{
		AddMacroCommand("Call", door[0], "KIClose", this, door[1]);
		AddMacroCommand("MoveTo", pNextWp);
		AddMacroCommand("Call", door[0], "KIOpen", this, door[1]);
		return 1;
	}
	return 0;
}

public func FindPath(object pStart, object pEnd)
{
  // maximale Iterationen fuer Wegfindung (abhaengig von Anzahl Wegpunkte)
  var iterationLimit = ObjectCount(WAYP);

  var ex = GetX(pEnd);
  var ey = GetY(pEnd);

  var pCurrent = pStart;

  var aWaypoints = CreateArray();

  for(var j=0; j< iterationLimit; ++j) {

    aWaypoints[j] = pCurrent;

    // fertig!
    if(pCurrent == pEnd)
      return aWaypoints;

		var pNext = pCurrent->GetNextWaypoint(pEnd);
		if(!pNext)
		{
      DebugLog("Failure: Selected waypoint leads into a dead end.","waypoints");
      return false;
    }

    pCurrent = pNext;
  }
  DebugLog("Failure: No path found after %d iterations.","waypoints",iterationLimit);
  return false;
}

// nächsten Wegpunkt von einer Position aus suchen (geht nur, wenn PathFree!)
global func FindWaypoint(int iX, int iY, object pLayer)
{
  var wp;
  for(wp in FindObjects(Find_ID(WAYP), Sort_Distance(iX-GetX(), iY-GetY()))) {
    if(PathFree(iX, iY, wp->GetX(), wp->GetY()) || GBackSolid(wp->GetX()-GetX(), wp->GetY()-GetY()))
      return wp;
		else if(wp->IsUsingCommands() && GetPathLength(iX, iY, wp->GetX(), wp->GetY()))
			return wp;
	}
  // Nix gefunden
	return;
}

func ControlLadder() { return _inherited(...); }
