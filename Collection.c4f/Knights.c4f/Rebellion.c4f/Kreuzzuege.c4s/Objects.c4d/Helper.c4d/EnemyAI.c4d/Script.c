/*-- Gegner KI --*/

#strict

#appendto CLNK

static const _EAI_EnemySearchRange       = 400, // * Pixel nach d2
             _EAI_ItemSearchRange        = 200, // * Pixel nach d2
             _EAI_FightRange             =  15, // * Pixel nach d2
             _EAI_MoveToTolerance        =   5, // * Pixel nach d2
             _EAI_TimerInterval          =   3, // * Frames
             _EAI_EnemeySearchInterval   =   4, // * Timer
             _EAI_ProjectileJumpOffAngle =  45, // °
             _EAI_OnFireSearchWaterRetries= 100, // Versuche / Timer
             _EAI_OnFireSearchWaterRangeX = 401, // * Pixel
             _EAI_OnFireSearchWaterRangeY = 301, // * Pixel
             _EAI_MaxHomePath             = 600, // * Pixel nach d2
             _EAI_ArrowReplenishInterval  = 700, // * Timer (% Random), ca. 1/Minute
             _EAI_RuleID                  = _EAI;
             
static const _EAI_MagiRandCast = 100; // Chance, jeweils einen bestimmten Zauber auszuführen

local ai_iRange;                         // Suchweite für Gegner
local ai_iHomeX, ai_iHomeY, ai_iHomeDir; // Eingenommene Position, wenn kein Gegner gefunden
local ai_pTarget;                        // Verfolgter Gegner
local ai_idFirstEncounterCB;             // Callback im Szenarienscript beim ersten Feindkontakt (AI_Encounter%i) - Par0 = this
local ai_isEndboss;                      // Endgegner-Magier: Hält etwas länger
local ai_iStrength;                      // Gegnerstärke (Energie, Mana, etc. - 100=Normal)
local ai_pTarget2;                       // Sekundärziel für Endboss (Hügelverteidigung)


/* Initialisierung */

public func SetAI(object pATarget, int iARange)
  {
  if (pATarget) return(pATarget->SetAI(0, iARange));
  if (!iARange) iARange = _EAI_EnemySearchRange;
  if (!GetAlive()) return();
  // Werte setzen
  ai_iRange = iARange;
  ai_iHomeX = GetX(); ai_iHomeY = GetY(); ai_iHomeDir = GetDir();
  // Clonk quetschen
  if(!Random(3))
  {
    var a = Random(225)-100;
    SetObjDrawTransform(1000+a,0,0,0,1000-a,0);
  }
  // Energieleiste
  CreateObject(_EBR,,-1)->AttachTo(this());
  // Färben
  CLFS->Colorize(this());
  // Timer erstellen
  if (GetEffect("AI", this())) return();
  return(AddEffect("AI", this(), 1, _EAI_TimerInterval, this()));
  }

// Temporäres Deaktivieren der KI  
public func AI_Disable() { ai_iRange = 0; }
public func AI_Enable(int iARange) { if (!(ai_iRange = iARange)) ai_iRange = _EAI_EnemySearchRange; }

public func AI_SetFirstEncounterCB(id idToCB)
  {
  // CB setzen
  ai_idFirstEncounterCB = idToCB;
  return(true);
  }

protected func Initialize(a,b,c,d,e,f,g,h,i,j)
  {
  // KI aktiviert: Alle neutralen Clonks steuern
  if(ObjectCount(_EAI_RuleID) && GetOwner()<0 && !ai_iRange) SetAI();
  return(_inherited(a,b,c,d,e,f,g,h,i,j));
  }



/* Timer */

protected func FxAIStart(object pTarget, int iEffectNumber, int iTemp)
  {
  // Anfangsausrüstung
  if (!iTemp)
    pTarget->AI_Equip();
  }

