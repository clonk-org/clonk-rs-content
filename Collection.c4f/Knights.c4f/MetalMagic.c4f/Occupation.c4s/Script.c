/*-- Einnehmen --*/

#strict

protected func Initialize()
{
 if(!ObjectCount(MELE))
  CreateObject(MELE,10,10,-1);
  
 if(!ObjectCount(SURR))
  CreateObject(SURR,10,10,-1);
 
 if(!ObjectCount(TACC))
  CreateObject(TACC,10,10,-1);
 
 if(!ObjectCount(MGES))
  CreateObject(MGES,10,10,-1);
 
 if(!ObjectCount(NPRY))
  CreateObject(NPRY,10,10,-1);
 
   // Truhen füllen
  for(var chest in FindObjects(Find_ID(CHST)))
  {
    while(Contents(0,chest)) RemoveObject(Contents(0,chest));
    FillChest(chest);
  }
 
  // Sonne!
  CreateObject(LENS, LandscapeWidth()/2, 168);

  ScriptGo(1);
}

protected func FillChest(object pChest)
{
 var count=4+Random(6);
 while(count--)
 CreateContents(RandChestID(),pChest);
}

protected func RandChestID()
{
 var rand=Random(20);
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
 if(!--rand)return(SBLP);
 if(!--rand)return(PBLP);
 if(!--rand)return(HBTP);
 if(!--rand)return(BOTP);
 if(!--rand)return(HOOK);
 if(!--rand)return(KNFP);
 if(!--rand)return(SCKZ);
 return(COKI);
}

func Script10()
{
 var iTeam=-1;
 for(var obj in FindObjects(Find_ID(SNHD)))
 {
  if(!GetPlayerName(GetOwner(obj))){iTeam=-1;break;}
  if(iTeam == -1){iTeam=GetPlayerTeam(GetOwner(obj));continue;}
  if(iTeam == GetPlayerTeam(GetOwner(obj)))continue;
  iTeam=-1;
  break;
 }
 
 
 if(iTeam == -1)return(goto(9));
 
 EndRound(iTeam);
 
 ScriptGo(0);
}

func SainthoodBufferTime(){return(35*3);}

func EndRound(iTeam)
{
 for(var i=0;i<GetPlayerCount();i++)
 {
  var iPlr=GetPlayerByIndex(i);
  if(GetPlayerTeam(iPlr) == iTeam)continue;
  EliminatePlayer(iPlr);
 }
 var sTeamName="$Left$";
 if(iTeam == 2)sTeamName="$Right$";
 Message("@$TeamWins$",0,sTeamName);
}

protected func InitializePlayer(int player, int tx, int ty, object pBase, int iTeam)
{
 var x=0;
 if(iTeam == 2)x=LandscapeWidth();
 
 var pSaint=FindObject(SNHD,x,LandscapeHeight()/2,-1,-1);
 if(GetPlayerTeam(GetOwner(pSaint)) == -1 || !GetPlayerName(GetOwner(pSaint)))
  SetOwner(player,pSaint);
 
 JoinPlayer(player);
 RefreshScoreboard();
 return(1);
}

func OnSainthoodOwnerChange()
{
 RefreshScoreboard();
}

func RefreshScoreboard()
{
 var sainthoods=FindObjects(Find_ID(SNHD),Sort_Distance(0,0));
 var sTopics=["$Left$","$Middle$","$Right$"];
 
 var index=-1;
 for(var obj in sainthoods)
 {
  index++;
  var sString="/";
  var iOwner=GetOwner(obj);
  var iTeam=GetPlayerTeam(GetOwner(obj));
  
  if(GetPlayerName(iOwner))
  {
   if(iTeam == 1)
    sString="Team $Left$";
   else if(iTeam == 2)
    sString="Team $Right$";
  }
  
  SetScoreboardData(-1,index,"{{SNHD}}",-1);
  SetScoreboardData(0,index,sString,0);
 }
 
 for(var cnt=0;cnt<GetPlayerCount();cnt++)
 {
  var plr=GetPlayerByIndex(cnt);
  DoScoreboardShow(5,plr);
 }  
}

protected func RelaunchPlayer(iPlr)
{
 var crew=CreateObject(CLNK,0,0,iPlr);
 MakeCrewMember(crew,iPlr);
 
 JoinPlayer(iPlr);
}

protected func JoinPlayer(iPlr)
{
 var crew=GetCrew(iPlr,0);
 
 DoEnergy(100,crew);
 DoMagicEnergy(20,crew); 
  
 var pHelper=CreateObject(_RLC,LandscapeWidth()/2,LandscapeHeight()/2,-1);
 pHelper->Init(iPlr);
 
 SelectCrew(iPlr,crew,1);
 SetPlrView(iPlr,crew);
}







