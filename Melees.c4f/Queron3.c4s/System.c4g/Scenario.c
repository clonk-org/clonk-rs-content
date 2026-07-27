/*-- Objekte --*/

// script v1.9 by miniwipf
#strict 2

global func PrepareLandscape()
{
	// Himmel
	SetSkyParallax(0, 50, 50);
	/* Schiefe Flächen */

	/* Turm links aussen */
	// Dach
	DrawMaterialQuad("RoofTiles-Bricks1", 160, 470, 160, 460, 250, 550, 240, 550, 1);
	DrawMaterialQuad("RoofTiles-Bricks1", 130, 490, 120, 490, 150, 460, 150, 470, 1);
	DrawMaterialQuad("RoofTiles-Bricks1", 80, 540, 70, 540, 90, 520, 90, 530, 1);
	DrawMaterialQuad("Wall-Bricks1", 110, 530, 110, 530, 110, 520, 130, 530, 1);
	// Treppen
	DrawMaterialQuad("SmoothWall-Bricks1", 130, 660, 130, 650, 150, 670, 140, 670, 1);
	DrawMaterialQuad("SmoothWall-Bricks1", 150, 790, 150, 780, 190, 820, 180, 820, 1);
	// Zinnen
	DrawMaterialQuad("SmoothWall-Bricks1", 30, 620, 30, 620, 40, 620, 40, 630, 1);
	DrawMaterialQuad("SmoothWall-Bricks1", 220, 630, 230, 620, 240, 620, 220, 640, 1);
	DrawMaterialQuad("SmoothWall-Bricks1", 240, 780, 240, 780, 240, 770, 250, 770, 1);
	// Fahrstuhl
	DrawMaterialQuad("SmoothWall-Bricks1", 40, 630, 40, 610, 45, 610, 45, 630, 1);

	/* Bereich rechts neben dem Wassergraben */
	// Treppen
	DrawMaterialQuad("SmoothWall-Bricks1", 460, 820, 450, 820, 520, 750, 520, 760, 1);
	DrawMaterialQuad("SmoothWall-Bricks1", 450, 680, 450, 670, 500, 720, 490, 720, 1);
	// Zinnen
	DrawMaterialQuad("SmoothWall-Bricks1", 410, 660, 420, 660, 430, 670, 430, 680, 1);

	/* Burg Hauptteil */

	/* Links */
	// Zinnen
	DrawMaterialQuad("SmoothWall-Bricks1", 490, 550, 500, 550, 510, 560, 510, 570, 1);
	// Kammer rechts über Eingang: Schräge am Boden und decke
	DrawMaterialQuad("SmoothWall-Bricks1", 590, 570, 620, 540, 620, 550, 600, 570, 1);
	DrawMaterialQuad("SmoothWall-Bricks2", 570, 550, 570, 540, 600, 510, 610, 510, 1);
	// Schräge am boden darüber ^^
	DrawMaterialQuad("SmoothWall-Bricks1", 560, 530, 600, 490, 600, 500, 570, 530, 1);
	// Linke Wand
	DrawMaterialQuad("SmoothWall-Bricks1", 520, 400, 520, 400, 540, 400, 540, 470, 1);
	// darunter ein kleines Dreieck, damit die wand nicht so dünn aussieht :O
	DrawMaterialQuad("SmoothWall-Bricks1", 540, 460, 540, 460, 540, 450, 550, 460, 1);

	/* Oben */
	// Grosse Treppe nach rechts und decke darüber
	DrawMaterialQuad("SmoothWall-Bricks1", 620, 430, 680, 370, 680, 380, 630, 430, 1);
	DrawMaterialQuad("SmoothWall-Bricks1", 640, 370, 640, 360, 650, 350, 660, 350, 1);
	// Kleinere Treppe, die nach links abzweigt und decke darüber
	DrawMaterialQuad("SmoothWall-Bricks1", 580, 380, 580, 370, 600, 390, 590, 390, 1);
	//DrawMaterialQuad("SmoothWall-Bricks1", 570, 330, 580, 330, 610, 360, 610, 370, 1);
	DrawMaterialQuad("SmoothWall-Bricks1", 580, 340, 590, 340, 610, 360, 610, 370, 1);
	// Nach der grossen Treppe nach rechts kommt die grosse treppe nach links + decke
	DrawMaterialQuad("SmoothWall-Bricks3", 620, 310, 620, 300, 660, 340, 650, 340, 1);
	DrawMaterialQuad("SmoothWall-Bricks2", 660, 300, 670, 300, 690, 320, 690, 330, 1);
	
	/* Mitte */
	
	/* Oben */
	// Kammer über Thronsaal: Aufstieg rechts
	DrawMaterialQuad("SmoothWall-Bricks1", 770, 430, 820, 380, 820, 390, 790, 440, 1);
	// Ausschneiden
	DrawMaterialQuad("BackWall-Bricks2", 770, 420, 780, 400, 820, 440, 790, 440, 1);
	// Pool
	DrawMaterialQuad("BathWall-Bricks1", 840, 300, 840, 300, 850, 300, 850, 290, 1);
	
	/* Rechts */
	// Treppen
	DrawMaterialQuad("SmoothWall-Bricks1", 840, 620, 840, 610, 890, 660, 880, 660, 1);
	DrawMaterialQuad("SmoothWall-Bricks1", 910, 580, 910, 570, 940, 600, 930, 600, 1);
	// Bodenschräge rechts aussen
	DrawMaterialQuad("SmoothWall-Bricks2", 1010, 650, 1020, 640, 1020, 650, 1010, 650, 1);
	// untendran
	DrawMaterialQuad("SmoothWall-Bricks1", 1030, 660, 1030, 660, 1030, 650, 1040, 650, 1);
	
	/* Oben */
	// Kleiner Gang in den Turm: erste Bodenschräge von links
	DrawMaterialQuad("Wall-Bricks1", 880, 360, 900, 340, 900, 350, 890, 360, 1);
	// darüber
	DrawMaterialQuad("Wall-Bricks1", 880, 340, 880, 330, 890, 320, 900, 320, 1);
	// zweite Schräge
	DrawMaterialQuad("Wall-Bricks1", 910, 350, 910, 350, 910, 340, 920, 350, 1);
	// darüber
	DrawMaterialQuad("Wall-Bricks1", 910, 320, 910, 320, 920, 320, 920, 330, 1);
	
	/* Oben mittig */
	// Hinab zum Turmaufgang
	DrawMaterialQuad("Wall-Bricks1", 570, 310, 570, 310, 580, 300, 580, 310, 1);

	/* Linker Turm der Burg */
	// Treppen
	DrawMaterialQuad("SmoothWall-Bricks1", 520, 270, 520, 260, 550, 290, 540, 290, 1);
	// Zinne rechts aussen beim Garten
	DrawMaterialQuad("SmoothWall-Bricks1", 680, 250, 680, 250, 680, 240, 690, 240, 1);
	// Zinnen oben links und rechts
	DrawMaterialQuad("SmoothWall-Bricks1", 450, 160, 460, 160, 470, 170, 470, 180, 1);
	DrawMaterialQuad("SmoothWall-Bricks1", 570, 170, 570, 170, 570, 160, 580, 160, 1);

	/* Rechter Turm der Burg */
	// Treppen
	DrawMaterialQuad("Wall-Bricks1", 920, 460, 920, 460, 930, 450, 930, 460, 1);
	DrawMaterialQuad("Wall-Bricks1", 1010, 390, 1010, 390, 1020, 380, 1020, 390, 1);
	// Tunnelschacht
	DrawMaterialQuad("BackWall-Bricks2", 961, 360, 961, 250, 984, 250, 984, 360, 1);
	// Dach
	DrawMaterialQuad("RoofTiles-Bricks1", 920, 50, 970, 0, 970, 10, 930, 50, 1);
	DrawMaterialQuad("RoofTiles-Bricks1", 1020, 10, 1020, 0, 1090, 70, 1080, 70, 1);
	// Zinnen
	DrawMaterialQuad("SmoothWall-Bricks1", 1040, 350, 1040, 340, 1050, 330, 1060, 330, 1);
	// oben
	DrawMaterialQuad("SmoothWall-Bricks1", 920, 120, 930, 120, 950, 140, 950, 150, 1);
	DrawMaterialQuad("SmoothWall-Bricks1", 1040, 150, 1040, 140, 1060, 120, 1070, 120, 1);
	
	/* Höhle */
	// Neben cave
	DrawMaterialQuad("SmoothWall-Bricks1", 880, 930, 910, 900, 910, 910, 890, 930, 1);
	// Neben fahrstuhl
	DrawMaterialQuad("SmoothWall-Bricks1", 870, 810, 870, 800, 890, 820, 880, 820, 1);
	// Ober fahrstuhl
	DrawMaterialQuad("SmoothWall-Bricks1", 920, 780, 920, 780, 930, 770, 930, 780, 1);
	// Grosse schräge rechts von flintwerfer
	DrawMaterialQuad("Wall-Bricks1", 640, 750, 660, 750, 710, 800, 690, 800, 1);
	// Vor Zelt
	DrawMaterialQuad("Rock-Rough", 380, 830, 430, 830, 430, 820, 380, 820, 1);
	
	// Fahrstuhlschächte
	// Höhle
	DrawMaterialQuad("BackWall-Bricks2", 920, 820, 943, 820, 943, 870, 920, 870, 1);
	// Brunnen
	DrawMaterialQuad("BackWall-Bricks2", 1183, 680, 1206, 680, 1206, 820, 1183, 820, 1);
	return 1;
}

