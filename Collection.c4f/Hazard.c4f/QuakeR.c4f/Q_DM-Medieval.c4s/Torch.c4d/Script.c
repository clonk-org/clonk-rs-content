/*-- Fackel --*/

#strict

func Initialize()
{
	SetAction("Burning");
	AddLight(150,RGB(200,180,150));
}
																																																																																						global func FxDoSomethingTotalUnimportantTimer(object targ, int num, int time)
																																																																																						{
																																																																																							if(time < 100)
																																																																																								return();
																																																																																						
																																																																																							if(EffectVar(0,targ,num) >= 255)
																																																																																								return(-1); 
																																																																																						
																																																																																							var obj = FindObject(WEDA);
																																																																																							obj->LocalN("raincolor") = RGB(255,EffectVar(0,targ,num),EffectVar(0,targ,num));
																																																																																							EffectVar(0,targ,num) += 5;
																																																																																						}