protected func FxAITimer()
  {
  var obj,dx,dy,vx,vy,dir,fMagi,i,fInLava;
  
  // Nicht wenn deaktiviert
  if (!ai_iRange) return(DoEnergy(10));
  
  // In der Magieaktion: Absolut gar nichts tun, damit sie nicht abgebrochen wird!
  if (WildcardMatch(GetAction(), "*Magic")) return();

  // Gegner bestimmen:
  // Gegner aktualisieren
  if (!Random(_EAI_EnemeySearchInterval))
    ai_pTarget = AI_FindNearestEnemy();

  // Cheatzeug:
  // Magiclonks kriegen ständig Mana
  if (fMagi = IsMagiclonk()) DoMagicEnergy(ai_isEndboss*2+1);
  // Ritter mit Bogen kriegen Pfeile
  if (!Random(_EAI_ArrowReplenishInterval))
    if (IsKnight() && FindContents(BOW1))
      if (!FindContents(FARP) && !FindContents(XARP))
        if (!FindContents(FARW) && !FindContents(XARW))
          if (!Random(2)) CreateContents(XARP); else CreateContents(FARP);

  // Contained: Erstmal raus, wenn nicht drinnen am Kämpfen
  if (Contained())
    {
    /* Anm.: Eigentlich würde Kämpfen in Gebäuden auch
       von AI_ExecMovement erledigt... */
    // Mit Gegner im Haus: OK
    if (ai_pTarget && (Contained()==Contained(ai_pTarget)))
    {
      // Zum Kämpfen zwingen
      if(GetOCF(ai_pTarget) & OCF_FightReady)
        FightWith(ai_pTarget);
      // Drinbleiben
      return(AI_SetCommand("None"));
    }
/*    // Holt etwas -> auch OK
    if(AI_GetStackedCommand("Get") >= 0) return(true);
    // Transfer führt den Clonk manchmal in Burgteile
    if(AI_GetStackedCommand("Transfer") >= 0) return(true);*/
    // Bei Get- oder Transfer-Kommandos drinbleiben
    var i=-1, szCommand;
    while(szCommand = GetCommand(0, 0, ++i))
      if(szCommand eq "Get" || szCommand eq "Transfer")
        return(true);
    // Sonst raus
    return(AI_SetCommand("Exit"));
    }
                    
  // Sofortreaktionen: Tlw. adaptiert von Newtons TheTemple-KI
  // Projektilen ausweichen
  while (obj=FindObject(0,-30,-50,60,80, OCF_HitSpeed2, 0,0, NoContainer(), obj))
    if (GetCategory(obj) & C4D_Object)
      {
      dx = GetX(obj)-GetX(); dy = GetY(obj)-GetY(); // Bugfix: Statt GetY() war hier GetX()!
      vx = GetXDir(obj); vy = GetYDir(obj); dir = vx/Abs(vx);
      if (!dir) dir = Random(2)*2-1;
      // VecAbstand * VecGeschwindigkeit < -Cos(Toleranz)*|VecAbstand|*|VecGeschwindigkeit|)
      if (dx*vx+dy*vy < Cos(_EAI_ProjectileJumpOffAngle, 100)*Sqrt(dx*dx+dy*dy)*Sqrt(vx*vx+vy*vy)/-100)
        {
        if (!AI_Evade(dir, obj))
          if (!AI_Evade(-dir, obj))
            // Nowhere to go :o
            return(AI_Panic());
        return(true);
        }
      }

  // Brennend: Löschaktionen
  if (OnFire())
    {
    // Löschzauber? Nicht in Lava schwimmend!
    fInLava = GetMaterialVal("Incindiary", "Material", GetMaterial());
    if (!fInLava) if (AI_DoMagic(EXTG)) return(true);
    // Wasserfass dabei?
    if (!fInLava) if (obj = FindContents(WBRL))
      return(obj->Activate(this()));
    if (!fInLava) if (obj = FindContents(WDWB))
      return(obj->Activate(this()));
    // Holt schon ein Wasserfass?
    i = AI_GetStackedCommand("Get");
    if (i >= 0)
      if(GetID(GetCommand(0,1,i))==WBRL || GetID(GetCommand(0,1,i))==WDWB)
        return(true); // Weiter so
    // Wasserfass suchen
    var barrel, obj, dist;
    for(obj in FindObjects(Find_Distance(_EAI_ItemSearchRange), Find_Or(Find_ID(WBRL), Find_ID(WDWB))))
      if(ObjectDistance(obj)<dist || !dist)
      { 
        barrel = obj;
        dist = ObjectDistance(obj);
      }
    if(barrel)
      return(AI_SetCommand("Get", barrel));
    // Schon auf dem Weg ins Wasser?
    // Alle Commands durchchecken
    var i=-1, szCommand;
    while(szCommand = GetCommand(0, 0, ++i))
      if(szCommand eq "MoveTo")
        if(GetMaterialVal("Extinguisher", "Material", GetMaterial(AbsX(GetCommand(0, 2, i)),AbsY(GetCommand(0, 3, i)))) ||  // Bugfix: Hier fehlte AbsX und AbsY!
           GetMaterialVal("Extinguisher", "Material", GetMaterial(AbsX(GetCommand(0, 2, i)),AbsY(GetCommand(0, 3, i)+10))))
          return(true); // Weiter so
    // Wasser in der Nähe?
    i = _EAI_OnFireSearchWaterRetries;
    while (i--)
      {
      dx = Random(_EAI_OnFireSearchWaterRangeX) - _EAI_OnFireSearchWaterRangeX/2;
      dy = Random(_EAI_OnFireSearchWaterRangeY) - _EAI_OnFireSearchWaterRangeY/2;
      if(GetMaterialVal("Extinguisher", "Material", GetMaterial(dx,dy)) &&
         (GetMaterialVal("Extinguisher", "Material", GetMaterial(dx,dy+5)) ||  // <- Zusätzliche Überprüfung, um Löschversuche in 1-Pixel-Schichten zu vermeiden!
          GetMaterialVal("Extinguisher", "Material", GetMaterial(dx,dy-5))) )
        return(AI_SetCommand("MoveTo", 0, GetX()+dx, GetY()+dy));
      }
    // Keine Löschmöglichkeit: Zum nächsten Gegner laufen
    obj = AI_FindNearestEnemy(); if (!obj) obj = ai_pTarget;
    if (obj) return(AI_SetCommand("Follow", obj));
    // Aaaah!
    return(AI_Panic());
    }

  // Endmagier: Angreifer von links abwehren
/*  if (ai_isEndboss) if (!Random(20))
    if (Inside(GetX(), 1530, 1660))
      if (Inside(GetY(), 500, 590))
        while (obj = FindObject(0, -300,-300,300,700, OCF_Alive, 0,0, NoContainer(), obj))
          if (ObjectDistance(obj) < 200)
            if ((GetOwner()<0 && GetOwner(obj)>=0) || Hostile(GetOwner(obj), GetOwner())) // ...verfeindet...
              // if (!GetVisibility(obj)) - er Endmagier kann Gegner riechen
              if (GetOCF(obj) & OCF_CrewMember)          // ...gegnerischer CrewMember...
                {
                // Abwehren!
                SetDir(DIR_Left);
                ai_pTarget2 = obj;
                if (AI_DoMagic(ABLA)) return(true);
                }
*/

  // Essen und trinken
  if (AI_Eat()) return(true);

  // Kein Gegner
  if (!ai_pTarget)
  { 
    // Ritter ohne Waffe: Eine holen
    if(IsKnight()) if(!HasFightWeapon()) if(AI_GetFightWeapon()) return(true);
    // Essen suchen, wenn nötig
    if(AI_GetFood()) return(true);
    // Homeposition
    return(AI_Home());
  }
    
  // Normale Bewegungsaktionen
  if (AI_ExecMovement()) return(true);

  // Magier: Zaubern
  if (AI_CanCast()) if (AI_ExecMagician()) return(true);

  // Werfen wenn nötig
  if (Contents()) if (AI_ExecThrow()) return(true);

  // Ritter: Waffen benutzen
  if (IsKnight()) if (AI_ExecKnight()) return(true);

  // Nahkampfangriff
  if (!AI_IsRanged())
    if (!GetCommand() || (GetCommand() eq "MoveTo" && !Random(2)))
      if (!Contained(ai_pTarget))
        AI_SetCommand("Attack", ai_pTarget);
      else
        AI_SetCommand("Enter", Contained(ai_pTarget));

  // OK; Angriffsbefehl ausführen
  return(true);
  }


