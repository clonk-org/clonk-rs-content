/* Gruppenschaden */

#strict

func Activate(pCaster,pRealcaster)
{
  if(GetActMapVal("Name","Magic",GetID(pCaster)))
    if(GetAction(pCaster) ne "Magic")
      if (GetAction(pCaster) eq "Ready") // Für den Zauberturm: Dieser setzt Magic ansonsten erst verspätet
        ObjectSetAction(pCaster, "Magic");
      else
        return(0,RemoveObject());
  AddEffect("GroupDamagePSpell",pCaster,250,2,0,GetID());
  return(1,RemoveObject());
}

func FxGroupDamagePSpellTimer(pCaster, iEffectNumber, iEffectTime)
{
  var iMLength=GetActMapVal("Length",GetAction(pCaster),GetID(pCaster));
  if(iEffectTime >= 300) return(-1);
  
  // Effekt machen
  CreateParticle("DAura",GetX(pCaster),GetY(pCaster), RandomX(-1,1),RandomX(-1,1), 5*160 + Sin(iEffectTime*3, 5*32),RGB(0,0,0));
  
  // alle Clonks in Reichweite heilen
  DoDamage(pCaster);
  return(1);
}

protected func FxGroupDamagePSpellInfo() { return("{{GGHG}} $GroupHealInfo$"); }

func DoDamage(pCaster)
{
  var pClonk;
  while(pClonk=FindObject(0,GetX(pCaster)-80,GetY(pCaster)-80,161,161,OCF_CrewMember(),0,0,NoContainer(),pClonk))
    if(ObjectDistance(pClonk,pCaster) <= 80)
      if(Hostile(GetOwner(pClonk),GetOwner(pCaster)) || ObjectCount(NTMG))
      {
        // schaden
        if(!Random(3)) DoEnergy(-2,pClonk,0,FX_Call_EngGetPunched,GetOwner(pCaster)+1);
        // Partikeleffekte für jeden betroffenen:
        CreateParticle("MSpark",GetX(pClonk)+RandomX(-8, 8),GetY(pClonk)+RandomX(-10, 10),0,-20,RandomX(18,38),RGB(20,210,20));
        CreateParticle("PSpark",GetX(pClonk),GetY(pClonk),0,0,180,RGB(0,16,8),pClonk);
      }
}

func DoesMagic(pObj) { return(WildcardMatch(GetAction(pObj), "*Magic")); }


/* Zaubercombo */

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("522"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
