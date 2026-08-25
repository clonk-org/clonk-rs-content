/*-- Grünzeug --*/

// #include TREE - nope

#strict 2

protected func Initialize()
  {
  SetAction(Format("Trunk%d", Random(4)));
	SetDir(Random(2));
  SetR(Random(80)-40);
  return 1;
  }

