/*-- Greifenrennen --*/

#strict

static aGriffons;
static aRespawnX;
static aRespawnY;

func Initialize()
{
  CreateObject(NTCH);
  aGriffons = [];
  aRespawnX = [];
  aRespawnY = [];
}

global func SetRespawn(iX, iY, iPlr) { aRespawnX[iPlr] = iX; aRespawnY[iPlr] = iY; }

global func GetRespawn(fY, iPlr) { if(fY) return(aRespawnY[iPlr]); return(aRespawnX[iPlr]); }

func CheckRACEGoal(iPlr)
{
  if(FindObject2(Find_ID(SNHD), Find_Distance(50, GetX(GetCursor(iPlr)), GetY(GetCursor(iPlr))))) return(1);
  return(-1);
}

protected func InitializePlayer(int player, int tx, int ty, object pBase, int iTeam)
{
  SetRespawn(64,726, player);
//  if(!MMGetPlayerChurch(player))
 //   Enter(CreateObject(DSHP, 100, 100, player),GetCrew(player));
//  else
  // Kirche setzen
   var iChurch = scLightchurch;
   MMSetPlayerChurch(player,iChurch);
   var pChooser = FindObjectOwner(CHCS,player);
   if(pChooser) RemoveObject(pChooser);
  JoinPlayer(player);
  return(1);
}

func OnChurchChoose(iPlr)
{
  RemoveObject(Contained(GetCrew(iPlr)), 1);
  JoinPlayer(iPlr);
}

protected func RelaunchPlayer(iPlr)
{
  var crew=CreateObject(PLDN,10,10,iPlr);
  MakeCrewMember(crew,iPlr);
 
  JoinPlayer(iPlr);
}

protected func JoinPlayer(iPlr)
{
  var x = GetRespawn(0, iPlr);
  var y = GetRespawn(1, iPlr);

  if(aGriffons[iPlr])
  {
    var pGriffon = aGriffons[iPlr];
    if(FindObject2(Find_ActionTarget(pGriffon)))
    {
      for(var obj in FindObjects(Find_ID(GRFN), Find_ID(SARN)))
      {
        if(!FindObject2(Find_ActionTarget(obj)))
        {
          pGriffon = obj;
          break;
        }
      }
    }
    if(pGriffon) Kill(pGriffon);
  }
 
  var crew = GetCrew(iPlr);
  SetPosition(x,y,crew);
  DoEnergy(100,crew);
  DoMagicEnergy(20,crew);
  SelectCrew(iPlr,crew,1);

  if(MMGetPlayerChurch(GetOwner(crew))==scBloodchurch) pGriffon = crew->CreateObject(SARN,0,5);
  else pGriffon = crew->CreateObject(GRFN,0,5);
  aGriffons[iPlr] = pGriffon;
  SetDir(1, pGriffon);
  SetCommand(crew, "Enter", pGriffon);
  SetCommand(pGriffon, "Follow", crew);
}
