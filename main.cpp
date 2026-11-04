#include <SKSE.h>
#include <SKSE/PluginAPI.h>
#include <SKSE/DebugLog.h>
#include <SKSE/GameRTTI.h>
#include "LootMenu.h"
#include "Settings.h"
#include "Console.h"
#include "Hooks.h"
#include "Papyrus.h"

//========================================================
// skse plugin
//========================================================

class QuickLootPlugin : public SKSEPlugin
{
public:
	QuickLootPlugin()
	{
	}

	virtual bool InitInstance() override
	{
		if (!Requires(kSKSEVersion_1_7_1, SKSETaskInterface::Version_2, SKSEPapyrusInterface::Version_1))
		{
			gLog << "ERROR: your skse version is too old." << std::endl;
			return false;
		}

		SetName("BetterLooting");
		SetVersion(3);

		return true;
	}

	virtual bool OnLoad() override
	{
		SKSEPlugin::OnLoad();

		// funcoes nativas para o MCM
		const SKSEPapyrusInterface *papyrus = GetInterface(SKSEPapyrusInterface::Version_1);
		if (papyrus)
			papyrus->Register(&Papyrus::RegisterFunctions);

		_MESSAGE("");

		return true;
	}

	virtual void OnModLoaded() override
	{
		Settings::Load();

		if (LootMenu::Install())
		{
			Hooks::Install();
			if (Settings::bUseConsole)
				ConsoleCommand::Register();
		}
	}
} thePlugin;
