#pragma once

class Actor;
class TESForm;

// Equipar/usar um item do inventario chamando as funcoes nativas do Papyrus diretamente
// (sem scripts): Actor.EquipItem e, para escolher a mao, Actor.EquipItemEx (SKSE).
namespace Equip
{
	// slotId: 0 = por omissao; outros valores = mao (ver iEquipSlotIdRight/iEquipSlotIdLeft)
	bool EquipItem(Actor *a_actor, TESForm *a_form, int a_slotId);
}
