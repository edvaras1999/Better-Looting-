#include <SKSE.h>
#include <SKSE/PluginAPI.h>
#include <SKSE/DebugLog.h>
#include <SKSE/GameForms.h>
#include <SKSE/GameReferences.h>
#include <SKSE/PapyrusInvoke.h>
#include <Skyrim/SkyrimVM.h>

#include "Equip.h"

namespace
{
	// Actor.EquipItem(Form akItem, bool abPreventRemoval, bool abSilent)
	typedef _NativeFunction<Actor, void, TESForm *, bool, bool> EquipNative;
	// Actor.EquipItemEx(Form item, int equipSlot, bool preventUnequip, bool equipSound)  (SKSE)
	typedef _NativeFunction<Actor, void, TESForm *, SInt32, bool, bool> EquipExNative;

	bool g_equipMissing = false;
	bool g_equipExMissing = false;
}


bool Equip::EquipItem(Actor *a_actor, TESForm *a_form, int a_slotId)
{
	if (!a_actor || !a_form || !g_skyrimVM)
		return false;

	VMState *state = g_skyrimVM->GetState();
	if (!state)
		return false;

	// escolher a mao: so com EquipItemEx
	if (a_slotId != 0 && !g_equipExMissing)
	{
		EquipExNative *equipEx = (EquipExNative *)SKSEScript::GetFunction(state, Actor::kTypeID, "EquipItemEx");
		if (equipEx && equipEx->Validate(state))
		{
			(*equipEx)(state, 0, a_actor, a_form, a_slotId, false, true);
			return true;
		}

		_MESSAGE("AVISO: Actor.EquipItemEx nao esta disponivel; o item e equipado na mao por omissao");
		g_equipExMissing = true;
	}

	if (g_equipMissing)
		return false;

	EquipNative *equip = (EquipNative *)SKSEScript::GetFunction(state, Actor::kTypeID, "EquipItem");
	if (!equip || !equip->Validate(state))
	{
		_MESSAGE("ERRO: nao encontrei a funcao nativa Actor.EquipItem; Usar/equipar fica desligado");
		g_equipMissing = true;
		return false;
	}

	(*equip)(state, 0, a_actor, a_form, false, false);
	return true;
}
