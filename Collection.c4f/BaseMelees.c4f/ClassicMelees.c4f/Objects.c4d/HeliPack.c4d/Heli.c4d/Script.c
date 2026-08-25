/*-- Heli --*/

//Local 0: Trudelcounter
//Local 1: Haken...
//Local 2: Hilfsvariable bei Beschädigung

#strict

protected Completion:
  SetAction("Walk");
  SetEntrance(0);
  SetComDir(COMD_Stop());
  return(0);

/* Steuerung */

protected ContainedUp:
  ClearCommand();
  Abheben();
  SetComDir(COMD_Up());
  return(1);

protected ContainedDown:
  ClearCommand();
  if (And(Not(GetXDir()),GreaterThan(GetYDir(),-1)))
     if (Or(IsAction("Bottom"),IsAction("Crashed_C")))  
        return(Aussteigen());
  SetComDir(COMD_Down());
  SetCommand(Par(0),"None");
  return(1);

//Zum Testen
//protected ContainedDownDouble:
//return(StartTrudel());

protected ContainedRight:
  ClearCommand();
  Abheben();
  if (Not(IsAction("Float"))) return(0);
  if (Not(Equal(GetDir(),DIR_Right()))) SetAction("Turn");
  SetComDir(COMD_Right()); 
  SetDir(DIR_Right());
  return(1);

protected ContainedLeft:
  ClearCommand();
  Abheben();
  if (Not(IsAction("Float"))) return(0);
  if (Not(Equal(GetDir(),DIR_Left()))) SetAction("Turn");
  SetComDir(COMD_Left()); 
  SetDir(DIR_Left());
  return(1);  

protected ContainedThrow: return(Chg_Hak());
protected ContainedThrowDouble: return(Chg_Hak()); //Für bessere Reaktion


protected ContainedDig:
  Stop();
  return(1);

protected ContainedDigSingle:
  if (Local(1)) CallHaken("HookActivate");
  return(1);

protected ContainedDigDouble:
  if (Local(1)) CallHaken("Connect",this());
  return(1);


/*CNAT*/

ContactBottom:
   if (IsAction("Float")) SetAction("Bottom");  
   if (And(Or(IsAction("Trudel"),IsAction("Crashed_C")), GreaterThan(GetYDir(),50))) Explodieren();  
   SetLocal(0,0);
   return(0);

ContactTop:
   if (Not(IsFlying())) return(1);
   if (GreaterThan(GetDamage(),15)) return(Explodieren());
   Sound("knack");
   SetAction("Crashed_C");
   SetRDir(10,CreateObject(_ROT, -20, -10, -1));
   SetVar(0,CreateObject(_ROT, 20, -10, -1));  
   SetR(180,Var(0));
   SetRDir(-10,Var(0));

   return(0);


/*Sonstige*/

protected Incineration:
   Incinerate(Contents());
   Aussteigen();
   SetAction("Burned");
   return(1);

protected Damage:
  if (GreaterThan(GetDamage(),50)) Explodieren();
  return(1);

public BlockHaken:
   if (IsFlying()) return(1);
   return(0);

public Explodieren:
   Incinerate(Contents());
   Aussteigen();
   CastObjects(DFLM,6,25);  
   Explode(35);
   return(0);

public Aussteigen:          //ContainedDownDouble:
   if (Not(Contents())) return (0); 
   SetVar(0,Contents());
   Delete_Hilfsobjekt(Var(0));
   Hakenremove();
   SetDir(GetDir(),Var(0));
   Exit(Var(0));
   if (Contained()) Enter(Contained(),Var(0));
   SetComDir(COMD_Stop()); 
   SetXDir(0); SetYDir(0);
   if (IsAction("Crashed_C")) return(SetAction("Crashed"));
   SetAction("Walk");
   return(1);

public IsBombTarget: return(1);

public IsFlying:
   return(Or(IsAction("Float"), IsAction("Turn"),
             IsAction("Trudel"), IsAction("Bottom")));

public IsDestroyed:
   return(Or(IsAction("Burned"), IsAction("Crashed"), IsAction("Crashed_C")));

public IsAction: //prüft ob aktuelle Aktion=Par(0)
   return(SEqual(GetAction(),Par(0)));

public IsDir: //prüft ob aktuelle Direction=Par(0)
   return(Equal(GetDir(),Par(0)));

public GetRelX: //Gibt relative X-Koordinaten zurück
   return(Sub(GetX(Par(0)),GetX()));


/*Ein- und Auslass von Objekten*/

/* Trefferkontrolle */

protected func LightningStrike()
{
StartTrudel();  
}

