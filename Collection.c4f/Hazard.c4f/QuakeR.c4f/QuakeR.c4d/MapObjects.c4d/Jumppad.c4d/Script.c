#strict 2

local lxdir, lydir;

func Initialize() { SetAction("Blink"); } //Actions doof.

func Set (int nxdir, int nydir) { lxdir=nxdir; lydir=nydir; }

func Timer()
{
	var w = GetObjWidth(),
	    h = GetObjHeight();
	var x = w/2,
	    y = h/2;

  for(var pClonk in FindObjects(Find_InRect(-x, -y, w, h), Find_Or(Find_OCF(OCF_Alive),Find_Func("IsBouncy")), Find_NoContainer()))
  {
  	pClonk->SetSpeed(lxdir,lydir);
    if(!pClonk->SetAction("JumpedArmed"))
    	pClonk->SetAction("JumpArmed");
    Sound("QJumppad*");
  }
}
  

func IsJumppad() { return true; }
