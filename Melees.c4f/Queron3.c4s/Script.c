/*-- Queron 3 --*/

// script v3.91 by miniwipf
#strict 2

/* Player */

static const C4P_Host = 0;

static g_aClonksOnRelaunch; // Relaunchte Clonks
static g_iRelaunchNumber; // Anzahl Relaunches
static g_fGameOptionsDone; // Einstellungen abgeschlossen

static const VIEW_range = 500;

/* Supplies */

static aChestIndex, aSpecialChestIndex;

static aExclusiveSpells, aSeldomSpells, aUsualSpells;


/*--------------------------------/ ENGINE /----------------------------------*/

protected func Initialize()
{
	// DEBUG: Keine zusätzlichen Objekte erzeugen
	if (GetScenarioVal("NoInitialize", "Head")) { Log("DEBUG: NoInitialize"); return; }
	
	// Konstanten
	aExclusiveSpells = [MGHL, MINV, GGHG, MARK, ELX2, CPAN, CAHE, AUHE];
	aSeldomSpells = [FRFS, EXTG, MMTR, MSSH, MFWL, MDBT, MFRB, MLGT, AFST, GZ9Z, MGRP];
	aUsualSpells = [ELX1, ABLA, CMFG, MBRG, MDFL, MSAS];
	
	CreateObject(SBRD); // Scoreboard-Objekt
	PrepareLandscape();
	CreateConstructions();
	GenerateCharacters();
	PlaceItems();
	// Stille, man hört nur Wind..
	SetPlayList(";");
	// Statics initialisieren
	g_aClonksOnRelaunch = [KNIG, ASAS, PLDN, MAGE, KING];
	g_iRelaunchNumber = GetLength(g_aClonksOnRelaunch)-1;
	g_fGameOptionsDone = 0;
	
	ScriptStart();
	return 1;
}

/*/******************************** Kistenfunktionen **************************/



global func SetSpells(object pScroll)
{
	if (!pScroll && this) pScroll = this;
	// Bei der Schriftrolle können den Zauber einfach so draufdrucken
	if (GetID(pScroll) == SCRL)
		pScroll -> SetSpell(RndSpellID(pScroll));
	// Auf dem Buch hingegen wollen wir die Zauber schön nebeneinander und möglichst zentral ablegen
	else if (GetID(pScroll) == BOOK) {
		for (var i=5, n, b; i; i--) {
			if (n == 0) b = 2;
			if (n == 1) b = 1;
			if (n == 2) b = 3;
			if (n == 3) b = 0;
			if (n == 4) b = 4;
			pScroll -> SetSpell(RndSpellID(pScroll), b);
			++n;
		}
	}
	else return;
	// Fertig belegt
	return 1;
}

global func RndSpellID(object pScroll)
{
	// Exklusiv
	if (!Random(8)) return aExclusiveSpells[Random(GetLength(aExclusiveSpells))];
	// Selten
	else if (!Random(3)) return aSeldomSpells[Random(GetLength(aSeldomSpells))];
	// üblich
	else return aUsualSpells[Random(GetLength(aUsualSpells))];
}

global func GetSpellRank(id idSpell)
	{
	var f = 0;
	for (var i=0; i < GetLength(aUsualSpells); i++)
		if (aUsualSpells[i] == idSpell) return 1;
	for (var i=0; i < GetLength(aSeldomSpells); i++)
		if (aSeldomSpells[i] == idSpell) return 2;
	for (var i=0; i < GetLength(aExclusiveSpells); i++)
		if (aExclusiveSpells[i] == idSpell) return 3;
	return 0; // kein Zauber von Uns
	}

global func RndChest() { return aChestIndex[Random(GetLength(aChestIndex))]; }

global func RndSpecialChest() { return aSpecialChestIndex[Random(GetLength(aSpecialChestIndex))]; }

global func ChestContents(id idItem)
{
	var cnt;
	for (chest in aChestIndex) cnt += ContentsCount(idItem, chest);
	return cnt;
}

global func SpecialChestContents(id idItem)
{
	var cnt;
	for (chest in aSpecialChestIndex) cnt += ContentsCount(idItem, chest);
	return cnt;
}

global func CreateMatchingKey(id idKey, object pLock, object pContainer)
{
	if (!pContainer && this) pContainer = this;
	var key = CreateContents(idKey, pContainer);
	key->SetRefID(pLock->GetRefID());
	return key;
}

/********************************** KISTEN ************************************/

