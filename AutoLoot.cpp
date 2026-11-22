#include <SKSE.h>
#include <SKSE/DebugLog.h>
#include <SKSE/GameRTTI.h>
#include <SKSE/GameForms.h>
#include <SKSE/GameObjects.h>
#include <SKSE/GameReferences.h>
#include <Skyrim/FileIO/TES.h>
#include <Skyrim/FileIO/TESDataHandler.h>
#include <Windows.h>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "AutoLoot.h"
#include "ItemData.h"
#include "Settings.h"
#include "Highlight.h"
#include "LootMenu.h"

namespace
{
	enum AutoType
	{
		kWeapon1H, kWeapon2H, kBow, kStaff, kAmmo,
		kArmorHeavy, kArmorLight, kClothing, kShield, kJewelry,
		kPotion, kPoison, kFood, kIngredient, kScroll,
		kSpellTome, kSkillBook, kBookOther, kKey,
		kSoulGemEmpty, kSoulGemFilled, kGem, kIngot, kOre, kLockpick, kGold,
		kTorch, kMiscOther, kFlora, kOreVein,
		kCount
	};

	// nomes usados nas seccoes [Type.<Nome>] do ficheiro de regras
	const char *kTypeNames[AutoLoot::kTypeCount] = {
		"Weapon1H", "Weapon2H", "Bow", "Staff", "Ammo",
		"ArmorHeavy", "ArmorLight", "Clothing", "Shield", "Jewelry",
		"Potion", "Poison", "Food", "Ingredient", "Scroll",
		"SpellTome", "SkillBook", "BookOther", "Key",
		"SoulGemEmpty", "SoulGemFilled", "Gem", "Ingot", "Ore", "Lockpick", "Gold",
		"Torch", "MiscOther", "Flora", "OreVein"
	};

	struct Rule
	{
		bool	pick;
		bool	glow;
		int		color;			// 0 dourado, 1 branco, 2 verde, 3 ciano, 4 vermelho
		int		minValue;
		int		minVpw;			// valor por peso
		int		minArmor;
		int		minDamage;
	};

	Rule g_rules[AutoLoot::kTypeCount];

	enum NameMode { kExact, kContains, kStarts, kEnds };

	struct NameFilter
	{
		NameMode		mode;
		std::string		text;		// em minusculas
	};

	std::vector<NameFilter>		g_allowNames;
	std::vector<NameFilter>		g_forbidNames;
	std::vector<NameFilter>		g_forbidActivatorNames;
	std::vector<NameFilter>		g_veinNames;
	std::set<UInt32>			g_allowForms;
	std::set<UInt32>			g_forbidForms;
	std::set<UInt32>			g_forbidActivatorForms;

	bool						g_toggled = false;
	bool						g_holdReady[2] = { false, false };
	bool						g_holdDown[2] = { false, false };
	std::map<UInt32, DWORD>		g_attempts;		// ultima tentativa de apanhar cada referencia

	//-----------------------------------------------------------------
	// texto
	//-----------------------------------------------------------------
	std::string Trim(const std::string &s)
	{
		size_t a = 0, b = s.size();
		while (a < b && (s[a] == ' ' || s[a] == '\t' || s[a] == '\r' || s[a] == '\n'))
			++a;
		while (b > a && (s[b - 1] == ' ' || s[b - 1] == '\t' || s[b - 1] == '\r' || s[b - 1] == '\n'))
			--b;
		return s.substr(a, b - a);
	}

	std::string Lower(const std::string &s)
	{
		std::string out = s;
		for (size_t i = 0; i < out.size(); ++i)
			out[i] = (char)tolower((unsigned char)out[i]);
		return out;
	}

	bool StartsWith(const std::string &s, const char *prefix)
	{
		return s.compare(0, strlen(prefix), prefix) == 0;
	}

