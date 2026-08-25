/*-- Inexantros --*/

#strict

static gAkt;

static gAuftrag;
static gTagebuch;
static gblock;
static gkopf;
static gfader; //Faderobjekt
static gGegner; //1.Endgegner
static gGang; //Geheimgang
static ggruft; //Friedhof
static gLocked; //Türsperre bei Flunheim
static gLocked2; //Türsperre beim Kloster
static gLocked3; //Türsperre beim Kloster
static gFlunheim; //Hilfsglobal für Flunheim
static gScript,gCode; //Wandscript
static gsaeule;
static gturm;
static gpfand1;
static gnacht,gkloster,gdreieck; //für die sonnenfinsternis
static gtest; //die 3 prüfungen
static gorakel,gbann;
static gmap; //aktuelle Karte
static gSky; //Skyfarbe
static gwaffenkammer,gxtele,gburg; //Wichtelburg
static ghandel,ghandel2,ghandel3;

static ginvasion;
static gzwerg;

static gSave; //Für die Speicherung der Objekte beim Wechseln der Map

static gGrad; //Schwierigkeitsstufe

static gxout;
static gyout;

static gxstart;
static gystart;

func Initialize() {

  var min_net2_version = 225;
  if (C4X_Ver1==4 && C4X_Ver2==9 && C4X_Ver3==6 && C4X_Ver4==0 && C4X_VerBuild < min_net2_version)
    {
    Message("@Bitte NET2-Version aktualisieren. Mindestens Version %d benötigt!", 0, min_net2_version);
    RemoveAll();
    return(GameOver());
    }


  gAkt=1; //1.Akt
  gSky=GetSkyAdjust();
  gGrad=1;
  SetSkyParallax(0,13,10); //Himmel
  CreateObject(TIME,0,0,-1); //Zeit
  CreateObject(_WET,0,0,-1); //Regen

  CreateObject(_Q0_,0,0,-1); //MainQuest

  CreateObject(SCAH,0,0,-1); //Klettern
//  CreateObject(SURR,0,0,-1); //Aufgeben
  CreateObject(TACC,0,0,-1); //Gemeinschaftskonto
  CreateObject(_SG_,0,0,-1); //Schwierigkeitsgrad
  CreateObject(S__A,0,0,-1); //Hud ein/aus

  CreateObject(KUTI,0,0,-1); //Künstliche Zeit, welche so ca. jeden Tag (21000 fps) den Clonk hungern lässt!
//Dunkle Stellen-Kanal
  CreateParticle("Fog",1950,970,0,0,10000,RGB(0,0,75));
  CreateParticle("Fog",1950,970,0,0,10000,RGB(0,0,75));
  CreateParticle("Fog",1950,970,0,0,10000,RGB(0,0,75));
//Dunkle Stellen-Kanal
  CreateParticle("Fog",4140,1150,0,0,10000,RGB(20,10,10));
  CreateParticle("Fog",4140,1150,0,0,10000,RGB(20,10,10));
  CreateParticle("Fog",4140,1150,0,0,10000,RGB(20,10,10));
//Dunkle Stellen-Friedhof
  CreateParticle("Fog",5320,1080,0,0,10000,RGB(0,30,50));
  CreateParticle("Fog",5320,1080,0,0,10000,RGB(0,30,50));
  CreateParticle("Fog",5320,1080,0,0,10000,RGB(0,30,50));
//Dunkle Stellen-Links
  CreateParticle("Fog",570,1040,0,0,10000,RGB(0,40,0));
  CreateParticle("Fog",570,1040,0,0,10000,RGB(0,40,0));
  CreateParticle("Fog",570,1040,0,0,10000,RGB(0,40,0));
//Dunkle Stellen-Eisberg
  CreateParticle("Fog",3830,550,0,0,10000,RGB(0,0,80));
  CreateParticle("Fog",3830,550,0,0,10000,RGB(0,0,80));
  CreateParticle("Fog",3830,550,0,0,10000,RGB(0,0,80));
//Soundquellen
  ObjectCall(CreateObject(SOU_,1430,320,-1),"Tag");
  ObjectCall(CreateObject(SOU_,2390,340,-1),"Tag");
  ObjectCall(CreateObject(SOU_,3030,570,-1),"Tag");
  ObjectCall(CreateObject(SOU_,480,320,-1),"Tag");
  ObjectCall(CreateObject(SOU_,4940,190,-1),"Tag");
  ObjectCall(CreateObject(SOU_,5580,140,-1),"Tag");

  ObjectCall(CreateObject(SOU_,350,1030,-1),"Grotte");
  ObjectCall(CreateObject(SOU_,570,780,-1),"Grotte");
  ObjectCall(CreateObject(SOU_,4150,900,-1),"Grotte");
  ObjectCall(CreateObject(SOU_,3380,1220,-1),"Grotte");

  ObjectCall(CreateObject(SOU_,2100,1060,-1),"Kanal");
  ObjectCall(CreateObject(SOU_,1710,1180,-1),"Kanal");
  ObjectCall(CreateObject(SOU_,1370,980,-1),"Kanal");

  ObjectCall(CreateObject(SOU_,1950,310,-1),"Stadt");

  ObjectCall(CreateObject(SOU_,3960,30,-1),"Sturm");

//Truhe
  SetVar(0,CreateObject(_CST,4190,430,-1));
	CreateContents(PHEA,Var(0));
	if(!Random(3))  CreateContents(_SC6,Var(0));
	CreateContents(_SC6,Var(0));
	if(!Random(3))  CreateContents(_SC6,Var(0));
	CreateContents(_SC6,Var(0));
	CreateContents(_OIL,Var(0));
  ObjectCall(Var(0),"Bild");
//Truhe2
  SetVar(0,CreateObject(_CST,1480,740,-1));
	if(!Random(3))  CreateContents(ARWP,Var(0));
	CreateContents(ARWP,Var(0));
	CreateContents(ARWP,Var(0));

	CreateContents(PFIS,Var(0));
	CreateContents(PFIS,Var(0));
	if(!Random(3))  CreateContents(BR__,Var(0));
	CreateContents(STR_,Var(0));
	CreateContents(METL,Var(0));
//Truhe3
  SetVar(0,CreateObject(_CST,4950,1320,-1));
	if(!Random(3))  CreateContents(NTCK,Var(0));
	if(!Random(3))  CreateContents(_LHA,Var(0));
	if(!Random(3))  CreateContents(_THL,Var(0));
	if(!Random(3))  CreateContents(_QUI,Var(0));
	CreateContents(BOW2,Var(0));
	CreateContents(METL,Var(0));

//Jagdgebiete
  CreateObject(_WIQ,2790,740,-1);
  CreateObject(_WIQ,5460,680,-1);

//FogOfWar (Lichter) +30 für net2
  ObjectCall(CreateObject(LI__,100,170,-1),"Licht",350);
  ObjectCall(CreateObject(LI__,440,300,-1),"Licht",370);
  ObjectCall(CreateObject(LI__,900,300,-1),"Licht",370);
  ObjectCall(CreateObject(LI__,1500,300,-1),"Licht",370);
  ObjectCall(CreateObject(LI__,2100,300,-1),"Licht",370);
  ObjectCall(CreateObject(LI__,2700,300,-1),"Licht",370);
  ObjectCall(CreateObject(LI__,2900,100,-1),"Licht",370);
  ObjectCall(CreateObject(LI__,4800,140,-1),"Licht",280);
//Wegweiser
  SetVar(0,CreateObject(_WG1,2300,530,-1));
  ObjectCall(Var(0),"Bild",2);
  LocalN("ptext",Var(0))="Willkommen in Hingen.||Westlich von Hingen liegt der Hingsee.||Folgt man dem Pfad über die Hängebrücke durch das Gebirge, so gelangt man nach Flunheim.";

  SetVar(0,CreateObject(_WG1,4390,890,-1));
  ObjectCall(Var(0),"Bild",4);
  LocalN("ptext",Var(0))="Folgt man dem Pfad dem Wasserfall hoch, so gelangt man nach Flunheim.||Der Tunnel zu euer Linken führt nach Hingen.";

  SetVar(0,CreateObject(_WG1,4870,410,-1));
  ObjectCall(Var(0),"Bild",5);
  LocalN("ptext",Var(0))="Flunheim:||Willkommen Reisender, in unserer Taverne >Wipfgrog< findet ihr eine warme Unterkunft.||Im Westen hinter dem Eisberg liegt Hingen.||Nach Osten gelangt man in den Schattenwald zur Wichtelburg.";

  SetVar(0,CreateObject(_WG1,2560,1150,-1));
  ObjectCall(Var(0),"Bild",3);
  LocalN("ptext",Var(0))="Die Kanalisation:||Gebaut vom Ingenieur Zwerg Udoakor.||Das Sichellabyrinth:||Das Sichellabyrinth wurde ebenfalls von ihm errichtet. Hier wird jedes Jahr die berühmte Slayer-Ehrenprüfung veranstaltet. Die jenigen die durchkommen und sich als würdig erweisen, werden in den Rang der Elite-Slayer-Zwerge aufgenommen.||Gezeichnet: Zwergen Hauptmann Oberkum";

  SetVar(0,CreateObject(_WG1,1330,940,-1));
  ObjectCall(Var(0),"Bild",3);
  LocalN("ptext",Var(0))="Das Sichellabyrinth:||Hier wird jedes Jahr die berühmte Slayer-Ehrenprüfung veranstaltet. Die jenigen die durchkommen und sich als würdig erweisen, werden in den Rang der Elite-Slayer-Zwerge aufgenommen.||Gezeichnet: Zwergen Hauptmann Oberkum|||Hinzugefügt: Diese Ruinen sind aus Vorsichtsmaßnahmen geschlossen! Gezeichnet: Bürgermeister Marvin";

//Manaquellen
  CreateObject(_MQU,500,830,-1);
  CreateObject(_MQU,1710,970,-1);
  CreateObject(_MQU,1679,530,-1);
  CreateObject(_MQU,2920,1070,-1);
  CreateObject(_MQU,4220,910,-1);
  CreateObject(_MQU,4640,480,-1);
  CreateObject(_MQU,5440,920,-1);

//Truhe
  SetVar(0,CreateObject(_CST,2190,1310,-1)); //Versteckte extra Startausrüstung, da es am Anfang sehr schwer ist.
	CreateContents(PHEA,Var(0));
	CreateContents(PHEA,Var(0));
	CreateContents(PHEA,Var(0));

	CreateContents(BR__,Var(0));
	CreateContents(BR__,Var(0));

  if(!Random(3))  CreateContents(PHEA,Var(0));
  if(!Random(3))  CreateContents(PHEA,Var(0));
  if(!Random(2))  CreateContents(PMAN,Var(0));
	CreateContents(YESH,Var(0));
  if(!Random(3))  CreateContents(_SBA,Var(0));
  if(!Random(3))  CreateContents(SHWD,Var(0));


//Fahrstühle
  CreateObject(ELEV,57,504,-1);
  CreateObject(ELEV,1367,912,-1);
  CreateObject(ELEV,4467,537,-1);
  CreateObject(ELEV,5587,9728,-1);

//Die Stadt Hingen
  ObjectCall(CreateObject(LOT_,1790,520,-1),"NoHome"); //Der Priester

  SetVar(0,CreateObject(_SH5,1767,530,-1));
	SetVar(1,CreateContents(_BW1,Var(0)));
	SetVar(2,CreateObject(HA__,0,0,-1));
	ghandel=Var(2);
	SetLocal(2,Var(1),Var(2));
	CreateContents(_KSW,Var(2)); //Verkaufsgegenstände
	CreateContents(STAF,Var(2));
	CreateContents(_QUI,Var(2));
	CreateContents(ARWP,Var(2));
	CreateContents(ARWP,Var(2));
	CreateContents(ARWP,Var(2));
	CreateContents(BOW1,Var(2));
	CreateContents(SHWD,Var(2));

	CreateContents(ARWP,Var(1));
	CreateContents(SHRT,Var(1));
	CreateContents(STAF,Var(1));
	CreateContents(SHWD,Var(1));
	CreateContents(_QUI,Var(1));
	CreateContents(BOW1,Var(1));

	CreateContents(CLUB,Var(1));
	CreateContents(_STK,Var(1));
	CreateContents(WOOD,Var(1));

	CreateContents(BR__,Var(1));
	CreateContents(CO__,Var(1));


	LocalN("ihaus",Var(1))=Var(0); //Händler
  SetVar(0,CreateObject(_SH1,2009,530,-1));
	SetVar(1,CreateContents(_BME,Var(0)));
	LocalN("ihaus",Var(1))=Var(0); //Bürgermeister
  CreateObject(_SH6,1890,530,-1);

  SetVar(0,CreateObject(_SH2,2122,530,-1));
	SetVar(1,CreateContents(_MEZ,Var(0)));
	LocalN("ihaus",Var(1))=Var(0); //Metzger
	SetVar(2,CreateObject(HA__,0,0,-1));
	ghandel2=Var(2);
	SetLocal(2,Var(1),Var(2));
	CreateContents(_LHA,Var(2));
	CreateContents(_SBA,Var(2));
	CreateContents(YESH,Var(2));
	CreateContents(TRSR,Var(2));
	CreateContents(SHRT,Var(2));
	CreateContents(_SBA,Var(2));
	CreateContents(_THL,Var(2));

	CreateContents(SHRT,Var(1));
	CreateContents(TRSR,Var(1));
	CreateContents(KP4R,Var(1));

	CreateContents(BR__,Var(1));
	CreateContents(BR__,Var(1));
	CreateContents(BR__,Var(1));
	CreateContents(CO__,Var(1));

  CreateObject(_SH4,2201,530,-1);

  SetVar(0,CreateObject(_HUE,2390,530,-1));
	SetVar(1,CreateContents(_PEN,Var(0)));
	LocalN("ihaus",Var(1))=Var(0); //Penner

  SetVar(0,CreateObject(AMB_,2070,530,-1)); //Amboss+Baupläne
	CreateContents(_KSW,Var(0));
	CreateContents(_STK,Var(0));
	CreateContents(_THL,Var(0));
	CreateContents(KP4R,Var(0));
	CreateContents(ARWP,Var(0));
	CreateContents(FARP,Var(0));
	CreateContents(AL__,Var(0));
	CreateContents(_QUI,Var(0));

  CreateObject(_ESE,2020,530,-1); //Esse
  CreateObject(_LQU,1970,530,-1); //Lebensquelle

//Die 2.Stadt Flunheim
  CreateObject(_HH4,5197,430,-1);
  CreateObject(_HH3,5108,430,-1);
  CreateObject(_HH1,5300,430,-1);
  CreateObject(_HH2,5386,430,-1);
  CreateObject(_HH5,5447,430,-1);
  SetVar(0,CreateObject(_TUR,5504,430,-1));
//	ObjectCall(Var(0),"ControlLeft");
//	ObjectCall(Var(0),"ControlRight");
  gturm=CreateObject(_TUR,5027,430,-1);
	ObjectCall(gturm,"ControlLeft");
	ObjectCall(gturm,"ControlRight");

//Das Kloster
  SetVar(0,CreateObject(_KL5,3444,183,-1));
  SetVar(1,CreateObject(HTU_,3320,410,-1));
  SetLocal(2,Var(1),Var(0));
  SetLocal(2,Var(0),Var(1));
  SetVar(0,CreateObject(_KL1,3610,183,-1));
  SetVar(1,CreateObject(HTU_,3645,590,-1));
  SetLocal(2,Var(1),Var(0));
  SetLocal(2,Var(0),Var(1));
  SetVar(0,CreateObject(_KL2,3769,183,-1));
  SetVar(1,CreateObject(HTU_,3700,430,-1));
  SetLocal(2,Var(1),Var(0));
  SetLocal(2,Var(0),Var(1));
  CreateObject(_KL4,3861,183,-1);
  CreateObject(_KL4,3309,183,-1);
  CreateObject(_KL3,3409,257,-1);
  CreateObject(_LAG,4440,150,-1);
  CreateObject(_REG,3727,590,-1);
  CreateObject(_REG,3584,590,-1);

//Sonstiges
  gblock=CreateObject(_BOK,150,531,-1);
  CreateObject(_GWA,2500,840,-1); //Altar
  CreateObject(_GWA,4440,150,-1); //Altar

  CreateObject(_BTF,1780,450,-1); //Schmetterling
  CreateObject(_BTF,2100,440,-1); //Schmetterling
  CreateObject(_BTF,1980,410,-1); //Schmetterling
  CreateObject(SLBT,910,560,-1); //Boot
  CreateObject(_BRD,1560,1034,-1); //Hängebrücke
  CreateObject(GAG_,2010,800,-1); //Gang
  CreateObject(BRU_,1790,530,-1); //Brunnen
  CreateObject(FW__,4681,899,-1); //Wasserfall
  CreateObject(_BRD,4740,575,-1); //Hängebrücke
  CreateObject(_WSC,4610,560,-1); //Wandscript
  CreateObject(_ID2,3170,1100,-1); //Wandscript
  CreateObject(KB__,3019,834,-1); //Kaputte Brücke
  ObjectCall(CreateObject(KB__,3115,832,-1),"Rechts"); //Kaputte Brücke rechts
  CreateObject(_SAL,3130,821,-1); //Säule
  CreateObject(_GRU,3180,810,-1); //Gruft
  CreateObject(_ID3,4890,890,-1); //Idol3
  CreateObject(_ID4,4220,910,-1); //Idol4
  CreateObject(_GRV,5310,900,-1); //Grabstein-Durchgang

//Monolithen
  CreateObject(MM__,1880,860,-1); //MegaMonolith
  ObjectCall(CreateObject(MN__,4190,1080,-1),"Rune",S_03,"Flunheim"); //Flunheim
  ObjectCall(CreateObject(MN__,3380,1060,-1),"Rune",S_25,"Schlucht"); //Schlucht
  ObjectCall(CreateObject(MN__,4360,300,-1),"Rune",S_09,"Kloster Ziokar"); //Kloster Ziokar
  ObjectCall(CreateObject(MN__,1040,1270,-1),"Rune",S_21,"Sichellabyrinth"); //Fallengrotte

//Falltore
  SetLocal(2,1,CreateObject(FT__,2295,970,-1));
  SetLocal(2,1,CreateObject(FT__,1305,930,-1));
  SetLocal(2,1,CreateObject(FT__,2055,950,-1));
  SetLocal(2,1,CreateObject(FT__,4015,850,-1));
  CreateObject(FT__,1795,850,-1);
  CreateObject(FT__,2605,1010,-1);
  CreateObject(FT__,1925,1190,-1);
  CreateObject(FT__,4505,850,-1);
  CreateObject(SC__,4450,865,-1);
  CreateObject(FT__,3305,1250,-1);
  CreateObject(FT__,3215,1080,-1);
  CreateObject(FT__,3335,850,-1);
  CreateObject(FT__,4935,880,-1);

//Kleiner Tümpel
  CastPXS("Acid",850,30,1470,1240);
  CastPXS("Acid",700,30,1710,1240);

//Warppoint
  gxout=1640;
  gyout=520;
//Startpoint
  gxstart=1640;
  gystart=520;
  return(1);
}

