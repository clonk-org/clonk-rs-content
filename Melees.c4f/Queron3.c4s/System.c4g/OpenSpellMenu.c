/* OpenSpellMenu: Stabzauber auch anzeigen */

/* Zaubermenü erzeugen und mit verfügbaren Zaubern eines Spielers füllen */

#strict 2

global func OpenSpellMenu(object clonk, object command_target, string itemtext, string cbfunc, int player, object magic_source, string emptytext, id select_spell)
  {
  OpenEmptySpellMenu(MCMS, clonk, command_target, magic_source, emptytext);
  var idMagic, i = 0;
  while (idMagic = GetPlrMagic(player, 0, i++)) 
    {
    AddMenuItem(itemtext, cbfunc, idMagic, clonk,0,clonk);
    if(select_spell == idMagic) SelectMenuItem(i-1);
    }
  // Stabsprüche
  var pStaff = clonk->~HasStaff(), j=0;
  if(pStaff)
		while (idMagic = pStaff->~GetSpell(j++))
			{
		  AddMenuItem(itemtext, cbfunc, idMagic, clonk,0,clonk);
		  if(select_spell == idMagic) SelectMenuItem(i-1);
			}
  return true;
  }
