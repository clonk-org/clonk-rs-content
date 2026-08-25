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
  DoEnergy(100000,obj);
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
  Global(123)=0;
  //return(1);
  
  if(FindObject(FGRV))RemoveObject(FindObject(FGRV));
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
  
var wipf;
wipf=CreateObject(WIPF,GetX(FindObject(PLCE)),GetY(FindObject(PLCE)),-1);
SetClrModulation(RGB(220,230,70),wipf);

  var s;
 while(s=FindObject(_STA,0,0,0,0,0,0,0,0,s))s->Initialize();

  
  for(var fog; fog < 100; fog++)
  {
  CreateParticle("Fog",RandomX(1110,1700),RandomX(50,590),0,0,RandomX(1000,1500),RGB(250,250,250));
  }
  
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


func Script1()
{
Message("<c ff4000>Bringt den goldenen Wipf sicher zum Turm zurück!");
}


func Script18()
{
if(!FindObject(WIPF)) {
Message("@Der Wipf ist tot! Du hast versagt");
for(var i=0; i < GetPlayerCount(); i++)
EliminatePlayer(GetPlayerByIndex(i));
SetPlrViewRange(0,GetCrew(i));
ScriptGo(0);
return(1);
}


var clnk;
//if(clnk=FindObject(0,2910,360,70,80,OCF_CrewMember(),0,0,0,clnk)) {
while(clnk=FindObject(0,2910,360,170,150,OCF_CrewMember(),0,0,0,clnk)) {
if(FindContents(WIPF,clnk)) { goto(20); return(1); } }
goto(16);
}

/*2910
450
2980*/

/*func Script20()
{



Global(123)=1;
for(var i=0;i < 12; i++) {
Exit(GetCursor(i));

SetObjectStatus(2,GetCrew(i));

for(var shield=0; shield < ObjectCount(SHIA); shield++)
{
var shia=FindObject(SHIA);
shia->Unbuckle();
if(GetOwner(shia)==-1)RemoveObject(shia);
Enter(GetCrew(GetOwner(shia)),shia);
SetObjectStatus(2,shia);
}


for(var c=0; c < ContentsCount(0,GetCrew(i)); c++)
{
var obj;
if(GetID(obj=Contents(c,GetCrew(i))) == MUSK)SetObjectStatus(2,Contents(0,obj));
if(GetID(obj=Contents(c,GetCrew(i))) == WIPF)RemoveObject(obj);
SetObjectStatus(2,Contents(c,GetCrew(i)));
}
}

LoadScenarioSection("Outpost");

  var s;
 while(s=FindObject(_STA,0,0,0,0,0,0,0,0,s))s->Initialize();

var i=0;
for(var i;i < 12; i++) {
SetObjectStatus(1,GetCrew(i));
SetPosition(GetX(FindObject(PLCE)),GetY(FindObject(PLCE)),GetCrew(i));
SetPlrViewRange(90,GetCrew(i));
SetFoW(1,i);

for(var c=ContentsCount(0,GetCrew(i)); c >-1; c--)
{
var obj;
var shield;

SetObjectStatus(1,Contents(c,GetCrew(i)));


if(GetID(obj=Contents(c,GetCrew(i))) == MUSK)SetObjectStatus(1,Contents(0,obj));
if(GetID(shield=Contents(c,GetCrew(i))) == SHIE) { SetCategory(3600,shield); shield->Activate(GetCursor(i));}
}


}

var wipf;
wipf=CreateObject(WIPF,GetX(FindObject(PLCE)),GetY(FindObject(PLCE)),-1);
SetClrModulation(RGB(220,230,70),wipf);

CreateObject(_ETG);
CreateObject(NPRY);
CreateObject(_SUI);
  CreateObject(FSTF);
  CreateObject(STES);
  CreateObject(MGES);

  //Truhen füllen
  FillAllChests();
}*/

/*func Script22()
{
var i=0;
for(var i;i < 12; i++)
SetObjectStatus(2,GetCrew(i));
var u=0;
for(var u;u < 12; u++)
SetObjectStatus(1,GetCrew(u));
}*/



