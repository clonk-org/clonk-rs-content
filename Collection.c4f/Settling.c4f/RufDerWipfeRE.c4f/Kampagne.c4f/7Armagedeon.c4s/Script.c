#strict

static g_pAngle,g_iFlair;

protected func Initialize()
{
  MoreFlair(1);
  var pGoal=FindObject2(Find_ID(SCRG));
  if(!pGoal) pGoal=CreateObject(SCRG,0,0,-1);
  pGoal->SetMessages("Die Rakete wurde gebaut! Auf zu den Sternen!", "Die Rakete muss noch gebaut werden...");
  return(1);
}

public func RocketBuild()
{
  var pGoal=FindObject2(Find_ID(SCRG));
  if(!pGoal) pGoal=CreateObject(SCRG,0,0,-1);
  pGoal->Fulfill();
  Schedule("GameCall(\"Outro\")",140);
  return(1);
}

public func ChooserFinished()
{
  ScriptGo(true);
}

public func Script10()
{
  g_pAngle=CreateObject(_Y8C,970,-10,-1);
  g_pAngle->Activate(GetCursor());
  g_pAngle->SetColorDw(RGB(255,255,0));
  g_pAngle->SetName(["øù˛˜Ò∏ë°§","∏ë°§øù˛˜Ò","Ò∏ëù˛°§ø˜"][Random(4)]);
}

public func Script60()
{
  ColorMessage("Seid gegr¸sst!",g_pAngle);
}

public func Script80()
{
  ColorMessage("Ich wollte euch nur nochmal daran errinern das die Abrissarbeiten f¸r diesen Planeten bald beginnen.",g_pAngle);
}

public func Script100()
{
  ColorMessage("Ihr solltet so schnell wie mˆglich von hier verschwinden...",g_pAngle);
}

public func Script120()
{
  ColorMessage("...falls euch eurer Leben lieb ist.",g_pAngle);
}

public func Script140()
{
  ColorMessage("Tsch¸ss.",g_pAngle);
  g_pAngle->Done();
  CreateObject(CLK_,0,0,-1)->Set(4200-GetDifficulty()*450,0,0,"RdW");
  ScriptGo(false);
}

public func TimeUp()
{
  AddEffect("IntPlanetDestruction",0,1,(3333-GetDifficulty()*333)/3);
  CreateObject(CLK_,0,0,-1)->Set(2100-GetDifficulty()*150,0,0,"RdW");
  Schedule("GameCall(\"MoreFlair\",1)",5,32);
}

public func MoreFlair(iVal)
{
  g_iFlair=BoundBy(g_iFlair+iVal,0,63);
  SetGamma(RGB(g_iFlair/10,g_iFlair/30,0),RGB(128,128-g_iFlair*3/2,128-g_iFlair*2),RGB(255,255-g_iFlair/3,255-g_iFlair/2));
  SetSkyAdjust(RGBa(255,255-g_iFlair*2,255-g_iFlair*3,g_iFlair),RGB(64+g_iFlair*3,80+g_iFlair*2,256-g_iFlair*4));
}

public func TimerPosition(pTimer)
{
  Local(0,pTimer)=250;
  Local(1,pTimer)=200;
}

global func FxIntPlanetDestructionTimer()
{
  var i=Random(8);
  {
    Message("<c ff0000>Warnung:</c>|!!!{{METO}}-Sturm aktiviert!!!");
    Schedule("Schedule(\"CreateObject(METO,Random(LandscapeWidth())),0,-1)->SetXDir(RandomX(3,-3),3,30)\",20,5);",500);
  }
  if(!i--)
  {
    Message("<c ff0000>Warnung:</c>|!!!{{FXL1}}-Kanone gestartet!!!");
    Schedule("Schedule(\"LaunchLightning(Random(LandscapeWidth()),0,RandomX(-5,5),20,20,20)\",10,40);",500);
  }
  if(!i--)
  {
    Message("<c ff0000>Warnung:</c>|!!!{{FXV1}}-Feld aktiviert!!!");
    Schedule("Schedule(\"LaunchVolcano(Random(LandscapeWidth()))\",30,10);",500);
  }
}

func LocalPlaceName(para)
{
  return("Komisches Unbekanntes Ding");
}

