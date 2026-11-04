#include <SKSE.h>
#include <SKSE/PluginAPI.h>
#include <SKSE/DebugLog.h>
#include <SKSE/GameRTTI.h>
#include <SKSE/GameForms.h>
#include <SKSE/GameObjects.h>
#include <SKSE/GameReferences.h>
#include <SKSE/PapyrusInvoke.h>
#include <Skyrim/SkyrimVM.h>

#include "Highlight.h"
#include "Settings.h"

namespace
{
	// Pool de shaders no BetterLooting.esp (ver tools/make_esp.py):
	// forma = 0x801 + cor * 32 + posicao ; posicoes 0-3 = mira (rodam), 4-31 = varrimento (v0.6)
	const int kColors = 5;
	const int kPerColor = 32;
	const int kCrosshairSlots = 4;
	const UInt32 kFirstLocalID = 0x801;
	const int kOffFullAlphaTime = 0x40;		// edgeEffectFullAlphaTime dentro dos dados do EFSH
	const int kOffEdgeColor = 0x38;			// edgeEffectColor (RGBA)
	const float kCrosshairFullAlpha = 0.4f;

	const UInt8 kPalette[kColors][3] = {
		{ 255, 215, 0 },	// dourado
		{ 255, 255, 255 },	// branco
		{ 0, 255, 102 },	// verde
		{ 0, 200, 255 },	// ciano
		{ 255, 40, 40 }		// vermelho
	};

	TESEffectShader	*g_shaders[kColors * kPerColor] = {};
	bool			g_ready = false;
	bool			g_canTuneTime = false;
	UInt32			g_slot = 0;
	bool			g_playFailed = false;

	typedef _NativeFunction<TESEffectShader, void, TESObjectREFR *, float> PlayNative;

	// EffectShader.Play(ObjectReference akObject, float afDuration) - funcao nativa do jogo
	bool CallPlay(TESEffectShader *shader, TESObjectREFR *ref, float duration)
	{
		if (g_playFailed || !g_skyrimVM)
			return false;

		VMState *state = g_skyrimVM->GetState();
		if (!state)
			return false;

		PlayNative *play = (PlayNative *)SKSEScript::GetFunction(state, TESEffectShader::kTypeID, "Play");
		if (!play || !play->Validate(state))
		{
			_MESSAGE("ERRO: nao encontrei a funcao nativa EffectShader.Play; o realce fica desligado");
			g_playFailed = true;
			return false;
		}

		(*play)(state, 0, shader, ref, duration);
		return true;
	}

	bool TypeEnabled(UInt32 t)
	{
		if (t == (UInt32)FormType::Activator)			return Settings::bHLActivator;
		if (t == (UInt32)FormType::TalkingActivator)	return Settings::bHLTalkingActivator;
		if (t == (UInt32)FormType::Container)			return Settings::bHLContainer;
		if (t == (UInt32)FormType::Door)				return Settings::bHLDoor;
		if (t == (UInt32)FormType::Flora || t == (UInt32)FormType::Tree)	return Settings::bHLFlora;
		if (t == (UInt32)FormType::Furniture)			return Settings::bHLFurniture;
		if (t == (UInt32)FormType::Armor)				return Settings::bHLArmor;
		if (t == (UInt32)FormType::Weapon)				return Settings::bHLWeapon;
		if (t == (UInt32)FormType::Ammo)				return Settings::bHLAmmo;
		if (t == (UInt32)FormType::Book)				return Settings::bHLBook;
		if (t == (UInt32)FormType::Ingredient)			return Settings::bHLIngredient;
		if (t == (UInt32)FormType::Key)					return Settings::bHLKey;
		if (t == (UInt32)FormType::Misc)				return Settings::bHLMisc;
		if (t == (UInt32)FormType::Potion)				return Settings::bHLPotion;
		if (t == (UInt32)FormType::ScrollItem)			return Settings::bHLScroll;
		if (t == (UInt32)FormType::SoulGem)				return Settings::bHLSoulGem;
		return false;
	}

