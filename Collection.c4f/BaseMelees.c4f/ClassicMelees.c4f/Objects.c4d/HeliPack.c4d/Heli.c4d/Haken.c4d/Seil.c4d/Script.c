/*--- Seil ---*/

#strict

Initialize:
  // Local 0 und 1 sind die Lininenfarben
  SetLocal(0,66); 
  //SetLocal(1,47);  
  // Die ersten beiden Vertices auf aktuelle Position legen
  SetVertex(0,0,GetX()); SetVertex(0,1,GetY());
  SetVertex(1,0,GetX()); SetVertex(1,1,GetY());
  return(1);

Connect:
  SetAction("Connect",Par(0),Par(1));
  return(1);
  
LineBreak:
  if (GetActionTarget()) ObjectCall(GetActionTarget(),"RopeBreak");
  return(1);
