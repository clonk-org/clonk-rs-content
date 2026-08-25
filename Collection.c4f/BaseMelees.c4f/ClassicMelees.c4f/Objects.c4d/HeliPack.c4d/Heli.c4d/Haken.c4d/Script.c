//Local 0: Transportobjekt
/*Werte relativ zu übergebenen Koordinaten*/
//Local 1: X-Pos des Hakens
//Local 2: X-Schwingung des Hakens(z.Z. nicht verwendet)
//Local 3: Y-Pos des Hakens
//Local 4: Y-Geschwindigkeit des Hakens 
//Local 5: Trägerobjekt(z.Z. nicht verwendet)
//Local 6: Wartetimer bei Auftrieb von schweren Objekten
//         (Um "Wackeln" zu vermindern)
//Local 7: Abweichendes Hakenoffset

#strict

/*PUBLIC*/

/*InitHook*
Setzt ein paar Startwerte und erstellt Seilverbindung
Par(0): Objekt mit dem das Seil verbunden werden soll(Musswert)
Par(1): Startlaenge(Kannwert)
*/ 
public InitHook:
   SetVar(0,CreateObject(_ROP));
   ObjectCall(Var(0),"Connect",this(),Par(0));
//   SetLocal(5,Par(0));
   SetYPos(Par(1));
   return(1);

public ChgDir: //Wechselt Richtung mit Stop dazwischen
   SetAction("Walk");
   if (Local(4)) return(SetLocal(4));
   Move(Not(GetDir()));
   return(1);

public Move: //Bewegt Haken nach oben(Par(0,0)) oder unten(Par(0,1))
   SetDir(Par());
   SetLocal(4, Dir2Int(Par()));
   return(1);

public Stop: //Hält Haken an
   return(SetLocal(4));

public HookActivate: //Aktiviert Transportobjekt
   if (Local(0)) return(ObjectCall(Local(0),"HookActivate",Par(0),Par(1),Par(2),Par(3),Par(4)));
   return(0);

public Connect: //Verbindet/Löst Transportobjekt mit Haken
                             //Parameter Trägerobjekt, mit dem ja nicht verbunden werden darf
   if (Local()) 
      {
      SetXDir(GetXDir(Par(0)),Local());
      SetYDir(GetYDir(Par(0)),Local());
      Sound ("Connect.wav");
      return(SetLocal());
      }
   SetLocal(7,-8);
   while(Suchobjekt(Par(0))) Var(); //Dummy
   if (Not(Local())) return(); //Kein Objekt gefunden
   Sound("Connect.wav");
   if (SetVar(0,ObjectCall(Local(0),"HookOffset"))) SetLocal(7,Var());
   SetAction("Walk");

   SetLocal(1,   Sum( Sub(GetX(Local()),GetX()),Local(1) ) );
   SetLocal(3,   Sum( Sub(GetY(Local()),GetY(),Abw()),Local(3) ) );
   SetVar(0,GetX());
   SetVar(1,GetY());
   SetPosition(GetX(Local()),Sub(GetY(Local()),Abw(0)));
   if (Or(Stuck(),SeilVerklemmt())) return(SetPosition(Var(0),Var(1)));
   return(1);

public XPos: return(Local(1));
public SetXPos: return(SetLocal(1,Par()));

public YPos: return(Local(3));
public SetYPos: return(SetLocal(3,Par()));

public GetWeight: 
   if (Local()) return(GetMass(Local(0)));
   return(0); 

public GetTransObj: return(Local(0));

public Pos_Haken: //Par 0: Trägerobjekt

// Objekt loslassen, wenn erforderlich
   if (Local(0))
      if (Or(Contained(Local(0)),ObjectCall(Local(0),"BlockHaken"))) SetLocal();   


// Hilfsvariable dekrementieren
   if (Local(6)) SetLocal(6,Sub(Local(6),1));
   if (LessThan(Local(6),0)) SetLocal(6);

// Y-Position anpassen
   SetVar(0,Local(4));
   if (Local(0)) 
      {
      // Schwere Objekte übersteuern(nur wenn kein Auftrieb) 
      if (And(GreaterThan(GetWeight(),GetMaxWeight(Par())),Not(Local(6)))) SetVar(0,1);

      // Objekte mit Auftrieb übersteuern
      if (LessThan(GetYDir(Local(0)),0))
         {
         SetVar(0,-1);
         Stop();
         //Hilfsvariable setzen
         SetLocal(6,12);
         }
      }
   SetLocal(3,Sum(Var(0),Local(3)));

// X-Position anpassen
   if (GreaterThan(Local(1),0)) SetLocal(1,Sub(Local(1),1));
   if (LessThan(Local(1),0))    SetLocal(1,Sum(Local(1),1));