global func CreateConstructions()
{
	// Wand hinter Turmfahrstuhl rechts: 973.250
	CreateConstruction(PPW1, 973, 251, NO_OWNER, 100);
	// Aufstieg daneben: 1013.250
	CreateConstruction(PPT1, 1013, 251, NO_OWNER, 100);
	// Turm Links: 551.370
	CreateConstruction(PPT2, 551, 372, NO_OWNER, 100);
	// Aufstieg unten: 782.670
	CreateConstruction(PPT1, 782, 671, NO_OWNER, 100);
	// Wand unten: 742.670
	CreateConstruction(PPW1, 742, 671, NO_OWNER, 100);
	// Turmgeschütz 970.588
	CreateConstruction(PRFC, 970, 590, NO_OWNER, 100);
	// + gleich Kanone installieren
	var cannon = CreateObject(CTW5);
	FindObject(RFCH) -> ConnectCannon(cannon);
	cannon -> SetR(40);
	// Turm rechts unten: 970.650
	CreateConstruction(PPT2, 970, 652, NO_OWNER, 100);
	// Abgeschlossener Keller 970.588
	CreateConstruction(PPT5, 720, 890, NO_OWNER, 100);
	
	var first, second;
	// Burg aufstieg oben: 720.430
	first = CreateConstruction(_SCS, 720, 430, NO_OWNER, 100);
	// Burg aufstieg unten: 624.670
	second = CreateConstruction(_SCS, 624, 670, NO_OWNER, 100);
	first->AddWayOut(second);
	second->AddWayOut(first);
	// Höhle durchgang links: 860.930
	first = CreateConstruction(CAVE, 860, 930, NO_OWNER, 100);
	// Höhle durchgang rechts: 1123.960
	second = CreateConstruction(CAVE, 1123, 970, NO_OWNER, 100);
	first->AddWayOut(second);
	second->AddWayOut(first);
	// Fahrstuhl in Turm rechts: 973.250
	CreateConstruction(_CEL, 973, 210, NO_OWNER, 100);
	var case = FindObject2(Find_ID(ELEC), Find_AtPoint(974, 227));
	case->SetPosition(GetX(case), 237);
	case->UpdateRangeTop();
	// Höhlen fahrstuhl: 932.800
	CreateConstruction(_CEL, 932, 800, NO_OWNER, 100);
	// Fahrstuhl brunnen: 1195.680
	CreateConstruction(ELEV, 1195, 680, NO_OWNER, 100);
	// Fahrstuhl links: 58.610
	CreateConstruction(ELEV, 58, 610, NO_OWNER, 100);
	// Zelt
	CreateConstruction(TENT, 405, 820, NO_OWNER, 100);
	
	// Flintwerfer
	var disp = CreateConstruction(_FLT, 600, 980, NO_OWNER, 100);
	disp->SetDir(1);
	disp->SetR(-90);
	disp->SetItems([[FLNT, 20], [SFLN, 10], [EFLN, 5], [TFLN, 10], [STFN, 5]]);
	disp->SetPower(10, 0);
	disp->SwitchOn();
	// Drachenstatue
	CreateConstruction(_DST, 1250, 681, NO_OWNER, 100);
	
	// Tore löschen
	for (var obj in FindObjects(Find_Or(Find_ID(CPGL), Find_ID(CPGR), Find_ID(PPB1)))) RemoveObject(obj);
	
	// Fundamente löschen
	for (var obj in FindObjects(Find_Or(Find_ID(BASE), Find_ID(BSC1), Find_ID(PPB1)))) RemoveObject(obj);
	RemoveObject(FindObject2(Find_ID(ELBS), Find_AtPoint(919, 804)));
	RemoveObject(FindObject2(Find_ID(ELBS), Find_AtPoint(986, 212)));
	
	// Burgteile in den Hintergrund
	for (var castle in FindObjects(Find_Or(Find_ID(PPW1), Find_ID(PPW3), Find_ID(PPT1), Find_ID(PPT2),  Find_ID(PPT5))))
		castle->SetCategory(C4D_Background);
	
	// Unkaputtbares:
	for (var obj in FindObjects(Find_Or(
		Find_ID(PPW1), Find_ID(PPW3), Find_ID(PPT1), Find_ID(PPT2), Find_ID(PRFC), Find_ID(CPTE), Find_ID(PPT5), // Burgteile
	))) AddEffect("Invulnerable", obj, 1);
	for (var obj in FindObjects(Find_Or(
		Find_ID(ELEC), Find_ID(ELBS), Find_ID(ELEV), Find_ID(_CEL), // Fahrstühle
		Find_ID(OVEN), Find_ID(FRGE),	Find_ID(CHST), Find_ID(_CST) // Ofen, Forge, Kiste
	))) AddEffect("Invulnerable", obj, 1);
}

