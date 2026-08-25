/*-- Neues Szenario --*/

#strict

public func RelaunchPlayer(iPlr)
{
  if(!GetPlayerName(iPlr)) return(0);
  Global(iPlr)--;
  if(Global(iPlr)==0){ Log("Du hast versagt."); EliminatePlayer(iPlr); return(0);}
  Log("%s hat einen Relaunch verbraucht! Er hat %d Relaunchs übrig!",GetPlayerName(iPlr),Global(iPlr));
  var clnk;
  MakeCrewMember(clnk=CreateObject(ZWPF,GetX(FindObject(PLCE)),GetY(FindObject(PLCE)),iPlr),iPlr);
  SelectCrew(iPlr, clnk, 1);
  DoEnergy(100000,clnk);
  return(1);
}

public func FillAllChests()
{
  for(var chest in FindObjects(Find_ID(_CST)))
  {
  while(Contents(0,chest))RemoveObject(Contents(0,chest));
  FillChest(chest);
  }
  for(var chest in FindObjects(Find_ID(TENT)))
  {
  FillChest(chest);
  }
  for(var chest in FindObjects(Find_ID(LORY)))
  {
  FillChest(chest);
  }
  return(1);
}

func Initialize() {
  //return(1);
  CreateObject(_ETG);
  CreateObject(NPRY);
  CreateObject(_SUI);
  CreateObject(FSTF);
  CreateObject(STES);
  CreateObject(MGES);
  //CreateObject(PLCE,300,300);
  ScriptGo(1);
  //Truhen füllen
  FillAllChests();
  
  return(1);
}

protected func FillChest(object pChest)
{
 var count=8+Random(6);
 while(count--)
 CreateContents(RandChestID(),pChest);
}

protected func RandChestID()
{
 var rand=Random(22);
 if(!rand)return(BRED);
 if(!--rand)return(SPER);
 if(!--rand)return(MUSK);
 if(!--rand)return(BLTP);
 if(!--rand)return(BOW1);
 if(!--rand)return(FARP);
 if(!--rand)return(XARP);
 if(!--rand)return(STFN);
 if(!--rand)return(EFLN);
 if(!--rand)return(AXE1);
 if(!--rand)return(SWOR);
 if(!--rand)return(METL);
 if(!--rand)return(SBLP);
 if(!--rand)return(PBLP);
 if(!--rand)return(CRBW);
 if(!--rand)return(BOTP);
 if(!--rand)return(NBTP);
 if(!--rand)return(HOOK);
 if(!--rand)return(TENP);
 if(!--rand)return(KNFP);
 if(!--rand)return(PMAN);
 return(COKI);
}

func InitializePlayer(iPlr)
{
  Global(iPlr)=25;
if(Global(123)==1) { SetPlrViewRange(90,GetCrew(iPlr)); SetFoW(1,iPlr); }
DoEnergy(100,GetCrew(iPlr));
SetPosition(GetX(FindObject(PLCE)),GetY(FindObject(PLCE)),GetCrew(iPlr));
}

func Script4()
{
Message("@<c ff4000>Man hat deinen goldenen Wipf geklaut!");
}

func Script10()
{
Message("<c ff4000>Töte die Wachen am Eingang und dringe in das Reich des bösen ein!");
}

func Script15()
{
Message("<c ff4000>loslos!");
}

func Script18()
{
if(FindObject(0,2899,289,160,200,OCF_CrewMember())) { goto(20); return(1); }
goto(16);
}

func Script20()
{
SetGlobal(123,1);
for(var i=0;i < 12; i++) {
Exit(GetCursor(i));
SetObjectStatus(2,GetCrew(i));

for(var c=0; c < ContentsCount(0,GetCrew(i)); c++)
{
SetObjectStatus(2,Contents(c,GetCrew(i)));
}
}

LoadScenarioSection("EvilTemple");

var i=0;
for(var i;i < 12; i++) {
SetObjectStatus(1,GetCrew(i));
SetPosition(GetX(FindObject(PLCE)),GetY(FindObject(PLCE)),GetCrew(i));
SetPlrViewRange(90,GetCrew(i));

for(var c=ContentsCount(0,GetCrew(i)); c >-1; c--)
{
SetObjectStatus(1,Contents(c,GetCrew(i)));
}

}


CreateObject(_ETG);
CreateObject(NPRY);
  CreateObject(_SUI);
  CreateObject(FSTF);
  CreateObject(STES);
  CreateObject(MGES);

  //Truhen füllen
  FillAllChests();
}

/*func Script22()
{
var i=0;
for(var i;i < 12; i++)
SetObjectStatus(2,GetCrew(i));
var u=0;
for(var u;u < 12; u++)
SetObjectStatus(1,GetCrew(u));
}*/


func Script23()
{
Message("<c ff4000>");
}

func Script25()
{
if(FindObject(0,2903,280,100,100,OCF_CrewMember())) { goto(26); return(1); }
goto(24);
}


func Script26()
{
SetGlobal(123,0);
for(var i;i < 12; i++) {
Exit(GetCursor(i));
SetObjectStatus(2,GetCrew(i));

for(var c=0; c < ContentsCount(0,GetCrew(i)); c++)
{
SetObjectStatus(2,Contents(c,GetCrew(i)));
}
}

LoadScenarioSection("EvilCastle");

var i=0;
for(var i;i < 12; i++) {
SetObjectStatus(1,GetCrew(i));
SetPosition(GetX(FindObject(PLCE)),GetY(FindObject(PLCE)),GetCrew(i));
SetPlrViewRange(200,GetCrew(i));

for(var c=ContentsCount(0,GetCrew(i)); c >-1; c--)
{
SetObjectStatus(1,Contents(c,GetCrew(i)));
}

}


AddEffect("Heal",FindObject(WIPF),20,1);
CreateObject(_ETG);
CreateObject(NPRY);
  CreateObject(_SUI);
  CreateObject(FSTF);
  CreateObject(STES);
  CreateObject(MGES);

  //Truhen füllen
  FillAllChests();
}



func Script30()
{
for(var i; i< 12; i++) {
if(FindContents(WIPF,GetCursor(i))) { goto(32); return(1); } }
goto(28);
}

func Script33()
{
Message("<c ff4000>Glückwunsch du hast den Wipf!");
}

func Script36()
{
GameOver();
}

global func FxHealTimer(pTarget)
{
DoEnergy(100,pTarget);
}


global func FxHealDamage(pTarget,iEffectNumber,iCause)
{
return(0);
}

