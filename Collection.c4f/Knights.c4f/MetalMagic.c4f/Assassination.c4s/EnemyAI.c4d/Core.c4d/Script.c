/*-- Gegner KI --*/
#strict 2

#appendto _GRD // Nur Ritter

static const EA69_EnemySearchRange       = 400; // * Pixel nach d2

static g_EA69_aNoThrow; // Array von Gegenständen, die nicht geworfen werden sollten
static g_EA69_aNoThrowKnight; // für Ritter

local ai_iRange;                         // Suchweite für Gegner
local ai_pTarget;                        // Verfolgter Gegner
local ai_iDisableTime;                   // Verhindert KI-Aufrufe für die vorgegebene Zeit

local ai_aWay;

/* Initialisierung */

public func SetAI(object pATarget, int iARange)
  {
  if (pATarget) return pATarget->SetAI(0, iARange);
  if (GetCategory() & C4D_StaticBack) return; // Keine Speedclonks
  if (!GetAlive()) return; // Keine totgestellten Clonks
  if (ai_iRange) return; // Hat schon?
  if (!iARange) iARange = EA69_EnemySearchRange;
  if (!GetAlive()) return;
  // Werte setzen
  ai_iRange = iARange;
  // Timer erstellen
  if (GetEffect("AI", this)) return;
  return AddEffect("AI", this, 1, 3, this);
  }
  
global func AI_InitSystem()
  {
  // Keine nützlichen Gegenstände wegwerfen
  g_EA69_aNoThrow = [BRED, COKI, PHEA, PMON, PWIP, PFIS, PFIR, CSNK, CBRD];
  g_EA69_aNoThrowKnight = [SWOR, AXE1, BOW1, ARWP, FARP, XARP]; // für Ritter
  return true;
  }

/* Timer */

