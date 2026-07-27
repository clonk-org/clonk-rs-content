/*-- Toter Greif --*/

#strict 2

/* Initialisierung */

protected func Initialize()
{
  SetAction("Dead");
}

/* Verwesung */

protected func Decaying()
{
  DoCon(-2);
}

private func ResetVertices()
{
  // Alle Vertices auf DefCore-Position zurücksetzen
  var vertexCount = GetDefCoreVal("Vertices", "DefCore", GetID());
  for (var i = 0; i < vertexCount; i++) 
  {
    var vertexX = GetDefCoreVal("VertexX", "DefCore", GetID(), i);
    var vertexY = GetDefCoreVal("VertexY", "DefCore", GetID(), i);
    SetVertex(i, 0, vertexX, this, 2);
    SetVertex(i, 1, vertexY, this, 2);
  }    
}
