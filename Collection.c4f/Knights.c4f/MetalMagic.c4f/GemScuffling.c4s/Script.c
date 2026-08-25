/*-- Szenarioscript --*/

#strict

static aGems;
static aTeamsPayed;

protected func Create(id ID, int iAmmount)
{
  for(var i = iAmmount; i > 0; i--)
    PlaceInMaterial(ID,Material("Earth"));
}

protected func Initialize()
{
  Create(FLNT,RandomX(4,8)*3);
  Create(ROCK,RandomX(5,9)*2);
  Create(LOAM,RandomX(3,5)*3);
  Create(GOLD,RandomX(4,9)*2);
 /*var of=FindObject(CPOF,1700,369);
 of->SetPosition(GetX(of)+1,GetY(of));
 
 var of=FindObject(CPCH,259,308);
 of->SetPosition(GetX(of)+2,GetY(of));*/
 
 // Team-Melee Objekt erschaffen
 if (!FindObject(MELE)) CreateObject(MELE);
 
 var aRules=[_ETG,NBTR,FGRV,TACC];
 
 for(var ID in aRules)
 {
  if(!ObjectCount(ID))
   CreateObject(ID,19,19,-1);
 }
 
 aGems=[[],[]];
 aTeamsPayed=[];
 
 // Am Anfang weht kein Wind
 SetWind(0);
 
 DigCastlesFree();
 
 // Scriptzähler starten
 return(ScriptGo(1));
}

func Script0()
{
 CreateGems();
}

func CreateGems()
{
 var xPos=[[111,252,141],[1712,1569,1685]];
 var yPos=[[379+5,381+5,257+5],[373+5,376+5,254+5]];
 
 for(var i=0;i<2;i++)
 {
  for(var cnt=0;cnt<3;cnt++)
  {
   var gem=CreateObject(GMSN,xPos[i][cnt],yPos[i][cnt],-1);
   LocalN("iTeam",gem)=i+1;
   var sGraphics="";
   if(i==1)sGraphics="3";
   gem->AdjustGraphics(sGraphics);
   aGems[i][cnt]=gem;
   gem->InitHere();
  }
 }
}

func RefreshScoreboard(norefresh)
{
 SetScoreboardData(-1,-1,"$GemOverview$");
 SetScoreboardData(-1,0,"$TeamLeft$",-1);
 SetScoreboardData(-1,1,"$TeamRight$",-1);
 
 if(!norefresh)
  Schedule("GameCall(\"RefreshScoreboard\",true)",5,0);
 
 for(var i=0;i<2;i++)
 {
  var infostring="";
  var gemstring="";
  for(var cnt=0;cnt<3;cnt++)
  {
   var pGem=aGems[i][cnt];
   if(!pGem)continue;
   var gemid=GMSN;
   if(i == 1) gemid=_SY5;
   if(Contained(pGem))infostring=Format("%s{{%i}}",infostring,_SY1);
   else if(pGem->AtHome())infostring=Format("%s{{%i}}",infostring,_SY2);
   else infostring=Format("%s{{%i}}",infostring,_SY3);
   gemstring=Format("%s{{%i}}",gemstring,gemid);
  }
  if(!GetLength(gemstring))
  {
   for(var plr=0;plr<GetPlayerCount();plr++)
   {
    var iPlr=GetPlayerByIndex(plr);
    if(GetPlayerTeam(iPlr) != i+1)continue;
    EliminatePlayer(iPlr);
   }
  }
  SetScoreboardData(0,i,infostring,1);
  SetScoreboardData(1,i,gemstring,1);
 }
}

protected func DigCastlesFree()
{
 for(var obj;obj=FindObject(0,0,0,0,0,0,0,0,NoContainer(),obj);)
 {
  if(!obj->~CastlePartWidth())continue;
  var x=GetX(obj)+GetDefCoreVal("Offset","DefCore",GetID(obj),0);
  var y=GetY(obj)+GetDefCoreVal("Offset","DefCore",GetID(obj),1);
  var w=GetDefCoreVal("Width","DefCore",GetID(obj),0);
  var h=GetDefCoreVal("Height","DefCore",GetID(obj),0);
  DigFreeRect(x,y,w,h);
 }
}

protected func InitializePlayer(int player, X, Y, foo, iTeam)
{
 // Den Spieler ausrüsten und platzieren
// CreateContents(FLAG, GetCrew(player,1));
 CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->~InitPlayer(player,iTeam);
 if(!aTeamsPayed[iTeam])
 {
  DoWealth(player,600);
  aTeamsPayed[iTeam]=true;
 }
 return(1);
}

protected func RemovePlayer(int player)
{
 // Spielervariable zurücksetzen, damit Laufzeitbeitritt mit Aufgeben funzt
 Global(player) = 0;
 return(1);
}

protected func RelaunchPlayer(int player)
{
 // Den Tod des Spielers verkünden
 Log("$Relaunch$", GetPlayerName(player));
 Sound("Trumpet");

 // Einen Ritter
 var knight = CreateObject(KNIG, 0,0, player);
 MakeCrewMember(knight, player);
 DoEnergy(100-GetEnergy(knight), knight);
 
 // Einen Paladin
 var pala = CreateObject(PLDN, 0,0, player);
 MakeCrewMember(pala, player);
 DoEnergy(100-GetEnergy(pala), pala);
 
 /*// Einen Assassin
 var asas = CreateObject(ASAS, 0,0, player);
 MakeCrewMember(asas, player);
 DoEnergy(100-GetEnergy(asas), asas);*/
 
 SetCursor(player, GetHiRank(player));
 CreateContents(FLAG, GetHiRank(player));

 // Jetzt muss der Spieler noch etwas warten
 CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->RelaunchPlayer(player);
 return(1);
}

protected func Script10()
{
 
}

protected func InitializePlayer(int player, X, Y, foo, iTeam)
{
 for(var cnt=0;GetCrew(player,cnt);cnt++) DoEnergy(100,GetCrew(player,cnt));
 return(_inherited(player, X, Y, foo, iTeam));
}

// Callbacks aus dem PlayerObject
func JoinTeam(iPlr, iTeam)
{
  if(iTeam==1) Log("$MsgLeftJoined$", GetPlayerName(iPlr));
  if(iTeam==2) Log("$MsgRightJoined$", GetPlayerName(iPlr));
}

func GetTeamStartPosition(iTeam, fY)
{
  if(iTeam==1) { if(fY) return(364); return(110); }
  if(iTeam==2) { if(fY) return(376); return(1715); }
}

func InitializeTeam(iTeam, iPlr)
{
  if(iTeam==1)
  {
    AcquireBase(iPlr, 40,135, 430-40,480-135);
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(CPOF, iPlr)));
  }
  if(iTeam==2)
  {
    AcquireBase(iPlr, 1385,175, 1784-1385,488-175);
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(CPOF, iPlr)));
  }
}

func GetTeamName(iTeam)
{
  if(iTeam==1) return("$TeamLeft$");
  if(iTeam==2) return("$TeamRight$");
}
