/*-- Neues Script --*/

#strict

#include CLNK

local Mod;
local iKiff;
local Folg1,FolgTo;
local kiffen,Timer;

local specialding;

protected func ActivateEntrance(object pClonk){
if(!GetAlive(this())) return(0,Message("Der ist tot.",this()));
SetCommand(pClonk,"");
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0); 
if(Mod==0){
AddMenuItem("Wer oder was bist du denn?!","Arschloch",0,pClonk,0,pClonk,0);
AddMenuItem("Tag?","Neugirig",0,pClonk,0,pClonk,0);
AddMenuItem("Tach!","Neugirig",0,pClonk,0,pClonk,0);
}
if(Mod==1){
Neugirig(0,pClonk);
}
if(Mod==2){
Mes2(0,pClonk);
}
if(Mod==3){
Mes3(0,pClonk);
}
if(Mod==4){
Mes4(0,pClonk);
}
if(Mod==5){
//if(Folg1==0){
Mes5(0,pClonk);
 }
}
if(Mod==7){
if(Folg1==0){
Mes7(0,pClonk);
 }
}
if(Folg1==1){
AddMenuItem("Sind wir da?","",0,pClonk,0,pClonk,0);
AddMenuItem("Ja","NotFolg",0,pClonk,0,pClonk,0);
AddMenuItem("Nein","EndTalk",0,pClonk,0,pClonk,0);
}
if(Folg1==0) AddMenuItem("Tschöö","EndTalk",0,pClonk,0,pClonk,0);
return(true);
}
protected func Check(){
if(GetAlive(this())){
if(!Random(430)){
   if(!Random(5)){
   if(ContentsCount(CNKT) < 2) CreateContents(CNKT,0,1);
   if(ContentsCount(WOOD) < 3) CreateContents(WOOD,0,1);
   if(ContentsCount(METL) < 2) CreateContents(METL,0,1);
   if(ContentsCount(ROCK) < 3) CreateContents(ROCK,0,1);
   if(ContentsCount(FLNT) < 3) CreateContents(FLNT,0,1);
   }
   var idObject;
   var Typ;
   var i;
   while(Typ=GetDefinition(i++,C4D_Object())){
   if(GetDefCoreVal("Collectible","DefCore",Typ))
    if(!Random(i)) idObject= Typ;
    if(!idObject) return(Message("omg.",this()));
    }
   if(!FindContents(GetID(specialding))){ 
   if(ObjectCount (0, 0, 0, 0, 0, 0, 0, 0, this()) < 18)
   specialding = CreateContents(idObject);
     }
   
   if(GetID(specialding) == CNKT) RemoveObject(specialding);
   if(GetID(specialding) == WOOD) RemoveObject(specialding);
   if(GetID(specialding) == METL) RemoveObject(specialding);
   if(GetID(specialding) == ROCK) RemoveObject(specialding);
   if(GetID(specialding) == FLNT) RemoveObject(specialding);         
   if(GetID(specialding) == BCOR) RemoveObject(specialding);
   if(GetID(specialding) == RCOR) RemoveObject(specialding);
   if(GetID(specialding) == GCOR) RemoveObject(specialding); 
   if(!Random(5)) specialding = 0;
 }
SetPlrViewRange(0);
if(GBackLiquid(GetX(this()),GetY(this()))) SetComDir(COMD_Up);

if(!Random(860)){ 
kiffen = RandomX(1,3);
 }
if(kiffen==1){ 
Timer++;
if(Timer < 100){ 
CreateParticle("Smoke",0,0,0,-7,40,RGBa(255,255,255,64));
   }
if(Timer > 100 && Timer < 200){ 
   }
if(Timer > 200){ 
CreateParticle("Smoke",0,0,0,-7,60,RGBa(255,255,255,64));
   }
if(Timer > 300){ kiffen = 0; Timer = 0;      }     
 } 
}
if(kiffen==2){ 
if(Folg1==0) Timer++;
if(Timer == 2){ 
Message("WUI!WUI!",this());
SetAction("Jump");
SetYDir(-50);
Sound("Magic*",0,this());
   }
if(Timer > 2){ 
CastParticles("PSpark", 4, 40, 0, 0, 50, 70, RGB(0,0,255), RGB(0,155,255));    
}
  
  if(Timer == 12){
   Sound("Blast3",0,this());
   CreateParticle("BUSpark",0,0,0,0,230,RGB(0,0,255));
   for(var i; i < 360; i++){
    CreateParticle("PSpark",Cos(i,25),Sin(i,25),0,0,70,RGB(0,120,255));
   }
   kiffen = 0; Timer = 0;   
  } 
if(Timer > 20){ kiffen = 0; Timer = 0;  }
}
if(kiffen==3){ 
if(Folg1==0) Timer++;
if(Timer == 2){ 
SetAction("GetPunched"); 
kiffen = 0; Timer = 0;   
 }
if(Timer > 20){ kiffen = 0; Timer = 0;    }
}
 
 
if(Mod==0) SetName("???");
if(Mod==1) SetName("Novo, aus'm Urwald");
if(Folg1==1){
SetCommand(this(),"MoveTo",FolgTo);
if(!Random(130)) Message("Sind wir bald da?",this());
  }
 }
}
private func EndTalk(id doof, object pClonk){
CloseMenu(pClonk);
}
private func CloseAndGo(id doof, object pClonk){
Folg1 = 1;
FolgTo = pClonk;

CloseMenu(pClonk);
}

