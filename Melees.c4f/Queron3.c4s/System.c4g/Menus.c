/*-- Menüs --*/

// script v1.1 by miniwipf
#strict 2

/*--------------------------------/ MENUS /-----------------------------------*/

// WeaponMenu

global func WeaponMenu()
{
	var ID = GetID();
	AddMenu(WEPN, this, ID, "$MnuWeaponTitle$");
	SetMenuExtra(WEPN, C4MN_Extra_Components);
	var entryIDs = [];
	// Je nach Clonktyp unterschiedliche Möglichkeiten
	if (ID == CLNK) { entryIDs = [_BMR, STFN]; }
	if (ID == KNIG) { entryIDs = [SWOR, BOW1]; }
	if (ID == PLDN) { entryIDs = [TSWD, MUSK]; }
	if (ID == MAGE) { entryIDs = [SMAR, SMER, SMFR, SMWT]; }
	if (ID == ASAS) { entryIDs = [KNFP, CRBW]; }
	if (ID == KAND) { entryIDs = [SCMT, BOW1]; }
	if (ID == MYST) { entryIDs = [TFLN, GUNP]; }
	if (ID == ACLK) { entryIDs = [ARPD, FLNT]; }
	if (ID == KING) { entryIDs = [AXE1, BOW1]; }
	//if (ID == SKLT) { entryIDs = [SKUL, BONE]; }
	// Waffenoptionen
	for (var entryID in entryIDs)
		AddMenuEntry(WEPN, entryID, -1, "On_WEPN_MenuChoice", entryID, "$MnuWeaponSelectCap$");
	// Menü anzeigen
	OpenMenu(WEPN);
	return 1;
}

global func On_WEPN_MenuChoice(int iEntryNr, int iMode, object pMenuObj)
{
	var idEntry = pMenuObj->M_EntryID(WEPN, iEntryNr);
	if (iMode < 1) return;
	var ID = GetID();
	var weapons;
	// Clonk
	if (ID == CLNK)
	{
		if (idEntry == _BMR)weapons = [_BMR];
		else if (idEntry == STFN) weapons = [STFN];
		SpreadClonkSupplies();
	}
	// Ritter
	if (ID == KNIG)
	{
		if (idEntry == SWOR)
		{
				weapons = [SWOR];
				CreateContents(SHIE) -> Activate(this);
		}
		else if (idEntry == BOW1) weapons = [BOW1, ARWP];
		SpreadKnightSupplies();
	}
	// Paladin
	if (ID == PLDN)
	{
		if (idEntry == TSWD)
		{
			weapons = [TSWD];
			CreateContents(SHIE) -> Activate(this);
		}
		else if (idEntry == MUSK) weapons = [MUSK, BLTP];
		SpreadPaladinSupplies();
	}
	// Magus
	if (ID == MAGE)
	{
		DoMagicEnergy(10); // Startzuschuss
		if (idEntry == SMAR) weapons = [SMAR, PMAN];
		else if (idEntry == SMER) weapons = [SMER, PMAN];
		else if (idEntry == SMFR) weapons = [SMFR, PMAN];
		else if (idEntry == SMWT) weapons = [SMWT, PMAN];
		SpreadMageSupplies();
	}
	// Assassin
	if (ID == ASAS) {
		if (idEntry == KNFP) weapons = [KNFP];
		else if (idEntry == CRBW) weapons = [CRBW, BOTP];
		SpreadAssaSupplies();
	}
	// Kendarier
	if (ID == KAND)
	{
		if (idEntry == SCMT)
		{
			weapons = [SCMT];
			CreateContents(SHI2) -> Activate(this);
		}
		else if (idEntry == BOW1) weapons = [BOW1, ARWP];
		SpreadKendarianSupplies();
	}
	// Mystiker
	if (ID == MYST) {
		if (idEntry == TFLN) weapons = [WOOD, FLNT, ROCK];
		else if (idEntry == GUNP) weapons = [GUNP, TFLN, LIQG];
		//else if (idEntry == CRYS) { weapons = [CRYS, GOLD, SAND]; }
		SpreadMysticSupplies();
	}
	// Aquaclonk
	if (ID == ACLK)
	{
		if (idEntry == ARPD) weapons = [ARPD];
		else if (idEntry == FLNT) weapons = [FLNT,2];
		SpreadAquaSupplies();
	}
	// König
	if (ID == KING)
	{
		if (idEntry == AXE1)
		{
			weapons = [AXE1, PSTO];
			CreateContents(SHIE) -> Activate(this);
		}
		else if (idEntry == BOW1) weapons = [BOW1, FARP];
		SpreadKingSupplies();
	}
	// Skelett
	/*if (ID == SKLT) {
		if (idEntry == BONE) { weapons = [BONE, 10]; }
		else if (idEntry == SKUL) { weapons = [SKUL, 5]; }
	}*/
	// ..und Waffen erzeugen
	for (var i=GetLength(weapons)-1; i >= 0; i--)
		if (GetType(weapons[i]) == C4V_Int) { CreateContents(weapons[--i],, weapons[i+1]); }
		else CreateContents(weapons[i]);
	// Sound
	Sound("Connect");
	// Fertig
	return RemoveMenu(WEPN);
}


