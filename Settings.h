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

	// realce de objetos (Highlight)
	static bool	bHLEnabled;
	static bool	bHLRedStealable;	// o que seria roubo fica a vermelho
	static bool	bHLIgnoreUnnamed;
	static int	iHLColor;			// 0 dourado, 1 branco, 2 verde, 3 ciano, 4 vermelho
	static int	iHLCrosshairMs;		// duracao do realce na mira
	static bool	bHLClassicPalette;	// brilho branco classico (paleta do shader original)
	static int	iHLFillPercent;		// forca do brilho de preenchimento (0 a 100)
	static bool	bHLDebug;			// regista no log o que o realce decide para cada objeto na mira

	// varrimento por raio (tecla premida) e cadaveres lootaveis
	static bool	bHLScanEnabled;
	static int	iHLScanKey;			// codigo DirectInput (E = 18)
	static int	iHLScanHoldMs;		// tempo a manter a tecla premida
	static int	iHLScanRadius;		// raio, em unidades do jogo
	static int	iHLScanMs;			// duracao do brilho do varrimento
	static int	iHLScanMax;			// maximo de objetos realcados por varrimento
	static bool	bHLCorpses;			// o varrimento inclui cadaveres com loot
	static bool	bHLCorpseHideSearched;
	static int	iHLCorpseColor;
	static int	iHLCorpseMaxDist;
	static bool	bHLActivator;
	static bool	bHLTalkingActivator;
	static bool	bHLContainer;
	static bool	bHLDoor;
	static bool	bHLFlora;
	static bool	bHLFurniture;
	static bool	bHLArmor;
	static bool	bHLWeapon;
	static bool	bHLAmmo;
	static bool	bHLBook;
	static bool	bHLIngredient;
	static bool	bHLKey;
	static bool	bHLMisc;
	static bool	bHLPotion;
	static bool	bHLScroll;
	static bool	bHLSoulGem;

	static bool Get(const char *name, int &out);			// valor atual
	static bool GetDefault(const char *name, int &out);		// valor por omissao
	static bool Set(const char *name, int val);				// so em memoria (consola)
	static bool SetAndSave(const char *name, int val);		// memoria + BetterLooting.ini

	static void Load();
};
