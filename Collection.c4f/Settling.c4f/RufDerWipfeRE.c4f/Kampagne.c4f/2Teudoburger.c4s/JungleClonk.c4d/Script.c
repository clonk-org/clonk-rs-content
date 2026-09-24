#strict

/* Sprechen */
Speak: 
  var clonk=FindObject(0,-20,-20,40,40,OCF_CrewMember());
  if(!clonk) return(0);
  Message("$MsgMission$",this());
  SetCommand(clonk,"MoveTo",0,120,0);
  return(1);
  