func ScriptStart()
{
	// Kisten index für RndChest()
	aChestIndex = FindObjects(Find_ID(CHST));
	aSpecialChestIndex = FindObjects(Find_ID(_CST));

	// Einmalige Items
	
	var racks, rack;
	// 3 Bögen
	racks = FindObjects(Find_ID(BWRC));
	var i = 3;
	while (i)
	{
		rack = racks[Random(GetLength(racks))];
		if (rack->ContentsCount() < 3)
		{
			rack -> CreateContents(BOW1);
			
		}
		i--;
	}
	// 13 Speere
	racks = FindObjects(Find_ID(SPRC));
	var i = 13;
	while (i)
	{
		rack = racks[Random(GetLength(racks))];
		if (rack->ContentsCount() < 5)
		{
			rack -> CreateContents(SPER);
			i--;
		}
	}
	// 4 Schwerter
	racks = FindObjects(Find_ID(SWRC));
	var i = 4;
	while (i)
	{
		rack = racks[Random(GetLength(racks))];
		if (rack->ContentsCount() < 5)
		{
			rack -> CreateContents(SWOR);
			i--;
		}
	}
	// 4 Schilde
	racks = FindObjects(Find_ID(SHRC));
	var i = 4;
	while (i)
	{
		rack = racks[Random(GetLength(racks))];
		if (rack->ContentsCount() < 5)
		{
			rack -> CreateContents(SHIE);
			i--;
		}
	}
	// 26 Holz
	racks = FindObjects(Find_ID(WODC));
	var i = 26;
	while (i)
	{
		rack = racks[Random(GetLength(racks))];
		if (rack->ContentsCount() < 9)
		{
			rack -> CreateContents(WOOD);
			i--;
		}
	}
	// Kiste bei den Kristallen rechts unten
	Object(174)->CreateContents(CRYS,, 2); // Kristall
	Object(174)->CreateContents(HLGN); // Holigoon
	// Kiste im abgeschlossenen Keller
	Object(455)->CreateContents(BHLG); // Holigoon geschliffen
	Object(455)->CreateContents(GOLD); // Gold
	Object(455)->CreateContents(GBLT); // Kelch
	Object(455)->CreateContents(_TAR); // Twonkys Pfeil
	Object(455)->CreateContents(MBAG); // Geldsack
	Object(455)->CreateContents(BRED); // Brot
	// Kiste hinter Thron
	Object(177)->CreateContents(GOLD,, 3);
	// Geheimes Erz auf dem Dach des rechten Turms
	CreateObject(ORE1, 995, 0, NO_OWNER);
	// Schlüssel zu Keller verstecken
	if (!Random(5)) RndSpecialChest()->CreateMatchingKey(_KEY, LocalN("pLockRight", FindObject(PPT5)));
	else RndChest()->CreateMatchingKey(_KEY, LocalN("pLockRight", FindObject(PPT5)));
	// Käfigschlüssel verstecken
	if (!Random(5)) RndSpecialChest()->CreateMatchingKey(_SKE, Object(348));
	else RndChest()->CreateMatchingKey(_SKE, Object(348));
	// 2 Muscheln
	PlaceObjects(SHEL,2,"GBackLiquid", 230, 900, 470-230, 970-900);
	
	// Alle Kisten füllen
	Script0();
	Script10();
	Script20();
	Script30();
	Script40();
	Script50();
	Script60();
	Script70();
	Script80();
	Script90();
	Script100();
	goto(110);
	
	// und ab jetzt für nachschub sorgen
	ScriptGo(1);
}

// Nahrung
func Script0()
{
	if (ChestContents(COKI) > 2) return;
	// Die Prinzessin hat immer was zu knabbern
	Object(183)->CreateContents(COKI,, 2);
	// und im Wald hat ein Jäger nen Vogel geschossen
	Object(175)->CreateContents(CBRD);
	// Sonst noch 2 Kekse irgendwo
	RndChest()->CreateContents(COKI);
	RndChest()->CreateContents(COKI);
}

// Küche
func Script10()
{
	if (ChestContents(DOGH) > 2) return;
	// 2 Teige + 1 Brot in die Küche
	Object(192)->CreateContents(DOGH);
	Object(192)->CreateContents(BRED);
	// Sonstwo liegt noch ein Teig rum
	RndChest()->CreateContents(DOGH);
}

// Anvil
func Script20()
{
	if (Object(184)->ContentsCount(METL) < 3)
	{
		// 3 Eisen in die Kisten daneben
		Object(184)->CreateContents(METL,, 3);
	}
	if (Object(184)->ContentsCount(COAL) < 3)
	{
		Object(184)->CreateContents(COAL,, 5);
	}
	if (Object(184)->ContentsCount(GUNP) < 1)
	{
		Object(184)->CreateContents(GUNP);
	}
	if (ChestContents(SPHR) < 2)
	{
		Object(184)->CreateContents(SPHR); // Schwefel
		RndChest()->CreateContents(SPHR); // und 1 noch inder Welt verteilen
	}
}

