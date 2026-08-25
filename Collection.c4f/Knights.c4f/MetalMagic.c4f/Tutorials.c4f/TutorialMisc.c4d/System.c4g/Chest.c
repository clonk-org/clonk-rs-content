/*-- Kiste --*/

#strict 2

#appendto CHST

static g_ChestFound;

protected func Open()
{
  if(!Local())
  {
    g_ChestFound ++;
    if(g_ChestFound < GetChestCount())
      Message("Geheime Truhe gefunden!|Schon %d von %d gefunden.",this,g_ChestFound, GetChestCount());
    else
    {
      Message("Geheime Truhe gefunden!|Du hast alle %d Geheimtruhen gefunden!",this,g_ChestFound);
      Sound("Applause");
    }
    Local() = 1;
  }
  return _inherited(...);
}

global func GetChestCount() { return ObjectCount2(Find_ID(CHST)); }
