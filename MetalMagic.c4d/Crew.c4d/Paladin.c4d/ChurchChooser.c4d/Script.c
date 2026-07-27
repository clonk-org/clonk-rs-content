/*-- Kirchenwähler --*/

#strict 2

local iTeamEffect;
local iTimer;
local szChurch;
local pViewCursor;
local fFinish;
local fDoCursorChange;

global func StartChurchChoose(iPlr)
{
  if(MMGetChurch(iPlr))
  {
    if(!GetKarmaHelper(iPlr, 1))
      MMSetPlayerChurch(iPlr, MMGetChurch(iPlr), 1);
    return;
  }
  // Sonderbehandlung seitens des Szenarios?
  if(GameCall("RejectChurchChoose",iPlr)) return true;
  if(FindObjectOwner(CHCS, iPlr)) return;
  CreateObject(CHCS, 0, 0, iPlr)->Create();
}

func Create() {
  SetVisibility(VIS_Owner);
  SetPosition(-100,-100);
  if(!Random(2)) SetAction("Light");
  else SetAction("Blood");
  SetViewCursor(GetOwner(), GetHiRank(GetOwner()));
  fDoCursorChange = 1;
  SetCursor(GetOwner(), this);
  ClearLastPlrCom(GetController());
  var iTeam = GetPlayerTeam(GetOwner());
  if(!iTeam) iTeam = GetOwner();
  var i;
  if(FindObject(NTCH)) iTeamEffect = AddEffect(Format("IntTeamChuch%d", GetOwner()), 0, 1, 0, 0);
  else if(i = GetEffect(Format("IntTeamChuch%d", iTeam), 0))
     iTeamEffect = i;
  else iTeamEffect = AddEffect(Format("IntTeamChuch%d", iTeam), 0, 1, 0, 0);
  if(!EffectVar(0, 0, iTeamEffect)) EffectVar(0, 0, iTeamEffect) = [this];
  else PushBack(this, EffectVar(0, 0, iTeamEffect));
  EffectVar(4, 0, iTeamEffect)++;

  return 1;
}

protected func ControlLeft()
{
  if(GetAction()=="Light")
    SetAction("TurnLeft");
  else if(GetAction()=="Blood")
    SetAction("TurnLeft2");
  szChurch = "";
  ClearLastPlrCom(GetController());
}
protected func ControlRight()
{
  if(GetAction()=="Light")
    SetAction("TurnRight");
  else if(GetAction()=="Blood")
    SetAction("TurnRight2");
  szChurch = "";
  ClearLastPlrCom(GetController());
}

protected func CrewSelection(fDeselect, fCursor)
{
  if(Local(2)) return;
  // Bei Crewabwahl schließen (mit Verzögerung)
  if (fDeselect) { Schedule(Format("SetCursor(%d, Object(%d))", GetOwner(), ObjectNumber(this)), 1); }
  return 1;
}

// Bestätigen: Werfen, Hoch oder Graben
protected func ControlUp() { return ControlLeft(); }
protected func ControlThrow() { return DoEnter(); }
protected func ControlDigSingle() { return ClearLastPlrCom(GetController()); }
protected func ControlDown() { return ControlRight(); }

// Maus: Auf's Amulett klicken = drehen, wo anders hin = bestätigen
protected func ControlCommand() { return DoEnter(); }
protected func MouseSelection() { return ControlRight(); }

