#strict

Initialize:
  SetAction("NotDiscovered");
  return(1);

WatchExplorer:
  if(!FindObject(0,0,-50,100,100,OCF_CrewMember())) return(0); 
  Sound("Trumpet");  
  Message("Artefakt gefunden!|Raketen können nun erforscht werden");
  SetAction("IsDiscovered");
  return(1);