RelaunchPlayer: //wenn man stirbt verliert man 50 erfahrung
  var q=55+(gGrad*5);

//Falls er durch verhungern starb, dann nicht wieder gleich abnippeln lassen, sondern ein bissi Kraft wieder geben!
  if(LocalN("phunger",Par(0) )<40)  LocalN("phunger",Par(0) )=40; //ein wenig im Bauch

  SetPlrViewRange(60,Par(0));
  SetPosition(gxstart+Random(10),gystart,Par(0));

  DoEnergy(+100,Par(0));
  var wt =100;
  while(wt>0) { DoMagicEnergy(+1,Par(0)); --wt; }

  SetVar(0,LocalN("pExp",Par(0)) );
  if(Var(0)<q)  Log("%s ist gestorben und verliert %d Erfahrungspunkte",GetPlayerName(GetOwner(Par()) ),Var(0));
  if(Var(0)>q-1)  Log("%s ist gestorben und verliert %d Erfahrungspunkte",GetPlayerName(GetOwner(Par()) ),q);
  LocalN("pExp",Par(0))=LocalN("pExp",Par(0))-q;
  if(LocalN("pExp",Par(0))<0)  LocalN("pExp",Par(0))=0;
  return(1);

InitializePlayer:
//Hunger
  LocalN("phunger",FindObjectOwner(KNIG,Par(0)) )=100; //Vollen Bauch!