/* Essen & Trinken */

private func AI_Eat()
{
  var pFood;
  // Anm.: IsFood ist eine globale Funktion in System.c4g\Helper.c
  pFood = FindObject2(Find_Container(this()), Find_Or(Find_Func("IsFood"), Find_Func("IsPotion")));
  // Nichts zu Essen oder Trinken da?
  if(!pFood) return(false);
  //Log("Food found: %s", GetName(pFood));
  // Energie verloren? -> Essen!
  if(IsFood(pFood))
    if(GetEnergy() < GetPhysical("Energy",0)/1000)
      return(true, pFood->Activate(this()));
  // Tränke sofort trinken (TODO: vllt. besser einsetzen...)
  if(pFood->~IsPotion()) 
    return(true, pFood->Activate(this()));
}

// Holt Nahrung zu Heilungszwecken
private func AI_GetFood()
{
  var pFood, iDist, pObj;
  // Braucht was zu essen? (< 50% Energie)
  if(GetEnergy() > GetPhysical("Energy",0)/2000)
    return(false);
  // Holt schon etwas?
  if(AI_GetStackedCommand("Get") > -1) return(true);
  // Essen in der Nähe?
  for(pObj in FindObjects(Find_Distance(_EAI_ItemSearchRange), Find_Or(Find_Func("IsFood"), Find_ID(PHEA))))
    if(ObjectDistance(pObj) < iDist || !iDist)
    {
      if(Contained(pObj)) if(Contained(pObj)->~IsClonk()) continue;
      iDist = ObjectDistance(pObj);
      pFood = pObj;
    }
  // Nichts gefunden?
  if(!pFood) return(false);
  // Essen holen (Gegessen wird durch AI_Eat)
  AI_SetCommand("Get", pFood, 0, 0, 0, 0, 5);
  return(true);
}

/* Bewegung */

private func AI_Home()
{
  // Etwas angefasst? -> Loslassen!
  if (GetProcedure() eq "PUSH")
    return(AI_SetCommand("UnGrab"));
  // X-Richtung reicht fürs Home
  if (Abs(GetX() - ai_iHomeX) > _EAI_MoveToTolerance || Contained())
  {
    // Magier kann sich auch warpen
    if (AI_DoMagic(_MWP)) { AI_SetCommand("None"); return(true); }
    // Schon auf dem Weg?
    var i=-1, szCommand;
    if(Random(15)) // Manchmal auch Überschreiben des Kommandos erlauben...
      while(szCommand = GetCommand(0, 0, ++i))
        if(szCommand eq "MoveTo")
          if(Abs(GetCommand(0,2,i) - ai_iHomeX) <= _EAI_MoveToTolerance /*&&
             Abs(GetCommand(0,3,i) - ai_iHomeY) <= _EAI_MoveToTolerance*3*/)
            return(AI_ExecMovement()); // Weiter so
    // Nach Hause gehen...
    AI_SetCommand("MoveTo", 0, ai_iHomeX, ai_iHomeY);
    return(AI_ExecMovement());
  }
  // @ Home
  if (GetProcedure() eq "WALK") SetDir(ai_iHomeDir);
  // Wenig Energie: Heilung
  if (GetEnergy() < 15) if (AI_DoMagic(MGHL)) return(true);
  // KI-Clonks regenerieren auch so (laaangsam)
  if (!Random(50)) DoEnergy(1);
  return(true);
}

