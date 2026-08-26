#strict

//Basen
static pStartHut1;
static pStartHut2;
static team1, team2;
static flag1, flag2;

func Initialize() {
  //Kein Wind zu Beginn
  SetWind(0);
  team1 = false;
  team2 = false;
  //Gebaeude Team 1
  CreateConstruction(TWR2,496,270,-1,100,1);
  pStartHut1 = CreateConstruction(HUT2,151,270,-1,100,1);
  Enter(pStartHut1,CreateObject(CNKT));
  Enter(pStartHut1,CreateObject(CNKT));
  //Gebaeude Team 2
  CreateConstruction(TWR2,1337,270,-1,100,1);
  pStartHut2 = CreateConstruction(HUT2,1686,270,-1,100,1);
  Enter(pStartHut2,CreateObject(CNKT));
  Enter(pStartHut2,CreateObject(CNKT));
  return(1);  
  }

protected func InitializePlayer(iPlr,x,y,bas,team) {
  if(team == 1) 
  { 
  	if (team1 == true)
		SetWealth(iPlr,0);
	team1 = true;
	PlacePlayer1(iPlr);	
  }
  if(team == 2) 
  {
	if (team2 == true)
		SetWealth(iPlr,0);
	team2 = true;
	PlacePlayer2(iPlr);	
  }
  return(1);  
  }

private func PlacePlayer1(int iPlr) {
  //wenn die Gebaeude niemand gehoeen(Beim ersten joinen eines Teammitglieds) - Besitzer setzen
  var objs=FindObjects(Find_Category(C4D_Structure),Find_InRect(0,0,LandscapeWidth()/2,LandscapeHeight()));
  for(var i=GetLength(objs);i>0;i--)  
    if(GetOwner(objs[i-1])==-1) SetOwner(iPlr,objs[i-1]);
  
  if(pStartHut1) {
    for(var i; i<GetCrewCount(iPlr);i++) Enter(pStartHut1,GetCrew(iPlr,i));
    if (flag1 == false)
    {
	Enter(pStartHut1,CreateObject(FLAG,0,0,iPlr));
	flag1 = true;
    }
    }
  //Beitritt mitten im Spiel?
  else for(var i; i<GetCrewCount(iPlr);i++) SetPosition(75,270,GetCrew(iPlr,i));
  Log("$TeamLeftJoin$",GetPlayerName(iPlr),Format("$TeamLeft$"));
  return(1);
}

private func PlacePlayer2(int iPlr) {
  var objs=FindObjects(Find_Category(C4D_Structure),Find_InRect(LandscapeWidth()/2,0,LandscapeWidth(),LandscapeHeight()));
  for(var i=GetLength(objs);i>0;i--)  
    if(GetOwner(objs[i-1])==-1) SetOwner(iPlr,objs[i-1]);

  if(pStartHut2) {
    for(var i; i<GetCrewCount(iPlr);i++) Enter(pStartHut2,GetCrew(iPlr,i));
    if (flag2 == false)
    {
	Enter(pStartHut2,CreateObject(FLAG,0,0,iPlr));
	flag2 = true;
    }
    }
  else for(var i; i<GetCrewCount(iPlr);i++) SetPosition(1395,270,GetCrew(iPlr,i));
  Log("$TeamRightJoin$",GetPlayerName(iPlr),Format("$TeamRight$"));
  return(1);
}