private func Neugirig(id doof, object pClonk){
Mod=1;
CreateMenu(MILT,pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("Agga Ugga mein Freund! Mir'se bin Novo und kommen aus Urwald, man.","",0,pClonk,0,pClonk,0);
AddMenuItem(Format("Tach! Ich heiße %s!",GetTaggedPlayerName(GetOwner(pClonk))),"Mes2",0,pClonk,0,pClonk,0);
}

private func Arschloch(id doof, object pClonk){
Mod=1;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("Ich Leute wie dich essen tue tief im Urwald! Ich sein Novo.","",0,pClonk,0,pClonk,0);
AddMenuItem(Format("Achso! Ich heiße %s!",GetTaggedPlayerName(GetOwner(pClonk))),"Mes2",0,pClonk,0,pClonk,0);
}

private func Mes2(id doof, object pClonk){
Mod=2;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("Wie zur Hölle kamst du hier her?","Mes3",0,pClonk,0,pClonk,0);
}

private func Mes3(id doof, object pClonk){
Mod=3;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("Ich nichts wissen von was. Ich rauchen gute Tüte und dann Iche war hier.","",0,pClonk,0,pClonk,0);
AddMenuItem("Ach so.","Mes4",0,pClonk,0,pClonk,0);
AddMenuItem("Was für eine gute Tüte.","Mesa4",0,pClonk,0,pClonk,0);
}
private func Mesa4(id doof, object pClonk){
iKiff = 1;
Mod=4;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("Also, bring'se mir eine Schwarzmuschel, Iche machen dir eine Tüte.","",0,pClonk,0,pClonk,0);
AddMenuItem("Super!","EndTalk",0,pClonk,0,pClonk,0);
}
private func Mes4(id doof, object pClonk){
Mod=5;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("Ich haben auch andere Ware.","",0,pClonk,0,pClonk,0);
AddMenuItem("Zeig mal her!","Handel",0,pClonk,0,pClonk,0);
AddMenuItem("Wenn das so ist...","EndTalk",0,pClonk,0,pClonk,0);
}

private func Mes5(id doof, object pClonk){
Mod=5;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("Hallu Fremder.","",0,pClonk,0,pClonk,0);
AddMenuItem("Ich will was kaufen.","Handel",0,pClonk,0,pClonk,0);
if(!Folg1) AddMenuItem("Komm mal mit.","CloseAndGo",0,pClonk,0,pClonk,0);
if(Folg1) AddMenuItem("Okay, wir sind da.","NotFolg",0,pClonk,0,pClonk,0);
if(iKiff) AddMenuItem("Machst du mir ein Joint?","MakeJoint",0,pClonk,0,pClonk,0);
if(!iKiff) AddMenuItem("Wie war das mit der Wundertüte?","Mesa4",0,pClonk,0,pClonk,0);
AddMenuItem("Ach nichts... (ENDE)","EndTalk",0,pClonk,0,pClonk,0);
}

private func Folg(id doof, object pClonk){
Folg1 = 1;
FolgTo = pClonk;
EndTalk(0,pClonk);
Message("Wolang denn?",this());
}
private func MakeJoint(id doof, object pClonk){
Mod=5;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
if(FindContents(BSHL,pClonk)) AddMenuItem("So Fertig, da hast du.","",0,pClonk,0,pClonk,0);
if(!FindContents(BSHL,pClonk)) AddMenuItem("Du haben keine Schwarzmuschel. Ich dir nur aus Schwarzmuschel machen kann Joint.","",0,pClonk,0,pClonk,0);
if(FindContents(BSHL,pClonk)){ RemoveObject(FindContents(BSHL,pClonk));
CreateContents(JOIN,pClonk,1); }
if(FindContents(JOIN,pClonk)) AddMenuItem("Danke Man!","EndTalk",0,pClonk,0,pClonk,0);
if(!FindContents(JOIN,pClonk)) AddMenuItem("Wenn das so ist...","EndTalk",0,pClonk,0,pClonk,0);
}
private func NotFolg(id doof, object pClonk){
Folg1 = 0;
FolgTo = 0;
Mod=5;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("Nettes Plätzchen!","",0,pClonk,0,pClonk,0);
AddMenuItem("Ja","EndTalk",0,pClonk,0,pClonk,0);
}

private func Mes6(id doof, object pClonk){
Mod=7;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("Ja. :)","",0,pClonk,0,pClonk,0);
AddMenuItem(" ... ","EndTalk",0,pClonk,0,pClonk,0);
}

private func Mes7(id doof, object pClonk){
Mod=7;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("Was Gibts?","",0,pClonk,0,pClonk,0);
AddMenuItem("Ich will Handeln","Handel",0,pClonk,0,pClonk,0);
AddMenuItem("Folge mir","CloseAndGo",0,pClonk,0,pClonk,0);
}

private func Handel(id doof, object pClonk){
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("Such dir was aus.","",0,pClonk,0,pClonk,0);
var i;
while(Contents(i,0)){
  AddMenuItem(Format("%s kaufen:%d",GetName(Contents(i,0)),GetValue(Contents(i,0))),"Buys",GetID(Contents(i,0)),pClonk,0,pClonk,0);
  i++;
 }
AddMenuItem("Nichts dabei.","EndTalk",0,pClonk,0,pClonk,0); 
}

private func Buys(id di, object pClonk){
 if(GetWealth(GetOwner(pClonk)) >= GetValue(0,di)){
       CreateContents(di,pClonk);

    DoWealth(GetOwner(pClonk),-GetValue(0,di));
    RemoveObject(FindContents(di));
   Sound("UnCash",0,this());
  }
  else
   Sound("ComandFailure1",0,this());
  Handel(0, pClonk);
}
