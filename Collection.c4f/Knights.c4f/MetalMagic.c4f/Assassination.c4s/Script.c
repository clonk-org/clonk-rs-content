/*-- Assassination --*/

#strict

static aASASX;
static aASASY;
static aKNIGX;
static aKNIGY;
static START_TIME;
static ITEM_TRANS_LOCKED;
static const TIME_PER_GEM=175;

static STONES_TO_STEAL;

func Initialize()
{
 ITEM_TRANS_LOCKED=false;
 
 STONES_TO_STEAL=12;
 START_TIME=TIME_PER_GEM*STONES_TO_STEAL;
 
 aKNIGX=[471,597,510];
 aKNIGY=[457,512,391];
 
 aASASX=[1939,2029,2094,1851,1965,2097];
 aASASY=[467,512,520,768,822,816];
 
 //for(var obj in FindObjects(Find_ID(GMSN)))
 // obj->Return();
 
 //if(!ObjectCount(_SUI))
 // CreateObject(_SUI,20,20,-1);
 if(!ObjectCount(GMSR))
  CreateObject(GMSR,20,20,-1);
 
 /*if(!ObjectCount(MEL2))
  CreateObject(MEL2,10,10,-1);
 if(!ObjectCount(SCRG))
  CreateObject(SCRG,15,15,-1);*/
 
 ScriptGo(1);
 // Keine KI :-(
// CreateScriptPlayer("Computer", RGB(Random(255),Random(255), Random(255)), 1);
}

func Script0()
{
 if(!GetEffect("MakeMoney"))
  AddEffect("MakeMoney",0,1,30,0,0);
 
 DoScoreboardRefresh();
 
// SetGamma(RGB(0,0,0),RGB(64,64,64),RGB(255,255,255));
 
 CastlePanic();
 
// IncinerateTorches();
 
 AddBabbleAI(FindObject2(Find_AtPoint(159,825),Find_Func("IsClonk")),"AlchemMage",6)->   SetName("Rubin");
 AddBabbleAI(FindObject2(Find_AtPoint(32,799),Find_Func("IsClonk")),"AlchemClonk",5)->   SetName("Miggi");
 AddBabbleAI(FindObject2(Find_AtPoint(377,632),Find_Func("IsClonk")),"LaborSorc",5)->    SetName("Bernhard");
 AddBabbleAI(FindObject2(Find_AtPoint(376,511),Find_Func("IsClonk")),"King",5)->         SetName("Guenther");
 AddBabbleAI(FindObject2(Find_AtPoint(319,506),Find_Func("IsClonk")),"NextToKing",5)->   SetName("Peter");
 AddBabbleAI(FindObject2(Find_AtPoint(153,394),Find_Func("IsClonk")),"ArmoryKnight",5)-> SetName("Kenny");
 AddBabbleAI(FindObject2(Find_AtPoint(403,819),Find_Func("IsClonk")),"KitchenSorc",5)->  SetName("Matthi");
 AddBabbleAI(FindObject2(Find_AtPoint(617,746),Find_Func("IsClonk")),"WaterClonk",6)->   SetName("Tobias");
 AddBabbleAI(FindObject2(Find_AtPoint(564,577),Find_Func("IsClonk")),"WorkshopClonk",5)->SetName("Arne");
 AddBabbleAI(FindObject2(Find_AtPoint(736,452),Find_Func("IsClonk")),"BedroomClonk",5)-> SetName("Merten");
 AddBabbleAI(FindObject2(Find_AtPoint(789,634),Find_Func("IsClonk")),"WebClonk",5)->     SetName("Marius");
 AddBabbleAI(FindObject2(Find_AtPoint(756,824),Find_Func("IsClonk")),"LibClonk",5)->     SetName("Minh");
 AddBabbleAI(FindObject2(Find_AtPoint(830,389),Find_Func("IsClonk")),"WatchClonk",5)->   SetName("Isi");
 AddBabbleAI(FindObject2(Find_AtPoint(609,638),Find_Func("IsClonk")),"MageDom",6)->      SetName("Sven");
 AddBabbleAI(FindObject2(Find_AtPoint(494,396),Find_Func("IsClonk")),"ConfessFather",5)->SetName("Richard");
 
 for(var obj in FindObjects(Find_ID(ELEV)))
  InitElevator(obj);
}

