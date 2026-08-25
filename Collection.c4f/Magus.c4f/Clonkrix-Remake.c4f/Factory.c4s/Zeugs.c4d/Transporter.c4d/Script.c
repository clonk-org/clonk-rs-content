#strict

protected func Initialize()
{
    SetAction("Fly");
    Pos();
}

protected func Left()
{
    SetXDir(RandomX(-80,-70),this());
}

protected func Right()
{
    SetXDir(RandomX(70,80),this());
}

protected func Pos()
{
    SetPosition(GetX(FindObject(SCHN,0,0,-1,-1)),GetY(FindObject(SCHN,0,0,-1,-1))-20,0);
}

func Damage(int iChange, int iByPlayer)
{
    SetOwner(iByPlayer);
    CastObjects2(EFLN, 10, 50,0,0,GetOwner());
    Explode(30);
}
