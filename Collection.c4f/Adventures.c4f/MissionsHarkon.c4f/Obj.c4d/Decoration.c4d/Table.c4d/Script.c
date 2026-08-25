/*-- Tisch --*/

#strict 2

local aChairs;
local aCards;
local iLastCard;
local sNextText;
local iLastPlayer;

public func SetChairs(aListe)
{
	aChairs = aListe;
	if(!aListe) aChairs = [1,1,1,1];
	if(aChairs[0])
	{
		SetGraphics(0, this, GetID(), 1, 2, "Chair1");
		SetObjDrawTransform(+1000, 0, -22000, 0, 1000, -2000, this, 1);
	}
	if(aChairs[1])
	{
		SetGraphics(0, this, GetID(), 2, 2, "Chair2");
		SetObjDrawTransform( 1000, 0,  -5000, 0, 1000, -2000, this, 2);
	}
	if(aChairs[2])
	{
		SetGraphics(0, this, GetID(), 3, 2, "Chair2");
		SetObjDrawTransform( 1000, 0,  +5000, 0, 1000, -2000, this, 3);
	}
	if(aChairs[3])
	{
		SetGraphics(0, this, GetID(), 4, 2, "Chair1");
		SetObjDrawTransform(-1000, 0,  22000, 0, 1000, -2000, this, 4);
	}	
	SetGraphics(0, this, GetID(), 20, 1);
}

//public func RackedItem() { return "IsShield"; }
public func RackedItemCount() { return 4; }

protected func UpdateTransferZone()
{
  SetAction("Update");
}

protected func Update()
{
  var i, pObj;
  while(pObj = Contents(i++))
  {
    Ejection(pObj);
    Collection2(pObj);
  }
}

protected func GetFreeSeat()
{
	var count = 0;
	for(var i=0; i < GetLength(aChairs); i++)
		if(!Local(10+i)) return i;
}

protected func GetFreeSeats()
{
	var count = 0;
	for(var i=0; i < GetLength(aChairs); i++)
		if(!Local(10+i)) count++;
	return count;
}

protected func GetFreePlaces(fCountGarbage)
{
	var count = RackedItemCount();
	for(var i=0; i < RackedItemCount(); i++)
		if(Local(i) && (!Local(i)->~IsGarbage() || fCountGarbage)) count--;
	return count;
}

protected func RejectCollect(idObj, pObj)
  {
  // Verhindert Aufnahme aller nichtpassender Objekte
//  if(!ObjectCall(pObj,RackedItem())) return 1;
	if(pObj->GetAlive()) return 0;
  // Maximal 4 Stück
  if (!GetFreePlaces(1))//ContentsCount() >= RackedItemCount())
  {
		var pGarbage = 0;
		for(var i=0; i < RackedItemCount(); i++)
			if(Local(i) && Local(i)->~IsGarbage())
				pGarbage = Local(i);
		if(!pGarbage)
		{
			Message( "Voll!",this);
			return 1;
		}
		pGarbage->RemoveObject();
  }
  // Besitzer (Farbe) setzen
//  if (GetOwner(pObj) != GetOwner()) SetOwner(GetOwner(pObj));
  return 0;
  }
  
protected func Collection2(pObj)
  {
  if(pObj->~IsWarning()) return 1;
	if(pObj->GetAlive())
	{
		var i=-1;
		while(i < GetLength(aChairs) && (Local(++i+10) || !aChairs[i]));
		Local(10+i) = pObj;
		SetGraphics(0,this, 0, 5+i, GFXOV_MODE_Object, 0, 0, pObj);
		if(i == 0)
		{
			SetObjDrawTransform(1000, 0, -19000, 0, 1000, -6000, this, 5+i);
			pObj->SetAction("Sit");
			pObj->SetDir(0);
			pObj->SetObjDrawTransform(-1000, 0, 0, 0, 1000, 0);
		}
		if(i == 1)
		{
			SetObjDrawTransform(1000, 0, -5000, 0, 1000, -6000, this, 5+i);
			pObj->SetAction("Sit");
			pObj->SetDir(1);
			pObj->SetObjDrawTransform(-1000, 0, 0, 0, 1000, 0);
		}
		if(i == 2)
		{
			SetObjDrawTransform(1000, 0, +5000, 0, 1000, -6000, this, 5+i);
			pObj->SetAction("Sit");
			pObj->SetDir(1);
		}
		if(i == 3)
		{
			SetObjDrawTransform(1000, 0, +19000, 0, 1000, -6000, this, 5+i);
			pObj->SetAction("Sit");
			pObj->SetDir(0);
		}
		return 1;
	}
  Sound("RackPut");
  var i=-1;
  while(Local(++i));
  Local(i) = pObj;
  SetGraphics(0,this, 0, 10+i+1, GFXOV_MODE_Object, 0, 0, pObj);
	SetObjDrawTransform(1000, 0, (-13+9*i)*1000, 0, 1000, -5000-pObj->GetDefHeight()/2*1000, this, 10+i+1);
  pObj->~EnterTable(i);
  return 1;
  }
  
