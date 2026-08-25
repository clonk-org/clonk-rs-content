/*-- Wachturm --*/

#strict 2
#appendto WGTW

func ControlLeft()  { return Message("$MsgCantBeClosed$", this); }
func ControlRight() { return Message("$MsgCantBeClosed$", this); }
