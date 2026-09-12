#strict 2
#include TTSC
func TTChapter() { return 1; }
func TTIntroduction() { return "$Introduction$"; }
func TTJournal() { return "$Journal$"; }
func TTEnding() { return "$Ending$"; }

func TTBuild()
{
  TTNode("intake", 430, 188, "$Intake$");
  TTNode("bypass", 630, 418, "$Bypass$");
  TTNode("relief", 1260, 418, "$Relief$");
  TTNode("pump", 910, 418, "$Pump$");
  TTNode("chain", 1080, 355, "$Chain$");
  TTNode("old", 310, 668, "$Old$");
  TTNode("bell", 200, 188, "$Bell$");
  TTNode("forge", 550, 188, "$Forge$");
  TTGate("school", 1120, 382);
  TTGate("spillway", 740, 602);
  TTChest("supplies", 575, 188, 5, 7);
  TTPerson("keeper", "Mara", 220, 188);
  TTPerson("child", "Pip", 1270, 418);
  TTPerson("miller", "Iven", 1370, 418);
  TTPerson("apprentice", "Nell", 1220, 638);
}

func TTClue(string key)
{
  if (key == "intake") return "$IntakeClue$";
  if (key == "bypass") return "$BypassClue$";
  if (key == "relief") return "$ReliefClue$";
  if (key == "pump") return Format("$PumpClue$", 2*TTState(0)+TTState(1), TTState(0)+2*TTState(2), TTState(1)+TTState(2));
  if (key == "chain") return "$ChainClue$";
  if (key == "old") return "$OldClue$";
  if (key == "bell") return "$BellClue$";
  return "$ForgeClue$";
}

func TTUse(string key, object actor)
{
  if (key == "intake") return TTCycle(key, 0, 4);
  if (key == "bypass") return TTCycle(key, 1, 4);
  if (key == "relief") return TTCycle(key, 2, 4);
  if (key == "old")
  {
    TTSet(8, true); TTOpen("spillway", true); Sound("Bow");
    return TTSay("$OldOpens$", actor);
  }
  if (key == "forge") return TTForge(actor);
  if (key == "pump")
  {
    if (TTState(6))
    {
      if (!TTSpend(actor, 2, 1)) return false;
      TTSet(6, false);
      return TTSay("$PumpRepaired$", actor);
    }
    if (TTState(0) != 2 || TTState(1) != 1 || TTState(2) != 1)
    {
      TTSet(6, true); TTSet(5, false); TTOpen("school", false);
      CastPXS("Water", 150, 20, 960, 340);
      return TTFault(actor, "$PumpFailure$", 900, 400);
    }
    TTSet(5, true);
    return TTSay("$PumpBalanced$", actor);
  }
  if (key == "bell")
  {
    if (!TTState(5) || !TTState(7)) return TTSay("$BellTooSoon$", actor);
    TTSet(9, true); Sound("Ding");
    return TTSay("$BellRings$", actor);
  }
  return TTInspect(key, actor);
}

func TTHit(string key, object arrow, bool back)
{
  if (key != "chain" || back) return false;
  if (!TTState(5))
  {
    TTSet(6, true);
    return TTFault(GetCrew(GetOwner(arrow)), "$ChainFailure$", 1100, 410);
  }
  TTSet(7, true); TTOpen("school", true);
  return true;
}

func TTDialogue(string key, object actor)
{
  if (key == "keeper") return TTSay("$KeeperTalk$", actor);
  if (key == "child") TTSay("$ChildTalk$", actor);
  if (key == "miller") TTSay("$MillerTalk$", actor);
  if (key == "apprentice") TTSay("$ApprenticeTalk$", actor);
  TTFollow(key, actor);
  return true;
}

func TTUpdate() {}
func TTCanFinish()
{
  return TTState(5) && TTState(7) && TTState(8) && TTState(9)
    && TTAtHaven("child", 200, 188) && TTAtHaven("miller", 200, 188)
    && TTAtHaven("apprentice", 200, 188);
}