// Heiltrank, Immunitätstank & Wipftrank
func Script30()
{
	// Heiltrank
	if (ChestContents(PHEA) < 1) RndChest()->CreateContents(PHEA);
	// Immunitätstrank
	if (ChestContents(PIMM) < 1) RndChest()->CreateContents(PIMM);
	// Heil-, Immunitäts- & Wipftränke in den Spezialkisten
	if (SpecialChestContents(PHEA) < 1) RndSpecialChest()->CreateContents(PHEA);
	if (SpecialChestContents(PIMM) < 1) RndSpecialChest()->CreateContents(PIMM);
	if (SpecialChestContents(PWIP) < 1) RndSpecialChest()->CreateContents(PWIP);
}

// Fischtränke & Manatränke
func Script40()
{
	// Fischtränke
	if (ChestContents(PFIS) < 1)
	{
		// Neben Flintwerfer
		Object(172)->CreateContents(PFIS);
	}
	// Manatränke
	if (ChestContents(PMAN) < 5)
	{
		// + 6 Manatränke verteilen
		for (var i=0; i < 5; i++) RndChest()->CreateContents(PMAN);
		// Einer immer in der Bibliothek
		Object(197)->CreateContents(PMAN);
	}
	// Fisch- & Manatränke in Spezialkisten
	if (SpecialChestContents(PFIS) < 1) RndSpecialChest()->CreateContents(PFIS);
	if (SpecialChestContents(PMAN) < 3) RndSpecialChest()->CreateContents(PMAN);
}

// Pfeilpakete
func Script50()
{
	if (ChestContents(ARWP) < 3)
	{
		// Auf dem Turm links ist immer eins
		Object(182)->CreateContents(ARWP);
		// Sonst noch 2 irgendwo 
		RndChest()->CreateContents(ARWP);
		RndChest()->CreateContents(ARWP);
	}
	if (ChestContents(XARP)<1 && ChestContents(FARP)<1)
	{
		// +  Spezielle Pfeile für Würze
		RndChest()->CreateContents([XARP, FARP][Random(2)]);
	}
}

// Flints
func Script60()
{
	// Flints
	if (ChestContents(FLNT) < 5)
	{
		// Auf dem Turm sind immer zwei zum runterwerfen. Weils Spass macht!
		Object(182)->CreateContents(FLNT,, 2);
		// Zufällig 5 weitere Feuersteine und 3 T-Flints verteilen
		for (var i=0; i < 5; i++) RndChest()->CreateContents(FLNT);

	}
	// T-Flints
	if (ChestContents(TFLN) < 3)
	{
		// Zufällig 4 T-Flints verteilen
		for (var i=0; i < 4; i++) RndChest()->CreateContents(TFLN);
	}
	// Super-T-Flints / Superflints
	if (ChestContents(SFLN) < 3)
	{
		for (var i=0; i < 2; i++) RndChest()->CreateContents([SFLN, STFN][Random(2)]); // Entweder oder
	}
	if (ChestContents(ARPD) < 2)
	{
		Object(171)->CreateContents(ARPD); // Unten links in der Höhle gibts immer ein Airpedo
		RndChest()->CreateContents(ARPD); // zufällig noch eines
	}
	if (ChestContents(FBMP) < 1)
	{
		Object(176)->CreateContents(FBMP); // Kiste in der Mitte: Brandbombe
	}
}

// Steintränke, Monstertränke & Feuertränke
func Script70()
{
	// Steintränke
	if (ChestContents(PSTO) < 1) RndChest()->CreateContents(PSTO);
	// Stein-,Monster- & Feuertränke in Spezialkisten
	if (SpecialChestContents(PSTO) < 1) RndSpecialChest()->CreateContents(PSTO);
	if (SpecialChestContents(PMON) < 1) RndSpecialChest()->CreateContents(PMON);
	if (SpecialChestContents(PFIR) < 1) RndSpecialChest()->CreateContents(PFIR);
}

