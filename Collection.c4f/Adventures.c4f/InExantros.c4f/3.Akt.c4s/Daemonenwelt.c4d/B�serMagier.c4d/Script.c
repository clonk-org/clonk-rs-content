#strict
#include KNIG

static gGrad;

local color;
local name;
local pAttack;
local pFern;
local pXp;
local pYp;
local sbow;
local ptyp;
local pMagier;
local istufe;
local pweg;
local pAusweichen;
local plicht;
local pende;
local ptime;

Stopp:
  SetAction("Walk");
  ObjectCall(pende,"Start");
  return(1);

LadeCheck:
  if(Local(9)>GetEnergy())  return(Stopp()); //wurde angegriffen
  SetLocal(9,GetEnergy());
  while(SetVar(0,FindObject(0,-100,-100,200,200,OCF_Prey(),0,0,NoContainer(),Var(0))))  if(GetAlive(Var(0)))  return(Stopp()); //ist in der nähe
  return(1);

Lade:
  plicht=Par(0);
  pende=Par(1);
  SetAction("Lade");
  return(1);

IsGegner: return(1);

Initialize:
  color=HSL2RGB(RGB(10,10,10));
  SetColorDw(color);
//  SetGraphics(0, this(), MAGE);
  SetGraphics(0, this(), KNIG);
  pXp=GetX();
  pYp=GetY();
  SetAction("Walk");
  SetDir(DIR_Right());
  SetLocal(9,GetEnergy());
  return(1);

// gegner Typ

Gegner1: //Endgegner-Magier
  var iplayer=GetPlayerCount();
  istufe =(Par(1)+gGrad)*2;
  ptyp=0;
  pMagier=1;
  pSchutz=2+Random(2)+iplayer+istufe;
  pAttack=20+Random(3)+iplayer+istufe;
  pFern=10+Random(2)+iplayer+istufe;
  var obj;
//Robe
  obj=0;
  obj=CreateContents(SUM_);
  if(obj)  Wear(obj);
//Nahkampfwaffe
  obj=0;
  DrawWeapon(CreateContents(_JST));
  EnergyBalken(this());
  return(1);

Hurt:
  Sound("Hurt*");
  return(1);

protected func CatchBlow(iLevel, pObj) 
{ 
  if(!Random(4))  Sound("Die*");
  if(!Random(2))  Sound("Hurt*");
  return(1);     
} 

//Funktionen die überladen werden

// Waffe ziehen
public func DrawWeapon(pWeap)
{
	if(!pWeap) return();
	if(pWeapon) CollectWeapon();
	pWeapon=pWeap;
//	CreateContents(EMPT);
	CreateContents(WEHA)->Initialize();
	Exit(pWeap);
	ShiftContents (this(), 0, WEHA); 
	pWeap->SetObjectLayer(this());
	SetCategory(C4D_Object() | C4D_MouseIgnore(), pWeap);
	ObjectSetAction(pWeap, "Fight");
	if(pWeap->~IsTwohanded() && pShield) { UnWear(TypeShield()); }
	OrdenLayers();
	return(1);
}

// Anziehen
public func Wear(pObj)
{
	if(!pObj) return();	
 	var iType = pObj->~GetWearType();
	if(!iType) return(DrawWeapon(pObj));
	if(SpecialWear(iType)) return(1);
	if(iType==TypeShield() && pWeapon) if(pWeapon->~IsTwohanded()) 
		return(1);
	if(GetWearObject(iType))
	{
		pWillWear = pObj;
		SetUnWearAction(iType);
		return(1);
	}
	var i = 0;	
	if(i = GetBlockingObject(iType))
	{
		pWillWear = pObj;
		SetUnWearAction(i);
		return(1);
	}
	if(i = GetBlockingTypes(pObj))
	{
		pWillWear = pObj;
		SetUnWearAction(i);
		return(1);
	}
	Exit(pObj);
	pObj->~PutOn(this());
	if(pObj->~GetUsedGraphics())
	{
	  var pOvrl = CreateObject(OVRL);
	  Enter(pOvrl, pObj);
	  SetGraphics(pObj->~GetUsedGraphics(), pOvrl);
	  SetClrModulation(pObj->~GetUsedColor(), pOvrl);
	  SetObjectBlitMode(pObj->~GetBlitMode(), pOvrl);
	  pObj = pOvrl;
	}
	pObj->SetObjectLayer(this());
	SetCategory(C4D_Object() | C4D_MouseIgnore(), pObj);
	ObjectSetAction(pObj, "Wear");
	GetWearObject(iType)=pObj;
	if(!GetEffect("Wear", this())) AddEffect("Wear", this(), 1, 1, this());
//	SetWearAction(iType);
	ScheduleCall(this(), "OrdenLayers", 1, 1);
	return(1);
}