protected func InitElevator(elev)
{
 var x=12;
 var y=0;
 for(var count=0;!GBackSolid(GetX(elev),GetY(elev)+count);count++)
   y++;
 DrawMaterialQuad("Tunnel",GetX(elev)-x,GetY(elev),GetX(elev)+x-1,GetY(elev),GetX(elev)+x-1,GetY(elev)+y,GetX(elev)-x,GetY(elev)+y,1);
 elev->DoCon(10);
}

global func DoScoreboardRefresh()
{ 
 AddEffect("ScheduleRefreshCall",0,1,1,0);
 if(GetEffect("AdjustGoalTime"))
  EffectCall(0,GetEffect("AdjustGoalTime",0),"Check");
}

global func FxScheduleRefreshCallTimer()
{
 GameCall("DoRealRefresh");
 return(-1);
}

global func LockItemTrans(bTo)
{
 ITEM_TRANS_LOCKED=bTo;
}

func DoRealRefresh()
{
 var cnt=ObjectCount2(Find_ID(GMSN),Find_NoContainer())-(18-STONES_TO_STEAL);
 var cnt2=ObjectCount2(Find_ID(GMSN),Find_AnyContainer());
 
 SetScoreboardData(0,SBRD_Caption,"{{GMSN}}",cnt);
 SetScoreboardData(1,SBRD_Caption,"{{_ASB}}",cnt2);
 SetScoreboardData(2,SBRD_Caption,"{{SCRG}}",cnt2);
 SetScoreboardData(0,0,Format("%d",cnt),cnt);
 SetScoreboardData(1,0,Format("%d",cnt2),cnt2);
 
 if(!GetEffect("AdjustGoalTime"))
 {
  AddEffect("AdjustGoalTime",0,1,35,0,0);
  SetScoreboardData(2,0,Format("%.2d:%.2d",START_TIME/60,START_TIME%60),-1);
 }
 
 //GameCall("CheckGoal");
}

global func FxAdjustGoalTimeStart(pTarget,iEffectNumber)
{
 EffectVar(0,pTarget,iEffectNumber)=0;
}

global func FxAdjustGoalTimeTimer(pTarget,iEffectNumber,iEffectTime)
{
 ++EffectVar(0,pTarget,iEffectNumber);
 EffectCall(pTarget,iEffectNumber,"Check");
 return(1);
}

global func FxAdjustGoalTimeCheck(pTarget,iEffectNumber)
{
 var timeGone=EffectVar(0,pTarget,iEffectNumber);
 var timeLeft=START_TIME - timeGone;
 if(timeLeft < 0)timeLeft=0;
 
 var timeString=Format("%.2d:%.2d",timeLeft/60,timeLeft%60);
 
 SetScoreboardData(2,0,timeString,-1);
 GameCall("CheckGoal",timeLeft);
 return(1);
}

func CheckGoal(int timeLeft)
{
 if(ObjectCount2(Find_ID(GMSN)) <= (18-STONES_TO_STEAL))
 {
  FindObject2(Find_ID(SCRG))->Fulfill();
  
  for(var cnt=0;cnt<GetPlayerCount();cnt++)
  {
   var plr=GetPlayerByIndex(cnt);
   if(GetPlayerTeam(plr) == 2)continue;
   EliminatePlayer(plr);
  }
 
  Message("$GoalReached$");
  return(1);
 }
 
 if(timeLeft <= 0)
 {
  FindObject2(Find_ID(SCRG))->Fulfill();
  
  for(var cnt=0;cnt<GetPlayerCount();cnt++)
  {
   var plr=GetPlayerByIndex(cnt);
   if(GetPlayerTeam(plr) == 1)continue;
   EliminatePlayer(plr);
  }
 
  Message("$GoalFailed$");
  return(1);
 }
 
 return(1);
}

