#strict 2

func DoAttach()
{
	var iOffsetX = 0, iOffsetY = 0;
	var pTarget = GetActionTarget();
	if(!pTarget) return RemoveObject();
	SetClrModulation(RGBa(255,255,255,255));
	var pLayer;
	if(pLayer = pTarget->GetObjectLayer())
	{
		SetPosition(pTarget->GetX()+pLayer->GetLayerOffset(), pTarget->GetY()+pLayer->GetLayerOffset(1));
		if(GetCursor(GetOwner(pTarget)) == pTarget) SetPlrView(GetOwner(pTarget), this);
	}
	else
		SetPosition(pTarget->GetX(), pTarget->GetY());
}

// Partikel verschoben erzeugen
global func CreateParticle (szName, int iX, int iY, int iXDir, int iYDir, int a, b, pObj, bool fBehind)
{
  if(!pObj && GetObjectLayer() && GetObjectLayer()->~IsAccesible())
  {
    var pLayer = GetObjectLayer();
    var pDouble = GetObjectLayer()->LocalN("pDouble");
    if( pDouble != this)
    {
			var X = GetX()-pLayer->GetX();
			var Y = GetY()-pLayer->GetY();
			iX += X;
			iY += Y;
			return pDouble->CreateParticle(szName, iX, iY, iXDir, iYDir, a, b, pDouble, 0);
    }
  }
  return _inherited(szName, iX, iY, iXDir, iYDir, a, b, pObj, fBehind);
}

// Objekte beim erzeugen gleich zum Haus hinzufügen
global func CreateObject (id id, int iXOffset, int iYOffset, int iOwner)
{
  var pObj = _inherited(id, iXOffset, iYOffset, iOwner);
	if(!pObj) return;
  var pLayer = GetObjectLayer(pObj);
  if(pLayer)
    pLayer->~AddObject(pObj, 1, 1);
  return pObj;
}

