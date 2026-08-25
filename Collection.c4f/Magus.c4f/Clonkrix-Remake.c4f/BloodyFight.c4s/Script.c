#strict
 
/*-- Bloody fight --*/

protected func Initialize()
 {
  
  SetSkyAdjust(RGB(70,70,70));
  SetMatAdjust(RGB(125,125,125));
    
  CreateObject(_PLO,LandscapeWidth()/2, LandscapeHeight()/2, -1);
  
  var spawn = CreateObject(OSPN, 725, 395, -1);
  spawn->Set(LCNT, 1000);
  
 var spawn = CreateObject(OSPN, 240, 505, -1);
  spawn->Set(LCNT, 1000);
  
  AddEffect("BloodyFog", 0, 20, 60, 0);
  
  SetSkyParallax(0,15,15);
  
  return(1);
 }

global func FxBloodyFogTimer()
{
    CreateParticle("Fog", -105, RandomX(LandscapeHeight()-20, LandscapeHeight()+20), RandomX(5,13), 0, RandomX(1000, 1500), RGB(250, 00, 20));
}

func InitializePlayer(int iPlr)
 {  
 InitializeScoreboard(iPlr);
 SetFoW(true, iPlr);
   return(JoinPlr(iPlr));  }
   
func RemovePlayer(iPlr)
{ 
  UpdateScoreboard(iPlr,1); 
}

func GetPositions() { return([665,300,
                              890,305,
                              470,230,
                              60,370,
                              650,120,
                              890,120,
                              390,420]); }
                              
                              
func GetChosableGoalList()
{
    return[MELE];
} 

func GetChosableRuleList()
{
    return[BLLT, NOHE, GRAV, MMRG];
} 

func RelaunchPlayer(int iPlayer, object pClonk)
{
    CreateObject(GOST, GetX(pClonk), GetY(pClonk), iPlayer);
}                               
