#include <SKSE.h>
#include <SKSE/PluginAPI.h>
#include <SKSE/DebugLog.h>
#include <SKSE/GameRTTI.h>
#include <SKSE/GameForms.h>
#include <SKSE/GameObjects.h>
#include <SKSE/GameReferences.h>
#include <SKSE/GameExtraData.h>
#include <SKSE/PapyrusInvoke.h>
#include <SKSE/SKSEEvents.h>
#include <Skyrim/SkyrimVM.h>
#include <Skyrim/FileIO/TESDataHandler.h>
#include <Skyrim/FileIO/TES.h>
#include <Windows.h>
#include <algorithm>
#include <cstdio>
#include <map>
#include <set>
#include <vector>

#include "Highlight.h"
#include "Settings.h"

namespace
{
	// Shaders no BetterLooting.esp (ver tools/make_esp.py):
	// forma = 0x801 + (nivel * 5 + cor) * 12 + posicao ; posicoes 0-3 = mira (rodam), 4-11 = varrimento
	// O nivel (0 = mais forte ... 4 = mais fraco) esta gravado em cada shader.
	const int kLevels = 5;
	const int kColors = 5;
	const int kSlots = 12;
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

	TESEffectShader	*g_shaders[kLevels * kColors * kSlots] = {};
	bool			g_ready = false;
	bool			g_canTune = false;		// os deslocamentos dos dados do shader foram confirmados
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

	// nivel de intensidade (0 = mais forte) a partir da definicao 0-100
	int LevelFromPercent(int percent)
	{
		if (percent >= 85) return 0;
		if (percent >= 58) return 1;
		if (percent >= 35) return 2;
		if (percent >= 17) return 3;
		return 4;
	}

	// ajusta o tempo com alfa total antes de aplicar o shader
	void TuneShader(TESEffectShader *shader, float seconds)
	{
		float fullAlpha = seconds - 0.4f;
		if (fullAlpha < 0.01f)
			fullAlpha = 0.01f;
		*(float *)(shader->unk14.pad + kOffFullAlphaTime) = fullAlpha;
	}

	// aplica o brilho: slot 0-3 = mira, 4-11 = varrimento
	void ApplyShader(TESObjectREFR *ref, int color, UInt32 slot, float seconds)
	{
		if (Settings::iHLFillPercent <= 0)
			return;

		const int level = LevelFromPercent(Settings::iHLFillPercent);
		const int c = ClampColor(color);
		TESEffectShader *shader = g_shaders[(level * kColors + c) * kSlots + slot];
		if (!shader)
			return;

		if (g_canTune)
			TuneShader(shader, seconds);

		CallPlay(shader, ref, seconds + 1.0f);
	}

	bool IsLootableCorpse(TESObjectREFR *ref);

