/*-- Relict Hunters --*/

#strict

static iAir;
static iWater;
static iEarth;
static iFire;

func Initialize() {

  // CastlePanic();
  DigCastlesFree();
  for(var chest in FindObjects(Find_ID(CHST)))
  {
  while(Contents(0,chest))RemoveObject(Contents(0,chest));
  FillChest(chest);
  }

  // Bodenmaterial erschaffen
  for(var cnt=0;cnt<30;cnt++)PlaceInMaterial(GOLD,Material("Earth"),0);
  for(var cnt=0;cnt<30;cnt++)PlaceInMaterial(FLNT,Material("Earth"),0);
  for(var cnt=0;cnt<20;cnt++)PlaceInMaterial(ROCK,Material("Earth"),0);
  for(var cnt=0;cnt<20;cnt++)PlaceInMaterial(BONE,Material("Earth"),0);
  for(var cnt=0;cnt<30;cnt++)PlaceInMaterial(LOAM,Material("Earth"),0);
  
  if(!FindObject(STES)) CreateObject(STES,10,10,-1); // Magie energie
  
  if(!ObjectCount(MELE))CreateObject(MELE,10,10,-1);
  if(!ObjectCount(_ETG))CreateObject(_ETG,10,10,-1);
  if(!ObjectCount(SPST))CreateObject(SPST,10,10,-1);
  if(!ObjectCount(CLGD))CreateObject(CLGD,10,10,-1);
  
  // Teamkonto einfügen
  if(!ObjectCount(TACC))CreateObject(TACC,10,10,-1);
  
  if(!FindObject(FSTF)) CreateObject(FSTF,10,10,-1); // Stäbe für Magi
  
  // Scoreboard!! :-)
  SetScoreboardData(SBRD_Caption, SBRD_Caption, "$TtlRelicts$",                    SBRD_Caption);
  SetScoreboardData(SBRD_Caption, ScoreboardCol(LEFT),       "{{CPT3}}",              ScoreboardCol(LEFT));
  SetScoreboardData(SBRD_Caption, ScoreboardCol(TEM1),       "{{SFB5}}",              ScoreboardCol(TEM1));
  SetScoreboardData(SBRD_Caption, ScoreboardCol(MIDL),       "{{ERTH}}",              ScoreboardCol(MIDL));
  SetScoreboardData(SBRD_Caption, ScoreboardCol(TEM2),       "{{SFB6}}",              ScoreboardCol(TEM2));
  SetScoreboardData(SBRD_Caption, ScoreboardCol(RIGT),       "{{CPT4}}",              ScoreboardCol(RIGT));
  SetScoreboardData(ScoreboardCol(AIR1), SBRD_Caption,  Format("%s", GetName(0,AIR1)), 1);
  SetScoreboardData(ScoreboardCol(WATR), SBRD_Caption,  Format("%s", GetName(0,WATR)), 2);
  SetScoreboardData(ScoreboardCol(EART), SBRD_Caption,  Format("%s", GetName(0,EART)), 3);
  SetScoreboardData(ScoreboardCol(FIRE), SBRD_Caption,  Format("%s", GetName(0,FIRE)), 4);
  SetScoreboardData(ScoreboardCol(AIR1), ScoreboardCol(MIDL),       "{{SFB1}}", 1);
  SetScoreboardData(ScoreboardCol(WATR), ScoreboardCol(MIDL),       "{{SFB2}}", 1);
  SetScoreboardData(ScoreboardCol(EART), ScoreboardCol(MIDL),       "{{SFB3}}", 1);
  SetScoreboardData(ScoreboardCol(FIRE), ScoreboardCol(MIDL),       "{{SFB4}}", 1);
  
  AddEffect("UpdateScoreboard", 0, 1, 35);
  
  return(1);
}

