#strict

protected func Initialize()
{
  // Parallax
  SetSkyParallax(0, 15, 10, 0, 0, 0, 0);
  // Besonderheiten
  CreateObject(_Z9A,10,250,-1)->Set(1);
  CreateObject(_Z9A,1430,10,-1)->Set(2);
  CreateObject(_Z9A,2050,450,-1)->Set(3);
  // Wache
  SetLocal(0,1,CreateObject(JCLK,30,257,-1));
  // Stamm
  var house;
  house=CreateObject(_A1A,1960,467,-1); Incinerate(house); Extinguish(house);
  house=CreateObject(_A1A,2050,453,-1); Incinerate(house); Extinguish(house);
  Kill(CreateObject(CLNK,2000,440,-1));
  ScriptGo(1);
  // Spielziel
  var pGoal=FindObject2(Find_ID(CROB));
  if(!pGoal) pGoal=CreateObject(CROB,0,0,-1);
  pGoal->AddType(_K1A, 1);
  pGoal->AddType(_F1A, 1);
  pGoal->AddType(_N2A, 1);
  pGoal->AddType(_M1A, 1);
  pGoal->SetMissionAccess("SoBeRdW2");
}

func Script1()
{
  GetTimeObject()->SetTime(65);
  ScriptGo(0);
}

public func Wiegethname(para)
{
  if(para==1) return("$TxtPose1$");
  if(para==2) return("$TxtPose2$");
  if(para==3) return("$TxtPose3$");
  return("$TxtUnDef$");
}

public func InitializePlayer(iPlayer)
{
  SetPosition(40,250,GetCrew(iPlayer));
  Golden(iPlayer);
  return true;
}
  
public func IsCampaignMission()
{
return true;
}

global func GetTimeObject()
{
var obj;
if(obj=FindObject(TIME))
 return obj;
return CreateObject(TIME,0,0,-1);
}