	bool ParseNameFilter(const std::string &line, NameFilter &out)
	{
		std::string t = Lower(Trim(line));
		if (t.empty())
			return false;

		out.mode = kExact;
		if (StartsWith(t, "exact:"))			{ out.mode = kExact;	t = t.substr(6); }
		else if (StartsWith(t, "contains:"))	{ out.mode = kContains;	t = t.substr(9); }
		else if (StartsWith(t, "starts:"))		{ out.mode = kStarts;	t = t.substr(7); }
		else if (StartsWith(t, "ends:"))		{ out.mode = kEnds;		t = t.substr(5); }

		out.text = Trim(t);
		return !out.text.empty();
	}

	bool NameMatches(const NameFilter &f, const std::string &name)
	{
		switch (f.mode)
		{
		case kExact:	return name == f.text;
		case kContains:	return name.find(f.text) != std::string::npos;
		case kStarts:	return name.size() >= f.text.size() && name.compare(0, f.text.size(), f.text) == 0;
		case kEnds:		return name.size() >= f.text.size() && name.compare(name.size() - f.text.size(), f.text.size(), f.text) == 0;
		}
		return false;
	}

	bool MatchesAny(const std::vector<NameFilter> &list, const std::string &lowerName)
	{
		for (size_t i = 0; i < list.size(); ++i)
		{
			if (NameMatches(list[i], lowerName))
				return true;
		}
		return false;
	}

	// "Plugin.esp|0x00ABCDEF" (id local ao plugin) ou "0x00ABCDEF" (id completo)
	bool ParseForm(const std::string &line, UInt32 &out)
	{
		std::string t = Trim(line);
		if (t.empty())
			return false;

		size_t bar = t.find('|');
		if (bar != std::string::npos)
		{
			std::string plugin = Trim(t.substr(0, bar));
			UInt32 local = (UInt32)strtoul(Trim(t.substr(bar + 1)).c_str(), nullptr, 16);

			TESDataHandler *dataHandler = TESDataHandler::GetSingleton();
			const UInt8 modIndex = dataHandler ? dataHandler->GetModIndex(plugin.c_str()) : 0xFF;
			if (modIndex == 0xFF)
				return false;

			out = ((UInt32)modIndex << 24) | (local & 0x00FFFFFF);
			return true;
		}

		out = (UInt32)strtoul(t.c_str(), nullptr, 16);
		return out != 0;
	}

	void AddListEntry(const std::string &section, const std::string &line)
	{
		NameFilter filter;
		UInt32 form = 0;

		if (section == "allownames")				{ if (ParseNameFilter(line, filter)) g_allowNames.push_back(filter); }
		else if (section == "forbidnames")			{ if (ParseNameFilter(line, filter)) g_forbidNames.push_back(filter); }
		else if (section == "orveinnames" || section == "oreveinnames")	{ if (ParseNameFilter(line, filter)) g_veinNames.push_back(filter); }
		else if (section == "allowforms")			{ if (ParseForm(line, form)) g_allowForms.insert(form); }
		else if (section == "forbidforms")			{ if (ParseForm(line, form)) g_forbidForms.insert(form); }
		else if (section == "forbidactivators")
		{
			// formulario (contem | ou comeca por 0x) ou nome
			std::string t = Trim(line);
			if (t.find('|') != std::string::npos || StartsWith(Lower(t), "0x"))
			{
				if (ParseForm(t, form))
					g_forbidActivatorForms.insert(form);
			}
			else if (ParseNameFilter(t, filter))
			{
				g_forbidActivatorNames.push_back(filter);
			}
		}
	}

	int TypeIndexFromName(const std::string &lowerName)
	{
		for (int i = 0; i < AutoLoot::kTypeCount; ++i)
		{
			if (lowerName == Lower(kTypeNames[i]))
				return i;
		}
		return -1;
	}

