/*-- Neues Szenario --*/

#strict

func Initialize()
{
for(var chest in FindObjects(Find_ID(_CST)))
{
	SetSkyAdjust(RGBa(0,25,105,127), RGB(10,17,17));

	while(Contents(0,chest))RemoveObject(Contents(0,chest));
	FillChest(chest);
	}
 	 var pEnemy;
 	 while(pEnemy = FindObject(0, 0, 0, 0, 0, OCF_CrewMember(), 0, 0, 0, pEnemy))
 	 {
    if(GetOwner(pEnemy) == -1)
	    {
	      SetColorDw(RGB(Random(255), Random(255), Random(255)), pEnemy);
  	  }
}
	FindObject(RACE)->SetMissionAccess("MagicForest");
	return(1);
}

protected func InitializePlayer(plr)
{
	var pClonk = GetCrew(plr, 0);
	SetPosition(20, 160, GetCrew(plr));
	return(1);
}

protected func FillChest(object pChest)
{
 var count=6+Random(6);
 while(count--)
 CreateContents(RandChestID(),pChest);
}

protected func RandChestID()
{
 var rand=Random(14);
 if(!rand)return(BRED);
 if(!--rand)return(PMAN);
 if(!--rand)return(PMON);
 if(!--rand)return(PMAN);
 if(!--rand)return(FLNT);
 if(!--rand)return(FARP);
 if(!--rand)return(XARP);
 if(!--rand)return(STFN);
 if(!--rand)return(ARWP);
 if(!--rand)return(PSTO);
 if(!--rand)return(BOOK);
 if(!--rand)return(PHEA);
 if(!--rand)return(WBRL);
 if(!--rand)return(COKI);
 return(COKI);
}