global func MirrorMap()
{
  var aObjects = FindObjects(Find_InRect(0,0,LandscapeWidth(),LandscapeHeight()));
  for(pObj in aObjects)
  {
    CreateObject(GetID(pObj), LandscapeWidth()-GetX(pObj), GetY(pObj)-GetDefOffset(GetID(pObj), 1));
  }
}

global func FxUpdateScoreboardTimer()
{
  var obj, szElement;
  var fShow;
  while(obj = FindObject(MRLC, 0, 0, 0, 0, 0, 0, 0, 0, obj))
  {
    szElement = LocalN("sElement", obj);
    if(szElement S= "Air")   if(CheckRelic(AIR1, SFB1, obj)) fShow = 1;
    if(szElement S= "Water") if(CheckRelic(WATR, SFB2, obj)) fShow = 1;
    if(szElement S= "Earth") if(CheckRelic(EART, SFB3, obj)) fShow = 1;
    if(szElement S= "Fire")  if(CheckRelic(FIRE, SFB4, obj)) fShow = 1;
  }
  if(fShow) DoScoreboardShowAll(1, 5);
}

global func GetState(idCol)
{
  if(idCol == AIR1) return(iAir);
  if(idCol == WATR) return(iWater);
  if(idCol == EART) return(iEarth);
  if(idCol == FIRE) return(iFire);
}

global func SetState(idCol, iValue)
{
  if(idCol == AIR1) iAir = iValue;
  if(idCol == WATR) iWater = iValue;
  if(idCol == EART) iEarth = iValue;
  if(idCol == FIRE) iFire = iValue;
}

global func CheckRelic(idCol, idSymbol, pRelic)
{
  var iState = GetState(idCol);
  SetScoreboardData(ScoreboardCol(idCol), ScoreboardCol(LEFT),       " ", 1);
  SetScoreboardData(ScoreboardCol(idCol), ScoreboardCol(TEM1),       " ", 1);
  SetScoreboardData(ScoreboardCol(idCol), ScoreboardCol(MIDL),       " ", 1);
  SetScoreboardData(ScoreboardCol(idCol), ScoreboardCol(TEM2),       " ", 1);
  SetScoreboardData(ScoreboardCol(idCol), ScoreboardCol(RIGT),       " ", 1);
  if( Inside(GetX(pRelic), 0, 467) && Inside(GetY(pRelic), 0, 950))
  {
    SetScoreboardData(ScoreboardCol(idCol), ScoreboardCol(LEFT),Format("{{%i}}", idSymbol), 1);
    if(iState != -2)
    {
      SetState(idCol, -2);
      return(1);
    }
  }
  else if( Inside(GetX(pRelic), 1033, 1500) && Inside(GetY(pRelic), 0, 950))
  {
    SetScoreboardData(ScoreboardCol(idCol), ScoreboardCol(RIGT),Format("{{%i}}", idSymbol), 1);
    if(iState != 2)
    {
      SetState(idCol, 2);
      return(1);
    }
  }
  else
  {
    if(Contained(pRelic))
    {
      if(GetPlayerTeam(GetOwner(Contained(pRelic)))==1)
      {
        SetScoreboardData(ScoreboardCol(idCol), ScoreboardCol(TEM1),Format("{{%i}}", idSymbol), 1);
        if(iState !=-1)
        {
          SetState(idCol,-1);
          return(1);
        }
    return();
      }
      else if(GetPlayerTeam(GetOwner(Contained(pRelic)))==2)
      {
        SetScoreboardData(ScoreboardCol(idCol), ScoreboardCol(TEM2),Format("{{%i}}", idSymbol), 1);
        if(iState != 1)
        {
          SetState(idCol, 1);
          return(1);
        }
    return();
      }
    }
    SetScoreboardData(ScoreboardCol(idCol), ScoreboardCol(MIDL),Format("{{%i}}", idSymbol), 1);
    if(iState != 0)
    {
      SetState(idCol, 0);
      return(1);
    }
  }
}

static iScoreboardShow;