protected func FxAITimer()
  {
  var obj,dx,dy,vx,vy,dir,fMagi,i,fInLava;
  
  // Nicht wenn deaktiviert
  if (!ai_iRange) return;

  // Während bestimmter Aktionen absolut gar nichts tun, damit sie nicht abgebrochen werden!
  if (ai_iDisableTime)
    {
    --ai_iDisableTime;
    //Log("%v", ai_iDisableTime);
    return;
    }

  // Gegner bestimmen:
  // Gegner aktualisieren
  if (!Random(4))
    ai_pTarget = AI_FindNearestEnemy();

  // Projektile im Umkreis suchen
  var aProjectiles = FindObjects(Find_InRect(-50,-50,100,80), Find_Or(Find_Category(C4D_Object), Find_Func("IsDangerous4AI")), Find_OCF(OCF_HitSpeed2), Find_NoContainer());

  // Contained: Erstmal raus, wenn nicht drinnen am Kämpfen
  var pContainer;
  if (pContainer = Contained())
    {
    if (ai_pTarget)
      {
      // Aus Stein und Flasche auslösen
      if (!Random(50))
        {
        if (GetID(pContainer) == MRCK)  if (!Contained(pContainer)) return pContainer->ContainedDigDouble(this);
        if (GetID(Contained()) == CITB)
          {
          return ProtectedCall(pContainer, "Break");
          }
        }
      // Mit Gegner im Haus: OK für Clonks; nicht für Tiere!
      if (ai_pTarget) if (pContainer == Contained(ai_pTarget))
      {
        // Clonks kämpfen
        return FightWith(ai_pTarget);
        return true;
      }
      // Nicht, wenn noch Projektile draußen rumfliegen
      if (GetLength(aProjectiles)) return true;
      // Sonst raus
      return AI_SetCommand("Exit");
      }
    }

  // Sofortreaktionen: Tlw. adaptiert von Newtons TheTemple-KI
  // Projektilen ausweichen
  else for (obj in aProjectiles)
    {
    // Berechnen, ob das Projektil treffen würde
    dx = GetX(obj)-GetX(); dy = GetX(obj)-GetX();
    vx = GetXDir(obj); vy = GetYDir(obj); dir = vx/Abs(vx);
    if (!dir) dir = Random(2)*2-1;
    // VecAbstand * VecGeschwindigkeit < -Cos(Toleranz)*|VecAbstand|*|VecGeschwindigkeit|)
    if (dx*vx+dy*vy < 70*Sqrt(dx*dx+dy*dy)*Sqrt(vx*vx+vy*vy)/-100)
      {
      // Vor einem Eingang? Prima! Rein da!
      if (pContainer = FindObject2(Find_AtPoint(), Find_OCF(OCF_Entrance), Find_Not(Find_Func("IsHorse"))))
        return AI_SetCommand("Enter", pContainer);
      // Sonst ausweichen
      if (!AI_Evade(dir, obj))
        if (!AI_Evade(-dir, obj))
          // Nowhere to go :o
          return AI_Panic();
      return true;
      }
    }

  // Brennend: Löschaktionen
  if (OnFire())
    {
    // Wasserfass dabei?
    if (!fInLava) if (obj = FindContents(WBRL))
      return ProtectedCall(obj, "Activate", this);
    // Feuerpotion?
    if (obj = FindContents(PFIR))
      return ProtectedCall(obj, "Activate", this);
    // Schon auf dem Weg ins Wasser?
    i = AI_GetStackedCommand("MoveTo");
    if (i >= 0)
      if(GetMaterialVal("Extinguisher", "Material", GetMaterial(GetCommand(0, 2, i),GetCommand(0, 3, i))))
        return true; // Weiter so
    // Wasser in der Nähe?
    i = 20;
    while (i--)
      {
      dx = Random(401) - 200;
      dy = Random(401) - 200;
      var dist;
      if(GetMaterialVal("Extinguisher", "Material", GetMaterial(dx,dy)))
        if (dist = GetPathLength(GetX(), GetY(), GetX()+dx, GetY()+dy))
          if (dist < 800)
            return AI_SetCommand("MoveTo", 0, GetX()+dx, GetY()+dy);
      }
    // Keine Löschmöglichkeit: Zum nächsten Gegner laufen
    obj = AI_FindNearestEnemy(); if (!obj) obj = ai_pTarget;
    if (obj) return AI_SetCommand("Follow", obj);
    // Aaaah!
    return AI_Panic();
    }

  // Kein Gegner: Homeposition
  if (!ai_pTarget) return AI_Home();

  // Normale Bewegungsaktionen
  if (AI_ExecMovement()) return true;

  if(Contained(ai_pTarget)) return AI_SetCommand("Attack", ai_pTarget);

  // Werfen wenn nötig
  if (Contents()) if (AI_ExecThrow()) return true;

  // Ritter: Waffen benutzen
  if (IsKnight()) if (AI_ExecKnight()) return true;

  // Nahkampfangriff
  if (!AI_IsRanged())
    {
    // Zum Gegner laufen
    if (!GetCommand() || (GetCommand() == "MoveTo" && !Random(8)))
      return AI_SetCommand("Attack", ai_pTarget);
    }
  else if (!GetCommand())
    {
    // Fernkämpfer aus blöden Situationen
    if (GetProcedure() == "SWIM")
      return AI_Home();
    if (GetProcedure() == "SCALE")
      if (GetDir()) return(SetComDir(COMD_Left)); else return(SetComDir(COMD_Right));
    if (GetProcedure() == "HANGLE")
      return SetComDir(COMD_Down);
    // Fernkämpfer hat nichts zu tun
    //Log("nada");
    }

  // OK; Angriffsbefehl ausführen
  return true;
  }

/* Bewegung */

local pLastPoint;

