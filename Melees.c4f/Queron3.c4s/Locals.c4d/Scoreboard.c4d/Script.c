/*-- Scoreboard Toll^2 mit Teams und so --*/

// script v1.8 by miniwipf
#strict 2

// [X1:[RLNCS, KILLS, DEATHS, SCORE, ..], X2:[..]]
static g_aPlayerData;
static g_aTeamData;
static g_aInitPlrTeam; // PreInitialize -> Initialize: speichert iOldTeam für TeamSwitch

static SBRD_fTeamMode; // Scoreboard im Teammodus (Konstante, wird bei Spielstart gesetzt)

// Scoreobard-Col Zuordnung
static const SBRD_Relaunches = 0,
	SBRD_Kills = 1;

// Scoreboard-Row Zuordnung ([team-1, team1, team2, ..])
static g_aSbrdTeamRows, g_aSbrdPlrRows, g_aSbrdFreeRows;

// Bitmasken
static const SBRD_dwCaption = 1,
	SBRD_dwRelaunches = 2,
	SBRD_dwKills = 4,
	SBRD_dwAll = 7; // SBRD_dwCaption|SBRD_dwRelaunches|SBRD_dwKills


/* ENGINE */

protected func Initialize()
{
	// Statics initialisieren
	g_aPlayerData = [];
	g_aTeamData = [];
	g_aInitPlrTeam = [];
	// row-ID's
	g_aSbrdPlrRows = [];
	g_aSbrdTeamRows = [];
	g_aSbrdFreeRows = [1];
	// Scoreboardgerüst erstellen
	UpdateHead(SBRD_dwAll);
	return 1;
}

protected func Destruction()
{
	// Scoreboardgerüst löschen
	SetScoreboardData(SBRD_Caption, SBRD_Caption);
	SetScoreboardData(SBRD_Caption, SBRD_Relaunches);
	SetScoreboardData(SBRD_Caption, SBRD_Kills);
	return 1;
}

protected func OnTeamSwitch(int iPlr, int iTeam, int iOldTeam)
{
	//if (iTeam == iOldTeam) return 1; // Bleibt alles beim Alten
	// Raus aus dem alten Team
	if (!CheckTeam(iPlr, iOldTeam, 1));
		UpdateTeam(iOldTeam, SBRD_dwAll);
	// ... und ins neue Team
	CheckTeam(iPlr, iTeam,, 1);
	UpdatePlayer(iPlr, SBRD_dwAll);
	return 1;
}

/* PLAYER */

// Vor Teambeitritt
protected func PreInitializePlayer(int iPlr)
{
	// Teamspiel? -> Teamsupported Scoreboard
	if (!SBRD_fTeamMode) CheckSbrdConfig(iPlr);
	// Spieler initialisieren
	g_aPlayerData[iPlr] = [];
	g_aInitPlrTeam[iPlr] = GetPlayerTeam(iPlr); // Init-Team speichern
	// Neue row-ID beziehen
	var last = GetLength(g_aSbrdFreeRows)-1;
	g_aSbrdPlrRows[iPlr] = g_aSbrdFreeRows[last];
	if (!last) g_aSbrdFreeRows[0]++; // [0] = max Row-ID
	else SetLength(g_aSbrdFreeRows, last);
	// In Team einordnen
	CheckTeam(iPlr, GetPlayerTeam(iPlr),, 1);
	UpdatePlayer(iPlr, SBRD_dwAll);
	//DoScoreboardShow(1, iPlr);
	return 1;
}

private func CheckSbrdConfig(int iPlr)
{
	// Spieler befindet sich in einem Team?
	if (GetPlayerTeam(iPlr) != 0)
		if (GetTeamConfig(4) == 1) return SBRD_fTeamMode = 1; // Teamsupport aktiviert
	return SBRD_fTeamMode = -1; // Teamsupport deaktiviert
}

// Zuerst das Szenarioscript (evtl. Veränderungen der team/plr-Werten)
//protected func InitializePlayer(int iPlr, x, y, base, int iTeam) { return ScheduleCall(this, "InitializePlayer2", 1,, Par(0), Par(1), Par(2), Par(3), Par(4), Par(5)); }

private func InitializePlayer(int iPlr, x, y, base, int iTeam)
{
	// Wie Teamswitch
	var oteam = g_aInitPlrTeam[iPlr];
	OnTeamSwitch(iPlr, iTeam, oteam);
	return 1;
}

// Zuerst das Szenarioscript (evtl. Veränderungen der team/plr-Werten)
//protected func RemovePlayer(int iPlr) { return ScheduleCall(this, "RemovePlayer2", 1,, Par(0), Par(1), Par(2), Par(3), Par(4), Par(5)); }

