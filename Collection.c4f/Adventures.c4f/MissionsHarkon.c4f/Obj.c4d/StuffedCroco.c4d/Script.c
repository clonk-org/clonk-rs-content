/*-- Ausgestopftes Krokodil zum drauf rumlaufen--*/

#strict

func Initialize() 
{ 
   if(!Random(2))
    {
     SetAction("Wait");
     SetDir(1);
    }
   else
    {
     SetAction("Wait2");
     SetDir(0);
    }
   
  return(1);
}

protected func Check()
{
    if (FindObject2(Find_OCF(OCF_Alive),Find_InRect(16,-5,20,20),Find_Exclude(this), Find_Layer(GetObjectLayer()),Find_NoContainer()))
     SetAction("SwingRight");
    else if (FindObject2(Find_OCF(OCF_Alive),Find_InRect(-36,-5,25,20),Find_Exclude(this), Find_Layer(GetObjectLayer()),Find_NoContainer()))
     SetAction("SwingLeft"); 
    return(1);
}

protected func CheckLeft()
{
    if (!FindObject2(Find_OCF(OCF_Alive),Find_InRect(-36,-5,25,20),Find_Exclude(this), Find_Layer(GetObjectLayer()),Find_NoContainer()))
     SetAction("SwingLeftBack");
    else if(!Random(3)) Sound("WoodKnack*");
    return(1);
}

protected func CheckRight()
{
    if (!FindObject2(Find_OCF(OCF_Alive),Find_InRect(16,-5,20,20),Find_Exclude(this), Find_Layer(GetObjectLayer()),Find_NoContainer()))
     SetAction("SwingRightBack");
    else if(!Random(3)) Sound("WoodKnack*");
    return(1);
}

protected func Check2()
{
    if (FindObject2(Find_OCF(OCF_Alive),Find_InRect(16,-5,20,20),Find_Exclude(this), Find_Layer(GetObjectLayer()),Find_NoContainer()))
     SetAction("SwingRight2");
    else if (FindObject2(Find_OCF(OCF_Alive),Find_InRect(-36,-5,25,20),Find_Exclude(this), Find_Layer(GetObjectLayer()),Find_NoContainer()))
     SetAction("SwingLeft2"); 
    return(1);
}

protected func CheckLeft2()
{
    if (!FindObject2(Find_OCF(OCF_Alive),Find_InRect(-36,-5,25,20),Find_Exclude(this), Find_Layer(GetObjectLayer()),Find_NoContainer()))
     SetAction("SwingLeftBack2");  
    else if(!Random(3)) Sound("WoodKnack*");
    return(1);
}

protected func CheckRight2()
{
    if (!FindObject2(Find_OCF(OCF_Alive),Find_InRect(16,-5,20,20),Find_Exclude(this), Find_Layer(GetObjectLayer()),Find_NoContainer()))
     SetAction("SwingRightBack2");
    else if(!Random(3)) Sound("WoodKnack*");
    return(1);
}