#strict

/*-- Hochhaus --*/
//hier sei fett vermerkt das cks C4Scripter stinkt!

protected func Initialize()
{
  SetSkyParallax(0,0,0,1);

  CreateObject(WEDA, 0, 0, -1);
  
  CreateObject(_PLO, LandscapeWidth()/2, LandscapeHeight()/2);
  
  var spawn;
  
  spawn = CreateObject(OSPN, 825, 265, -1);
  spawn->Set(_CRT, 1000);
  
  spawn = CreateObject(OSPN, 850, 265, -1);
  spawn->Set(_CRT, 1000);
  
  spawn = CreateObject(OSPN, 95, 350, -1);
  spawn->Set(_CRT, 1000);
  
  spawn = CreateObject(OSPN, 120, 350, -1);
  spawn->Set(_CRT, 1000);
  
  spawn = CreateObject(OSPN, 360, 200, -1);
  spawn->Set(_CRT, 1000);
  
  spawn = CreateObject(OSPN, 805, 400, -1);
  spawn->Set(_CRT, 1000);
  
  //Kunstregen
  CastPXS("Water2", 150, 50, 260,330);
  
  CastPXS("Water2", 150, 50, 150, 230);
  
  CastPXS("Water2", 150, 50, 355, 190);
  
  CastPXS("Water2", 150, 50, 560, 290);
  
  CastPXS("Water2", 200, 50, 850, 230);

  return(1);
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


/*func GetPositions() { return([510,250,
                              200,470,
                              490,470,
                              310,570,
                              440,660,
                              460,790,
                              270,860]); } */
                              
func GetPositions() { return([355,190,
                              155,240,
                              245,340,
                              790,250,
                              580,300,
                              415,330,
                              890,250,
                              740,380]); } 
                              
          
func GetChosableGoalList()
{
    return[MELE];
} 

func GetChosableRuleList()
{
    return[BLLT, NOHE, GRAV, MMRG];
}     