// HostMenu

global func HostMenu()
{
	// Hostmenü
	AddMenu(HOST, this, CXHM, "$MnuHostTitle$");
	SetMenuStyle(HOST, 3);
	AddMenuEntry(HOST, ICON, -1,, RSR1);
	AddMenuEntry(HOST, INFO, -1,,, "$MnuHostInfoCap$");
	AddMenuEntry(HOST, CLNK, -1, "On_HOST_MenuChoice", REAC, "$MnuHostClonksCap$", "$MnuHostClonksTxt$");
	AddMenuEntry(HOST, RLNC, -1, "On_HOST_MenuChoice", RSTR, "$MnuHostRelaunchesCap$", "$MnuHostRelaunchesTxt$");
	AddMenuEntry(HOST, REDY, -1, "On_HOST_MenuChoice", SWOR, "$MnuHostReadyCap$", "$MnuHostReadyTxt$");
	
	// Relaunchmenü
	AddMenu(RLNC, this, CXHM, "$MnuRelaunchTitle$");
	SetMenuParent(RLNC, HOST);
	SetMenuStyle(RLNC, 3);
	AddMenuEntry(RLNC, ICON, -1,, RSTR);
	AddMenuEntry(RLNC, INFO, -1);
		Update_RLNC_MenuText(); // Relaunchinfo updaten
	var pos = AddMenuEntry(RLNC, MORE, -1, "On_RLNC_MenuChoice", XMEN, "$MnuRelaunchIncreaseCap$");
	SetMenuEntryExtra(RLNC, pos, 2, 2);
	pos = AddMenuEntry(RLNC, LESS, -1, "On_RLNC_MenuChoice", XMEN, "$MnuRelaunchDecreaseCap$");
	SetMenuEntryExtra(RLNC, pos, 2, 3);
	
	// Clonkmenü
	AddMenu(CDEF, this, CXHM, "$MnuClonkDefTitle$");
	SetMenuParent(CDEF, HOST);
	SetMenuStyle(CDEF, 3);
	AddMenuEntry(CDEF, ICON, 0,, REAC);
	var pos;
	for (var type in g_aClonksOnRelaunch) {
		pos = AddMenuEntry(CDEF, type, -1, "On_CDEF_MenuChoice");
		Update_CDEF_MenuRelaunch(pos); // Text anpassen
	}

	// Clonkauswahlmenü
	AddMenu(CLNK, this, CXHM, "$MnuClonkTitle$");
	SetMenuParent(CLNK, CDEF);
	SetMenuSize2(CLNK, 3, 3);
	var clonks = [CLNK,ACLK,KING, KNIG,PLDN,MAGE, KAND,MYST,ASAS]; // SKLT
	for (var clonk in clonks)
		AddMenuEntry(CLNK, clonk, -1, "On_CLNK_MenuChoice", clonk, "$MnuClonkCap$");

	// Hostmenü anzeigen
	OpenMenu(HOST);
	return 1;
}

global func On_HOST_MenuChoice(int iEntryNr, int iMode, object pMenuObj)
{
	var idEntry = pMenuObj->M_EntryID(HOST, iEntryNr);
	var plr = GetOwner();
	if (iMode < 1) return; // Abbrechen klemmt :D
	// Auswahl
	if (idEntry == RLNC) {
		Sound("ArrowHit", 1,,, plr+1);
		return OpenMenu(RLNC);
	}
	if (idEntry == CLNK) {
		Sound("ArrowHit", 1,,, plr+1);
		return OpenMenu(CDEF);
	}
	// Check: Alle Clonks gesetzt?
	for (var clonk in g_aClonksOnRelaunch)
		if (!clonk) {
			// Error
			Sound("Error", 1,,, plr+1);
			PlrMessage("$MsgClonksMustBeDefined$", plr);
			return; // Geth nicht
		}
	// Fertig gewählt
	GameCall("OnGameOptionsDone");
	// Fertig
	return RemoveMenu(HOST);
}

