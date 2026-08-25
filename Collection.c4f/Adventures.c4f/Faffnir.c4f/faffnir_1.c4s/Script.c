/*-- Faffnir 1 --*/

#strict

func Initialize() {
 for(var Kanderianer in FindObjects(Find_ID(KAND)))
 { MakeCrewMember(Kanderianer, 2); 
   SetOwner(2,Kanderianer); 
   Kanderianer -> SetAI();
 }
var Lawl = FindObject(LAWL);
 MakeCrewMember(Lawl, 2);
 SetOwner(2, Lawl);
 Lawl -> SetAI();
    FindObject(TIME) -> SetTime(50);
    ScriptGo(1);
    goto(2);
  return(1);
}

protected func InitializePlayer(int iPlr)
{
  SetFoW(1, iPlr);
  SetPlrViewRange(150, GetCrew(iPlr));
  SetPosition( 60, 360, GetCrew(iPlr));
  SetOwner(iPlr, GetCrew(iPlr));
  CreateContents(BOTP, GetCrew(iPlr));
  CreateContents(CRBW, GetCrew(iPlr));
  CreateContents(HOOK, GetCrew(iPlr));
  CreateContents(KNFP, GetCrew(iPlr)); 
  return(1);
}

 private func RelaunchPlayer(iPlr, object pCursor)
{   Global(9)++;
    RemoveAll(BOTP);
    RemoveAll(CRBW);
    RemoveAll(HOOK);
    RemoveAll(KNPF);
   if(Global(9) < 5) {
    MakeCrewMember(CreateObject(ASAS, 60, 360), iPlr);
    SetPlrViewRange(150, GetCrew(iPlr));
    SetCursor(iPlr, GetCrew(iPlr));
    GetCrew(iPlr)->~Feed(80);
    CreateContents(BOTP, GetCrew(iPlr));
    CreateContents(CRBW, GetCrew(iPlr));
    CreateContents(HOOK, GetCrew(iPlr));
    CreateContents(KNFP, GetCrew(iPlr));
    Log("Noch %d Versuche!", 5 -Global(9)); }
      return(1); }
    
    

func Script3() {
  Message("Ich werde dieses {{MSKU}} Artefakt bekommen, koste es was es wolle...", FindObject(ASAS)); return(1); }

func Script7() {
 FindObject(TIME) -> SetTime(50);
 var Geheim = ["Man hat uns entdeckt!", "Schützt den magischen Kelch!", "Wir sind doch geheim!", "Man hat uns verraten!", "Ich werde den Kelch mit meinem LEBEN  beschützen!", "Ich sag doch das Versteck is scheiße -.-", "Greift ihn an!"];
 var NichtGeheim = ["Ein Attentäter!", "Ein Eindringling!", "Wer ist da?", "Freund oder Feind?", "Muss ich jetzt sterben?", "Verzieh dich!", "Ich würde mich jetz ganz schnell in Sicherheit bringen!", "Hallo?!?!", "Ab in den Schlund mit dir!", "Stirb!", "Ich habe schon 54 Assasinen getötet!!", "MUAHhAhAH"];
 for(var GeheimeClonks in FindObjects(Find_ID(KAND), Find_InRect(180, 600, 500, 190)))
 if(FindObject2(Find_ID(ASAS), Find_Distance(190,GetX(GeheimeClonks),GetY(GeheimeClonks))) && GetAlive(GeheimeClonks) && !GetEffect("Nixsag", GeheimeClonks) && PathFree(GetX(GeheimeClonks), GetY(GeheimeClonks), GetX(FindObject(ASAS)), GetY(FindObject(ASAS))))
   { Message(Geheim[Random(6)], GeheimeClonks); AddEffect("Nixsag", GeheimeClonks,5,0); } 
 for(var NichtGeheimeClonks in FindObjects(Find_ID(KAND), Find_Not(Find_InRect(180, 600, 500, 190))))
 if(FindObject2(Find_ID(ASAS),Find_Distance(190,GetX(NichtGeheimeClonks),GetY(NichtGeheimeClonks))) && GetAlive(NichtGeheimeClonks) && !GetEffect("Nixsag", NichtGeheimeClonks) && PathFree(GetX(NichtGeheimeClonks), GetY(NichtGeheimeClonks), GetX(FindObject(ASAS)), GetY(FindObject(ASAS))))
   { Message(NichtGeheim[Random(11)],NichtGeheimeClonks); AddEffect("Nixsag", NichtGeheimeClonks,5,0); }
 if(FindObject2(Find_ID(ASAS),Find_InRect(1271,618,60,60)) && !GetEffect("WasserFalle")) { var pStone =CreateObject(ROCK,1255,533,NO_OWNER); Explode(33, pStone); AddEffect("WasserFalle",0,2,0); Schedule("Ausruf()", 240,0);}
 if(!GetAlive(FindObject(LAWL))) RemoveObject(FindObject(513B));
 if(Contained(FindObject(MSKU)) == FindObject(ASAS)) GameOver(3);
 if(FindObject2(Find_ID(ASAS), Find_InRect(813,549,120,200)) && !GetEffect("Niemand")) { Message("An mir ist noch niemand vorbeigekommen!!", FindObject(LAWL)); AddEffect("Niemand",0,56); }
 if(FindObject2(Find_ID(ASAS), Find_InRect(752,734,280,50)) && !GetEffect("Geschafft")){ Message("Ich habe es tatsächlich geschafft...", FindObject(ASAS)); AddEffect("Geschafft",0,59); }
 if(FindObject2(Find_ID(ASAS), Find_InRect(1205,334,280,50)) && !GetEffect("Aufspreng")){ Message("Hmmm, mit Sprengstoff lässt sich da sicher was machen.", FindObject(ASAS)); AddEffect("Aufspreng",0,51); }
 goto(4);
 return(1);
 }
 
 global func Ausruf() {
  Message("Ich muss irgendeinen anderen Weg in die Katakomben finden, vielleicht kann ich mich irgendwo reingraben?", FindObject(ASAS)); return(1); }
