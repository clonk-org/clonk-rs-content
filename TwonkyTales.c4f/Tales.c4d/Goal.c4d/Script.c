#strict 2
#include GOAL
public func IsFulfilled() { return GameCall("TTReadyToEnd"); }
protected func Activate(int player) { if (GameCall("TTFailed")) MessageWindow(GameCall("TTFailureText"), player, TTGL); else if (GameCall("TTComplete")) MessageWindow(GameCall("TTEnding"), player, TTGL); else MessageWindow(GameCall("TTJournal"), player, TTGL); }
