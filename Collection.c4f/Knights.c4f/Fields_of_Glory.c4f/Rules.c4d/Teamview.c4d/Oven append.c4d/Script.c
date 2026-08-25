#strict
#appendto OVEN


private func Baking()
{
  if(FindObject(RTVW)) SetPlrViewRange(10);
  // Noch nicht fertig
  if(GetActTime() < 250) return(1);

  if(FindObject(RTVW)) SetPlrViewRange(0);
  SetAction("Idle");
  // Alle Teige backen. Bei Holz bringt Bake einfach nix.
  var obj, i;
  var baked = false;
  while(obj = Contents(i ++) )
    if(obj->~Bake() )
    {
      Exit(obj);
      baked = true;
      -- i;
    }
    
  if(baked) SetAction("OpenDoor");
  Smoke(0, 0, 12);
  Sound("Pshshsh");
}