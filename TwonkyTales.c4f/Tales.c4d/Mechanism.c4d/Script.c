#strict 2
local tt_key, tt_label;
protected func Initialize() { SetAction("Hold"); SetComDir(COMD_Stop); }
public func Setup(string key, string label) { tt_key=key; tt_label=label; SetName(label); }
public func Key() { return tt_key; }
public func Display(int value) { SetName(Format("%s [%d]", tt_label, value)); SetColorDw(RGB(180+value*15,180-value*25,110)); }
protected func ControlThrow(object actor) { [$Operate$] return GameCall("TTOperate", tt_key, actor); }
protected func ControlUp(object actor) { [$Inspect$] return GameCall("TTInspect", tt_key, actor); }
protected func ContextOperate(object actor) { [$Operate$] return ControlThrow(actor); }
protected func ContextInspect(object actor) { [$Inspect$] return ControlUp(actor); }

public func IsArrowTarget(object arrow) { return tt_key == "chain" && GameCall("TTChapter") == 1; }