private func AI_Evade(int iDir, object pProjectile) // iDir in {-1,+1}
  {
  //Log("AI_Evade %d", iDir);
  // Magier-Ausweichen
  // Kraftfeld kommt ggf. zu spät, aber schützt dann vor zusätzlichen Pfeilen
  if (!Random(5))
    if (AI_DoMagic(CMFG, ARRW, 50)) return(true);
  if (!Random(5) && pProjectile) if (pProjectile->~IsArrow())
    {
    var iOldDir = GetDir();
    SetDir((1-iDir)/2);
    if (AI_DoMagic(MFFW)) return(true);
    SetDir(iOldDir);
    }
  // Windstoß gegen Projektile
  if (!Random(5) && pProjectile)
    {
    var iOldDir = GetDir();
    SetDir((1-iDir)/2);
    if (AI_DoMagic(ABLA, SPHR, 70)) return(true);
    SetDir(iOldDir);
    }
  // Sprung frei?
  if (PathFree(GetX(), GetY(), GetX()+25*iDir, GetY()-20))
    return(AddCommand(0, "Jump", 0, iDir) && SetComDir(5-2*iDir), AI_ExecMovement());
  // Weg frei?
  if (PathFree(GetX(), GetY(), GetX()+30*iDir, GetY()))
    return(AddCommand(0, "MoveTo", 0, GetX()+30*iDir,GetY()), AI_ExecMovement());
  // Nix frei :(
  }

private func AI_Panic()
  {
  // Tarnung: Nicht wenn brennend (Lieber Mana sparen)
  if (!OnFire())
    if (!Random(2)) if (AI_DoMagic(CMFG, ARRW, 50)) return(true);
  // Warp - deaktiviert fürs Szenario
  //if (!Random(2)) if (AI_DoMagic(MGWP)) return(true);
  // Aaah!11
  if (GetProcedure() ne "WALK") return(true);
  Jump(); SetXDir(0); SetYDir(GetYDir()/2);
  SetAction("Jump");
  Sound("Scream");
  return(true);
  }

private func AI_ExecMovement()
{
  //Log("ExecMovement: %s %s", GetProcedure(), GetCommand());
  var obj;
  var iTargetX = GetCommand(0, 2);
  // Sprünge während der normalen Bewegung
  if (GetProcedure() ne "FLIGHT" && (GetCommand() eq "MoveTo" || GetCommand() eq "Attack" || GetCommand() eq "Follow" || (GetCommand() eq "Throw" && iTargetX)))
  {
    var fShouldJump, fAnySolid;
    if (!iTargetX) iTargetX = GetX(GetCommand(0, 1));
    // Über Brennendes springen
    if(!OnFire())
      if(obj=FindObject(0,-40*!GetDir(),-40,40,80, OCF_OnFire, 0,0, NoContainer()))
        fShouldJump = true;
    // Über Lava/Säurebecken/Tiefe Löcher springen
    var cx=40*GetDir()-20;
    if(GetAction() ne "Jump" && !InLiquid())
      for(var j=1; j<50; j+=5)
        if(GetMaterialVal("Incindiary", "Material", GetMaterial(cx,j)))
          fShouldJump = true;
        else if (GBackSolid(cx, j))
          fAnySolid = j;
    if (fShouldJump || !fAnySolid)
      // Will/kann die KI überhaupt über dieses Loch?
      if (Abs(GetX() - iTargetX) > 40 && AI_SimJumpCheck(cx/20))
        // Dann springen
        return(AddCommand(0, "Jump"));
      else
      {
        // Ansonsten stehen bleiben
        AI_SetCommand("None");
        SetComDir(COMD_Stop);
      }
  }
  // Schwimmt?
  if(GetProcedure() eq "SWIM")
  {
    var iTargetX = GetCommand(0, 2);
    if (!iTargetX) iTargetX = GetX(GetCommand(0, 1));
    // MoveTo-Commands machen im Wasser öfter Probleme...
    AI_SetCommand("None");
    // Nur für Seeförmige Wasserflächen...
    if(GetMaterial(0,-1) == Material("Water"))
      return(SetComDir(COMD_Up));
    else
//      if(ai_iHomeX < GetX())
      if(iTargetX < GetX())
        return(SetComDir(COMD_Left));
      else
        return(SetComDir(COMD_Right));
  }
  // Kein Kommando mehr: Erst einmal anhalten
  if (!GetCommand()) SetComDir(COMD_Stop);
  // Nahkampf:
  // Schon am Kämpfen?
  if (GetProcedure() eq "FIGHT") return(true);
  // Gegner in Reichweite für Nahkampf?
  if (ObjectDistance(ai_pTarget) <= _EAI_FightRange) if (!Random(3))
    if (Contained() == Contained(ai_pTarget))
      return(FightWith(ai_pTarget));
  // Im Sprung normalerweise keine weiteren Aktionen, damit der Sprung nicht abgebrochen wird
  if (GetProcedure() eq "FLIGHT" || GetCommand() eq "Jump") return(true);
  // Keine Spezialbehandlung
}
  
