#strict

Initialize:
  SetComDir(COMD_Down());
  SetYDir(1);
  SetAction("Speak");
  return(1);

Text:
  if(GreaterThan(GetActTime(),5000))  RemoveObject();
  SetComDir(COMD_Down());
  Message("@Als ihr zur Wichtelburg zurück gekehrt seid, erwartete euch ein Fest, welches wohl nie in Exantros vergessen wird. Euer Name ist nun eine Legende.",this());
  return(1);