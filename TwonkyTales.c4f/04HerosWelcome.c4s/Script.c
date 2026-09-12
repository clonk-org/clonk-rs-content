#strict 2
#include TTSC
func TTChapter() { return 4; }
func TTIntroduction() { return "$Introduction$"; }
func TTJournal() { return "$Journal$"; }
func TTEnding() { return "$Ending$"; }
func TTBuild()
{
  TTNode("forge", 300, 188, "$Forge$");
  TTNode("intakes", 530, 418, "$Intakes$");
  TTNode("warehouses", 940, 418, "$Warehouses$");
  TTNode("signature", 1210, 188, "$Signature$");
  TTNode("docket", 1090, 418, "$Docket$");
  TTNode("vent", 850, 638, "$Vent$");
  TTNode("pardon", 1020, 573, "$Pardon$");
  TTNode("confession", 920, 818, "$Confession$");
  TTNode("escape", 200, 188, "$Escape$");
  TTGate("cells", 1040, 602);
  TTGate("testimony", 720, 792);
  TTChest("supplies", 330, 188, 7, 10);
  TTPerson("keeper", "Mara", 1140, 638);
  TTPerson("maker", "Orren", 1320, 638);
  TTPerson("clerk", "Sella", 1400, 638);
}
func TTClue(string key)
{
  if (key == "intakes") return "$IntakesClue$";
  if (key == "warehouses") return "$WarehousesClue$";
  if (key == "signature") return "$SignatureClue$";
  if (key == "docket") return "$DocketClue$";
  if (key == "vent") return "$VentClue$";
  if (key == "pardon") return "$PardonClue$";
  if (key == "confession") return "$ConfessionClue$";
  if (key == "escape") return "$EscapeClue$";
  return "$ForgeClue$";
}
func TTAlarm(object actor)
{
  TTSet(20, TTState(20)+1);
  var guard = CreateObject(KNIG, 1440, 418, NO_OWNER);
  SetName("$Bailiff$", guard); CreateContents(SWOR, guard);
  SetCommand(guard, "Attack", actor);
  return TTFault(actor, "$Alarm$", 1100, 400);
}
func TTUse(string key, object actor)
{
  if (key == "forge") return TTForge(actor);
  if (key == "intakes") return TTCycle(key, 0, 4);
  if (key == "warehouses") return TTCycle(key, 1, 4);
  if (key == "signature") return TTCycle(key, 2, 3);
  if (key == "vent") return TTCycle(key, 3, 4);
  if (key == "docket")
  {
    // Both makers signed. Erasing a guilty friend's name is an incorrect defence.
    if (TTState(0) != 3 || TTState(1) != 2 || TTState(2) != 2) return TTAlarm(actor);
    TTSet(10, true); return TTSay("$DocketAccepted$", actor);
  }
  if (key == "confession")
  {
    if (!TTState(12)) return TTSay("$ConfessionLocked$", actor);
    TTSet(13, true); return TTSay("$ConfessionFound$", actor);
  }
  if (key == "escape")
  {
    if (!TTState(13)) return TTSay("$EscapeRefused$", actor);
    TTSet(14, true); return TTSay("$EscapeReady$", actor);
  }
  return TTInspect(key, actor);
}
func TTHit(string key, object arrow, bool back)
{
  if (key != "pardon") return false;
  if (!back && TTState(3) == 1)
  {
    TTSet(11, arrow); Sound("Ding"); return true;
  }
  if (back && TTState(11) == arrow && TTState(3) == 3 && TTState(10))
  {
    TTSet(12, true); TTOpen("cells", true); TTOpen("testimony", true); return true;
  }
  if (back) return TTAlarm(GetCrew(GetOwner(arrow)));
  return false;
}
func TTDialogue(string key, object actor)
{
  if (!TTState(12)) return TTSay("$ThroughBars$", actor);
  if (key == "keeper") TTSay("$KeeperTalk$", actor);
  if (key == "maker") TTSay("$MakerTalk$", actor);
  if (key == "clerk") TTSay("$ClerkTalk$", actor);
  TTFollow(key, actor); return true;
}
func TTUpdate() {}
func TTCanFinish()
{
  return TTState(10) && TTState(12) && TTState(13) && TTState(14)
    && TTAtHaven("keeper",200,188) && TTAtHaven("maker",200,188) && TTAtHaven("clerk",200,188);
}
