/*-- Inexantros --*/

#strict

static gAkt;

static gAuftrag;
static gTagebuch;
static gblock;
static gkopf;
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


  gAkt=2; //der 2.Akt!
  gSky=GetSkyAdjust();
  gGrad=1;
  CreateObject(TIME,0,0,-1); //Zeit
  var i=0; //Nebel
  while(i<200) { CreateParticle("Fog",Random(LandscapeWidth()),Random(LandscapeHeight()),0,0,RandomX(900,1700)); ++i; }

//Standart Sachen
  SetSkyParallax(0,11,10); //Himmel

  CreateObject(_GL_,0,0,-1); //Gottheit

  CreateObject(_Q0_,0,0,-1); //MainQuest
  CreateObject(_BQ_,0,0,-1); //BigQuestion

  CreateObject(SCAH,0,0,-1); //Klettern
//  CreateObject(SURR,0,0,-1); //Aufgeben
  CreateObject(TACC,0,0,-1); //Gemeinschaftskonto
  CreateObject(_SG_,0,0,-1); //Schwierigkeitsgrad
  CreateObject(S__A,0,0,-1); //Hud ein/aus

  CreateObject(KUTI,0,0,-1); //Künstliche Zeit, welche so ca. jeden Tag (21000 fps) den Clonk hungern lässt!
//Monolithen
  CreateObject(MM__,3590,430,-1); //MegaMonolith
  ObjectCall(CreateObject(MN__,1050,960,-1),"Rune",S_03,"Unterholz"); //Unterholz
  ObjectCall(CreateObject(MN__,3790,1100,-1),"Rune",S_24,"Zwergenhalle"); //Zwergenreich
  ObjectCall(CreateObject(MN__,3430,1430,-1),"Rune",S_23,"Zwergenbibliothek"); //Zwergenreich2
//Wegweiser

  SetVar(0,CreateObject(_WG1,190,470,-1));
  ObjectCall(Var(0),"Bild",4);
  LocalN("ptext",Var(0))= "Der Schattenwald:||Im Westen liegt das Hochland, wo der Reisende nach Flunheim gelangt.||Folgt man dem Pfad nach Osten so gelangt man zur Wichtelburg. Folge dem Weg durchs Tor und halte dich stets oben. Nachdem du die Brücke passiert hast geht es nur noch durch den Wald.";

  SetVar(0,CreateObject(_WG1,1270,320,-1));
  ObjectCall(Var(0),"Bild",5);
  LocalN("ptext",Var(0))= "Weg oben:||Hier gelangt man zur Wichtelburg.||Weg unten:||Dieser Pfad für in das Unterholz vom Schattenwald. Dort unten befindet sich ebenfalls auch das alte Zwergenreich. Reisende und Forscher die die Ruinen besichtigen wollen, betreten es aufgrund der Einsturtzgefahr, auf eigene Gefahr!";

  SetVar(0,CreateObject(_WG1,3200,340,-1));
  LocalN("ptext",Var(0))= "Willkommen Reisender in der Wichtelburg:||Hier regiert Graf Alfons II. Er ist Landeigentümer vom Schattenwald. Die Burg wurde von seinem Vater Graf Alfons I erbaut.||Regeln:||Diebstahl wird mit der Hand bezahlt, also Halunkenpack, seid gewarnt!";

//extra mit der brücke
  SetVar(0,CreateObject(_WG1,3200,340,-1));
  LocalN("ptext",Var(0))= "Der Mondschien in einigen Zyklen der Zeit zeigt einem den Weg, welcher einen Schatz in sich birgt.";


  SetVar(0,CreateObject(_WG1,1930,1070,-1));
  ObjectCall(Var(0),"Bild",5);
  LocalN("ptext",Var(0))= "Unterholz:||Der kleine Pfad durch das Tor links führt zum alten Zwergenreich.||Lange bevor wir Clonks hierher kamen, lebten in diesen Landstrichen die Zwerge. Heute sind sie alle auf mysteriöse Weise in diesen Landstrich ausgestorben.||Viele alte Runinen und Mauerwerke sowie Tunnel sind die Überreste dieser alten Kultur.";

  SetVar(0,CreateObject(_WG1,960,1130,-1));
  LocalN("ptext",Var(0))= "Hier endet der Pfad:||Viele die die 3 Kronen der 3 Zwergenkönige suchen glauben hinter dieser gewaltigen Wand fündig zu werden. Doch Niemand konnte sie bis jetzt durchdringen.||Gezeichnet: Dr.Prof. Sucher";