// Inhaltsobjekte auch beim Haus hinzufügen
global func CreateContents (id id, object pObj, int iCount)
{
  var pObj, pLayer;
  if(!iCount) iCount = 1;
  for(var i = 0; i < iCount; i++)
  {
    pObj = _inherited(id, pObj);
		if(!pObj) return;
    pLayer = GetObjectLayer(pObj);
    if(pLayer)
      pLayer->~AddObject(pObj);
  }
  return pObj;
}
/*
global func UpdateSolidMask(object pObj)
{
	if(!pObj) pObj = this;
	var iX, iY, iWdt, iHgt, iTx, iTy;
	var iEff;
	if(iEff = GetEffect("IntSolidMask", pObj))
	{
		iX = EffectVar(0, pObj, iEff);
		iY = EffectVar(1, pObj, iEff);
	  iWdt = EffectVar(2, pObj, iEff);
	  iHgt = EffectVar(3, pObj, iEff);
	  iTx = EffectVar(4, pObj, iEff);
		iTy = EffectVar(5, pObj, iEff);
	}
	else
	{
		iX = GetDefCoreVal("SolidMask", "DefCore", GetID(pObj), 0);
		iY = GetDefCoreVal("SolidMask", "DefCore", GetID(pObj), 1);
	  iWdt = GetDefCoreVal("SolidMask", "DefCore", GetID(pObj), 2);
	  iHgt = GetDefCoreVal("SolidMask", "DefCore", GetID(pObj), 3);
	  iTx = GetDefCoreVal("SolidMask", "DefCore", GetID(pObj), 4);
		iTy = GetDefCoreVal("SolidMask", "DefCore", GetID(pObj), 5);
	}
	return SetSolidMask (iX, iY, iWdt, iHgt, iTx, iTy, pObj);
}

global func SetSolidMask (int iX, int iY, int iWdt, int iHgt, int iTx, int iTy, object pObj)
{
	var iXOffset = 0, iYOffset = 0, pLayer;
	if(pLayer = GetObjectLayer(pObj))
	{
		iXOffset = pLayer->GetLayerOffset(0);
		iYOffset = pLayer->GetLayerOffset(1);
	}
	RemoveEffect("IntSolidMask", pObj);
	var iEff = AddEffect("IntSolidMask", pObj, 1, 0);
	EffectVar(0, pObj, iEff) = iX;
	EffectVar(1, pObj, iEff) = iY;
	EffectVar(2, pObj, iEff) = iWdt;
	EffectVar(3, pObj, iEff) = iHgt;
	EffectVar(4, pObj, iEff) = iTx;
	EffectVar(5, pObj, iEff) = iTy;
	return _inherited(iX, iY, iWdt, iHgt, iTx+iXOffset, iTy+iYOffset, pObj);
}

global func PathFree (int x1, int y1, int x2, int y2)
{
	var iXOffset = 0, iYOffset = 0, pLayer;
	if(pLayer = GetObjectLayer())
	{
		iXOffset = pLayer->GetLayerOffset(0);
		iYOffset = pLayer->GetLayerOffset(1);
	}
	return _inherited(x1+iXOffset, y1+iYOffset, x2+iXOffset, y2+iYOffset);
}

global func GBackSolid (int x, int y)
{
	var iXOffset = 0, iYOffset = 0, pLayer;
	if(pLayer = GetObjectLayer())
	{
		iXOffset = pLayer->GetLayerOffset(0);
		iYOffset = pLayer->GetLayerOffset(1);
	}
	return _inherited(x+iXOffset, y+iYOffset);
}

global func GBackSky (int x, int y)
{
	var iXOffset = 0, iYOffset = 0, pLayer;
	if(pLayer = GetObjectLayer())
	{
		iXOffset = pLayer->GetLayerOffset(0);
		iYOffset = pLayer->GetLayerOffset(1);
	}
	return _inherited(x+iXOffset, y+iYOffset);
}

global func GBackSemiSolid (int x, int y)
{
	var iXOffset = 0, iYOffset = 0, pLayer;
	if(pLayer = GetObjectLayer())
	{
		iXOffset = pLayer->GetLayerOffset(0);
		iYOffset = pLayer->GetLayerOffset(1);
	}
	return _inherited(x+iXOffset, y+iYOffset);
}

global func GBackLiquid (int x, int y)
{
	var iXOffset = 0, iYOffset = 0, pLayer;
	if(pLayer = GetObjectLayer())
	{
		iXOffset = pLayer->GetLayerOffset(0);
		iYOffset = pLayer->GetLayerOffset(1);
	}
	return _inherited(x+iXOffset, y+iYOffset);
}

global func SetCategory(iCategory, pObj)
{
	if(!pObj) pObj = this;
	if(pObj->GetObjectLayer() && pObj->GetObjectLayer()->IsAccesible() && !(iCategory & 8388608))
		iCategory = iCategory | 8388608;
	return inherited(iCategory, pObj);
}

global func SetR(iR, pObj)
{
	if(!pObj) pObj = this;
	var result = inherited(iR, pObj);
	var iEff;
	if(iEff = GetEffect("IntUpdateInteriorRotation", pObj))
		EffectCall(pObj, iEff, "Timer");
	return result;
}*/

global func Message (string strMessage, object pObj)
{
  if(pObj && pObj->GetObjectLayer() && pObj->GetObjectLayer()->~IsAccesible())
  {
    var pLayer = pObj->GetObjectLayer();
    var pDouble = pObj->GetObjectLayer()->LocalN("pDouble");
    if( pDouble != this)
    {
			var X = GetX()-pLayer->GetX();
			var Y = GetY()-pLayer->GetY();
			return CustomMessage (strMessage, pDouble, -1, X, Y+pLayer->GetDefHeight()/2-pObj->GetDefHeight()/2, 0, 0, 0, MSG_Multiple); 
    }
  }
  return _inherited(strMessage, pObj, ...);
}

global func Sound (string szSound, bool fGlobal, object pObj, int iVolume, int iPlayer, int iLoopCount)
{
	if(!pObj) pObj = this;
	if(pObj && pObj->GetObjectLayer() && pObj->GetObjectLayer()->~IsAccesible())
  {
    var pLayer = pObj->GetObjectLayer();
    var pDouble = pObj->GetObjectLayer()->LocalN("pDouble");
    return _inherited(szSound, fGlobal, pDouble, iVolume, iPlayer, iLoopCount, ...);
  }
  return _inherited(szSound, fGlobal, pObj, iVolume, iPlayer, iLoopCount, ...);
}

