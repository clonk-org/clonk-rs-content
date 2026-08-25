/*-- RdW-Lernrunde --*/

#strict

static g_pLastArrow,g_pGoal;

func Initialize()
{
  DigFreeRect(145,260,55,10);
  SetSkyParallax(0,13,11,0,0,0,0);
  ScriptGo(1);
  g_pGoal=FindObject2(Find_ID(SCRG));
  if(!g_pGoal) g_pGoal=CreateObject(SCRG,5,5,-1);
}

func InitializePlayer(iPlr)
{
  SetPosition(320,-20,GetHiRank(iPlr));
}

func Script4()
{
  Sound("Boing");
  dayspeed=0;
  return(1);
}

func Script10()
{
  Message("Moin %s!|Das ist ein Clonk...| ... aber er unterscheidet sich von den Clonks den du vielleicht kennt. Er ist... anders.",0,GetTaggedPlayerName());
  return(1);
}

func Script30()
{
  Message("Denn er muss essen und schlafen, kann krank werden, regeneriert sich usw.");
  return(1);
}

func Script50()
{
  Message("In der oberen linken Ecke seht du wie viel Hunger er verspürt, wie müde er ist, und so weiter.");
  SetArrow(100,250,true,true);
  return(1);
}

func Script80()
{
  CreateContents(BANN,GetHiRank());
  SetArrow(64,-64,true);
  Message("Was der Clonk da im Inventar hat ist das Banner.|Wenn du es aktiviert bekommst du die Grundlage für eine Siedlung:|Eine Kneipe und eine Hütte deren Position selbst bestimmt werden kann");
  return(1);
}

func Script100()
{
  RemoveArrow();
  Sound("Ding");
  g_pGoal->SetMessages("", "Aktiviere das Banner und platziere deine Gebäude");
  Message("@Benutzt nun das Banner.");
  return(1);
}

func Script105()
{
  if(ObjectCount2(Find_ID(_I1A),Find_OCF(OCF_Fullcon)))
    return goto(120);
  if(!ObjectCount2(Find_ID(HCKT)))
    return goto(102);
  Message("@Nun befindet sich im Inventar des Clonks 2x kostenlos Bauen. Eine Hütte und eine Kneipe... platziere sie.");
}

func Script120()
{
  if(!ObjectCount2(Find_ID(_I1A),Find_OCF(OCF_Fullcon)))
    return goto(115);
  Message("");
  return(1);
}

func Script125()
{
  Sound("Applause");
  return(1);
}

func Script140()
{
  Message("Dein Clonk hat Hunger bekommen...");
  GetHiRank()->CheckFood(-1300);
  return(1);
}

func Script150()
{
  Message("@Gehe in die Kneipe und kauf ihm ein Brot.");
  Sound("Ding");
  g_pGoal->SetMessages("", "Gehe in die Kneipe und kaufe deinem hungrigen Clonk Brot und lassen sie den Clonk das Brot essen");
  var pBar=FindObject(_I1A);
  SetArrow(GetX(pBar),GetY(pBar)-40,false);
  SetPlrShowCommand(0,3);
  return(1);
}

func Script154()
{
  if(GetID(Contents(0,GetHiRank()))!=BRED)
     return goto(151);
  SetPlrShowCommand(0,0);
  Message("");
}

func Script156()
{
  if(Contained(GetHiRank()))
    return goto(155);
  Message("@Gehe aus der Kneipe...");
}

func Script158()
{
  if(Contained(GetHiRank()))
    return goto(155);
  RemoveArrow();
  Message("@Esse das Brot mit doppelt <c ffff00>%s</c>",0,GetPlrControlName(0, CON_Dig));
  SetPlrShowCommand(0,128+6);
}

func Script165()
{
  if(GetHiRank()->CheckFood()<50)
    return goto(160);
  Sound("Applause");
  Message("");
  SetPlrShowCommand(0,0);
  return(1);
}

func Script170()
{
  Message("@Warte mal kurz...");
  dayspeed=40;
}

