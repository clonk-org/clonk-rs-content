#strict

func Initialize() {
	var id;
	Local()=1;
	if (id=FindObject(GetID()))
	  { Local()+=Local(0,id); RemoveObject(id); }
	SetAction("Wait");}

func Initialized() {
var i;
SetGamma(RGB(Local()*6,Local()*6,Local()*6),RGB(128+Local()*2,128+Local()*2,128+Local()*2),RGB(255-Local(),255-Local(),255-Local()));
while(i<(Local()*15*Sqrt(LandscapeWidth()*LandscapeHeight())/800)) 
	{i++; CreateParticle("Nebel",Random(LandscapeWidth()+150)-GetX(),Random(LandscapeHeight()+150)-GetY(),0,0,RandomX(1000,1500));}}