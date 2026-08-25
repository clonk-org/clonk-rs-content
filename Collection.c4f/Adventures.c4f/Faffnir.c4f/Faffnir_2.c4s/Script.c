/*-- Faffnir 2 --*/

#strict

//jahahahah, soho!! //so heißt das: CreateScriptPlayer("Kanderianer", 0xff0000, 7, CSPF_FixedAttributes | CSPF_NoScenarioInit | CSPF_NoEliminationCheck)

func Initialize() {
  // for(var Kanderianer in FindObjects(Find_ID(KAND)))
 //{ Kanderianer -> SetAI();
   //MakeCrewMember(Kanderianer, 0);
   //SetOwner(0,Kanderianer);
   //CreateContents(AXE1, Kanderianer, 1);
   //CreateContents(BOW1, Kanderianer, 1);
   //CreateContents(ARWP, Kanderianer, 1);
   //CreateContents(SPER, Kanderianer, 1);
 //}
   //for(var Koenig in FindObjects(Find_ID(KING)))
 //{ Koenig -> SetAI();
   //CreateContents(CRWN, Koenig, 3);
   //MakeCrewMember(Kanderianer, 0);
   //SetOwner(0,Kanderianer);
 //}
 
 //for(var Schalter in FindObjects(Find_ID(_SWT)))
  //{CreateObject(793B, GetX(Schalter) + 200, GetY(Schalter) + 10); CreateObject(803B, GetX(Schalter) + 200, GetY(Schalter) + 10); }
  
 //for(var Schalter in FindObjects(Find_ID(_SWT)))
  //Schalter -> ZielHinzufuegen();
   
  //Vulkanfalle
  Global(7) =0;
  ScriptGo(1);
  goto(2);
  return(1);
}

protected func InitializePlayer(int iPlr)
{
  SetFoW(1, iPlr);
  SetPlrViewRange(300, GetCrew(iPlr));
  SetPosition( 1910, 733, GetCrew(iPlr));
  SetOwner(iPlr, GetCrew(iPlr));
  CreateContents(BOTP, GetCrew(iPlr));
  CreateContents(CRBW, GetCrew(iPlr));
  CreateContents(HOOK, GetCrew(iPlr));
  CreateContents(KNFP, GetCrew(iPlr));
  SetPlrKnowledge(iPlr, PFIR);
  return(1);
}


 private func RelaunchPlayer(iPlr, object pCursor)
{   if(iPlr==1) {Global(9)++;
    RemoveAll(BOTP);
    RemoveAll(CRBW);
    RemoveAll(HOOK);
    RemoveAll(KNPF);
   if(Global(9) < 5) {
    if(!GetEffect("NeuerRelaunch")) MakeCrewMember(CreateObject(ASAS, 1910, 733), 1);
      else MakeCrewMember(CreateObject(ASAS, 727,761), 1);
    SetPlrViewRange(300, GetCrew(1));
    SetCursor(1, GetCrew(1));
    SetOwner(iPlr, GetCrew(1));
    GetCrew(1)->~Feed(80);
    CreateContents(BOTP, GetCrew(1));
    CreateContents(CRBW, GetCrew(1));
    CreateContents(HOOK, GetCrew(1));
    CreateContents(KNFP, GetCrew(1));
    Log("Noch %d Versuche!", 5 -Global(9)); } }
      return(1); }
      
 public  func Script3() {
 CustomMessage("Mein {{MSKU}}-Artefakt wird mir auf der Suche nach neuen Schätzen bestimmt hilfreich sein.", 0, NO_OWNER, 100,100,0,DECO,"Portrait:ASAS::00ff00::1");
  return(1); }
  
 public  func Script33() {
  CustomMessage("(Artefakt über das Kontextmenü verwenden)", 0, NO_OWNER, 100,100,0,DECO,"Portrait:ASAS::00ff00::1");
  return(1); }
  
  
 public func Script40() {
 var NichtGeheim = ["Ein Attentäter!", "Ein Eindringling!", "Wer ist da?", "Freund oder Feind?", "Muss ich jetzt sterben?", "Verzieh dich!", "Ich würde mich jetz ganz schnell in Sicherheit bringen!", "Hallo?!?!", "Ab in den Schlund mit dir!", "Stirb!", "Ich habe schon 54 Assasinen getötet!!", "MUAHhAhAH"];
 for(var NichtGeheimeClonks in FindObjects(Find_ID(KAND), Find_Not(Find_InRect(180, 600, 500, 190))))
 if(FindObject2(Find_ID(ASAS),Find_Distance(190,GetX(NichtGeheimeClonks),GetY(NichtGeheimeClonks))) && GetAlive(NichtGeheimeClonks) && !GetEffect("Nixsag", NichtGeheimeClonks) && !GetEffect("Hide", FindObject(ASAS)) && PathFree(GetX(NichtGeheimeClonks), GetY(NichtGeheimeClonks), GetX(FindObject(ASAS)), GetY(FindObject(ASAS))))
   { Message(NichtGeheim[Random(11)],NichtGeheimeClonks); AddEffect("Nixsag", NichtGeheimeClonks,5,0); }
 if(FindObject2(Find_ID(ASAS), Find_InRect(416,929,155,40)) && Global(7) != 1) {LaunchVolcano(484); Global(7) =1; }
 if(Contained(FindObject(BSKU)) == FindObject(ASAS)) GameOver(3);
 if(FindObject2(Find_ID(ASAS), Find_InRect(569,640,300,250)))
  { if(!GetEffect("NeuerRelaunch")) AddEffect("NeuerRelaunch", 0, 99, 0); 
    RemoveEffect("BanBurnPotion", FindObject(ASAS)); }
 if(!Random(3)) Smoke(230, 248, RandomX(35,70), RGB(240,240,231));
 goto(38);
 return(1); }
 
  
  