private func AI_Home()
  {
  // Keine Basis, aber Fahne?
  if(FindContents(FLAG))
  {
    var pBase = FindObject(CPOF, 0, 0, -1, -1);
    if(pBase)
    {
      if(pBase == Contained())
      {
        AI_SetCommand("Put", pBase, 0, 0, FindContents(FLAG));
	return true;
      }
      //AI_SetCommand("MoveTo", pTarget);
//      AddCommand(this, "Call", pContainer, this, 0, 0, 0, szCommand);
      AI_SetCommand("Enter", pBase);
      AddCommand(this, "MoveTo", pBase);
      return true;
    }
  }
  // Wenig Energie: Heilung
  if (GetEnergy() < 15) if (AI_Heal()) return true;
  if(GetCommand())
  {/*
    if(!Random(30)) SetCommand(this, "None");
    else return true;*/
    return;
  }
  ai_pTarget = AI_FindNearestEnemy();
  // Ziel suchen
  var pPoint = FindObject(WPNT, 0, 0, -1, -1);
  if(ObjectDistance(pPoint)>10) return AI_SetCommand("MoveTo", pPoint);
  var iWay = pPoint->~GetRandomWay();
  if(pPoint->GetConnected(iWay) == pLastPoint) iWay = pPoint->~GetRandomWay();
  var pTarget = pPoint->GetConnected(iWay);
  var pContainer = pPoint->GetConnected(iWay, 1);
  var szCommand = pPoint->GetConnected(iWay, 2);
  pLastPoint = pPoint;
  if(pContainer)
  {
    AI_SetCommand("MoveTo", pTarget);
    AddCommand(this, "Call", pContainer, this, 0, 0, 0, szCommand);
    AddCommand(this, "Enter", pContainer);
    return true;
  }
  return AI_SetCommand("MoveTo", pTarget);
  }

private func AI_Evade(int iDir, object pProjectile) // iDir in {-1,+1}
  {
  // Sprung frei?
  if (PathFree(GetX(), GetY(), GetX()+25*iDir, GetY()-20))
  { AI_ExecMovement();  return AddCommand(0, "Jump", 0, iDir) && SetComDir(5-2*iDir); }
  // Weg frei?
  if (PathFree(GetX(), GetY(), GetX()+30*iDir, GetY()))
  { AI_ExecMovement();  return AddCommand(0, "MoveTo", 0, GetX()+30*iDir,GetY()); }
  // Nix frei :(
  }

private func AI_Panic()
  {
  // Aaah!11
  if (GetProcedure() != "WALK") return true;
  Jump(); SetXDir(0); SetYDir(GetYDir()/2);
  SetAction("Jump");
  Sound("Scream");
  return true;
  }