/*func Script25()
{

if(!FindObject(WIPF)) {
Message("@Der Wipf ist tot! Du hast versagt");
for(var i=0; i < GetPlayerCount(); i++)
EliminatePlayer(GetPlayerByIndex(i));
SetPlrViewRange(0,GetCrew(i));
ScriptGo(0);
return(1);
}
var clnk;
if(clnk=FindObject(0,2900,460,60,65,OCF_CrewMember())) {
if(FindContents(WIPF,clnk)) { goto(26); return(1); } }
goto(23);
}*/


func Script26()
{

Global(123)=1;
for(var i;i < 12; i++) {
Exit(GetCursor(i));
SetObjectStatus(2,GetCrew(i));

for(var shield=0; shield < ObjectCount(SHIA); shield++)
{
var shia=FindObject(SHIA);
shia->Unbuckle();
if(GetOwner(shia)==-1)RemoveObject(shia);
Enter(GetCrew(GetOwner(shia)),shia);
SetObjectStatus(2,shia);
}

for(var c=0; c < ContentsCount(0,GetCrew(i)); c++)
{
var musk;
if(GetID(musk=Contents(c,GetCrew(i))) == MUSK)SetObjectStatus(2,Contents(0,musk));
if(GetID(musk=Contents(c,GetCrew(i))) == WIPF)RemoveObject(musk);
SetObjectStatus(2,Contents(c,GetCrew(i)));
}
}

LoadScenarioSection("Swamp");

  var s;
 while(s=FindObject(_STA,0,0,0,0,0,0,0,0,s))s->Initialize();

var i=0;
for(var i;i < 12; i++) {
SetObjectStatus(1,GetCrew(i));
SetPosition(GetX(FindObject(PLCE)),GetY(FindObject(PLCE)),GetCrew(i));
SetPlrViewRange(90,GetCrew(i));
SetFoW(1,i);

for(var c=ContentsCount(0,GetCrew(i)); c >-1; c--)
{
var obj;
var shield;

SetObjectStatus(1,Contents(c,GetCrew(i)));


if(GetID(obj=Contents(c,GetCrew(i))) == MUSK)SetObjectStatus(1,Contents(0,obj));
if(GetID(shield=Contents(c,GetCrew(i))) == SHIE) { SetCategory(3600,shield); shield->Activate(GetCursor(i));}
}

}

var wipf;
wipf=CreateObject(WIPF,GetX(FindObject(PLCE)),GetY(FindObject(PLCE)),-1);
SetClrModulation(RGB(220,230,70),wipf);

CreateObject(_ETG);
CreateObject(NPRY);
CreateObject(_SUI);
CreateObject(FSTF);
CreateObject(STES);
CreateObject(MGES);
SetSkyAdjust(RGB(10,10,100));

  //Truhen füllen
  FillAllChests();
}



func Script30()
{

if(!FindObject(WIPF)) {
Message("@Der Wipf ist tot! Du hast versagt");
for(var i=0; i < GetPlayerCount(); i++)
EliminatePlayer(GetPlayerByIndex(i));
SetPlrViewRange(0,GetCrew(i));
ScriptGo(0);
return(1);
}


var clnk;
//if(clnk=FindObject(0,2910,360,70,80,OCF_CrewMember(),0,0,0,clnk)) {
while(clnk=FindObject(0,2910,600,100,80,OCF_CrewMember(),0,0,0,clnk)) {
if(FindContents(WIPF,clnk)) { goto(32); return(1); } }
goto(28);

}