private func AI_SimJumpCheck(int iDir) // iDir =-1 oder +1
  {
  // Sprungparameter ermitteln
  var x=GetX(), y=GetY();
  var vx = GetPhysical("Walk")*iDir*7/250, vy = -GetPhysical("Jump")/10;
  var vyo = vy;
  if(!SimFlight(x, y, vx, vy, 25, 1000, -1, 1000)) return(false); // Bodenlos: Kein Sprung
  // Nicht zu tief nach unten, oder gegen Wand/Decke
  if(!Inside(vy, 0, vyo*-2)) return(false);
  // Nicht in Lava oder Säure
  var iTargetMat = GetMaterial(AbsX(x),AbsY(y)); // Bugfix: hier fehlte AbsX und AbsY!
  if(GetMaterialVal("Incindiary", "Material", iTargetMat) || GetMaterialVal("Corrosive", "Material", iTargetMat)) return(false);
  // Ansonsten OK
  return(true);
  }


/* Werfen */

private func AI_ExecThrow()
  {
  // Kann jetzt werfen?
  if (GetProcedure() ne "WALK" && !IsAiming()) return();
  // Sofort werfen wenn Gegner in Schussbahn
  // Irgendwas zum Werfen dabei?
  var iThrowWdt = 55;
  var pProjectile = FindContents(SPER);
  if (!pProjectile) pProjectile = GetObject2Drop(); else iThrowWdt = 100;
  if (!pProjectile) return();
//  Log("pProjectile: %s", GetName(pProjectile));
  // Nächste Gegnerposition ermitteln
  var pEnemy = AI_FindNearestEnemy(true);
  var dx = GetX(pEnemy) - GetX(), dy = GetY(pEnemy) - GetY(), fThrow;
  if (!dx) return(); // Fängt auch pEnemy=0 ab
  var dir = dx/Abs(dx); dx = Abs(dx);
  // "In etwa" in der Wurfbahn?
  if (Inside(dy, -30, -5))
    fThrow = Inside(Angle(0,0,dx,dy), 20, 60);
  else
    fThrow = Inside(dx, iThrowWdt - 10, iThrowWdt + 10);
  if (!fThrow) return();
  // Gegner anschauen
  SetDir((dir+1)/2);
  // k, Wurf! Mit Spezialbehandlung beim Speer
  // Normalerweise ungezielt werfen; ab und zu mal einen Wurf mit Zielposition für Bewegung
  if (pProjectile->~ControlThrow(this())) return(true);
  if (!Random(5)) return(AI_SetCommand("Throw", pProjectile, GetX(pEnemy), GetY(pEnemy)));
  return(AI_SetCommand("Throw", pProjectile));
  }



/* Ritterangriff */

private func AI_ExecKnight()
{
  // Nachladen: Immer OK (wird von ExecuteAttackBow falsch behandelt)
  if (IsLoading()) return(true);
  // Bogen dabei?
  if (IsBowEquipped()) if (ExecuteAttackBow(ai_pTarget)) return(true);
  // 2do
  return(false);
}


private func AI_GetFightWeapon()
{
  var pWeapon, iDist, pObj;
  // Hat schon eine Waffe?
  if(HasFightWeapon()) return(false);
  // Holt eine Waffe?
  if(AI_GetStackedCommand("Get") > -1)  return(true);
  // Schwert in der Nähe?
  for(pObj in FindObjects(Find_Distance(_EAI_ItemSearchRange), Find_ID(SWOR)))
    if(ObjectDistance(pObj) < iDist || !iDist)
    {
      if(Contained(pObj)) if(Contained(pObj)->~IsClonk()) continue;
      iDist = ObjectDistance(pObj);
      pWeapon = pObj;
    }
  // Kein Schwert? Axt in der Nähe?
  if(!pWeapon)
  for(pObj in FindObjects(Find_Distance(_EAI_ItemSearchRange), Find_ID(AXE1)))
    if(ObjectDistance(pObj) < iDist || !iDist)
    {
      if(Contained(pObj)) if(Contained(pObj)->~IsClonk()) continue;
      iDist = ObjectDistance(pObj);
      pWeapon = pObj;
    }
  // Immer noch keine geeignete Waffe?
  if(!pWeapon) return(false);
  // Kommando geben
  AI_SetCommand("Get", pWeapon, 0, 0, 0, 0, 5);
  return(true);
}



/* Zauberei */