// Zauberbücher und Rollen
func Script80()
{
	// Tarnen in der Burgkiste mitte
	var scroll = FindContents(SCRL, Object(176));
	if (!scroll || LocalN("idSpell", scroll)!=CMFG) CreateContents(SCRL, Object(176))->SetSpell(CMFG);
	// Fischhaut in der Kiste beim und im See
	scroll = FindContents(SCRL, Object(171));
	if (!scroll || LocalN("idSpell", scroll)!=FHSK) CreateContents(SCRL, Object(171))->SetSpell(FHSK);
	if (ChestContents(SCRL) < 5)
	{
		// + 5 Schriftrollen
		for (var i=0; i < 5; i++)
		{
			scroll = RndChest()->CreateContents(SCRL);
			scroll -> SetSpells();
		}
		// eine ist immer im Zelt zum Assas aufdecken
		if (FindObject(TENT)) {
			scroll = FindObject(TENT)->CreateContents(SCRL);
			scroll -> SetSpell(MSAS);
		}
		// In der Bibliothek hat es noch eine
		scroll = Object(197)->CreateContents(SCRL);
		scroll -> SetSpells();
	}
	if (ChestContents(BOOK) < 2)
	{
		var book;
		// Zufall
		book = RndChest()->CreateContents(BOOK);
		book -> SetSpells();
		// In der Kiste im Schlund
		book = Object(181)->CreateContents(BOOK);
		book -> SetSpells();
		// Unter dem Magier
		book = Object(180)->CreateContents(BOOK);
		book -> SetSpells();
	}
}

// Balloonpacks
func Script90()
{
	if (ChestContents(BPCK) < 2)
	{
		Object(182)->CreateContents(BPCK);
		Object(175)->CreateContents(BPCK);
		RndChest()->CreateContents(BPCK);
	}
}

// MONEY MONEY MONEY
func Script100()
{
	if (ChestContents(MBAG) < 3)
	{
		// 4 money bags
		for (var i=0; i < 6; i++) RndChest()->CreateContents(MBAG);
	}
}

func Script500()
{
	var flags = FindObjects(Find_ID(BANR));
	if (GetLength(flags) > 2)
	{
		var owner = GetOwner(flags[0]), b=0;
		for (var i=1; i < GetLength(flags); i++)
			if (owner != GetOwner(flags[i])) { b=1; break; }
		if (!b) DoWealth(owner, 1);
	}
	// Neu Pflanzen
	PlaceVegetation(BSH2, 1030, 630, 1220-1030, 700-630, 1);
	goto(0);
} // alles von vorne


/****************************** SUPPLIES **************************************/

global func SpreadClonkSupplies()
{
	// + 4 Flints
	for (var i=0; i < 4; i++) RndChest()->CreateContents(FLNT);
	// + 2 T-Flints und 1 Superflint
	RndChest()->CreateContents(TFLN);
	RndChest()->CreateContents(TFLN);
	RndChest()->CreateContents(SFLN);
	// + Bumerang oder Tera
	if (Random(2)) RndChest()->CreateContents(_BMR);
	else RndChest()->CreateContents(EFLN);
}

global func SpreadKnightSupplies()
{
	// Sicher 2 normale Pfeilpakete
	for (var i=0; i < 2; i++) RndChest()->CreateContents(ARWP);
	// + 1 Brand und 1 Sprengpfeilpaket
	if (!Random(3)) RndChest()->CreateContents(XARP);
}

global func SpreadPaladinSupplies()
{
	// Sicher 2 normale Pfeilpakete
	for (var i=0; i < 2; i++) RndChest()->CreateContents(ARWP);
	// + vielleicht 1 Sprengpfeilpaket oder Tera
	if (!Random(3)) RndChest()->CreateContents(XARP);
	else RndChest()->CreateContents(EFLN);
	// Und auch noch Munition dafür
	for (var i=0; i < 3; i++) RndChest()->CreateContents(BLTP);
	RndChest()->CreateContents(SBLP);
	// Vielleicht auch noch giftige oder Weihwasser
	if (Random(2)) RndChest()->CreateContents(PBLP);
	else RndChest()->CreateContents(HLWT);
}

global func SpreadMageSupplies()
{
	// Manatränke
	for (var i=0; i < 4; i++) RndChest()->CreateContents(PMAN);
}

global func SpreadAssaSupplies()
{
	// Bolzen x2
	for (var i=0; i < 2; i++) RndChest()->CreateContents(BOTP);
	// Selten auch Nadelbolzen
	if (!Random(4)) RndChest()->CreateContents(NBTB);
	// + Auch ein Messer
	RndChest()->CreateContents(KNFP);
}

global func SpreadKendarianSupplies()
{
	// Sicher 2 normale Pfeilpakete
	for (var i=0; i < 2; i++) RndChest()->CreateContents(ARWP);
	// + 1 Sprengpfeilpaket
	if (!Random(3)) RndChest()->CreateContents(XARP);
}

global func SpreadMysticSupplies()
{
	// Ein paar Kristalle
	RndChest()->CreateContents(CRYS);
	// Nix besonderes, das meiste ist ja schon in der Welt
	RndChest()->CreateContents(COKI);
	var scroll = RndChest()->CreateContents(SCRL);
	scroll->SetSpells();
}