//FogofWar
  SetFoW(1,Par(0));
  if(gmap==0)  SetPlrViewRange(60,FindObjectOwner(KNIG,Par(0)) );
  if(gmap==0)  SetPlrViewRange(90,FindObjectOwner(KNIG,Par(0)) );
//Rucksack
  if(!FindObjectOwner(_BAG,Par(0)) )  CreateObject(_BAG,50,50,Par(0));
// Baupläne
  DefinitionCall(WPPL, "SetKnowledge", Par());
  DefinitionCall(CPPL, "SetKnowledge", Par());
//Waffe erzeugen
  CreateContents(_KSW,GetHiRank(Par(0)));
//Heiltrank erzeugen
  CreateContents(PHEA,GetHiRank(Par(0)));
//Warprolle erzeugen
  ObjectCall(CreateContents(SCRL,GetHiRank(Par(0))),"SetSpell",MGWP);
//Brot als Startproviant
  CreateContents(BR__,GetHiRank(Par(0)));
//Position
  SetPosition(gxstart+Random(10),gystart,FindObjectOwner(KNIG,Par(0)));
//Sound
  Sound("JoinPlayer");
//Starten
  if(!gfader)  ScriptGo(1);
  return(1);

Script1: //einblenden
  if(gfader)  return(ScriptGo(0));
  gfader=CreateObject(SFA_,10,10,-1);
  ObjectCall(gfader,"ScreenFadeIn");