func Script32()
{

Global(123)=0;
for(var i;i < 12; i++) {
Exit(GetCursor(i));
SetObjectStatus(2,GetCrew(i));

for(var shield=0; shield < ObjectCount(SHIA); shield++)
{
var shia=FindObject(SHIA);
shia->Unbuckle();
if(GetOwner(shia)==-1)RemoveObject(shia);
Enter(GetCrew(GetOwner(shia)),shia);
SetObjectStatus(2,shia);
}

for(var c=0; c < ContentsCount(0,GetCrew(i)); c++)
{
var musk;
if(GetID(musk=Contents(c,GetCrew(i))) == MUSK)SetObjectStatus(2,Contents(0,musk));
if(GetID(musk=Contents(c,GetCrew(i))) == WIPF)RemoveObject(musk);
SetObjectStatus(2,Contents(c,GetCrew(i)));
}
}

LoadScenarioSection("Boss");

  var s;
 while(s=FindObject(_STA,0,0,0,0,0,0,0,0,s))s->Initialize();

var i=0;
for(var i;i < 12; i++) {
SetObjectStatus(1,GetCrew(i));
SetPosition(GetX(FindObject(PLCE)),GetY(FindObject(PLCE)),GetCrew(i));
SetPlrViewRange(90,GetCrew(i));
SetFoW(1,i);

for(var c=ContentsCount(0,GetCrew(i)); c >-1; c--)
{
var obj;
var shield;

SetObjectStatus(1,Contents(c,GetCrew(i)));


if(GetID(obj=Contents(c,GetCrew(i))) == MUSK)SetObjectStatus(1,Contents(0,obj));
if(GetID(shield=Contents(c,GetCrew(i))) == SHIE) { SetCategory(3600,shield); shield->Activate(GetCursor(i));}
}

}

var wipf;
wipf=CreateObject(WIPF,GetX(FindObject(PLCE)),GetY(FindObject(PLCE)),-1);
SetClrModulation(RGB(220,230,70),wipf);

CreateObject(MNST,LandscapeWidth()-200,300,-1);

CreateObject(_ETG);
CreateObject(NPRY);
CreateObject(_SUI);
  CreateObject(FSTF);
  CreateObject(STES);
  CreateObject(MGES);
SetSkyAdjust(RGB(10,10,100));

  //Truhen füllen
  FillAllChests();
}

func Script37()
{

if(!FindObject(WIPF)) {
Message("@Der Wipf ist tot! Du hast versagt");
for(var i=0; i < GetPlayerCount(); i++)
EliminatePlayer(GetPlayerByIndex(i));
SetPlrViewRange(0,GetCrew(i));
ScriptGo(0);
return(1);
}


//var clnk;
//if(clnk=FindObject(0,2910,360,70,80,OCF_CrewMember(),0,0,0,clnk)) {
if(!FindObject(MNST)) {
for(var i; i < GetPlayerCount(); i++)if(FindContents(WIPF,GetCursor(GetPlayerByIndex(i)))) {
goto(38); return(1); } }
goto(35);

}

func Script38()
{
Global(123)=0;
for(var i;i < 12; i++) {
Exit(GetCursor(i));
SetObjectStatus(2,GetCrew(i));

for(var shield=0; shield < ObjectCount(SHIA); shield++)
{
var shia=FindObject(SHIA);
shia->Unbuckle();
if(GetOwner(shia)==-1)RemoveObject(shia);
Enter(GetCrew(GetOwner(shia)),shia);
SetObjectStatus(2,shia);
}

for(var c=0; c < ContentsCount(0,GetCrew(i)); c++)
{
var musk;
if(GetID(musk=Contents(c,GetCrew(i))) == MUSK)SetObjectStatus(2,Contents(0,musk));
if(GetID(musk=Contents(c,GetCrew(i))) == WIPF)RemoveObject(musk);
SetObjectStatus(2,Contents(c,GetCrew(i)));
}
}

LoadScenarioSection("Tower");

var i=0;
for(var i;i < 12; i++) {
SetObjectStatus(1,GetCrew(i));
SetPosition(GetX(FindObject(PLCE)),GetY(FindObject(PLCE)),GetCrew(i));
SetFoW(1,i);
SetPlrViewRange(100,GetCrew(i));

for(var c=ContentsCount(0,GetCrew(i)); c >-1; c--)
{
var obj;
var shield;

SetObjectStatus(1,Contents(c,GetCrew(i)));


if(GetID(obj=Contents(c,GetCrew(i))) == MUSK)SetObjectStatus(1,Contents(0,obj));
if(GetID(shield=Contents(c,GetCrew(i))) == SHIE) { SetCategory(3600,shield); shield->Activate(GetCursor(i));}
}

}
var wipf;
wipf=CreateObject(WIPF,GetX(FindObject(PLCE)),GetY(FindObject(PLCE)),-1);
SetClrModulation(RGB(220,230,70),wipf);
SetSkyAdjust(RGB(250,250,250));
}


