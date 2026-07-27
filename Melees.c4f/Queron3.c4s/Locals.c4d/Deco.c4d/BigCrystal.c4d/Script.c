/*-- Groﬂer Kristall --*/

#strict 2

protected func Initialize() 
{
 var rand=1;
 if(Random(2) == 1)rand=-1;
 SetObjDrawTransform(1000*rand,0,0,0,1000);
}

protected func Activity()
{
  // Dieser Kristall regeneriert Zauberenergie
  var cnt;
  for (var obj in FindObjects(Find_AtRect(-15,-23,30,45), Find_OCF(OCF_Alive)))
    if (GetPhysical("Magic", 0, obj))
      if (DoMagicEnergy(1, obj))
      {
        // Aktiv am Aufladen!
        cnt=5;
        while(cnt--) CreateParticle("PSpark", GetX(obj)-GetX()+Random(13)-6, GetY(obj)-GetY()+RandomX(11),
                                              0,-1-Random(2),40,0x1f7fff);
      }
}

func Break()
{
  Sound("RockBreak1");
  CastObjects(CRYS,5,10,GetOwner());
  return RemoveObject(this, true);
}

public func Activate()
{
	return SetAction("Active");
}