func Script175()
{
  if(IsDay())
   return goto(173);
  dayspeed=0;
  if(GetSystemTime(4)>21||GetSystemTime(4)<7)
    Message("Es ist nun Nacht( draußen auch!): die ideale Zeit zum schlafen");
  else
    Message("Es ist nun Nacht( nur im Spiel... draußen nicht): die ideale Zeit zum schlafen");
}

func Script185()
{
  Message("Und gerade jetzt ist der Clonk müde!");
  GetHiRank()->CheckSleep(-800);
}

func Script195()
{
  Message("Gehe mit ihm in die Hütte. (Du hast übrigens noch einen 2. Clonk in der Hütte)");
  g_pGoal->SetMessages("", "Gehe mit deinem müden Clonk in die Hütte und leg ihn schlafen");
  var pHut=FindObject(_A1A);
  SetArrow(GetX(pHut),GetY(pHut)-40,false);
}

func Script198()
{
  if(GetID(Contained(GetHiRank()))!=_A1A)
    return goto(197);
  SetPlrShowCommand(0,3);
  Message("@Legen sie den Clonk per <c ffff00>%s</c> schlafen.",0,GetPlrControlName(0, CON_Up));
  return(1);
}

func Script200()
{
  if(GetID(Contained(GetHiRank()))!=_A1A)
    return goto(197);
  if(GetAction(GetHiRank())ne"Sleep")
    return goto(199);
  SetPlrShowCommand(0,0);
  RemoveArrow();
  Message("@Warten sie jetzt einfach eine Weile bis sich der Clonk fertig ausgeruht hat.");
  return(1);
}

func Script202()
{
  if(GetHiRank()->CheckSleep()<85)
    return goto(201);
  Message("Der Clonk hat nun genug geschlafen...");
  return(1);
}

func Script210()
{
  Message("Er hört sofort mit dem schlafen auf wenn er die Hütte verlässt.");
  return(1);
}

func Script220()
{
  Message("@Verlassen sie die Hütte");
  return(1);
}

func Script222()
{
  if(Contained(GetHiRank()))
    return goto(221);
  Sound("Applause");
  Message("");
  Message("Aber hier ist es so dunkel... :/",GetHiRank());
  return(1);
}

func Script230()
{
  Message("Dagegen kann man was tun.");
  return(1);
}

func Script240()
{
  Sound("Ding");
  Message("@Gehe in die Kneipe ,kaufe 2 Fackeln und platziere sie so das eine vernünftige Beleuchtung entsteht.|(Platzieren per Aktivieren)");
  g_pGoal->SetMessages("", "Kaufe 2 Fackeln und schaffe damit für eine Beleuchtung");
  return(1);
}

func Script250()
{
  if(ObjectCount2(Find_ID(TOR2),Find_OCF(OCF_Fullcon))<2)
    return goto(245);
  Sound("Applause");
  Message("");
  dayspeed=3;
  return(1);
}

func Script260()
{
  Message("Doch es fehlt noch eine ganz wichtige Sache damit due Siedlung gedeihen kann:| eine Forschungshütte|, denn ohne Forschung können sie weder neue Gebäude bauen noch alte aufrüsten");
  return(1);
}

func Script270()
{
  Sound("Ding");
  Message("@Gehe in die Kneipe, kaufe dort einen Bausatz und baue eine Forschungshütte");
  g_pGoal->SetMessages("", "Baue eine Forschungshütte");
  ____->Researched();
  return(1);
}

func Script275()
{
  if(!ObjectCount2(Find_ID(_B1A)))
    return goto(273);
  Message("@Die Forschungshütte benötigt 2x einfaches Holz ({{WOOD}}) um aufgebaut zu werden...| Um Geld zu sparen sollte um an das Holz zu kommen ein Baum gefällt werden...| Sobald dieser gefällt ist fängt der Clonk an den Baum zu zerkleinern... dabei entsteht genügend Holz für die Forschungshütte.");
}

func Script277()
{
  if(GetCon(FindObject2(Find_ID(_B1A)))<35)
    return goto(276);
  Message("");
}

func Script280()
{
  if(!ObjectCount2(Find_ID(_B1A),Find_OCF(OCF_Fullcon)))
    return goto(278);
  Sound("Applause");
  Message("");
  return(1);
}