//Nachricht
  var i=-1, iPlr;
  while (++i < GetPlayerCount())
  {
  iPlr=GetPlayerByIndex(i);
  ObjectCall(CreateObject(KM__),"StartAkt",8,FindObjectOwner(KNIG,iPlr) );
  }

  ScriptGo(0);
  return(1);

Teil1:
//Den Dorfältesten suchen
  Log("Kapitel 1: Die suche nach dem Dorfältesten");

  var i=-1, iPlr;
  while (++i < GetPlayerCount())
  {
  iPlr=GetPlayerByIndex(i);
  ObjectCall(CreateObject(KM__),"Kapitel",1,FindObjectOwner(KNIG,iPlr) );
  }

  gAuftrag=1; //Erster Teil des Auftrags
  CreateObject(H_01,10,10,-1);
  CreateObject(H_03,150,500,-1);
  return(1);

Teil2:
//Kapitel beendet
  Log("Ihr habt den Dorfältesten befreit");
  gAuftrag=2;
  SetVar(0,FindObject(_SH4));
	SetVar(1,CreateContents(_DAE,Var(0)));
	CreateContents(PHEA,Var(1)); //Verkaufsgegenstände
	CreateContents(PHEA,Var(1));
	CreateContents(PHEA,Var(1));
	CreateContents(PMAN,Var(1));
	CreateContents(PMAN,Var(1));
	CreateContents(PMAN,Var(1));
	CreateContents(FLNT,Var(1));
	CreateContents(FLNT,Var(1));
	CreateContents(SCRL,Var(1));

	CreateContents(AL__,Var(1));

	ObjectCall(Var(1),"Scroll");
	ObjectCall(Var(1),"Scroll");
	ObjectCall(Var(1),"ScrollWarp");
	ObjectCall(Var(1),"ScrollWarp");
	ObjectCall(Var(1),"ScrollWarp");

	LocalN("ihaus",Var(1))=Var(0); //Dorfälteste
