#strict

//Local(1) wird vom Schalter geändert. 0=Off 1=On
//Local(2) ist dann 1, darf das Tor unter keinen Umständen aufgehen darf
//Local(2) ist dann 2, Geht das tor komplett auf!

Initialize:
  SetAction("Gate");
  Open();
  return(1);

Close:
  Sound("GateOpen");
  SetComDir( COMD_Up() );
  return(1);

Open:
  if(Local(5)==0)  return(SetComDir(COMD_Down()),Sound("Saeule_01"),SetLocal(5,1),Sound("GateOpen"));
  if(Local(5)==1)  return(SetComDir(COMD_Up()),Sound("Saeule_01"),SetLocal(5,0),Sound("GateOpen"));
  return(1);