protected func RejectCollect(id idObject, object pObj)
{
//  if (And(Not(IsAction("Turn")),And(IsDir(DIR_Right()),GreaterThan(GetRelX(Par(1)), -10)))) return(1); 
//  if (And(Not(IsAction("Turn")),And(IsDir(DIR_Left()),LessThan(GetRelX(Par(1)), 10)))) return(1);

// Clonkinventar nicht aktivieren
  if (Contained(pObj)) return(1);

  // Pfeile vertragen sich nicht mit einem Heli
  if (DefinitionCall(idObject,"IsArrow") )
   // Nur, wenn der Pfeil sich auch bewegt
   if(GetXDir(pObj) || GetYDir(pObj))
  {
    if (Not(IsFlying())) return(1);
    // bei brennendem Pfeil anzünden
    if(OnFire(pObj)) Incinerate();
    // ansonsten abstürzen
    else StartTrudel();
    // Pfeil schlägt immer auch ein...
    ProtectedCall(pObj, "Hit");
    return(1);
  }
  // in jedem Fall bei allen Objekten Hit auslösen (Sprengpfeil / Flint explodiert zB) wenn sie sich bewegen
  if(Abs(GetXDir(pObj))>10 || Abs(GetYDir(pObj))>10) ProtectedCall(pObj, "Hit");
  return(1);
}

ActivateEntrance:
  //verschiedene Prüfungen ob Zutritt erlaubt
  if (Contained(Par(0))) return(1); //Aussteigen abfangen; return 1 damit kein "HMM"(Eingang sowieso blockiert)
  if (Or(Not(CrewMember(Par(0))),Contents())) return(BlockEntrance(Par(0))); //Nur Crewmitglieder Ein-/Auslass gewähren; MAX 1 Clonk
  if (Not(IsAction("Walk"))) return(BlockEntrance(Par(0))); 
  if (InLiquid()) return(BlockEntrance(Par(0)));
  if (And(IsDir(DIR_Left()),GreaterThan(GetRelX(Par(0)),3))) return(BlockEntrance(Par(0)));
  if (And(IsDir(DIR_Right()), LessThan(GetRelX(Par(0)),-3))) return(BlockEntrance(Par(0)));

  //Clonk einsteigen lassen
  SetAction("Bottom");
  SetOwner(GetOwner(Par(0)));
  Enter(this(),Par(0));
  Create_Hilfsobjekt(Par(0)); //Aktivieren-Menü übersteuern
  return(1);

protected Entrance:
  //Wenn Heli in anderes Objekt verfrachtet wird, Pilot auswerfen
  Aussteigen();
  //Heli weniger Wert beim verkaufen und reparierbar
  if (Not(IsDestroyed())) return(0);
  if (IsAction("Burned")) SetLocal(2,1);    
  if (IsAction("Crashed")) SetLocal(2,2);
  if (IsAction("Crashed_C")) SetLocal(2,3); 
  if (Local(2)) SetCon(75);
  return();

protected Departure:
  //Beim Verlassen von Objekten Con-Wert wieder rückgängig wenn erforderlich
  if (Not(Local(2))) return();
  if (Equal(GetCon(),100)) return(SetLocal(2));
  SetCon(100);
  if (Equal(Local(2),1)) SetAction("Burned"); 
  if (Equal(Local(2),2)) SetAction("Crashed");
  if (Equal(Local(2),3)) SetAction("Crashed_C");
  return(SetLocal(2));

/*Eigene*/

private StartTrudel:
  SetLocal(0,Trudelzahl());
  if (IsAction("Trudel")) return();
  if (IsAction("Turn")) SetDir(DIR_Right());
  SetVar(0,GetPhase());
  SetAction("Trudel");
  SetPhase(Var(0));
  return(1);

private Abheben:
  if (IsAction("Bottom")) return(SetAction("Float"));  
  return(1);

private Flugcheck:      
  Create_Hilfsobjekt(Contents()); //Sicherheitshalber immer neu, falls aktueller Inhalt entfernt werden sollte
  if(InLiquid()) return(Aussteigen()); //Wasserlandung
  if(Not(GetEnergy(Contents()))) return(Aussteigen()); //Tote auswerfen

//Wenden, auch wenn von Engine gesteuert
  CheckCommandAction();
 
//Heli am Kartenrand stoppen
  if (And(LessThan(GetXDir()),LessThan(GetX(),32))) SetXDir();
  if (And(GreaterThan(GetXDir()),GreaterThan(GetX(),Sub(LandscapeWidth(),32)))) SetXDir();
  if (And(LessThan(GetYDir()),LessThan(GetY(),20))) SetYDir();