	bool ApplyRuleKey(Rule &rule, const std::string &key, int value)
	{
		const std::string k = Lower(key);
		if (k == "pick")					rule.pick = (value != 0);
		else if (k == "glow")				rule.glow = (value != 0);
		else if (k == "color")				rule.color = (value < 0) ? 0 : ((value > 4) ? 4 : value);
		else if (k == "minvalue")			rule.minValue = value < 0 ? 0 : value;
		else if (k == "minvalueperweight")	rule.minVpw = value < 0 ? 0 : value;
		else if (k == "minarmor")			rule.minArmor = value < 0 ? 0 : value;
		else if (k == "mindamage")			rule.minDamage = value < 0 ? 0 : value;
		else return false;
		return true;
	}

	//-----------------------------------------------------------------
	// classificacao
	//-----------------------------------------------------------------
	int MapItemType(int t, TESForm *form)
	{
		using T = ItemData;

		switch (t)
		{
		case T::kType_WeaponGreatSword: case T::kType_WeaponBattleAxe: case T::kType_WeaponHammer:
			return kWeapon2H;
		case T::kType_WeaponStaff:
			return kStaff;
		case T::kType_WeaponBow: case T::kType_WeaponCrossbow:
			return kBow;
		case T::kType_WeaponArrow: case T::kType_WeaponBolt:
			return kAmmo;
		case T::kType_DefaultWeapon: case T::kType_WeaponSword: case T::kType_WeaponDaedra: case T::kType_WeaponDagger:
		case T::kType_WeaponWarAxe: case T::kType_WeaponMace: case T::kType_WeaponPickAxe: case T::kType_WeaponWoodAxe:
			return kWeapon1H;

		case T::kType_LightArmorShield: case T::kType_ArmorShield: case T::kType_ClothingShield:
			return kShield;
		case T::kType_ArmorAmulet: case T::kType_ArmorRing: case T::kType_Circlet:
			return kJewelry;
		case T::kType_LightArmorBody: case T::kType_LightArmorHead: case T::kType_LightArmorHands:
		case T::kType_LightArmorForearms: case T::kType_LightArmorFeet: case T::kType_LightArmorCalves:
		case T::kType_LightArmorMask:
			return kArmorLight;
		case T::kType_DefaultArmor:
		case T::kType_ArmorBody: case T::kType_ArmorHead: case T::kType_ArmorHands: case T::kType_ArmorForearms:
		case T::kType_ArmorFeet: case T::kType_ArmorCalves: case T::kType_ArmorMask: case T::kType_ArmorBracer:
		case T::kType_ArmorDaedra:
			return kArmorHeavy;
		case T::kType_ClothingBody: case T::kType_ClothingRobe: case T::kType_ClothingHead: case T::kType_ClothingPants:
		case T::kType_ClothingHands: case T::kType_ClothingForearms: case T::kType_ClothingFeet:
		case T::kType_ClothingCalves: case T::kType_ClothingShoes: case T::kType_ClothingMask:
			return kClothing;

		case T::kType_PotionPoison:
			return kPoison;
		case T::kType_DefaultPotion: case T::kType_PotionHealth: case T::kType_PotionStam: case T::kType_PotionMagic:
		case T::kType_PotionFrost: case T::kType_PotionFire: case T::kType_PotionShock:
			return kPotion;
		case T::kType_DefaultFood: case T::kType_FoodWine: case T::kType_FoodBeer:
			return kFood;
		case T::kType_DefaultIngredient:
			return kIngredient;
		case T::kType_DefaultScroll:
			return kScroll;

		case T::kType_BookTome: case T::kType_TomeRead:
			return kSpellTome;
		case T::kType_DefaultBook: case T::kType_BookRead:
		{
			TESObjectBOOK *book = static_cast<TESObjectBOOK *>(form);
			if (book && (book->data.flags & TESObjectBOOK::Data::kType_Skill))
				return kSkillBook;
			return kBookOther;
		}
		case T::kType_BookJournal: case T::kType_BookNote: case T::kType_BookMap:
			return kBookOther;

		case T::kType_DefaultKey: case T::kType_KeyHouse:
			return kKey;

		case T::kType_SoulGemEmpty: case T::kType_SoulGemGrandEmpty:
			return kSoulGemEmpty;
		case T::kType_MiscSoulGem: case T::kType_SoulGemPartial: case T::kType_SoulGemFull:
		case T::kType_SoulGemGrandPartial: case T::kType_SoulGemGrandFull: case T::kType_SoulGemAzura:
			return kSoulGemFilled;
		case T::kType_MiscGem:
			return kGem;
		case T::kType_MiscIngot:
			return kIngot;
		case T::kType_MiscOre:
			return kOre;
		case T::kType_MiscLockPick:
			return kLockpick;
		case T::kType_MiscGold: case T::kType_MiscGoldSack:
			return kGold;
		case T::kType_MiscTorch:
			return kTorch;

		case T::kType_None:
			return kMiscOther;
		}

		return kMiscOther;		// restantes tipos diversos
	}

