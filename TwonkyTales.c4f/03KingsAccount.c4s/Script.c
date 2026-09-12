#strict 2
#include TTSC
func TTChapter() { return 3; }
func TTIntroduction() { return "$Introduction$"; }
func TTJournal() { return "$Journal$"; }
func TTEnding() { return "$Ending$"; }
func TTBuild()
{
  TTNode("forge", 290, 188, "$Forge$");
  TTNode("province", 540, 188, "$Province$");
  TTNode("road", 830, 418, "$Road$");
  TTNode("toll", 1150, 638, "$Toll$");
  TTNode("dispatch", 660, 418, "$Dispatch$");
  TTNode("receipt", 1390, 638, "$Receipt$");
  TTNode("lamp", 1080, 418, "$Lamp$");
  TTNode("seal", 1280, 367, "$Seal$");
  TTNode("duplicate", 900, 818, "$Duplicate$");
  TTGate("roadgate", 920, 382);
  TTGate("records", 740, 792);
  TTChest("supplies", 320, 188, 8, 10);
  TTChest("consignment", 570, 418, 3, 2);
  TTPerson("factor", "Veyr", 710, 188);
  TTPerson("clerk", "Sella", 1190, 638);
  TTPerson("porter", "Bram", 970, 418);
}
func TTClue(string key)
{
  if (key == "province") return "$ProvinceClue$";
  if (key == "road") return "$RoadClue$";
  if (key == "toll") return "$TollClue$";
  if (key == "dispatch") return Format("$DispatchClue$", ContentsCount(METL, TTObject("consignment")), ContentsCount(WOOD, TTObject("consignment")));
  if (key == "receipt") return "$ReceiptClue$";
  if (key == "lamp") return "$LampClue$";
  if (key == "seal") return "$SealClue$";
  if (key == "duplicate") return "$DuplicateClue$";
  return "$ForgeClue$";
}
func TTUse(string key, object actor)
{
  if (key == "forge") return TTForge(actor);
  if (key == "province") return TTCycle(key, 0, 4);
  if (key == "road") return TTCycle(key, 1, 3);
  if (key == "toll") return TTCycle(key, 2, 4);
  if (key == "lamp") return TTCycle(key, 3, 2);
  if (key == "dispatch")
  {
    var cargo = TTObject("consignment");
    var metal = ContentsCount(METL, cargo), wood = ContentsCount(WOOD, cargo);
    if (ObjectDistance(cargo, TTObject("dispatch")) > 60) return TTSay("$CartMissing$", actor);
    if (metal != 3 || wood != 2 || TTState(0) != (metal*3+wood)%4
        || TTState(1) != (metal+wood*2)%3 || TTState(2) != metal-wood)
    {
      var seized = FindContents(METL, cargo);
      if (seized) RemoveObject(seized);
      TTSet(10, false); TTOpen("roadgate", false);
      return TTFault(actor, "$DispatchFailure$", 670, 400);
    }
    TTSet(10, true); TTOpen("roadgate", true);
    return TTSay("$DispatchSuccess$", actor);
  }
  if (key == "receipt")
  {
    var cargo = TTObject("consignment");
    if (!TTState(10) || ObjectDistance(cargo, TTObject("receipt")) > 50
        || ContentsCount(METL, cargo) != 3 || ContentsCount(WOOD, cargo) != 2)
      return TTSay("$ReceiptRefused$", actor);
    TTSet(11, true); return TTSay("$ReceiptSigned$", actor);
  }
  if (key == "duplicate")
  {
    if (!TTState(12)) return TTSay("$DuplicateLocked$", actor);
    TTSet(13, true); return TTSay("$DuplicateFound$", actor);
  }
  return TTInspect(key, actor);
}
func TTHit(string key, object arrow, bool back)
{
  if (key != "seal") return false;
  if (!back && !TTState(3))
  {
    TTSet(14, arrow); Sound("Ding"); return true;
  }
  if (back && TTState(3) && TTState(14) == arrow && TTState(11))
  {
    TTSet(12, true); TTOpen("records", true); return true;
  }
  if (back) return TTFault(GetCrew(GetOwner(arrow)), "$SealFailure$", 1270, 405);
  return false;
}
func TTDialogue(string key, object actor)
{
  if (key == "factor" && TTState(13)) return TTSay("$FactorAfter$", actor);
  if (key == "factor") return TTSay("$FactorBefore$", actor);
  if (key == "clerk") return TTSay("$ClerkTalk$", actor);
  return TTSay("$PorterTalk$", actor);
}
func TTUpdate() {}
func TTCanFinish() { return TTState(10) && TTState(11) && TTState(12) && TTState(13); }
