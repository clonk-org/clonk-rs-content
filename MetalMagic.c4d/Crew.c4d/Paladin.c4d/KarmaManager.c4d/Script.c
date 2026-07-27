/*-- Karmaenergie --*/

#strict 2

local iKarmaPrayer; // Die Gunst, die die Beter eigententlich produzieren
local iShow;
local iTime;
local iChurch;

func Create(iPlr, iNewChurch) {
  SetOwner(iPlr);
  if(FindObject(NTCH)) SetVisibility(VIS_Owner);
  else SetVisibility(VIS_Owner | VIS_Allies);
  SetPosition(-50,90);
  var szEnergy = "Energy";
  iChurch = iNewChurch;
  if(iChurch==scBloodchurch)
  {
    SetGraphics("DarkEmpty");
    SetGraphics("DarkFull",this, GetID(), 1, GFXOV_MODE_Action,"0");
  }
  UpdateGraphic();
  return 1;
}

public func SetChurch(iNewChurch)
{
  // Grafik entleeren
  for(var i=0; i<10; i++)
    SetGraphics(0, this, 0, i);
  // Neue Kirche speichern
  iChurch = iNewChurch;
  // Grafik anpassen
  if(iChurch==scBloodchurch)
  {
    SetGraphics("DarkEmpty");
    SetGraphics("DarkFull",this, GetID(), 1, GFXOV_MODE_Action,"0");
  }
  UpdateGraphic();
  return 1;
}

protected func Timer(fNoTime)
{
  var pClonk = GetCursor(GetOwner());
  if (!pClonk) return SetVisibility(VIS_None);
  if(!pClonk->~MaxKarmaEnergy() && !pClonk->~IsClonk())
    if (!(pClonk = GetActionTarget(0, pClonk))) return SetVisibility(VIS_None);
  var iKarmaEnergy = pClonk->~GetKarmaEnergy();
  var iMax = pClonk->~MaxKarmaEnergy();  
  if(!iMax) return SetVisibility(VIS_None);
  else SetVisibility(VIS_Owner);
  
  if(iMax != 100)
  {
  	var fac=(iKarmaEnergy*1000)/iMax;
  	iMax=100;
  	iKarmaEnergy=(fac*iMax)/1000;
  }
  
  if(iShow!=iKarmaEnergy)
  {
    if(Abs(iKarmaEnergy-iShow)<250)
      iShow = iKarmaEnergy;
    else if(iShow<iKarmaEnergy)
      iShow+=250;
    else iShow-=250;
    UpdateGraphic(iMax);
  }
  if(!iKarmaEnergy) return;
  return;
 /*
  // Effekt!!
  var iColor;
  iTime++;
  if(fNoTime) iTime--;
  if(MMGetChurch(GetOwner())==scBloodchurch)
  {
    iColor = RGB(200+Sin(iTime*2 ,50),150+Cos(iTime*3/2 ,50),0);
    for(var i=1; i<=3; i++)
    SetClrModulation(iColor, this, i);
  }
  else
  {
    iColor = RGB(220+Sin(iTime*2 ,30),220+Sin(iTime*2 ,30),100+Cos(iTime*2,100));
    for(var i=1; i<=9; i++)
    SetClrModulation(iColor, this, i);
  }*/
}

public func AdjustOverlay(iAmount, iUnder, iUpper, szName, iIndex, iR, iStart, iEnd)
{
  iUnder = iUnder/1000; iUpper = iUpper/1000;
  iAmount = (iAmount-iUnder)*(iEnd-iStart)/(iUpper-iUnder)+iStart;
  iAmount += Mod(iAmount, 2);
  iAmount = BoundBy(iAmount, 0, 74);
  SetGraphics(szName,this, GetID(), iIndex, GFXOV_MODE_Action,Format("%d", iAmount));
  var fsin=-Sin(iR, 1000), fcos=Cos(iR, 1000);
  SetObjDrawTransform(+fcos, +fsin, 520*(75-iAmount)*fsin/1000, -fsin, +fcos, 520*(75-iAmount)*fcos/1000, 0, iIndex);
}

