/*-- Bolzen --*/

#appendto LGTS
#strict 2

static g_lightningCommand;

private func Remove()
{
  if(g_lightningCommand)
  {
    FindObject(_STY)->Call(g_lightningCommand);
    g_lightningCommand = 0;
  }
  return inherited(...);
}