func Outro()
{
  GainMissionAccess("SoBeRdW7");
  ScriptGo(true);
  Schedule("CreateObject(METO,Random(LandscapeWidth())),0,-1)->SetXDir(RandomX(3,-3),3,30)",50,30);
  Music("@Outro",true);
  goto(300);
  // Von Schwarz einfaden
  AddEffect("IntFadeIn",0,10,1);
  var pSpaceShip=FindObject2(Find_ID(RAKT),Find_OCF(OCF_Fullcon));
  
  // Alle Clonks der Spieler deaktivieren und Sicht auf das Raumschiff setzen
  var j,i = 0;
  for (var plrnum = GetPlayerCount (); plrnum; i++)
    if (GetPlayerName (i))
    {
      plrnum--;
      SetFoW (0, i);
      j=GetCrewCount(0); while (j--) SetCrewEnabled(0,GetCrew(i, j));
      SetPlrView(i,pSpaceShip);
    }
    
  // Clonks erschaffen die in das Raumschiff reingehen
  var pClonk,fContinue;
  for(i = 1; i <= 100; i++)
  {
    pClonk=CreateObject(CLNK,GetX(pSpaceShip)+RandomX(-700,700),GetDefBottom(pSpaceShip)+RandomX(-100,100),-1);
    while(!Stuck(pClonk))
    {
      pClonk->SetPosition(GetX(pClonk),GetY(pClonk)+1);
      if(!Random(200))
      {
        fContinue=true;
        break;
      }
    }
    while(Stuck(pClonk))
    {
      pClonk->SetPosition(GetX(pClonk),GetY(pClonk)-1);
      if(!Random(200))
      {
        fContinue=true;
        break;
      }
    }
    if(fContinue)
    {
      RemoveObject(pClonk);
      fContinue=false;
      continue;
    }
    
    SetPhysical("CanScale",1,2,pClonk);
    SetPhysical("CanHangle",1,2,pClonk);

    if(Random(5))
      pClonk->SetColorDw(HSL(Random(256),255,127));
    else if(Random(2))
      pClonk->SetColorDw(HSL(Random(256),255,Random(128)));
    else if(Random(3))
      pClonk->SetColorDw(HSL(0,0,Random(256)));
    else
      pClonk->SetColorDw(HSL(Random(256),255,Random(256)));
  }
  
  for(pClonk in FindObjects(Find_OCF(OCF_CrewMember)))
    SetCommand(pClonk,"Enter",pSpaceShip);
  return(1);
}

func Script350()
{
  var pSpaceShip=FindObject2(Find_ID(RAKT),Find_OCF(OCF_Fullcon));
  pSpaceShip->Start();
}

func Script450()
{
  GameOver();
}

global func FxIntFadeInTimer(tar,nr,iTime)
{
  if(iTime>500) return -1;
  SetGamma(RGB(0,0,0),RGB(iTime/4,iTime/4,iTime/4),RGB(iTime/2,iTime/2,iTime/2),7);
  SetMatAdjust(RGB(iTime/2,iTime/2,iTime/2));
  //SetSkyAdjust(RGB(iTime/2,iTime/2,iTime/2));
}

global func FxIntFadeInStop(tar,n)
{
  SetMatAdjust(RGB(255,255,255));
  //SetSkyAdjust(RGB(255,255,255));
  ResetGamma(7);
}

func InitializePlayer(iPlr)
{
  return(1);
}

public func IsCampaignMission()
{
  return true;
}


// Gibt das forcierte Zeitalter des Szenarios zur¸ck (f¸r den Regelw‰hler)
func ForcedTechLevel()
{
  return RdW_TL_Modern;
}

public func RejectBuildUp(pAngle)
{
  return pAngle==g_pAngle;
}

//

global func ColorMessage(strColorMessage,pObj,iAlpha)
{
  var at;
  if(strColorMessage eq "")
    return(Message(strColorMessage,pObj,Par(3),Par(4),Par(5),Par(6),Par(7),Par(8),Par(9)));
  if(GetChar(strColorMessage)==GetChar("@"))
  {
    at=1;
    strColorMessage=DeleteChar(strColorMessage);
  }
  var dwColor=GetColorDw(pObj);if(!dwColor)dwColor=RGB(127,127,127);
  if(iAlpha)
    dwColor=SetRGBaValue(dwColor,iAlpha);
  var sSpeaker=GetName(pObj);
  strColorMessage=Format("<c %x>%s:</c>|<c %x>%s</c>",RGBa(255,255,255,iAlpha),sSpeaker,dwColor, strColorMessage);
  if(at)
    strColorMessage=Format("@%s",strColorMessage);
  return(Message(strColorMessage,pObj,Par(3),Par(4),Par(5),Par(6),Par(7),Par(8),Par(9)));
}

global func DeleteChar(strText,iPos)
{
  var temp="";
  for(var i = 0; i <= GetLength(strText); i++)
  {
    if(i==iPos)continue;
    temp=Format("%s%s",temp,Int2Char(GetChar(strText,i)));
  }
  return(temp);
}

global func Int2Char(iChar)
{
  if(iChar>255)//Hˆchsten 255
    return("");
  iChar-=32;//Zeichen die nur spezielle Textverarbeitungsprogramme anzeigen kˆnnen
  if(iChar<0)
    return("");
  var aChars=[" ","!","\"","#","$","%","&","'","(",")","*","+",",","-",".","/",
					   "0","1","2","3","4","5","6","7","8","9",
					   ":",";","<","=",">","?","@",
					   "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
					   "[","\\","]"," ","_","`",
					   "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
					   "{","|","}","~","","Ä","Å","Ç","É","Ñ","Ö","Ü","á","à","â","©","ã","¶","´","Æ","¨","ê","ë","í","ì","î","ï","ñ",
					   "ó","ò","ô","ë","õ","∂","ª","æ","º"," ","∑","¢","£","§","°","ä","ß","®","å","™","ç","é","≠","è","Ø","∞","ë","≤",
					   "≥","¥","ú","ù","û","∏","±","∫","ü","•","Ω","µ","ø","¿","¡","¬","√","ƒ","≈","∆","«","»","…"," ","À","Ã","Õ","Œ",
					   "œ","–","—","“","”","‘","’","÷","◊","ÿ","Ÿ","⁄","€","‹","›","ﬁ","ﬂ","‡","·","‚","„","‰","Â","Ê","Á","Ë","È",
					   "Í","Î","Ï","Ì","Ó","Ô","","Ò","Ú","Û","Ù","ı","ˆ","˜","¯","˘","˙","˚","¸","˝","˛"];
  return(aChars[iChar]);
}