	// -1 = nao e algo que apanhamos
	int ClassifyRef(TESObjectREFR *ref, const std::string &lowerName)
	{
		TESForm *base = ref->baseForm;
		const FormType ft = base->formType;

		if (ft == FormType::Flora || ft == FormType::Tree)
			return kFlora;

		if (ft == FormType::Activator || ft == FormType::Container)
		{
			// filoes de minerio reconhecem-se pelo nome (ver [OreVeinNames])
			return MatchesAny(g_veinNames, lowerName) ? kOreVein : -1;
		}

		if (ft == FormType::Light)
		{
			TESObjectLIGH *light = static_cast<TESObjectLIGH *>(base);
			return light->CanBeCarried() ? kTorch : -1;
		}

		if (ft == FormType::Weapon || ft == FormType::Armor || ft == FormType::Ammo || ft == FormType::Book ||
			ft == FormType::Ingredient || ft == FormType::Key || ft == FormType::Misc || ft == FormType::Potion ||
			ft == FormType::ScrollItem || ft == FormType::SoulGem)
		{
			return MapItemType(ItemTypeOfForm(base), base);
		}

		return -1;
	}

	bool IsWeaponType(int t) { return t == kWeapon1H || t == kWeapon2H || t == kBow || t == kStaff || t == kAmmo; }
	bool IsArmorType(int t) { return t == kArmorHeavy || t == kArmorLight || t == kShield || t == kJewelry || t == kClothing; }

	bool PassesThresholds(const Rule &r, TESForm *base, int type)
	{
		if (type == kFlora || type == kOreVein)
			return true;

		double value = 0.0;
		double weight = 0.0;

		TESValueForm *valueForm = DYNAMIC_CAST<TESValueForm *>(base);
		if (valueForm)
			value = (double)valueForm->value;

		TESWeightForm *weightForm = DYNAMIC_CAST<TESWeightForm *>(base);
		if (weightForm)
			weight = (double)weightForm->weight;

		if (r.minValue > 0 && value < r.minValue)
			return false;

		if (r.minVpw > 0)
		{
			const double vpw = (weight > 0.0) ? (value / weight) : (value > 0.0 ? 1.0e9 : 0.0);
			if (vpw < r.minVpw)
				return false;
		}

		if (r.minArmor > 0 && IsArmorType(type))
		{
			TESObjectARMO *armor = DYNAMIC_CAST<TESObjectARMO *>(base);
			const double rating = armor ? armor->armorValTimes100 / 100.0 : 0.0;
			if (rating < r.minArmor)
				return false;
		}

		if (r.minDamage > 0 && IsWeaponType(type))
		{
			TESAttackDamageForm *damageForm = DYNAMIC_CAST<TESAttackDamageForm *>(base);
			const double damage = damageForm ? (double)damageForm->GetAttackDamage() : 0.0;
			if (damage < r.minDamage)
				return false;
		}

		return true;
	}

	struct Hit
	{
		TESObjectREFR	*ref;
		float			dist2;
		bool			pick;
		bool			glow;
		int				color;
	};