global func DoScoreboardShowAll(iAmount, iTime)
{
    ResetScoreboard();
    DoScoreboardShow(iAmount);
    iScoreboardShow += iAmount;
    if(iTime)
    {   //Schedule("ResetScoreboard()", 38*iTime);
        goto(Max(1000-iTime*3, 0));
    ScriptGo(1);
    }
}

global func ResetScoreboard()
{
    DoScoreboardShow(-iScoreboardShow);
    iScoreboardShow=0;
    ScriptGo(0);
}

protected func FillChest(object pChest)
{
 var count=4+Random(7);
 while(count--)
 CreateContents(RandChestID(),pChest);
}

protected func RandChestID()
{
 var rand=Random(17);
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
 if(!--rand)return(TENP);
 if(!--rand)return(BHLG);
 if(!--rand)return(PBLP);
 if(!--rand)return(TSWD);
 if(!--rand)return(SBLP);
 return(COKI);
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
  if(iTeam == 1)
   MMSetPlayerChurch(player,scBloodchurch);
  else if(iTeam == 2)
   MMSetPlayerChurch(player,scLightchurch);
  CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->~InitPlayer(player,iTeam);
  // Magus mit voller Zauberenergie
  DoMagicEnergy(200, FindObject2(Find_ID(MAGE), Find_Owner(player)), 1);
  return(1);
}

protected func SetCastleOwner(int iPlr)
{
  var x=LandscapeWidth()/2;
  if(Global(iPlr) == 1) x=1;
 
  for(var obj=0;obj=FindObject(0,x,0,LandscapeWidth()/2,LandscapeHeight(),0,0,0,NoContainer(),obj);)
  {
    if(~GetCategory(obj) & C4D_Structure() && !obj->~CastlePartWidth() && GetID(obj) != BANR)continue;
    if(GetOwner(obj) != -1)continue;
    SetOwner(iPlr,obj);
  }
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

 var aCrewID = [PLDN, MAGE];
 // Dem Spieler Paladin, Ritter und Magus geben
 for (var i; i < 2; i++)
 {
  var knight = CreateObject(aCrewID[i], 0,0, player);
  MakeCrewMember(knight, player);
  DoEnergy(200, knight);
  DoMagicEnergy(200, knight, 1);
 }
 
 SetCursor(player, GetHiRank(player));
 CreateContents(FLAG, GetHiRank(player));

 // Jetzt muss der Spieler noch etwas warten
 CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->RelaunchPlayer(player);
 return(1);
}

// Callbacks aus dem PlayerObject
func JoinTeam(iPlr, iTeam)
{
  if(!MMGetChurch(iPlr))
  {
    var iChurch=scBloodchurch;
    if(iTeam == 2) iChurch=scLightchurch;
    MMSetPlayerChurch(iPlr,iChurch);
    var pChooser = FindObjectOwner(CHCS);
    if(pChooser) RemoveObject(pChooser);
  }
}

func GetTeamStartPosition(iTeam, fY)
{
  if(iTeam==1) { if(fY) return(816); return(64); }
  if(iTeam==2) { if(fY) return(816); return(1439); }
}

func InitializeTeam(iTeam, iPlr)
{
  if(iTeam==1)
  {
    AcquireBase(iPlr, 0, 0, 467,950);
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(CPOF, iPlr)));
    // Betenden Priester bekommen
   // var pClonk = CreateObject(PRST, 100, 627, iPlr);
    //MakeCrewMember(pClonk, iPlr);
   // pClonk->SetAction("Pray");
  }
  if(iTeam==2)
  {
    AcquireBase(iPlr, 1033,0,1500,950);
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(CPOF, iPlr)));
    // Betenden Priester bekommen
   // var pClonk = CreateObject(PRST,1400, 627, iPlr);
   // MakeCrewMember(pClonk, iPlr);
   // pClonk->SetAction("Pray");
  }
}

func GetTeamName(iTeam)
{
  if(iTeam==1) return("$TLeft$");
  if(iTeam==2) return("$TRight$");
}
