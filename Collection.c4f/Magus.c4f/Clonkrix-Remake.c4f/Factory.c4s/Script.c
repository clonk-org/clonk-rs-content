#strict

/*-- Chemiefabrik --*/

protected func Initialize()
 {
  var spawn = CreateObject(OSPN, 900, 315, -1);
  spawn->Set(GF3V, 1000);
  
  spawn = CreateObject(OSPN, 986, 230, -1);
  spawn->Set(GF3V, 1000);
  
  spawn = CreateObject(OSPN, 225, 365, -1);
  spawn->Set(GF3V, 1000);
  
  spawn = CreateObject(OSPN, 620, 295, -1);
  spawn->Set(GF3V, 1000);    

  spawn = CreateObject(OSPN, 1130, 315, -1);
  spawn->Set(TN3V, 1000);
  
  spawn = CreateObject(OSPN, 775, 485, -1);
  spawn->Set(TN3V, 2000);
  
  spawn = CreateObject(OSPN, 300, 205, -1);
  spawn->Set(LCNT, 1000, 0, "Acid");      
  
  AddEffect("Transporter", 0, 20, 250, 0);
  
  SetAtmosphere();
  
  CreateObject(_PLO,LandscapeWidth()/2, LandscapeHeight()/2, -1);
  
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
 

func GetPositions() { 
  return([RandomX(184,250),320,RandomX(512,560),260,RandomX(165,175),160,RandomX(950,1050),200,RandomX(670,740),100,RandomX(370,500),170]); } 
 
global func FxTransporterTimer()
{
    if(!Random(12))
    {
        if(!Random(2))
            CreateObject(TRNS,LandscapeWidth(),0)->Left();
         else
            CreateObject(TRNS,0,0)->Right();   
    }
}
 
 
func Particle()
{
    for(var i; i < 50; i++)
        CreateParticle("Fog",RandomX(0,1250),RandomX(700,800),0,0,1500,HSL(0,0,20));
}
 
  
func SetAtmosphere()
 {
  Particle();

  var dark = CreateObject(DARK);
  dark->SetDarkness(3);
    
  //SetSkyAdjust(RGBa(106,106,106,127), RGB(106,106,106));
  
  CreateObject(CLGH,298,229,-1)->Flickering();
  CreateObject(CLGH,257,79,-1)->Flickering();
  CreateObject(CLGH,1125,331,-1)->Flickering();
  
  /* Alte Lichter zu grell 
  CreateObject(BLGH,790,430,-1);
  CreateObject(BLGH,330,370,-1);
  CreateObject(BLGH,320,165,-1); */
  
  //var lgh1=CreateObject(FLGH,667,320,-1);
  var lgh1=CreateObject(FLGH,667,320,-1);
  lgh1->Light()->TurnTo(50);
  lgh1->Light()->SetTurningPoints(45,170);
  lgh1->Light()->TurnRight();
  //var lgh2=CreateObject(FLGH,946,310,-1)->ControlRight();
  var lgh2=CreateObject(FLGH,944,234,-1);
  lgh2->Light()->TurnTo(-60);
  lgh2->Light()->SetTurningPoints(-150,-45);
  lgh2->Light()->TurnLeft();

  return(1);
 }
 
func DayMode()
 {
  Particle();
  RemoveAll(BLGH);
  RemoveAll(FLGH);
  RemoveAll(FLHH);
  RemoveAll(DARK);
  SetSkyAdjust(RGBa(255,255,255),0);
  
  return(1);
 }
 
 func GetChosableGoalList()
{
    return[MELE];
} 

func GetChosableRuleList()
{
    return[BLLT, NOHE, GRAV, MMRG];
}   
