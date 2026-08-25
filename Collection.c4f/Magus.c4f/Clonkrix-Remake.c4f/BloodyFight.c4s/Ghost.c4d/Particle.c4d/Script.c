/*-- Neues Objekt --*/

#strict

func Initialize()
{
    AddEffect("Check", this(), 20, 1, this());
    if(Stuck())
        RemoveObject();
}

func FxCheckTimer(pTarget)
{
    var obj = FindObject2(Find_Distance(10), Find_Func("IsClonk"));
    if(obj)
    {
        DoMagicEnergy(5, obj);
        DoEnergy(5, obj);
        pTarget->Hit();
    } 
    
    CreateParticle("cFire3", 0, 0, 0, 0, 60, RGB(15,155,15));
}

func Hit()
{
    CastParticles("cFire3", 20, 50, 0, 0, 50, 60, RGB(15,155,15), RGB(15,200, 15));
    RemoveObject();
}