public func GetSeatPos(i)
{
	var pos = [-19,-5,5,19][i];
	return [GetX()+pos, GetY()-4];
}    
  
public func GetClonkPos(pObj)
{
	for(var i=0; i<GetLength(aChairs); i++)
		if(Local(10+i) == pObj) break;
	if(i == -1) return [GetX(), GetY()];
	return [-19,-5,5,19][i];
}  
  
public func GetPos(pObj)
{
	for(var i=RackedItemCount()-1; i; i--)
		if(Local(i) == pObj) break;
	if(i == -1) return [GetX(), GetY()];
	return [GetX()-13+9*i, GetY()];
}

protected func Ejection(pObj)
  {
  pObj->~LeaveTable();
  var i=-1;
  while(Local(++i)!=pObj && i<RackedItemCount());
	if(Local(i) == pObj)
	{
		SetGraphics(0,this, 0, 10+i+1, 0);
		Local(i) = 0;
	}
	i = -1;
  while(Local(++i+10)!=pObj && i<4);
	if(Local(10+i) == pObj)
	{
		SetGraphics(0,this, 0, i+5, 0);
		pObj->SetObjDrawTransform(1000, 0, 0, 0, 1000, 0);
		pObj->SetAction("Walk");
		if(pObj->FindContents(BO69))
			pObj->FindContents(BO69)->RemoveObject();
		pObj->SetPosition(GetSeatPos(i)[0], GetDefBottom()-pObj->GetDefHeight()/2);
		Local(10+i) = 0;
	}
  return 1;
  }

protected func ControlDig(pClonk)
  {
  // Gegenstand nehmen
  var pItem;
  for(var i=RackedItemCount()-1; i; i--)
    if(Local(i)) break;	  
  pItem = Local(i);
  if (!pItem) return 0;
  if (Collect(pItem, pClonk)) Sound("RackPut");
  return 1;
  }
  

local iState;
local aClonks;
local aClonksStates;

local aBookshelves;


func ExecActivity(pClonk)
{
	if(!aClonks) aClonks = [];
	if(GetIndexOf(pClonk, aClonks) >= 0) return;
	aClonks[GetLength(aClonks)] = pClonk;
}

func CancelActivity(pClonk)
{
	var index = GetIndexOf(pClonk, aClonks);
	if(index < 0) return; // nicht mehr in der Liste
//	pClonk->MessageEx("Ich muss weg Leute.", pClonk);
	if(index > 0)
		return DeleteArrayItem(index, aClonks);
	DeleteArrayItem(index, aClonks);
}

/*
func ExecActivity(pClonk)
{
	if(pClonk->Contained()!=this) pClonk->Enter(this);
}*/

