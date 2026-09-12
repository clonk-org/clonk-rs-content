/* Original campaign library. Scenario state is serialized by ordinary C4Script. */
#strict 2
static tt_host, tt_bow, tt_state, tt_nodes, tt_people, tt_finished, tt_faults;
static tt_clock, tt_read, tt_start_x, tt_start_y, tt_goal, tt_failed;

func Initialize()
{
  tt_host = -1;
  tt_bow = 0;
  tt_state = CreateArray(64);
  tt_nodes = [];
  tt_people = [];
  tt_read = [];
  tt_finished = false;
  tt_failed = 0;
  tt_faults = 0;
  tt_clock = 0;
  tt_start_x = 200; tt_start_y = 188;
  TTBuild();
  TTScenery();
  tt_goal = CreateObject(TTGL, 0, 0, NO_OWNER);
  if (TTChapter() < 6) tt_goal->SetMissionAccess(Format("TwonkyTales%d", TTChapter() + 1));
  AddEffect("TTCampaign", 0, 10, 10);
}

func InitializePlayer(int player)
{
  var crew = GetCrew(player);
  if (!crew) crew = CreateObject(KNIG, tt_start_x, tt_start_y, player);
  MakeCrewMember(crew, player);
  SetPosition(tt_start_x + 18 * GetPlayerCount(), tt_start_y, crew);
  SetCursor(player, crew);
  DoEnergy(100, crew);
  for (var i = 0; i < GetPlayerCount(); ++i)
  {
    var other = GetPlayerByIndex(i);
    SetHostility(player, other, false, true);
  }
  if (tt_host < 0)
  {
    // The first user slot belongs to the hosting player in a normal lobby.
    tt_host = player;
    SetName("Twonky", crew);
    tt_bow = CreateContents(BOW1, crew);
    SetName("$BowName$", tt_bow);
    for (var a = 0; a < 3; ++a) CreateContents(TTAR, crew);
  }
  else SetName(Format("$CompanionName$", GetPlayerName(player)), crew);
  MessageWindow(TTIntroduction(), player, TTGL);
  return true;
}

func RemovePlayer(int player)
{
  // This callback precedes crew removal. Ordinary dropped objects remain in the world.
  for (var i = 0; i < GetCrewCount(player); ++i)
  {
    var crew = GetCrew(player, i), item;
    while (item = Contents(0, crew)) Exit(item);
  }
}

func TTHost() { return tt_host; }
func TTComplete() { return tt_finished; }
func TTFailed() { return !!tt_failed; }
func TTFailureText() { return tt_failed; }
func TTFaults() { return tt_faults; }
func TTState(int index) { return tt_state[index]; }
func TTSet(int index, value) { tt_state[index] = value; return value; }
func TTTime() { return tt_clock; }
func TTGetBow() { return tt_bow; }

func TTNode(string key, int x, int y, string label)
{
  var node = CreateObject(TTMN, x, y, NO_OWNER);
  SetPosition(x, y, node);
  node->Setup(key, label);
  tt_nodes[GetLength(tt_nodes)] = node;
  return node;
}

func TTGate(string key, int x, int y)
{
  var gate = CreateObject(TTGT, x, y, NO_OWNER);
  SetPosition(x, y, gate);
  gate->Setup(key);
  tt_nodes[GetLength(tt_nodes)] = gate;
  return gate;
}

func TTObject(string key)
{
  for (var node in tt_nodes) if (node && node->Key() == key) return node;
  return 0;
}

func TTOpen(string key, bool open)
{
  var node = TTObject(key);
  if (node) return node->Open(open);
  return false;
}

func TTCycle(string key, int index, int count)
{
  tt_state[index] = (tt_state[index] + 1) % count;
  TTObject(key)->Display(tt_state[index]);
  Sound("Connect");
  return true;
}

func TTPerson(string key, string name, int x, int y)
{
  var person = CreateObject(KNIG, x, y, NO_OWNER);
  SetPosition(x, y, person);
  SetName(name, person);
  SetColorDw(RGB(180, 145, 100), person);
  tt_people[GetLength(tt_people)] = [key, person, 0, 0, 0, name];
  return person;
}

