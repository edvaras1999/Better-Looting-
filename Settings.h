#pragma once

struct Settings
{
	static bool	bDisableInCombat;
	static bool	bDisableTheft;
	static bool	bDisablePickpocketing;
	static bool	bDisableIfEmpty;
	static bool	bCloseIfEmpty;
	static bool bUseConsole;
	static bool bForceAnsi;
	static bool bDisableLootSingle;
	static bool bShowValue;
	static bool bShowWeight;
	static bool bShowValuePerWeight;
	static bool bShowItemIcons;
	static int	iOpacity;
	static int	iScale;
	static int	iPositionX;
	static int	iPositionY;
	static int	iItemLimit;
	static int	iKeyTakeAll;
	static int	iKeySearch;
	static int	iSearchHoldMs;
	static int	iAnchorX;
	static int	iAnchorY;
	static int	iOffsetX;
	static int	iOffsetY;
	static int	iMinLines;
	static int	iAlphaNormal;
	static int	iAlphaEmpty;

	static bool SetBool(const char *name, bool val);
	static bool SetInt(const char *name, int val);
	static bool Set(const char *name, int val);
	static void Eval(const char *line);

	static void Load();
};