//Soundquellen
  ObjectCall(CreateObject(SOU_,410,200,-1),"Tag");
  ObjectCall(CreateObject(SOU_,740,240,-1),"Tag");
  ObjectCall(CreateObject(SOU_,1270,160,-1),"Tag");
  ObjectCall(CreateObject(SOU_,1750,120,-1),"Tag");
  ObjectCall(CreateObject(SOU_,2240,100,-1),"Tag");
  ObjectCall(CreateObject(SOU_,2770,130,-1),"Tag");
  ObjectCall(CreateObject(SOU_,3060,120,-1),"Tag");
  ObjectCall(CreateObject(SOU_,4260,130,-1),"Tag");
  ObjectCall(CreateObject(SOU_,570,570,-1),"Grotte");


  ObjectCall(CreateObject(SOU_,550,950,-1),"Grotte");
  ObjectCall(CreateObject(SOU_,630,1340,-1),"Tag");
  ObjectCall(CreateObject(SOU_,1610,1010,-1),"Tag");
  ObjectCall(CreateObject(SOU_,3230,1180,-1),"Grotte");
  ObjectCall(CreateObject(SOU_,3800,1010,-1),"Grotte");
  ObjectCall(CreateObject(SOU_,4000,1350,-1),"Grotte");


//Zwergenfriedhof
  SetVar(0,CreateObject(_ID9,1130,1320,-1));
  LocalN("ptext",Var(0))="Grohom der Harte:||Er soll in Frieden ruhen und niemehr gestört werden.||Rutschi-Putschi";
  CreateContents(_ZKR,Var(0));

  SetVar(0,CreateObject(_ID9,450,1160,-1));
  LocalN("ptext",Var(0))="Zwingli der Große:||Sein Gold fließe sein Wille geschehe.||Möge er in der Ewigkeit ruhen.";
  CreateContents(_ZKR,Var(0));
  ObjectCall(Var(0),"Bild1");

  SetVar(0,CreateObject(_ID9,170,1160,-1));
//  CreateContents(_ZKR,Var(0));
  LocalN("ptext",Var(0))="Wickim von Hammer:||Glorreiche Jahre hatten wir Zwerge mit ihm, möge er nun ruhen.";
//FogOfWar (Lichter)
  ObjectCall(CreateObject(LI__,3650,130,-1),"Licht",320);
  ObjectCall(CreateObject(LI__,4240,130,-1),"Licht",320);

  ObjectCall(CreateObject(LI__,100,130,-1),"Licht",320);
  ObjectCall(CreateObject(LI__,440,130,-1),"Licht",340);
  ObjectCall(CreateObject(LI__,900,130,-1),"Licht",340);
  ObjectCall(CreateObject(LI__,1500,130,-1),"Licht",330);
  ObjectCall(CreateObject(LI__,2100,130,-1),"Licht",340);
  ObjectCall(CreateObject(LI__,2700,130,-1),"Licht",335);
  ObjectCall(CreateObject(LI__,3100,130,-1),"Licht",340);

//Jagdgebiete
  CreateObject(_WIQ,2590,270,-1);
  CreateObject(_WIQ,430,310,-1);
  CreateObject(_WIQ,1620,1440,-1);
  CreateObject(_WIQ,2840,340,-1);


//Truhe
  SetVar(0,CreateObject(_CST,110,820,-1));
	CreateContents(PHEA,Var(0));
	CreateContents(SPER,Var(0));
	CreateContents(METL,Var(0));
	CreateContents(_TSH,Var(0));
  SetVar(3,Random(2));
  if(Var(3)==1)  CreateContents(_BLR,Var(0));
  if(Var(3)==2)  CreateContents(BHLM,Var(0));



  SetVar(0,CreateObject(_ID9,1950,960,-1));
  LocalN("ptext",Var(0))="Memento dem Schützen.";
	CreateContents(BOW3,Var(0));
  ObjectCall(Var(0),"Bild1");


  SetVar(0,CreateObject(_ID9,2950,1240,-1));
	CreateContents(_RAX,Var(0));
  LocalN("ptext",Var(0))="Die Axt ist der beste Freund des Zwerges.";
  ObjectCall(Var(0),"Bild1");


  SetVar(0,CreateObject(_ID9,1190,1050,-1));
  LocalN("ptext",Var(0))="Schutz gebührt den Helden.";
	CreateContents(_TSH,Var(0));
  ObjectCall(Var(0),"Bild1");