func TTPersonObject(string key)
{
  for (var entry in tt_people) if (entry[0] == key) return entry[1];
  return 0;
}

func TTConversation(object person, object actor)
{
  if (!actor || !GetAlive(actor) || ObjectDistance(person, actor) > 40) return false;
  for (var entry in tt_people)
    if (entry[1] == person) return TTDialogue(entry[0], actor);
  return false;
}

func TTFollow(string key, object actor)
{
  for (var i = 0; i < GetLength(tt_people); ++i)
    if (tt_people[i][0] == key) tt_people[i][2] = actor;
  return true;
}

func TTAtHaven(string key, int x, int y)
{
  var person = TTPersonObject(key);
  return person && GetAlive(person) && Distance(GetX(person), GetY(person), x, y) < 60;
}

func TTChest(string key, int x, int y, int metal, int wood)
{
  var chest = CreateObject(TTCT, x, y, NO_OWNER);
  SetPosition(x, y, chest);
  chest->Setup(key);
  if (key == "supplies") SetName("$SuppliesChest$", chest);
  if (key == "counterweight" || key == "ballast") SetName("$CounterweightChest$", chest);
  if (key == "consignment") SetName("$ConsignmentChest$", chest);
  if (key == "convoy") SetName("$ConvoyChest$", chest);
  if (key == "legacy") SetName("$LegacyChest$", chest);
  tt_nodes[GetLength(tt_nodes)] = chest;
  for (var i = 0; i < metal; ++i) CreateContents(METL, chest);
  for (var j = 0; j < wood; ++j) CreateContents(WOOD, chest);
  return chest;
}

func TTSpend(object actor, int metal, int wood)
{
  // A nearby supply chest is a workbench inventory. All checks precede consumption.
  var stock = [];
  for (var item in FindObjects(Find_Or(Find_ID(METL), Find_ID(WOOD))))
  {
    var holder = Contained(item);
    if (holder == actor || (holder && GetID(holder) == TTCT && ObjectDistance(holder, actor) < 45))
      stock[GetLength(stock)] = item;
  }
  var m = 0, w = 0;
  for (var item in stock)
  {
    if (GetID(item) == METL) ++m;
    if (GetID(item) == WOOD) ++w;
  }
  if (m < metal || w < wood) return TTSay("$NeedSupplies$", actor);
  for (var item in stock)
  {
    if (GetID(item) == METL && metal) { --metal; RemoveObject(item); }
    else if (GetID(item) == WOOD && wood) { --wood; RemoveObject(item); }
  }
  return true;
}

func TTForge(object actor)
{
  if (!TTSpend(actor, 1, 2)) return false;
  CreateContents(TTAR, actor);
  Sound("MetalHit1");
  return true;
}

func TTSay(string text, object actor)
{
  if (actor) MessageWindow(text, GetOwner(actor), TTGL);
  return false;
}

func TTInspect(string key, object actor)
{
  var node = TTObject(key);
  if (!actor || !node || ObjectDistance(node, actor) > 100) return false;
  return TTSay(TTClue(key), actor);
}

func TTOperate(string key, object actor)
{
  var node = TTObject(key);
  if (!actor || !GetAlive(actor) || !node || ObjectDistance(node, actor) > 45) return false;
  if (tt_finished || tt_failed) return false;
  return TTUse(key, actor);
}

func TTFault(object actor, string text, int x, int y)
{
  ++tt_faults;
  // Each fault leaves a real collapse to dig around and costs health. No rewind.
  if (actor) DoEnergy(-12, actor);
  for (var i = 0; i < 4 + Min(tt_faults, 8); ++i)
    CreateObject(ROCK, x + i * 6 - 18, y - 30, NO_OWNER);
  Sound("RockHit1");
  return TTSay(text, actor);
}

func TTRecall(object actor)
{
  if (!actor || !tt_bow || Contained(tt_bow) != actor) return false;
  if (!TTRecallEnabled()) return TTSay("$BowSilent$", actor);
  var recalled = false;
  for (var arrow in FindObjects(Find_ID(TTAR), Find_NoContainer()))
    if (arrow->Recall()) recalled = true;
  if (recalled) Sound("Magic1");
  return recalled;
}