// Bestätigung
public func DoEnter(fNoChoose)
{
  // Noch nicht im ersten Frame, da sind vll die Teammitglieder noch nicht da.
  if(GetActTime()<=1) return;
  if(!fNoChoose && GetAction() != "Light" && GetAction() != "Blood") return;
  var szString, szSound;
  // Effektarbeit
  // aus der Liste löschen
  EffectVar(4, 0, iTeamEffect)--;
  // Wahl setzen
  if(!fNoChoose)
  {
    if(GetAction()=="Blood") EffectVar(scBloodchurch, 0, iTeamEffect)++;
    else EffectVar(scLightchurch, 0, iTeamEffect)++;
    // Sound!
    Sound("Ding");
  }
  var iChurch;
  // Letzter? dann Liste löschen und Abstimmung auswerten
  if(!EffectVar(4, 0, iTeamEffect))
  {
    if(EffectVar(scBloodchurch, 0, iTeamEffect) == EffectVar(scLightchurch, 0, iTeamEffect))
      iChurch = EffectVar(3, 0, iTeamEffect);
    else if(EffectVar(scBloodchurch, 0, iTeamEffect) > EffectVar(scLightchurch, 0, iTeamEffect))
      iChurch = scBloodchurch;
    else iChurch = scLightchurch;
    // Kirche setzen
    if(iChurch == scBloodchurch)
    { szString = "$Bloodchurch$"; szSound = "Inflame"; }
    else { szString = "$Lightchurch$"; szSound = "MagicElementAir";}
    ChurchSparks(iChurch);
    szString = Format("%s $Choosed$", szString);
    szString = ColorString(szString, iChurch);
    if(GetLength(EffectVar(0, 0, iTeamEffect))>1) szString = Format("$VotingEnded$:|Teiwaz %d: Sawelô %d|%s", EffectVar(scBloodchurch, 0, iTeamEffect), EffectVar(scLightchurch, 0, iTeamEffect), szString);
    MMSetPlayerChurch(GetOwner(),iChurch);
    RemoveEffect(0, 0, iTeamEffect);
    iTeamEffect = 0;
  }
  // Erster? dann Timer starten
  else if(!iTimer)
  {
    for(var pObj in EffectVar(0, 0, iTeamEffect)) pObj->~StartTimer();
    // und Wahl speichern, denn die kommt bei Gleichstand zum Zug
    if(GetAction()=="Blood") EffectVar(3, 0, iTeamEffect) = scBloodchurch;
    else EffectVar(3, 0, iTeamEffect) = scLightchurch;
  }
  if(!szString) szString = Format("@$Voting$:|Teiwaz %d: Sawelô %d", EffectVar(scBloodchurch, 0, iTeamEffect), EffectVar(scLightchurch, 0, iTeamEffect), szString);
  // Bei allen Verbündeten die Abstimmung zeigen
  for(var cnt=0;cnt<GetPlayerCount();cnt++)
  {
    var plr = GetPlayerByIndex(cnt);
    if(FindObject(NTCH)) if(GetOwner()!=plr) continue;
    if(Hostile(GetOwner(),plr)) continue;
    // Nachricht
    if(GetOwner()==plr || !FindObjectOwner(CHCS, plr)) PlayerMessage(plr, szString);
    // Einen Sound zur Kirchen wahl abspielen
    if(szSound)
    {
      Sound(szSound, 0, 0, 0, plr+1);
      Sound("Holy", 0, 0, 0, plr+1);
    }
    var pObj;
    // Eine Kirche gewählt? Dann Clonks umfärben
    if(iChurch)
    {
      var i=GetCrewCount(plr);
      while (i--)
        if(GetID( pObj = GetCrew(plr, i)) == PLDN || GetID(pObj) == PRST)
        {
          if(iChurch == scBloodchurch) SetGraphics("Dark", pObj, GetID(pObj));
          else SetGraphics("", pObj, GetID(pObj));
          pObj->~NewPortrait();
          if(GetCrew(plr, i)->~HasArmor()) {
            if(iChurch == scBloodchurch) SetGraphics("ArmoredDark", pObj, GetID(pObj));
            else SetGraphics("Armored", pObj, GetID(pObj)); }
        }
      GameCall("OnChurchChoose", plr);
    }
  }
  Local(2) = 1;
  // Sicht zurücksetzen
  SetCursor(GetOwner(), GetHiRank(GetOwner()));
  // ViewCursor zurücksetzen
  SetViewCursor(GetOwner());
  fDoCursorChange = 0;
  // Fertig; Objekt löschen
  var iPhase = 0;
  if(GetAction()=="Blood") iPhase = 20;
  SetAction("Turning"); SetPhase(iPhase);
  Message("", this);
  return AddEffect("IntVanish", this, 1, 1, this);
}

public func FxIntVanishTimer(pTarget, iNumber, iTime)
{
  if(GetPhase()==39) SetPhase(0);
  else SetPhase(GetPhase()+1);
  SetPosition(-100+iTime*2, -100-iTime*2+iTime*iTime/16);
  SetObjDrawTransform(1000-iTime*20, 0, 0, 0, 1000-iTime*20);
  if(iTime>=50) { RemoveObject(); return -1; }
}

public func ColorString(szString, iChurch)
{
  var iLength = GetLength(szString);
  var szNewString = "";
  for(var i=0; i < iLength; i++)
  {
    if(iChurch==scLightchurch) szNewString = Format("%s<c %x>%c</c>", szNewString, RGB(250-80*i/iLength, 100+80*i/iLength), GetChar(szString, i));
    else szNewString = Format("%s<c %x>%c</c>", szNewString, RGB(250-80*i/iLength, 0, 120*i/iLength), GetChar(szString, i));
  }
  return szNewString;
}

