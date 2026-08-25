/*-- Toter Vogel --*/

#strict

public func Cook()
{
    ChangeDef(CBRD);
    return(1);
}

protected func Activate(object pClonk)
{
    pClonk->~Feed(15);
    RemoveObject();
    return(1);
}