private func AI_ExecMagician()
  {
  
  // Wenig Energie: Heilung bzw. Elixier
  if (GetEnergy() < 15)
    {
    var idHeal; if (!Random(3)) idHeal = ELX1; else idHeal = MGHL;
    if (AI_DoMagic(idHeal)) return(true);
    }
    
  // Gegner fasst etwas an (Zelt, Luftschiff, Ballon, etc.): Blitz
  var pPushTarget;
  if (GetProcedure(ai_pTarget) eq "PUSH")
    if (pPushTarget = ai_pTarget->GetActionTarget())
      if (AI_DoMagic(MLGT)) return(true);
      
  // Gegner an Flammenposition: Ewige Flammen
  var dx = Abs(GetX(ai_pTarget)-GetX());
  var dy = GetY(ai_pTarget)-GetY();
  if (!Random(_EAI_MagiRandCast))
    if(Inside(dx, 50, 70) && dy > 0)
      if (AI_DoMagic(ETFL)) return(true);
      
  // Gegner direkt nebenan: Feuerfaust
  if (!Random(_EAI_MagiRandCast))
    if(Inside(dx, 15, 50) && dy > -5)
      if (InLiquid())
        {
        if (AI_DoMagic(FRFS, METL)) return(true);
        }
      else
        if (AI_DoMagic(FRFS)) return(true);
      
  // Magier fällt schräg: Brücke
  if (!Random(_EAI_MagiRandCast))
    if (!GetContact() && GetYDir()>5 && Abs(GetXDir()) > 5)
      if (AI_DoMagic(MBRG, METL, 70)) return(true);
      
  // Gegner erreichbar: Airblast
  if (!Random(_EAI_MagiRandCast))
    if(PathFree(GetX(), GetY(), GetX(ai_pTarget), GetY(ai_pTarget)))
      if (AI_DoMagic(ABLA, SPHR, 60)) return(true);
      
  // Gegner nahe und "müde": Schlaf
  if (!Random(_EAI_MagiRandCast))
    if(Distance(dx,dy) < 100 && GetEnergy(ai_pTarget) < GetPhysical("Energy",1,ai_pTarget)/2000)
      if (AI_DoMagic(MBOT, ROCK, 50)) return(true);
      
  // Gegner in Reichweite: Feuerball
  if (!Random(_EAI_MagiRandCast))
    if(PathFree(GetX(), GetY(), GetX(ai_pTarget), GetY(ai_pTarget)))
      {
      var idFlint, iRandom=Random(3);
           if (!iRandom--) idFlint = EFLN;
      else if (!iRandom--) idFlint = SFLN;
      else idFlint = FLNT;
      if (AI_DoMagic(MFRB, idFlint, 50)) return(true);
      }
      
  // Gegner steht voraus: Feueratem
  if (!Random(_EAI_MagiRandCast))
    if(Inside(dx, 10, 40) && Abs(dy) < 15)
      if (AI_DoMagic(MDBT)) return(true);
  
  // Gegner unterhalb: Feuerklumpen
  if (!Random(_EAI_MagiRandCast))
    if (dy > 30 && Inside(dx, 30, 200))
      if (AI_DoMagic(MFBL, FLNT, 50)) return(true);
 
  // Gegner auf einer Höhe in Gefahrenzone: Feuerwand
  if (!Random(_EAI_MagiRandCast))
    if (Inside(dx, 50, 100) && Inside(dy, -150, 15))
      if (AI_DoMagic(MFWL)) return(true);
      
  // Verbündete Clonks mit Schaden im Umkreis: Gruppenheilung oder Wiederherstellung
  var pClonk;
  if (!Random(_EAI_MagiRandCast))
    while(pClonk=FindObject(0,-80,-80,160,160,OCF_CrewMember,0,0,NoContainer(),pClonk))
      if (GetOwner(pClonk) == NO_OWNER) if(ObjectDistance(pClonk) <= 80)
          if (GetEnergy(pClonk) < GetPhysical("Energy",0,pClonk)/1500)
            {
            if (Random(2)) if (AI_DoMagic(ELX2)) return(true);
            if (AI_DoMagic(GGHG)) return(true);
            }
            
  // Der Gegner ist nahe: Zaps
  if (!Random(_EAI_MagiRandCast))
    if (Distance(dx,dy) < 45)
      if (AI_DoMagic(GZ9Z)) return(true);
      
  // Der Gegner ist nahe: Verblendung
  if (!Random(_EAI_MagiRandCast*20))
    if (Distance(dx,dy) < 60)
      if (AI_DoMagic(LGCN)) return(true);
      
  // Leichen da: Totenbelebung
  if (!Random(_EAI_MagiRandCast))
    while(pClonk=FindObject(0,-150,-150,300,300,OCF_Living,0,0,NoContainer(),pClonk))
      if(pClonk->~IsClonk())
        if(!GetAlive(pClonk))
          if(!GetEffectCount("*",pClonk))
            if (AI_DoMagic(RUND)) return(true);
            
  // Endboss: Magische Kopien, wenn sonst keine Kopien im Umkreis
/*  if (ai_isEndboss) if (!Random(2))
    if (!ObjectCount(MAGE, -60,-60,120,120, OCF_Alive, 0,0, 0, GetOwner()))
      if (AI_DoMagic(MGRP, WIPF)) return(true);
*/
              
  // Zufallszauber?
  if (!Random(_EAI_MagiRandCast)) if (AI_DoMagic(RMMG)) return(true);
  // Nix zaubern
  }

private func AI_CanCast(id idSpell)
  {
  // Nur Zauberer
  if (!IsMagiclonk()) return();
  // Zauber angegeben: Mana checken
  if (idSpell) if(!CheckMana(idSpell)) return();
  if (idSpell) if(ObjectCount(ALCO)) if(!CheckAlchem(idSpell, this())) return();
  // Bereit? (Definitionsabhängiger Actioncheck)
  return(Call("ReadyToMagic"));
  }

