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