protected func RemovePlayer(int iPlr)
{
	// Spielerdaten löschen
	var plrID = g_aSbrdPlrRows[iPlr];
	SetScoreboardData(plrID, SBRD_Caption);
	SetScoreboardData(plrID, SBRD_Relaunches);
	SetScoreboardData(plrID, SBRD_Kills);
	// row-ID freigeben
	g_aSbrdFreeRows[GetLength(g_aSbrdFreeRows)] = g_aSbrdPlrRows[iPlr];
	g_aSbrdPlrRows[iPlr] = 0;
	// Letzter Spieler -> Team raus
	CheckTeam(iPlr, GetPlayerTeam(iPlr), 1, 1);
	return 1;
}

/* TEAMS */

private func CheckTeam(int iPlr, int iTeam, bool fOut, bool fIncludePlr)
{
	var teamplrs = GetTeamPlayerCount(iTeam)-fIncludePlr;
	if (teamplrs <= 0) {
		if (fOut) {
			if (g_aSbrdTeamRows[iTeam]) return RemoveTeam(iTeam);
		}
		else {
			if (!g_aSbrdTeamRows[iTeam]) return InitializeTeam(iTeam);
		}
	}
	return;
}

private func InitializeTeam(int iTeam)
{
	// Nettwerweise wird mit -1 direkt Index 0 angesprochen, deshalb müssen wir
	//bei Team -1 nicht extra umdefinieren
	// Team initialisieren
	g_aTeamData[iTeam] = []; // Wird noch initialisiert, damit Szenarienscripte nicht immer nach Teams checken müssen bevor sie die Teamvar verändern
	if (SBRD_fTeamMode < 0) return 0;
	// Neue row-ID beziehen
	var last = GetLength(g_aSbrdFreeRows)-1;
	g_aSbrdTeamRows[iTeam] = g_aSbrdFreeRows[last];
	if (!last) g_aSbrdFreeRows[0]++;
	else SetLength(g_aSbrdFreeRows, last);
	// Teamdaten erstellen (nur Beschriftung)
	UpdateTeam(iTeam, SBRD_dwCaption); // Kommt eh noch der Spieler und updatet alles
	return 1;
}

private func RemoveTeam(int iTeam)
{
	if (SBRD_fTeamMode < 0) return 0;
	// Teamdaten löschen
	var teamID = g_aSbrdTeamRows[iTeam];
	SetScoreboardData(teamID, SBRD_Caption);
	SetScoreboardData(teamID, SBRD_Relaunches);
	SetScoreboardData(teamID, SBRD_Kills);
	// row-ID freigeben
	g_aSbrdFreeRows[GetLength(g_aSbrdFreeRows)] = g_aSbrdTeamRows[iTeam];
	g_aSbrdTeamRows[iTeam] = 0;
	return 1;
}

/* UPDATE */

public func UpdateHead(int dwVal)
{
	var val, txt;
	// Scoreboardname
	if (dwVal & SBRD_dwCaption)
		; //SetScoreboardData(SBRD_Caption, SBRD_Caption, "$TxtTitle$"); // Kein Titel -> Scoreboard kleiner und nervt weniger
	// Titel für Relaunches
	if (dwVal & SBRD_dwRelaunches)
		SetScoreboardData(SBRD_Caption, SBRD_Relaunches, "{{CLNK}}");
	// Titel für Kills
	if (dwVal & SBRD_dwKills)
		SetScoreboardData(SBRD_Caption, SBRD_Kills, "{{SWOR}}");
	return 1;
}

public func UpdatePlayer(int iPlr, int dwVal)
{
	if (iPlr < 0) return 1; // Ungültiger Spieler

	var team = GetPlayerTeam(iPlr);
	var plrID = g_aSbrdPlrRows[iPlr], clr, add;

	// Spielerfarbe
	if (team < 0) // Teamwahl
		clr = 0x999999;
	else clr = GetSbrdPlrClr(iPlr);

	// Clonktyp & Spielername
	if (dwVal & SBRD_dwCaption) {
		var val = g_aPlayerData[iPlr][4], txt = Format("<c %x>%s</c>", clr, GetSbrdPlrName(iPlr));
		if (team < 0) ;
		else {
			if (val) txt = Format("%d %s", val, txt);
			else txt = Format("<c %x>{{%i}}</c> %s", RGBa(255, 255, 255, 150), GetID(GetCrew(iPlr)), txt);
			if (SBRD_fTeamMode > 0) txt = Format("  %s", txt); // Bei Teams einrücken
		}
		SetScoreboardData(plrID, SBRD_Caption, txt);
	}
	// Relaunches
	if (dwVal & SBRD_dwRelaunches) {
		var val = g_iRelaunchNumber-g_aPlayerData[iPlr][0], txt = "";
		if (team < 0) txt = "/";
		else {
			if (g_fGameOptionsDone) {
				if (val < 0) txt = "out";
				else txt = Format("%d", val);
			}
			else txt = "?";
		}
		SetScoreboardData(plrID, SBRD_Relaunches,  Format("<c %x>%s</c>", clr, txt), CalcSbrdVal(iPlr, val));
	}
	// Kills
	if (dwVal & SBRD_dwKills) {
		var val = g_aPlayerData[iPlr][1], txt = "";
		if (team < 0) txt = "/";
		else txt = Format("%d", val);
		SetScoreboardData(plrID, SBRD_Kills, Format("<c %x>%s</c>", clr, txt), CalcSbrdVal(iPlr, val));
	}
	// Team updaten
	UpdateTeam(team, dwVal);
	// Sortierung: 1. Kills, 2. Relaunches
	// Absteigend nach Relaunches sortieren
	SortScoreboard(SBRD_Kills, 1);
	SortScoreboard(SBRD_Relaunches, 1);
	return 1;
}