public func ChurchSparks(iChurch)
{
  // Helper
  var pObj = CreateObject(CHCS, 0, 0, GetOwner());
  pObj->SetAction("ShowSparks");
  if(FindObject(NTCH)) pObj->SetVisibility(VIS_Owner);
  else pObj->SetVisibility(VIS_Owner | VIS_Allies);
  // Graphischer Effekt
  var lwdt=LandscapeWidth(),lhgt=LandscapeHeight(),x=GetX(),y=GetY();
  var i=LandscapeWidth()*LandscapeHeight()/500;
  if(iChurch==scLightchurch)
       while (i--) CreateParticle("NoGravSpark", Random(lwdt)-x,Random(lhgt)-y, RandomX(-8,8),-20, 50+Random(50), RGBa(255-Random(100),255-Random(100),0,0), pObj);
  else while (i--) CreateParticle("NoGravSpark", Random(lwdt)-x,Random(lhgt)-y, RandomX(-8,8),20, 50+Random(50), RGBa(255-Random(100),0,Random(100),0), pObj);
  // Durchschütteln
  DoShakeViewPort(100, pObj);
}

public func DoShakeViewPort(int iLevel, object pObj) {
  if(iLevel <= 0) return false;
  var eff=AddEffect("ShakeEffect",pObj,200,1,0,CHCS);
  if (!eff) return false;
  EffectVar(0,pObj,eff)=iLevel;
  EffectVar(1,pObj,eff)=GetOwner(pObj);
  return (true);
}

// Dauer des Effektes: sobald iStrength==0
// Stärke des Effektes: iStrength=iLevel/(1.5*iTime+3)-iTime^2/400
public func FxShakeEffectTimer(object pTarget, int iEffectNumber, int iTime) {
  var iPlr, iLevel = EffectVar(0,pTarget,iEffectNumber);
  if(iLevel/((3*iTime)/2+3)-iTime**2/400<=0) return -1;
  var iStrength;
  for(var i=0; i<GetPlayerCount(); i++) {
    iPlr=GetPlayerByIndex(i);
    if(FindObject(NTCH)) if(EffectVar(1,pTarget,iEffectNumber)!=iPlr) continue;
    if(Hostile(iPlr, EffectVar(1,pTarget,iEffectNumber))) continue;
    if((iStrength=iLevel/((3*iTime)/2+3)-iTime**2/400)<=0) continue;
    // FIXME: Use GetViewOffset, make this relative, not absolute
    SetViewOffset(iPlr,Sin(iTime*100,iStrength),Cos(iTime*100,iStrength));
  }
}

public func FxShakeEffectStart(object pTarget, int iEffectNumber) {
  FxShakeEffectTimer(pTarget, iEffectNumber, GetEffect (0, pTarget, iEffectNumber, 6));
}

public func FxShakeEffectStop() {
  for(var i=0; i<GetPlayerCount(); i++) {
    // FIXME: Return the offset to the previous value, not zero
    SetViewOffset(GetPlayerByIndex(i),0,0);
  }
}

public func Remove() { RemoveObject(); }

public func StartTimer()
{
  iTimer = 36*10;
}

public func Light() { szChurch = ColorString("$Lightchurch$", scLightchurch); }
public func Blood() { szChurch = ColorString("$Bloodchurch$", scBloodchurch); }

protected func Timer()
{
  if(GetCursor(GetOwner())!=this) if(fDoCursorChange)
  {
    SetCursor(GetOwner(), this);
    SetViewCursor(GetOwner(), GetHiRank(GetOwner()));
  }
  if(GetAction()=="Turning" || GetAction()=="ShowSparks") return;
  if (iTeamEffect) if(GetLength(EffectVar(0, 0, iTeamEffect))<=1) return Message("$ChooseGod$:|%s", this, szChurch);
  if(!iTimer) return Message("$Voting$:|Teiwaz %d : Sawelô %d|%s", this, EffectVar(scBloodchurch, 0, iTeamEffect), EffectVar(scLightchurch, 0, iTeamEffect), szChurch);
  Message("$Voting$ (%ds):|Teiwaz %d : Sawelô %d|%s", this, iTimer/36, EffectVar(scBloodchurch, 0, iTeamEffect), EffectVar(scLightchurch, 0, iTeamEffect), szChurch);
  iTimer--;
  if(!iTimer) DoEnter(1);
  return;
}