func IncinerateTorches()
{
 for(var obj in FindObjects(Find_Or(Find_ID(_TOR),Find_ID(TORC)),Find_Action("Burning")))
 {
  //Vertexaktualisierung
  obj->SetR(1);
  
  var light=CreateObject(LGSH,0,0,-1);
  light->AttachTo(obj);
  light->SetSize(500+Random(100));
  light->SetLightColor(RGBa(200+Random(55),200+Random(30),200,90));
  light->AddFlicker();
  light->SetR(Random(360));
  
  SetPlrViewRange(10,light);
 }
}

protected func InitializePlayer(int player, int tx, int ty, object pBase, int iTeam)
{
  // Ist es ein Scriptspieler?
  if (GetPlayerType(player) == C4PT_Script)
  {
    if(iTeam == 2)
    {
      Message("Error: AI-Players can't play the assassins!");
      EliminatePlayer(player);
      return();
    }
    GetHiRank(player)->Redefine(_GRD);
    GetHiRank(player)->SetGraphics(0, 0, KNIG);
  }

 //Kriegsnebel an!
 SetFoW(1,player);
 
 if(iTeam == 1)Log("$JoinGuards$",GetTaggedPlayerName(player));
 else
 if(iTeam == 2)Log("$JoinAssassin$",GetTaggedPlayerName(player));
 
 //RemoveObject(FindObject2(Find_OCF(OCF_CrewMember()),Find_Owner(player)));
 JoinPlayer(player);
 return(1);
}

protected func RelaunchPlayer(iPlr)
{
 if(!GetPlayerName(iPlr))return();
 var sTeam="$TeamGuard$";
 if(GetPlayerTeam(iPlr) == 2)sTeam="$TeamAssassin$";
 Log("$Relaunch$",GetTaggedPlayerName(iPlr),sTeam);
 JoinPlayer(iPlr);
}

global func GetPlayerTeam(iPlr)
{
  if (GetPlayerType(iPlr) == C4PT_Script) return(1);
  return(_inherited(iPlr));
}

protected func JoinPlayer(iPlr)
{
 if(ITEM_TRANS_LOCKED)return();
 if(!GetCrewCount(iPlr))
 {
  var clnk=CreateObject(TeamClonkID(GetPlayerTeam(iPlr), GetPlayerType(iPlr) == C4PT_Script),10,10,iPlr);
  MakeCrewMember(clnk,iPlr);
 }
 
 for(var crew in FindObjects(Find_Owner(iPlr),Find_OCF(OCF_Alive())))
 {
  var x,y;

  GetRelaunchPos(GetPlayerTeam(iPlr),x,y);
  if(GetPlayerTeam(iPlr) == 1 && Global(iPlr))
  {
   var rel=CreateObject(_RLC,AbsX(x),AbsY(y),iPlr);
   Enter(rel,crew);
  }
  else
   SetPosition(x,y,crew); 
   
  Global(iPlr)=true;
  
  DoEnergy(100,crew);
  DoMagicEnergy(20,crew);
  
  SetPlrViewRange(350,crew);
   // Ist es ein Scriptspieler? dann KI
  if (GetPlayerType(iPlr) == C4PT_Script)
    crew->SetAI();
 }
 
 var helper;
 if(helper=FindObject2(Find_ID(_HLP),Find_Owner(iPlr)))
  {//crew->GrabContents(helper);
   for(var cnt=ContentsCount(0,helper);cnt>=0;cnt--)
   {
    //if(!Collect(Contents(cnt,helper),crew))continue;
    if(!Contents(cnt,helper))continue;
    Enter(crew,Contents(cnt,helper));
   }
  }
 
 if(!FindContents(FLAG,crew))
 CreateContents(FLAG,crew);
 
 
 SelectCrew(iPlr,crew,1);
 SetPlrView(iPlr,crew);
}

func TeamClonkID(iTeam, iAI)
{
 if(iTeam == 1) { if(iAI) return(_GRD); return(KNIG); }
 if(iTeam == 2)return(ASAS);
}

func GetRelaunchPos(iTeam,&x,&y)
{
 if(iTeam == 1)
 {
  var rand=Random(3);
  x=aKNIGX[rand];
  y=aKNIGY[rand];
 }
 else
 if(iTeam == 2)
 {
  var rand=Random(6);
  x=aASASX[rand];
  y=aASASY[rand];
 }
}




