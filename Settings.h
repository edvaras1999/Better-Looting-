#pragma once

// Definicoes do Better Looting. Vem do BetterLooting.ini (ao lado da DLL) e podem ser
// alteradas no MCM, que grava no mesmo ficheiro. Os nomes comecam por b (0/1) ou i (inteiro).
struct Settings
{
	// comportamento
	static bool	bDisableInCombat;
	static bool	bDisableTheft;
	static bool	bDisablePickpocketing;
	static bool	bDisableIfEmpty;
	static bool	bCloseIfEmpty;
	static bool	bUseConsole;
	static bool	bForceAnsi;
	static bool	bDisableLootSingle;

	// ordenacao da lista
	static bool	bSortByCategory;		// agrupa por categoria (ordem do QuickLootIE)
	static bool	bSortDescending;		// atributo em ordem descendente
	static bool	bSortWeightlessFirst;	// itens sem peso primeiro, dentro da categoria
	static int	iSortBy;				// 0 = nome, 1 = valor, 2 = peso, 3 = valor por peso

	// colunas e icones
	static bool	bShowValue;
	static bool	bShowWeight;
	static bool	bShowValuePerWeight;
	static bool	bShowItemIcons;

	// janela
	static int	iScale;
	static int	iItemLimit;		// maximo de itens na lista (0 = sem limite)
	static int	iMaxLines;		// linhas visiveis da lista
	static int	iMinLines;
	static int	iAnchorX;
	static int	iAnchorY;
	static int	iOffsetX;
	static int	iOffsetY;
	static int	iAlphaNormal;
	static int	iAlphaEmpty;

	// teclas (codigos DirectInput)
	static int	iKeyTakeAll;
	static int	iKeySearch;
	static int	iSearchHoldMs;

	// visual: 0 = Redesigned, 1 = QuickLootIE original
	static int	iVisual;
	// idioma do MCM: 0 = English, 1 = Portugues
	static int	iLanguage;

	static bool Get(const char *name, int &out);			// valor atual
	static bool GetDefault(const char *name, int &out);		// valor por omissao
	static bool Set(const char *name, int val);				// so em memoria (consola)
	static bool SetAndSave(const char *name, int val);		// memoria + BetterLooting.ini

	static void Load();
};