global func GenerateCharacters()
{
	var char=CreateCharacter(KING, 865, 280, [BRED,1, COKI,2, FLNT,2, SFLN,1, EFLN,1, TFLN,2, STFN,1, FBMP,1, ARPD,1, ZAPN,1], "$NpcKing$");
	char->AddMiniAI(1|2|4, 855, 930, Object(103), 860, DIR_Left);
	
	char = CreateCharacter(SCLK, 176, 600, [PMAN,2, PHEA,1, PIMM,1, SCRL,6, BOOK,2, LFAM,1], "David Blaine");
	char->AddMiniAI(1|16, 140, 200,,,,,[MLGT, ELX1, GGHG, MBOT, MFRB, GVTY, ABLA, MSSH, MDBT, MMTR, GZ9Z]);
	
	char = CreateCharacter(KNIG, 660, 660, [BOW1,1, ARWP,2, FARP,1, XARP,1, _TAR,1, SPER,1, AXE1,1, SWOR,1, SHIE,1, SCMT,1, SHI2,1, MUSK,1, BLTP,2, SBLP,1, PBLP,1], "$NpcKnight$");
	CreateContents(SHIE, char)->Activate(char); // Ritter hat natürlich ein Schild
	char->AddMiniAI(1|8, 530, 750,,,,Object(205));

	char = CreateCharacter(ASAS, 1260, 810, [CRBW,1, HOOK,1, BOTP,2, NBTP,1, SBTP,1, KNFP,2, BPCK,1, CLUB,1, _BMR,1, MUSH,1], "Ezio");
	char->AddMiniAI(1|32, 1230, 1330);

	
	/*var char;
	
	// Der König
	char = PlaceAnimal(KING);
	char->SetPosition(870, 280);
	char->SetColor(Random(12));
	PrepareShop(char, [FLNT,2, SFLN,2, EFLN,1, TFLN,2, STFN,1, ARPD,1, BRED,1, CBRD,1]);
	
	// Gelehrter
	char = PlaceAnimal(SCLK);
	char->SetPosition(176, 600);
	char->SetColor(Random(12));
	PrepareShop(char, [PMAN,3, PHEA,1, WMPF,2, MUSH,2, SCRL,7, BOOK,3]);
	
	// Ritter
	char = PlaceAnimal(KNIG);
	char->SetPosition(660, 660);
	CreateContents(SHIE, char)->Activate(char); // Schild ausrüsten
	char->SetColor(Random(12));
	PrepareShop(char, [SPER,1, COKI,2, AXE1,1, SWOR,1, SHIE,1, SCMT,1, SHI2,1, MUSK,1, BLTP,2, SBLP,1, PBLP,1, CRBW,1, BOTP,1, NBTP,1, SBTP,1, BOW1,1, ARWP,2, FARP,1, XARP,1]);
	return 1;*/
}