protected func Death(int iKiller, a,b,c,d,e)
{
// Angelegtes Zeugs löschen
  GameCall("Abspann");
  if(pWeapon)  SetVar(0,pWeapon); //Speichern
  CollectWeapon();
  RemoveObject(Var(0));
  SetVar(0,0);
  if(sbow)  RemoveObject(sbow); //Bogen entfernen
  if(pArmor)  SetVar(0,Contents(0,pArmor));
  if(pBelt)  SetVar(1,Contents(0,pBelt));
  if(pHelmet)  RemoveObject(pHelmet);
  if(pShield)  RemoveObject(pShield);
  if(pBoots)  SetVar(4,Contents(0,pBoots));
  if(pTrousers)  SetVar(5,Contents(0,pTrousers));
  var i = 0;
  while(++i<=GetMaxType())
  if(GetWearObject(i)) UnWear(i);
  if(Var(0))  RemoveObject(Var(0));
  if(Var(1))  RemoveObject(Var(1));
  if(Var(4))  RemoveObject(Var(4));
  if(Var(5))  RemoveObject(Var(5));
  ObjectCall(CreateObject(_GEG,0,0,-1),"Los",0,0);
  ObjectCall(pende,"Tot");
  RemoveObject();
}

GoBack: //Damit der Clonk einen Abstand zum Gegner einhält
  var pEnemy;
  if(GetDir()==DIR_Left())  while(pEnemy = FindObject(0,-13,-15,12,30, OCF_Prey(), 0, 0, 0, pEnemy))  if(GetAlive(pEnemy))
	if(!GBackSolid(+10,0))  return(SetYDir(-3),SetXDir(+6),SetDir(DIR_Left()));

  if(GetDir()==DIR_Right())  while(pEnemy = FindObject(0,1,-15,12,30,OCF_Prey(), 0, 0, 0, pEnemy))  if(GetAlive(pEnemy))
	if(!GBackSolid(-10,0))  return(SetYDir(-3),SetXDir(-6),SetDir(DIR_Right()));
  return(1);

protected func Strike()
{
  SetComDir(COMD_Stop);	
  if(!pWeapon) return(0);
  var pEnemy;
  if(GetDir()==DIR_Left())  while(pEnemy = FindObject(0,-22,-15,21,30, OCF_Prey(), 0, 0, 0, pEnemy))  if(GetAlive(pEnemy))  if( (Contained()==Contained(pEnemy)) )  break;
  if(GetDir()==DIR_Right())  while(pEnemy = FindObject(0,1,-15,21,30,OCF_Prey(), 0, 0, 0, pEnemy))  if(GetAlive(pEnemy))  if( (Contained()==Contained(pEnemy)) )  break;
  if(!pEnemy) return(Sound("Schlag*"));
  if(pEnemy==this()) return(0); // So sollst nicht sein! Keine Selbsttreffer
//Ausweichen?
  SetVar(5,Random(30));
  if(LocalN("pAusweichen",pEnemy)>Var(5)-1)  return(Sound("Schlag*")); //Ausgewichen :o
//Getroffen
  pWeapon->~StrikeSound(); 
  var iDamage=pAttack;
  iDamage=Random(iDamage+pKraft);
  if(LocalN("pSchutz",pEnemy)>iDamage-1)  return(Sound("ArmorHit*"),DoEnergy(-Random(2),pEnemy));
  iDamage=iDamage-LocalN("pSchutz",pEnemy);
  if(GetAction(pEnemy)S="Block")  return(Sound("SwordHit*"));
  Punch(pEnemy,iDamage);
//  Message("Schaden: %d", this(),iDamage);
  Sound("Travel*");
//BlutEffekt
  var i = 3+Random(3);
  if(GetDir()==DIR_Left())  while(i>0) { SetR(Random(360),CreateObject(_BLS,GetX(pEnemy)-GetX()-6,GetY(pEnemy)-GetY()+3+Random(4),-1)); --i; }
  if(GetDir()==DIR_Right()) while(i>0) { SetR(Random(360),CreateObject(_BLS,GetX(pEnemy)-GetX()+6,GetY(pEnemy)-GetY()+3+Random(4),-1)); --i; }
}


