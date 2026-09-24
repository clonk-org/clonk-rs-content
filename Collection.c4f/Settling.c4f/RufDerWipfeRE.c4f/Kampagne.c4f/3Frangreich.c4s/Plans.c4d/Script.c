#strict

static g_bViewedPlans;

public func UnfoldID() { return(EPPU); }

public func CalcValue() { return(20); }

public func Activate(pByObject)
{
  [$TxtViewplans$]
  // Ansichtsobjekt erzeugen
  Sound("PlanUnfold");
  var pUnfold = CreateObject(UnfoldID(), 0, -10, GetOwner(pByObject));
  pUnfold->Activate();
  SetPlrView(GetOwner(pByObject), pUnfold);
  if(!g_bViewedPlans)
    Message("$MsgPlans$",pByObject);
  // Pläne setzen
  g_bViewedPlans=true;
  return(1);
}
