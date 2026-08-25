/*-- Quadromago --*/

#strict

func Initialize() {

  // Truhen füllen
  for(var chest in FindObjects(Find_ID(CHST)))
  {
    while(Contents(0,chest)) RemoveObject(Contents(0,chest));
    FillChest(chest);
  }

  if(!ObjectCount(MELE)) CreateObject(MELE,10,10,-1);
  if(!ObjectCount(NMNL)) CreateObject(NMNL,11,11,-1);
  if(!ObjectCount(STES)) CreateObject(STES,12,12,-1);
  if(!ObjectCount(RSST)) CreateObject(RSST,13,13,-1);
  if(!ObjectCount(ATCC)) CreateObject(ATCC,14,14,-1);
  if(!ObjectCount(FXST)) CreateObject(FXST,15,15,-1);
  CreateObject(_EVS, 10, 10, -1);

  for(var i=0; i<5; i++)
    PlaceAnimal(BIRD);
}

protected func FillChest(object pChest)
{
 var count=4+Random(6);
 while(count--)
 CreateContents(RandChestID(),pChest);
}

protected func RandChestID()
{
 var rand=Random(13);
 if(!rand)return(BRED);
 if(!--rand)return(SPER);
 if(!--rand)return(FARP);
 if(!--rand)return(XARP);
 if(!--rand)return(STFN);
 if(!--rand)return(EFLN);
 if(!--rand)return(SKUL);
 if(!--rand)return(TENP);
 if(!--rand)return(SCRL);
 if(!--rand)return(PMAN);
 if(!--rand)return(PMON);
 if(!--rand)return(PWIP);
 return(COKI);
}

protected func InitializePlayer(int player, int tx, int ty, object pBase, int iTeam)
{
  if(!GetEffect("FragsSaver"))AddEffect("FragsSaver",0,20,1000,0,0);
 
  SetScoreboardData(GetPlayerID(player),SBRD_Caption,GetTaggedPlayerName(player),GetPlayerID(player));
  EffectCall(0,GetEffect("FragsSaver",0),"RefreshScoreboard");
  
  Global(player) = 0;
//  Enter(CreateObject(DSHP, 100, 100, player),GetCrew(player));
  JoinPlayer(player);
  return(1);
}

func OnChurchChoose(iPlr)
{
  RemoveObject(Contained(GetCrew(iPlr)), 1);
  JoinPlayer(iPlr);
}

protected func RelaunchPlayer(iPlr)
{
  if(iPlr < 0) return();
  ++Global(iPlr);
  if(Global(iPlr)>3)
  {
    SetScoreboardData(GetPlayerID(iPlr),ScoreboardCol(SCKZ),Format("{{%i}}",SKUL),Global(iPlr));
    SortScoreboard(ScoreboardCol(SWOR),1);
    SortScoreboard(ScoreboardCol(SCKZ),0);
    return();
  }
 
  var crew=CreateObject(MAGE,10,10,iPlr);
  MakeCrewMember(crew,iPlr); 
  JoinPlayer(iPlr);
}

protected func RemovePlayer(iPlr)
{
  // Gestorbener Spieler? Dann behalten
  if(Global(iPlr)>3) return();
  // Relaunchzähler ium Scoreboard ausleeren
  var iPlrID = GetPlayerID(iPlr);
  SetScoreboardData(iPlrID,       ScoreboardCol(SWOR),       0, -1);
  SetScoreboardData(iPlrID,       ScoreboardCol(SCKZ),       0, -1);
}

protected func JoinPlayer(iPlr)
{
 var x=-1;
 var y=-1;
 GetSpawnPosition(x,y);
 if(x==-1)
 {
   x=100;y=400;
 }
 
 var crew = GetCrew(iPlr);
 var help = CreateObject(DSHP,x,y,iPlr);
 SetYDir(0,help);
 SetPosition(x,y,crew);
 Enter(help,crew);
 AddEffect("Relaunch",help,20,4,0,0);
 
 DoEnergy(100,crew);
 DoMagicEnergy(200, crew, 1);
 if(FindContents(SCKZ, crew)) RemoveObject(FindContents(SCKZ, crew));
 if(Global(iPlr)==0) CreateContents(SMAR, crew);
 if(Global(iPlr)==1) CreateContents(SMFR, crew);
 if(Global(iPlr)==2) CreateContents(SMER, crew);
 if(Global(iPlr)==3) CreateContents(SMWT, crew);
 
 SelectCrew(iPlr,crew,1);
 
 EffectCall(0,GetEffect("FragsSaver",0),"RefreshScoreboard");
}