func Script290()
{
  Message("Doch die Forschungshütte forscht nicht von alleine...");
  return(1);
}

func Script300()
{
  Sound("Ding");
  Message("@Schicke einen deiner Clonks hinein und erforsche Architektur.");
  g_pGoal->SetMessages("", "Erforsche Architektur");
  return(1);
}

func Script310()
{
  var pLab=FindObject(_B1A);
  if(ActIdle(pLab))
    return goto(305);
  Message("Wie lange das forschen dauert hängt von den Fähigkeiten des Clonks ab.|Die Fähigkeiten des Clonks lassen sich übers Kontextmenü ansehen.|Wenn ein Clonk lange Zeit eine Aktivität ausführt erhöht sich der Bonus minimal.");
  return(1);
}

func Script320()
{
  if(!GetPlrKnowledge(0,SCAC))
    return goto(315);
  Message("Architektur wurde nun zu Ende erforscht. Dies ermöglicht euch das du das Labor, die Hütte und Dörfer aufrüsten könnt.");
  return(1);
}

func Script330()
{
  Message("Auf doppelt-<c ffff00>%s</c> lässt sich ein Aufrüstmenü öffnen.",0,GetPlrControlName(0, CON_Dig));
  return(1);
}

func Script340()
{
  Sound("Ding");
  Message("@Baue die Forschungshütte zum Forschungsbau aus");
  g_pGoal->SetMessages("Die Lernrunde ist hiermit beendet|Alles wichtige für Ruf der Wipfe ist nun gelernt| du könnt ja noch weiterspielen wenn du wollt... oder mit der Kampange starten... oder ne Runde mit euren Freunden spielen... oder... oder...| Aber auf jeden Fall: viel Spaß :D", "Baue die Forschungshütte zum Forschungsbau aus");
  return(1);
}

func Script350()
{
  if(!ObjectCount2(Find_ID(_B2A),Find_OCF(OCF_Fullcon)))
    return goto(345);
  Sound("Applause");
  Message("");
  return(1);
}

func Script360()
{
  Message("Jetzt ist noch die Frage offen wie man an neue Clonks kommt.");
  return(1);
}

func Script380()
{
  Message("Da gibt es 2 Möglichkeiten:|1. Hütte bauen und dort einen Clonk anheuern (nur möglich wenn genügend Schlafplatz vorhanden ist)|2. Clonks in Kneipen anheuern");
  return(1);
}

func Script390()
{
  Message("@Du brauchst erstmal mehr Platz zum schlafen...");
  return(1);
}

func Script400()
{
  Sound("Ding");
  Message("@Baue deine Hütte aus");
  return(1);
}

func Script410()
{
  if(!ObjectCount2(Find_Or(Find_ID(_A2A),Find_ID(_A3A)),Find_OCF(OCF_Fullcon)))
    return goto(408);
  Sound("Applause");
  Message("");
  return(1);
}

func Script420()
{
  Message("@Schicke nun einen deiner Clonks hinein, öffne das Kontextmenü und heuer einen neuen Clonk an.");
  Sound("Ding");
  return(1);
}

func Script410()
{
  if(GetCrewCount()<3)
    return goto(418);
  Sound("Applause");
  Message("");
  goto(980);
  return(1);
}

func Script990()
{
  Sound("Applause");
  Message("");
  g_pGoal->Fulfill();
  return(1);
}

func Script1000()
{
  Sound("Applause");
  Sound("Ding");
  Message("<c ffff00>H<c ffff11>e<c ffff22>r<c ffff33>z<c ffff44>l<c ffff55>ichen Glückwu</c>n</c>s</c>c</c>h</c>!</c>|Die Lernrunde ist hiermit beendet.|Alles wichtige für Ruf der Wipfe ist nun gelernt| du könnt ja noch weiterspielen wenn du wollt... oder mit der Kampange starten... oder ne Runde mit euren Freunden spielen... oder... oder...| Aber auf jeden Fall: viel Spaß :D");
  return(1);
}

func Script1010()
{
  Sound("Applause");
  return(1);
}

func Script1020()
{
  //GameOver();
  ScriptGo(false);
}