//Händler im wald
  SetVar(0,CreateObject(_SH3,1220,320,-1));
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

  CreateContents(NTCK,ghandel);
  CreateContents(_GUG,ghandel);
  CreateContents(_GUG,ghandel);
  CreateContents(_KST,ghandel);
  CreateContents(SHSM,ghandel);
  CreateContents(SPER,ghandel);
  CreateContents(MORS,ghandel);
  CreateContents(_FEL,ghandel);
  CreateContents(BOW2,ghandel);
  CreateContents(AXE1,ghandel);

  CreateContents(_HEL,ghandel);
  CreateContents(BOW2,ghandel);
  CreateContents(RECK,ghandel);
  CreateContents(_NST,ghandel);

  CreateContents(_SCP,ghandel);
  CreateContents(SWOR,ghandel);
  CreateContents(SABR,ghandel);


	SetVar(2,ghandel);
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

	LocalN("ihaus",Var(1))=Var(0); //Händler

//Wichtelburg
  SetVar(0,CreateObject(AMB_,3500,290,-1));

//der kram vom 1. akt
	CreateContents(_KSW,Var(0));
	CreateContents(_STK,Var(0));
	CreateContents(_THL,Var(0));
	CreateContents(KP4R,Var(0));
	CreateContents(ARWP,Var(0));
	CreateContents(FARP,Var(0));
	CreateContents(AL__,Var(0));
	CreateContents(_QUI,Var(0));

	CreateContents(CLUB,FindObject(AMB_)); //keule
	CreateContents(YESH,FindObject(AMB_)); //schuhe

	CreateContents(AXE1,FindObject(AMB_)); //Akt
	CreateContents(MORS,FindObject(AMB_)); //Flegel
	CreateContents(_FEL,FindObject(AMB_)); //Fellrüstung

	CreateContents(_HEL,FindObject(AMB_)); //streithelm
	CreateContents(SHSM,FindObject(AMB_)); //flegel

	CreateContents(SWOR,FindObject(AMB_)); //Schwert
	CreateContents(HEL3,FindObject(AMB_)); //Hörnerhelm
	CreateContents(_SCP,FindObject(AMB_)); //Schuppenpanzer

	CreateContents(BOW4,FindObject(AMB_)); //Armbrust
	CreateContents(XARP,FindObject(AMB_)); //Elfenpfeile

	CreateContents(SHIE,FindObject(AMB_)); //Ritterschild
	CreateContents(SPER,FindObject(AMB_)); //Speer

	CreateContents(_JST,FindObject(AMB_)); //Meisterstab

//Neuer Kram zum 2. akt
	CreateContents(ARMR,Var(0)); //gestechrüstung
	CreateContents(_TSW,Var(0)); //Zweihänder
	CreateContents(HELM,Var(0)); //plattenhelm

  CreateObject(_ESE,3430,290,-1);
  SetVar(0,CreateObject(_SHM,3560,290,-1));
	SetVar(1,CreateContents(BLS_,Var(0)));
	SetVar(2,CreateObject(HA__,0,0,-1));
	SetLocal(2,Var(1),Var(2));
	CreateContents(SABR,Var(2)); //Verkaufsgegenstände
	CreateContents(ARWP,Var(2));
	CreateContents(ARWP,Var(2));
	CreateContents(ARWP,Var(2));
	CreateContents(SPER,Var(2));
	CreateContents(_ZST,Var(2));
	CreateContents(_TSW,Var(2));
	CreateContents(SWOR,Var(2));
	CreateContents(AXE2,Var(2));
	CreateContents(BOW1,Var(2));
	CreateContents(BOW4,Var(2));
	CreateContents(BOW2,Var(2));
	CreateContents(CRBW,Var(2));
	CreateContents(_SCP,Var(2));
	CreateContents(ARMR,Var(2));
	CreateContents(MGCK,Var(2));

	CreateContents(MGCK,Var(1));
	CreateContents(KP4R,Var(1));

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

	CreateContents(_MBO,Var(1));
	CreateContents(BRBT,Var(1));

  CreateContents(_LKU,Var(1));
  CreateContents(SHIE,Var(1));
  CreateContents(BOW4,Var(1));
  CreateContents(_ZST,Var(1));

	LocalN("ihaus",Var(1))=Var(0); //Schmied
  CreateObject(H_50,0,0,-1); //Stadtwachen
  CreateObject(XBOW,4030,190,-1); //Crossbow

  CreateObject(GAF_,3730,100,-1); // Ja der Graf ist kein Punkrocker, er ist ein Feigling!
  SetVar(0,CreateObject(HTU_,3647,290,-1));
  SetVar(1,CreateObject(HTU_,3590,110,-1));
  SetLocal(2,Var(1),Var(0));
  SetLocal(2,Var(0),Var(1));
  SetVar(0,CreateObject(HTU_,3755,290,-1));
  SetVar(1,CreateObject(HTU_,3660,430,-1));
  SetLocal(2,Var(1),Var(0));
  SetLocal(2,Var(0),Var(1));