func Timer()
{
	if(GetEffect("IntWait", this)) return 1;
	var i = -1;
	var j = 0;
	var iOutside = 0;
	if(!aClonks) aClonks = [];
	if(!aClonksStates) aClonksStates = [];
	for(var pClonk in aClonks)
	{
		if(pClonk && pClonk->GetLifeState() != 1)
			pClonk->FinishActivity();
	}
	for(var pClonk in aClonks)
	{
    if(!pClonk)
      continue;
		while(!aChairs[++i]);
		if(pClonk->Contained() != this)
		{
			var pos = GetSeatPos(i);
			if(pClonk->ExecMoveTo(pos[0], pos[1]))
			{
				pClonk->Enter(this);
				if(iState == 2)
					pClonk->SetAction("SitPlay");
				if(iState == 1)
				{
					if(!FindContents(BO69, pClonk)) pClonk->CreateContents(BO69);
					var pBook = FindContents(BO69, pClonk);
					pBook->SetGraphics("Read");
					if(i<2) pBook->SetObjDrawTransform(-1000, 0, 0, 0, 1000);
					else pBook->SetObjDrawTransform(1000, 0, 0, 0, 1000);
					pBook->SetAction("SitRead");
					pBook->SetColorDw(HSL(Random(256), 255, Random(128)+128));
					pClonk->SetAction("SitRead");
					pClonk->SetGraphics(0, 0, 0, 19, 5, 0, 0, pBook);
					pBook->SetDir(pClonk->GetDir());
				}
			}
			else
				iOutside++;
		}
		j++;
	}
	//var aClonks = FindObjects(Find_Container(this), Find_OCF(OCF_Alive));
	if(!GetLength(aClonks)) return 0;
	var index = Random(GetLength(aClonks));
	var pClonk = aClonks[index];
  if(iState == 2)
  {
    if(iLastPlayer >= GetLength(aClonks))
      iLastPlayer = 0;
    pClonk = aClonks[iLastPlayer];
    index = iLastPlayer;
    if(!pClonk) iLastPlayer += 1;
  }
  if(!pClonk) return 0;
	if(iState == 0 && !Random(12-GetLength(aClonks)) && pClonk->GetAction() == "Sit")
	{
		if(pClonk->GetLifeState() != 1)
		{
			pClonk->FinishActivity();
			return 1;
		}
		var pFood = FindObject2(Find_Container(this), Find_Func("IsFood"), Sort_Random());
		if(pFood)
		{
			pFood->Eat(pClonk);
			pClonk->SetAction("SitEat");
			var aMessages = ["hmm", "mjam", "njm"];
			var sMessage = aMessages[Random(GetLength(aMessages))];
			TalkMessage(sMessage, pClonk);
		}
	}
	if(iState == 1 && !Random(12-GetLength(aClonks)))
	{
		if(pClonk->GetLifeState() != 1)
		{
			pClonk->FinishActivity();
			return 1;
		}
		if(!Random(10) && aBookshelves)
		{
			var pShelve = aBookshelves[Random(GetLength(aBookshelves))];
			pClonk->Exit();
			pClonk->ExecMoveTo(pShelve->GetX(), pShelve->GetY());
			TalkMessage("nächstes Buch", pClonk);
		}
		else if(pClonk->GetAction() == "SitRead")
		{
			var pBook = FindContents(BO69, pClonk);
			pBook->SetAction("SitRead2");
			pClonk->SetAction("SitRead2");
		}
	}
	if(iState == 2 && !iOutside && !Random(6))// && !Random(12-GetLength(aClonks)))
	{
		if(pClonk->GetLifeState() != 1)
		{
			pClonk->FinishActivity();
			return 1;
		}
		if(pClonk->GetAction() == "SitPlay")
		{
      if(sNextText)
      {
        pClonk = sNextText[0];
        TalkMessage(sNextText[1], pClonk);
        if(GetLength(sNextText) == 2)
          sNextText = 0;
        else
          DeleteArrayItem(1, sNextText);
      }
      else {
        iLastPlayer += 1;
        if(!aCards) RestartGame();
        var iCard = CheckPlayCard(index);
        if(iCard != -1)
        {
          var iCardName = aCards[index][iCard];
          DeleteArrayItem2(iCard, aCards[index]);
          iLastCard = iCardName;
          
          pClonk->SetAction("SitPlay2");
          var aMessages = ["Herz", "Kreuz", "Karo", "Pik"];
          var aMessages2 = ["Ass", "König", "Dame", "Bube", "Zehn","Neun","Acht","Sieben","Sechs","Fünf","Vier","Drei","Zwei"];
          var aMessages2 = ["{{_CRD}}", "{{_CRD:1}}", "{{_CRD:2}}", "{{_CRD:3}}"];
    //      var aMessages = ["A", "K", "Q", "J", "10","9","8","7","6","5","4","3","2"];
          var aMessages = ["A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"];
          
          var sMessage = Format("<c %x>%s %s</c>", RGB(255,255,255), aMessages[iCardName%13], aMessages2[iCardName/13]);
          
          if(iLastCard%13 == 10) // Farbe wählen bei Bube
          {
            var iDesiredValue = GetDesiredColor(index);
            iLastCard = 10+13*iDesiredValue;
            sNextText = [pClonk, Format("Ich wünsche mir <c %x>%s</c>",RGB(255,255,255), aMessages2[iLastCard/13])];
          }
          if(GetLength(aCards[index])==1)
            sMessage = Format("%s|Mau!", sMessage);
          if(GetLength(aCards[index])==0)
          {
            sMessage = Format("%s|Mau Mau!", sMessage);
            RestartGame();
            sNextText = [pClonk, "Ich hab gewonnen!", Format("Du schuldest mir jetzt %d Bier!", Random(5)), "Neues Spiel."];
          }
          TalkMessage(sMessage, pClonk);
          }
        else
        {
          TalkMessage("Ich muss ziehen.", pClonk); 
          DrawCard(index);
        }
      }
		}
	}
}

