#include <SKSE.h>
#include <SKSE/DebugLog.h>
#include <Windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Settings.h"

bool	Settings::bDisableInCombat = false;
bool	Settings::bDisableTheft = false;
bool	Settings::bDisablePickpocketing = false;
bool	Settings::bDisableIfEmpty = false;
bool	Settings::bCloseIfEmpty = false;
bool	Settings::bUseConsole = false;
bool	Settings::bForceAnsi = false;
bool	Settings::bDisableLootSingle = false;
bool	Settings::bSortByCategory = true;
bool	Settings::bSortDescending = false;
bool	Settings::bSortWeightlessFirst = false;
int		Settings::iSortBy = 0;
bool	Settings::bShowValue = true;
bool	Settings::bShowWeight = true;
bool	Settings::bShowValuePerWeight = false;
bool	Settings::bShowItemIcons = true;

int		Settings::iScale = 100;
int		Settings::iItemLimit = 0;
int		Settings::iMaxLines = 7;
int		Settings::iMinLines = 0;
int		Settings::iAnchorX = 0;
int		Settings::iAnchorY = 0;
int		Settings::iOffsetX = 100;
int		Settings::iOffsetY = -200;
int		Settings::iAlphaNormal = 100;
int		Settings::iAlphaEmpty = 30;

int		Settings::iKeyTakeAll = 46;		// C
int		Settings::iKeySearch = 19;		// R
int		Settings::iSearchHoldMs = 0;	// 0 = um toque

bool	Settings::bHLEnabled = true;
bool	Settings::bHLRedStealable = true;
bool	Settings::bHLIgnoreUnnamed = true;
int		Settings::iHLColor = 0;
int		Settings::iHLCrosshairMs = 800;
int		Settings::iHLFillPercent = 15;
int		Settings::iHLEdgePercent = 60;
bool	Settings::bHLDebug = false;
bool	Settings::bHLScanEnabled = true;
int		Settings::iHLScanKey = 18;			// E
int		Settings::iHLScanHoldMs = 500;
int		Settings::iHLScanRadius = 1500;
int		Settings::iHLScanMs = 3000;
int		Settings::iHLScanMax = 64;
bool	Settings::bHLCorpses = true;
bool	Settings::bHLCorpseHideSearched = true;
int		Settings::iHLCorpseColor = 2;		// verde
int		Settings::iHLCorpseMaxDist = 2000;
bool	Settings::bHLActivator = true;
bool	Settings::bHLTalkingActivator = true;
bool	Settings::bHLContainer = true;
bool	Settings::bHLDoor = true;
bool	Settings::bHLFlora = true;
bool	Settings::bHLFurniture = true;
bool	Settings::bHLArmor = true;
bool	Settings::bHLWeapon = true;
bool	Settings::bHLAmmo = true;
bool	Settings::bHLBook = true;
bool	Settings::bHLIngredient = true;
bool	Settings::bHLKey = true;
bool	Settings::bHLMisc = true;
bool	Settings::bHLPotion = true;
bool	Settings::bHLScroll = true;
bool	Settings::bHLSoulGem = true;

int		Settings::iVisual = 0;			// Redesigned
int		Settings::iLanguage = 0;		// English

namespace
{
	struct Entry
	{
		const char	*name;
		void		*ptr;
		bool		isBool;
		int			def;
		int			minVal;
		int			maxVal;
	};

