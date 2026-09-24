/*-- RdW-Intro --*/

#strict

static g_fDonotspeak,g_pBoss,g_pPlayer,g_aAngles,g_AnglesDone;

static const dayhelligkeit=30;

func Initialize()
{
  SetGamma(RGB(0,0,0), RGB(48,48,68), RGB(255,255,255));
  g_pBoss=CreateObject(CCLK,680,290);
  g_pBoss->SetColorDw(RGB(255,0,0));
  g_pBoss->SetName("Boss");
  LocalizeNames();
  ScriptGo(1);
  return(1);
}

// Objects.txt names these in German, and the engine reads it without a
// string table: they get the names of the player's language here
func LocalizeNames()
{
  for(var pObj in FindObjects(Find_Or(Find_ID(CCLK),Find_ID(FISH),Find_ID(GIDL))))
  {
    if(GetName(pObj) eq "Koch") SetName("$NameCook$",pObj);
    if(GetName(pObj) eq "Küchenhilfe") SetName("$NameKitchenHand$",pObj);
    if(GetName(pObj) eq "Hodur|der grossartige Vorzeitmagier") SetName("$NameHodur$",pObj);
    if(GetName(pObj) eq "Kristallstatue") SetName("$NameStatue$",pObj);
  }
  return(1);
}

func InitializePlayer(iPlr)
{
  SetFoW(1,iPlr);
  ColorMessage("$Dlg01$",GetCursor(iPlr));
  SetCommand(g_pPlayer,"MoveTo",0,610,320);
  return(1);
}
  
func IsNight(){return true;}  
  
func Script1()
{
  for(var musicobj in FindObjects(Find_ID(SMUS)))
    RemoveObject(musicobj);
  Music("@Intro",1);
} 

func Script6()
{
  if(!(g_pPlayer=FindObject2(Find_ID(CCLK),Find_OCF(OCF_CrewMember),Find_Exclude(g_pBoss),Find_Owner(0),Find_Distance(100,GetX(g_pBoss),GetY(g_pBoss)))))
    goto(5);
}

func Script32()
{
  SetCrewEnabled(false,g_pPlayer);
  Sound("Ding");
  ColorMessage("$Dlg02$",g_pPlayer);
  SetCommand(g_pPlayer,"MoveTo",0,630,300);
  g_pPlayer->SetPhysical("Walk",g_pPlayer->GetPhysical("Walk")/2,PHYS_Temporary);
  LookTo(g_pBoss,g_pPlayer);
}

func Script44()
{
  LookTo(g_pPlayer,g_pBoss);
  ColorMessage("$Dlg03$",g_pBoss);
  g_pPlayer->SetPhysical("Walk",g_pPlayer->GetPhysical("Walk")/2,PHYS_Temporary);
}

func Script64()
{
  ColorMessage("$Dlg04$",g_pPlayer);
  // Geschwindigkeit reseten
  g_pPlayer->SetPhysical("Walk",g_pPlayer->GetPhysical("Walk"),1);
  SetCommand(g_pPlayer,"None");
}

func Script80()
{
  ColorMessage("$Dlg05$",g_pBoss);
}

func Script96()
{
  ColorMessage("$Dlg06$",g_pPlayer);
}

func Script112()
{
  g_fDonotspeak=true;
  SetGamma(RGB(255,255,255), RGB(127,127,127), RGB(64,64,64),7);
  Schedule("SetGamma(RGB(200,200,200), RGB(127,127,127), RGB(80,80,80),7)",2);
  Schedule("SetGamma(RGB(127,127,127), RGB(127,127,127), RGB(127,127,127),7)",3);
  Schedule("SetGamma(RGB(32,32,32), RGB(127,127,127), RGB(180,180,180),7)",4);
  Schedule("SetGamma(RGB(16,16,16), RGB(127,127,127), RGB(235,235,235),7)",5);
  Schedule("ResetGamma(7)",6);
  Sound("Teleport");
  g_aAngles=[CreateObject(_Y8C,970,-10,-1),CreateObject(_Y8C,1000,-10,-1)];
  for(var angle in g_aAngles)
  {
    angle->Activate(g_pBoss);
    SetCommand(angle,"MoveTo",0,940+RandomX(-50,50),70);
    angle->SetName(["¿¶þ÷ñð¸±¥¤","¸±¥¤¿¶þ÷ñð","ñð¸±¶þ¥¤¿÷"][Random(4)]);
  }
  g_aAngles[0]->SetColorDw(RGB(255,255,0));
  g_aAngles[1]->SetColorDw(RGB(0,255,255));
}

func Script120()
{
  ColorMessage("$Dlg07$",g_aAngles[0]);
}

func Script128()
{
  ColorMessage("¿O_o?",g_pPlayer);
  ColorMessage("?o_O¿",g_pBoss);
}

func Script144()
{
  ColorMessage("$Dlg08$",g_aAngles[0]);
  ColorMessage("$Dlg09$",g_aAngles[1]);
}

func Script160()
{
  ColorMessage("$Dlg10$",g_aAngles[0]);
  ColorMessage("$Dlg11$",g_aAngles[1]);
}

func Script176()
{
  ColorMessage("$Dlg12$",g_aAngles[0]);
  ColorMessage("$Dlg13$",g_aAngles[1]);
}

