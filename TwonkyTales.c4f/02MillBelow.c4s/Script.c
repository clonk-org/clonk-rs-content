#strict 2
#include TTSC
func TTChapter() { return 2; }
func TTIntroduction() { return "$Introduction$"; }
func TTJournal() { return "$Journal$"; }
func TTEnding() { return "$Ending$"; }
func TTBuild()
{
  TTNode("forge", 380, 188, "$Forge$");
  TTNode("restore", 440, 188, "$Restore$");
  TTNode("flywheel", 610, 418, "$Flywheel$");
  TTNode("shuttle", 980, 418, "$Shuttle$");
  TTNode("witness", 1360, 374, "$Witness$");
  TTNode("receiver", 1120, 350, "$Receiver$");
  TTNode("press", 1160, 638, "$Press$");
  TTNode("drawing", 980, 818, "$Drawing$");
  TTGate("workshop", 790, 382);
  TTGate("archive", 790, 792);
  TTChest("supplies", 405, 188, 8, 9);
  TTChest("counterweight", 950, 638, 0, 0);
  TTPerson("maker", "Orren", 480, 188);
  TTPerson("apprentice", "Nell", 1030, 638);
}
func TTClue(string key)
{
  if (key == "restore") return "$RestoreClue$";
  if (key == "flywheel") return "$FlywheelClue$";
  if (key == "shuttle") return "$ShuttleClue$";
  if (key == "witness") return "$WitnessClue$";
  if (key == "receiver") return "$ReceiverClue$";
  if (key == "press") return "$PressClue$";
  if (key == "drawing") return "$DrawingClue$";
  return "$ForgeClue$";
}
func TTUse(string key, object actor)
{
  if (key == "forge") return TTForge(actor);
  if (key == "restore")
  {
    if (TTState(10)) return TTSay("$RestoredAlready$", actor);
    if (!TTSpend(actor, 2, 1)) return false;
    TTSet(10, true); TTOpen("workshop", true);
    return TTSay("$Restored$", actor);
  }
  if (key == "flywheel") return TTCycle(key, 0, 3);
  if (key == "shuttle")
  {
    TTCycle(key, 1, 4);
    SetPosition(1120, 350 + TTState(1)*24, TTObject("receiver"));
    return true;
  }
  if (key == "press")
  {
    var tray = TTObject("counterweight");
    if (TTState(6))
    {
      if (!TTSpend(actor, 2, 1)) return false;
      TTSet(6, false); return TTSay("$PressRepair$", actor);
    }
    if (!TTState(12) || ObjectDistance(tray, TTObject("press")) > 45
        || ContentsCount(METL, tray) != 3 || ContentsCount(WOOD, tray) != 1)
    {
      TTSet(6, true);
      return TTFault(actor, "$PressFailure$", 1180, 620);
    }
    TTSet(14, true); TTOpen("archive", true);
    return TTSay("$PressSuccess$", actor);
  }
  if (key == "drawing")
  {
    if (!TTState(14)) return TTSay("$DrawingLocked$", actor);
    TTSet(15, true); return TTSay("$DrawingFound$", actor);
  }
  return TTInspect(key, actor);
}
func TTHit(string key, object arrow, bool back)
{
  if (key == "witness" && !back)
  {
    if (TTState(0) != 1) return false;
    TTSet(11, arrow); Sound("Ding"); return true;
  }
  if (key == "receiver" && back && TTState(11) == arrow)
  {
    if (TTState(0) != 2 || TTState(1) != 2)
      return TTFault(GetCrew(GetOwner(arrow)), "$ReturnFailure$", 1100, 400);
    TTSet(12, true); Sound("Connect"); return true;
  }
  return false;
}
func TTDialogue(string key, object actor)
{
  if (key == "maker" && TTState(15)) return TTSay("$MakerAfter$", actor);
  if (key == "maker") return TTSay("$MakerBefore$", actor);
  return TTSay("$ApprenticeTalk$", actor);
}
func TTUpdate() {}
func TTCanFinish() { return TTState(10) && TTState(12) && TTState(14) && TTState(15); }