func RestartGame()
{
  aCards = [[],[],[],[]];
  iLastCard = Random(52);
  for(var i = 0; i < 4; i++)
  {
    for(var j = 0; j < 7; j++)
    {
      DrawCard(i);
    }
  }
}

func DrawCard(i)
{
  aCards[i][GetLength(aCards[i])] = Random(52);
}

func CheckPlayCard(i)
{
  var iLastColor = iLastCard/13;
  var iLastValue = iLastCard%13;
  for(var j = 0; j < GetLength(aCards[i]); j++)
  {
    var iColor = aCards[i][j]/13;
    var iValue = aCards[i][j]%13;
    if(iColor == iLastColor || iLastValue == iValue)
      return j;
  }
  return -1;
}

func GetDesiredColor(i)
{
  var iColors = [0,0,0,0];
  for(var j = 0; j < GetLength(aCards[i]); j++)
  {
    var iColor = aCards[i][j]/13;
    var iValue = aCards[i][j]%13;
    iColors[iColor]++;
  }
  var iMax = -1;
  var iMaxIndex = -1;
  for(var i = 0; i < 4; i++)
  {
    if(iColors[i] > iMax)
    {
      iMax = iColors[i];
      iMaxIndex = i;
    }
  }
  return iMaxIndex;
}

func TalkMessage(sMessage, pClonk)
{
	var dwColor = GetColorDw(pClonk); if (!dwColor) dwColor=255;
	dwColor = MakeReadableColor(dwColor);
	if (!pClonk) dwColor=16777215;
	sMessage = ColorizeString(Format("%s:|%s", GetName(pClonk), sMessage), dwColor);
  
  var pLayer = GetObjectLayer();
  var pDouble = 0;
  if(pLayer) pDouble = pLayer->LocalN("pDouble");
  if( pLayer && pDouble != this)
  {
    var X = GetX()-pLayer->GetX();
    var Y = GetY()-pLayer->GetY();
    return CustomMessage (sMessage, pDouble, -1, GetClonkPos(pClonk)+X, -5+Y+pLayer->GetDefHeight()/2-GetDefHeight()/2, 0, 0, 0, MSG_Multiple); 
  }
  
	CustomMessage(sMessage, this, -1, GetClonkPos(pClonk), -5);
}

func CancelActivity(pClonk)
{
	var index = GetIndexOf(pClonk, aClonks);
	if(index < 0) return; // nicht mehr in der Liste
	pClonk->MessageEx("Ich muss weg Leute.", pClonk);
	if(index > 0)
		return DeleteArrayItem(index, aClonks);
	DeleteArrayItem(index, aClonks);
	iState = 0;
}

func CancelActivity(pClonk)
{
	var index = GetIndexOf(pClonk, aClonks);
	if(index < 0) return; // nicht mehr in der Liste
	
	DeleteArrayItem(index, aClonks);
	
	if(pClonk->Contained()!=this) return;
	pClonk->Exit();
//	pClonk->SetPosition(GetX()+GetClonkPos(pClonk), GetDefBottom()-pClonk->GetDefHeight()/2);
}

