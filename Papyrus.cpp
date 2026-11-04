#include <SKSE.h>
#include <SKSE/DebugLog.h>
#include <SKSE/PapyrusFunctions.h>

#include "Papyrus.h"
#include "Settings.h"
#include "LootMenu.h"

//========================================================
// funcoes nativas usadas pelo MCM (BetterLootingMCM.psc)
//========================================================
namespace
{
	// valor atual de uma definicao (ou o valor dado se a chave nao existir)
	SInt32 N_GetInt(BSFixedString key, SInt32 defaultValue)
	{
		int v = 0;
		if (Settings::Get(key.c_str(), v))
			return v;
		return defaultValue;
	}

	// valor por omissao de uma definicao
	SInt32 N_GetDefault(BSFixedString key, SInt32 fallback)
	{
		int v = 0;
		if (Settings::GetDefault(key.c_str(), v))
			return v;
		return fallback;
	}

	// altera uma definicao e grava-a no BetterLooting.ini
	void N_SetInt(BSFixedString key, SInt32 value)
	{
		if (!Settings::SetAndSave(key.c_str(), value))
			_MESSAGE("papyrus: SetInt com chave desconhecida: %s", key.c_str());
	}

	// o MCM mudou o visual: o menu de loot e recriado na proxima abertura
	void N_ApplyVisual()
	{
		LootMenu::RequestVisualReload();
	}

	void N_Log(BSFixedString msg)
	{
		_MESSAGE("papyrus: %s", msg.c_str());
	}
}


bool Papyrus::RegisterFunctions(VMState *state)
{
	SKSEScript::RegisterFunction("BetterLootingNative", "GetInt", &N_GetInt, state);
	SKSEScript::RegisterFunction("BetterLootingNative", "GetDefault", &N_GetDefault, state);
	SKSEScript::RegisterFunction("BetterLootingNative", "SetInt", &N_SetInt, state);
	SKSEScript::RegisterFunction("BetterLootingNative", "ApplyVisual", &N_ApplyVisual, state);
	SKSEScript::RegisterFunction("BetterLootingNative", "Log", &N_Log, state);

	_MESSAGE("Funcoes Papyrus registadas (BetterLootingNative)");
	return true;
}
