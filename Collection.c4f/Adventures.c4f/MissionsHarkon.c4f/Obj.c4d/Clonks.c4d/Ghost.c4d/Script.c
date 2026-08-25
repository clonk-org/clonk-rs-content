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
	DialogMessage([GetID(), Format("Geist %s", GetMyName()), "1", RGB(200,200,255)], Format("%s||<i>Der Geist von %s wurde befreit</i>", GetMyName(1), GetMyName()), "Nothing");
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
		var szMessage = Format("<i>Der Geist von %s wurde befreit</i>", GetGhostName(idName));
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
			szGhosts = Format("%s und %s", szGhosts, aFoundGhosts[i]);
		else
			szGhosts = Format("%s, %s", szGhosts, aFoundGhosts[i]);
	}
	var szMessage1 = Format("%d von %d Geistern im %d. Kapitel gefunden.", iCount, GetLength(aGhosts), g_chapter);
	var szMessage2 = Format("Die Geister von %s wurden bisher befreit.", szGhosts);
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
		return "Os Seele, die litt große Not,|für seine Zukunft sah er rot.|  er wollte endlich gehen|  und nicht mehr auf dieser Erde stehen,|denn sein Körper war schon lange tot.";
	}
	if(idName == 111)
	{
		if(!fDesc) return "Marog";
		return "Marog lebte einst in diesem Tal,|ob Sonn ob Wind, war ihm egal,|  sein Haus das hat er stets beschützt,|  am End hats ihm dann doch nix genützt,|den er erlag der Niederlage Qual.";
	}
	if(idName == 112)
	{
		if(!fDesc) return "Maroga";
		return "Maroga war seine Frau und Olrogs Mutter,|buk Kuchen, kochte Braten, stampfte Butter|  keine Arbeit die hat sie gescheut,|  Doch warum lebt sie nicht auch heut?|Sie starb, den Zaps wurde sie ein Futter.";
	}
	if(idName == 121)
	{
		if(!fDesc) return "Posipex";
		return "Posipex ist so manch ein Fang gelungen,|hat oft am Fluss ein Lied gesungen,|  die Angelrute fest in seiner Hand,|  zog er auch diesen Fisch an Land.|Da hat der Fisch ihn schwups verschlungen.";
	}
	if(idName == 131)
	{
		if(!fDesc) return "Mortum";
		return "Ein Monster ganz aus festem Stein,|das fing Mortums kleine Seele ein,|  nie konnte er von dort wieder gehen,|  niemand hat ihn je dannach gesehen,|Sollte dass dann nun das Ende sein?";
	}
	if(idName == 141)
	{
		if(!fDesc) return "Ralum";
		return "In einem Zug reist er durch Raum und Zeit,|so besucht Ralum die Welt so weit,|  Doch der Zug ist stehen geblieben.|  Vor acht Jahren, oder sieben.|Und Ralum doch kein End von seinem Leid.";
	}
	if(idName == 151)
	{
		if(!fDesc) return "Fruor";
		return "Fruor war eine sehr gemeine Schlange.|Doch hat man ihn eingesperrt sehr lange,|  nie brachte man ihm eine Speise,|  oder kümmerte sich auf andere Weise,|so wurd selbst seiner Seele bange.";
	}
	if(idName == 152)
	{
		if(!fDesc) return "Malolch";
		return "Ja er war ein kleiner Strolch|spielte gerne Streicher, der Malolch|  nichts ist ihm heilig geblieben|  doch heut hat er es übertrieben|und spürte dann schnell den Dolch.";
	}
}