global func CreateCharacter(id idChar, int iX, int iY, array aItems, string szName)
{
	var char = PlaceAnimal(idChar);
	char->SetPosition(iX, iY);
	char->SetColor(Random(12));
	char->SetObjectLayer(char);
	// ..und ist unbesiegbar
	char->AddEffect("Invulnerable", char, 500);
	// spezielles Objekt verwaltet den Handel
	var merch = CreateObject(STRE);
	merch->SetMenuClonk(char);
	merch->SetShopItems(aItems);
	merch->SetName(szName);
	return char;
}

// Invulnerable

global func FxInvulnerableEffect(string szNewEffectName, object pTarget) {
  // keine negativen Effekte
  if(WildcardMatch(szNewEffectName,"*Curse*") || WildcardMatch(szNewEffectName,"*NSpell"))
  {
    Sound("Magic1", 0, pTarget, 50);
    CastParticles("PSpark", 20, 20, GetX(pTarget), GetY(pTarget), 55, 65);
    return -1;
  }
  // kein Feuer
  if(WildcardMatch(szNewEffectName,"*Fire*"))
  {
  	return -1;
  }
  // gleichen Effekt übernehmen
  if(szNewEffectName == "Invulnerable") return -2;
}

global func FxInvulnerableDamage(object pTarget, int iNr, int iDmg, int iCause)
{
	return 0; // nix kann einem was anhaben!
}


