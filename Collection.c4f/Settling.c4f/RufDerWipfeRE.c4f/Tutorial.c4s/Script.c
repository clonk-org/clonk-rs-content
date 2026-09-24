/*-- RdW-Lernrunde --*/

#strict

static g_pLastArrow,g_pGoal;

func Initialize()
{
  DigFreeRect(145,260,55,10);
  SetSkyParallax(0,13,11,0,0,0,0);
  ScriptGo(1);
  g_pGoal=FindObject2(Find_ID(SCRG));
  if(!g_pGoal) g_pGoal=CreateObject(SCRG,5,5,-1);
}

func InitializePlayer(iPlr)
{
  SetPosition(320,-20,GetHiRank(iPlr));
}

func Script4()
{
  Sound("Boing");
  dayspeed=0;
  return(1);
}

func Script10()
{
  Message("$Tut01$",0,GetTaggedPlayerName());
  return(1);
}

func Script30()
{
  Message("$Tut02$");
  return(1);
}

func Script50()
{
  Message("$Tut03$");
  SetArrow(100,250,true,true);
  return(1);
}

func Script80()
{
  CreateContents(BANN,GetHiRank());
  SetArrow(64,-64,true);
  Message("$Tut04$");
  return(1);
}

func Script100()
{
  RemoveArrow();
  Sound("Ding");
  g_pGoal->SetMessages("", "$Tut05$");
  Message("$Tut06$");
  return(1);
}

func Script105()
{
  if(ObjectCount2(Find_ID(_I1A),Find_OCF(OCF_Fullcon)))
    return goto(120);
  if(!ObjectCount2(Find_ID(HCKT)))
    return goto(102);
  Message("$Tut07$");
}

func Script120()
{
  if(!ObjectCount2(Find_ID(_I1A),Find_OCF(OCF_Fullcon)))
    return goto(115);
  Message("");
  return(1);
}

func Script125()
{
  Sound("Applause");
  return(1);
}

func Script140()
{
  Message("$Tut08$");
  GetHiRank()->CheckFood(-1300);
  return(1);
}

func Script150()
{
  Message("$Tut09$");
  Sound("Ding");
  g_pGoal->SetMessages("", "$Tut10$");
  var pBar=FindObject(_I1A);
  SetArrow(GetX(pBar),GetY(pBar)-40,false);
  SetPlrShowCommand(0,3);
  return(1);
}

func Script154()
{
  if(GetID(Contents(0,GetHiRank()))!=BRED)
     return goto(151);
  SetPlrShowCommand(0,0);
  Message("");
}

func Script156()
{
  if(Contained(GetHiRank()))
    return goto(155);
  Message("$Tut11$");
}

func Script158()
{
  if(Contained(GetHiRank()))
    return goto(155);
  RemoveArrow();
  Message("$Tut12$",0,GetPlrControlName(0, CON_Dig));
  SetPlrShowCommand(0,128+6);
}

func Script165()
{
  if(GetHiRank()->CheckFood()<50)
    return goto(160);
  Sound("Applause");
  Message("");
  SetPlrShowCommand(0,0);
  return(1);
}

func Script170()
{
  Message("$Tut13$");
  dayspeed=40;
}

func Script175()
{
  if(IsDay())
   return goto(173);
  dayspeed=0;
  if(GetSystemTime(4)>21||GetSystemTime(4)<7)
    Message("$Tut14$");
  else
    Message("$Tut15$");
}

func Script185()
{
  Message("$Tut16$");
  GetHiRank()->CheckSleep(-800);
}

func Script195()
{
  Message("$Tut17$");
  g_pGoal->SetMessages("", "$Tut18$");
  var pHut=FindObject(_A1A);
  SetArrow(GetX(pHut),GetY(pHut)-40,false);
}

func Script198()
{
  if(GetID(Contained(GetHiRank()))!=_A1A)
    return goto(197);
  SetPlrShowCommand(0,3);
  Message("$Tut19$",0,GetPlrControlName(0, CON_Up));
  return(1);
}

