/*-- Neues Objekt --*/

#strict
local timeToSpawn;

public func Initialize() {
  timeToSpawn = 300; 
  return(1);
}

public func SpawnLoad()
{
	timeToSpawn--;	
	PlayerMessage(GetOwner(this),"Time before Spawning: %i",this,timeToSpawn);
	if (timeToSpawn == 0)
	{
		Spawn();
		RemoveObject();
	}
	return(1);
}


public func Spawn()
{
    var iPlr = GetOwner(this);
    var member = Contents(0);
    Exit(member,0,-8);
    var belt = CreateContents(938Z,member);
    AddCommand(member,"Call",belt,0,0,0,0,"Activate");
    AddEffect("Rejoin",member,1,1,0);
    return(1);
}