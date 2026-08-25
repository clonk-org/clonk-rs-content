/*-- Auswertungsstatus --*/

#strict 2

static aPlayerNames;
static aPlayerColor;

static aKills;
static aKillsForTeam;
static aTeamkills;

static aPlayerDamage;
static aPlayerTeamdamage;
static aHeal;

protected func Initialize()
{
  aPlayerNames      = [];
  aPlayerColor      = [];
  aKills            = [];
  aPlayerDamage     = [];
  aPlayerTeamdamage = [];
  aTeamkills        = [];
  aHeal             = [];
  aKillsForTeam     = [];
  return true;
}

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}

public func OnGameOver() { DoEvaluateStats(); Log("Yipii!"); }

public func GetTaggedPlayerNameByID(iPlrID)
{
  return Format("<c %x>%s</c>", aPlayerColor[iPlrID], aPlayerNames[iPlrID]);
}

public func DoEvaluateStats()
{
 // Normalschaden
 var maxDmg=0,dmgPlayer=0;
 for(var iPlr=1;iPlr<GetLength(aPlayerDamage);iPlr++)
 {
  if(aPlayerDamage[iPlr] > maxDmg)
  {
   maxDmg=aPlayerDamage[iPlr];
   dmgPlayer=iPlr;
  }
 }
 
 if(dmgPlayer)
 {
  AddEvaluationData(Format("$MostDamage$",GetTaggedPlayerNameByID(dmgPlayer),maxDmg),0);//GetPlayerID(dmgPlayer));
  Log("$MostDamage$",aPlayerNames[dmgPlayer],maxDmg);
 }
 
 // Teamschaden
 var maxDmg=0,dmgPlayer=0;
 for(var iPlr=1;iPlr<GetLength(aPlayerTeamdamage);iPlr++)
 {
  if(aPlayerTeamdamage[iPlr] > maxDmg)
  {
   maxDmg=aPlayerTeamdamage[iPlr];
   dmgPlayer=iPlr;
  }
 }
 
 if(dmgPlayer)
 {
  AddEvaluationData(Format("$MostTeamdamage$",GetTaggedPlayerNameByID(dmgPlayer),maxDmg),0);//GetPlayerID(dmgPlayer));
  Log("$MostTeamdamage$",aPlayerNames[dmgPlayer],maxDmg);
 }
 
 
 // Teamkills
 var maxKills=0,dmgPlayer=0;
 for(var iPlr=0;iPlr<GetLength(aTeamkills);iPlr++)
 {
  if(aTeamkills[iPlr] > maxKills)
  {
   maxKills=aTeamkills[iPlr];
   dmgPlayer=iPlr;
  }
 }
 
 if(dmgPlayer)
 {
  AddEvaluationData(Format("$MostTeamkills$",GetTaggedPlayerNameByID(dmgPlayer),maxKills),0);//GetPlayerID(dmgPlayer));
  Log("$MostTeamkills$",aPlayerNames[dmgPlayer],maxKills);
 }
 
 // Heilung
 var maxHeal=0,dmgPlayer=0;
 for(var iPlr=0;iPlr<GetLength(aHeal);iPlr++)
 {
  if(aHeal[iPlr] > maxHeal)
  {
   maxHeal=aHeal[iPlr];
   dmgPlayer=iPlr;
  }
 }
 
 if(dmgPlayer)
 {
  AddEvaluationData(Format("$MostHeal$",GetTaggedPlayerNameByID(dmgPlayer),maxHeal),0);//GetPlayerID(dmgPlayer));
  Log("$MostHeal$",aPlayerNames[dmgPlayer],maxHeal);
 }
 
}

public func InitializePlayer(int iPlr, int x, int y, object pBase, int iTeam)
{
  // Namen speichern
  var iPlrID = GetPlayerID(iPlr);
  aPlayerNames[iPlrID] = GetPlayerName(iPlr);
  aPlayerColor[iPlrID] = GetPlrColorDw(iPlr);
  return true;
}

public func OnClonkCreation(object pClonk, int iKilledByPlr)
{
  // Effekt verzögert hinzufügen (damit wir z.B. Starten mit voller Energie nicht als Heilung gezählt)
  Schedule(Format("AddEffect(\"_EVSDamageCounter\",Object(%d),1,0,this))", ObjectNumber(pClonk)), 1, 1);
}

public func OnClonkDeath(object pClonk, int iKilledByPlr)
{
 if(Hostile(GetOwner(pClonk),iKilledByPlr) && iKilledByPlr != GetOwner(pClonk))
 {
  if(GetPlayerName(iKilledByPlr))
   {
    aKills[GetPlayerID(iKilledByPlr)]++;
    aKillsForTeam[GetPlayerTeam(iKilledByPlr)-1]++;
   }
 }
 else 
 {
  if(GetPlayerTeam(iKilledByPlr) == GetPlayerTeam(GetOwner(pClonk)))
  {
   aKills[GetPlayerID(iKilledByPlr)]--;
   aKillsForTeam[GetPlayerTeam(iKilledByPlr)-1]--;
   aTeamkills[GetPlayerID(iKilledByPlr)]++;
  } 
 }
 return true;
}

func Fx_EVSDamageCounterDamage(pTarget,iEffectNumber,iDmgEngy,iCause,iBy)
{
 if(iDmgEngy > 0)
 {
  aHeal[GetPlayerID(GetOwner(pTarget))]+=iDmgEngy/1000;
  return iDmgEngy;
 }
 
 if(iBy != -1 && GetPlayerName(iBy))
 {
  if(iBy == GetOwner(pTarget) || !Hostile(iBy,GetOwner(pTarget)))
  {
   aPlayerTeamdamage[GetPlayerID(iBy)]-=iDmgEngy/1000;
  }
  else
  {
   aPlayerDamage[GetPlayerID(iBy)]-=iDmgEngy/1000;
  }
 }
 
 return iDmgEngy;
}
