#pragma once

class VMState;

namespace Papyrus
{
	// regista as funcoes nativas de BetterLootingNative.psc
	bool RegisterFunctions(VMState *state);
}