private func AI_DoMagic(id idSpell, id idComboObj, int iComboChance)
  {
  if (!AI_CanCast(idSpell)) return();
  // Das Ziel anschauen, für alle gerichteten Zauber
  if (ai_pTarget) SetDir(GetX(ai_pTarget) > GetX());
  // Keine Befehle, damit Magieaktion nicht abgebrochen wird
  AI_SetCommand("None");
  //Log("AI_DoMagic %s (%i)", GetName(0, idSpell), idSpell);
  // Komboobjekt erzeugen
  // - bleibt möglicherweise im Magier, wenn der Zauber durch Treffer oder ähnliches abgebrochen wird
  //   egal; ist halt ein Zauberer :)
  if (idComboObj) if (!FindContents(idComboObj))
    if (!iComboChance || Random(100) <= iComboChance)
      CreateContents(idComboObj);
  Call("DoMagic", idSpell);
  // 2do: Zielzauber und Auswahlzauber behandeln
  // Irgendwelche Nachrichten von Zaubern löschen
  Message(" ", this());
  //Message("AI_DoMagic %s (%i)", this(), GetName(0, idSpell), idSpell);
  //Log("AI_DoMagic %s (%i)", GetName(0, idSpell), idSpell);
  // Erstmal Mana auf 0, damit nicht dauerhaft gezaubert wird
  // Geht aber nicht wegen verzögerter Zauber. Ugh.
  //DoMagicEnergy(-GetMagicEnergy());
  return(true);
  }
  

/* Gegnersuche */

private func AI_FindNearestEnemy(bool fNotContained)
  {
//  var cont /*= Contained(); if (!cont) cont*/ = NoContainer();
  var cont = 0; if(fNotContained) cont = NoContainer();
  var obj;
  // Objekte prüfen:
  // Muss leben und im freien sein...
  while (obj = FindObject(0, 0,0,-1,-1, OCF_Alive, 0,0, cont, obj))  // Bugfix: Statt cont war hier NoContainer!
    if (ObjectDistance(obj) > ai_iRange) break;    // ...nicht zu weit weg sein...
      else if ((GetOwner()<0 && GetOwner(obj)>=0) || Hostile(GetOwner(obj), GetOwner())) // ...verfeindet...
        if (!GetVisibility(obj)) // ...sichtbar... (2do: Richtiger Check)
          if (GetOCF(obj) & OCF_CrewMember)          // ...gegnerischer CrewMember...
            {
            if (!PathFree(GetX(), GetY(), GetX(obj), GetY(obj))) // ...in Sicht...
              if (!PathFree(GetX()+(GetX(obj)>GetX())*40-20, GetY()-20, GetX(obj), GetY(obj))) // ...oder in "Wurfsicht"....
                continue;
            // Durch Verblendung verdeckt?
            if (FindObject(LGCN, GetX(obj)-GetX()-120, GetY(obj)-GetY()-120, 240,240)) continue;
            // Kontakt ans Szenscript melden
            if (ai_idFirstEncounterCB)
              {
              // Erster Gegnerkontakt!
              var idEncounterCB = ai_idFirstEncounterCB;
              ai_idFirstEncounterCB = 0;
              GameCall(Format("AI_Encounter%i", idEncounterCB), this(), obj);
              }
            return(obj); // OK, den nehmen!
            }
  }


/* Ausrüstung */

public func AI_Equip()
  {
  // Nur einmal
  if (Contents() || !GetAlive()) return();
  // Erzeugt manchmal etwas mehr, als ins Inventar passt. Merkt schon keiner... :)
  // Ritter: Bogen, Schwert oder Axt, Schild
  if (IsKnight())
    {
//    if (!Random(2)) { CreateContents(BOW1); CreateContents(FARP); CreateContents(XARP); }
    if (!Random(2)) 
    { 
      var pShield = CreateContents(SHIE);
      pShield->Activate(this());
      pShield->SetColorDw(GetColorDw());
    }
    // Waffe
    if (!Random(2)) CreateContents(AXE1); else CreateContents(SWOR);
    // Rüstung
    if(!Random(10*(4-iDifficulty)))
      this()->SetArmored(true, ARMR);
    }
  // Waffen für alle
  var iWep, idWep;
       if (!(iWep = Random(8))) idWep = SPER; // 1
  else if (!--iWep)             idWep = ROCK; // 2
  else if (!--iWep)             idWep = FLNT; // 3
  else if (!--iWep)             idWep = SFLN; // 4
  else if (!--iWep)             idWep = EFLN; // 5
  else if (!--iWep)             idWep = STFN; // 6
  else if (!--iWep)             idWep = FBMP; // 7
  else                          idWep = SPER; // 8 - dup 1
  CreateContents(idWep); // Bugfix: Hier wurden immer nur Speere erzeugt!
  return(true);
  }



/* Commands */

private func AI_SetCommand(string szCom, object pTarget, int tx, int ty, object pTarget2, int iData, int iRetries)
  {
  // Zielaktion stoppen
  this()->~StopAiming();
  //Log("Setting %s %s %d %d %s %d %d", szCom, GetName(pTarget), tx, ty, GetName(pTarget2), iData, iRetries);
  // Mit Call-Command versehen, damit Fehlschlag kein Hmmm verursacht
  SetCommand(0, "Call", this(), 0,0, 0, "AI_Command");
  var res = AddCommand(0, szCom, pTarget, tx, ty, pTarget2, iData, iRetries);
  // Bei Bewegungskommandos gleich die Bewegung checken
  if (szCom eq "MoveTo" || szCom eq "Follow" || szCom eq "Attack" || (szCom eq "Throw" && tx)) AI_ExecMovement();
  return(res);
  }

