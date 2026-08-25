/*-- Kleines Siedelfest --*/

#strict

func Initialize() {
  CreateObject(_SCR,10,10);
for (var i=LandscapeWidth()/4;i<LandscapeWidth();i+=(LandscapeWidth()/2))
{
// Kleine Burg
  Var(0)=i; Var(1)=LandscapeHeight()/3;
if(FindConstructionSite(CST1, 0, 1))
	var pObj = CreateObject(CST1, Var(0), Var(1), -1);
	FreeRect(GetX(pObj)-30, GetY(pObj)-25, 60, 50);
	//DrawMaterialRect(Material("Earth"),GetX(pObj)-30,GetY(pObj)+45,60,20);
// Turm 
  if (i<LandscapeWidth()/2) Var(0)=i+300;
  if (i>LandscapeWidth()/2) Var(0)=i-300;
  Var(1)=LandscapeHeight()/3;
if(FindConstructionSite(CST1, 0, 1))
	var pTow = CreateObject(TWR2, Var(0), Var(1), -1);
	FreeRect(GetX(pTow)-24, GetY(pTow)-32, 48, 64);
	//DrawMaterialRect(Material("Granite"),GetX(pTow)-24,GetY(pTow)+59,48,20);
  if (i<LandscapeWidth()/2) pTow->ControlRight();
  if (i>LandscapeWidth()/2) pTow->ControlLeft();
}
// Am Anfang weht kein Wind
SetWind(0);
// Zähler starten
ScriptGo(1);
// Meeresspiegel
  var x = -150;
  while ((x += 200) < LandscapeWidth())
    CreateObject(DRAI, x, GetHorizonHeight(x)+1,-1);
return(1);
}

public func Script20()
{
  SetMaxPlayer(0);
  return(ScriptGo(0));
}

protected func InitializePlayer(int player, X, Y, foo, iTeam)
{
  // Den Spieler ausrüsten und platzieren
  CreateObject(_PLO, LandscapeWidth()/2, LandscapeHeight()/2, player)->InitPlayer(player,iTeam);
  UpdateScoreBoard(iTeam, true);
  return(1);
}

global func DrawMaterialRect(int mat, int x, int y, int w, int h)
 {
    DrawMaterialQuad(Format("%s-Smooth", MaterialName(mat)), x, y, x + w, y, x + w, y - h, x, y - h, false);
 }

private func GetHorizonHeight(x)
{
  var y;
  while (y < LandscapeHeight() && !GBackSemiSolid(x, y))
    y += 10;
  while (y > LandscapeHeight() || GBackSemiSolid(x, y))
    --y;
  return(y);
}

// Gemeinschaftskonten GameCall

public func TACC_BlockFirstAllianceAdd()
{  return(1);  }

public func TACC_BlockTeamExchange(int iType)
{
  if (iType == TACC_TypeBaseMat) return(1);
  return(0);
}