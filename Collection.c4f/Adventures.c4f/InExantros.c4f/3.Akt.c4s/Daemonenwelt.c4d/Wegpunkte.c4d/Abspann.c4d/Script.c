#strict

Initialize:
  SetComDir(COMD_Down());
  SetYDir(1);
  SetAction("Speak");
  return(1);

Text:
  if(GreaterThan(GetActTime(),5000))  RemoveObject();
  SetComDir(COMD_Down());
  Message("@Der Sieg ist Euer!",this());
  return(1);