//der Priester Lothar ist abgereist, wird nicht mehr gebraucht!
  if(FindObject(LOT_))  RemoveObject(FindObject(LOT_));
//Neue Verkaufsgegenstände
  CreateContents(NTCK,ghandel);
//amboss
	CreateContents(CLUB,FindObject(AMB_)); //keule
	CreateContents(YESH,FindObject(AMB_)); //schuhe
  return(1);

Teil3:
//Die Suche nach dem Teleporter
  Log("Kapitel 2: Der Monolith");

  var i=-1, iPlr;
  while (++i < GetPlayerCount())
  {
  iPlr=GetPlayerByIndex(i);
  ObjectCall(CreateObject(KM__),"Kapitel",2,FindObjectOwner(KNIG,iPlr) );
  }

  gAuftrag=3;
  CreateObject(H_04,2520,670,-1);
  CreateObject(MO__,0,0,-1);
//Neue Verkaufsgegenstände
  CreateContents(_GUG,ghandel2);
  CreateContents(_GUG,ghandel2);

  SetVar(1,CreateContents(_PER,FindObject(_DAE)));
  ObjectCall(Var(1),"SetSpell",MFBL);

  CreateObject(_BQ_,0,0,-1); //BigQuestion
  gbigquest="Irgendetwas stimmt in dieser Gegend nicht, aber ich werde der Sache auf den Grund gehen.";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");

  CreateObject(EX_1,1840,840,-1);
  return(1);

