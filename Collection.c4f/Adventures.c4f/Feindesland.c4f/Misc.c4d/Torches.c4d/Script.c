#strict

#appendto CLNK

protected func Initialize() {
	if(GetID()!=TS9Z) return(SetPlrViewRange(0),_inherited());
	var id;
	Local()=1;
	if (id=FindObject(TS9Z))
	  { Local()+=Local(0,id); RemoveObject(id); }
	SetAction("Wait");}

protected func Initialized() {SetGamma(0,RGB(128-Local()*16,128-Local()*16,128-Local()*16),RGB(255-Local()*30,255-Local()*30,255-Local()*30)); FogofWar();}

//FoW aufrechterhalten bei abgespeicherten Spielen
public func SetTransferZone() {Log("Set"); SetFoW(1,GetOwner());}
public func UpdateTransferZone() {Log("Update"); SetFoW(1,GetOwner());}

/*Alles zusammen nochmal. Und zwar weil beim initialisieren der Startclonks die SetPlrViewRange-Funzion nicht richtig funzt, da zu der Zeit noch garnicht die ganzen TS9Z-Objekte erstellt wurden. Auﬂerdem auch weil man ja die Maussteuerung w‰hrend des Spiels ausschalten kann (FoW wird dabei auch ausgestellt)*/
public func FogofWar() {
for(var i1=0; i1<GetPlayerCount(); i1++) {
	SetFoW(1,i1);
	for(var i2=0; i2<GetCrewCount(i1); i2++) {
		SetPlrViewRange(211-Local()*23-Local()*12*( GetMaterial(GetX(GetCrew(i1,i2))-GetX(),GetY(GetCrew(i1,i2))-GetY())!=-1 || IsNight()),GetCrew(i1,i2));
}}}