private func AI_ExecMovement()
  {
  //Log("ExecMovement: %s %s", GetProcedure(), GetCommand());
  var obj;
  var szCommand = GetCommand();
  var pTarget = GetCommand(0, 1), iTargetX, iTargetY;
  var iTargetX = GetCommand(0, 2), iTargetY = GetCommand(0, 3);
  // Sprünge während der normalen Bewegung
  if (GetProcedure() == "WALK")
    {
    if (szCommand == "MoveTo" || szCommand == "Attack" || szCommand == "Follow" || (szCommand == "Throw" && iTargetX))
      {
      var fShouldJump, fAnySolid;
      if (!iTargetX) iTargetX = GetX(GetCommand(0, 1));
      if (!iTargetY) iTargetY = GetY(GetCommand(0, 1));
      //Log("ExecMovement: %d %d", iTargetX, iTargetY);
      // Über Brennendes springen
      if(!OnFire())
        if(obj=FindObject2(Find_InRect(-40*!GetDir(),-40,40,80), Find_Exclude(this), Find_OCF(OCF_OnFire), Find_NoContainer()))
          fShouldJump = true;
      // Über Lava/Säurebecken/Tiefe Löcher springen
      var cx=40*GetDir()-20;
      if(GetAction() != "Jump" && !InLiquid())
        for(var j=1; j<50; j+=5)
          if(GetMaterialVal("Incindiary", "Material", GetMaterial(cx,j)))
            fShouldJump = true;
          else if (GBackSolid(cx, j))
            {
            fAnySolid = j;
            break;
            }
      // Sprung zum Gegner, bzw. Sprung einfach so
      if (GetY() - iTargetY > 16)
        if (!Random(8)) fShouldJump = true;
      // Sprung ausführen
      if (fShouldJump || (!fAnySolid && (iTargetY - GetY() < 20)))
        // Will/kann die KI überhaupt über dieses Loch?
        if (Abs(GetX() - iTargetX) > 40 && AI_SimJumpCheck(cx/20))
          // Dann springen
          return AddCommand(0, "Jump");
        else
          {
          // Ansonsten stehen bleiben
          AI_SetCommand("None");
          SetComDir(COMD_Stop);
          }
      }
    }
  // Im Wasser: Forcieren, dass geschwommen wird
  else if (GetProcedure() == "SWIM")
    {
    var dx = iTargetX-GetX(), dy = iTargetY-GetY(), iJumpOutDir;
    //Log ("Swimming %d  %d", dx, dy);
    if (dy<0 && GBackLiquid(0,-2))
      SetComDir(COMD_Up);
    else
      {
      if (dx>0)
        {
        SetComDir(COMD_Right);
        if (GetContact(0, -1, CNAT_Right)) iJumpOutDir=1;
        }
      else
        {
        SetComDir(COMD_Left);
        if (GetContact(0, -1, CNAT_Left)) iJumpOutDir=-1;
        }
      // Längere Schwimmzeit, wenn das Ziel weit oberhalb liegt, damit die KI nicht ewig im Wasser hin und herschwimmt
      if (-dy-10 > Abs(dx))
        AddCommand(0, "Wait", 0, 0,0, 0, 0,20);
      // Hängt an der Seite im Wasser? Rausspringen.
      if (iJumpOutDir)
        {
        AddCommand(0, "Call", this, iJumpOutDir,0, 0, 0, "AI_WaterJumpOut");
        AddCommand(0, "MoveTo", 0, GetX()-iJumpOutDir*10,GetY());
        ai_iDisableTime += 15;
        }
      }
    return true;
    }
  // Reitend: Bewegungssteuerung ans Tier
  else if (WildcardMatch(GetAction(), "Ride*"))
    {
    var pAnimal;
    if (pAnimal = GetActionTarget())
      {
      if (szCommand == "MoveTo") 
        {
        SetCommand(pAnimal, "MoveTo", pTarget, iTargetX, iTargetY);
        }
      else
        {
        SetCommand(pAnimal, "None");
        SetComDir(COMD_Stop, pAnimal);
        if (szCommand == "Enter")
          {
          // Enter-Command: Geht nicht. Verwerfen.
          SetCommand(0, "None");
          }
        else if (szCommand == "Throw")
          {
          // Werfen umsetzen
          ControlThrow();
          SetCommand(0, "None");
          }
        }
      }
    }
  // Kein Kommando mehr: Erst einmal anhalten
  if (!GetCommand()) SetComDir(COMD_Stop);
  // Nahkampf:
  // Schon am Kämpfen?
  if (GetProcedure() == "FIGHT") return AI_ExecFight();
  // Gegner in Reichweite für Nahkampf?
  if (ai_pTarget) if (ObjectDistance(ai_pTarget) <= 15) if (!Random(3))
    if (Contained() == Contained(ai_pTarget))
        if (!WildcardMatch(GetAction(), "Ride*")) // Nicht ganz so leicht vom Pferd stoßen lassen
          // Clonks kämpfen
          return FightWith(ai_pTarget);
  // Im Sprung normalerweise keine weiteren Aktionen, damit der Sprung nicht abgebrochen wird
  if (GetProcedure() == "FLIGHT" || GetCommand() == "Jump") return true;
  // Normales Gehen: Zum Gegner lieber laufen, wenn es geht
  var iPhase;
  if (ai_pTarget) if (GetAction() == "Walk")
    {
    iPhase = GetPhase();
    if (SetAction("Run")) SetPhase(iPhase);
    }
  // Keine Spezialbehandlung
  }
  
protected func AI_WaterJumpOut(object pThis, int iJumpDir)
  {
  //Log("AI_WaterJumpOut %v %v", pThis, iJumpDir);
  if (GetProcedure() != "SWIM") return;
  if (iJumpDir>0)
    {
    SetComDir(COMD_Right);
    SetDir(DIR_Right);
    }
  else
    {
    SetComDir(COMD_Left);
    SetDir(DIR_Left);
    }
  SetXDir(BoundBy(GetPhysical("Swim")/2500,24,38) * iJumpDir);
  return Call("ControlUpDouble");
  }
  