//Sonstiges

  CreateObject(_LQU,3370,290,-1); //Lebensquelle
  CreateObject(_HEB,1074,300,-1); //Hebel
  CreateObject(H_39,3640,270,-1); //Wichtelburg
  CreateObject(_WC_,930,1297,-1);
  CreateObject(_WC_,820,1087,-1);
  CreateObject(_WC_,820,1377,-1);

  CreateObject(_PST,650,280,-1); //Pilzstelle
  CreateObject(_PST,2510,240,-1); //Pilzstelle 2

  CreateObject(_GWA,1610,230,-1); //Altar
//Falltore
  CreateObject(FT__,2175,670,-1);
  CreateObject(FT__,2225,1090,-1);
  CreateObject(SC__,2270,1105,-1);
  CreateObject(FT__,1105,300,-1);
  CreateObject(BFT_,4015,310,-1);

  ObjectCall(CreateObject(FT__,4015,800,-1),"Bild2");
  ObjectCall(CreateObject(FT__,3375,1260,-1),"Bild2");
//Fahrstühle
  ObjectCall(CreateObject(ELEV,3757,792,-1),"Bild");
//Warppoint
  gxout=570;
  gyout=290;
//Startpoint
  gxstart=15;
  gystart=370;
//Manaquellen
  CreateObject(_MQU,3841,292,-1);
  CreateObject(H_51,0,0,-1);
//Alle Türengraphiken umstellen
  while(SetVar(0,FindObject(FT__,0,0,0,0,0,0,0,0,Var(0))))  ObjectCall(Var(0),"Bild");
  return(1);

RelaunchPlayer: //wenn man stirbt verliert man 50 erfahrung
  var q=60+(gGrad*6);

//Falls er durch verhungern starb, dann nicht wieder gleich abnippeln lassen, sondern ein bissi Kraft wieder geben!
  if(LocalN("phunger",Par(0) )<40)  LocalN("phunger",Par(0) )=40; //ein wenig im Bauch

  DoEnergy(+100,Par(0));
  var wt =100;
  while(wt>0) { DoMagicEnergy(+1,Par(0)); --wt; }

  SetPlrViewRange(60,Par(0));
  SetPosition(gxstart+Random(10),gystart,Par(0));

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
//
  DoEnergy(+100,FindObjectOwner(KNIG,Par(0)) );
  var wt =100;
  while(wt>0) { DoMagicEnergy(+1,FindObjectOwner(KNIG,Par(0)) ); --wt; }
// Baupläne
  DefinitionCall(WPPL, "SetKnowledge", Par());
  DefinitionCall(CPPL, "SetKnowledge", Par());
//Position
  SetPosition(gxstart+Random(10),gystart,FindObjectOwner(KNIG,Par(0)));
//Gespeicherte Ausrüstung erzeugen
  ObjectCall(FindObjectOwner(KNIG,Par(0)),"SpeicherErstellung");
//Sound
  Sound("JoinPlayer");
  return(1);

Teil17:
//Der Schattenwald
  Log("Kapitel 9: Der Schattenwald");

  var i=-1, iPlr;
  while (++i < GetPlayerCount())
  {
  iPlr=GetPlayerByIndex(i);
  ObjectCall(CreateObject(KM__),"Kapitel",9,FindObjectOwner(KNIG,iPlr) );
  }

  gAuftrag=17;
  CreateObject(H_35,350,490,-1);

  gTagebuch="Als erstes muss ich nach Wichtelburg gelangen. Dort sollte ich mir Auskunft über die Lage hier holen.||Ich wurde vor meiner Ankunft hierher gewarnt, dass es hier sehr Gefährlich sein kann!";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");