func Script200()
{
  if(GetID(Contained(GetHiRank()))!=_A1A)
    return goto(197);
  if(GetAction(GetHiRank())ne"Sleep")
    return goto(199);
  SetPlrShowCommand(0,0);
  RemoveArrow();
  Message("$Tut20$");
  return(1);
}

func Script202()
{
  if(GetHiRank()->CheckSleep()<85)
    return goto(201);
  Message("$Tut21$");
  return(1);
}

func Script210()
{
  Message("$Tut22$");
  return(1);
}

func Script220()
{
  Message("$Tut23$");
  return(1);
}

func Script222()
{
  if(Contained(GetHiRank()))
    return goto(221);
  Sound("Applause");
  Message("");
  Message("$Tut24$",GetHiRank());
  return(1);
}

func Script230()
{
  Message("$Tut25$");
  return(1);
}

func Script240()
{
  Sound("Ding");
  Message("$Tut26$");
  g_pGoal->SetMessages("", "$Tut27$");
  return(1);
}

func Script250()
{
  if(ObjectCount2(Find_ID(TOR2),Find_OCF(OCF_Fullcon))<2)
    return goto(245);
  Sound("Applause");
  Message("");
  dayspeed=3;
  return(1);
}

func Script260()
{
  Message("$Tut28$");
  return(1);
}

func Script270()
{
  Sound("Ding");
  Message("$Tut29$");
  g_pGoal->SetMessages("", "$Tut30$");
  ____->Researched();
  return(1);
}

func Script275()
{
  if(!ObjectCount2(Find_ID(_B1A)))
    return goto(273);
  Message("$Tut31$");
}

func Script277()
{
  if(GetCon(FindObject2(Find_ID(_B1A)))<35)
    return goto(276);
  Message("");
}

func Script280()
{
  if(!ObjectCount2(Find_ID(_B1A),Find_OCF(OCF_Fullcon)))
    return goto(278);
  Sound("Applause");
  Message("");
  return(1);
}

func Script290()
{
  Message("$Tut32$");
  return(1);
}

func Script300()
{
  Sound("Ding");
  Message("$Tut33$");
  g_pGoal->SetMessages("", "$Tut34$");
  return(1);
}

func Script310()
{
  var pLab=FindObject(_B1A);
  if(ActIdle(pLab))
    return goto(305);
  Message("$Tut35$");
  return(1);
}

func Script320()
{
  if(!GetPlrKnowledge(0,SCAC))
    return goto(315);
  Message("$Tut36$");
  return(1);
}

func Script330()
{
  Message("$Tut37$",0,GetPlrControlName(0, CON_Dig));
  return(1);
}

func Script340()
{
  Sound("Ding");
  Message("$Tut38$");
  g_pGoal->SetMessages("$Tut39$", "$Tut40$");
  return(1);
}

func Script350()
{
  if(!ObjectCount2(Find_ID(_B2A),Find_OCF(OCF_Fullcon)))
    return goto(345);
  Sound("Applause");
  Message("");
  return(1);
}

func Script360()
{
  Message("$Tut41$");
  return(1);
}

func Script380()
{
  Message("$Tut42$");
  return(1);
}

func Script390()
{
  Message("$Tut43$");
  return(1);
}

func Script400()
{
  Sound("Ding");
  Message("$Tut44$");
  return(1);
}

func Script410()
{
  if(!ObjectCount2(Find_Or(Find_ID(_A2A),Find_ID(_A3A)),Find_OCF(OCF_Fullcon)))
    return goto(408);
  Sound("Applause");
  Message("");
  return(1);
}

func Script420()
{
  Message("$Tut45$");
  Sound("Ding");
  return(1);
}

func Script430()
{
  if(GetCrewCount()<3)
    return goto(428);
  Sound("Applause");
  Message("");
  goto(980);
  return(1);
}

func Script990()
{
  Sound("Applause");
  Message("");
  g_pGoal->Fulfill();
  return(1);
}

func Script1000()
{
  Sound("Applause");
  Sound("Ding");
  Message("$Tut46$");
  return(1);
}

func Script1010()
{
  Sound("Applause");
  return(1);
}

func Script1020()
{
  //GameOver();
  ScriptGo(false);
}
