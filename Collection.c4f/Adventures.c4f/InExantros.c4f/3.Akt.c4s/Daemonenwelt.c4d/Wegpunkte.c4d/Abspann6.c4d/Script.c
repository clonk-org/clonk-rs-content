#strict

Initialize:
  SetComDir(COMD_Down());
  SetYDir(1);
  SetAction("Speak");
  return(1);

Text:
  if(GreaterThan(GetActTime(),5000))  RemoveObject();
  SetComDir(COMD_Down());
  Message("Danke an Randrian(Richard Gerum) dafür, dass ich sein Kampfsystempack als Vorlage benutzen durfte, und danke an Sven2 für die schöne FolderMap!",this());
  return(1);