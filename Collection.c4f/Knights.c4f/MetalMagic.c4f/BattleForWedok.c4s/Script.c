/*-- Kampf um Wedok --*/

#strict

static iSaintHoodTeam;
static pTimerLight;
static pTimerDark;

func Initialize() {
  RemoveAll(CLK_);
//  DigCastlesFree();
  
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(GOLD,Material("Earth"),0);
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(FLNT,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(ROCK,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(BONE,Material("Earth"),0);
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(LOAM,Material("Earth"),0);
  
  if(!FindObject(CNMT)) CreateObject(CNMT,10,10,-1); // Baumaterial
//  if(!FindObject(FGRV)) CreateObject(FGRV,10,10,-1); // Flagge entfernbar
  if(!FindObject(REAC)) CreateObject(REAC,10,10,-1); // Clonk entlassen

  if(!FindObject(_ETG)) CreateObject(_ETG,10,10,-1); // Lï¿½scher

  if(!FindObject(SURR)) CreateObject(SURR,10,10,-1); // Aufgeben
  if(!FindObject(TACC)) CreateObject(TACC,10,10,-1); // Teamkonto
  if(!FindObject(REAC)) CreateObject(REAC,10,10,-1); // Clonk entlassen
  
  if(!FindObject(NOUD)) CreateObject(NOUD,10,10,-1); // Keine Untoten
  if(!FindObject(STES)) CreateObject(STES,10,10,-1); // Magie energie
	if(!FindObject(MGES)) CreateObject(MGES,10,10,-1); // Zauberenergie nachschub
  if(!FindObject(SPST)) CreateObject(RSST,10,10,-1); // Eingeschränktes Heiligtum
  if(!FindObject(FSTF)) CreateObject(FSTF,10,10,-1); // Stäbe für Magi
  
  if(!FindObject(NBTR)) CreateObject(NBTR,10,10,-1); // Nichtbrennbare Bäume

  if(!FindObject(MELE)) CreateObject(MELE,10,10,-1);

  var obj;
  for(obj in FindObjects(Find_ID(CPEC)))
    obj->SetPhysical("Float",400, 2);

  pTimerLight = CreateObject(CLK_);
  pTimerLight->Stop();
  pTimerDark = CreateObject(CLK_);
  pTimerDark->Stop();
  SetPosition(50,158, pTimerDark);
  SetGraphics("Dark", pTimerDark, CLK_);

  iSaintHoodTeam = 0;
  
  // Schilde links von der Mitte sollen schwarz sein
  for(obj in FindObjects(Find_ID(SHIE), Find_InRect(0,0,LandscapeWidth()/2, LandscapeHeight())))
  {
    obj->OnChurchChange(scBloodchurch);
  }

  // Schilde links von der Mitte sollen schwarz sein
  for(obj in FindObjects(Find_ID(CPGL)))
    AddEffect("DoorCheck", obj, 1, 10);
  for(obj in FindObjects(Find_ID(CPGR)))
    AddEffect("DoorCheck", obj, 1, 10);

  ScriptGo(1);
}

protected func Script15()
{
  var pSaintHood = FindObject(SNHD);
  if(!pSaintHood) return GameOver();
  var iPlr = GetOwner(pSaintHood);
  if(iPlr==-1) return goto(5);
  var iTeam = GetPlayerTeam(iPlr);
  if(iTeam != iSaintHoodTeam)
  {
    iSaintHoodTeam = iTeam;
    if(iTeam==1)
    {
      pTimerDark->Start();
      pTimerLight->Stop();
      SetPosition(50,158, pTimerLight);
      SetPosition(50,120, pTimerDark);
    }
    else
    {
      pTimerLight->Start();
      pTimerDark->Stop();
      SetPosition(50,138, pTimerDark);
      SetPosition(50,100, pTimerLight);
    }
    Sound("Trumpet");
  }
  
  return goto(5);
}

global func FxDoorCheckTimer(pTarget, iNumber, iTime)
{
  if(FindObject2(Find_Allied(GetOwner(pTarget)), Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive), Find_Distance(50, GetX(pTarget), GetY(pTarget))))
    pTarget->Open();
  else
    pTarget->Close();
}

protected func TimeUp()
{
  for(var cnt=0;cnt<GetPlayerCount();cnt++)
  {
   var plr=GetPlayerByIndex(cnt);
   if(CheckTeam(plr)!=iSaintHoodTeam) EliminatePlayer(plr);
  }
}

func SainthoodConquerTime() { return 300; }

protected func RandomPalaID()
{
 var rand=Random(12)+1;
 if(!--rand) return BRED;
 if(!--rand) return MUSK;
 if(!--rand) return SBLP;
 if(!--rand) return PBLP;
 if(!--rand) return BLTP;
 if(!--rand) return BOW1;
 if(!--rand) return FARP;
 if(!--rand) return XARP;
 if(!--rand) return ARWP;
 if(!--rand) return SWOR;
 if(!--rand) return AXE1;
 if(!--rand) return SPER;
}

protected func RandomID()
{
 var rand=Random(14)+1;
 if(!--rand) return STFN;
 if(!--rand) return BRED;
 if(!--rand) return SFLN;
 if(!--rand) return GUNP;
 if(!--rand) return WBRL;
 if(!--rand) return SHIE;
 if(!--rand) return BOW1;
 if(!--rand) return FARP;
 if(!--rand) return XARP;
 if(!--rand) return ARWP;
 if(!--rand) return SWOR;
 if(!--rand) return AXE1;
 if(!--rand) return SPER;
 if(!--rand) return EFLN;
}

protected func InitializePlayer(int player, X, Y, foo, iTeam)
{
  CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->~InitPlayer(player,iTeam);
  MAHP->ShowMessage("$ScenDesc$", player);
  return(1);
}

protected func SetCastleOwner(int iPlr)
{
 var x=1;
 if(GetPlayerTeam(iPlr) == 2) x=(LandscapeWidth()/3)*2;
 
 for(var obj=0;obj=FindObject(0,x,0,LandscapeWidth()/3,LandscapeHeight(),0,0,0,0,obj);)
 {
//  if(~GetCategory(obj) & C4D_Structure() && !obj->~CastlePartWidth() && GetID(obj) != BANR)continue;
  if(GetOwner(obj) != -1)continue;
  SetOwner(iPlr,obj);
 }
}

protected func RelaunchPlayer(int player)
{
 // Den Tod des Spielers verkünden
 Log("$Relaunch$", GetPlayerName(player));

 Sound("Trumpet");

 // Dem Spieler zwei neue Ritter und nen Paladin geben und diese ausrüsten
 var pClonk = CreateObject(PLDN, 0,0, player);
 MakeCrewMember(pClonk, player);
 DoEnergy(100, pClonk);

 pClonk = CreateObject(MAGE, 0,0, player);
 MakeCrewMember(pClonk, player);
 DoEnergy(100, pClonk);
 
 SetCursor(player, GetHiRank(player));

 // Jetzt muss der Spieler noch etwas warten
 CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->RelaunchPlayer(player);
 return(1);
}

protected func RemovePlayer(int player)
{
 // Spielervariable zurücksetzen, damit Laufzeitbeitritt mit Aufgeben funzt
 Global(player) = 0;
 return(1);
}

// Callbacks aus dem PlayerObject
func JoinTeam(iPlr, iTeam)
{
 var iChurch=scBloodchurch;
 if(iTeam == 2) iChurch=scLightchurch;
 MMSetPlayerChurch(iPlr,iChurch);
 var pChooser = FindObjectOwner(CHCS, iPlr);
 if(pChooser) RemoveObject(pChooser);
}

func GetTeamStartPosition(iTeam, fY)
{
  if(iTeam==1) { if(fY) return(458); return(145); }
  if(iTeam==2) { if(fY) return(458); return(1781); }
}

func InitializeTeam(iTeam, iPlr)
{
  if(iTeam==1)
  {
    SetCastleOwner(iPlr);
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(CPOF, iPlr)));
  }
  if(iTeam==2)
  {
    SetCastleOwner(iPlr);
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(CPOF, iPlr)));
  }
  // Schilde links von der Mitte sollen schwarz sein
  for(obj in FindObjects(Find_ID(SHIE), Find_InRect(0,0,LandscapeWidth()/2, LandscapeHeight())))
  {
    obj->OnChurchChange(scBloodchurch);
  }
}

func GetTeamName(iTeam)
{
  if(iTeam==1) return("$TLeft$");
  if(iTeam==2) return("$TRight$");
}
