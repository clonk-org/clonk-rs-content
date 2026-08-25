/* Symbol */

#strict 2

local pTarget;
local idStaff;
local iTime;

public func SetBagID(idStaff, pNewTarget)
{
  pTarget = pNewTarget;
//  SetGraphics(0, this, idStaff,1,GFXOV_MODE_IngamePicture);
  SetPosition(40, -65);
	SetPosition(30, 70);
  SetVisibility(VIS_Owner);
}

protected func Timer()
{
  var pCursor = GetCursor(GetOwner(pTarget));
  if(pCursor == pTarget || GetActionTarget(0, pCursor)==pTarget)
    SetVisibility(VIS_Owner);
  else SetVisibility(VIS_None);
}

protected func GetClonk() { return pTarget; }

public func Ejection (object pObj)
{
//	pObj->SetObjectLayer(pTarget->GetObjectLayer());
  UpdateQuestDisplay();
}

local iLastCount;

func UpdateQuestDisplay()
{
	var i = 0;
	while(Contents(i))
	{
		var pObj = Contents(i);
		SetGraphics(0, this, GetID(pObj), i+1, GFXOV_MODE_IngamePicture);
		SetObjDrawTransform(1000, 0, 1000*45*i, 0, 1000, 0, this, i+1);
		if(pObj->GetClrModulation())
			SetClrModulation(pObj->GetClrModulation(), this, i+1);
		else
			SetClrModulation(RGB(255,255,255), this, i+1);
		i += 1;
	}
	var j = i;	
	while(j < iLastCount)
	{
		SetGraphics(0, this, 0, i+1, 0);
		j += 1;
	}
	iLastCount = i;
}

public func Highlight(pHigh)
{
  var iEff = AddEffect("HighlightTimer", this, 1, 1, this);
  EffectVar(1, this, iEff) = pHigh;
}

func FxHighlightTimerStart(pTarget, iNumber)
{
  EffectVar(0, pTarget, iNumber) = 35;
}

func FxHighlightTimerTimer(pTarget, iNumber)
{
  var i = 0;
  while(Contents(i))
  {
    var pObj = Contents(i);
    if(pObj == EffectVar(1, pTarget, iNumber))
      break;
    i += 1;
  }
  var size = EffectVar(0, pTarget, iNumber)*3000/35;
  SetObjDrawTransform(size+1000, 0, 1000*45*i+size*25/2, 0, size+1000, size*25/2, this, i+1);
  EffectVar(0, pTarget, iNumber)--;
  if(EffectVar(0, pTarget, iNumber)==0)
    return -1;
}