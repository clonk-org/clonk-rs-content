/* Ballon Mine Munition */

#strict

func Initialize()
{
Schedule("Hit()", Random(5)+20);
return(1);
}

protected func Hit()
{
Explode(Random(15)+10);
}
