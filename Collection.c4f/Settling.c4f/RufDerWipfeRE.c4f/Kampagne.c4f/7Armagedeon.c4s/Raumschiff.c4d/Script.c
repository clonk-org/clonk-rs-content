/*-- Raumschiff --*/

#strict
#include B210

local iYDir;

func BasicalConstruction(){ return !ObjectCount(RAKT);}

func Initialize()
{
  SetEntrance(true);
  GameCall("RocketBuild");
  SetCategory(C4D_Vehicle);
}

func Start()
{
  SetAction("Start");
}

func StartEff()
{
  Sound("Blast1");
  CreateParticle("Blast",RandomX(-100,100),GetDefBottom()-GetY(),RandomX(-1,1),RandomX(-1,1),RandomX(50,600),RGB(255,RandomX(127,255),RandomX(127,255)));
}

func Started()
{
  Sound("Blast2");
  iYDir=5;
  SetComDir(COMD_Up);
  CreateObject(LGFL,0,0,-1)->Init(this, 0, 0, 200, RGB(155,155,100), true);
}

func FlyEff()
{
  if(Random(4))
    CreateParticle("Antrieb",RandomX(-100,100),GetDefBottom()-GetY()-35,RandomX(-1,1),-GetYDir(),RandomX(300,500),RGB(255,255,255),this,true);
  ShakeFree(GetX(),GetY(),75);
  ShakeFree(GetX(),GetY()+345,75);
  ShakeFree(GetX(),GetY()-345,75);
  if(Stuck())
    SetPosition(GetX(),GetY()-1);
  ResetYDir();
  if(GetY()<-200)
  {
    GameOver();
    Schedule("RemoveObject()",350);
  }
}

func ResetYDir()
{
  iYDir=BoundBy(iYDir+1,0,300);
  SetYDir(-iYDir/2);
}