//Schutz gegen unerwünschtes Aussteigen
  if (IsAction("Bottom")) if (Not(Or(GBackSolid(10,15),GBackSolid(-10,15)))) SetAction("Float"); 

//Rest nur nötig, wenn Haken ausgefahren ist
  if (Not(Local(1))) return(1); 
  if (Not(IsFlying())) return(Hakenremove());

  CallHaken("Pos_Haken",this());
  
  if (And(GreaterThan(CallHaken("XPos"), MAXRIGHT()),LessThan(GetXDir(Par(0)),0))) SetXDir();
  if (And(LessThan(CallHaken("XPos"), MAXLEFT()),GreaterThan(GetXDir(Par(0)),0))) SetXDir();

  if (And(GreaterThan(CallHaken("GetWeight"),MAXWEIGHT()),LessThan(GetYDir(),0))) SetYDir();

  if (LessThan(CallHaken("YPos"), 14)) return(Hakenremove());

  if (LessThan(CallHaken("YPos"), MAXLENGTH())) return(1);
  CallHaken("Stop");
  if (Not(GreaterThan(CallHaken("YPos"), MAXLENGTH()))) return(1);

  if (LessThan(GetYDir(),0)) SetYDir(); 
  if (And(GreaterThan(CallHaken("GetWeight"),MAXWEIGHT()),LessThan(GetYDir(),20))) Runterziehen();

  CallHaken("SetYPos",Sub(CallHaken("YPos"),1)); 
  return(1);

private Runterziehen:
  SetYDir(20);
  if (Equal(GetComDir(),COMD_Stop())) SetComDir(COMD_Up()); 
  return(1);

private Trudel:
  SetLocal(0,Sub(Local(),1));
  if (LessThan(Local())) SetAction("Float");
  SetDir(Not(GetDir()));
  return(1);

private Hakeninit:
  if (Local(1)) return(0);
  SetLocal(1,CreateObject(_HAK,0,27));
  if (Stuck(Local(1))) return (Hakenremove());
  CallHaken("InitHook",this(),16);
  CallHaken("Move",1);
  return(0);

private Hakenremove:
  if (Not(Local(1))) return();
  RemoveObject(Local(1));
  SetLocal(1);
  return();

private Chg_Hak:
  if (Not(IsFlying())) return(0);
  if (Not(Local(1))) return(Hakeninit());
  CallHaken("ChgDir");
  return(1);

private Stop:
  ClearCommand();
  SetComDir(COMD_Stop());
  //SetXDir(); SetYDir();
  if (LessThan(Abs(GetXDir()),12)) SetXDir();
  if (LessThan(Abs(GetYDir()),12)) SetYDir();
  return();

private CheckCommandAction:
  // Wenden nach rechts
  if (GetCommand())
    if (Inside(GetComDir(),COMD_UpRight(),COMD_DownRight())) 
      if (Equal(GetDir(),DIR_Left()))
        return(SetAction("Turn",0,0,SetDir(DIR_Right())));
  // Wenden nach links
  if (GetCommand())
    if (Inside(GetComDir(),COMD_DownLeft(),COMD_UpLeft())) 
      if (Equal(GetDir(),DIR_Right()))
        return(SetAction("Turn",0,0,SetDir(DIR_Left())));
  return(0);    

private BlockEntrance: //Par(0) Objekt
  //So tun, als wäre kein Eingang vorhanden
  ClearCommand(Par(0));
  if (InLiquid(Par(0))) return(SetComDir(COMD_Up(),Par(0)));  
  if (CrewMember(Par(0))) Jump(Par(0));
  return();

private ClearCommand: //Par(0) Objekt
  SetVar(0,Par(0));
  if (Not(Var(0))) SetVar(0,this());
  SetCommand(Var(0),"None");
  return();

private Create_Hilfsobjekt: //Unsichtbares Hilfsobjekt als Inhalt erzeugen, falls Objekt leer ist
  if (Contents(0,Par(0))) return();
  return(CreateContents(_LER,Par(0)));

private Delete_Hilfsobjekt: //Wenn Objekt unsichtbares Hilfsobjekt enthält, Hilfsobjekt löschen
  if (SetVar(0,FindContents(_LER, Par(0)))) RemoveObject(Var(0));
  return (0);

private CallHaken: //Objectcall auf Haken
  return(ObjectCall(Local(1),Par(0),Par(1),Par(2),Par(3),Par(4),Par(5),Par(6),Par(7),Par(8)));


private Trudelzahl: return(Sum(2,Random(2)));


/* Forschung */

public GetResearchBase: return(BLMP); 


// Konstante Werte
public MAXWEIGHT: return(350);
public MAXLENGTH: return(200);
public MAXRIGHT:  return(7);
public MAXLEFT:   return(-7);