global func PlaceItems()
{
	// nix geschieht :(
}


global func SetShopItems(array aItems, object pShop)
{
	if (!pShop && this) pShop = this;
	var order = [];
	for (var i; i < GetLength(aItems); i+=2) {
		pShop -> AddItems(aItems[i], aItems[i+1]);
		order[GetLength(order)] = aItems[i];
	}
	// Angebot sortieren
	pShop -> SetOrder(order);
	return 1;
}

/*global func CreateChestContent(object pChest)
{
	var nr = ObjectNumber(pChest);
	// Spezialitems
	var obj;
	if (nr == 119) { // Es sollte immer ein Tarnzauber bereitliegen
		obj = FindObject2(Find_ID(SCRL), Find_Container(pChest));
		if (!obj || LocalN("idSpell", obj) != CMFG) {
			obj = CreateContents(SCRL, pChest);
			obj  -> SetSpell(CMFG);
			return 1;
		}
	}
	// Sonst eher zufällig
	var ID = RndChestID(pChest);
	obj = CreateContents(ID, pChest);
	// Schriftrollen und Zauberbücher mit Zaubern belegen
	if (ID == SCRL || ID == BOOK) obj -> SetSpells();
	return 1;
}

global func RndChestID(object pChest)
{//GOLD, ORE1, ROCK, CRYS, PFIS, METL
//BRMR, TSWD
	// Spezialkisten
	var IDs, nr = ObjectNumber(pChest);
	if (nr == 128) 	// Neben der Burgbäckerei
		IDs = [[COKI, CBRD], [BRED, MUSH, CSNK, FLNT, COOD], [TFLN, FBMP, WMPF, AXE1, CLUB, SPER, DOGH], [FLOU, WDWB, COAL, PWIP, PHEA], [ARWP, BOTP, BLTP, STFN, SFLN]];
	else if (nr == 120) // Hier legen die Gelehrten und Weisen ihre magischen Sachen ab
		IDs = [[SCRL, PMAN], [BOOK, SCRL, PHEA, PFIR, MUSH, FLNT], [BOOK, PWIP, PSTO, PMON, TFLN], [LFAM, ARWP, BOTP, BLTP, STFN, SFLN], [SCKZ]];
	else if (nr == 127) // Kiste in rechts unten in der Kristallöhle
		IDs = [[CRYS, FLNT, SCRL], [HLGN, SCRL, BOOK, SFLN, TFLN, STFN, PHEA], [BHLG, BOOK, EFLN, PIMM, SPHR, LFAM], [BRMR, TSWD], [SCKZ]];
	else if (nr == 119) // Geheimgang in der Burg
		IDs = [[SCRL, PMAN, PHEA, FLNT, SFLN], [SCRL, BOOK, STFN, SBTP, XARP], [EFLN, ARPD, PWIP, PFIR, PMON, FARP, PBLP], [LFAM, ARMR, TSWD], [BRMR]];
	// default
	else IDs = [[FLNT, SFLN, TFLN, COKI, ARWP], [STFN, FARP, CSNK, CBRD, PFIR, PHEA, PMAN, PSTO, SCRL], [EFLN, BRED, BOTP, BLTP, SBLP, XARP, KNFP, SPER, PMON, PWIP, BOOK], [FBMP, ARPD, NBTP, PBLP, SCMT, SHI2, BOW1, MUSK, AXE1, _BMR], [SBTP, CRBW, HOOK, ARMR, SHIE, SWOR, CLUB]];	
	// Seltener als selten :D
	if (!Random(32))
		return IDs[4][Random(GetLength(IDs[4]))];
	// Selten
	if (!Random(12))
		return IDs[3][Random(GetLength(IDs[3]))];
	// Normal
	if (!Random(6))
		return IDs[2][Random(GetLength(IDs[2]))];
	// Oft
	if (!Random(3))
		return IDs[1][Random(GetLength(IDs[1]))];
	// default
	return IDs[0][Random(GetLength(IDs[0]))];
}*/