private func AI_SimJumpCheck(int iDir) // iDir =-1 oder +1
  {
  // Sprungparameter ermitteln
  var x=GetX(), y=GetY();
  var vx = GetPhysical("Walk")*iDir*7/250, vy = -GetPhysical("Jump")/10;
  var vyo = vy;
  //Log("SimFlight %d %d   %d %d",x,y,vx,vy);
  if (!SimFlight(x, y, vx, vy, 25, 1000, -1, 1000)) return false; // Bodenlos: Kein Sprung
  //Log("->SimFlight %d %d   %d %d",x,y,vx,vy);
  // Nicht zu tief nach unten, oder gegen Wand/Decke
  if (!Inside(vy, 0, vyo*-2)) return false;
  // Nicht in Lava oder Säure
  var iTargetMat = GetMaterial(x,y);
  if(GetMaterialVal("Incindiary", "Material", iTargetMat) || GetMaterialVal("Corrosive", "Material", iTargetMat)) return false;
  // Ansonsten OK
  return true;
  }


/* Werfen */

private func AI_ExecThrow()
  {
  // Kann jetzt werfen?
  if (GetProcedure() != "WALK" && !IsAiming()) return;
  // Sofort werfen wenn Gegner in Schussbahn
  // Irgendwas zum Werfen dabei?
  var iThrowWdt = 55;
  var pProjectile;
  if (pProjectile = FindContents(SPER))
    iThrowWdt = 100;
  else if (!(pProjectile = FindContents(TSWD)))
    {
    // Brennend bevorzugen
    if (!(pProjectile = FindObject2(Find_Container(this), Find_OCF(OCF_OnFire))))
      {
      // Ansonsten alle Inhaltsobjekte durchgehen, und mit Blacklist (Heiltränke, etc) vergleichen
      var i, bBreak;
      for(;;)
        {
        if (!(pProjectile = Contents(i)))
          {
          // Kein Projektil gefunden
          return;
          }
	++i;
        if (GetIndexOf(GetID(pProjectile), g_EA69_aNoThrow) >= 0) continue;
        if (IsKnight())		if (GetIndexOf(GetID(pProjectile), g_EA69_aNoThrowKnight) >= 0) continue;
        break;
        }
      }
    }
  // Nächste Gegnerposition ermitteln
  var pEnemy = AI_FindNearestEnemy();
  var dx = GetX(pEnemy) - GetX(), dy = GetY(pEnemy) - GetY(), fThrow;
  if (!dx) return; // Fängt auch pEnemy=0 ab
  var dir = dx/Abs(dx); dx = Abs(dx);
  fThrow = Inside(dx, iThrowWdt - 10, iThrowWdt + 10);
  if (!fThrow) return;
  // Gegner anschauen
  SetDir((dir+1)/2);
  // k, Wurf! Mit Spezialbehandlung beim Speer und Zweihänder
  if (pProjectile->~ControlThrow(this)) return true;
  // Normalerweise ungezielt werfen; ab und zu mal einen Wurf mit Zielposition für Bewegung
  if (!Random(5)) return AI_SetCommand("Throw", pProjectile, GetX(pEnemy), GetY(pEnemy));
  // Beim Reiten muss direkt geworfen werden, weil die engineinterne Steuerung es nicht packt
  if (WildcardMatch(GetAction(), "Ride*"))
    {
    ShiftContents(this, false, GetID(pProjectile));
    return ControlThrow();
    }
  return AI_SetCommand("Throw", pProjectile);
  }



/* Nahkampf */

private func AI_ExecFight()
  {
  // Wenig Energie: Heilung wenn möglich
  if (GetEnergy() < 15) if (AI_Heal()) return true;
  // Fernkämpfer wollen eigentlich gar nicht
  if (!Random(15))
    {
    var pOpp = GetActionTarget();
    SetAction("Walk");
    if (GetX() < GetX(pOpp)) SetComDir(COMD_Left); else SetComDir(COMD_Right);
    AI_SetCommand("MoveTo", 0, GetX()+(GetX() >= GetX(pOpp))*50-25, GetY());
    return AI_ExecMovement();
    }
  return true;
  }

/* Ritterangriff */

