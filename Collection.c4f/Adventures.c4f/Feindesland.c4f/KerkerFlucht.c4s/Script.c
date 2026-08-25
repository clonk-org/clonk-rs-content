/*-- Neues Szenario --*/

#strict

func Initialize()
{
for(var chest in FindObjects(Find_ID(_CST)))
{
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
	return(1);
}

protected func InitializePlayer(plr)
{
  CreateMenu(KNIG, GetHiRank(0), GetHiRank(0), 0, "Spieloptionen wählen. Mit Graben schließen.");
	AddMenuItem("7 Versuche - Leicht", "leicht", WIPF, GetHiRank(0));
	AddMenuItem("4 Versuche - Mittel", "mittel", SNKE, GetHiRank(0));
	AddMenuItem("1 Versuch - Schwer", "schwer", MONS, GetHiRank(0));

	var pClonk = GetCrew(plr, 0);
	SetPosition(150, 700, GetCrew(plr));
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
 if(!--rand)return(SPER);
 if(!--rand)return(SHIE);
 if(!--rand)return(BLTP);
 if(!--rand)return(BOW1);
 if(!--rand)return(FARP);
 if(!--rand)return(XARP);
 if(!--rand)return(STFN);
 if(!--rand)return(ARWP);
 if(!--rand)return(AXE1);
 if(!--rand)return(SWOR);
 if(!--rand)return(PHEA);
 if(!--rand)return(WBRL);
 if(!--rand)return(COKI);
 return(COKI);
}

global func leicht()
{
		// Relaunchs erhöhen
		var i = GetPlayerCount();
		var index;
		for(var h = 0; h <= i; h++)
		{
			Global(index)++;
			Global(index)++;
			Global(index)++;
			Global(index)++;
			Global(index)++;
			Global(index)++;
			index++;
		}
		Log("Der Schwierigkeitsgrad ist jetzt auf leicht.");	
		return(menu());
}

global func mittel()
{
	for (var i; i < 2; i++)
	{
		var i = GetPlayerCount();
		var index;
		for(var h = 0; h <= i; h++)
		{
			Global(index)++;
			Global(index)++;
			Global(index)++;
			index++;
		}
	}
	Log("Der Schwierigkeitsgrad ist jetzt auf mittel.");
	return(menu());
}

global func schwer()
{
	Log("Der Schwierigkeitsgrad ist jetzt auf schwer.");
	return(menu());
}

global func wtf()
{
	RemoveAll(WTFL);
	return(menue());
}

global func sto()
{
	CloseMenu(GetCursor(0));
        return(menue());
}

global func loesch()
{
	CreateObject(_ETG);
	return(1);
}

global func stoe()
{
		CloseMenu(GetCursor(0));
}

protected func RelaunchPlayer(plr)
{
if(GetPlayerTeam(plr) == 1)
   if(Global(plr) == 0)
   {
   		return(1);
   } else {
     	var bote = CreateObject(KNIG, 150, 700, plr);
    	MakeCrewMember(bote, plr);
  		DoEnergy(100, bote);
    	SelectCrew(plr, bote);
   		Global(plr)--;
   		Log("%s hat noch %d Relaunch/es übrig.", GetPlayerName(plr), Global(plr));
  		return(1);
	 }
	 return(1);
}



global func menu()
{
	CreateMenu(KNIG, GetHiRank(0), GetHiRank(0), 0, "Spieloptionen wählen.");
	AddMenuItem("Wasserfälle entfernen", "wtf", WATR, GetHiRank(0));
	AddMenuItem("Schließen", "sto", EXTG, GetCursor(0));
	return(1);
}

global func menue()
{
	CreateMenu(KNIG, GetHiRank(0), GetHiRank(0), 0, "Spieloptionen wählen.");
	AddMenuItem("Löscher einfügen", "loesch", EXTG, GetHiRank(0));
	AddMenuItem("Schließen", "stoe", _BOK, GetHiRank(0));
	return(1);
}