func Manage(object pTarget) { SetAction("Mng",pTarget); SetVisibility(VIS_None); }

func PlrManage(int player) { SetAction("PlrMng"); SetOwner(player); SetVisibility(VIS_Owner); }

func Ramp() {
  Local(0)=0;
  Local(1)=0;
  SetVisibility(VIS_Owner);
  SightOrder();
  // Leute: Nicht mit Auflösungen über 2000x2000 spielen. k?
  SetObjDrawTransform(1000*2000, 0, GetDefWidth(GetID())*Abs(1000-2000*1000)/2, 0, 1000*2000, GetDefHeight(GetID())*Abs(1000-2000*1000)/2);
  SetObjDrawTransform(1000*1024, 0, 1000*1024-2000, 0, 1000*768,  1000*768-2000);
}

func SightOrder() {
  // tut des nach vorne
}

// Aufrüfe an Manager
func Enable()  { for(var i=0; i<MaxRampCount(); ++i) if(Local(i)) SetVisibility(VIS_Owner,Local(i)); }
func Disable() { for(var i=0; i<MaxRampCount(); ++i) if(Local(i)) SetVisibility(VIS_None,Local(i)); }

global func MaxRampCount() { return 10; }

global func SetSightModulation(int iRGBa, int iBlitMode, int iRamp, object pObj) {

  if(!pObj) if(!(pObj=this())) return false;
  if(GetOwner(pObj) == -1) return false;

  // Manager und Rampe
  var pClrObj, pRamp;
  if(pClrObj = FindObjectOwner(_VIW,GetOwner(pObj),0,0,0,0,0,"Mng",pObj))
    pRamp = Local(iRamp,pClrObj);

  // keine Farbe
  var r,g,b,a;
  SplitRGBaValue(iRGBa,r,g,b,a);
  if(!(r+g+b+a) || a==255) {
    if(pRamp) RemoveObject(pRamp);
    return false;
  }

  // ansonsten ggf. Manager und Rampe erzeugen
  if(!pClrObj) (pClrObj = CreateObject(_VIW,AbsX(2),AbsY(2),GetOwner(pObj)))->_VIW::Manage(pObj);
  if(!pRamp) (pRamp = Local(iRamp,pClrObj) = CreateObject(_VIW,AbsX(2),AbsY(2),GetOwner(pObj)))->_VIW::Ramp(); 

  // Modulation...
  SetObjectBlitMode(iBlitMode,pRamp);
  SetClrModulation(iRGBa,pRamp);

  // Sichtbarkeit prüfen
  if(GetCursor(GetOwner(pObj)) != pObj) SetVisibility(VIS_None,pRamp);

  return true;
}

global func SetPlrSightModulation(int iRGBa, int iBlitMode, int iRamp, int player, int fForeground) {

  // Manager und Rampe
  var pClrObj, pRamp;
  if(pClrObj = FindObjectOwner(_VIW,player,0,0,0,0,0,"PlrMng"))
    pRamp = Local(iRamp,pClrObj);

  // keine Farbe
  var r,g,b,a;
  SplitRGBaValue(iRGBa,r,g,b,a);
  if(!(r+g+b+a) || a==255) {
    if(pRamp) RemoveObject(pRamp);
    return false;
  }

  // ansonsten ggf. Manager und Rampe erzeugen
  if(!pClrObj) (pClrObj = CreateObject(_VIW,AbsX(2),AbsY(2),player))->_VIW::PlrManage(player);
  if(!pRamp)   (pRamp = Local(iRamp,pClrObj) = CreateObject(_VIW,AbsX(2),AbsY(2),player))->_VIW::Ramp();

	if(fForeground)
	{
		pRamp->SetCategory(2097153|C4D_Foreground|C4D_Object);
		pRamp->SetAction("View");
		pRamp->Message("@!", pRamp);
	}
	else
		pRamp->SetCategory(2097153);
	
  // Modulation...
  SetObjectBlitMode(iBlitMode,pRamp);
  SetClrModulation(iRGBa,pRamp);

  return true;
}

