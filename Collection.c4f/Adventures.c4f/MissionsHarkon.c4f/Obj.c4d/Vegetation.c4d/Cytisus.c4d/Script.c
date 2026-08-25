/*-- Grünzeug --*/

// #include TREE - nope

#strict 2

protected func Initialize()
  {
  SetAction(Format("Green%d", Random(2)));
  //SetR(Random(80)-40);
  return 1;
  }