	void OnCrosshair(TESObjectREFR *ref)
	{
		if (!g_ready || !Settings::bHLEnabled || !ref || ref == g_thePlayer || !ref->baseForm)
			return;

		// cadaver com loot na mira: brilha na cor dos cadaveres
		if (ref->formType == FormType::Character)
		{
			if (Settings::bHLCorpses && IsLootableCorpse(ref))
			{
				const UInt32 corpseSlot = (g_slot++) % kCrosshairSlots;
				if (Settings::bHLDebug)
					_MESSAGE("Highlight: mira %08X cadaver: cor %d, posicao %u", ref->formID, Settings::iHLCorpseColor, corpseSlot);
				ApplyShader(ref, Settings::iHLCorpseColor, corpseSlot, Settings::iHLCrosshairMs * 0.001f);
			}
			else if (Settings::bHLDebug)
			{
				_MESSAGE("Highlight: mira %08X ator: ignorado (nao e um cadaver com loot)", ref->formID);
			}
			return;
		}

		const FormType type = ref->baseForm->formType;

		bool stealable = false;
		if (!PassesObjectFilters(ref, type, stealable))
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

	// item valido para levar (mesma ideia do menu de loot)
	bool IsValidLootForm(TESForm *item)
	{
		if (!item || item->Is(FormType::LeveledItem))
			return false;

		if (item->Is(FormType::Light))
		{
			TESObjectLIGH *light = static_cast<TESObjectLIGH *>(item);
			if (!light->CanBeCarried())
				return false;
		}
		else if (!item->IsPlayable())
		{
			return false;
		}

		TESFullName *fullName = DYNAMIC_CAST<TESFullName *>(item);
		if (!fullName)
			return false;

		const char *name = fullName->GetFullName();
		return name && name[0];
	}

	bool IsEnchantedItem(TESForm *form, InventoryEntryData *entry)
	{
		if (!form || !(form->IsArmor() || form->IsWeapon()))
			return false;

		if (entry && entry->extraList && !entry->extraList->empty())
		{
			BaseExtraList *baseExtraList = entry->extraList->front();
			ExtraEnchantment *exEnchant = baseExtraList ? baseExtraList->GetExtraData<ExtraEnchantment>() : nullptr;
			if (exEnchant && exEnchant->enchant)
				return true;
		}

		TESEnchantableForm *enchantForm = DYNAMIC_CAST<TESEnchantableForm *>(form);
		return enchantForm && enchantForm->enchantment;
	}

	bool FilterActive()
	{
		return Settings::bHLCorpseFilter && (Settings::bHLCFQuest || Settings::bHLCFKey || Settings::bHLCFEnchanted ||
			Settings::bHLCFBooks || Settings::bHLCFConsumables || Settings::iHLCMinValue > 0);
	}

	bool ItemMatchesFilter(TESForm *item, InventoryEntryData *entry)
	{
		const FormType t = item->formType;

		if (Settings::bHLCFQuest && entry && entry->IsQuestItem())
			return true;
		if (Settings::bHLCFKey && t == FormType::Key)
			return true;
		if (Settings::bHLCFEnchanted && IsEnchantedItem(item, entry))
			return true;
		if (Settings::bHLCFBooks && t == FormType::Book)
			return true;
		if (Settings::bHLCFConsumables && (t == FormType::Potion || t == FormType::Ingredient || t == FormType::ScrollItem))
			return true;

		if (Settings::iHLCMinValue > 0)
		{
			SInt32 value = 0;
			if (entry)
			{
				value = entry->GetValue();
			}
			else
			{
				TESValueForm *valueForm = DYNAMIC_CAST<TESValueForm *>(item);
				if (valueForm)
					value = valueForm->value;
			}

			if (value >= Settings::iHLCMinValue)
				return true;
		}

		return false;
	}

	struct LootInfo
	{
		SInt32				count;
		InventoryEntryData	*entry;
	};

	// O cadaver tem loot? Sem filtros, qualquer item valido serve; com filtros, so os que combinam.
	// Le o contentor base e as alteracoes sem criar nem mudar nada.
	bool HasInterestingLoot(TESObjectREFR *ref)
	{
		const bool filtering = FilterActive();

		std::map<TESForm *, LootInfo> items;

		TESContainer *container = ref->GetContainer();
		if (container)
		{
			TESContainer::Entry *entry = nullptr;
			UInt32 index = 0;
			while (container->GetContainerItemAt(index++, entry))
			{
				if (!entry || !IsValidLootForm(entry->form))
					continue;

				LootInfo &info = items[entry->form];
				info.count += entry->count;
			}
		}

		ExtraContainerChanges *exChanges = ref->extraData.GetExtraData<ExtraContainerChanges>();
		InventoryChanges *changes = exChanges ? exChanges->changes : nullptr;
		if (changes && changes->entryList)
		{
			for (InventoryEntryData *pEntry : *changes->entryList)
			{
				if (!pEntry || !IsValidLootForm(pEntry->baseForm))
					continue;

				LootInfo &info = items[pEntry->baseForm];
				info.count += pEntry->GetCount();
				if (!info.entry)
					info.entry = pEntry;
			}
		}

		for (const auto &node : items)
		{
			if (node.second.count <= 0)
				continue;

			if (!filtering)
				return true;

			if (ItemMatchesFilter(node.first, node.second.entry))
				return true;
		}

		return false;
	}

	// cadaver com loot: morto, com 3D, com itens (e que passam os filtros) e (opcionalmente) ainda nao revistado
	bool IsLootableCorpse(TESObjectREFR *ref)
	{
		Actor *actor = static_cast<Actor *>(ref);

		if (!actor->IsDead(true) || actor->IsGhost())
			return false;

		if (ref->IsDeleted() || ref->IsDisabled() || !ref->Is3DLoaded())
			return false;

		if (Settings::bHLCorpseHideSearched && g_searched.find(ref->formID) != g_searched.end())
			return false;

		return HasInterestingLoot(ref);
	}

	struct Candidate
	{
		TESObjectREFR	*ref;
		float			dist2;
		bool			corpse;
		bool			stealable;
	};

	void VisitCell(TESObjectCELL *cell, float px, float py, float pz, bool corpsesOnly, std::vector<Candidate> &out)
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

			// um ator e uma referencia do tipo Character (o formulario base e um NPC)
			if (ref->formType == FormType::Character)
			{
				if (!Settings::bHLCorpses || d2 > corpseR2 || !IsLootableCorpse(ref))
					continue;

				Candidate c = { ref, d2, true, false };
				out.push_back(c);
			}
			else
			{
				if (corpsesOnly || d2 > objectR2)
					continue;

				const FormType type = ref->baseForm->formType;
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
	void Scan(bool corpsesOnly)
	{
		if (!g_ready || (!corpsesOnly && !Settings::bHLScanEnabled) || !g_thePlayer || !g_thePlayer->parentCell)
			return;

		const float px = g_thePlayer->pos.x;
		const float py = g_thePlayer->pos.y;
		const float pz = g_thePlayer->pos.z;

		std::vector<Candidate> found;
		found.reserve(256);

		TESObjectCELL *playerCell = g_thePlayer->parentCell;
		if (playerCell->IsInterior() || !g_TES || !g_TES->gridCellArray)
		{
			VisitCell(playerCell, px, py, pz, corpsesOnly, found);
		}
		else
		{
			GridCellArray *grid = g_TES->gridCellArray;
			for (UInt32 y = 0; y < grid->size; ++y)
			{
				for (UInt32 x = 0; x < grid->size; ++x)
					VisitCell(grid->Get(x, y), px, py, pz, corpsesOnly, found);
			}
		}

		std::sort(found.begin(), found.end(), CandidateCloser);

		size_t limit = (size_t)Settings::iHLScanMax;
		if (found.size() < limit)
			limit = found.size();

		const float seconds = corpsesOnly ? 2.0f : Settings::iHLScanMs * 0.001f;
		const UInt32 scanSlots = kSlots - kCrosshairSlots;

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
	for (int i = 0; i < kLevels * kColors * kSlots; ++i)
	{
		TESForm *form = LookupFormByID(((UInt32)modIndex << 24) | (kFirstLocalID + (UInt32)i));
		if (form && form->formType == FormType::EffectShader)
		{
			g_shaders[i] = static_cast<TESEffectShader *>(form);
			++found;
		}
	}
	_MESSAGE("Highlight: %d de %d shaders encontrados (indice do ESP %02X)", found, kLevels * kColors * kSlots, modIndex);
	if (found == 0)
		return;

	// as texturas do brilho (paletas de cor e preenchimentos) tem de estar em Data\\Textures\\Effects\\Gradients;
	// sem elas o jogo carrega os shaders mas o brilho nao aparece
	{
		const char *colors[] = { "Gold", "White", "Green", "Cyan", "Red" };
		int missing = 0;
		int total = 0;
		for (int level = 0; level <= kLevels; ++level)
		{
			// level == kLevels: a textura de preenchimento
			const int count = (level < kLevels) ? kColors : 1;
			for (int n = 0; n < count; ++n)
			{
				char path[MAX_PATH];
				if (level < kLevels)
					sprintf_s(path, "Data\\Textures\\Effects\\Gradients\\BetterLooting_%s_L%d.dds", colors[n], level);
				else
					sprintf_s(path, "Data\\Textures\\Effects\\Gradients\\BetterLooting_Fill0.dds");

				++total;
				if (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES)
				{
					++missing;
					_MESSAGE("AVISO: textura em falta: %s", path);
				}
			}
		}
		if (missing)
			_MESSAGE("Highlight: faltam %d de %d texturas; o brilho nao vai aparecer (instale a pasta Textures do pacote)", missing, total);
		else
			_MESSAGE("Highlight: as %d texturas do brilho existem", total);
	}

	// verificacao: os dados do shader em memoria batem certo com o que o ESP gravou?
	TESEffectShader *probe = g_shaders[0];
	if (probe)
	{
		const UInt8 *d = probe->unk14.pad;
		const float fullTime = *(const float *)(d + kOffFullAlphaTime);
		const bool timeOk = (fullTime > kCrosshairFullAlpha - 0.01f) && (fullTime < kCrosshairFullAlpha + 0.01f);
		const bool colorOk = d[kOffEdgeColor] == kPalette[0][0] && d[kOffEdgeColor + 1] == kPalette[0][1] && d[kOffEdgeColor + 2] == kPalette[0][2];
		_MESSAGE("Highlight: verificacao dos dados do shader: tempo %s, cor %s", timeOk ? "OK" : "DIFERENTE", colorOk ? "OK" : "DIFERENTE");
		g_canTune = timeOk && colorOk;
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


void Highlight::OnScanKey(bool a_down, bool a_up, float a_heldSeconds, bool a_lootMenuVisible, bool a_pad)
{
	// estados separados para o teclado/rato e para o comando
	static bool s_active[2] = { false, false };
	static float s_nextAt[2] = { 0.0f, 0.0f };

	const int idx = a_pad ? 1 : 0;

	if (!g_ready || !Settings::bHLScanEnabled)
	{
		s_active[idx] = false;
		return;
	}

	// com o menu de loot aberto o botao de pegar esta em uso: nao ha varrimento
	if (a_lootMenuVisible)
	{
		s_active[idx] = false;
		return;
	}

	if (a_down)
	{
		s_active[idx] = true;
		s_nextAt[idx] = Settings::iHLScanHoldMs * 0.001f;
	}

	if (a_up)
	{
		s_active[idx] = false;
		return;
	}

	if (!s_active[idx] || a_heldSeconds < s_nextAt[idx])
		return;

	Scan(false);

	// mantido premido: refresca antes de o brilho acabar
	float interval = Settings::iHLScanMs * 0.001f - 0.8f;
	if (interval < 0.5f)
		interval = 0.5f;
	s_nextAt[idx] = a_heldSeconds + interval;
}


void Highlight::Tick()
{
	if (!g_ready || !Settings::bHLCorpseAlways || !Settings::bHLCorpses)
		return;

	static DWORD s_last = 0;
	const DWORD now = GetTickCount();
	if (now - s_last < 1000)
		return;

	s_last = now;
	Scan(true);
}


//===================================================
// lista de cadaveres revistados guardada no save (SKSE cosave)
//===================================================
namespace
{
	const UInt32 kRecordSearched = 'SRCH';

	void OnSerializationRevert(SKSESerializationInterface *)
	{
		g_searched.clear();
	}

	void OnSerializationSave(SKSESerializationInterface *a_intfc)
	{
		if (!a_intfc->OpenRecord(kRecordSearched, 1))
			return;

		UInt32 count = (UInt32)g_searched.size();
		a_intfc->WriteRecordData(&count, sizeof(count));
		for (UInt32 id : g_searched)
			a_intfc->WriteRecordData(&id, sizeof(id));
	}

	void OnSerializationLoad(SKSESerializationInterface *a_intfc)
	{
		g_searched.clear();

		UInt32 type = 0, version = 0, length = 0;
		while (a_intfc->GetNextRecordInfo(&type, &version, &length))
		{
			if (type != kRecordSearched)
				continue;

			UInt32 count = 0;
			if (a_intfc->ReadRecordData(&count, sizeof(count)) != sizeof(count) || count > 100000)
				continue;

			for (UInt32 i = 0; i < count; ++i)
			{
				UInt32 id = 0;
				if (a_intfc->ReadRecordData(&id, sizeof(id)) != sizeof(id))
					break;

				UInt32 resolved = 0;
				if (a_intfc->ResolveFormId(id, &resolved))
					g_searched.insert(resolved);
			}
		}

		_MESSAGE("Highlight: %u cadaveres revistados carregados do save", (unsigned)g_searched.size());
	}
}


void Highlight::RegisterSerialization(const SKSESerializationInterface *a_intfc)
{
	if (!a_intfc)
	{
		_MESSAGE("AVISO: sem interface de serializacao; os cadaveres revistados nao ficam guardados no save");
		return;
	}

	a_intfc->SetUniqueID('BLTR');
	a_intfc->SetRevertCallback(OnSerializationRevert);
	a_intfc->SetSaveCallback(OnSerializationSave);
	a_intfc->SetLoadCallback(OnSerializationLoad);
}


void Highlight::MarkSearched(TESObjectREFR *a_ref)
{
	if (a_ref && a_ref->formType == FormType::Character)
		g_searched.insert(a_ref->formID);
}
