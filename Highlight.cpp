#include <SKSE.h>
#include <SKSE/PluginAPI.h>
#include <SKSE/DebugLog.h>
#include <SKSE/GameRTTI.h>
#include <SKSE/GameForms.h>
#include <SKSE/GameObjects.h>
#include <SKSE/GameReferences.h>
#include <SKSE/PapyrusInvoke.h>
#include <SKSE/SKSEEvents.h>
#include <Skyrim/SkyrimVM.h>
#include <Skyrim/FileIO/TESDataHandler.h>
#include <Skyrim/FileIO/TES.h>
#include <algorithm>
#include <set>
#include <vector>

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

	std::set<UInt32>	g_searched;		// cadaveres ja revistados (so nesta sessao)
	UInt32				g_scanSlot = 0;

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

	bool TypeEnabled(FormType t)
	{
		if (t == FormType::Activator)			return Settings::bHLActivator;
		if (t == FormType::TalkingActivator)	return Settings::bHLTalkingActivator;
		if (t == FormType::Container)			return Settings::bHLContainer;
		if (t == FormType::Door)				return Settings::bHLDoor;
		if (t == FormType::Flora || t == FormType::Tree)	return Settings::bHLFlora;
		if (t == FormType::Furniture)			return Settings::bHLFurniture;
		if (t == FormType::Armor)				return Settings::bHLArmor;
		if (t == FormType::Weapon)				return Settings::bHLWeapon;
		if (t == FormType::Ammo)				return Settings::bHLAmmo;
		if (t == FormType::Book)				return Settings::bHLBook;
		if (t == FormType::Ingredient)			return Settings::bHLIngredient;
		if (t == FormType::Key)					return Settings::bHLKey;
		if (t == FormType::Misc)				return Settings::bHLMisc;
		if (t == FormType::Potion)				return Settings::bHLPotion;
		if (t == FormType::ScrollItem)			return Settings::bHLScroll;
		if (t == FormType::SoulGem)				return Settings::bHLSoulGem;
		return false;
	}

	// filtros comuns a mira e ao varrimento (objetos que nao sao atores)
	bool PassesObjectFilters(TESObjectREFR *ref, FormType type, bool &stealable)
	{
		stealable = false;

		if (ref->IsDeleted() || ref->IsDisabled())
			return false;

		if ((*(const UInt32 *)&ref->flags) & TESObjectREFR::kFlag_Harvested)
			return false;

		if (!TypeEnabled(type))
			return false;

		if (!ref->Is3DLoaded())
			return false;

		if (Settings::bHLIgnoreUnnamed)
		{
			const char *name = ref->GetReferenceName();
			if (!name || !name[0])
				return false;
		}

		// o que seria roubo fica a vermelho (portas nao)
		stealable = Settings::bHLRedStealable && type != FormType::Door && ref->IsOffLimits();
		return true;
	}

	int ClampColor(int color)
	{
		return (color < 0 || color >= kColors) ? 0 : color;
	}

	// aplica o brilho: slot 0-3 = mira, 4-31 = varrimento
	void ApplyShader(TESObjectREFR *ref, int color, UInt32 slot, float seconds)
	{
		TESEffectShader *shader = g_shaders[ClampColor(color) * kPerColor + slot];
		if (!shader)
			return;

		if (g_canTuneTime)
		{
			float fullAlpha = seconds - 0.4f;
			if (fullAlpha < 0.01f)
				fullAlpha = 0.01f;
			*(float *)(shader->unk14.pad + kOffFullAlphaTime) = fullAlpha;
		}

		CallPlay(shader, ref, seconds + 1.0f);
	}

	void OnCrosshair(TESObjectREFR *ref)
	{
		if (!g_ready || !Settings::bHLEnabled || !ref || ref == g_thePlayer || !ref->baseForm)
			return;

		const FormType type = ref->baseForm->formType;

		bool stealable = false;
		if (type == FormType::Character || !PassesObjectFilters(ref, type, stealable))
		{
			if (Settings::bHLDebug)
				_MESSAGE("Highlight: mira %08X tipo %d: ignorado", ref->formID, (int)type);
			return;
		}

		const int color = stealable ? 4 : Settings::iHLColor;
		const UInt32 slot = (g_slot++) % kCrosshairSlots;

		if (Settings::bHLDebug)
			_MESSAGE("Highlight: mira %08X tipo %d: cor %d, posicao %u", ref->formID, (int)type, color, slot);

		ApplyShader(ref, color, slot, Settings::iHLCrosshairMs * 0.001f);
	}

	// cadaver com loot: morto, com 3D, com itens e (opcionalmente) ainda nao revistado
	bool IsLootableCorpse(TESObjectREFR *ref)
	{
		Actor *actor = static_cast<Actor *>(ref);

		if (!actor->IsDead(true) || actor->IsGhost())
			return false;

		if (ref->IsDeleted() || ref->IsDisabled() || !ref->Is3DLoaded())
			return false;

		if (Settings::bHLCorpseHideSearched && g_searched.find(ref->formID) != g_searched.end())
			return false;

		// sem itens nao ha o que levar (nao cria o contentor, so conta)
		return ref->GetNumItems(false, false) > 0;
	}

	struct Candidate
	{
		TESObjectREFR	*ref;
		float			dist2;
		bool			corpse;
		bool			stealable;
	};

	void VisitCell(TESObjectCELL *cell, float px, float py, float pz, std::vector<Candidate> &out)
	{
		if (!cell)
			return;

		const float radius = (float)Settings::iHLScanRadius;
		const float corpseRadius = (float)Settings::iHLCorpseMaxDist;
		const float objectR2 = radius * radius;
		const float corpseR2 = corpseRadius * corpseRadius;

		for (TESObjectREFR *ref : cell->objectList)
		{
			if (!ref || ref == g_thePlayer || !ref->baseForm)
				continue;

			const float dx = ref->pos.x - px;
			const float dy = ref->pos.y - py;
			const float dz = ref->pos.z - pz;
			const float d2 = dx * dx + dy * dy + dz * dz;

			const FormType type = ref->baseForm->formType;
			if (type == FormType::Character)
			{
				if (!Settings::bHLCorpses || d2 > corpseR2 || !IsLootableCorpse(ref))
					continue;

				Candidate c = { ref, d2, true, false };
				out.push_back(c);
			}
			else
			{
				if (d2 > objectR2)
					continue;

				bool stealable = false;
				if (!PassesObjectFilters(ref, type, stealable))
					continue;

				Candidate c = { ref, d2, false, stealable };
				out.push_back(c);
			}
		}
	}

	bool CandidateCloser(const Candidate &a, const Candidate &b)
	{
		return a.dist2 < b.dist2;
	}

	// varre as celulas carregadas a volta do jogador e realca o que encontra
	void Scan()
	{
		if (!g_ready || !Settings::bHLEnabled || !Settings::bHLScanEnabled || !g_thePlayer || !g_thePlayer->parentCell)
			return;

		const float px = g_thePlayer->pos.x;
		const float py = g_thePlayer->pos.y;
		const float pz = g_thePlayer->pos.z;

		std::vector<Candidate> found;
		found.reserve(256);

		TESObjectCELL *playerCell = g_thePlayer->parentCell;
		if (playerCell->IsInterior() || !g_TES || !g_TES->gridCellArray)
		{
			VisitCell(playerCell, px, py, pz, found);
		}
		else
		{
			GridCellArray *grid = g_TES->gridCellArray;
			for (UInt32 y = 0; y < grid->size; ++y)
			{
				for (UInt32 x = 0; x < grid->size; ++x)
					VisitCell(grid->Get(x, y), px, py, pz, found);
			}
		}

		std::sort(found.begin(), found.end(), CandidateCloser);

		size_t limit = (size_t)Settings::iHLScanMax;
		if (found.size() < limit)
			limit = found.size();

		const float seconds = Settings::iHLScanMs * 0.001f;
		const UInt32 scanSlots = kPerColor - kCrosshairSlots;

		for (size_t i = 0; i < limit; ++i)
		{
			const Candidate &c = found[i];
			const int color = c.corpse ? Settings::iHLCorpseColor : (c.stealable ? 4 : Settings::iHLColor);
			const UInt32 slot = kCrosshairSlots + (g_scanSlot++ % scanSlots);
			ApplyShader(c.ref, color, slot, seconds);
		}

		if (Settings::bHLDebug)
			_MESSAGE("Highlight: varrimento: %u candidatos, %u realcados", (unsigned)found.size(), (unsigned)limit);
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
		if (form && form->formType == FormType::EffectShader)
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
	BSTEventSource<SKSECrosshairRefEvent> *source = mes ? (BSTEventSource<SKSECrosshairRefEvent> *)mes->GetEventDispatcher(SKSEMessagingInterface::kDispatcher_CrosshairEvent) : nullptr;
	if (!source)
	{
		_MESSAGE("ERRO: nao consegui registar o evento da mira; o realce fica desligado");
		return;
	}

	source->AddEventSink(&g_crosshairSink);
	g_ready = true;
	_MESSAGE("Highlight: pronto");
}


void Highlight::OnScanKey(bool a_down, bool a_up, float a_heldSeconds, bool a_lootMenuVisible)
{
	static bool s_active = false;
	static float s_nextAt = 0.0f;

	if (!g_ready || !Settings::bHLEnabled || !Settings::bHLScanEnabled)
	{
		s_active = false;
		return;
	}

	// com o menu de loot aberto o E pega itens: nao ha varrimento
	if (a_lootMenuVisible)
	{
		s_active = false;
		return;
	}

	if (a_down)
	{
		s_active = true;
		s_nextAt = Settings::iHLScanHoldMs * 0.001f;
	}

	if (a_up)
	{
		s_active = false;
		return;
	}

	if (!s_active || a_heldSeconds < s_nextAt)
		return;

	Scan();

	// mantido premido: refresca antes de o brilho acabar
	float interval = Settings::iHLScanMs * 0.001f - 0.8f;
	if (interval < 0.5f)
		interval = 0.5f;
	s_nextAt = a_heldSeconds + interval;
}


void Highlight::MarkSearched(TESObjectREFR *a_ref)
{
	if (a_ref && a_ref->baseForm && a_ref->baseForm->formType == FormType::Character)
		g_searched.insert(a_ref->formID);
}
