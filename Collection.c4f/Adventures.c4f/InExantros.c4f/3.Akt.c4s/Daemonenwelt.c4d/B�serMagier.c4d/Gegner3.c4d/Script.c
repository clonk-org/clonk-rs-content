#strict

Initialize:
  SetAction("New");
  SetPosition(0,0,this());
  return(1);

Gegner:
//Gegner
  if(!Random(5))  if(!Local(1))  return(SetLocal(1,CreateObject(MMO_,2900,330,-1)),ObjectCall(Local(1),"Gegner2",0,2) );
  if(!Random(7))  if(!Local(2))  return(SetLocal(2,CreateObject(MMO_,3270,330,-1)),ObjectCall(Local(2),"Gegner2",0,2) );
  if(!Random(5))  if(!Local(3))  return(SetLocal(3,CreateObject(MMO_,3110,290,-1)),ObjectCall(Local(3),"Gegner2",0,2) );
  if(!Random(7))  if(!Local(4))  return(SetLocal(4,CreateObject(MMO_,2720,350,-1)),ObjectCall(Local(4),"Gegner2",0,2) );

/*
  if(!Random(5))  if(!Local(5))  return(SetLocal(5,CreateObject(_SPN,1350+Random(450),1230,-1)),ObjectCall(Local(5),"Gegner1",0,2) );
  if(!Random(7))  if(!Local(6))  return(SetLocal(6,CreateObject(_SPN,1350+Random(450),1230,-1)),ObjectCall(Local(6),"Gegner1",0,2) );
  if(!Random(7))  if(!Local(7))  return(SetLocal(7,CreateObject(_SPN,1350+Random(450),1230,-1)),ObjectCall(Local(7),"Gegner1",0,2) );
  if(!Random(7))  if(!Local(8))  return(SetLocal(8,CreateObject(_SPN,1350+Random(450),1230,-1)),ObjectCall(Local(8),"Gegner1",0,2) );

  if(!Random(7))  if(!Local(9))  return(SetLocal(9,CreateObject(_SPN,1570+Random(170),850,-1)),ObjectCall(Local(9),"Gegner1",0,2) );
  if(!Random(7))  if(!Local(10))  return(SetLocal(10,CreateObject(_SPN,1570+Random(170),850,-1)),ObjectCall(Local(10),"Gegner1",0,2) );
  if(!Random(7))  if(!Local(11))  return(SetLocal(11,CreateObject(_SPN,1570+Random(170),850,-1)),ObjectCall(Local(11),"Gegner1",0,2) );
*/
  return(1);

Vorbei:
/*
  if(Local(1))  DoEnergy(-10000000,Local(1));
  if(Local(2))  DoEnergy(-10000000,Local(2));
  if(Local(3))  DoEnergy(-10000000,Local(3));
  if(Local(4))  DoEnergy(-10000000,Local(4));
*/

  DoEnergy(-GetEnergy(Local(1)),Local(1));
  DoEnergy(-GetEnergy(Local(2)),Local(2));
  DoEnergy(-GetEnergy(Local(3)),Local(3));
  DoEnergy(-GetEnergy(Local(4)),Local(4));
  RemoveObject();
  return(1);