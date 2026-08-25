/*-- Kampf um den Rat --*/

#strict

func Initialize() {

  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(GOLD,Material("Earth"),0);
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(FLNT,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(ROCK,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(BONE,Material("Earth"),0);
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(LOAM,Material("Earth"),0);
  
  if(!FindObject(CNMT)) CreateObject(CNMT,10,10,-1); // Baumaterial
  if(!FindObject(FGRV)) CreateObject(FGRV,10,10,-1); // Flagge entfernbar
  if(!FindObject(REAC)) CreateObject(REAC,10,10,-1); // Clonk entlassen

  if(!FindObject(_ETG)) CreateObject(_ETG,10,10,-1); // Löscher

  if(!FindObject(SURR)) CreateObject(SURR,10,10,-1); // Aufgeben
  if(!FindObject(TACC)) CreateObject(TACC,10,10,-1); // Teamkonto
  if(!FindObject(REAC)) CreateObject(REAC,10,10,-1); // Clonk entlassen
  
  if(!FindObject(CLGD)) CreateObject(CLGD,10,10,-1); // Gold konvertieren
  if(!FindObject(STES)) CreateObject(STES,10,10,-1); // Magie energie
  if(!FindObject(SPST)) CreateObject(SPST,10,10,-1); // Holigon zum zaubern
  if(!FindObject(FSTF)) CreateObject(FSTF,10,10,-1); // Kostenlose Stäbe
  if(!FindObject(NTCH)) CreateObject(NTCH,10,10,-1); // Keine Team Kirchen
  
  if(!FindObject(MELE)) CreateObject(MELE,10,10,-1);
  
  SetSkyParallax(0, 90, 40);
}

global func MoveMap(iX, iY, iMovX, iMovY)
{
  for(var obj in FindObjects(Find_InRect(iX, iY, LandscapeWidth(), LandscapeHeight())))
    SetPosition(GetX(obj)+iMovX, GetY(obj)+iMovY, obj);
}

protected func RandomPalaID()
{
 var rand=Random(12)+1;
 if(!--rand)return(BRED);
 if(!--rand)return(MUSK);
 if(!--rand)return(SBLP);
 if(!--rand)return(PBLP);
 if(!--rand)return(BLTP);
 if(!--rand)return(BOW1);
 if(!--rand)return(FARP);
 if(!--rand)return(XARP);
 if(!--rand)return(ARWP);
 if(!--rand)return(SWOR);
 if(!--rand)return(AXE1);
 if(!--rand)return(SPER);
}

protected func RandomID()
{
 var rand=Random(14)+1;
 if(!--rand)return(STFN);
 if(!--rand)return(BRED);
 if(!--rand)return(SFLN);
 if(!--rand)return(GUNP);
 if(!--rand)return(WBRL);
 if(!--rand)return(SHIE);
 if(!--rand)return(BOW1);
 if(!--rand)return(FARP);
 if(!--rand)return(XARP);
 if(!--rand)return(ARWP);
 if(!--rand)return(SWOR);
 if(!--rand)return(AXE1);
 if(!--rand)return(SPER);
 if(!--rand)return(EFLN);
}

protected func InitializePlayer(int player, X, Y, foo, iTeam)
{
  CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->~InitPlayer(player,iTeam);
  RefreshScoreboard(FindObject2(Find_ID(_GID)));
  return(1);
}

protected func SetCastleOwner(int iPlr)
{
 var x=1;
 if(GetPlayerTeam(iPlr) == 2) x=(LandscapeWidth()/3)*2;
 
 for(var obj=0;obj=FindObject(0,x,0,LandscapeWidth()/3,LandscapeHeight(),0,0,0,NoContainer(),obj);)
 {
  if(~GetCategory(obj) & C4D_Structure() && !obj->~CastlePartWidth() && GetID(obj) != BANR)continue;
  if(GetOwner(obj) != -1)continue;
  SetOwner(iPlr,obj);
 }
}

protected func RelaunchPlayer(int player)
{
 // Dem Spieler einen neue Ritter und nen Paladin geben und diese ausrüsten
 if(GetPlayerTeam(player) == 1)
 {
   // Paladine haben kein Relaunch
   return();
/*   var pClonk = CreateObject(PLDN, 0,0, player);
   MakeCrewMember(pClonk, player);
   DoEnergy(100, pClonk);
 
   pClonk = CreateObject(KNIG, 0,0, player);
   MakeCrewMember(pClonk, player);
   DoEnergy(100, pClonk);*/
 }
 else
 {
   var pClonk = CreateObject(MAGE, 0,0, player);
   MakeCrewMember(pClonk, player);
   DoEnergy(100, pClonk);
 
   pClonk = CreateObject(MAGE, 0,0, player);
   MakeCrewMember(pClonk, player);
   DoEnergy(100, pClonk);
 }
 
 // Den Tod des Spielers verkünden
 Log("$Relaunch$", GetPlayerName(player));

 Sound("Trumpet");

 SetCursor(player, GetHiRank(player));
 CreateContents(FLAG, GetHiRank(player));

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
  if(iTeam==2)
  {
    // Magier haben keine Kirche
    MMSetPlayerChurch(iPlr,-1);
    var iCount;
    var obj;
    while(obj = GetCrew(iPlr,iCount++))
      DoMagicEnergy(1000, obj, 1); 
  }
}

func RefreshScoreboard(pStatue)
{
 var sLives="";
 if(!pStatue)
 {
  sLives="/";
 }
 else
  sLives=Format("%d",pStatue->MaxDamage()-pStatue->GetDamage());
 SetScoreboardData (-1,0,"$StatueLives$",-1);
 SetScoreboardData(0,0,sLives,1); 
 
 for(var cnt=0;cnt<GetPlayerCount();cnt++)
 {
  var plr=GetPlayerByIndex(cnt);
  DoScoreboardShow(5,plr);
 }
}

func StatueDamage(pFrom)
{
 RefreshScoreboard(pFrom);
} 

func StatueDestroyed()
{
 for(var cnt=0;cnt<GetPlayerCount();cnt++)
 {
  var iPlr=GetPlayerByIndex(cnt);
  if(GetPlayerTeam(iPlr) == 2)EliminatePlayer(iPlr);
 }
}

func GetTeamStartPosition(iTeam, fY)
{
  if(iTeam==1) { if(fY) return(1511); return(105+RandomX(-50,50)); }
  if(iTeam==2) { if(fY) return(600); return(1523+RandomX(-50,50)); }
}

func InitializeTeam(iTeam, iPlr)
{
  if(iTeam==1)
  {
    SetCastleOwner(iPlr);
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(BRCK, iPlr)));
  }
  if(iTeam==2)
  {
    SetCastleOwner(iPlr);
    SetOwner(iPlr, CreateContents(FLAG, FindObjectOwner(CPOF, iPlr)));
    SetOwner(iPlr, CreateContents(FLAG, FindObject(AHUT)));
  }
}

func GetTeamName(iTeam)
{
  if(iTeam==1) return("$TLeft$");
  if(iTeam==2) return("$TRight$");
}