global func SpreadAquaSupplies()
{
	// + 4 Flints
	for (var i=0; i < 4; i++) RndChest()->CreateContents(FLNT);
	// + 2 T-Flints und 1 Super-TFlint
	RndChest()->CreateContents(TFLN);
	RndChest()->CreateContents(TFLN);
	RndChest()->CreateContents(STFN);
	// + Airpedo
	RndChest()->CreateContents(ARPD);
}

global func SpreadKingSupplies()
{
	// Sicher 2 normale Pfeilpakete
	for (var i=0; i < 2; i++) RndChest()->CreateContents(ARWP);
	// + 1 Sprengpfeilpaket
	if (!Random(3)) RndChest()->CreateContents(XARP);
}

/*--------------------------------/ PLAYER /----------------------------------*/

// Player Steps

// 0 Teamwahl
// 1 Warten auf Abschluss der Optionen
// 2 Warten auf (Re-)Launch
// 3 Aktiv im Spiel
// 4 
static const QRON_Host = 0;
static QRON_fGameOptionsDone, QRON_afIsNoNewPlr;

public func Cycle(int iPlr, string szEvent, bool fSkip)
	{
	var clonk = GetCrew(iPlr);
	if ("PlrChoosesTeam" == szEvent) return SetFoW(1, iPlr); // warten auf Teamwahl
	if ("PlrJoins" == szEvent)
		{
		if (!g_fGameOptionsDone) return SetForOptions(iPlr);
		else return SetForLaunch(iPlr);
		}
	if ("HostDoneWithSettings" == szEvent) return SetForLaunch(iPlr);
	if ("PlrSetForLaunch" == szEvent)
		{
		clonk = NewClonk(iPlr, 1);
		//SwitchToCrew(clonk, iPlr);
		return AddEffect("IntWait2Launch",, 1, 5, clonk,, iPlr, 15+iPlr, GetID(clonk));
		}
	if ("TimerFinished" == szEvent)
		{
		AddEffect("FoWFadeIn", clonk, 1, 2, clonk,, -1, VIEW_range, 35*2);
		return StartToPlay(iPlr);
		}
	if ("PlrStartedPlaying" == szEvent) return 0; // warten bis er stirbt
	if ("PlrClonkDied" == szEvent)
		{
		HandleDeath(iPlr, GetKiller(clonk));
		if (g_aPlayerData[iPlr][0] <= g_iRelaunchNumber)
			{
			NewClonk(iPlr);
			AddEffect("FoWFadeOut", clonk, 1, 1, clonk,, VIEW_range, -1, 35*3, fSkip);
			}
		}
	if ("FoWFadeOutFinished" == szEvent)
		{
		Sound("Gong", 1,,, iPlr+1);
		SwitchToCrew(iPlr);
		return AddEffect("IntWait2Launch",, 1, 5, clonk,, iPlr, 60, GetID(clonk));
		}
	}

/* Engine Callbacks */

protected func PreInitializePlayer(int iPlr) { return Cycle(iPlr, "PlrChoosesTeam"); }

protected func InitializePlayer(int iPlr)
	{
	// DEBUG
	if (GetScenarioVal("NoInitialize", "Head")) { Log("DEBUG: Player not initialized"); return 0; }
	return Cycle(iPlr, "PlrJoins");
	}

/* Funktionen */

private func UpdateSbrdPlayer(int iPlr, int iVal)
{
	var sbrd = FindObject(SBRD);
	if (sbrd) sbrd->UpdatePlayer(iPlr, iVal);
}

private func SetForLaunch(int iPlr)
	{
	if (GetPlayerTeam(iPlr) < 0) return 0; // Noch am Teamwählen
	// Team-Relaunches aufstocken
	g_aTeamData[GetPlayerTeam(iPlr)][0] += g_iRelaunchNumber;
	UpdateSbrdPlayer(iPlr, SBRD_dwRelaunches);
	// Spielerobjekt weg
	var plo = FindObject2(Find_ID(_PLO), Find_Owner(iPlr));
	if (plo) RemoveObject(plo);
	else RemoveObject(GetCrew(iPlr));
	
	// Und los gehts
	Sound("Horn", 1,,, iPlr);

	//JoinPlayer(iPlr, 15+iPlr, true);
	// Sicht wechseln (geschieht hier nicht automatisch)
	//SetCursor(iPlr, GetCrew(iPlr), 1, 1);
	//SetPlrView(iPlr, GetCrew(iPlr));
	return Cycle(iPlr, "PlrSetForLaunch");
	}
	
	
private func SetForOptions(int iPlr)
	{
	Enter(CreateObject(_PLO,,, iPlr), GetCrew(iPlr));
	if (iPlr == QRON_Host) return GetCrew(iPlr)->HostMenu(); // Hostmenü
	else return PlrMessage("$MsgHostConfiguring$", iPlr);
	}

