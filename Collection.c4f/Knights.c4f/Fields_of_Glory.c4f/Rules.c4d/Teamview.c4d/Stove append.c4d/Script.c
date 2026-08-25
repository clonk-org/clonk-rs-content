#strict
#appendto STOV

private func Burning()
{
  if(FindObject(RTVW)) SetPlrViewRange(20);
  if(GetActTime() > 5000) {
   SetAction("Idle");
   if(FindObject(RTVW)) SetPlrViewRange(0);
   }
}