func TTRecallEnabled() { return TTChapter() > 2 || (TTChapter() == 2 && TTState(10)); }

global func FxTTCampaignTimer(object target, int effect, int time)
{
  return GameCall("TTTick");
}

func TTTick()
{
  ++tt_clock;
  if (tt_finished) return 1;
  if (tt_failed)
  {
    if (TTMenusClosed())
    {
      for (var i = 0; i < GetPlayerCount(); ++i) EliminatePlayer(GetPlayerByIndex(i));
      GameOver();
      return -1;
    }
    return 1;
  }
  if (tt_host >= 0)
  {
    if (!tt_bow) return TTDefeat("$BowLost$");
    for (var entry in tt_people)
      if (!entry[1] || !GetAlive(entry[1])) return TTDefeat(Format("$PersonLost$", entry[5]));
  }
  for (var i = 0; i < GetLength(tt_people); ++i)
  {
    var person = tt_people[i][1], leader = tt_people[i][2];
    if (person && leader && GetAlive(person) && GetAlive(leader) && ObjectDistance(person, leader) > 30)
      if (!GetCommand(person) || Abs(GetX(leader)-tt_people[i][3]) > 20 || Abs(GetY(leader)-tt_people[i][4]) > 20)
      {
        SetCommand(person, "MoveTo", 0, GetX(leader), GetY(leader));
        tt_people[i][3] = GetX(leader); tt_people[i][4] = GetY(leader);
      }
  }
  TTUpdate();
  // Evidence pages carry clues for later chapters; never replace an unread page.
  if (TTCanFinish() && TTMenusClosed())
  {
    tt_finished = true;
    for (var i = 0; i < GetPlayerCount(); ++i) MessageWindow(Format("%s|$FinishPrompt$", TTEnding()), GetPlayerByIndex(i), TTGL);
  }
  return 1;
}

func TTDecor(string graphic, int x, int y)
{
  var scenery = CreateObject(TTDC, x, y, NO_OWNER);
  SetPosition(x, y, scenery);
  SetGraphics(graphic, scenery);
  return scenery;
}
func TTScenery()
{
  var chapter = TTChapter();
  if (chapter == 1 || chapter == 5)
  {
    TTDecor("House", 280, 200);
    if (chapter == 1) TTDecor("School", 760, 200);
    else TTDecor("Ruin", 760, 200);
    TTDecor("Mill", 1200, 200);
    TTDecor("Tank", 930, 310);
    TTDecor("House", 1400, 200);
  }
  if (chapter == 2)
  {
    TTDecor("Mill", 450, 200); TTDecor("Engine", 670, 330);
    TTDecor("Tank", 1040, 550); TTDecor("Archive", 980, 840);
  }
  if (chapter == 3)
  {
    TTDecor("School", 680, 200); TTDecor("House", 1180, 200);
    TTDecor("Archive", 1170, 650); TTDecor("Archive", 900, 840);
    TTDecor("Tank", 1350, 310);
  }
  if (chapter == 4)
  {
    TTDecor("Prison", 610, 200); TTDecor("Prison", 1200, 200);
    TTDecor("Archive", 1060, 430); TTDecor("Tank", 860, 520);
    TTDecor("Prison", 1270, 650); TTDecor("Archive", 920, 840);
  }
  if (chapter == 5) TTDecor("Engine", 1210, 740);
  if (chapter == 6)
  {
    TTDecor("Prison", 1140, 200); TTDecor("Engine", 790, 330);
    TTDecor("Engine", 1190, 330); TTDecor("Tank", 950, 530);
    TTDecor("Archive", 1400, 650); TTDecor("House", 1180, 840);
  }
}

func TTDefeat(string text)
{
  tt_failed = text;
  for (var i = 0; i < GetPlayerCount(); ++i) MessageWindow(Format("%s|$DefeatPrompt$", text), GetPlayerByIndex(i), TTGL);
  return 1;
}

func TTReadyToEnd() { return tt_finished && !tt_failed && TTMenusClosed(); }

func TTMenusClosed()
{
  for (var i = 0; i < GetPlayerCount(); ++i)
    if (GetMenu(GetCursor(GetPlayerByIndex(i)))) return false;
  return true;
}
