/* schriftrolle */

#strict 2

local idSpell, pAimingClonk;

protected func Initialize() {
  // Zufälligen Zauber setzen
  var possible_spells = [RVLT, MINV, MWP2, MMTR, MGHL];
  SetSpell(possible_spells[Random(GetLength(possible_spells))]);
  return true;
}

protected func CalcValue()
{
  var iSpell = 0;
  if(idSpell) iSpell=GetValue(0,idSpell);
  return GetValue(0,GetID())+iSpell;
}

protected func RejectEntrance()
{
  if(GetAction() == "WannaRemove") return true;
}

public func Selection()
{
  // wer bin ich? der da!
  if(idSpell) PlayerMessage(GetOwner(Contained()),"{{%i}} %s",Contained(),idSpell,GetName(0,idSpell));
}

protected func Entrance(object pContainer) { if(GetOCF(pContainer) & OCF_CrewMember) Selection(); }

protected func Activate(object pByClonk)
{
  if (idSpell)
  {
    DoMagic(pByClonk);
  }
  else
  {
    PlayerMessage(GetOwner(Contained()),"$MsgNothingHappened$",Contained());
  }
  return true;
}

public func SetSpell(id idMagic) {
  // Rolle ist nun beschrieben
  // Grafik setzen
  SetGraphics(0,0,idMagic,1,GFXOV_MODE_Picture);
  SetObjDrawTransform(350,-300,3000,150,500,-2000, this, 1);

  // Aktion für die Schriftrolle setzen
  SetAction("Be");

  idSpell = idMagic;
  
  // Name setzen
  UpdateName();
}

private func DoMagic(object pByClonk) {
  // Richtung
  SetDir(GetDir(pByClonk));
  // In den Besitz des Clonk-Controllers, damit der Verfeindungscheck diverser Zauber funktioniert
  SetOwner(GetController(pByClonk));
  SetController(GetOwner());
  // Zauber
  if(CreateObject(idSpell,0,0,GetController(pByClonk))->~Activate(this,pByClonk))
    { Exit(); SetAction("WannaRemove",pByClonk); }
  else PlayerMessage(GetOwner(Contained()),"$MsgNothingHappened$",Contained());
}

protected func WannaRemove() {
  // manche Zauber hängen gerne Effekte an den Caster
  if(!GetEffectCount(0,this) && !pAimingClonk) RemoveObject();
}

protected func AttachTargetLost() {
  RemoveObject();
}

public func SpellAiming(object pSpell, object pAimingClnk)
  {
  // Clonk zielt: Schriftrolle vorerst nicht entfernen
  pAimingClonk = pAimingClnk;
  }
  
public func SpellSucceeded(id idSpell, object pAimingClnk)
  {
  // Clonk hat fertig gezielt: Schriftrolle kann weg
  pAimingClonk = 0;
  }
  
public func SpellFailed(id idSpell, object pAimingClnk)
  {
  // Zauber beim Zielen abgebrochen: Schriftrolle restaurieren
  pAimingClonk = 0;
  return(SetSpell(idSpell));
  }

public func IsMagicProduct() { return true; }

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
  