//Alte script k.a. was ich da brauche?!

private func Hostile(player1, player2)
{
	if(player1==player2) return(0);
	if(player1==-1) return(1);
	if(player2==-1) return(1);
	return(_inherited(player1, player2));
}

public func DoOrder()
{
  fNoOrder = 0;
  OrdenLayers();
}

protected func GetRandomEquipment(iType)
{
  var i, id; var j;
  // Liste mit Helmen machen
  while(id = GetDefinition(i++, C4D_Object))
    if(id->~GetWearType()==iType) { Local(j++) = id; }	  
  
  return(Local(Random(j)));
}

protected func GetRandomWeapon()
{
  var i, id; var j;
  // Liste mit Helmen machen
  while(id = GetDefinition(i++, C4D_Object))
    if(id->~IsFightWeapon()) { Local(j++) = id; }	  
  
  return(Local(Random(j)));
}

Kampf:
//Keine Waffe, also abbrechen
  if(!FindContents(WEHA))  return(SetAction("Walk"));
//Gegner abchecken
  while(SetVar(0,FindObject(0,-21,-15,20,30,OCF_Prey(),0,0,NoContainer(),Var(0))))  if(GetAlive(Var(0)))  return(SetDir(DIR_Left()),GoBack(),Nahkampf(Var(0)));
  while(SetVar(0,FindObject(0,  1,-15,20,30,OCF_Prey(),0,0,NoContainer(),Var(0))))  if(GetAlive(Var(0)))  return(SetDir(DIR_Right()),GoBack(),Nahkampf(Var(0)));
 //Kein Gegner mehr in da
  SetAction("Walk");
  return(1);

Schlagen:
  SetVar(0,Par(0));
//Keine Waffe, also abbrechen
  if(!FindContents(WEHA))  return(0);
//Waffe auswählen
  if(FindContents(WEHA))  if(!GetID(Contents(0))==WEHA)  ShiftContents(0,true,WEHA);
//In KampfStellung gehen
  SetAction("Kampf");
  return(1);

Nahkampf:
//Waffe auswählen
  if(FindContents(WEHA))  if(!GetID(Contents(0))==WEHA)  ShiftContents(0,true,WEHA);
//evtl. Parieren?
  if(!Random(6))  if(GetAction()S="Kampf")  if(GetAction(Var(0))S="Spike")  return(SetAction("Block"));
  if(!Random(6))  if(GetAction()S="Kampf")  if(GetAction(Var(0))S="HeadStrike")  return(SetAction("Block"));
  if(!Random(6))  if(GetAction()S="Kampf")  if(GetAction(Var(0))S="HeadSpike")  return(SetAction("Block"));
  if(!Random(6))  if(GetAction()S="Kampf")  if(GetAction(Var(0))S="Strike")  return(SetAction("Block"));
  if(!Random(6))  if(GetAction()S="Kampf")  if(GetAction(Var(0))S="MiddleSpike")  return(SetAction("Block"));