	bool Evaluate(TESObjectREFR *ref, Hit &hit)
	{
		if (!ref || ref == g_thePlayer || !ref->baseForm)
			return false;

		if (ref->formType == FormType::Character)
			return false;		// nunca atores nem cadaveres

		if (ref->IsDeleted() || ref->IsDisabled() || !ref->Is3DLoaded())
			return false;

		if ((*(const UInt32 *)&ref->flags) & TESObjectREFR::kFlag_Harvested)
			return false;

		const char *rawName = ref->GetReferenceName();
		if (!rawName || !rawName[0])
			return false;

		const std::string name = Lower(rawName);
		const int type = ClassifyRef(ref, name);
		if (type < 0)
			return false;

		// nunca apanha o que tem dono (seria roubar)
		if (ref->IsOffLimits())
			return false;

		// listas de proibicao
		if (g_forbidForms.count(ref->formID) || g_forbidForms.count(ref->baseForm->formID) || MatchesAny(g_forbidNames, name))
			return false;

		if ((type == kFlora || type == kOreVein) &&
			(g_forbidActivatorForms.count(ref->formID) || g_forbidActivatorForms.count(ref->baseForm->formID) ||
				MatchesAny(g_forbidActivatorNames, name)))
			return false;

		const Rule &rule = g_rules[type];
		const bool allowed = g_allowForms.count(ref->formID) || g_allowForms.count(ref->baseForm->formID) ||
			MatchesAny(g_allowNames, name);

		hit.ref = ref;
		hit.color = rule.color;

		if (allowed)
		{
			// as listas de permissao apanham e dao brilho mesmo abaixo dos limiares ou com o tipo desligado
			hit.pick = true;
			hit.glow = true;
			return true;
		}

		if (!rule.pick && !rule.glow)
			return false;

		if (!PassesThresholds(rule, ref->baseForm, type))
			return false;

		hit.pick = rule.pick;
		hit.glow = rule.glow;
		return true;
	}

	void VisitCell(TESObjectCELL *cell, float px, float py, float pz, std::vector<Hit> &out)
	{
		if (!cell)
			return;

		const float glowRadius = (float)Settings::iALGlowRadius;
		const float pickRadius = (float)Settings::iALPickupRadius;
		const float maxRadius = (glowRadius > pickRadius ? glowRadius : pickRadius);
		const float maxR2 = maxRadius * maxRadius;

		for (TESObjectREFR *ref : cell->objectList)
		{
			if (!ref)
				continue;

			const float dx = ref->pos.x - px;
			const float dy = ref->pos.y - py;
			const float dz = ref->pos.z - pz;
			const float d2 = dx * dx + dy * dy + dz * dz;
			if (d2 > maxR2)
				continue;

			Hit hit = { nullptr, d2, false, false, 0 };
			if (Evaluate(ref, hit))
			{
				hit.dist2 = d2;
				out.push_back(hit);
			}
		}
	}

	bool HitCloser(const Hit &a, const Hit &b)
	{
		return a.dist2 < b.dist2;
	}

