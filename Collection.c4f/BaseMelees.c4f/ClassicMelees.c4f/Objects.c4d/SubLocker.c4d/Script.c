/*-- U-Boot-Schutz --*/

#strict

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(),iByPlayer);
  return(1);
}