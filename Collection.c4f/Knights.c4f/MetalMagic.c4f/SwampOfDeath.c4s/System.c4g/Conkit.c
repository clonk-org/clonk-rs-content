/*--- Bausatz ---*/

#strict 2

#appendto CNKT

static iLevel;

public func Activate(pClonk)
{ 
  [$TxtCreateconstructionsi$]
  // Clonk anhalten
  SetComDir(COMD_Stop, pClonk);
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(CXCN, pClonk, this, C4MN_Extra_Value, "$TxtNoconstructionplansa$");
  var pRepair;
  if(pRepair = FindObject2(Find_Or(Find_Category(C4D_Structure), Find_Category(C4D_Vehicle)), Find_AtPoint()))
  {
    if(pRepair->~GetRepairValue())
    {
      SetGraphics(0, pRepair, CXCN, 1, GFXOV_MODE_Picture);
      SetObjDrawTransform(800, 0, 0, 0, 800, 0, pRepair, 1);
      AddMenuItem("$TxtRepair$", "RepairSite", GetID(pRepair), pClonk, 0, pRepair, "", 128+4, pRepair, pRepair->~GetRepairValue());
      SetGraphics(0, pRepair, 0, 1);
    }
    if(pRepair->~UpgradeTo())
      if(iLevel >= pRepair->~UpgradeTo()->~NeedLevel())
    {
      var pIcon = CreateContents(pRepair->~UpgradeTo());
      var iValue = GetValue(0, pRepair->~UpgradeTo())-GetValue(pRepair);
			if(pIcon)
			{
        SetGraphics(0, pIcon, RSR1, 1, GFXOV_MODE_Picture);
        SetObjDrawTransform(800, 0, 0, 0, 800, 0, pIcon, 1);
        AddMenuItem("$TxtUpgrade$", "UpgradeSite", pRepair->~UpgradeTo(), pClonk, 0, pRepair, "", 128+4, pIcon, iValue);
        RemoveObject(pIcon);
      }
      else
      {
      SetGraphics(0, pRepair, RSR1, 1, GFXOV_MODE_Picture);
      SetObjDrawTransform(800, 0, 0, 0, 800, 0, pRepair, 1);
      AddMenuItem("$TxtUpgrade$", "UpgradeSite", pRepair->~UpgradeTo(), pClonk, 0, pRepair, "", 128+4, pIcon, iValue);
      SetGraphics(0, pRepair, 0, 1);
      }
    }
  }
  var idType; var i = 0;
  while (idType = GetPlrKnowledge(GetOwner(pClonk), 0, i++, C4D_Structure))
    if(iLevel >= idType->~NeedLevel())
      AddMenuItem("$TxtConstructions$", "CreateConstructionSite", idType, pClonk, 0, pClonk);
  return 1;
}

protected func RepairSite(idType, pSite, bRight, iValue)
{
  var pClonk = Contained();
  if(GetWealth(GetOwner(pClonk)) < iValue)
  {
    PlayerMessage(GetOwner(pClonk),"$NotEnoughMoney$",this);
    return 1;
  }
  DoWealth(GetOwner(pClonk),-iValue);
  EffectVar(0,pSite,GetEffect("DestructionCheck", pSite)) = 0;
  Message("@100%", pSite);
  // Sound
  Sound("Cash");
  Sound("Build1"); Sound("Build2");
}

protected func UpgradeSite(idType, pSite, bRight, iValue)
{
  var pClonk = Contained();
  if(GetWealth(GetOwner(pClonk)) < iValue)
  {
    PlayerMessage(GetOwner(pClonk),"$NotEnoughMoney$",this);
    return 1;
  }
  DoWealth(GetOwner(pClonk),-iValue);
  AddEffect("Upgrade", pSite, 1, 1, 0, GetID());
//  pSite->~DoUpgrade();
  // Sound
  Sound("Cash");
  Sound("Build1"); Sound("Build2");
}


func FxUpgradeTimer(pTarget, iNumber, iTime)
{
  if(!EffectVar(0, pTarget, iNumber))
  {
    EffectVar(0, pTarget, iNumber) = 1;
    SetGraphics(0, pTarget, GetID(pTarget), 1, 1, 0, 2);
  }
  var pOverlay = pTarget;
  if(iTime<UpgradeTime())
  {
    SetClrModulation(RGBa(255,255,255,Max(255-iTime*255/UpgradeTime(), 0)), pOverlay, 1); 
  }
  else if(iTime==UpgradeTime())
  {
    pTarget->DoUpgrade();
    SetGraphics(0, pOverlay, GetID(pTarget), 1, 1, 0, 2);
    if(GetID(pTarget)==WZKP) SetObjDrawTransform(800, 0, 5000, 0, 1000, 0, pOverlay, 1);
    SetClrModulation(RGBa(255,255,255,0), pOverlay, 1); 
  }
  else if(iTime==UpgradeTime()*2)
  {
    SetGraphics(0, pOverlay, 0, 1);
    return -1;
  }
  else
  {
    SetClrModulation(RGBa(255,255,255,Min((iTime-UpgradeTime())*255/UpgradeTime(), 255)), pOverlay, 1); 
  }
}

func UpgradeTime() { return 30; }

public func MouseCreateConstructionSite(pClonk, idType)
{
  CreateConstructionSite(idType, pClonk);
}

protected func CreateConstructionSite(idType, pClonk)
{
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (GetAction(Contained()) != "Walk") return;
  if (Contained(Contained())) return;
  // Pruefen, ob das Gebaeude hier gebaut werden kann
  if (idType->~RejectConstruction(0, 10, Contained()) ) return;
  // Besitzer setzen für CreateConstruction
  SetOwner(GetOwner(Contained()));
  if(GetWealth(GetOwner(pClonk)) < GetValue(0,idType))
  {
    PlayerMessage(GetOwner(pClonk),"$NotEnoughMoney$",this); 
    return 1;
  }
  // Baustelle erschaffen
  var pSite;
  if (!(pSite = CreateConstruction(idType, 0, 10, -1, 100, 1,1))) return;
  pSite->SetPlrViewRange(200);
  pSite->~Completion();
  DoWealth(GetOwner(pClonk),-GetValue(0,idType));
  if(idType==ALTR) SetPosition(GetX(pClonk), GetY(pClonk)-10, pClonk);
  // Meldung
  Message("$TxtConstructions$", Contained(), GetName(pSite));
  // Sound
  Sound("Cash");
  Sound("Build1"); Sound("Build2");
  // Bausatz verbrauchen
 // RemoveObject();
  return 1;
}

protected func Hit()
{
  Sound("RockHit*");
	return 1;
}

/* Produkteigenschaften */

public func IsAnvilProduct() { return 1; }
