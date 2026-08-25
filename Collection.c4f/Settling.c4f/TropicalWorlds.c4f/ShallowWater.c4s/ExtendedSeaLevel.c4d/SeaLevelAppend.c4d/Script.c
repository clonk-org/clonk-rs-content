/*-- Meeresspiegel-Append --*/

#strict
#appendto DRAI

local iMaterial;

local iTimer;
local iTimer2;
local iMod;

protected func Initialize()
{
  // Doppelte Drainagen entfernen (z.B. wenn zusätzlich zum erweiterten der normale Meeresspiegel aktiviert ist)
  if(FindObject2(Find_ID(DRAI),Find_AtPoint(0,0),Find_Exclude(this()))) RemoveObject();
  iMaterial=Material("Water");
  if(GBackLiquid(0,1)) iMaterial=GetMaterial(0,1);
  if(InLiquid()) iMaterial=GetMaterial();
  return(_inherited());
}

private func Draining()
{
  if(GBackLiquid()) ExtractMaterialAmount(0,0,GetMaterial(),20);
  if(FindObject(BS5P))
  {
    if(!GBackLiquid(0,1))
    {
      var y;
      for(var i=0;i<100;i++) if(GBackLiquid(0,i) && PathFree(GetX(),GetY(),GetX(),GetY()+i)) y=i;
      for(i=0;i<20;i++) InsertMaterial(iMaterial,0,y);
    }
  }
 if(iMod==0) {
 iTimer+=10;
 if(iTimer == 100 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 200 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 300 && iMod == 0){ SetPosition(GetX(),GetY()+1);  } 
 if(iTimer == 400 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 500 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 600 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 700 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 800 && iMod == 0){ SetPosition(GetX(),GetY()+1);  } 
 if(iTimer == 900 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 1000 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 1100 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 1200 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 1300 && iMod == 0){ SetPosition(GetX(),GetY()+1);  } 
 if(iTimer == 1400 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 1500 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 1600 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 1700 && iMod == 0){ SetPosition(GetX(),GetY()+1);  } 
 if(iTimer == 1800 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 1900 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 2000 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 2100 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 2200 && iMod == 0){ SetPosition(GetX(),GetY()+1);  } 
 if(iTimer == 2300 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 2400 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 2500 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 2600 && iMod == 0){ SetPosition(GetX(),GetY()+1);  }
 if(iTimer == 2700 && iMod == 0){ SetPosition(GetX(),GetY()+1);  } 
 if(iTimer == 2800 && iMod == 0){ SetPosition(GetX(),GetY()+1);  } 
 if(iTimer > 2900 && iMod == 0){
  SetPosition(GetX(),GetY()+1); 
  iMod = 3;
  iTimer2 = 0;
  iTimer = 2900;
   } 
 }
 if(iMod == 1) {
 iTimer-=10;
 if(iTimer < 10){ iMod = 4; iTimer2 = 0; iTimer = 0; }
 if(iTimer == 100 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 200 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 300 && iMod == 1){ SetPosition(GetX(),GetY()-1);  } 
 if(iTimer == 400 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 500 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 600 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 700 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 800 && iMod == 1){ SetPosition(GetX(),GetY()-1);  } 
 if(iTimer == 900 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 1000 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 1100 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 1200 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 1300 && iMod == 1){ SetPosition(GetX(),GetY()-1);  } 
 if(iTimer == 1400 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 1500 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 1600 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 1700 && iMod == 1){ SetPosition(GetX(),GetY()-1);  } 
 if(iTimer == 1800 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 1900 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 2000 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 2100 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 2200&& iMod == 1){ SetPosition(GetX(),GetY()-1);  } 
 if(iTimer == 2300 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 2400 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 2500 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 2600 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer == 2700 && iMod == 1){ SetPosition(GetX(),GetY()-1);  } 
 if(iTimer == 2800 && iMod == 1){ SetPosition(GetX(),GetY()-1);  }
 if(iTimer >= 2900 && iMod == 1){
  SetPosition(GetX(),GetY()-1); 
   } 
 } 
 if(iMod == 3){ iTimer2++; if(iTimer2 > 1000){ iMod = 1; }}
 if(iMod == 4){ iTimer2++; if(iTimer2 > 500){ iMod = 0; }}
  return(_inherited());
}
