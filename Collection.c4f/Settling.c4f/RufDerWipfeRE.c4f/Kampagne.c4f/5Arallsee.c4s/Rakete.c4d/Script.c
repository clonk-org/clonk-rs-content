#strict

Initialize:
  SetAction("NotDiscovered");
  return(1);

WatchExplorer:
  if(!FindObject(0,0,-50,100,100,OCF_CrewMember())) return(0); 
  Sound("Trumpet");  
  Message("$MsgFound$");
  SetAction("IsDiscovered");
  return(1);