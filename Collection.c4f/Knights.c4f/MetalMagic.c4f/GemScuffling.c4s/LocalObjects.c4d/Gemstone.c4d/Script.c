/*-- Edelstein --*/

#strict

local iX,iY,sGraphic;
local iTeam;

func CarryLimit()
{
 return 1;
}

func Hit()
{
 Sound("Crystal*");
}

func Initialize()
{
 if(!sGraphic)
 {
  var rand=Random(3);
  if(!rand)sGraphic="";
  if(!--rand)sGraphic="2";
  if(!--rand)sGraphic="3";
  SetGraphics(sGraphic,this());
 }
 
 if(!GetEffect("RandomSparkle",this()))
  AddEffect("RandomSparkle",this(),20,10+Random(30),this());
}

func AdjustGraphics(sNew)
{
 sGraphic=sNew;
 SetGraphics(sGraphic,this());
}

func FxRandomSparkleTimer(pTarget,iEffectNumber)
{
 if(!Random(3))pTarget->Timer();
}

public func InitHere()
{
 iX=GetX();
 iY=GetY();
 Return();
}

func Return()
{
 Exit();
 SetAction("Float");
 SetPosition(iX,iY);
 CreateParticle("MaterialSpark",0,0,0,0,9*(10+Random(60)),GetRightColor(100),this());
 
 if(!GetEffect("DoFloating",this()))
  AddEffect("DoFloating",this(),20,2,this());
 EffectVar(0,this(),GetEffect("DoFloating",this()))=0;
 
 DoScoreboardRefresh();
}

func FxDoFloatingStart(pTarget,iEffectNumber,iTemp)
{
 if(iTemp)return();
 EffectVar(0,pTarget,iEffectNumber)=0;
 return(1);
}

func FxDoFloatingTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(GetAction(pTarget) ne "Float")
  {EffectVar(0,pTarget,iEffectNumber)=0; return();}
 else
  EffectVar(0,pTarget,iEffectNumber)+=2;
 
 var iProc=EffectVar(0,pTarget,iEffectNumber);
 
 SetXDir();
 SetYDir(Cos(iProc*5,5));
 return(1);
}

public func AtHome()
{
 if(Distance(GetX(),GetY(),iX,iY) < 10)return(1);
 return;
}

func Timer()
{
 if(!Random(3))
  CreateParticle("MaterialSpark",0,0,0,0,5*(1+Random(20)),GetRightColor(),this());
}

public func RejectEntrance(pObj)
{
 if(GetOCF(pObj) & OCF_Alive() && !(GetOCF(pObj) & OCF_CrewMember()))return(1);
 
 if(GetPlayerTeam(GetOwner(pObj)) == iTeam)
  if(Distance(GetX(),GetY(),iX,iY) < 10)return(1);
 
 if(!(GetOCF(pObj) & OCF_CrewMember()))
  LeaveTimer(pObj);
 
 return();
}

func Departure()
{
 DoScoreboardRefresh();
}

public func Entrance(pObj)
{
 DoScoreboardRefresh();
 ShowScoreboard();
 
 SetAction("Idle");
 if(!GetPlayerName(GetOwner(pObj)))return(1);
 if(GetPlayerTeam(GetOwner(pObj)) != iTeam)
 {
  Sound("MagicElementEarth",1);
  return(1);
 }
 if(GetOCF(pObj) & OCF_Alive)
  return(Return());
}

func ShowScoreboard()
{
 for(var i=0;i<GetPlayerCount();i++)
 {
  var iPlr=GetPlayerByIndex(i);
  DoScoreboardShow(1,iPlr);
 }
}

func Destruction()
{
 GetID()->DoScoreboardRefresh();
 Sound("Trumpet");
 ShowScoreboard();
}

func LeaveTimer(pObj)
{
 AddEffect("LeaveSoon",this(),20,36*5,this(),0,pObj);
}

func FxLeaveSoonStart(pTarget,iEffectNumber,iTemp,val1)
{
 if(iTemp)return();
 EffectVar(0,pTarget,iEffectNumber)=val1;
 return(1);
} 

func FxLeaveSoonTimer(pTarget,iEffectNumber)
{
 if(Contained(pTarget) == EffectVar(0,pTarget,iEffectNumber))
  SetCommand(pTarget,"Exit");
 return(-1);
}

func GetRightColor(iAlpha)
{
 if(sGraphic == "")return(RGBa(10,10,255,iAlpha));
 if(sGraphic == "2")return(RGBa(10,255,10,iAlpha));
 if(sGraphic == "3")return(RGBa(255,10,10,iAlpha));
}

func DoScoreboardRefresh()
{
 Schedule("GameCall(\"RefreshScoreboard\")",1,0);
}
