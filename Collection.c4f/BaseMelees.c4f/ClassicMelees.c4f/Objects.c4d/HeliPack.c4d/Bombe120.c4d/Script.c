/*-- Bombe --*/
//Local 0: Drehung

#strict

public HookOffset: return(-16);

protected Initialize:
  SetLocal(0,0);
  return(1);


protected UpdateRotation:
   if (Contained()) return(1);
   if (LessThan(GetXDir(),0)) SetLocal(0,180);
   if (GreaterThan(GetXDir(),0)) SetLocal(0,0);
   if (LessThan(GetYDir(),9)) return(SetRot(Local(0)));

   if (GreaterThan(GetYDir(),30)) SetVar(0,Or(FindObject(SUB1,0,1,0,0,0, 0,0, NoContainer()),
   						FindObject2(Find_ID(_RTA),Find_AtPoint(0,1),Find_Not(Find_ActionTarget(GetActionTarget())), Find_NoContainer()),
		    				 FindObject(MCTW,0,1,0,0,0, 0,0, NoContainer()),
                                             	  FindObject(_HEL,0,1,0,0,0, 0,0, NoContainer())));  
   if (Var(0)) return(Explodieren());

   SetVar(0,15);
   if (Local(0)) SetVar(0,Mul(Var(0),-1));
 
   return(SetRot(Sub(Angle(0,0,Sum(Var(0),GetXDir()),Sub(GetYDir(),8)),90)));


protected ContactRight:
   if (GreaterThan(GetYDir(),30)) return(Explodieren());
   return(0);

protected Incineration:
   Explodieren();
   return(0);

protected Departure:
   if (GetDir(Par(0))) return(SetLocal(0,0));
   return(SetLocal(0,180));


private SetRot:
   SetVar(0,GetR());
   SetR(Par(0));
   if (Stuck()) SetR(Var(0));
   return(0);


private Explodieren:
   Explode(30,CreateObject(GUNP));
   Explode(30,CreateObject(GUNP));
   Explode(30);
   return(0);


/* Produkteigenschaften */

public IsChemicalProduct: return(1);

/* Forschung */

public GetResearchBase: return(EFLN);