func Script40()
{
if(!FindObject(WIPF)) {
Message("@Der Wipf ist tot! Du hast versagt");
for(var i=0; i < GetPlayerCount(); i++)
EliminatePlayer(GetPlayerByIndex(i));
SetPlrViewRange(0,GetCrew(i));
ScriptGo(0);
return(1);
}


var clnk;
//if(clnk=FindObject(0,2910,360,70,80,OCF_CrewMember(),0,0,0,clnk)) {
while(clnk=FindObject(0,918,111,100,50,OCF_CrewMember(),0,0,0,clnk)) {
if(FindContents(WIPF,clnk)) { goto(41); return(1); } }
goto(39);

}

func Script41()
{
Message("@<c %x>Sven2: Hurra! Du hast uns den Wipf wieder gebracht!",FindObject(CLNK),GetColorDw(FindObject(CLNK)));
}

func Script52()
{
Message("@<c %x>Sven2: Nun können wir die GWE weiter entwickeln!",FindObject(CLNK),GetColorDw(FindObject(CLNK)));
}

func Script56()
{
CreateObject(FADE);
}

func Script69()
{
Global(123)=0;
for(var i;i < 12; i++) {
Exit(GetCursor(i));
SetObjectStatus(2,GetCrew(i));

for(var shield=0; shield < ObjectCount(SHIA); shield++)
{
var shia=FindObject(SHIA);
shia->Unbuckle();
if(GetOwner(shia)==-1)RemoveObject(shia);
Enter(GetCrew(GetOwner(shia)),shia);
SetObjectStatus(2,shia);
}

for(var c=0; c < ContentsCount(0,GetCrew(i)); c++)
{
var musk;
if(GetID(musk=Contents(c,GetCrew(i))) == MUSK)SetObjectStatus(2,Contents(0,musk));
SetObjectStatus(2,Contents(c,GetCrew(i)));
}
}

LoadScenarioSection("TowerEnd");

SetSkyAdjust(RGB(10,10,10));
var i=0;
for(var i;i < 12; i++) {
SetObjectStatus(1,GetCrew(i));
SetPosition(GetX(FindObject(PLCE)),GetY(FindObject(PLCE)),GetCrew(i));
SetFoW(0,i);
SetPlrViewRange(200,GetCrew(i));


if(GetCursor(i)) {
Enter(FindObject(PLCE),GetCursor(i));
}


for(var c=ContentsCount(0,GetCrew(i)); c >-1; c--)
{
var obj;
var shield;

SetObjectStatus(1,Contents(c,GetCrew(i)));


if(GetID(obj=Contents(c,GetCrew(i))) == MUSK)SetObjectStatus(1,Contents(0,obj));
if(GetID(obj=Contents(c,GetCrew(i))) == WIPF)RemoveObject(obj);
if(GetID(shield=Contents(c,GetCrew(i))) == SHIE) { SetCategory(3600,shield); shield->Activate(GetCursor(i));}
}

}

SetGamma(RGB(0,0,0),RGB(128,128,128),RGB(255,255,255));

Global(50)=CreateObject(CLNK,987,155,-1);
SetColorDw(RGB(255,255,0),Global(50));
SetDir(0,Global(50));
SetName("Sven2",Global(50));
AddEffect("Heal",Global(50),20,1);

Global(51)=CreateObject(CLNK,1090,275,-1);
SetColorDw(RGB(128,128,255),Global(51));
SetDir(0,Global(51));
SetName("boni",Global(51));


Global(52)=CreateObject(CLNK,890,95,-1);
SetColor(1,Global(52));
SetDir(0,Global(52));

}

func Script71()
{
Message("@<c %x>Sven2: So, endlich herscht wieder Frieden im Clonkreich",Global(50),GetColorDw(Global(50)));
}

func Script80()
{
CreateObject(HELP);
}


func Script81()
{
Message("@<c %x>boni: Wtf?!",Global(51),GetColorDw(Global(51)));
}

func Script85()
{
Message("@<c %x>Sven2: Wir können nun ungestört an unserer Engine weiterarbeiten :)",Global(50),GetColorDw(Global(50)));
}


func Script89()
{
Message("@<c %x>boni: EH?",Global(51),GetColorDw(Global(51)));
SetCommand(Global(51),"MoveTo",Global(51),10);
SetCommand(Global(52),"MoveTo",Global(52),10);
}



func Script95()
{
Message("@<c %x>boni: HILFE! Das kitzelt!",Global(51),GetColorDw(Global(51)));
SetCommand(Global(51),"MoveTo",Global(51),-15);
}