global func On_RLNC_MenuChoice(int iEntryNr, int iMode, object pMenuObj)
{
	var idEntry = pMenuObj->M_EntryID(RLNC, iEntryNr);
	var plr = GetOwner();
	if (!iMode) return;
	if (iMode < 0) {
		Sound("ControlRate", 1,,, plr+1);
		// Clonkarray anpassen
		var rel = GetLength(g_aClonksOnRelaunch)-1;
		if (rel != g_iRelaunchNumber) {
			if (rel < g_iRelaunchNumber) {
				var pos;
				while (rel++ < g_iRelaunchNumber) {
					pos = AddMenuEntry(CDEF, CLNK, -1, "On_CDEF_MenuChoice");
					Update_CDEF_MenuRelaunch(pos); // Text anpassen
				}
			}
			else {
				while (rel-- > g_iRelaunchNumber)
					RemoveMenuEntry(CDEF, -1);
			}
			SetLength(g_aClonksOnRelaunch, g_iRelaunchNumber+1);
		}
		return CloseMenu2(RLNC);
	}
	var n = 1;
	if (iMode == 2) n = 5; // Bei Special2 +/- 5 Relaunches
	// Auswahl
	if (idEntry == MORE) {
		Sound("CatapultSet", 1,,, plr+1);
		g_iRelaunchNumber += n;
	}
	if (idEntry == LESS) {
		if (!g_iRelaunchNumber) return 1;
		Sound("CatapultSet", 1,,, plr+1);
		g_iRelaunchNumber = BoundBy(g_iRelaunchNumber-n, 0, g_iRelaunchNumber);
	}
	// Relaunchanzeige anpassen
	Update_RLNC_MenuText();
	return 1;
}

global func Update_RLNC_MenuText()
{
	SetMenuEntryBasic(RLNC, 1,, Format("$MnuRelaunchInfoCap$", g_iRelaunchNumber));
	return 1;
}

global func On_CDEF_MenuChoice(int iEntryNr, int iMode, object pMenuObj)
{
	var idEntry = pMenuObj->M_EntryID(CDEF, iEntryNr);
	var plr = GetOwner();
	if (!iMode) return;
	if (iMode < 0) {
		Sound("ControlRate", 1,,, plr+1);
		return CloseMenu2(CDEF);
	}
	Sound("ArrowHit", 1,,, plr+1);
	// Auswahl
	return OpenMenu(CLNK);
}

global func Update_CDEF_MenuRelaunch(int iPos)
{
	var relaunch = iPos-1; // -2 wegen Icon und Info
	var ID = g_aClonksOnRelaunch[relaunch], cap = "", infocap = "", name = "";
	if (!ID) {
		ID = CXIN;
		infocap = "$MnuClonkDefUndefinedTxt$";
	}
	else
		name = GetName(, ID);
	if (!relaunch) { // Startclonk
		cap = Format("$MnuClonkDefStartclonkCap$", name);
		infocap = Format("$MnuClonkDefStartclonkTxt$", name);
	}
	else
		cap = Format("$MnuClonkDefRelaunchCap$", relaunch, name);
	SetMenuEntryBasic(CDEF, iPos, ID, cap, infocap, 1);
	return 1;
}

global func On_CLNK_MenuChoice(int iEntryNr, int iMode, object pMenuObj)
{
	var idEntry = pMenuObj->M_EntryID(CLNK, iEntryNr);
	var plr = GetOwner();
	if (!iMode) return;
	if (iMode < 0) {
		// Close
		Sound("ControlRate", 1,,, plr+1);
		return CloseMenu2(CLNK);
	}
	Sound("Click", 1,,, plr+1);
	// Wir ändern den ausgewählten Relaunchclonk
	var pos = GetMenuSelection2(CDEF);
	var relaunch = pos-1; // -2 wegen Icon und Info
	// Eintragen
	g_aClonksOnRelaunch[relaunch] = idEntry;
	// Eintrag updaten
	Update_CDEF_MenuRelaunch(pos);
	return CloseMenu2(CLNK);
}