public func UpdateTeam(int iTeam, int dwVal)
{
	if (SBRD_fTeamMode != 1 || iTeam < 0) return; // nur falls Teams aktiviert & tatsächlich ein Team
	
	var teamID = g_aSbrdTeamRows[iTeam];

	// Teamname
	if (dwVal & SBRD_dwCaption) {
		SetScoreboardData(teamID, SBRD_Caption, Format("<i>%s</i>", GetTeamName(iTeam)));
	}
	// Gesamtrelaunches
	if (dwVal & SBRD_dwRelaunches) {
		var val = TotalTeamRelaunches(iTeam), txt = "";
		if (g_fGameOptionsDone) {
			if (val < 0) txt = "out";
			else txt = Format("%d", val);
		}
		else txt = "?";
		SetScoreboardData(teamID, SBRD_Relaunches, Format("<i>%s</i>", txt), CalcSbrdVal(-1, iTeam));
	}
	// Kills gesamt
	if (dwVal & SBRD_dwKills) {
		var val = g_aTeamData[iTeam][1], txt = Format("%d", val);
		SetScoreboardData(teamID, SBRD_Kills, Format("<i>%d</i>", txt), CalcSbrdVal(-1, iTeam));
	}
	return 1;
}

// fAsc = Sortierungsfaktor: 0: Absteigend, 1: Aufsteigend
global func CalcSbrdVal(int iPlr, int iVal, bool fAsc)
{
	var team = GetPlayerTeam(iPlr), diff = 100;
	// Singlescoreboard
	if (SBRD_fTeamMode < 0) if (team > 0) team = 0;
	// Teamorientiert berechnen
	if (fAsc) {
		if (iPlr < 0) return iVal*diff; // Für Teamval wird iPlr < 0 gesetzt und für iVal das Team angegeben
		return team*diff + 1 + iVal;
	}
	else {
		if (iPlr < 0) return -iVal*diff;
		return -(team+1)*diff + 1 + iVal;
	}
}

global func TotalTeamRelaunches(int iTeam)
{
	var sum = -1;
	for (var i=0, b, plr; i < GetPlayerCount(); i++) {
		plr = GetPlayerByIndex(i);
		if (GetPlayerTeam(plr) == iTeam) {
			if (g_aPlayerData[plr][0] > g_iRelaunchNumber) continue; // Eliminierte Spieler zählen nicht mit
			b = 1;
			sum += g_iRelaunchNumber-g_aPlayerData[plr][0];
		}
	}
	return sum+b;
}

/* GLOBALS */

global func GetTeamPlayerCount(int iTeam)
{
	var n;
	for (var i=0, plr; i < GetPlayerCount(); i++) {
		plr = GetPlayerByIndex(i);
		if (GetPlayerTeam(plr) == iTeam) n++;
	}
	return n;
}

global func GetSbrdPlrName(int iPlr)
{
	// Noob-Namen kürzen
	var name = GetPlayerName(iPlr);
	if (GetLength(name) > 12) name = Format("%.10s..", name);
	return name;
}

global func GetSbrdPlrClr(int iPlr)
{
	var clr = GetPlrColorDw(iPlr);
	MakeColorReadableOnBlack(clr);
	return clr;
}

// backport von OC
global func MakeColorReadableOnBlack(& rdwClr)
{
		rdwClr &= 0xffffff; // remove alpha
		// determine brightness
		// 50% red, 87% green, 27% blue (max 164 * 255)
		var r=(rdwClr>>16&255), g=(rdwClr>>8&255), b=(rdwClr&255);
		var iLightness = r*50 + g*87 + b*27;
		// above 65/164 (*255) is OK
		if (iLightness < 16575)
		{
			var iInc = (16575-iLightness) / 164;
			// otherwise, lighten
			rdwClr = (Min(r+iInc, 255)<<16) | (Min(g+iInc, 255)<<8) | Min(b+iInc, 255);
		}
		return 1;
}