//Tagebuch
  gbigquest="Das Orakel gab mir zu verstehen, dass ein Wanderer ein Portal in eine Dämonenwelt öffnete um die Kraft eines Kristalls zu holen. Er sollte das Böse gerufen haben. Nun ist er im Schattenwald und der Weg dorthin sei versperrt...||Das klingt für mich sehr verwirrend, ich brauche wohl noch mehr andere Informationen.";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");
  return(1);

Teil18:
//Den Schattenwald durchquert
  Log("Ihr habt den Weg nach Wichtelburg gefunden.");
  gAuftrag=18;
//Warppoint
  gxout=3450;
  gyout=280;
//Startpoint
  gxstart=3450;
  gystart=280;
  return(1);

Teil19:
//Die dunkle Armee
  Log("Kapitel 10: Die dunkle Armee");

  var i=-1, iPlr;
  while (++i < GetPlayerCount())
  {
  iPlr=GetPlayerByIndex(i);
  ObjectCall(CreateObject(KM__),"Kapitel",10,FindObjectOwner(KNIG,iPlr) );
  }

  gAuftrag=19;
  CreateObject(H_37,0,0,-1);

//Punkte
  ginvasion=200;

  gTagebuch="Heute Nach sollte die dunkle Armee die Wichtelburg angreifen.|| Ich sollte zusehen dass ich dann hier bin um die Burg zu beschützen.";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");
  gbigquest="Es Scheint Krieg zwischen Gut und Böse hier ausgebrochen zu sein.||Es wird wohl immer mehr und immer schlimmeres Böses hierher gerufen.||Ich muss heraus finden warum es hierher gerufen wurde. Ambesten ich muss den Wanderer finden.";
  return(1);

Teil20:
//Die dunkle Armee besiegt
  Log("Wichtelburg hat den Angriff der dunklen Armee standgehalten");
  gAuftrag=20;
  Sound("DarkArmyBack");
  gTagebuch="Wichtelburg hat den Angriff der dunklen Armee standgehalten, jedoch hat sie großen Schaden davon getragen.||Mir wurde versichert, dass je weniger die Burg verstört wurde, desto mehr Geld würde ich für meine Hilfe bekommen. Ich wollte mir die Belohnung von Graf Alfons abholen.";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");
  SetVar(1,CreateObject(_DAE,3760,100,-1));
	CreateContents(AL__,Var(1));
  CreateObject(H_44,0,0,-1);
  return(1);

Teil21:
//Das Zwergenreich
  Log("Kapitel 11: Das Zwergenreich");

  var i=-1, iPlr;
  while (++i < GetPlayerCount())
  {
  iPlr=GetPlayerByIndex(i);
  ObjectCall(CreateObject(KM__),"Kapitel",11,FindObjectOwner(KNIG,iPlr) );
  }

  gAuftrag=21;
//Extra ist freigeschaltet
  CreateObject(G_B_,575,837,-1);
  return(1);

Teil22:
//Die Zwergenkrohnen
  Log("Kapitel 12: Die Drei Zwergenkrohnen");

  var i=-1, iPlr;
  while (++i < GetPlayerCount())
  {
  iPlr=GetPlayerByIndex(i);
  ObjectCall(CreateObject(KM__),"Kapitel",12,FindObjectOwner(KNIG,iPlr) );
  }

  gTagebuch="Um in In die Hallen zu kommen, wo der Schatz der Zwerge gehortet wurde, muss ich die drei Kronen der Zwergenkönige finden.||Vielleicht sollte ich den Dorfältesten um Rat fragen.";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");

  gbigquest="Es ist wahr: Die Zwerge haben vor langer Zeit einen Kristall gefunden. Er zog alles Böse an, deswegen wurde er verbannt. Der Wanderer öffnete ein Portal in diese Welt, wodurch eine Verbindung zum Böse entstand und es angezogen wurde.||Jetzt will der Wanderer dem Kristall die Macht entziehen.||Er muss wohl das verschwundene Buch haben, da er alle Geheimnisse des Kristalls kennt.||Fragt sich nur wer der Wanderer ist und wieso er so besessen ist?!";

  CreateObject(H_55,0,0,-1);
  gAuftrag=22;

  return(1);

