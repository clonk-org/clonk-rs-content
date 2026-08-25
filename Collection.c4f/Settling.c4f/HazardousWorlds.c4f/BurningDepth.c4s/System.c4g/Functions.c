/* Functions */

#strict 2

global func Object2Overlay(object target, object obj)
{
  // Safety/Local call
  if (!target) return 0;
  if (!obj) if (!(obj = this)) return 0;
  // Create new overlay using object parameters
  var overlay_mode = GFXOV_MODE_Base;
  var overlay_action = 0;
  var overlay_id = GetUnusedOverlayID(GFX_Overlay, target);
  if (!ActIdle(obj)) { overlay_mode = GFXOV_MODE_Action; overlay_action = GetAction(obj); }
  if (!SetGraphics(0, target, GetID(obj), overlay_id, overlay_mode, overlay_action, GetObjectBlitMode(obj), 0)) return 0;
  if (GetClrModulation(obj)) SetClrModulation(GetClrModulation(obj), target, overlay_id);
  SetObjDrawTransform(1000,0,(GetX(obj)-GetX(target))*1000, 0,1000,(GetY(obj)-GetY(target))*1000, target, overlay_id);
  // Remove old object
  RemoveObject(obj);
  // Return id of newly created overlay
  return overlay_id;
}

global func AddKnowledge(id def)
{
  var player = 0;
  var knowledge = [];
  for(var entry in knowledge)
    SetPlrKnowledge(player, entry);
  knowledge[GetLength(knowledge)] = def;
  for(var i = 0; i < GetPlayerCount(); ++ i)
    SetPlrKnowledge(GetPlayerByIndex(i), def);
}

global func AddHomebaseMaterial(id material, int amount)
{
  var player = 0;
  var homebase_material = [];
  for(var entry in homebase_material)
    DoHomebaseMaterial(player, entry[0], entry[1]);  
  homebase_material[GetLength(homebase_material)] = [material, amount];
  for(var i = 0; i < GetPlayerCount(); ++ i)
    DoHomebaseMaterial(GetPlayerByIndex(i), material, amount);
  
}
