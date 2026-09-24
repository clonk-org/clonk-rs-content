/*-- Neues Script --*/

#strict

#include CLNK

local Mod;
local iKiff;
local Folg1,FolgTo;
local kiffen,Timer;

local specialding;

protected func ActivateEntrance(object pClonk){
if(!GetAlive(this())) return(0,Message("$Dlg01$",this()));
SetCommand(pClonk,"");
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0); 
if(Mod==0){
AddMenuItem("$Dlg02$","Arschloch",0,pClonk,0,pClonk,0);
AddMenuItem("$Dlg03$","Neugirig",0,pClonk,0,pClonk,0);
AddMenuItem("$Dlg04$","Neugirig",0,pClonk,0,pClonk,0);
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
AddMenuItem("$Dlg05$","",0,pClonk,0,pClonk,0);
AddMenuItem("$Dlg06$","NotFolg",0,pClonk,0,pClonk,0);
AddMenuItem("$Dlg07$","EndTalk",0,pClonk,0,pClonk,0);
}
if(Folg1==0) AddMenuItem("$Dlg08$","EndTalk",0,pClonk,0,pClonk,0);
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
if(Mod==1) SetName("$Dlg09$");
if(Folg1==1){
SetCommand(this(),"MoveTo",FolgTo);
if(!Random(130)) Message("$Dlg10$",this());
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
AddMenuItem("$Dlg11$","",0,pClonk,0,pClonk,0);
AddMenuItem(Format("$Dlg12$",GetTaggedPlayerName(GetOwner(pClonk))),"Mes2",0,pClonk,0,pClonk,0);
}

private func Arschloch(id doof, object pClonk){
Mod=1;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("$Dlg13$","",0,pClonk,0,pClonk,0);
AddMenuItem(Format("$Dlg14$",GetTaggedPlayerName(GetOwner(pClonk))),"Mes2",0,pClonk,0,pClonk,0);
}

private func Mes2(id doof, object pClonk){
Mod=2;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("$Dlg15$","Mes3",0,pClonk,0,pClonk,0);
}

private func Mes3(id doof, object pClonk){
Mod=3;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("$Dlg16$","",0,pClonk,0,pClonk,0);
AddMenuItem("$Dlg17$","Mes4",0,pClonk,0,pClonk,0);
AddMenuItem("$Dlg18$","Mesa4",0,pClonk,0,pClonk,0);
}
private func Mesa4(id doof, object pClonk){
iKiff = 1;
Mod=4;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("$Dlg19$","",0,pClonk,0,pClonk,0);
AddMenuItem("$Dlg20$","EndTalk",0,pClonk,0,pClonk,0);
}
private func Mes4(id doof, object pClonk){
Mod=5;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("$Dlg21$","",0,pClonk,0,pClonk,0);
AddMenuItem("$Dlg22$","Handel",0,pClonk,0,pClonk,0);
AddMenuItem("$Dlg23$","EndTalk",0,pClonk,0,pClonk,0);
}

private func Mes5(id doof, object pClonk){
Mod=5;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("$Dlg24$","",0,pClonk,0,pClonk,0);
AddMenuItem("$Dlg25$","Handel",0,pClonk,0,pClonk,0);
if(!Folg1) AddMenuItem("$Dlg26$","CloseAndGo",0,pClonk,0,pClonk,0);
if(Folg1) AddMenuItem("$Dlg27$","NotFolg",0,pClonk,0,pClonk,0);
if(iKiff) AddMenuItem("$Dlg28$","MakeJoint",0,pClonk,0,pClonk,0);
if(!iKiff) AddMenuItem("$Dlg29$","Mesa4",0,pClonk,0,pClonk,0);
AddMenuItem("$Dlg30$","EndTalk",0,pClonk,0,pClonk,0);
}

private func Folg(id doof, object pClonk){
Folg1 = 1;
FolgTo = pClonk;
EndTalk(0,pClonk);
Message("$Dlg31$",this());
}
private func MakeJoint(id doof, object pClonk){
Mod=5;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
if(FindContents(BSHL,pClonk)) AddMenuItem("$Dlg32$","",0,pClonk,0,pClonk,0);
if(!FindContents(BSHL,pClonk)) AddMenuItem("$Dlg33$","",0,pClonk,0,pClonk,0);
if(FindContents(BSHL,pClonk)){ RemoveObject(FindContents(BSHL,pClonk));
CreateContents(JOIN,pClonk,1); }
if(FindContents(JOIN,pClonk)) AddMenuItem("$Dlg34$","EndTalk",0,pClonk,0,pClonk,0);
if(!FindContents(JOIN,pClonk)) AddMenuItem("$Dlg23$","EndTalk",0,pClonk,0,pClonk,0);
}
private func NotFolg(id doof, object pClonk){
Folg1 = 0;
FolgTo = 0;
Mod=5;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("$Dlg35$","",0,pClonk,0,pClonk,0);
AddMenuItem("$Dlg06$","EndTalk",0,pClonk,0,pClonk,0);
}

private func Mes6(id doof, object pClonk){
Mod=7;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("$Dlg36$","",0,pClonk,0,pClonk,0);
AddMenuItem(" ... ","EndTalk",0,pClonk,0,pClonk,0);
}

private func Mes7(id doof, object pClonk){
Mod=7;
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("$Dlg37$","",0,pClonk,0,pClonk,0);
AddMenuItem("$Dlg38$","Handel",0,pClonk,0,pClonk,0);
AddMenuItem("$Dlg39$","CloseAndGo",0,pClonk,0,pClonk,0);
}

private func Handel(id doof, object pClonk){
CreateMenu(GetID(this()),pClonk,0,C4MN_Extra_None,GetName(),0,C4MN_Style_Dialog,0);
AddMenuItem("$Dlg40$","",0,pClonk,0,pClonk,0);
var i;
while(Contents(i,0)){
  AddMenuItem(Format("$Dlg41$",GetName(Contents(i,0)),GetValue(Contents(i,0))),"Buys",GetID(Contents(i,0)),pClonk,0,pClonk,0);
  i++;
 }
AddMenuItem("$Dlg42$","EndTalk",0,pClonk,0,pClonk,0); 
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