func Script192()
{
  ColorMessage("$Dlg14$",g_aAngles[0]);
  ColorMessage("$Dlg15$",g_aAngles[1]);
}

func Script208()
{
  ColorMessage("$Dlg16$",g_pBoss);
  ColorMessage("!",g_pPlayer);
}

func Script224()
{
  ColorMessage("$Dlg17$",g_aAngles[0]);
  ColorMessage("$Dlg15$",g_aAngles[1]);
}

func Script240()
{
  ColorMessage("$Dlg18$",g_pBoss);
  ColorMessage("?",g_pPlayer);
}

func Script256()
{
  ColorMessage("$Dlg19$",g_aAngles[0]);
  ColorMessage("$Dlg20$",g_aAngles[1]);
}

func Script280()
{
  ColorMessage("$Dlg21$",g_aAngles[0]);
}

func Script295()
{
  ColorMessage("$Dlg22$",g_pBoss);
}

func Script296()
{
  ColorMessage("$Dlg23$",g_pBoss);
}

func Script297()
{
  ColorMessage("$Dlg22$",g_pBoss);
}

func Script298()
{
  ColorMessage("$Dlg23$",g_pBoss);
}

func Script299()
{
  ColorMessage("$Dlg22$",g_pBoss);
}

func Script303()
{
  ColorMessage("$Dlg23$",g_pBoss);
}

func Script306()
{
  Message("",g_pBoss);
  ColorMessage("$Dlg24$",g_aAngles[0]);
  ColorMessage("$Dlg25$",g_aAngles[1]);
  for(var angle in g_aAngles)
    SetCommand(angle,"MoveTo",0,970+RandomX(-50,50),0);
}

func Script320()
{
  ColorMessage("$Dlg26$",g_pBoss);
  SetGamma(RGB(255,255,255), RGB(127,127,127), RGB(64,64,64),7);
  Schedule("SetGamma(RGB(127,127,127), RGB(127,127,127), RGB(127,127,127),7)",2);
  Schedule("SetGamma(RGB(16,16,16), RGB(127,127,127), RGB(235,235,235),7)",3);
  Schedule("ResetGamma(7)",4);
  Sound("Teleport");
  for(var angle in g_aAngles)
    RemoveObject(angle);
}

func Script336()
{
  ColorMessage("@...",g_pBoss);
  ColorMessage("@...",g_pPlayer);
}

func Script360()
{
  ColorMessage("",g_pBoss);
  ColorMessage("$Dlg27$",g_pPlayer);
}

func Script380()
{
  ColorMessage("$Dlg28$",g_pBoss);
}

func Script400()
{
  ColorMessage("$Dlg29$",g_pPlayer);
}

func Script420()
{
  ColorMessage("$Dlg30$",g_pPlayer);
}


func Script440()
{
  ColorMessage("$Dlg31$",g_pBoss);
}

func Script450()
{
  ColorMessage("$Dlg32$",g_pPlayer);
}

func Script500()
{
  g_fDonotspeak=false;
  SetCrewEnabled(true,g_pPlayer);
  GameOver();
}

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
  if(iChar>255)// Höchsten 255
    return("");
  iChar-=32;// Zeichen die nur spezielle Textverarbeitungsprogramme anzeigen können
  if(iChar<0)
    return("");
  var aChars=[" ","!","\"","#","$","%","&","'","(",")","*","+",",","-",".","/",
					   "0","1","2","3","4","5","6","7","8","9",
					   ":",";","<","=",">","?","@",
					   "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
					   "[","\\","]"," ","_","`",
					   "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
					   "{","|","}","~","","€","","‚","ƒ","„","…","†","‡","ˆ","‰","Š","‹","Œ","","Ž","","","‘","’","“","”","•","–",
					   "—","˜","™","‘","›","œ","","ž","Ÿ"," ","¡","¢","£","¤","¥","¦","§","¨","©","ª","«","¬","­","®","¯","°","±","²",
					   "³","´","µ","¶","·","¸","¹","º","»","¼","½","¾","¿","À","Á","Â","Ã","Ä","Å","Æ","Ç","È","É","Ê","Ë","Ì","Í","Î",
					   "Ï","Ð","Ñ","Ò","Ó","Ô","Õ","Ö","×","Ø","Ù","Ú","Û","Ü","Ý","Þ","ß","à","á","â","ã","ä","å","æ","ç","è","é",
					   "ê","ë","ì","í","î","ï","ð","ñ","ò","ó","ô","õ","ö","÷","ø","ù","ú","û","ü","ý","þ"];
  return(aChars[iChar]);
}

global func LookTo(pObj,pThis)
{
  SetDirTo(pObj, pThis);
  Schedule(Format("LookTo(Object(%d))",ObjectNumber(pObj)),10,0,pThis);
  return(1);
}

// Richtung anpassen, so dass das Objekt zum anderen Objekt schaut
global func SetDirTo(object pTarget, object pThis)
{
  if (GetX(pTarget) > GetX(pThis))
    SetDir(DIR_Right, pThis);
  else if (GetX(pTarget) < GetX(pThis))
    SetDir(DIR_Left, pThis);
}
  
global func SetColor(iColor,pObj)
{
  iColor = BoundBy(iColor,0,12);
  var Clr;
  Clr=HSL(BoundBy(20*iColor+Random(19),0,255),255,127);
  SetColorDw(Clr,pObj);
}

public func RejectBuildUp()
{
  return 1;
}
