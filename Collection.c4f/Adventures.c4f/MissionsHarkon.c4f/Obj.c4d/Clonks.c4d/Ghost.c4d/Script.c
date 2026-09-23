/*-- Geist --*/

#strict 2

local idName;
local fFly;

static g_foundGhosts;
static g_foundGhostsSaved;

func Initialize()
{
	SetVisibility(VIS_None);
	AddEffect("IntTimer", this, 1, 10, this);
}

func FxIntTimerTimer()
{
	var pObj = Contained();
	var pObj2 = 0;
	if(pObj) pObj2 = pObj->Contained();
	if(pObj2 && pObj2->~IsClonk())
	{
		Activate();
		return -1;
	}
	if(!pObj)
	{
		var pObj = FindObject2(Find_ID(ASAS), Find_PlayerType(C4PT_User), Find_Distance(100));
		if(pObj)
		{
			Activate();
			return -1;
		}
	}
}

func Activate(szNewName)
{
	if(!idName) idName = szNewName;
	Exit();
	RemoveEffect("IntTimer", this);
	AddEffect("IntFlyUp", this, 1, 5, this);
	CastObjects(SPRK, 10, 20);
	SetVisibility(VIS_All);
	SetComDir(COMD_Up);
	SetAction("Fly");
	SetPosition(GetX(), GetY());
	Sound("GhostFree.ogg", this);
	DialogMessage([GetID(), Format("$Ghost01$", GetMyName()), "1", RGB(200,200,255)], Format("$Ghost02$", GetMyName(1), GetMyName()), "Nothing");
	Sound(Format("%d_%s.ogg", idName, GetMyName()), GetCursor(GetPlayerByIndex(0, C4PT_User)));
	if(!g_foundGhosts) g_foundGhosts = [];
	g_foundGhosts[GetLength(g_foundGhosts)] = idName;
}

func Nothing() {}

func FxIntFlyUpTimer(pThis, iNumber, iTime)
{
	SetClrModulation(RGBa(255,255,255,Min(iTime/2,128)));
	SetObjDrawTransform(1300-Cos(iTime*5, 300), 0, 0, 0, 1300+Cos(iTime*5, 300), 0);
	CastParticles("PSpark", 5, 10,-Cos(iTime*5, 20), -10, 30,60,0,RGB(255,255,255));
	CastParticles("PSpark", 5, 10, Cos(iTime*5, 20), -10, 30,60,0,RGB(255,255,255));
	if(GetY() < -50)
	{
		RemoveObject();
	}
}

func GetEvaluationText()
{
	// Neue Geister speichern
	if(g_foundGhosts)
	for(var idName in g_foundGhosts)
	{
		for(var iPlrIndex = 0; iPlrIndex < GetPlayerCount(C4PT_User); iPlrIndex++)
		{
			var iPlr = GetPlayerByIndex(iPlrIndex, C4PT_User);
			SetPlrExtraData(iPlr, Format("Harkon_G%d", idName), 1);
		}
		var szMessage = Format("$Ghost03$", GetGhostName(idName));
		AddEvaluationData(szMessage);
		Log(szMessage);
	}
	
	var aGhosts = GetGhostsOfChapter(g_chapter);
	var aFoundGhosts = [];
	var iCount = 0;
	for(var iName in aGhosts)
	{
		var iPlr = GetPlayerByIndex(0, C4PT_User);
		if(GetPlrExtraData(iPlr, Format("Harkon_G%d", iName)))
		{
			aFoundGhosts[GetLength(aFoundGhosts)] = GetGhostName(iName);
			iCount++;
		}
	}
	if(!iCount) return;
	var szGhosts = aFoundGhosts[0];
	for(var i = 1; i < GetLength(aFoundGhosts); i++)
	{
		if(i == GetLength(aFoundGhosts)-1)
			szGhosts = Format("$Ghost04$", szGhosts, aFoundGhosts[i]);
		else
			szGhosts = Format("%s, %s", szGhosts, aFoundGhosts[i]);
	}
	var szMessage1 = Format("$Ghost05$", iCount, GetLength(aGhosts), g_chapter);
	var szMessage2 = Format("$Ghost06$", szGhosts);
	AddEvaluationData(szMessage1);
	AddEvaluationData(szMessage2);
	Log(szMessage1);
	Log(szMessage2);
}

func GetGhostsOfChapter(iChapter)
{
	if(iChapter == 1)
		return [111, 112, 121, 131, 141, 151, 152];
	if(iChapter == 2)
		return [271];
}

func GetMyName(fDesc)
{
	return GetGhostName(idName, fDesc);
}
	
func GetGhostName(idName, fDesc)
{
	if(idName == 271)
	{
		if(!fDesc) return "Os";
		return "$Ghost07$";
	}
	if(idName == 111)
	{
		if(!fDesc) return "Marog";
		return "$Ghost08$";
	}
	if(idName == 112)
	{
		if(!fDesc) return "Maroga";
		return "$Ghost09$";
	}
	if(idName == 121)
	{
		if(!fDesc) return "Posipex";
		return "$Ghost10$";
	}
	if(idName == 131)
	{
		if(!fDesc) return "Mortum";
		return "$Ghost11$";
	}
	if(idName == 141)
	{
		if(!fDesc) return "Ralum";
		return "$Ghost12$";
	}
	if(idName == 151)
	{
		if(!fDesc) return "Fruor";
		return "$Ghost13$";
	}
	if(idName == 152)
	{
		if(!fDesc) return "Malolch";
		return "$Ghost14$";
	}
}