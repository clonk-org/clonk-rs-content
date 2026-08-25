/*-- Musik --*/

#strict 2

//func Initialize() { Log("InitMusic"); SetGraphics(0, this, GIDL, 1, 1); }

func DoAttach()
{
	var iOffsetX = 0, iOffsetY = 0;
	var pTarget = GetCursor(GetOwner());
	if(g_pCamera) pTarget = g_pCamera;
	if(!pTarget) { return RemoveObject(); }
	var pLayer;
	var iX = pTarget->GetX(); var iY = pTarget->GetY();
	if(pLayer = pTarget->GetObjectLayer())
	{
		iX += pLayer->GetLayerOffset();
		iY += pLayer->GetLayerOffset(1);
	}
	SetPosition(iX, iY);
//	Message("!", this);
}

global func GetMusicLength(szName)
{
	var aListe = [
	["Epic Unease.ogg", 37], // Juncus
	["Colossus.ogg", 4*60+30], // Crows, Canyon
	["Dark Walk.ogg", 1*60+25],
	["Final Count.ogg", 3*60+23],
	["Halls of the Undead.ogg", 4*60+52],
	["Midnight Meeting.ogg", 50],
	["Serpentine Trek.ogg", 1*60+46],
	["Tabuk.ogg", 2*60+38], // Orientalic, Kaftlak
	["The Forest and the Trees.ogg", 1*60+34], // Joyfull forest
	["The Pyre.ogg", 3*60+53],	// Intro, Rockfort
	
	["Breaking the siege.ogg", 3*60+29],
	["Cavern of Time.ogg", 3*60+20],
	["Chee Zee Cave.ogg", 3*60+51],
	["For the Fallen.ogg", 1*60+35],
	["Heart of Medieval.ogg", 4*60+32],
	["La busqueda de Ianna.ogg", 2*60+37],
	["Megalo Spirit Village.ogg", 1*60+39],
	["Moon waltz.ogg", 3*60+58],
	["No one Escape.ogg", 58],
	["Pactum serva.ogg", 3*60+49],
	["Ruins.ogg", 47],
	["Strange Vision.ogg", 1*60+45],
	["The fall of Gilead.ogg", 3*60+31],
	["The Last Mission.ogg", 59],
	["The Shining City.ogg", 1*60+47],
	["The Silver Tree.ogg", 1*60+12],
	];
	for(var aTest in aListe)
		if(aTest[0] == szName) return aTest[1];
}

/*------------- List of used Music -------------//
Mission1A:
Intro: The Pyre.ogg
FirstPart: Midnight Meeting.ogg The Forest and the Trees.ogg
AttacK: Final Count.ogg
End: Epic Unease.ogg

Mission1B:
Rockfort: The Pyre.ogg
Lager: The Forest and the Trees.ogg
End: Epic Unease.ogg

Mission1C:
Intro: The Pyre.ogg
Kanderia: Tabuk.ogg
Krähen: Colossus.ogg
End: Epic Unease.ogg

Mission1D:
Rockfort: The Pyre.ogg
Canyon: Colossus.ogg
End: Epic Unease.ogg

Mission1E:
Rockfort: The Pyre.ogg
Außenposten: Final Count.ogg
End: Epic Unease.ogg
//----------------------------------------------*/

static g_pMusicObject;
static g_aMusicList;
static g_iMusicLastIndex;
static g_szMusic;

global func DoPlayMusic(szSound)
{/*
  var strList = "";
  if(GetType(szSound) == C4V_Array)
  {
    for(var szMusic in szSound)
    {
      strList = Format("%s;%s", strList, szMusic);
    }
  }
  else
    strList = szSound;
  Log("MUSIC: %s", strList);
  SetPlayList(strList, 1);
  return 1;*/
  //return 0;
	SetPlayList("none");
	if(!g_pMusicObject) g_pMusicObject = [];
	
	if(GetType(szSound) == C4V_Array)
	{
		g_aMusicList = szSound;
		szSound = szSound[0];
		g_iMusicLastIndex = 0;
	}
	else g_aMusicList = 0;
	RemoveEffect("ChangeSound");
	var iLength = GetMusicLength(szSound);
	if(g_aMusicList) AddEffect("ChangeSound", 0, 1, iLength*36, 0);
	
//	Music(g_lastMusic, 0);
	MyMusic(szSound);
}