Teil4:
//Kapitel beendet
  Log("Ihr habt den Monolithen und den Monostone gefunden.");
  gAuftrag=4;
  CreateObject(H_09,4190,1080,-1);
//Neue Verkaufsgegenstände
  CreateContents(_KST,ghandel);
  CreateContents(_KST,ghandel);
  CreateContents(SHSM,ghandel);
  CreateContents(SPER,ghandel);
  CreateContents(MORS,ghandel);
  CreateContents(_FEL,ghandel2);
  CreateContents(BOW2,ghandel);
  CreateContents(AXE1,ghandel2);

  SetVar(1,CreateContents(_PER,FindObject(_DAE)));
  ObjectCall(Var(1),"SetSpell",GZ9Z);

//amboss
	CreateContents(AXE1,FindObject(AMB_)); //Akt
	CreateContents(MORS,FindObject(AMB_)); //Flegel
	CreateContents(_FEL,FindObject(AMB_)); //Fellrüstung
  return(1);

Teil5:
//Der Weg von Flunheim
  Log("Kapitel 3: Der Weg von Flunheim");


  var i=-1, iPlr;
  while (++i < GetPlayerCount())
  {
  iPlr=GetPlayerByIndex(i);
  ObjectCall(CreateObject(KM__),"Kapitel",3,FindObjectOwner(KNIG,iPlr) );
  }

  gAuftrag=5;
//pilze wachsen jetzt auch
  CreateObject(_PST,3020,1020,-1); //Pilzstelle
  return(1);

Teil6:
//Kapitel beendet
  Log("Der Magier ist besiegt und nun können die Paladine die Stadt Hingen anlegen!");
  if(FindObject(SLBS))  RemoveObject(FindObject(SLBS));
  if(FindObject(SLBT))  RemoveObject(FindObject(SLBT));
  CreateObject(SIF_,1415,604,-1);

  SetVar(0,FindObject(_SH6)); //Haus wird beansprucht!
	SetVar(1,CreateContents(_HAU,Var(0)));
	LocalN("ihaus",Var(1))=Var(0); //Hauptmann

  CreateObject(H_13,10,10,-1);
//Neue Verkaufsgegenstände
  CreateContents(_HEL,ghandel);
  CreateContents(BOW2,ghandel);
  CreateContents(RECK,ghandel2);
  gAuftrag=6;
//Eintrag
  gbigquest="Hier in diesem Landstrich wimmelt es wohl in letzter Zeit nur so von Räubern. Sie scheinen wie benommen und verzaubert zu sein. Und jetzt treibt auch noch ein Magier hier sein Unwesen.||Ich will der Sache auf den Grund gehen.";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");
  return(1);

Teil7:
//Der große Angriff
  Log("Kapitel 4: Der große Angriff");

  var i=-1, iPlr;
  while (++i < GetPlayerCount())
  {
  iPlr=GetPlayerByIndex(i);
  ObjectCall(CreateObject(KM__),"Kapitel",4,FindObjectOwner(KNIG,iPlr) );
  }

  CreateObject(H_14,2980,770,-1);
  CreateObject(H_15,4010,840,-1);
  gAuftrag=7;
