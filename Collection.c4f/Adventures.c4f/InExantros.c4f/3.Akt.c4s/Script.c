/*-- DämonenWelt --*/

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

static gleben;

static gSave; //Für die Speicherung der Objekte beim Wechseln der Map

static gGrad; //Schwierigkeitsstufe

static gxout;
static gyout;

static gxstart;
static gystart;

static pforte1,pforte2,pforte3;

func Initialize() {

  var min_net2_version = 225;
  if (C4X_Ver1==4 && C4X_Ver2==9 && C4X_Ver3==6 && C4X_Ver4==0 && C4X_VerBuild < min_net2_version)
    {
    Message("@Bitte NET2-Version aktualisieren. Mindestens Version %d benötigt!", 0, min_net2_version);
    RemoveAll();
    return(GameOver());
    }


  gAkt=3; //3.Akt
//Schwierigkeitsgrad
  gGrad=1;
//Warppoint
  gxout=50;
  gyout=140;
//Startpoint
  gxstart=50;
  gystart=140;

  gleben=0;
  var i=0; //Nebel

//  SetGamma( RGB(10,10,50),RGB(70,70,120),RGB(140,140,230));

  while(i<200) { CreateParticle("Fog",Random(LandscapeWidth()),Random(LandscapeHeight()),0,0,RandomX(900,1700)); ++i; }
  SetSkyParallax(0,11,10,0,0,0,-1300); //Himmel

  CreateObject(_Q0_,0,0,-1); //MainQuest
  CreateObject(_BQ_,0,0,-1); //BigQuestion
  CreateObject(SCAH,0,0,-1); //Klettern
//  CreateObject(SURR,0,0,-1); //Aufgeben
  CreateObject(TACC,0,0,-1); //Gemeinschaftskonto
  CreateObject(_SG_,0,0,-1); //Schwierigkeitsgrad

  CreateObject(S__A,0,0,-1); //Hud ein/aus

  ObjectCall(CreateObject(DKR_,3147,484,-1),"Dreh");
  ObjectCall(CreateObject(DKR_,3122,399,-1),"Dreh");
  SetVar(1,CreateObject(DKR_,3060,532,-1));
  ObjectCall(Var(1),"Dreh");
  SetLocal(2,1,Var(1));
  SetVar(1,CreateObject(DKR_,3103,468,-1));
  SetColorDw(RGBa(255,255,255,100),Var(1));
  SetLocal(2,1,Var(1));

//Die 5 Pforten
  pforte1=CreateObject(DPF_,660,290,-1);
  pforte2=CreateObject(DPF_,1890,340,-1);
  pforte3=CreateObject(DPF_,2430,340,-1);
//Lavaquellen
  CreateObject(W_00,310,450,-1);
  CreateObject(W_00,550,450,-1);

  CreateObject(W_00,1240,500,-1);
  CreateObject(W_00,1240,500,-1);

  CreateObject(W_00,2090,500,-1);
  CreateObject(W_00,2230,500,-1);

  CreateObject(W_00,3200,500,-1);
  CreateObject(W_00,3020,500,-1);
//Wegpunkte
  CreateObject(W_01,170,150,-1);
//Hintergrundsound
  SoundLevel("DamonWorld",99);
//Farbwechsel
  CreateObject(W_05,0,0,-1);
  return(1);
}
RelaunchPlayer: //wenn man stirbt verliert man 50 erfahrung
  SetPlrViewRange(200,Par(0));
  SetPosition(gxstart+Random(10),gystart,Par(0));
//Leben und Mana voll
  DoEnergy(+100,Par(0));
  var wt =100;
  while(wt>0) { DoMagicEnergy(+1,Par(0)); --wt; }
//relaunch weniger
  --gleben;
  if(gleben<0)  ObjectCall(Par(0),"GoOut"); //Raus schmeißen!
  if(gleben==0)  return(Log("%s ist gestorben und ihr habt keinen Relaunch mehr",GetPlayerName(GetOwner(Par())) ));
  if(gleben>1)  return(Log("%s ist gestorben und ihr habt noch %d Relaunches",GetPlayerName(GetOwner(Par())),gleben));
  Log("%s ist gestorben und ihr habt noch 1 Relaunch",GetPlayerName(GetOwner(Par())) );
  return(1);

InitializePlayer:
//Relaunches
  gleben=gleben+10;
//FogofWar
  SetFoW(1,Par(0));
//  if(gmap==0)  SetPlrViewRange(60,FindObjectOwner(KNIG,Par(0)) );
  SetPlrViewRange(200,FindObjectOwner(KNIG,Par(0)) );
//Rucksack
  if(!FindObjectOwner(_BAG,Par(0)) )  CreateObject(_BAG,50,50,Par(0));
// Baupläne
  DefinitionCall(WPPL, "SetKnowledge", Par());
  DefinitionCall(CPPL, "SetKnowledge", Par());