/*   SetVar(0,Div(GetXDir(Par(0)),-5));
*
*   if (And(Equal(Local(1),Var(0)),Inside(Local(2),-12,12))) SetLocal(2); 
*   if (GreaterThan(Local(1),Var(0))) 
*      {
*      if (GreaterThan(Local(2),0)) 
*         {
*        if (Not(LessThan(Local(1),ObjectCall(Par(0),"MAXRIGHT")))) SetLocal(2);
*         SetLocal(2,Sub(Local(2),1)); 
*         }
*      SetLocal(2,Sub(Local(2),6));
*      }
*   if (LessThan(Local(1),Var(0))) 
*      {
*      if (LessThan(Local(2),0)) 
*         {
*         if (Not(GreaterThan(Local(1),ObjectCall(Par(0),"MAXLEFT")))) SetLocal(2);
*         SetLocal(2,Sum(Local(2),1));
*         }
*      SetLocal(2,Sum(Local(2),6));
*      }
*
*   if (GreaterThan(Local(2),4)) SetLocal(1,Sum(Local(1),1));
*   if (LessThan(Local(2),-4)) SetLocal(1,Sub(Local(1),1));
*/

// Geschwindigkeit von Haken und Transportobjekt anpassen
   SetYDir(0);
   SetXDir(0);

   if (Local())
      {
      SetXDir(0,Local());
      if (GreaterThan(GetXDir(Par(0)),0)) SetXDir(1,Local());
      if (LessThan(GetXDir(Par(0)),0)) SetXDir(-1,Local());

      //if (Not(ObjectCall(Local(), "NoHeliSpeed"))) SetXDir(GetXDir(Par(0)),Local());
      SetYDir(0,Local());
      }


// Positionieren
   SetVar(0,GetX()); SetVar(1,GetY()); //Alte Position retten

   //Neue Position annehmen
   SetzePosition(Sum(GetX(Par(0)),Local(1)),Sum(GetY(Par(0)),Local(3)),Par()); 

   //Kollisionsabfrage
   if (Not(Verklemmt(Par(0)))) return(1); //Wenn neue Position frei dann fertig...
   SetLocal(2,0);
   SetVar(2,GetX());                      //sonst neue X-Position speichern
   SetzePosition(Var(0),GetY(),Par(0));   //und alte X-Pos/neue Y-Pos annehmen

   if (Not(Verklemmt(Par(0)))) return(1); //Wenn neue Position frei dann fertig...
   SetzePosition(Var(2),Var(1),Par(0));   //sonst neue X-Pos/alte Y-Pos annehmen
   Stop(); //Haken stoppen

   if (Not(Verklemmt(Par(0)))) return(1); //Wenn neue Position frei dann fertig...
   SetzePosition(Var(0),Var(1),Par(0));   //sonst alte Position wieder annehmen

   return();

public RopeBreak: 
   Sound("LineBreak");
   return(RemoveObject());


/*PRIVATE*/

protected Initialize:
   SetAction("Walk");
   SetDir(1);
   return(1);

private SetzePosition:
  SetPosition(Par(0),Par(1));
   SetXPos(Sub(GetX(),GetX(Par(2)))); //X-Pos aktualisieren
   SetYPos(Sub(GetY(),GetY(Par(2)))); //Y-Pos aktualisieren
   if (Local()) SetPosition(Par(0),Abw(Par(1)),Local());
   return(1);

private Verklemmt:
   if (Stuck()) return(1);
   if (Local()) if (Stuck(Local())) return(1);
   return(SeilVerklemmt(Par(0)));  

private Dir2Int: //wandelt 0 oder 1 in -1 bzw. +1 um...
   return (Sub(Mul(Par(),2),1));

private Abw: //Koordinate inkl. Abweichung vom Objektzentrum oder 0 um Abweichung zu erhalten
   return(Sub(Par(),Local(7)));

private Suchobjekt: 

   SetLocal(0, FindObject(0,-12,-6,24,26,OCF_Grab(),0,0, NoContainer(),Local()));
   if (Not(Local())) return(); //Kein Objekt gefunden
   if (Equal(Local(),Par())) return(1); //Objekt ist Bezugsobjekt
   if (Not(BitAnd(GetCategory(Local()),C4D_Vehicle()))) return(1); //Objekt ist kein Fahrzeug

   while (And(SetVar(0, FindObject(GetID(),0,0,0,0,0, 0,0, 0, Var(0))),Not(Var(1))))
      SetVar(1,HaengtSchon(Var(0)));
   return(Var(1));


private HaengtSchon:
   if (Equal(Par(0),this())) return(0); 
   if(Equal(ObjectCall(Par(0), "GetTransObj"),Local(0))) return(1);
   return(0);

private SeilVerklemmt:
   //Check ob Seil verklemmt:
   if(Not(PathFree( 
         Sum(GetX(Par(0)),Dir2Int(GetDir(Par(0)))), 
         GetY(Par(0)),  
         Sum(GetX(),Dir2Int(GetDir(Par(0)))),
         GetY() 
         )))  return(1);
   return();

private GetMaxWeight:
   return (ObjectCall(Par(0),"MAXWEIGHT"));