public func OnGameOptionsDone()
{
	g_fGameOptionsDone = 1;
	// Musik
	SetPlayList("@WeAreAllOne.ogg;@B13a.ogg;@MoonlightPixies.ogg;@JudgementDay.ogg;@MysticNight.ogg;HowlingWind.ogg");
	Music("@WeAreAllOne.ogg");
	// Weiter
	for (var i=0, plr; i < GetPlayerCount(); i++) Cycle(GetPlayerByIndex(i), "HostDoneWithSettings");
}

private func NewClonk(int iPlr, bool fStart)
{
	// ID des neuen Clonks ermitteln
	var ID = g_aClonksOnRelaunch[g_aPlayerData[iPlr][0]];
	// Relaunchposition mithilfe eines Testclonks bestimmen
	var tester, x, y, f;
	while (1)
		{
		tester = PlaceAnimal(CLNK);
		f = !Stuck(tester);
		if (f) f = CheckPosition(x=GetX(tester), y=GetDefBottom(tester), ID, fStart);
		RemoveObject(tester);
		if (f) break; // Relaunchposition ok
		}
	// An dieser Stelle einen Clonk erstellen
	var clonk = CreateObject(ID, x, y, iPlr);
	MakeCrewMember(clonk, iPlr);
	DoEnergy(100, clonk);
	DoBreath(100, clonk);
	// Nach der Geburt ist man ja noch blind
	SetPlrViewRange(-1, clonk);
	// Clonkspezifische Effekte
	if (ID == MAGE) ; //DoMagicEnergy(50, clonk, 1);
	if (ID == ASAS) clonk->Hide(1, 1, 1); // instant Tarnen ohne Message
	// Waffenauswahlmenü öffnen
	clonk->WeaponMenu();
	// Für Relaunch
	AddEffect("IntTrackClonk", clonk, 500);
	// Unbeweglich
	AddEffect("IntInactive", clonk, 1,, clonk);
	return clonk;
}

private func CheckPosition(int iX, int iY, id idClonk, bool fStart)
	{
		// Beim ersten mal ganz oben auf der Burg spawnen
		if (fStart)
			{
			if (Inside(iX, 450, 940) && Inside(iY, 140, 310))
				return 1;
			}
		else if (idClonk == ASAS) // Assa spawnt im Keller
			{
			if (Inside(iX, 820, 1300) && Inside(iY, 680, 930))
			if (!(Inside(iX, 990, 1060)&&Inside(iY, 870, 920)))
				return 1;
			}
		else // standart
			{
			if (Inside(iX, 450, 1090) && Inside(iY, 140, 700))
			if (!(Inside(iX, 640, 670)&&Inside(iY, 420, 450)))
			if (!(Inside(iX, 520, 620)&&Inside(iY, 320, 410)))
			if (!(Inside(iX, 520, 650)&&Inside(iY, 510, 570)))
			if (!(Inside(iX, 890, 940)&&Inside(iY, 480, 520)))
				return 1;
			}
		return 0;
	}

// Goo! (nach IntWait2Launch)
private func StartToPlay(int iPlr)
	{	
	var clonk = GetCrew(iPlr);
	// Go Nachricht
	PlrMessage("$MsgGo$", iPlr);
	// Los geht's
  RemoveEffect("IntInactive", clonk);
  SetComDir(COMD_Stop, clonk);
  SetCursor(iPlr, clonk);
  // Clonktyp im Scoreboard anzeigen
  UpdateSbrdPlayer(iPlr, SBRD_dwCaption);
	// Effekt nur wenn kein Assa
	if (GetID(clonk) != ASAS)
		{
		// Sound
		Sound("PlayerJoin",, clonk);
		// Partikel
		clonk->CastParticles("PxSpark", 50, 100,,, 5*8, 5*50, RGB(196, 196, 196), RGB(255, 255, 255));
		for (var i = 0; i < 5; i++)
			clonk->DrawParticleLine("PSpark", RandomX(-200, 200), -400,,, 8, 150 + 100*!Random(3), RGBa(255, 50+Random(100), 50+Random(100), 255), RGBa(50+Random(100), 200, 50+Random(100), 80));
		}
	return Cycle(iPlr, "PlrStartedPlaying");
	}
	