	const Entry kTable[] =
	{
		{ "bDisableInCombat",		&Settings::bDisableInCombat,		true,	0, 0, 1 },
		{ "bDisableTheft",			&Settings::bDisableTheft,			true,	0, 0, 1 },
		{ "bDisablePickpocketing",	&Settings::bDisablePickpocketing,	true,	0, 0, 1 },
		{ "bDisableIfEmpty",		&Settings::bDisableIfEmpty,			true,	0, 0, 1 },
		{ "bCloseIfEmpty",			&Settings::bCloseIfEmpty,			true,	0, 0, 1 },
		{ "bUseConsole",			&Settings::bUseConsole,				true,	0, 0, 1 },
		{ "bForceAnsi",				&Settings::bForceAnsi,				true,	0, 0, 1 },
		{ "bDisableLootSingle",		&Settings::bDisableLootSingle,		true,	0, 0, 1 },
		{ "bSortByCategory",		&Settings::bSortByCategory,			true,	1, 0, 1 },
		{ "bSortDescending",		&Settings::bSortDescending,			true,	0, 0, 1 },
		{ "bSortWeightlessFirst",	&Settings::bSortWeightlessFirst,		true,	0, 0, 1 },
		{ "iSortBy",				&Settings::iSortBy,					false,	0, 0, 3 },
		{ "bShowValue",				&Settings::bShowValue,				true,	1, 0, 1 },
		{ "bShowWeight",			&Settings::bShowWeight,				true,	1, 0, 1 },
		{ "bShowValuePerWeight",	&Settings::bShowValuePerWeight,		true,	0, 0, 1 },
		{ "bShowItemIcons",			&Settings::bShowItemIcons,			true,	1, 0, 1 },

		{ "iScale",					&Settings::iScale,					false,	100,	25,		400 },
		{ "iItemLimit",				&Settings::iItemLimit,				false,	0,		0,		200 },
		{ "iMaxLines",				&Settings::iMaxLines,				false,	7,		1,		25 },
		{ "iMinLines",				&Settings::iMinLines,				false,	0,		0,		25 },
		{ "iAnchorX",				&Settings::iAnchorX,				false,	0,		0,		2 },
		{ "iAnchorY",				&Settings::iAnchorY,				false,	0,		0,		2 },
		{ "iOffsetX",				&Settings::iOffsetX,				false,	100,	-2000,	2000 },
		{ "iOffsetY",				&Settings::iOffsetY,				false,	-200,	-2000,	2000 },
		{ "iAlphaNormal",			&Settings::iAlphaNormal,			false,	100,	10,		100 },
		{ "iAlphaEmpty",			&Settings::iAlphaEmpty,				false,	30,		0,		100 },

		{ "iKeyTakeAll",			&Settings::iKeyTakeAll,				false,	46,		1,		255 },
		{ "iKeySearch",				&Settings::iKeySearch,				false,	19,		1,		255 },
		{ "iSearchHoldMs",			&Settings::iSearchHoldMs,			false,	0,		0,		2000 },

		{ "bHLEnabled",				&Settings::bHLEnabled,				true,	1, 0, 1 },
		{ "bHLRedStealable",		&Settings::bHLRedStealable,			true,	1, 0, 1 },
		{ "bHLIgnoreUnnamed",		&Settings::bHLIgnoreUnnamed,			true,	1, 0, 1 },
		{ "iHLColor",				&Settings::iHLColor,				false,	0, 0, 4 },
		{ "iHLFillPercent",			&Settings::iHLFillPercent,			false,	15, 0, 100 },
		{ "iHLEdgePercent",			&Settings::iHLEdgePercent,			false,	60, 0, 100 },
		{ "bHLDebug",				&Settings::bHLDebug,				true,	0, 0, 1 },
		{ "bHLScanEnabled",			&Settings::bHLScanEnabled,			true,	1, 0, 1 },
		{ "iHLScanKey",				&Settings::iHLScanKey,				false,	18, 1, 255 },
		{ "iHLScanHoldMs",			&Settings::iHLScanHoldMs,			false,	500, 0, 2000 },
		{ "iHLScanRadius",			&Settings::iHLScanRadius,			false,	1500, 300, 5000 },
		{ "iHLScanMs",				&Settings::iHLScanMs,				false,	3000, 500, 10000 },
		{ "iHLScanMax",				&Settings::iHLScanMax,				false,	64, 8, 128 },
		{ "bHLCorpses",				&Settings::bHLCorpses,				true,	1, 0, 1 },
		{ "bHLCorpseHideSearched",	&Settings::bHLCorpseHideSearched,	true,	1, 0, 1 },
		{ "iHLCorpseColor",			&Settings::iHLCorpseColor,			false,	2, 0, 4 },
		{ "iHLCorpseMaxDist",		&Settings::iHLCorpseMaxDist,			false,	2000, 500, 5000 },
		{ "iHLCrosshairMs",			&Settings::iHLCrosshairMs,			false,	800, 100, 10000 },
		{ "bHLActivator",			&Settings::bHLActivator,			true,	1, 0, 1 },
		{ "bHLTalkingActivator",			&Settings::bHLTalkingActivator,			true,	1, 0, 1 },
		{ "bHLContainer",			&Settings::bHLContainer,			true,	1, 0, 1 },
		{ "bHLDoor",			&Settings::bHLDoor,			true,	1, 0, 1 },
		{ "bHLFlora",			&Settings::bHLFlora,			true,	1, 0, 1 },
		{ "bHLFurniture",			&Settings::bHLFurniture,			true,	1, 0, 1 },
		{ "bHLArmor",			&Settings::bHLArmor,			true,	1, 0, 1 },
		{ "bHLWeapon",			&Settings::bHLWeapon,			true,	1, 0, 1 },
		{ "bHLAmmo",			&Settings::bHLAmmo,			true,	1, 0, 1 },
		{ "bHLBook",			&Settings::bHLBook,			true,	1, 0, 1 },
		{ "bHLIngredient",			&Settings::bHLIngredient,			true,	1, 0, 1 },
		{ "bHLKey",			&Settings::bHLKey,			true,	1, 0, 1 },
		{ "bHLMisc",			&Settings::bHLMisc,			true,	1, 0, 1 },
		{ "bHLPotion",			&Settings::bHLPotion,			true,	1, 0, 1 },
		{ "bHLScroll",			&Settings::bHLScroll,			true,	1, 0, 1 },
		{ "bHLSoulGem",			&Settings::bHLSoulGem,			true,	1, 0, 1 },

		{ "iVisual",				&Settings::iVisual,					false,	0,		0,		1 },
		{ "iLanguage",				&Settings::iLanguage,				false,	0,		0,		1 },
	};