private func AI_ExecKnight()
  {
  // Nachladen: Immer OK (wird von ExecuteAttackBow falsch behandelt)
  if (IsLoading()) return true;
  // Bogen dabei?
  if (IsBowEquipped()) if (ExecuteAttackBow(ai_pTarget)) return true;
  // Wenig Energie: Heilung wenn möglich
  if (GetEnergy() < 15) if (AI_Heal()) return true;
  // 2do
  }

private func AI_Heal()
  {
  // Wird schon geheilt?
  if (GetEffect("Cure*PSpell", this)) return;
  // Versuchen, an Energie zu kommen
  // Brote? Heiltränke? Alles essbare ist gut!
  var pFood;
  if (pFood = FindObject2(Find_Container(this), Find_OCF(OCF_Edible)))
    {
    // Clonk kämpft? Brotzeit!
    if (GetProcedure() == "FIGHT")
      SetAction("Walk");
    else if (GetProcedure() != "WALK")
      // Ansonsten essen, wenn Zeit dazu ist
      return true;
    if (ProtectedCall(pFood, "Activate", this))
      return true;
    }
  // Heilgegenstand in der Nähe suchen
  for (pFood in FindObjects(Find_InRect(-150,-20,300,70), Find_OCF(OCF_Edible)))
    if (!Contained(pFood) || !GetAlive(Contained(pFood)))
      return AI_SetCommand("Get", pFood);
  // Kontor suchen
  if(pFood = FindObject(CPOF, -1, -1))
    if(ObjectDistance(pFood)<500) return AI_SetCommand("Enter", pFood);
  // Kein Erfolg: Nicht heilen
  }

/* Gegnersuche */

private func AI_FindNearestEnemy()
  {
//  var cont = Contained(); if (!cont) cont = NoContainer();
  var obj;
  // Objekte prüfen:
  // Muss leben und im Freien sein, wenn die KI auch im Freien ist...
  while (obj = FindObject(0, 0,0,-1,-1, OCF_Alive, 0,0, 0, obj))
    if (ObjectDistance(obj) > ai_iRange) break;    // ...nicht zu weit weg sein...
      else if ((GetOwner()<0 && GetOwner(obj)>=0) || Hostile(GetOwner(obj), GetOwner())) // ...verfeindet...
        if (GetRGBaValue(GetClrModulation(obj), 0)<200 || ObjectDistance(obj) <= 15 || GetAction(obj)=="Throw") // ...sichtbar... (2do: Richtiger Check)
          if (GetOCF(obj) & OCF_CrewMember)          // ...gegnerischer CrewMember...
            {
            if (!PathFree(GetX(), GetY(), GetX(obj), GetY(obj))) // ...in Sicht...
              if (!PathFree(GetX()+(GetX(obj)>GetX())*40-20, GetY()-20, GetX(obj), GetY(obj))) // ...oder in "Sprung/Wurfsicht"....
              {
                AI_SetCommand("MoveTo", 0, GetX(obj), GetY(obj));
                continue;
              }
            return obj; // OK, den nehmen!
            }
  }

private func FindWayPoint(iX, iY)
{
  var pPoint;
  while(pPoint = FindObject(WPNT, 0, 0, -1, -1, 0, 0, 0, 0, pPoint))
    if(PathFree(iX, iY, GetX(pPoint), GetY(pPoint))) return pPoint;
}

public func FindPath(iX, iY)
{
  var pTarget = FindWayPoint(iX, iY);
  var pPoint = FindWayPoint(GetX(), GetY());
  if(!pPoint || !pTarget) return;
  var aWay = RandomPath(pPoint, pTarget);
  var i = GetLength(aWay);
  var pNext;
  while(--i>=0)
  {
    if(pNext)
    {
      aWay[i]->DoMoveTo(pNext, this);
    }
    pNext = aWay[i];
  }
  ai_aWay = aWay;
  return 1;
//  AddCommand(0, "Call", this, 0,0, 0, 0, "AI_NextPoint");
}

public func AI_NextPoint()
{
  if(GetLength(ai_aWay)<=1) return true;
  ai_aWay[0]->DoMoveTo(ai_aWay[1], this);
  DeleteArrayItem(0, ai_aWay);
  return true;
}