//evtl. Angreifen?
  SetVar(5,Random(30));
  if(Var(5)==0)  if(GetAction()S="Kampf")  return(SetAction("Spike"));
  if(Var(5)==1)  if(GetAction()S="Kampf")  return(SetAction("HeadStrike"));
  if(Var(5)==2)  if(GetAction()S="Kampf")  return(SetAction("HeadSpike"));
  if(Var(5)==3)  if(GetAction()S="Kampf")  return(SetAction("Strike"));
  if(Var(5)==4)  if(GetAction()S="Kampf")  return(SetAction("MiddleSpike"));
  return(1);

Collection2:
  return(1);

func Timer()
{
//Kristall saugen
  if(GetAction()S="Lade")  LadeCheck();
  if(GetAction()S="Lade")  return(DrawParticleLine ("Flash", 0, 0, -GetX()+GetX(plicht)-30+Random(60), -GetY()+GetY(plicht)-30+Random(60), 20, 100, RGBa(255,2,2,Random(120)),RGBa(255,50,50,Random(120)), -10));
//Tod nix mehr machen!
  if(!GetAlive())  return(0);
//Zeit ab
  ++ptime;
  if(ptime>40)  return(Zeitab());
//Rum laufen
  if(!Random(4))  if(Distance(pXp,pYp,GetX(),GetY())<200)  SetCommand(this(),"MoveTo",0,GetX()-40+Random(81),GetY(),0,0);
//Zurück laufen
  if(!Random(3))  if(Distance(pXp,pYp,GetX(),GetY())>200)  return(SetCommand(this(),"MoveTo",0,pXp,pYp,0,0,5));
//Teleportieren falls zuweit entfernt
  if(pweg==0)  if(Distance(pXp,pYp,GetX(),GetY())>350)  return(SetPosition(pXp,pYp,this()));
//Gegner suchen
  if(!Random(2))  if(GetAction()S="Swim")  return(SetCommand(this(),"MoveTo",0,pXp,pYp,0,0,5)); //Zurück!
  if(!Random(2))  if(!IsBowEquipped())  if(pMagier==0)  while(SetVar(0,FindObject(0,0,0,-1,-1,OCF_Prey(),0,0,NoContainer(),Var(0))))  if(ObjectDistance(this(),Var(0))<300)  if(GetAlive(Var(0)))  SetCommand(this(), "MoveTo", Var(0));
//Magie
  if(!Random(2))  while(SetVar(0,FindObject(0,0,0,-1,-1,OCF_Prey(),0,0,NoContainer(),Var(0))))  if(ObjectDistance(this(),Var(0))<300)  if(GetAlive(Var(0)))  if(pMagier==1)  if(KIMagier(Var(0)))  return(1);
//Nahkampf
  if(GetAction()S="Walk")  while(SetVar(0,FindObject(0,-21,-15,20,30,OCF_Prey(),0,0,NoContainer(),Var(0))))  if(GetAlive(Var(0)))  return(SetDir(DIR_Left()),Schlagen(Var(0)));
  if(GetAction()S="Walk")  while(SetVar(0,FindObject(0,  1,-15,20,30,OCF_Prey(),0,0,NoContainer(),Var(0))))  if(GetAlive(Var(0)))  return(SetDir(DIR_Right()),Schlagen(Var(0)));
  return(1);
}

Zeitab:
  ObjectCall(pende,"Next",GetEnergy(),GetX(),GetY()); //Leben speichern
  CastParticles("MSpark", 200, 50, 0,0, 10, 50, RGBa(20,10,160,128), RGB(0,0,244));

// Angelegtes Zeugs löschen
  if(pWeapon)  SetVar(0,pWeapon); //Speichern
  CollectWeapon();
  RemoveObject(Var(0));
  SetVar(0,0);
  if(sbow)  RemoveObject(sbow); //Bogen entfernen
  if(pArmor)  SetVar(0,Contents(0,pArmor));
  if(pBelt)  SetVar(1,Contents(0,pBelt));
  if(pHelmet)  RemoveObject(pHelmet);
  if(pShield)  RemoveObject(pShield);
  if(pBoots)  SetVar(4,Contents(0,pBoots));
  if(pTrousers)  SetVar(5,Contents(0,pTrousers));
  var i = 0;
  while(++i<=GetMaxType())
  if(GetWearObject(i)) UnWear(i);
  if(Var(0))  RemoveObject(Var(0));
  if(Var(1))  RemoveObject(Var(1));
