/*-- Priester --*/

#strict 2

#include CLNK

local alreadyInfo;

protected func Recruitment()
{
  if(!alreadyInfo)
  {
    alreadyInfo = 1;
    var clnk = CreateObject(CLNK,0,0,GetOwner());
    MakeCrewMember(clnk,GetOwner());
    GrabObjectInfo(clnk,this);
    RemoveObject(clnk);
    DoEnergy(100);
  }
  StartChurchChoose(GetOwner());
  // Grafik nach Kirche
  SetGraphics("");
  if(MMGetChurch(GetOwner())==scBloodchurch) SetGraphics("Dark");
  _inherited();

  NewPortrait();

  AddEffect("God", this, 1, 1, this);
}

func FxGodTimer(pTarget, iNumber, iTime)
{
  if(EffectVar(0, pTarget, iNumber)) EffectVar(0, pTarget, iNumber)--;
  if(EffectVar(1, pTarget, iNumber)) EffectVar(1, pTarget, iNumber)--;
  if(!Random(50))
    if(GetEnergy()<GetKarmaEnergy())
    {
      DoEnergy(1);
      if(!Random(2)) DoKarmaEnergy(-1);
      CreateParticle("PSpark",RandomX(-7,7),RandomX(-7,7),0,-1,15,RGB(255,255,10),this,Random(2));
    }
  if(!EffectVar(1, pTarget, iNumber))
  if(GetProcedure()=="FIGHT")
  {
   if(!Random(800-GetKarmaEnergy(GetOwner())*2) && GetActionTarget())
     if(GetEnergy()<GetEnergy(GetActionTarget()))
    {
      DoLightningEffect(GetActionTarget());
      DoEnergy(-20-Random(5), GetActionTarget());
      DoKarmaEnergy(-20, GetOwner());
      EffectVar(1, pTarget, iNumber)=(150-GetKarmaEnergy(GetOwner()))*10;
    }
  }
}

func DoLightningEffect(pClonk)
{
  DrawLightning(GetX(pClonk),GetY(pClonk),GetX(pClonk),GetY(pClonk)-100);
  CreateParticle("PxSpark", 0,-100,0,0, 60, RGB(0,100,150), 0, 1);
  for(var i=0; i<3; i++)
    pClonk->CreateParticle("Smoke", 0,-80,0,0,300, RGB(0,150,250));
  for(var i=0; i<10; i++)
    pClonk->CreateParticle("PxSpark", 0,0,-Sin(Random(360), 7),Cos(Random(360),7), 40, RGB(0,100,150), 0, 1);
  pClonk->Sound("Thunder");
}

func FxGodDamage(pTarget, iNumber, iDmgEngy, iCause)
{
  if(iDmgEngy>0) return iDmgEngy;
  if(EffectVar(0, pTarget, iNumber)) return;
  if(!Random(2) && -iDmgEngy/1000>=GetEnergy())
    if(GetKarmaEnergy(GetOwner())>(iDmgEngy/500))
    {
      iDmgEngy = -GetEnergy()*1000 + 1000;
      for(var i=0; i<360; i+=30)
        CreateParticle("NoGravSpark", 0,0,-Sin(i, 5)+RandomX(-2,2),Cos(i,20)+RandomX(-2,2), 50, RGB(155,130,0), this, 1);
      EffectVar(0, pTarget, iNumber) = 36*10;
    }
  return iDmgEngy;
}

func FxGodEffect(string szNewEffectName, object pTarget) {
  // nur einmal den Hilfseffekt des Gottes
  if(szNewEffectName == "God") return -1;
}

protected func NewPortrait()
{
  if(MMGetChurch(GetOwner())==scBloodchurch) SetPortrait(Format("Dark%d",Random(StdPortraitCount())+1), this); 
  else SetPortrait(Format("%d",Random(StdPortraitCount())+1), this); 
  return;
}

// Gibt Anzahl an Standard-Portraits an
public func StdPortraitCount() { return 2; }

public func ChangedTo()
{
  // Grafik nach Kirche
  SetGraphics("");
  if(MMGetChurch(GetOwner())==scBloodchurch) SetGraphics("Dark");
  // Portrait
  NewPortrait();
  AddEffect("God", this, 1, 1, this);
}

public func OnChurchChange(iChurch)
{
  ChangedTo();
}

// Beten
public func ContextPray(pCaller)
{
  [$CtxPrayDesc$|Image=PRST|Condition=PrayingAllowed]
  
  if(GetAction() == "Walk")
    SetAction("Pray");
  else if(GetAction() == "Pray")
    SetAction("KneelUp");
  
  return 1;
}

protected func StartPraying()
{
  var iAmount = 8;
  if(FindObject2(Find_ID(SNHP), Find_AtPoint())) iAmount = 16;
  else if(FindObject2(Find_ID(ALTR), Find_AtPoint())) iAmount = 12;
  if(!MMStartPraying(iAmount)) SetAction("Walk");
}

protected func StopPraying()
{
  if(GetAction()=="Pray") return;
  MMStopPraying();
}
 
// Aufhören mit dem Beten
protected func ControlUpDouble()
{
  if(GetAction() == "Pray")
  {
    SetAction("KneelUp");
    SetComDir(COMD_Down);
    return 1;
  }
   
  return _inherited(Par(),Par(1),Par(2));
}
 
protected func ControlLeftDouble()
{
  if(GetAction() == "Pray")
  {
    SetAction("KneelUp");
    SetComDir(COMD_Left);
    return 1;
  }
   
  return _inherited(Par(),Par(1),Par(2));
}
 
protected func ControlRightDouble()
{
  if(GetAction() == "Pray")
  {
    SetAction("KneelUp");
    SetComDir(COMD_Right);
    return 1;
  }
  
  return _inherited(Par(),Par(1),Par(2));
}

protected func PrayingAllowed()
{
  return !ObjectCount(NPRY) && !Contained() && MMGetChurch(GetOwner());
}

// Karmaenergie fuer den Priester

local iKarma;

public func GetKarmaEnergy()
{
  return iKarma;
}

public func DoKarmaEnergy(iAmount)
{
	// Karma anpassen und zurueckgeben, ob die Anpassung vollstaendig war.
	var iNewKarma = iKarma+iAmount;
	iKarma = BoundBy(iNewKarma, 0, MaxKarmaEnergy());
  return iKarma == iNewKarma;
}

public func MaxKarmaEnergy()
{
  return 100;
}


/* Status */

public func IsPriest() { return true; }