global func FxChangeSoundTimer()
{
	var index = Random(GetLength(g_aMusicList)-1);
	if(index >= g_iMusicLastIndex)
		index += 1;
	g_iMusicLastIndex = index;
	var szSound = g_aMusicList[index];
	MyMusic(szSound);
	
	var iLength = GetMusicLength(szSound);
	AddEffect("ChangeSound", 0, 1, iLength*36, 0);
	return -1;
}

global func MyMusic(szSound)
{
	g_szMusic = szSound;
	var iLength = GetMusicLength(szSound);
	if(iLength)
	{
		Log("Music: %s", szSound);
	}
	else
		Log("ERROR: Music %s not found!", szSound);
	
	//for(pObj in g_pMusicObject) if(pObj) { pObj->SetObjectStatus(1); pObj->DoAttach(); pObj->FinishSound();}
		if(szSound)
		{
			for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
			{
				var iPlr = GetPlayerByIndex(i, C4PT_User);
        if(g_pMusicObject[iPlr])
        {
          if(g_pMusicObject[iPlr]->LocalN("mysound") == szSound)
            continue;
          g_pMusicObject[iPlr]->SetObjectStatus(1); g_pMusicObject[iPlr]->DoAttach(); g_pMusicObject[iPlr]->FinishSound();
        }
				g_pMusicObject[iPlr] = CreateObject(_MUS, 0, 0, iPlr);
				g_pMusicObject[iPlr]->SetSound(szSound, GetHiRank(iPlr));
			}
		}
	if(!GetEffect("IntCheckNewPlayer"))
	{
		AddEffect("IntCheckNewPlayer", 0, 1, 1, 0, _MUS);
	}
}

func FxIntCheckNewPlayerTimer()
{
	if(!g_szMusic) return;
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var pMusic = FindObject2(Find_ID(_MUS), Find_Owner(GetPlayerByIndex(i,C4PT_User)));
		if(pMusic) continue;
    if(g_pMusicObject[iPlr]) continue;
		var iPlr = GetPlayerByIndex(i, C4PT_User);
		g_pMusicObject[iPlr] = CreateObject(_MUS, 0, 0, iPlr);
		g_pMusicObject[iPlr]->SetSound(g_szMusic, GetHiRank(iPlr));
	}
}

func Timer()
{
//	if(GetAction() != "MoveAway")
//		SetPosition(GetX(GetCursor(GetOwner())), GetY(GetCursor(GetOwner())));
	if(GetAction() == "Idle")
		SetAction("Speak", GetCursor(GetOwner()));
}

local mysound;

func FinishSound()
{
	SetAction("MoveAway");
  ScheduleCall(this, "DoFinishSound", 35*60-1, 1);
	ScheduleCall(this, "DoFinishSound2", 35*60, 1);
}
func MoveAway()
{
	SetPosition(GetX(), GetY()-100);
  if(GetActTime() > 35)
    DoFinishSound();
}
func DoFinishSound()
{
	SetPosition(GetX(), -100*35*60*100);
  Sound(mysound, 0, this, 50, GetOwner()+1, -1, 0, 1000);
	RemoveObject();
}

func SetSound(szSound, pSpeaker)
{
//	SetAction("Speak", this);
	SetPosition(GetX(pSpeaker), GetY(pSpeaker));
	mysound = szSound;
  Sound(mysound, 0, this, 50, GetOwner()+1, 1, 0, 1000);
  SetObjectStatus(2);
}

func DoSetSound(szSound)
{
	Sound(mysound, 0, this, 50, GetOwner()+1, 1, 0, 1000);
  SetObjectStatus(2);
  return;
}