//Neue Verkaufsgegenstände
  CreateContents(_NST,ghandel);

//amboss
	CreateContents(_HEL,FindObject(AMB_)); //streithelm
	CreateContents(SHSM,FindObject(AMB_)); //flegel
  return(1);

Teil8:
//Der große Angriff ist vorbei
//  Log("Flunheim ist zurück erobert.");
  SetVar(0,FindObject(_HH2)); //Haus wird beansprucht!
	SetVar(1,CreateContents(UNT_,Var(0)));
	LocalN("ihaus",Var(1))=Var(0); //Unteroffizier

  SetVar(0,FindObject(_HH5)); //Haus wird beansprucht!
	SetVar(1,CreateContents(_ALM,Var(0)));
	LocalN("ihaus",Var(1))=Var(0); //Alter Mann

  SetVar(0,FindObject(_HH4)); //Haus wird beansprucht!
	SetVar(1,CreateContents(_WI_,Var(0)));
	LocalN("ihaus",Var(1))=Var(0); //Wirt

  gLocked=1; //alle türen auf!
  gAuftrag=8;
//Neue Verkaufsgegenstände
  CreateContents(_SCP,ghandel);
  CreateContents(SWOR,ghandel);
  CreateContents(SABR,ghandel);
  return(1);

Teil9:
//Die Sonnenfinsternis
  Log("Kapitel 5: Die schwarze Sonne");

  var i=-1, iPlr;
  while (++i < GetPlayerCount())
  {
  iPlr=GetPlayerByIndex(i);
  ObjectCall(CreateObject(KM__),"Kapitel",5,FindObjectOwner(KNIG,iPlr) );
  }

  gAuftrag=9;
//Eintrag
  gbigquest="Etwas absolut Merkwürdiges geht hier vor. Die Sonne ist auf einmal verdunkelt, als käme ein Schatten übers Land...||Dieses merkwürdige Rästel scheint größer zu sein als ich dachte.";
  Message("<c ff0000> Neuer Tagebucheintrag </c>||<c 00ff00> Auftrag erfüllt! </c>");

  RemoveObject(FindObject(_Q5_));

//amboss
	CreateContents(SWOR,FindObject(AMB_)); //Schwert
	CreateContents(HEL3,FindObject(AMB_)); //Hörnerhelm
	CreateContents(_SCP,FindObject(AMB_)); //Schuppenpanzer
  return(1);

Teil10:
//Kloster erreicht
  Log("Ihr habt den Weg zum Kloster gefunden");
  gAuftrag=10;
//Neue Verkaufsgegenstände
  CreateContents(_LKU,ghandel);
  CreateContents(SHIE,ghandel);
  CreateContents(BOW4,ghandel);
//amboss
	CreateContents(BOW4,FindObject(AMB_)); //Armbrust
	CreateContents(XARP,FindObject(AMB_)); //Elfenpfeile
  return(1);

Teil11:
//Die 3 Prüfungen
  Log("Kapitel 6: Die 3 Prüfungen");

  var i=-1, iPlr;
  while (++i < GetPlayerCount())
  {
  iPlr=GetPlayerByIndex(i);
  ObjectCall(CreateObject(KM__),"Kapitel",6,FindObjectOwner(KNIG,iPlr) );
  }

  gAuftrag=11;
  CreateObject(H_22,0,0,-1);
  gtest=1;
  return(1);

Teil12:
//Bestanden
  Log("Ihr habt die 3 Prüfungen, und den Schrein erleuchtet");
  gAuftrag=12;
  gtest=0;
//Neue Verkaufsgegenstände
  CreateContents(_ZST,ghandel);
  return(1);

Teil13:
//Das Orakel
  Log("Kapitel 7: Das Orakel");

  var i=-1, iPlr;
  while (++i < GetPlayerCount())
  {
  iPlr=GetPlayerByIndex(i);
  ObjectCall(CreateObject(KM__),"Kapitel",7,FindObjectOwner(KNIG,iPlr) );
  }

  CreateObject(H_26,0,0,-1);
  gAuftrag=13;
//Tagebuch
  gbigquest="Alle Bewohner scheinen ratlos zu sein, selbst der Dorfälteste weiß kein Rat. Die Antwort auf das ganze Geschehen muss jetzt wohl das Orakel beantworten. Ich habe viele Geschichten über viele Orakels gehört, und dass man sich erst als würdig erweisen muss, bevor man seine Antworten bekommt.||Ich will nun die Antwort finden!";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");
  return(1);

Teil14:
//Das Orakel besiegt und die Antworten bekommen
  Log("Ihr habt die Antwort des Orakels bekommen");
  CreateObject(H_30,3550,60,-1);
  CreateObject(H_31,0,0,-1);
  gAuftrag=14;
  gLocked3=1;

