/* Zauber-Lernschriftrolle */

#strict 2

local idSpell, fSkill;

static SCRL_FirstCollection; // Wurde jemals eine Rolle eingesammelt? Für Intro-Nachricht

public func Selection()
  {
  // wer bin ich? der da!
  if (idSpell)
    {
    // Erstes Einsammeln: Intronachricht
    if (!SCRL_FirstCollection)
      {
      ++SCRL_FirstCollection;
      Message("$MsgIntro$");
      Sound("Ding", true);
      }
    PlayerMessage(GetOwner(Contained()),"{{%i}} %s",Contained(),idSpell,GetName(0,idSpell));
    }
  }

protected func Entrance(object pContainer) { if(GetOCF(pContainer) & OCF_CrewMember) Selection(); }

protected func Activate(object pByClonk)
  {
  var iPlr = GetController(pByClonk);
  if(!idSpell || !GetName(0, idSpell))
    {
    // Kein Zauber belegt
    Sound("Error", false, this, 100, iPlr+1);
    PlayerMessage(iPlr, "$MsgNoSpell$", this);
    return true;
    }
  // Wunder?
  else if(idSpell->~IsWonder())
  {
    // Ist er überhaupt Paladin?
    if(!pByClonk->~IsPaladin()) return PlayerMessage(iPlr, "$MsgNoPaladin$", this);
    // Stimmt die Kirche?
    if(idSpell->~NeedChurch())
      if(idSpell->~NeedChurch() != MMGetPlayerChurch(iPlr)) return PlayerMessage(iPlr, "$MsgWrongChurch$", this);
    // oder schon bekannt?
    if(pByClonk->~GetSpell(idSpell)) return PlayerMessage(iPlr, "$MsgKnown$", this, GetName(0, idSpell));
    // Lernen
    pByClonk->LearnSpell(idSpell);
    // Nachricht
    PlayerMessage(iPlr, "$MsgLearnedWonder$", pByClonk, GetName(0, idSpell));
  }
  // Bauplan?
  else if(GetCategory(0, idSpell) & 1024) // 1024 = C4D_SelectKnowledge
  {
    // Kennt er den Plan schon?
    if (GetPlrKnowledge(iPlr, idSpell))
    {
      Sound("Error", false, this, 100, iPlr+1);
      PlayerMessage(iPlr, "$MsgKnown$", this, GetName(0, idSpell));
      return true;
    }
    // den Plan lernen
    SetPlrKnowledge(iPlr, idSpell);
    // Nachricht
    PlayerMessage(iPlr, "$MsgLearnedPlan$", pByClonk, GetName(0, idSpell));
  }
  else
  {
    // Kennt den Zauber schon?
    if (GetPlrMagic(iPlr, idSpell))
    {
      Sound("Error", false, this, 100, iPlr+1);
      PlayerMessage(iPlr, "$MsgKnown$", this, GetName(0, idSpell));
      return true;
    }
    // Den Zauber lernen
    SetPlrMagic(iPlr, idSpell);
    // Nachricht
    PlayerMessage(iPlr, "$MsgLearnedSpell$", pByClonk, GetName(0, idSpell));
  }
  Sound("SpellSysCombo*", 0, 0, 0, GetOwner()+1);
  CreateParticle("NoGravSpark", 0,0,  0,-6, 5*10, RGBa(255,255,0, 0), pByClonk);
  CreateParticle("NoGravSpark", 0,0, -5,-3, 5*10, RGBa(255,255,0, 0), pByClonk);
  CreateParticle("NoGravSpark", 0,0, -5, 3, 5*10, RGBa(255,255,0, 0), pByClonk);
  CreateParticle("NoGravSpark", 0,0,  5,-3, 5*10, RGBa(255,255,0, 0), pByClonk);
  CreateParticle("NoGravSpark", 0,0,  5, 3, 5*10, RGBa(255,255,0, 0), pByClonk);
  CreateParticle("NoGravSpark", 0,0,  0, 6, 5*10, RGBa(255,255,0, 0), pByClonk);
  return true;
  }

public func SetSpell(id idMagic, bool fIsSkill)
  {
  // Rolle ist nun beschrieben
  // Grafik setzen
  SetGraphics(0,0,idMagic,1,GFXOV_MODE_Picture);
  SetObjDrawTransform(350,-300,3000,150,500,-2000, this, 1);
  // Ist es ein Zauber, sondern ein Bauplan?
  SetGraphics(0, this);
  fSkill = 0;
  if(GetCategory(0, idMagic) & 1024) // 1024 = C4D_SelectKnowledge
    SetGraphics("Red", this);
  if(fIsSkill)
  {
    SetGraphics("Green", this);
    fSkill = 1;
  }
  SetAction("Be"); // Für zwei Richtungen
  idSpell = idMagic;
  // Name setzen
  UpdateName();
}

public func SetSkill(id idSkill) { return SetSpell(idSkill, 1); }

protected func UpdateTransferZone()
  {
  // Für Objects.txt-Schriftrollen
  UpdateName();
  }

protected func UpdateName()
  {
  // Namen nach Zauber setzen
  if (idSpell)
    SetName(Format("%s: %s", GetName(0, GetID()), GetName(0,idSpell)));
  else
    SetName();
  }
  