	void Run()
	{
		if (!g_thePlayer || !g_thePlayer->parentCell)
			return;

		// nunca em combate nem com a janela de loot aberta
		if (g_thePlayer->IsInCombat())
			return;

		LootMenu *lootMenu = LootMenu::GetSingleton();
		if (lootMenu && lootMenu->IsVisible())
			return;

		const float px = g_thePlayer->pos.x;
		const float py = g_thePlayer->pos.y;
		const float pz = g_thePlayer->pos.z;

		std::vector<Hit> hits;
		hits.reserve(128);

		TESObjectCELL *playerCell = g_thePlayer->parentCell;
		if (playerCell->IsInterior() || !g_TES || !g_TES->gridCellArray)
		{
			VisitCell(playerCell, px, py, pz, hits);
		}
		else
		{
			GridCellArray *grid = g_TES->gridCellArray;
			for (UInt32 y = 0; y < grid->size; ++y)
			{
				for (UInt32 x = 0; x < grid->size; ++x)
					VisitCell(grid->Get(x, y), px, py, pz, hits);
			}
		}

		std::sort(hits.begin(), hits.end(), HitCloser);

		const float glowR2 = (float)Settings::iALGlowRadius * (float)Settings::iALGlowRadius;
		const float pickR2 = (float)Settings::iALPickupRadius * (float)Settings::iALPickupRadius;
		const DWORD now = GetTickCount();

		int glowed = 0;
		int picked = 0;

		for (size_t i = 0; i < hits.size(); ++i)
		{
			const Hit &hit = hits[i];

			if (hit.glow && hit.dist2 <= glowR2 && glowed < 48)
			{
				Highlight::Glow(hit.ref, hit.color, 2.0f);
				++glowed;
			}

			if (hit.pick && hit.dist2 <= pickR2 && picked < Settings::iALMaxPerTick)
			{
				std::map<UInt32, DWORD>::iterator it = g_attempts.find(hit.ref->formID);
				if (it != g_attempts.end() && now - it->second < 15000)
					continue;		// tentou ha pouco: nao repetir

				g_attempts[hit.ref->formID] = now;
				hit.ref->Activate(g_thePlayer, false);
				++picked;
			}
		}

		// limpa tentativas antigas
		if (g_attempts.size() > 512)
		{
			for (std::map<UInt32, DWORD>::iterator it = g_attempts.begin(); it != g_attempts.end();)
			{
				if (now - it->second > 15000)
					g_attempts.erase(it++);
				else
					++it;
			}
		}

		if (Settings::bHLDebug && (glowed || picked))
			_MESSAGE("AutoLoot: %u candidatos, %d com brilho, %d apanhados", (unsigned)hits.size(), glowed, picked);
	}

	void Notify(const char *text)
	{
		typedef void(*FnDebug_Notification)(const char *, bool, bool);
		const FnDebug_Notification fnDebug_Notification = (FnDebug_Notification)0x008997A0;
		fnDebug_Notification(text, false, true);
	}

	void ResetRules()
	{
		for (int i = 0; i < AutoLoot::kTypeCount; ++i)
		{
			g_rules[i].pick = false;
			g_rules[i].glow = false;
			g_rules[i].color = 0;
			g_rules[i].minValue = 0;
			g_rules[i].minVpw = 0;
			g_rules[i].minArmor = 0;
			g_rules[i].minDamage = 0;
		}

		g_allowNames.clear();
		g_forbidNames.clear();
		g_forbidActivatorNames.clear();
		g_veinNames.clear();
		g_allowForms.clear();
		g_forbidForms.clear();
		g_forbidActivatorForms.clear();
	}
}


void AutoLoot::Load()
{
	ResetRules();

	const char *path = Settings::GetAutoLootIniPath();
	_MESSAGE("AutoLoot::Load() de %s", path);

	FILE *file = nullptr;
	if (fopen_s(&file, path, "r") != 0 || !file)
	{
		_MESSAGE("  AVISO: o ficheiro de regras nao existe; o apanhar automatico fica sem regras");
		return;
	}

	std::string section;
	char buffer[1024];
	while (fgets(buffer, sizeof(buffer), file))
	{
		std::string line = Trim(buffer);
		if (line.empty() || line[0] == ';' || line[0] == '#')
			continue;

		if (line[0] == '[')
		{
			size_t end = line.find(']');
			section = Lower(Trim(line.substr(1, end == std::string::npos ? std::string::npos : end - 1)));
			continue;
		}

		if (StartsWith(section, "type."))
		{
			const int index = TypeIndexFromName(section.substr(5));
			size_t eq = line.find('=');
			if (index >= 0 && eq != std::string::npos)
				ApplyRuleKey(g_rules[index], Trim(line.substr(0, eq)), atoi(Trim(line.substr(eq + 1)).c_str()));
			continue;
		}

		AddListEntry(section, line);
	}
	fclose(file);

	int picking = 0, glowing = 0;
	for (int i = 0; i < kTypeCount; ++i)
	{
		if (g_rules[i].pick) ++picking;
		if (g_rules[i].glow) ++glowing;
	}
	_MESSAGE("  %d tipos com apanhar ligado, %d com brilho; listas: %u nomes permitidos, %u proibidos, %u formas permitidas, %u proibidas",
		picking, glowing, (unsigned)g_allowNames.size(), (unsigned)g_forbidNames.size(),
		(unsigned)g_allowForms.size(), (unsigned)g_forbidForms.size());
}


