#strict

#appendto _BAE

func ResearchAllowed(level,owner)
{
  if(!g_bViewedPlans) return(0);
	return(_inherited(level,owner));
}