private func HandleDeath(int iPlr, int iKiller)
{
	// Scoreboard für den Toten
	++g_aPlayerData[iPlr][0];
	UpdateSbrdPlayer(iPlr, SBRD_dwRelaunches);
	
	// Tod
	if (!GetPlayerName(iKiller))
		Log(["$MsgDeath$", "$MsgDeath2$", "$MsgDeath3$"][Random(3)], GetTaggedPlayerName(iPlr));
	// Teamkill
	else if (GetPlayerTeam(iKiller) == GetPlayerTeam(iPlr))
	{
		// Selfkill
		if (iKiller == iPlr)
			Log(["$MsgSuicide$", "$MsgSuicide2$", "$MsgSuicide3$"][Random(3)], GetTaggedPlayerName(iPlr));
		else
			Log(["$MsgTeamkill$", "$MsgTeamkill2$", "$MsgTeamkill3$"][Random(3)], GetTaggedPlayerName(iKiller), GetTaggedPlayerName(iPlr));
	}
	// Regulärer Kill
	else
	{
		Log(["$MsgKill$", "$MsgKill2$", "$MsgKill3$"][Random(3)], GetTaggedPlayerName(iKiller), GetTaggedPlayerName(iPlr));
		// Scoreboard für den Killer
		++g_aPlayerData[iKiller][1];
		++g_aTeamData[GetPlayerTeam(iKiller)][1];
		UpdateSbrdPlayer(iKiller, SBRD_dwKills);
		// Killbelohnung
		DoWealth(iKiller, 2);
		// Nachricht über dem Clonk anzeigen
		var murderer = GetCursor(iKiller);
		if (!murderer) // wenn der Killer vorher gestorben- und schon für den Relaunch deaktiviert worden ist
			{
			Sound("1Up", 1,,, iKiller+1);
			}
		else
			{
			murderer->Sound("1Up",,,, iKiller+1);
			murderer->CastMsg("$MsgPlusKill$", 0xffff00);
			}
	}
	return 1;
}

private func SwitchToCrew(int iPlr)
	{
	SetCursor(iPlr, GetCrew(iPlr), 1, 1);
	SetPlrView(iPlr, GetCrew(iPlr));
	}

/*--------------------------------/ EFFEKTE /---------------------------------*/

// IntTrackClonk

global func FxIntTrackClonkStop(object pTarget, int iNr, int iReason, bool fTemp)
{
	//Log("Track Stop: fTemp=%d, killer=%d, reason=%d", fTemp, GetKiller(pTarget), iReason);
	if (fTemp || !GetPlayerName(GetOwner(pTarget))) return 0;
	// Als Inhaltsobjekt gelöscht? (z.B. bei getarnten Magis)
	if (iReason==3)
		return GameCall("Cycle", GetOwner(pTarget), "PlrClonkDied", true);
	else
		return GameCall("Cycle", GetOwner(pTarget), "PlrClonkDied");
}

global func FxIntTrackClonkDamage(object pTarget, int iNr, int iDmg, int iCause)
{
	//Log("Prio=%d; iDmg = %d, Nr=%d", GetEffect(, pTarget, iNr, 2), iDmg, iNr);
	if (!iDmg) return 0; // Wird manchmal einfach zum Spass aufgerufen..
	
	// Schaden prozentual berechnen. Da wir höchste Prio haben sind hier evtl.
	// Steinschilde miteingeschlossen. Nicht über 100 Prozent falls Heilung.
	var dmg = Min(100*iDmg/GetPhysical("Energy", 0, pTarget), (GetPhysical("Energy", 0, pTarget)-GetEnergy(pTarget)*100000/100)*100/GetPhysical("Energy", 0, pTarget));
	//Log("iDmg=%d, %=%d, CurrentHealth=%d, Fill Up=%d", iDmg, dmg, GetEnergy(pTarget)*100000/100, (GetPhysical("Energy", 0, pTarget)-GetEnergy(pTarget)*100000/100)*100/GetPhysical("Energy", 0, pTarget));
	if (dmg) pTarget->ViewDamage(dmg, GetKiller(pTarget)); // wenn unter 1% nicht anzeigen..
	return iDmg;
}

/* IntInactive */

global func FxIntInactiveStart(object pTarget, int iNr, int iTemp)
{
	if (!pTarget) return -1;
	// Clonk inaktiv machen
	pTarget->SetCrewEnabled(0);
	pTarget->SetObjectStatus(C4OS_INACTIVE); DoBreath(100, pTarget); // Der Clonk verschluckt sich manchmal
}

global func FxIntInactiveStop(object pTarget, int iNr, int iReason, bool fTemp)
{
	// Clonk reaktivieren
	pTarget->SetCrewEnabled(1);
	pTarget->SetObjectStatus(C4OS_NORMAL);
}

/* IntWait2Launch */

global func FxIntWait2LaunchStart(object pTarget, int iNr, int iTemp, int iPlr, int iTime, id idTo)
{
	if (iTemp) return;
	EffectVar(0, pTarget, iNr) = iPlr;
	EffectVar(1, pTarget, iNr) = iTime;
	EffectVar(2, pTarget, iNr) = idTo;
	EffectCall(pTarget, iNr, "Timer"); // gleich anzeigen
	return 1;
}