//  if(Var(2))  RemoveObject(Var(2));
//  if(Var(3))  RemoveObject(Var(3));
  if(Var(4))  RemoveObject(Var(4));
  if(Var(5))  RemoveObject(Var(5));
  RemoveObject();
  return(1);

KIMagier:
  if(!Random(5))  if(GetAction()S="Walk"||GetAction()S="Kampf")  if(GetEnergy()<30)  return(ZHeilung2());
  if(!Random(5))  if(GetAction()S="Walk"||GetAction()S="Kampf")  while(SetVar(0,FindObject(0,-30,-30,40,40,OCF_Edible(),0,0,NoContainer(),Var(0))))  if(GetAlive(Var(0)))  if(GetEnergy(Var(0))<30)  return(ZHeilung());
  if(!Random(4))  if(GetAction()S="Walk"||GetAction()S="Kampf")  while(SetVar(0,FindObject(0,0,0,-1,-1,OCF_Prey(),0,0,NoContainer(),Var(0))))  if(ObjectDistance(this(),Var(0))<70)  if(GetAlive(Var(0)))  return(ZKreis());

  if(!Random(2))  if(GetAction()S="Walk"||GetAction()S="Kampf")  while(SetVar(0,FindObject(0,0,0,-1,-1,OCF_Prey(),0,0,NoContainer(),Var(0))))  if(ObjectDistance(this(),Var(0))<200)  if(GetAlive(Var(0)))  if(PathFree(GetX(), GetY(), GetX(Var(0)), GetY(Var(0))))  return(ZFire(Var(0)));

  return(1);

ZKreis:
  SetAction("Magic");
  SetVar(0,CreateObject(MDBT,0,0,-1));
  ObjectCall(Var(0),"Activate",this(),0,1);
  return(1);

ZFire:
  SetAction("Magic");
  var iAngle;
  iAngle=Angle(GetX(),GetY(),GetX(Par(0)),GetY(Par(0)) );
//  Message("Geger im Winkel von: %d Grad",this(),iAngle);
  SetDir(DIR_Right());
  if(GetX()-GetX(Par(0))>0)  SetDir(DIR_Left());
  SetVar(5,Random(4));
  if(Var(5)==0)  SetVar(0,CreateObject(_EIS,0,0,-1));
  if(Var(5)==1)  SetVar(0,CreateObject(_EIS,0,0,-1));
  if(Var(5)==2)  SetVar(0,CreateObject(_EIS,0,0,-1));
  if(Var(5)==3)  SetVar(0,CreateObject(MFRB,0,0,-1));
  var iplayer=GetPlayerCount();
  ObjectCall(Var(0),"Activate2",this(),iAngle,Random(14+iplayer+istufe) );
  return(1);

ZHeilung:
  SetAction("Magic");
  SetVar(0,CreateObject(GGHG,0,0,-1));
  ObjectCall(Var(0),"Activate",this(),0,1);
  return(1);

ZHeilung2:
  SetAction("Magic");
  SetVar(0,CreateObject(MGHL,0,0,-1));
  ObjectCall(Var(0),"Activate",this(),0,1);
  return(1);

public func GetRandomName()
{
  var index = Random(10);
  if(!index--) return("Tyr");
  if(!index--) return("Klat");
  if(!index--) return("Fotan");
  if(!index--) return("Gofer");
  if(!index--) return("Dewk");
  if(!index--) return("Lohr");
  if(!index--) return("Serkan");
  if(!index--) return("Olai");
  if(!index--) return("Pokat");
  if(!index--) return("Wert");
}

GoWeg:
  pXp=Par(0);
  pYp=Par(1);
  pweg=Par(2);
  return(1);