#strict

protected func Initialize()
 {
  
  //Fässer
  
  var spwn;
  spwn = CreateObject(OSPN, 95, 275, -1);
  spwn->Set(LCNT, 1000, 0, "DuroLava");
  
  spwn = CreateObject(OSPN, LandscapeWidth()-95, 275, -1);
  spwn->Set(LCNT, 1000, 0, "DuroLava");
  
  CreateObject(CHAN, 230, 370, -1);
  CreateObject(CHAN, LandscapeWidth()-230, 370, -1);
   

  //Fog Fog Foga!
  for(var i;i<50;++i) CreateParticle("Fog",Random(LandscapeWidth()),Random(LandscapeHeight()),RandomX(3,6),0,RandomX(1000,1500),RGB(250,175,0));

  SetSkyAdjust(HSL(0,0,100));
  SetMatAdjust(HSL(0,0,100));
  //SetMaterialColor(Material("Earth")); //Laut Günther nur in 8bit möglich..
  
  
  CreateObject(_PLO, LandscapeWidth()/2, LandscapeHeight()/2, NO_OWNER);
  
  AddEffect("KillZone", 0, 20, 5, 0, 0);
  
  ScriptGo(1);
  
  return(1);
 }


global func FxKillZoneTimer()
{
    for(var obj in FindObjects(Find_Func("IsClonk"), Find_OCF(OCF_Alive()), Find_InRect(0, LandscapeHeight()/2+200, LandscapeWidth(), LandscapeHeight()-LandscapeHeight()/2+200)))
    {
        if(GetKiller(obj) > -1)
            GetCrew(GetKiller(obj))->Kill(obj);
        else
            Kill(obj);
        
        for(var i = 0; i < 35; i++)
        {
            CreateParticle("Smoke", GetX(obj) + RandomX(-20,20), GetY(obj)+RandomX(-20,20), 0, 0, 250, RGB(200, 10, 10));
        }    
        
    }
    
    ExtractMaterialAmount(LandscapeWidth()/2, 510, Material("DuroLava"), 50);
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



func Script2()
{
 CreateParticle("Fog",RandomX(10,200),Random(LandscapeHeight()),RandomX(3,6),0,RandomX(1000,1500),RGB(250,175,0));

 for(var i=3;i>0;i--)
  CastParticles("Smoke",35,100,RandomX(30,870),RandomX(430,550),400,450,RGB(1,1,1),RGB(1,1,1));
  
 
 //Objektspawnpunkt: LandscapeWidth()/2,367
 goto(1);
}

func GetPositions() { return([80,240,
                             870,240,
                             240,340,
                             710,340,
                             350,340,
                             600,340,
                             480,340]); }
                             
                             
func GetChosableGoalList()
{
    return[MELE];
} 

func GetChosableRuleList()
{
    return[BLLT, NOHE, GRAV, MMRG];
}                           

func VolcanoMaterial() { return(Material("Durolava")); }