global func FxIntWait2LaunchTimer(object pTarget, int iNr, int iTime)
{
	var plr = EffectVar(0, pTarget, iNr);
	var time = g_aPlayerData[plr][4] = EffectVar(1, pTarget, iNr)--;
	if (time <= 0) return -1;
	
	// Nachricht
	PlayerMessage(plr, Format("$MsgTimeToRelaunch$", EffectVar(2, pTarget, iNr), time));
	// Zahl im Scoreboard anzeigen
	GameCall("UpdateSbrdPlayer", plr, SBRD_dwCaption);
	return 1;
}

global func FxIntWait2LaunchStop(object pTarget, int iNr, int iReason, bool fTemp)
{
	if (!fTemp && !iReason) return GameCall("Cycle", EffectVar(0, pTarget, iNr), "TimerFinished");
}

/* FoWFadeIn */

global func FxFoWFadeInEffect(string szNewName, object pTarget, int iNr, int iNewNr)
{
	if (szNewName == GetEffect(, pTarget, iNr, 1)) return RemoveEffect(, pTarget, iNr);
}

global func FxFoWFadeInStart(object pTarget, int iNr, int iTemp, int iStartRange, int iEndRange, int iT1, bool fSkip)
{
	if (iTemp) return 0;
	if (fSkip) RemoveEffect(, pTarget, iNr);
	// Init
	EffectVar(0, pTarget, iNr) = iStartRange;
	EffectVar(1, pTarget, iNr) = iEndRange;
	EffectVar(2, pTarget, iNr) = iT1;
	// Startwert setzen
	SetPlrViewRange(iStartRange, pTarget, 1);
	return 1;
}

global func FxFoWFadeInTimer(object pTarget, int iNr, int iTime)
{
	var t1 = EffectVar(2, pTarget, iNr);
	if (iTime > t1) return -1;
	
	var start = EffectVar(0, pTarget, iNr), end = EffectVar(1, pTarget, iNr);
	var r = (end-start)*Sqrt(iTime*1000)/Sqrt(t1*1000)+start;
	if (r) // für r=0 wird FoW aufgelöst -> überspringen
		{
		if (end > start) SetPlrViewRange(Min(r, end), pTarget, 1);
		else if (start > end) SetPlrViewRange(Max(r, end), pTarget, 1);
		}
	return 1;
}

global func FxFoWFadeInStop(object pTarget, int iNr, int iReason, bool fTemp)
{
	if (! fTemp) return GameCall("Cycle", GetOwner(pTarget), "FoWFadeInFinished");
}

/* FoWFadeOut */

global func FxFoWFadeOutEffect(string szNewName, object pTarget, int iNr, int iNewNr)
{
	if (szNewName == GetEffect(, pTarget, iNr, 1)) return RemoveEffect(, pTarget, iNr);
}

global func FxFoWFadeOutStart(object pTarget, int iNr, int iTemp, int iStartRange, int iEndRange, int iT1, bool fSkip)
{
	if (iTemp) return 0;
	if (fSkip) RemoveEffect(, pTarget, iNr);
	// Init
	EffectVar(0, pTarget, iNr) = iStartRange;
	EffectVar(1, pTarget, iNr) = iEndRange;
	EffectVar(2, pTarget, iNr) = iT1;
	// Startwert setzen
	SetPlrViewRange(iStartRange, pTarget, 1);
	return 1;
}

global func FxFoWFadeOutTimer(object pTarget, int iNr, int iTime)
{
	var t1 = EffectVar(2, pTarget, iNr);
	if (iTime > t1) return -1;
	
	var start = EffectVar(0, pTarget, iNr), end = EffectVar(1, pTarget, iNr);
	var r = (end-start)*(iTime**2)/(t1**2)+start;
	if (r) // für r=0 wird FoW aufgelöst -> überspringen
		{
		if (end > start) SetPlrViewRange(Min(r, end), pTarget, 1);
		else if (start > end) SetPlrViewRange(Max(r, end), pTarget, 1);
		}
	return 1;
}

global func FxFoWFadeOutStop(object pTarget, int iNr, int iReason, bool fTemp)
{
	if (fTemp || !GetPlayerName(GetOwner(pTarget))) return 0;
	return GameCall("Cycle", GetOwner(pTarget), "FoWFadeOutFinished");
}

//AddEffect("FoWFadeOut", this, 1, 1, this,, VIEW_range, -1, 35*3);
//AddEffect("FoWFadeIn", this, 1, 2, this,, -1, VIEW_range, 35*2, 1);