public func RandomPath(pStart, pTarget)
{
  if(!pStart) return Log("No start point!");
  if(!pTarget) return Log("No target point!");
  var aWay = [pStart];
  var aTryed = [pStart];
  var iRandom;
  var pNext;
  var j = 0;
  for(var i = 1; i < 500; i++)
  {
    // Anscheinend unmöglich :-(
    if(i <= 0) return 0;
    // Zu lang gebraucht :-(
    if(j++ > 100) { break;}
    // Nächsten Weg wüfeln
    iRandom = aWay[i-1]->GetRandomWay(aTryed);
    // Keiner mehr da? -> Sackgasse, zurück gehen
    if(iRandom==-1) { SetLength(aWay, i-1); i-=2; continue; }
    // Punkt hohlen und speichern
    pNext = aWay[i-1]->GetConnected(iRandom);
    aWay[i] = pNext;
    PushBack(pNext, aTryed);
    // Sind wir schon da?
    if(pNext == pTarget) break;
  }
/*  pNext = 0;
  for(var pPoint in aWay)
  {
    if(pNext)  DrawParticleLine("NoGravSpark", GetX(pNext)-GetX(), GetY(pNext)-GetY(), GetX(pPoint)-GetX(), GetY(pPoint)-GetY(), 10, 50, RGB(255), RGB(0,255));
    pNext = pPoint;
  }*/
  return aWay;
}

/* Commands */

private func AI_SetCommand(string szCom, object pTarget, int tx, int ty, object pTarget2, int iData, int iRetries)
  {
  // Zielaktion stoppen
  this->~StopAiming();
  //Log("Setting %s %v %d %d %v %d %d", szCom, pTarget, tx, ty, pTarget2, iData, iRetries);
  // Mit Call-Command versehen, damit Fehlschlag kein Hmmm verursacht
  SetCommand(0, "Call", this, 0,0, 0, "AI_Command");
  var res = AddCommand(0, szCom, pTarget, tx, ty, pTarget2, iData, iRetries, C4CMD_SilentBase);
  // Bei Bewegungskommandos gleich die Bewegung checken
  if (szCom == "MoveTo" || szCom == "Follow" || szCom == "Attack" || (szCom == "Throw" && tx) || szCom == "Enter")
  {
    if(pTarget) FindPath(GetX(pTarget), GetY(pTarget));
    else FindPath(tx, ty);
    AI_ExecMovement();
  }
  return res;
  }

public func AI_Command() { return true; }
public func AI_CommandFailed() { return true; }

private func AI_GetStackedCommand(string szCom)
  {
  // Alle Commands durchcheken
  var i=-1, szCommand;
  while (szCommand = GetCommand(0, 0, ++i))
    if (!szCom || szCommand == szCom)
      return i;
  // Nichts gefunden
  return -1;
  }

public func OnAIIdle() { return _inherited(); }

public func Attack(object pEnemy) { return _inherited(pEnemy); }

// Ritterfunktionen: Definition der Basisfunktionen im Clonk
private func GetObject2Drop(object pCollObj) { return _inherited(pCollObj); }
private func IsBowEquipped() { return _inherited(); }
private func ExecuteAttackBow(object pTarget) { return _inherited(pTarget); }
private func ExecuteAttackMuskete(object pTarget) { return _inherited(pTarget); }
private func ExecuteAttackGun(object pTarget) { return _inherited(pTarget); }
private func IsAiming() { return _inherited(); }
private func IsKnight() { return _inherited(); }
private func IsPirate() { return _inherited(); }
private func IsLoading() { return _inherited(); }
private func HasStaff() { return _inherited(); }
private func GetAimingDeviation() { return 5; }
private func SpellSucceeded(a,b,c,d,e) { return _inherited(a,b,c,d,e); }
private func SpellFailed(a,b,c,d,e) { return _inherited(a,b,c,d,e); }
public func CanUse(a,b,c,d,e,f,g) { return _inherited(a,b,c,d,e,f,g); }
public func ControlThrow(a,b,c,d,e,f,g) { return _inherited(a,b,c,d,e,f,g); }

// Fernkämpfer?
public func AI_IsRanged()
  {
  if (IsBowEquipped()) return true;
  }

public func IsAI()
  {
  return (!!ai_iRange);
  }