void AutoLoot::Tick()
{
	if (!Settings::bAutoLoot)
		return;

	const bool active = Settings::bALAlways || g_toggled || g_holdReady[0] || g_holdReady[1];
	if (!active)
		return;

	static DWORD s_last = 0;
	const DWORD now = GetTickCount();
	if (now - s_last < 500)
		return;

	s_last = now;
	Run();
}


void AutoLoot::OnToggleKey(bool a_lootMenuVisible)
{
	if (a_lootMenuVisible)
		return;

	if (!Settings::bAutoLoot)
	{
		Notify((Settings::iLanguage == 1) ? "Apanhar automatico esta desligado no MCM." : "Auto-loot is turned off in the MCM.");
		return;
	}

	g_toggled = !g_toggled;
	if (Settings::iLanguage == 1)
		Notify(g_toggled ? "Apanhar automatico ligado." : "Apanhar automatico desligado.");
	else
		Notify(g_toggled ? "Auto-loot on." : "Auto-loot off.");
}


void AutoLoot::OnHoldKey(bool a_down, bool a_up, float a_heldSeconds, bool a_lootMenuVisible, bool a_pad)
{
	const int idx = a_pad ? 1 : 0;

	if (a_lootMenuVisible || !Settings::bAutoLoot)
	{
		g_holdDown[idx] = false;
		g_holdReady[idx] = false;
		return;
	}

	if (a_down)
		g_holdDown[idx] = true;

	if (a_up)
	{
		g_holdDown[idx] = false;
		g_holdReady[idx] = false;
		return;
	}

	if (g_holdDown[idx] && a_heldSeconds * 1000.0f >= (float)Settings::iALHoldMs)
		g_holdReady[idx] = true;
}


bool AutoLoot::GetRule(const char *a_section, const char *a_key, int &a_out)
{
	if (!a_section || !a_key)
		return false;

	const std::string section = Lower(a_section);
	if (StartsWith(section, "type."))
	{
		const int index = TypeIndexFromName(section.substr(5));
		if (index < 0)
			return false;

		const std::string key = Lower(a_key);
		const Rule &r = g_rules[index];
		if (key == "pick")						a_out = r.pick ? 1 : 0;
		else if (key == "glow")					a_out = r.glow ? 1 : 0;
		else if (key == "color")				a_out = r.color;
		else if (key == "minvalue")				a_out = r.minValue;
		else if (key == "minvalueperweight")	a_out = r.minVpw;
		else if (key == "minarmor")				a_out = r.minArmor;
		else if (key == "mindamage")			a_out = r.minDamage;
		else return false;
		return true;
	}

	return false;
}


bool AutoLoot::SetRule(const char *a_section, const char *a_key, int a_value)
{
	if (!a_section || !a_key)
		return false;

	const std::string section = Lower(a_section);
	if (!StartsWith(section, "type."))
		return false;

	const int index = TypeIndexFromName(section.substr(5));
	if (index < 0 || !ApplyRuleKey(g_rules[index], a_key, a_value))
		return false;

	// grava o valor ja limitado
	int stored = 0;
	GetRule(a_section, a_key, stored);

	char buf[16];
	sprintf_s(buf, "%d", stored);
	const std::string sectionName = std::string("Type.") + kTypeNames[index];
	if (!WritePrivateProfileStringA(sectionName.c_str(), a_key, buf, Settings::GetAutoLootIniPath()))
		_MESSAGE("AVISO: nao consegui gravar %s/%s no ficheiro de regras (erro %lu)", sectionName.c_str(), a_key, GetLastError());

	return true;
}
