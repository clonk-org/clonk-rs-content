#strict



protected func Activate(object pByClonk)
{
Sound("Magic1");
CastParticles("MSpark",10,20,0,0,70,100,RGB(250,250,0),RGB(250,250,0));

var wipf=FindObject(WIPF);
DrawParticleLine("PxSpark",AbsX(GetX(wipf)),AbsY(GetY(wipf)),0,0,5,70,RGB(250,250,10),RGB(250,250,10));
RemoveObject(wipf);

wipf=CreateObject(WIPF,GetX(FindObject(PLCE)),GetY(FindObject(PLCE)),-1);
SetClrModulation(RGB(220,230,70),wipf);

Enter(pByClonk,wipf);

RemoveObject();
}

