/*-- Gleichgewicht --*/

#strict
#appendto KNIG

local pExp;
local pLevel;

func ExpBalance()	{

  var i=-1, iPlr;
  while (++i < GetPlayerCount())
  {
  iPlr=GetPlayerByIndex(i);

		if(SetVar(0,FindObjectOwner(KNIG,iPlr)) ) //Den Ritter einspeichern
			if(VisuellesLevel( LocalN("pLevel",Var(0)) )+LocalN("pExp",Var(0))<VisuellesLevel(pLevel)+pExp-50) //Erfahrung vergleichen (balanciert bis auf 50 pExp aus)
				if(pExp>0)  { --pExp; ++LocalN("pExp",Var(0)); } //Ein pExp austauschen (wenn er überhaupt kann)

  }
  return(1);
			}

func VisuellesLevel()	{ //Sein gesamte bisher verdiente Erfahrung ausrechnen.
  var iEx=50;
  var i=1;
  if(Par(0)<2)  return(0);
  if(Par(0)==2)  return(50);
  while(i<Par(0)-1)
	{
	++i;
	SetVar(0,i*i);
	SetVar(1,i+57);
	SetVar(2,Var(1)*Var(0));
	SetVar(3,Var(2)/3);
	iEx=iEx+Var(3);
	}
  return(iEx);
			}