public func UpdateGraphic(iMax)
{
  if(iChurch==scBloodchurch)
  {
    AdjustOverlay(iShow,     0, 38000, "DarkFull1", 1, 0, 12, 50); // 12+iShow*100/MaxKarmaEnergy()
    AdjustOverlay(iShow, 38000, 72000, "DarkFull2", 2, 0, 28, 62); // 28+iShow*100/MaxKarmaEnergy()-38
    AdjustOverlay(iShow, 72000,100000, "DarkFull3", 3, 0, 42, 70); // (42+iShow*100/MaxKarmaEnergy()-72
  }
  else
  {
    SetGraphics("LightFullCenter",this, GetID(), 1, GFXOV_MODE_Base);
    var iValue = Min(iShow*100/iMax,36);
    SetObjDrawTransform(880*iValue/36+120, 0, 0, 0, 880*iValue/36+120, 0, 0, 1);
    AdjustOverlay(iShow, 36000, 44000, "LightFullRing", 2,   0, 52, 74); // 52+(iShow*100/MaxKarmaEnergy()-36-8*0)*275/100
    AdjustOverlay(iShow, 44000, 52000, "LightFullRing", 3, -45, 52, 74);
    AdjustOverlay(iShow, 52000, 60000, "LightFullRing", 4, -90, 52, 74);
    AdjustOverlay(iShow, 60000, 68000, "LightFullRing", 5,-135, 52, 74);
    AdjustOverlay(iShow, 68000, 76000, "LightFullRing", 6,-180, 52, 74);
    AdjustOverlay(iShow, 76000, 84000, "LightFullRing", 7,-225, 52, 74);
    AdjustOverlay(iShow, 84000, 92000, "LightFullRing", 8,-270, 52, 74);
    AdjustOverlay(iShow, 92000,100000, "LightFullRing", 9,-315, 52, 74);
  }
  return;
}

global func GetKarmaHelper(int iPlr, int fNoTeam)
{
  var pObj;
  // Eigenen Helfer mit passender Kirche suchen
  pObj = FindObject2(Find_ID(KRME), Find_Owner(iPlr));
  // Bei Teamkirchen auch verbündete suchen
  if(!pObj && !FindObject(NTCH) && !fNoTeam)
    pObj = FindObject2(Find_ID(KRME), Find_Allied(iPlr));
  // Keinen gefunden? -> keine Kirche
  if(!pObj)
    return 0;
  return pObj;
}

static const scBloodchurch=1;
static const scLightchurch=2;

global func MMGetChurch(int iPlr) // Entweder Spieler oder Teamkriche, zu empfehlen
{
  var pObj;
  if(pObj = GetKarmaHelper(iPlr))
    return LocalN("iChurch", pObj);
}

global func MMGetPlayerChurch(int iPlr) // Nur SPielerkirche, Teams werden ignoriert
{
  return MMGetChurch(iPlr);
}

global func MMSetPlayerChurch(int iPlr,int iChurch, int fNoRecursion) // Spielerkirche setzen
{
  var pObj;
  if(!(pObj=GetKarmaHelper(iPlr, 1)))
    CreateObject(KRME, 0, 0, iPlr)->Create(iPlr, iChurch);
  else
    pObj->SetChurch(iChurch);
 
  var aAllied = Find_Allied(iPlr);
  if(FindObject(NTCH)) aAllied = Find_Owner(iPlr);
  // Allen Objekten vom Spieler den Wechsel mitteilen
  for(var pObj in FindObjects(aAllied))
    pObj->~OnChurchChange(iChurch);
    
  var iPlr2;
  if(!fNoRecursion)
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    iPlr2 = GetPlayerByIndex(i);
    if(iPlr2 != iPlr && !Hostile(iPlr, iPlr2))
      MMSetPlayerChurch(iPlr2, iChurch, 1);
  }
 
  return 1;
}

global func MMStartPraying(int iAmout, object pPrayer)
{
  return;
}

global func MMStopPraying(object pPrayer)
{
  return;
}

func FxPrayingTimer(pTarget, iNumber, iTime)
{
  if(iChurch==scBloodchurch)
    for(var pObj in EffectVar(2, pTarget, iNumber))
    {
      pObj->CreateParticle("PSpark",RandomX(-7,7),RandomX(-7,7),0,-1,15,RGB(180,10,10),pObj,Random(2));
      DoMagicEnergy(1, pObj);
    }
  else
    for(var pObj in EffectVar(2, pTarget, iNumber))
    {
      pObj->CreateParticle("PSpark",RandomX(-7,7),RandomX(-7,7),0,-1,15,RGB(255,255,10),pObj,Random(2));
      DoMagicEnergy(1, pObj);
    }
}

func FxIntPrayingTimer(pObj, iNumber, iTime)
{
  if(iChurch==scBloodchurch)
      pObj->CreateParticle("PSpark",RandomX(-7,7),RandomX(-7,7),0,-1,15,RGB(180,10,10),pObj,Random(2));
  else
      pObj->CreateParticle("PSpark",RandomX(-7,7),RandomX(-7,7),0,-1,15,RGB(255,255,10),pObj,Random(2));
  if(!DoMagicEnergy(1, pObj)) return 0;
}

