/*-- Groﬂer Kristall --*/

#strict 2

protected func Initialize() 
{
 var rand=1;
 if(Random(2) == 1)rand=-1;
 SetObjDrawTransform(1000*rand,0,0,0,1000);
}
