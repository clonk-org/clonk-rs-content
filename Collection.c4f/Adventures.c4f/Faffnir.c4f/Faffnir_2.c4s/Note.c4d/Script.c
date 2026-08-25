/*-- Zettel --*/

#strict

local Text;

protected func SetCode(iText) {
	//if(GetType(iText) != C4V_Int || GetType(iText) != C4V_String) return(false);
	Text = iText;
        return 1;
}

protected func Activate(object pByObj) {
	if(GetType(Text) == C4V_Int)    MessageWindow(Format("%d", Text), GetOwner(pByObj), NOTE);
	if(GetType(Text) == C4V_String) MessageWindow(Text, GetOwner(pByObj), NOTE);
}