	void OnCrosshair(TESObjectREFR *ref)
	{
		if (!g_ready || !Settings::bHLEnabled || !ref || ref == g_thePlayer || !ref->baseForm)
			return;

		if (ref->IsDeleted() || ref->IsDisabled())
			return;

		if (ref->flags & TESObjectREFR::kFlag_Harvested)
			return;

		const UInt32 type = ref->baseForm->formType;
		if (type == (UInt32)FormType::Character || !TypeEnabled(type))
			return;

		if (!ref->Is3DLoaded())
			return;

		if (Settings::bHLIgnoreUnnamed)
		{
			const char *name = ref->GetReferenceName();
			if (!name || !name[0])
				return;
		}

		// o que seria roubo fica a vermelho (portas nao)
		const bool stealable = Settings::bHLRedStealable && type != (UInt32)FormType::Door && ref->IsOffLimits();

		int color = stealable ? 4 : Settings::iHLColor;
		if (color < 0 || color >= kColors)
			color = 0;

		const UInt32 slot = (g_slot++) % kCrosshairSlots;
		TESEffectShader *shader = g_shaders[color * kPerColor + slot];
		if (!shader)
			return;

		const float duration = Settings::iHLCrosshairMs * 0.001f;
		if (g_canTuneTime)
		{
			float fullAlpha = duration - 0.4f;
			if (fullAlpha < 0.01f)
				fullAlpha = 0.01f;
			*(float *)(shader->unk14.pad + kOffFullAlphaTime) = fullAlpha;
		}

		CallPlay(shader, ref, duration + 1.0f);
	}

	class CrosshairSink : public BSTEventSink<SKSECrosshairRefEvent>
	{
	public:
		EventResult ReceiveEvent(SKSECrosshairRefEvent *evn, BSTEventSource<SKSECrosshairRefEvent> *src) override
		{
			if (evn && evn->crosshairRef)
				OnCrosshair(evn->crosshairRef);
			return kEvent_Continue;
		}
	};

	CrosshairSink g_crosshairSink;
}


void Highlight::Init()
{
	TESDataHandler *dataHandler = TESDataHandler::GetSingleton();
	const UInt8 modIndex = dataHandler ? dataHandler->GetModIndex("BetterLooting.esp") : 0xFF;
	if (modIndex == 0xFF)
	{
		_MESSAGE("Highlight: BetterLooting.esp nao esta ativo; o realce fica desligado");
		return;
	}

	int found = 0;
	for (int i = 0; i < kColors * kPerColor; ++i)
	{
		TESForm *form = LookupFormByID(((UInt32)modIndex << 24) | (kFirstLocalID + (UInt32)i));
		if (form && form->formType == (UInt32)FormType::EffectShader)
		{
			g_shaders[i] = static_cast<TESEffectShader *>(form);
			++found;
		}
	}
	_MESSAGE("Highlight: %d de %d shaders encontrados (indice do ESP %02X)", found, kColors * kPerColor, modIndex);
	if (found == 0)
		return;

	// verificacao: os dados do shader em memoria batem certo com o que o ESP gravou?
	TESEffectShader *probe = g_shaders[0];
	if (probe)
	{
		const UInt8 *d = probe->unk14.pad;
		const float fullTime = *(const float *)(d + kOffFullAlphaTime);
		const bool timeOk = (fullTime > kCrosshairFullAlpha - 0.01f) && (fullTime < kCrosshairFullAlpha + 0.01f);
		const bool colorOk = d[kOffEdgeColor] == kPalette[0][0] && d[kOffEdgeColor + 1] == kPalette[0][1] && d[kOffEdgeColor + 2] == kPalette[0][2];
		_MESSAGE("Highlight: verificacao dos dados do shader: tempo %s, cor %s", timeOk ? "OK" : "DIFERENTE", colorOk ? "OK" : "DIFERENTE");
		g_canTuneTime = timeOk;
	}

	const SKSEPlugin *plugin = SKSEPlugin::GetSingleton();
	const SKSEMessagingInterface *mes = plugin ? plugin->GetInterface(SKSEMessagingInterface::Version_1) : nullptr;
	BSTEventSource<SKSECrosshairRefEvent> *source = mes ? mes->GetEventDispatcher<SKSECrosshairRefEvent>() : nullptr;
	if (!source)
	{
		_MESSAGE("ERRO: nao consegui registar o evento da mira; o realce fica desligado");
		return;
	}

	source->AddEventSink(&g_crosshairSink);
	g_ready = true;
	_MESSAGE("Highlight: pronto");
}