protected func GetSpawnPosition(&x,&y)
{
 for(var cnt=0;cnt<10000 && x==-1;cnt++)
 {
  var pWipf=PlaceAnimal(WIPF);
  if(GBackSolid(GetX(pWipf)-10,GetY(pWipf)-5)
  || GBackSolid(GetX(pWipf)+10,GetY(pWipf)-5)
  || GBackSolid(GetX(pWipf),GetY(pWipf)-30)
  || GBackSolid(GetX(pWipf),GetY(pWipf)-5)) { RemoveObject(pWipf); continue; }
  
  if(FindObject2(Find_OCF(OCF_CrewMember()), Find_NoContainer(), Find_Distance(100, GetX(pWipf), GetY(pWipf)))) { RemoveObject(pWipf); continue; }
  
  x=GetX(pWipf);
  y=GetY(pWipf);
  
  RemoveObject(pWipf);
  break;
 }
}

global func FxFragsSaverStart(pTarget,iEffectNumber)
{
  SetScoreboardData(SBRD_Caption, SBRD_Caption, "Frags/Kills",SBRD_Caption);
  SetScoreboardData(SBRD_Caption,ScoreboardCol(SWOR),"{{SWOR}}",ScoreboardCol(SWOR));
  SetScoreboardData(SBRD_Caption,ScoreboardCol(SCKZ),"{{SCKZ}}",ScoreboardCol(SCKZ));

  EffectCall(pTarget,iEffectNumber,"RefreshScoreboard");
  DoScoreboardShow(1);
  return(1);
}

global func FxFragsSaverTimer()
{ 
 return(1);
}

global func FxFragsSaverGetFrags(pTarget,iEffectNumber,iPlr)
{
 return(EffectVar(iPlr,pTarget,iEffectNumber));
}

global func FxFragsSaverAddFrag(pTarget,iEffectNumber,iPlr)
{
 DoScoreboardShow(1);
 EffectVar(iPlr,pTarget,iEffectNumber)++;
 EffectCall(pTarget,iEffectNumber,"RefreshScoreboard");
 return(1);
}

global func FxFragsSaverRemFrag(pTarget,iEffectNumber,iPlr)
{
 DoScoreboardShow(1);
 EffectVar(iPlr,pTarget,iEffectNumber)--;
 EffectCall(pTarget,iEffectNumber,"RefreshScoreboard");
 return(1);
}

global func FxFragsSaverRefreshScoreboard(pTarget,iEffectNumber)
{ 
 for(var cnt=0;cnt<GetPlayerCount();cnt++)
 {
   var plr=GetPlayerByIndex(cnt);
   var plrID=GetPlayerID(GetPlayerByIndex(cnt));
    
   SetScoreboardData(plrID,ScoreboardCol(SWOR),Format("%d",EffectVar(plr,pTarget,iEffectNumber)),EffectVar(plr,pTarget,iEffectNumber));
   if(Global(plr)==0) SetScoreboardData(plrID,ScoreboardCol(SCKZ),Format("{{%i}}",AIR1),Global(plr));
   if(Global(plr)==1) SetScoreboardData(plrID,ScoreboardCol(SCKZ),Format("{{%i}}",FIRE),Global(plr));
   if(Global(plr)==2) SetScoreboardData(plrID,ScoreboardCol(SCKZ),Format("{{%i}}",EART),Global(plr));
   if(Global(plr)==3) SetScoreboardData(plrID,ScoreboardCol(SCKZ),Format("{{%i}}",WATR),Global(plr));
   if(Global(plr)> 3) SetScoreboardData(plrID,ScoreboardCol(SCKZ),Format("{{%i}}",SKUL),Global(plr));
 }

 SortScoreboard(ScoreboardCol(SWOR),1);
 SortScoreboard(ScoreboardCol(SCKZ),0);
 
 return(1);
}

global func FxRelaunchStart(pTarget)
{
 SetVisibility(VIS_Owner()|VIS_God(),pTarget);
 return(1);
}

global func FxRelaunchTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(iEffectTime>90)return(-1);
 
 var angle=Random(360);
 var rand=Random(20);
 var cos=Cos(angle,rand);
 var sin=Sin(angle,rand);
 
 CreateParticle("PxSpark",GetX(pTarget)+cos,GetY(pTarget)+sin,-cos,-sin-10,50,RGB(10+iEffectTime,10+iEffectTime,10+iEffectTime),pTarget);
 
 return(1);
}

global func FxRelaunchStop(pTarget)
{
  var pClonk = FindObject(MAGE, 0, 0, 0, 0, 0, 0, 0, pTarget);
  if(pTarget) RemoveObject(pTarget,1);
}