//amboss
	CreateContents(SHIE,FindObject(AMB_)); //Ritterschild
	CreateContents(SPER,FindObject(AMB_)); //Speer
  return(1);

Teil15:
//Die Bibliothek
  Log("Kapitel 8: Die geheime Bibliothek");

  var i=-1, iPlr;
  while (++i < GetPlayerCount())
  {
  iPlr=GetPlayerByIndex(i);
  ObjectCall(CreateObject(KM__),"Kapitel",8,FindObjectOwner(KNIG,iPlr) );
  }
  gAuftrag=15;
//Tagebuch
  gbigquest="Das Orakel gab mir zu verstehen, dass ein Wanderer ein Portal in eine Dämonenwelt öffnete, um die Kraft eines Kristalls zu holen. Er sollte das Böse gerufen haben. Nun ist er im Schattenwald und der Weg dorthin sei versperrt...||Das klingt für mich sehr verwirrend, ich brauche wohl noch mehr andere Informationen.";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");
  return(1);

Teil16:
//Barriere gesprengt
  Log("Ihr habt die Barriere gesprengt");

  gTagebuch="Ich habe die Barriere gesprengt, nun sollte ich in den Schattenwald wandern und dort den Urspung des Bösen finden.||Wandern:|Alle Spieler müssen nun im Regel-Menü zustimmen, dass sie bereit sind. Wenn alle bereit sind, und einer der Gruppe die Karte rechts verlässt, wird der nächste Akt freigeschaltet.||ACHTUNG:|Alles was nicht eingesammelt ist, wird liegen gelassen. (Ihr kommt nicht wieder hierher zurück!)";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");

  CreateObject(_WA_,0,0,-1);
  CreateObject(H_41,5990,350,-1);

  gAuftrag=16;
  return(1);

Saving:
  GainMissionAccess("Schattenwald");
  Sound("Win");
  Message("<c 00ff00> DU HAST DEN 2.AKT FREIGESPIELT </c>");
  while(SetVar(0,FindObject(KNIG,0,0,0,0,0,0,0,0,Var(0))))  ObjectCall(Var(0),"Speicher"); //Alle Spieler abspeichern
  GameOver(); //Spiel vorbei!
//  while(SetVar(0,FindObject(KNIG,0,0,0,0,0,0,0,0,Var(0))))  ObjectCall(Var(0),"GoOut"); //Alle Spieler raus schmeißen
  return(1);

Win:
  Sound("Win");
  return(1);

Horn:
  Sound("Horn");
  return(1);

Quake:
  Sound("Earthquake");
  return(1);

Sonne:
  Sound("Horror1");
  gnacht=1;
  gTagebuch="Die Sonne ist verdunkelt, ich befürchte ziemlich Schlimmes. Ich sollte sofort den Dorfältesten fragen was hier vor sich geht.||Der Auftrag von Klodwig hat keine Bedeutung mehr.";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");
  while(SetVar(0,FindObject(LI__,0,0,0,0,0,0,0,0,Var(0))))  ObjectCall(Var(0),"LichtOut");
  return(1);

Sonne2:
//NeueLichter (Kloster wurde erhellt!)
  ObjectCall(CreateObject(LI__,3370,100,-1),"Licht",170);
  ObjectCall(CreateObject(LI__,3530,100,-1),"Licht",170);
  ObjectCall(CreateObject(LI__,3740,100,-1),"Licht",170);
  ObjectCall(CreateObject(LI__,4000,100,-1),"Licht",170);
  ObjectCall(CreateObject(LI__,4200,70,-1),"Licht",170);
  ObjectCall(CreateObject(LI__,4360,10,-1),"Licht",170);
  ObjectCall(CreateObject(LI__,4500,10,-1),"Licht",170);

  Sound("Gong");
  gnacht=0;
  gTagebuch="Die Sonne leuchtet wieder, jetzt trauen sich die Untoten wohl nicht mehr ans Tageslicht. Wenigstens ist es jetzt ein bisschen sicherer.||Ich sollte trotzdem den Dorfältesten aufsuchen und fragen was eigentlich der Grund des ganzen >merkwürdigen< Geschehens ist.";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");
  while(SetVar(0,FindObject(LI__,0,0,0,0,0,0,0,0,Var(0))))  ObjectCall(Var(0),"LichtOn");
  return(1);

//TagesAnbruch und Abbruch!

NewNight:
  return(1);

NewDay:
  while(SetVar(0,FindObject(KNIG,0,0,0,0,0,0,0,0,Var(0))))  ObjectCall(Var(0),"NeuerHunger",-15-(gGrad*2)-Random(10)  ); //Par(0) =Den Hunger den der Clonk bekommt
  return(1);