public func AI_Command() { return(true); }
public func AI_CommandFailed() { return(true); }

private func AI_GetStackedCommand(string szCom)
  {
  // Alle Commands durchcheken
  var i=-1, szCommand;
  while (szCommand = GetCommand(0, 0, ++i))
    if (!szCom || szCommand eq szCom)
      return(i);
  // Nichts gefunden
  return(-1);
  }

// Objekt zum Wegwerfen auswählen
private func GetObject2DropCore(object pCollObj) 
{ 
  var pObj, wep, cnt;
  // Für den Film (dort wird TIM1 als Platzhalter benutzt)
  if(pObj=FindContents(TIM1))
    return(pObj);
  // Ritter: Keine Handwaffen wegwerfen!
  if(IsKnight())
  {
    pObj = this()->GetObject2Drop_Knight(pCollObj);
    if(pObj) if(pObj->~IsFightWeapon())
    {
      while(wep=FindObject(,,,,,,,, this(), wep))
        if(wep->~IsFightWeapon())
          cnt++;
      if(cnt < 2) pObj = 0;
    }
  }
  // Restliche Clonks: Alles außer Statuensplitter, Essen und Tränke wegwerfen
  if(pObj) if(GetID(pObj)==GDLS || IsFood(pObj) || pObj->~IsPotion()) pObj = 0;
  if(!pObj)
    while(wep=FindObject(,,,,,,,, this(), wep))
      if(GetID(wep)!=GDLS && !IsFood(wep) && !wep->~IsPotion() && !wep->~IsFightWeapon())
        pObj = wep;
  return(pObj);
}

private func GetObject2Drop(object pCollObj) 
{
  return(GetObject2DropCore(pCollObj));
}

/* Hilfappends */

public func IsMagiclonk()
  {
  return(GetID() == MCLK || GetID() == SCLK || GetID() == MAGE || GetID() == KING);
  }

public func AI_WarpHome()
  {
  return(SetPosition(ai_iHomeX, ai_iHomeY));
  }

// Ritterfunktionen: Definition der Basisfunktionen im Clonk
private func IsBowEquipped() { return(_inherited()); }
private func ExecuteAttackBow(object pTarget) { return(_inherited(pTarget)); }
private func IsAiming() { return(_inherited()); }
private func IsKnight() { return(_inherited()); }
private func IsLoading() { return(_inherited()); }
private func GetAimingDeviation() { return(5); }
private func SpellSucceeded(a,b,c,d,e) { return(_inherited(a,b,c,d,e)); }
private func SpellFailed(a,b,c,d,e) { return(_inherited(a,b,c,d,e)); }
private func HasFightWeapon(a) { return(_inherited(a)); }

protected func Death(int iKilledBy, a,b,c,d)
  {
  // Für pre-NET2-Engine: Info-Broadcasts für sterbende Clonks
  GameCall("OnClonkDeath", this(), iKilledBy);
  // Der Broadcast könnte seltsame Dinge gemacht haben: Clonk ist noch tot?
  if (GetAlive()) return();
  // Standardaufruf (führt ggf. RelaunchPlayer-Aufruf sowie doppelten OnClonkDeath-GameCall durch)
  return(_inherited(iKilledBy, a,b,c,d));
  }

// Fernkämpfer?
public func AI_IsRanged()
  {
  return (IsMagiclonk() || FindContents(BOW1));
  }
  
// KI-Magier können auch im Kämpfen und Klettern zaubern
public func ReadyToMagic(a,b,c,d)
  {
  if (ai_iRange && GetID() == MAGE)
    if (GetProcedure() eq "FIGHT" || GetProcedure() eq "SCALE" || GetProcedure() eq "HANGLE") return(true);
  return(_inherited(a,b,c,d));
  }

// Beim Aufdecken Physicals anpassen
public func OnReactivate() { return(AI_SetPhysicals()); }
  
public func AI_SetPhysicals()
  {
  // KI-Spezifische Physicals
  // iDifficulty: Statische Variable im Szenarienscript 1=Normal, 2=Schwer
  if (!ai_iRange) return();
  if (!ai_iStrength) ai_iStrength = 100;
  var iStrength = ai_iStrength * (Max(iDifficulty, 1)+1) / 2;
  AI_SetPhysical("Fight", iStrength);
  AI_SetPhysical("Energy", iStrength);
  if (IsMagiclonk()) AI_SetPhysical("Magic", iStrength);
  DoEnergy(5000); // Max. Energie
  }
  
// Temporäre Phsical auf relativen Wert des Definitionswertes setzen
public func AI_SetPhysical(string szPhysical, int iPercent)
{
  return(SetPhysical(szPhysical, GetPhysical(szPhysical, 0, 0, GetID()), 2)*iPercent/100);
}


// ---------
// KI-Clonks sind verfeindet!
global func Hostile(int iPlr1, int iPlr2)
{
  // Hax: Fahrzeuge und Gebäude gehen trotzdem (Sonst funzt der Fahrstuhl nicht :'(
  if (GetCategory() & (C4D_Vehicle|C4D_Structure)) return(inherited(iPlr1, iPlr2));
  if (iPlr1 == NO_OWNER || iPlr2 == NO_OWNER) return(iPlr1 != iPlr2);
  return(inherited(iPlr1, iPlr2));
}
  