//Leben und Mana voll!
  DoEnergy(+100,FindObjectOwner(KNIG,Par(0)) );
  var wt =100;
  while(wt>0) { DoMagicEnergy(+1,FindObjectOwner(KNIG,Par(0)) ); --wt; }
//Position
  SetPosition(gxstart+Random(10),gystart,FindObjectOwner(KNIG,Par(0)));
//Gespeicherte Ausrüstung erzeugen
  ObjectCall(FindObjectOwner(KNIG,Par(0)),"SpeicherErstellung");
//Sound
  Sound("JoinPlayer");
  return(1);

Teil1:
  gTagebuch="Ich habe die Dämonenwelt betreten, nun gilt es die letzte Bastion zu erklimmen. Ich werde den dunklen Wanderer töten und all dem Übel hier ein Ende bereiten.||Vor mir stehen noch die 3 Pforten die ich zu öffnen habe.";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");
  gbigquest="Es ist wahr: Die Zwerge haben vor langer Zeit einen Kristall gefunden. Er zog alles Böse an, deswegen wurde er verbannt. Der Wanderer öffnete ein Portal in diese Welt, vodurch eine Verbindung zum Böse entstand und es angezogen wurde.||Jetzt will der Wanderer dem Kristall die Macht entziehen.||Er muss wohl das verschwundene Buch haben, da er alle Geheimnisse des Kristalls kennt.||Fragt sich nur wer der Wanderer ist und wieso er so besessen ist?!";

//Die 5. Pforten

  var i = GetPlayerCount();
  while(i>0) { ObjectCall(CreateObject(KM__),"Kapitel",14,FindObjectOwner(KNIG,i-1) ); --i; }
  return(1);

Teil2:
//Warppoint
  gxout=700;
  gyout=280;
//Startpoint
  gxstart=700;
  gystart=280;

  Win();
  gTagebuch="Die erste Pforte ist geöffnet, scheint wohl ein Kinderspiel zu sein.||Dunkler Wanderer nehm dich in acht, ich muss nur noch 2 öffnen!";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");
//Pforte öffnen
  ObjectCall(pforte1,"Open");
//Wegpunkte
  CreateObject(W_02,650,240,-1);
  return(1);

NextPforte:
//Warppoint
  gxout=1580;
  gyout=210;
//Startpoint
  gxstart=1580;
  gystart=210;
  return(1);

Teil3:
//Warppoint
  gxout=1960;
  gyout=330;
//Startpoint
  gxstart=1960;
  gystart=330;

  CreateObject(W_06,1890,300,-1);

  ObjectCall(pforte2,"Open");
  Win();

  ObjectCall(CreateObject(LI__,2150,200,-1),"Licht",350);

  gTagebuch="Die 2. Pforte steht nun ebenfalls offen.||Nur noch die letzte Pforte.";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");
  return(1);

Teil4:
//Warppoint
  gxout=2630;
  gyout=330;
//Startpoint
  gxstart=2630;
  gystart=330;

  CreateObject(ENDE,2670,320,-1);

  ObjectCall(pforte3,"Open");

  Win();
  gTagebuch="Die letzte Pforte ist geöffnet. Jetzt hängt das Schicksal vieler Bewohner von Exantros in meinen Händen.||Das Ende steht bevor!";
  Message("<c ff0000> Neuer Tagebucheintrag </c>");

//Das letzte Kapitel

  var i = GetPlayerCount();
  while(i>0) { ObjectCall(CreateObject(KM__),"Kapitel",15,FindObjectOwner(KNIG,i-1) ); --i; }

  return(1);

Abspann:
  Music();
//Hintergrundsound
  SoundLevel("DamonWorld",1);
  SoundLevel("EndKampf",1);
  ObjectCall(FindObject(G__E),"Vorbei");
//  while(FindObect(W_00))  RemoveObejct(FindObect(W_00));
  CreateObject(_CLK,2850,340,-1);
  return(1);

DasEnde:
  Music();
  Sound("DasEnde");
  while(FindObject(DKR_))  ObjectCall(FindObject(DKR_),"Kaputt");
  gTagebuch=0;
  ScriptGo(1);
  return(1);

Saving:
  Sound("Win");
  Message("<c 00ff00> DAS SPIEL IST VORBEI! </c>");
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

Script2:
  CreateObject(_AB1,3100,96,-1);
  return(1);

Script22:
  CreateObject(_AB2,3100,96,-1);
  return(1);

Script42:
  CreateObject(_AB3,3100,96,-1);
  return(1);

Script62:
  CreateObject(_AB4,3100,96,-1);
  return(1);

Script82:
  CreateObject(_AB5,3100,96,-1);
  return(1);

Script111:
  CreateObject(_AB6,3100,96,-1);
  return(1);

Script140:
  CreateObject(_AB7,3100,96,-1);
  return(1);

Script178:
  Saving(); //Alle abspeichern
  GameOver();
  return(1);