	const char kSection[] = "Settings";

	const Entry *Find(const char *name)
	{
		if (!name)
			return nullptr;
		for (const Entry &e : kTable)
		{
			if (_stricmp(e.name, name) == 0)
				return &e;
		}
		return nullptr;
	}

	int Clamp(const Entry &e, int v)
	{
		if (v < e.minVal) return e.minVal;
		if (v > e.maxVal) return e.maxVal;
		return v;
	}

	int Read(const Entry &e)
	{
		return e.isBool ? (*static_cast<bool *>(e.ptr) ? 1 : 0) : *static_cast<int *>(e.ptr);
	}

	void Write(const Entry &e, int v)
	{
		v = Clamp(e, v);
		if (e.isBool)
			*static_cast<bool *>(e.ptr) = (v != 0);
		else
			*static_cast<int *>(e.ptr) = v;
	}

	// BetterLooting.ini fica ao lado da DLL (funciona tambem com o MO2)
	const char *IniPath()
	{
		static char path[MAX_PATH] = "";
		if (!path[0])
		{
			HMODULE hm = nullptr;
			if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				(LPCSTR)&IniPath, &hm) && hm && GetModuleFileNameA(hm, path, MAX_PATH))
			{
				char *dot = strrchr(path, '.');
				if (dot)
					strcpy_s(dot, (size_t)(path + MAX_PATH - dot), ".ini");
				else
					strcat_s(path, MAX_PATH, ".ini");
			}
			else
			{
				strcpy_s(path, MAX_PATH, "Data\\SKSE\\Plugins\\BetterLooting.ini");
			}
		}
		return path;
	}
}


bool Settings::Get(const char *name, int &out)
{
	const Entry *e = Find(name);
	if (!e)
		return false;
	out = Read(*e);
	return true;
}


bool Settings::GetDefault(const char *name, int &out)
{
	const Entry *e = Find(name);
	if (!e)
		return false;
	out = e->def;
	return true;
}


bool Settings::Set(const char *name, int val)
{
	const Entry *e = Find(name);
	if (!e)
		return false;
	Write(*e, val);
	_MESSAGE("  %s = %d", e->name, Read(*e));
	return true;
}


bool Settings::SetAndSave(const char *name, int val)
{
	const Entry *e = Find(name);
	if (!e)
		return false;

	Write(*e, val);

	char buf[16];
	sprintf_s(buf, "%d", Read(*e));
	if (!WritePrivateProfileStringA(kSection, e->name, buf, IniPath()))
		_MESSAGE("AVISO: nao consegui gravar %s no ini (erro %lu)", e->name, GetLastError());

	return true;
}


void Settings::Load()
{
	const char *path = IniPath();
	_MESSAGE("Settings::Load() de %s", path);

	DWORD attr = GetFileAttributesA(path);
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		_MESSAGE("  AVISO: o ficheiro nao existe, uso os valores por omissao");
		return;
	}

	for (const Entry &e : kTable)
	{
		char buf[32] = "";
		GetPrivateProfileStringA(kSection, e.name, "", buf, sizeof(buf), path);
		if (buf[0])
			Write(e, atoi(buf));
		else
			Write(e, e.def);
		_MESSAGE("  %s = %d", e.name, Read(e));
	}
	_MESSAGE("");
}