global func FadeSightModulation(int iRGBa, int iBlitMode, int iRamp, int player, int fBack, int fForeground)
{
  // Manager und Rampe
  var pClrObj, pRamp;
  if(pClrObj = FindObjectOwner(_VIW,player,0,0,0,0,0,"PlrMng"))
    pRamp = Local(iRamp,pClrObj);

  // ansonsten ggf. Manager und Rampe erzeugen
  if(!pClrObj) (pClrObj = CreateObject(_VIW,AbsX(2),AbsY(2),player))->_VIW::PlrManage(player);
  if(!pRamp)   (pRamp = Local(iRamp,pClrObj) = CreateObject(_VIW,AbsX(2),AbsY(2),player))->_VIW::Ramp();
	
  if(fBack) pRamp->SetCategory(C4D_Foreground);
	if(fForeground)
	{
		pRamp->SetCategory(C4D_Foreground|C4D_Object);
		pRamp->SetAction("View");
		pRamp->Message("@!", pRamp);
	}
  AddEffect("FadeSightModulation", 0, 1, 1, 0, 0, iRGBa, iRamp, player, [fBack, fForeground]);
}

global func FxFadeSightModulationStart(pTarget, iNumber, fTmp, iRGBa, iRamp, player, a)
{
  if(fTmp) return;
  var r,g,b,a;
	var fBack = a[0];
	var fForeground = a[1];
  SplitRGBaValue(iRGBa,r,g,b,a);
  EffectVar(0, pTarget, iNumber) = 0;
  EffectVar(1, pTarget, iNumber) = r;
  EffectVar(2, pTarget, iNumber) = g;
  EffectVar(3, pTarget, iNumber) = b;

  EffectVar(5, pTarget, iNumber) = iRamp;
  EffectVar(6, pTarget, iNumber) = player;
  EffectVar(7, pTarget, iNumber) = fBack;

	EffectVar(8, pTarget, iNumber) = fForeground;
}

global func FxFadeSightModulationTimer(pTarget, iNumber)
{
/*	if(EffectVar(8, pTarget, iNumber))
	{
		var pClrObj, pRamp;
			if(pClrObj = FindObjectOwner(_VIW,EffectVar(6, pTarget, iNumber),0,0,0,0,0,"PlrMng"))
				pRamp = Local(EffectVar(5, pTarget, iNumber),pClrObj);
		pRamp->SetCategory(C4D_Foreground|C4D_Object);
		pRamp->SetAction("View");
	}*/

  var a = 255-EffectVar(0, pTarget, iNumber);
  if(EffectVar(7, pTarget, iNumber)) a = EffectVar(0, pTarget, iNumber);
  SetPlrSightModulation(RGBa(EffectVar(1, pTarget, iNumber),EffectVar(2, pTarget, iNumber),EffectVar(3, pTarget, iNumber),a), EffectVar(4, pTarget, iNumber), EffectVar(5, pTarget, iNumber), EffectVar(6, pTarget, iNumber), EffectVar(8, pTarget, iNumber));
  EffectVar(0, pTarget, iNumber) += 26;
  if(EffectVar(0, pTarget, iNumber)>=255)
  {
    if(EffectVar(7, pTarget, iNumber)) SetPlrSightModulation(0, EffectVar(4, pTarget, iNumber), EffectVar(5, pTarget, iNumber), EffectVar(6, pTarget, iNumber));
    else SetPlrSightModulation(RGBa(EffectVar(1, pTarget, iNumber),EffectVar(2, pTarget, iNumber),EffectVar(3, pTarget, iNumber),0), EffectVar(4, pTarget, iNumber), EffectVar(5, pTarget, iNumber), EffectVar(6, pTarget, iNumber));
    return -1;
  }
}

func OnFadeDone() {
  // keine Farbe
  var r,g,b,a, pClrObj;
  SplitRGBaValue(GetClrModulation(),r,g,b,a);
  if(!(r+g+b+a) || a==255)
    RemoveObject();
}
