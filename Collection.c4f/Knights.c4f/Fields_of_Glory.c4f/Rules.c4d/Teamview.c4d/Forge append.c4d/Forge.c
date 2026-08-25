#strict
#appendto FRGE


private func Burning()
{
  if(FindObject(RTVW)) SetPlrViewRange(10);
  if(ContentsCount(METL))
    SetAction("Melting");

  if(GetActTime() > 1500) {
    if(FindObject(RTVW)) SetPlrViewRange(0);
    SetAction("Idle");
    }
}