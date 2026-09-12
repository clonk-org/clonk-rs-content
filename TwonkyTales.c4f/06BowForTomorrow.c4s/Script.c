#strict 2
#include TTSC
func TTChapter() { return 6; }
func TTIntroduction() { return "$Introduction$"; }
func TTJournal() { return "$Journal$"; }
func TTEnding() { return "$Ending$"; }
func TTBuild()
{
  TTNode("forge", 350, 188, "$Forge$");
  TTNode("intake", 550, 418, "$Intake$");
  TTNode("bypass", 820, 418, "$Bypass$");
  TTNode("relief", 1260, 638, "$Relief$");
  TTNode("witness", 1390, 374, "$Witness$");
  TTNode("anchor", 420, 638, "$Anchor$");
  TTNode("receiver", 1120, 350, "$Receiver$");
  TTNode("shuttle", 1000, 418, "$Shuttle$");
  TTNode("surrender", 1390, 638, "$Surrender$");
  TTNode("cache", 1170, 818, "$Cache$");
  TTGate("engine", 760, 382);
  TTGate("prison", 1100, 602);
  TTGate("road", 1320, 792);
  TTChest("supplies", 380, 188, 9, 12);
  TTChest("ballast", 620, 638, 0, 0);
  TTChest("legacy", 1210, 818, 0, 0);
  TTPerson("maker", "Orren", 470, 638);
  TTPerson("factor", "Veyr", 1410, 638);
  TTPerson("captive", "Sella", 1260, 638);
}
func TTClue(string key)
{
  if (key == "intake") return "$IntakeClue$";
  if (key == "bypass") return "$BypassClue$";
  if (key == "relief") return "$ReliefClue$";
  if (key == "witness") return "$WitnessClue$";
  if (key == "anchor") return "$AnchorClue$";
  if (key == "receiver") return "$ReceiverClue$";
  if (key == "shuttle") return "$ShuttleClue$";
  if (key == "surrender") return "$SurrenderClue$";
  if (key == "cache") return "$CacheClue$";
  return "$ForgeClue$";
}
func TTUse(string key, object actor)
{
  if (key == "forge") return TTForge(actor);
  if (key == "intake") { TTCycle(key, 0, 4); TTOpen("engine", TTState(0) == 2); return true; }
  if (key == "bypass") return TTCycle(key, 1, 4);
  if (key == "relief") return TTCycle(key, 2, 4);
  if (key == "shuttle")
  {
    TTCycle(key, 3, 4);
    SetPosition(1120, 350 + TTState(3)*24, TTObject("receiver")); return true;
  }
  if (key == "anchor")
  {
    if (TTState(0) != 2 || TTState(1) != 1 || TTState(2) != 1)
      return TTFault(actor, "$AnchorFailure$", 440, 620);
    var ballast = TTObject("ballast");
    if (GetPlayerCount() == 1)
    {
      if (ObjectDistance(ballast, TTObject("anchor")) > 50 || ContentsCount(METL, ballast) != 3 || ContentsCount(WOOD, ballast) != 1)
        return TTSay("$SoloAnchor$", actor);
      TTSet(17, true);
    }
    else if (GetCrew(TTHost()) && GetOwner(actor) != TTHost()) return TTSay("$TwonkyAnchor$", actor);
    TTSet(10, actor); return TTSay("$AnchorReady$", actor);
  }
  if (key == "surrender")
  {
    if (!TTState(13)) return TTSay("$SurrenderRefused$", actor);
    TTSet(14, true); TTFollow("captive", actor);
    return TTSay("$SurrenderAccepted$", actor);
  }
  if (key == "cache")
  {
    var chest = TTObject("legacy");
    if (!TTState(14) || !TTAtHaven("captive",1170,818)) return TTSay("$CacheTooSoon$", actor);
    if (Contained(TTGetBow()) != chest || ContentsCount(TTAR, chest) < 3)
      return TTSay("$CacheNeedsBow$", actor);
    TTSet(15, true); TTOpen("road", true); Sound("Bow"); return TTSay("$CacheComplete$", actor);
  }
  return TTInspect(key, actor);
}
func TTAnchorHeld()
{
  if (TTState(17))
  {
    var ballast = TTObject("ballast");
    return ObjectDistance(ballast, TTObject("anchor")) < 50
      && ContentsCount(METL, ballast) == 3 && ContentsCount(WOOD, ballast) == 1;
  }
  var operator = TTState(10);
  return operator && GetAlive(operator) && ObjectDistance(operator, TTObject("anchor")) < 45
    && Contained(TTGetBow()) != operator;
}
func TTHit(string key, object arrow, bool back)
{
  if (key == "witness" && !back && TTState(0) == 2)
  {
    TTSet(11, arrow); Sound("Ding"); return true;
  }
  if (key != "receiver" || !back || TTState(11) != arrow) return false;
  if (TTState(3) != 2 || !TTAnchorHeld() || TTState(0) != 2 || TTState(1) != 1 || TTState(2) != 1)
  {
    TTSet(10, 0); TTSet(17, false);
    return TTFault(GetCrew(GetOwner(arrow)), "$FinalShotFailure$", 1090, 400);
  }
  if (GetPlayerCount() > 1 && GetOwner(arrow) == TTHost())
    return TTSay("$TrustShot$", GetCrew(GetOwner(arrow)));
  TTSet(13, true); TTOpen("prison", true); Sound("Trumpet");
  return true;
}
func TTDialogue(string key, object actor)
{
  if (key == "maker") return TTSay("$MakerTalk$", actor);
  if (key == "factor" && TTState(13)) return TTSay("$FactorAfter$", actor);
  if (key == "factor") return TTSay("$FactorBefore$", actor);
  if (TTState(14)) TTFollow("captive", actor);
  return TTSay("$CaptiveTalk$", actor);
}
func TTUpdate() {}
func TTCanFinish() { return TTState(13) && TTState(14) && TTState(15); }
