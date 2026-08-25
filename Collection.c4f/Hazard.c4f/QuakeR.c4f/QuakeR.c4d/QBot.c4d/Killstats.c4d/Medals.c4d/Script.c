/* Medal */

static const QMDL_Rowlength=8;
local shown;

#strict

protected func Initialize()
{
	SetVisibility(VIS_None); // Ich bin ein Hud \o/, aber erst mal unsichtbar
}

//Nur noch tolles Ausrichten und aktivieren!
public func Set(int iPos, bool fActive)
{
	var iRow=iPos/QMDL_Rowlength;
	var iColumm=iPos%QMDL_Rowlength;
	SetPosition(100+60*iColumm,100+60*iRow);
	if (fActive) SetAction("Be");
}

public func IsMedal(id type, bool fonlyshown) {
	if(fonlyshown)
		if(!shown) return false;
	if (!type) return true;
	else return type == GetID();	}

public func Activate() {return SetAction("Be");}
//Name doof, ich weiß.
public func Deactivate() { return SetAction("Idle"); }

//Will ja vllt wer mal was anders machen.
public func IsActiveMedal() { return GetAction() eq "Be";}

//Alle Medals meines Types, die gezeigt wurden oder alle anzeigen oder verbergen.
public func SetMedalsVisibility(bool fVis, bool fAll)
{
 var dwVis;
 var idMedal;
 if (fVis) dwVis=VIS_Owner; else dwVis=VIS_None;
 if (!fAll) idMedal=GetID();
 for (var pMedal in FindObjects(Find_Owner(GetOwner()),Find_Func("IsMedal",idMedal, true)))
  pMedal->SetVisibility(dwVis);
}

global func EvalMedals(int iPlr, id idMedal)
{
 var aMedals=FindObjects(Find_ID(idMedal),Find_Owner(iPlr));
 AddEvaluationData(Format("{{%i}}: %d", idMedal, GetLength(aMedals)), GetPlayerID(iPlr));
 for (var pMedal in aMedals) RemoveObject(pMedal);
}

global func MedalCount(object pMedal)
{
	if(!pMedal) pMedal=this(); if(!pMedal) return false;
	return ObjectCount2(Find_ID(GetID(pMedal)),Find_Owner(GetOwner(pMedal)));
}

//Actioncalls
private func Show() {shown=true; SetMedalsVisibility(false, true); SetMedalsVisibility(true); }
private func Hide() { SetMedalsVisibility(false); }

//Medaleffekt, der sich um den HUD kümmert
func FxMedalHUDStart(object pTarget, int iEffectNumber, int iTemp, int iPlr, id idMedal)
{
	if(iTemp) return(FX_OK);
	//Welcher Spieler?
	EffectVar(0, pTarget, iEffectNumber) = iPlr;
	//Medals, die gerne noch gezeigt werden wollen.
	EffectVar(1, pTarget, iEffectNumber) = CreateArray();
	var iMedalCount = ObjectCount2(Find_Owner(iPlr),Find_Func("IsMedal",idMedal));
	var pMedal = CreateObject(idMedal,0,0,iPlr);
	pMedal->Set(iMedalCount);
	EffectVar(1, pTarget, iEffectNumber)[0]	= pMedal;
	pMedal->Activate();
}

func FxMedalHUDAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, int iPlr, id idMedal)
{
	var iMedalCount = ObjectCount2(Find_Owner(iPlr),Find_Func("IsMedal",idMedal));
	var pMedal = CreateObject(idMedal,0,0,iPlr);
	pMedal->Set(iMedalCount);
	EffectVar(1, pTarget, iEffectNumber)[GetLength(EffectVar(1, pTarget, iEffectNumber))]	= pMedal;
}

func FxMedalHUDTimer(object pTarget, int iEffectNumber)
{
	DeleteArrayFirst(EffectVar(1, pTarget, iEffectNumber));
	if(!GetLength(EffectVar(1, pTarget, iEffectNumber))) return -1;
	EffectVar(1, pTarget, iEffectNumber)[0]->Activate();
	}

//Noch eine andere Medal? Kommt in die Warteschlange
func FxMedalHUDEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber, iPlr) {
  if(szNewEffectName S= "MedalHUD")
		if(iPlr==EffectVar(0, pTarget, iEffectNumber))
    	return(-2);
}

func FxMedalMessageStart(object pTarget, int iEffectNumber, int iTemp, id idMedal)
{
	if(iTemp) return FX_OK;
	//Die Nachricht
	EffectVar(0, pTarget, iEffectNumber) = CreateArray(0);
	//Relativer Startzeitpunkt
	EffectVar(1, pTarget, iEffectNumber) = CreateArray(0);
	EffectVar(0, pTarget, iEffectNumber)[0]= Format("{{%i}}|<c ff0000><i>%s!</i></c>",idMedal,GetName(0,idMedal));
	EffectVar(1, pTarget, iEffectNumber)[0] = 0;
}

func FxMedalMessageAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, id idMedal)
{
	var iPos=GetLength(EffectVar(0, pTarget, iEffectNumber));
	EffectVar(0, pTarget, iEffectNumber)[iPos]= Format("{{%i}}|<c ff0000><i>%s!</i></c>",idMedal,GetName(0,idMedal));
	EffectVar(1, pTarget, iEffectNumber)[iPos]=GetEffect("MedalMessage", pTarget, 0, 6);
}

func FxMedalMessageTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	//Erstmal alte aussortieren
	while(iEffectTime - EffectVar(1, pTarget, iEffectNumber)[0] > 72)
	{
		DeleteArrayFirst(EffectVar(0, pTarget, iEffectNumber));
		DeleteArrayFirst(EffectVar(1, pTarget, iEffectNumber));
		if(!GetLength(EffectVar(1, pTarget, iEffectNumber))) return -1;
	}
	//Message basteln
	var szMessage="";
	for (var szMedals in EffectVar(0, pTarget, iEffectNumber)) szMessage=Format("%s||%s",szMessage,szMedals);
	Message(szMessage,pTarget);
}

//Noch eine andere Medal? Kommt unten hin.
func FxMedalMessageEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "MedalMessage")
    	return(-3);
}

func FxMedalMessageStop(object pTarget, int iEffectNumber) { Message("",pTarget); }

global func DeleteArrayFirst(&array)
{
	if(!GetLength(array)) return array;
	var newarray=CreateArray(GetLength(array)-1);
	for (var i=0;i<GetLength(newarray);i++) newarray[i]=array[i+1];
	return array=newarray;
}