Teil23:
//Der Drachenhort
  Log("Kapitel 13: Das Portal");

  var i=-1, iPlr;
  while (++i < GetPlayerCount())
  {
  iPlr=GetPlayerByIndex(i);
  ObjectCall(CreateObject(KM__),"Kapitel",13,FindObjectOwner(KNIG,iPlr) );
  }

  gTagebuch="Ich habe den Weg in die Schatzkammer der Zwerge gefunden. Hoffentlich ist dort der Wanderer, ansonsten irre ich noch ewig hier rum!";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");
  gAuftrag=23;

  SetVar(0,CreateObject(WA__,3320,1430,-1));
  SetVar(1,CreateObject(WA__,3870,1090,-1));
  ObjectCall(Var(0),"Effekt");
  SetLocal(0,Var(0),Var(1));
  SetLocal(0,Var(1),Var(0));
  CreateObject(H_58,3870,1090,-1); //Drachenendgegner
  CreateObject(XBOW,3950,1130,-1); //2X Crossbow
  CreateObject(XBOW,4130,1130,-1);

  CastObjects(DU__,5+Random(3),80,4090,1070);
  CastObjects(SI__,37+Random(6),100,4090,1070);
  CastObjects(GOLD,12+Random(3),80,4090,1070);
  return(1);

Teil24:
//Der Drache ist tot.
  Log("Ihr habt den Geisterdrachen besiegt.");

  gTagebuch="Ich habe den Geisterdrachen besiegt!||Rechts vom Schatz hat sich ein Wurmloch geöffnet, wo das wohl hin führt.|Hoffentlich war das die richtige Spur!";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");
  CreateObject(H_41,4170,1400,-1);
  CreateObject(POR_,4180,1340,-1);
  CreateObject(POF_,4080,1500,-1);
  CreateObject(POH_,3860,1390,-1);
  SetVar(0,CreateObject(WA__,4270,1130,-1));
  SetVar(1,CreateObject(WA__,3750,1430,-1));
  ObjectCall(Var(0),"Effekt");
  SetLocal(0,Var(0),Var(1));
  SetLocal(0,Var(1),Var(0));
  gAuftrag=24;
  return(1);

Teil25: //Der letzte Teil vom 2.Akt
  gTagebuch="Das Portal in die Dämonenwelt wurde geöffnet. Jetzt ist unsere einzige Change, dass ich selbst durch das Portal gehe und den Wanderer daran hindere dem Kristall seine Macht zu entziehen.||Wandern:|Alle Spieler müssen nun im Regel-Menü zustimmen, dass sie bereit sind. Wenn alle bereit sind, und einer der Gruppe durch das Protal geht, wird der nächste Akt freigeschaltet.||ACHTUNG:|Alles was nicht eingesammelt ist, wird liegen gelassen. (Ihr kommt nicht wieder hierher zurück!)";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");
  gbigquest="Der Wanderer hat das Portal beschwört. Doch es ist noch nicht zu spät. Ich werde ihn in die Welt folgen und ihn hindern, wenn es sein muss töten.";

//Truhe
  SetVar(0,CreateObject(_CST,3810,1430,-1));
	CreateContents(PHEA,Var(0));
	CreateContents(SPER,Var(0));
	CreateContents(METL,Var(0));
	CreateContents(METL,Var(0));
  ObjectCall(Var(0),"Bild");
//Truhe2
  SetVar(0,CreateObject(_CST,3790,1430,-1));
	CreateContents(_SCP,Var(0));
	CreateContents(TRSR,Var(0));
	CreateContents(ARWP,Var(0));
	CreateContents(ARWP,Var(0));
	CreateContents(BOW4,Var(0));
	CreateContents(SHIE,Var(0));
	CreateContents(METL,Var(0));
  ObjectCall(Var(0),"Bild");

  CreateObject(_WA_,0,0,-1);
  return(1);

Saving:
  GainMissionAccess("Damonwelt");
  Sound("Win");
  Message("<c 00ff00> DU HAST DEN 3.AKT FREIGESPIELT </c>");
  while(SetVar(0,FindObject(KNIG,0,0,0,0,0,0,0,0,Var(0))))  ObjectCall(Var(0),"Speicher"); //Alle Spieler abspeichern
  GameOver();
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

//TagesAnbruch und Abbruch!

NewNight:
  return(1);

NewDay:
  while(SetVar(0,FindObject(KNIG,0,0,0,0,0,0,0,0,Var(0))))  ObjectCall(Var(0),"NeuerHunger",-15-(gGrad*2)-Random(10)  ); //Par(0) =Den Hunger den der Clonk bekommt
  return(1);