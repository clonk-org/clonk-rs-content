/*-- Zappers CastlePanic hack! --*/

#strict 2

global func CastlePanic2()
  {
   for(var bld=0;bld=FindObject(CPT2,0,0,0,0,0,0,0,0,bld);) ResortTower(bld);
   
   for(var bld=0;bld=FindObject(CPTL,0,0,0,0,0,0,0,0,bld);) ResortTower(bld);
  }
  
  
  global func ResortTower(bld)
  {
   var wand=FindObject(CPW3,GetX(bld)-GetX(),GetY(bld)-GetY());
   SetObjectOrder(bld,wand,1);
   SetObjectOrder(wand,bld,0);
  }
  
global func CastlePanic()
{

 
 
 for(var obj in FindObjects(Find_Category(1)))
 {
  if(! obj ->~ CastlePartWidth()) continue;
  
   for(var obj2 in FindObjects(Find_Category(1)))
     {
      if(! obj2 ->~ CastlePartWidth()) continue;
      if(obj == obj2)continue;
      
      if(GetDefBottom(obj) < GetDefBottom(obj2))
      SetObjectOrder(obj,obj2,1);
      else
      SetObjectOrder(obj2,obj,1);
     }
  
  
 }
 
  CastlePanic2();
 
 //return _inherited();
}