func Script99()
{
Message("@<c %x>Sven2: Solang wir boni zur Unterhaltung haben kann nichts schief gehen...!",Global(50),GetColorDw(Global(50)));
Message("@<c %x>Clonk: Yay!",Global(52),GetColorDw(Global(52)));
}

func Script105()
{
SetCommand(Global(51),"MoveTo",Global(51),-30,-100);
Message("@<c %x>boni: Omg! blaue Particle naiiin!",Global(51),GetColorDw(Global(51)));
SetCommand(Global(52),"MoveTo",Global(52),-40);
Message("<c %x>",Global(52),GetColorDw(Global(52)));
}

func Script115()
{
Message("@<c %x>Sven2: Öhm wo ist boni überhaupt?!",Global(50),GetColorDw(Global(50)));
SetCommand(Global(50),"MoveTo",Global(50),20);
SetCommand(Global(52),"MoveTo",Global(52),10);
Message("@<c %x>Clonk: kA?!",Global(52),GetColorDw(Global(52)));
}

func Script127()
{
Message("@<c %x>Sven2: BONI?!",Global(50),GetColorDw(Global(50)));
SetCommand(Global(50),"MoveTo",Global(50),-40);
Message("@<c %x>",Global(52),GetColorDw(Global(52)));
}

func Script133()
{
Message("@<c %x>Sven2: Holt die Paladine zurück!",Global(50),GetColorDw(Global(50)));
SetCommand(Global(50),"MoveTo",Global(50),-30);
}

func Script135()
{
Message("@<c %x>Clonk: KE!",Global(52),GetColorDw(Global(52)));
SetCommand(Global(52),"Enter",FindObject(CPKT));
}

func Script140()
{
Message("@<c %x>",Global(52),GetColorDw(Global(52)));
for(var i; i<GetPlayerCount(); i++) {
SetFoW(1,GetPlayerByIndex(i));
SetPlrViewRange(0,GetCursor(GetPlayerByIndex(i)));
}
Message("@<c %x>",Global(50),GetColorDw(Global(50)));
}

func Script141()
{
Message("@<c %x>- [ -",0,RGB(250,0,0));
Sound("Keyboard1",0,FindObject(PLCE));
}

func Script142()
{
Message("@<c %x>- [ -",0,RGB(250,0,0));
Sound("Keyboard1",0,FindObject(PLCE));
}

func Script143()
{
Message("@<c %x>- [ T -",0,RGB(250,0,0));
Sound("Keyboard1",0,FindObject(PLCE));
}

func Script144()
{
Message("@<c %x>- [ TH -",0,RGB(250,0,0));
Sound("Keyboard1",0,FindObject(PLCE));
}

func Script145()
{
Message("@<c %x>- [ THE -",0,RGB(250,0,0));
Sound("Keyboard1",0,FindObject(PLCE));
}

func Script146()
{
Message("@<c %x>- [ THE  -",0,RGB(250,0,0));
Sound("Keyboard1",0,FindObject(PLCE));
}

func Script147()
{
Message("@<c %x>- [ THE E -",0,RGB(250,0,0));
Sound("Keyboard1",0,FindObject(PLCE));
}


func Script148()
{
Message("@<c %x>- [ THE EN -",0,RGB(250,0,0));
Sound("Keyboard1",0,FindObject(PLCE));
}

func Script149()
{
Message("@<c %x>- [ THE END -",0,RGB(250,0,0));
Sound("Keyboard1",0,FindObject(PLCE));
}

func Script150()
{
Message("@<c %x>- [ THE END  -",0,RGB(250,0,0));
Sound("Keyboard1",0,FindObject(PLCE));
}

func Script151()
{
Message("@<c %x>- [ THE END ] -",0,RGB(250,0,0));
Sound("Keyboard1",0,FindObject(PLCE));
}

func Script155()
{
Message("@<c %x>- [ THE END ] (?) -",0,RGB(250,0,0));
Sound("Keyboard1",0,FindObject(PLCE));
}


func Script161()
{
GameOver();
}

global func FxHealTimer(pTarget)
{
return(1);
}


global func FxHealDamage(pTarget,iEffectNumber,iCause)
{
return(0);
}


