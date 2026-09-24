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
      Message("$Chest01$",this,g_ChestFound, GetChestCount());
    else
    {
      Message("$Chest02$",this,g_ChestFound);
      Sound("Applause");
    }
    Local() = 1;
  }
  return _inherited(...);
}

global func GetChestCount() { return ObjectCount2(Find_ID(CHST)); }
