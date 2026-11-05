Scriptname BetterLootingMCM extends SKI_ConfigBase
{ Menu MCM do Better Looting (SkyUI). Textos em English e Portugues (sem acentos, por causa das fontes do jogo).
  Todas as opcoes gravam no BetterLooting.ini atraves da DLL; nada fica guardado no save. }

int lang = 0

; ---------------- interruptores ----------------
string[] tKey
string[] tEN
string[] tPT
int[] tOpt

; ---------------- sliders ----------------
string[] sKey
string[] sEN
string[] sPT
float[] sMin
float[] sMax
float[] sStep
string[] sFmt
int[] sOpt

; ---------------- menus e teclas ----------------
int oLang
int oVisual
int oAnchorX
int oAnchorY
int oSortBy
int oHLColor
int oHLCorpseColor
int oScanKey
int oKeyAll
int oKeySearch

int Function GetVersion()
	return 1
EndFunction

Event OnConfigInit()
	ModName = "Better Looting"
	BuildPages()
	BuildTables()
EndEvent

Event OnVersionUpdate(int a_version)
	BuildPages()
	BuildTables()
EndEvent

Function BuildPages()
	Pages = new string[4]
	Pages[0] = "General / Geral"
	Pages[1] = "Window / Janela"
	Pages[2] = "Keys / Teclas"
	Pages[3] = "Highlight / Realce"
EndFunction

string Function L(string en, string pt)
	if lang == 1
		return pt
	endif
	return en
EndFunction

Function BuildTables()
	tKey = new string[35]
	tEN = new string[35]
	tPT = new string[35]
	tOpt = new int[35]
	tKey[0] = "bDisableInCombat"
	tEN[0] = "Disable in combat"
	tPT[0] = "Desativar em combate"
	tKey[1] = "bDisableTheft"
	tEN[1] = "Disable when stealing"
	tPT[1] = "Desativar em roubo"
	tKey[2] = "bDisablePickpocketing"
	tEN[2] = "Disable when pickpocketing"
	tPT[2] = "Desativar em pickpocket"
	tKey[3] = "bDisableIfEmpty"
	tEN[3] = "Disable if the container is empty"
	tPT[3] = "Desativar se o contentor estiver vazio"
	tKey[4] = "bCloseIfEmpty"
	tEN[4] = "Close when it becomes empty"
	tPT[4] = "Fechar quando ficar vazio"
	tKey[5] = "bDisableLootSingle"
	tEN[5] = "Take whole stack (ignore Left Shift)"
	tPT[5] = "Pegar a pilha toda (ignorar Shift esquerdo)"
	tKey[6] = "bShowValue"
	tEN[6] = "Show value column"
	tPT[6] = "Mostrar coluna de valor"
	tKey[7] = "bShowWeight"
	tEN[7] = "Show weight column"
	tPT[7] = "Mostrar coluna de peso"
	tKey[8] = "bShowValuePerWeight"
	tEN[8] = "Show value per weight column"
	tPT[8] = "Mostrar coluna de valor por peso"
	tKey[9] = "bShowItemIcons"
	tEN[9] = "Show item icons"
	tPT[9] = "Mostrar icones dos itens"
	tKey[10] = "bSortByCategory"
	tEN[10] = "Group by category (gold, gems, potions, weapons...)"
	tPT[10] = "Agrupar por categoria (ouro, gemas, pocoes, armas...)"
	tKey[11] = "bSortDescending"
	tEN[11] = "Descending order"
	tPT[11] = "Ordem descendente"
	tKey[12] = "bSortWeightlessFirst"
	tEN[12] = "Weightless items first"
	tPT[12] = "Itens sem peso primeiro"
	tKey[13] = "bHLEnabled"
	tEN[13] = "Highlight objects under the crosshair"
	tPT[13] = "Realcar objetos na mira"
	tKey[14] = "bHLRedStealable"
	tEN[14] = "Red highlight for what would be stealing"
	tPT[14] = "Realce vermelho para o que seria roubo"
	tKey[15] = "bHLIgnoreUnnamed"
	tEN[15] = "Ignore objects without a name"
	tPT[15] = "Ignorar objetos sem nome"
	tKey[16] = "bHLActivator"
	tEN[16] = "Activators"
	tPT[16] = "Ativadores"
	tKey[17] = "bHLTalkingActivator"
	tEN[17] = "Talking activators"
	tPT[17] = "Ativadores falantes"
	tKey[18] = "bHLContainer"
	tEN[18] = "Containers"
	tPT[18] = "Contentores"
	tKey[19] = "bHLDoor"
	tEN[19] = "Doors"
	tPT[19] = "Portas"
	tKey[20] = "bHLFlora"
	tEN[20] = "Flora and trees"
	tPT[20] = "Flora e arvores"
	tKey[21] = "bHLFurniture"
	tEN[21] = "Furniture"
	tPT[21] = "Mobilia"
	tKey[22] = "bHLArmor"
	tEN[22] = "Armor and clothes"
	tPT[22] = "Armaduras e roupas"
	tKey[23] = "bHLWeapon"
	tEN[23] = "Weapons"
	tPT[23] = "Armas"
	tKey[24] = "bHLAmmo"
	tEN[24] = "Ammo"
	tPT[24] = "Municao"
	tKey[25] = "bHLBook"
	tEN[25] = "Books"
	tPT[25] = "Livros"
	tKey[26] = "bHLIngredient"
	tEN[26] = "Ingredients"
	tPT[26] = "Ingredientes"
	tKey[27] = "bHLKey"
	tEN[27] = "Keys"
	tPT[27] = "Chaves"
	tKey[28] = "bHLMisc"
	tEN[28] = "Misc items"
	tPT[28] = "Itens diversos"
	tKey[29] = "bHLPotion"
	tEN[29] = "Potions"
	tPT[29] = "Pocoes"
	tKey[30] = "bHLScroll"
	tEN[30] = "Scrolls"
	tPT[30] = "Pergaminhos"
	tKey[31] = "bHLSoulGem"
	tEN[31] = "Soul gems"
	tPT[31] = "Gemas de alma"
	tKey[32] = "bHLScanEnabled"
	tEN[32] = "Area scan (hold the key)"
	tPT[32] = "Varrimento por raio (manter a tecla)"
	tKey[33] = "bHLCorpses"
	tEN[33] = "Scan also highlights lootable corpses"
	tPT[33] = "O varrimento realca tambem cadaveres com loot"
	tKey[34] = "bHLCorpseHideSearched"
	tEN[34] = "Hide corpses already searched"
	tPT[34] = "Esconder cadaveres ja revistados"

	sKey = new string[15]
	sEN = new string[15]
	sPT = new string[15]
	sMin = new float[15]
	sMax = new float[15]
	sStep = new float[15]
	sFmt = new string[15]
	sOpt = new int[15]
	sKey[0] = "iScale"
	sEN[0] = "Window scale"
	sPT[0] = "Escala da janela"
	sMin[0] = 25.0
	sMax[0] = 400.0
	sStep[0] = 5.0
	sFmt[0] = "{0}%"
	sKey[1] = "iMaxLines"
	sEN[1] = "Visible lines (maximum)"
	sPT[1] = "Linhas visiveis (maximo)"
	sMin[1] = 1.0
	sMax[1] = 25.0
	sStep[1] = 1.0
	sFmt[1] = "{0}"
	sKey[2] = "iMinLines"
	sEN[2] = "Visible lines (minimum)"
	sPT[2] = "Linhas visiveis (minimo)"
	sMin[2] = 0.0
	sMax[2] = 25.0
	sStep[2] = 1.0
	sFmt[2] = "{0}"
	sKey[3] = "iItemLimit"
	sEN[3] = "Maximum items in the list (0 = no limit)"
	sPT[3] = "Maximo de itens na lista (0 = sem limite)"
	sMin[3] = 0.0
	sMax[3] = 200.0
	sStep[3] = 1.0
	sFmt[3] = "{0}"
	sKey[4] = "iOffsetX"
	sEN[4] = "Horizontal offset"
	sPT[4] = "Deslocamento horizontal"
	sMin[4] = -1000.0
	sMax[4] = 1000.0
	sStep[4] = 10.0
	sFmt[4] = "{0} px"
	sKey[5] = "iOffsetY"
	sEN[5] = "Vertical offset"
	sPT[5] = "Deslocamento vertical"
	sMin[5] = -1000.0
	sMax[5] = 1000.0
	sStep[5] = 10.0
	sFmt[5] = "{0} px"
	sKey[6] = "iAlphaNormal"
	sEN[6] = "Opacity"
	sPT[6] = "Opacidade"
	sMin[6] = 10.0
	sMax[6] = 100.0
	sStep[6] = 5.0
	sFmt[6] = "{0}%"
	sKey[7] = "iAlphaEmpty"
	sEN[7] = "Opacity when empty"
	sPT[7] = "Opacidade quando vazio"
	sMin[7] = 0.0
	sMax[7] = 100.0
	sStep[7] = 5.0
	sFmt[7] = "{0}%"
	sKey[8] = "iSearchHoldMs"
	sEN[8] = "Search: time to hold the key (0 = one tap)"
	sPT[8] = "Procurar: tempo a manter a tecla (0 = um toque)"
	sMin[8] = 0.0
	sMax[8] = 1000.0
	sStep[8] = 50.0
	sFmt[8] = "{0} ms"
	sKey[9] = "iHLCrosshairMs"
	sEN[9] = "Crosshair highlight duration"
	sPT[9] = "Duracao do realce na mira"
	sMin[9] = 100.0
	sMax[9] = 10000.0
	sStep[9] = 100.0
	sFmt[9] = "{0} ms"
	sKey[10] = "iHLScanHoldMs"
	sEN[10] = "Scan: time to hold the key (0 = one tap)"
	sPT[10] = "Varrimento: tempo a manter a tecla (0 = um toque)"
	sMin[10] = 0.0
	sMax[10] = 2000.0
	sStep[10] = 50.0
	sFmt[10] = "{0} ms"
	sKey[11] = "iHLScanRadius"
	sEN[11] = "Scan radius"
	sPT[11] = "Raio do varrimento"
	sMin[11] = 300.0
	sMax[11] = 5000.0
	sStep[11] = 100.0
	sFmt[11] = "{0}"
	sKey[12] = "iHLScanMs"
	sEN[12] = "Scan glow duration"
	sPT[12] = "Duracao do brilho do varrimento"
	sMin[12] = 500.0
	sMax[12] = 10000.0
	sStep[12] = 500.0
	sFmt[12] = "{0} ms"
	sKey[13] = "iHLScanMax"
	sEN[13] = "Maximum objects per scan"
	sPT[13] = "Maximo de objetos por varrimento"
	sMin[13] = 8.0
	sMax[13] = 128.0
	sStep[13] = 8.0
	sFmt[13] = "{0}"
	sKey[14] = "iHLCorpseMaxDist"
	sEN[14] = "Corpse highlight distance"
	sPT[14] = "Distancia do realce de cadaveres"
	sMin[14] = 500.0
	sMax[14] = 5000.0
	sStep[14] = 100.0
	sFmt[14] = "{0}"

	; ids de opcoes por preencher: -1 (o id 0 e valido, por isso nao serve de "vazio")
	int n = 0
	while n < tOpt.Length
		tOpt[n] = -1
		n += 1
	endwhile
	n = 0
	while n < sOpt.Length
		sOpt[n] = -1
		n += 1
	endwhile
	oLang = -1
	oVisual = -1
	oAnchorX = -1
	oAnchorY = -1
	oSortBy = -1
	oHLColor = -1
	oHLCorpseColor = -1
	oScanKey = -1
	oKeyAll = -1
	oKeySearch = -1
EndFunction

string Function VisualName(int v)
	if v == 1
		return L("Original (QuickLootIE)", "Original (QuickLootIE)")
	endif
	return "Redesigned"
EndFunction

string Function LangName(int v)
	if v == 1
		return "Portugues"
	endif
	return "English"
EndFunction

string Function ColorName(int v)
	if v == 1
		return L("White", "Branco")
	elseif v == 2
		return L("Green", "Verde")
	elseif v == 3
		return L("Cyan", "Ciano")
	elseif v == 4
		return L("Red", "Vermelho")
	endif
	return L("Gold", "Dourado")
EndFunction

string Function SortByName(int v)
	if v == 1
		return L("Value", "Valor")
	elseif v == 2
		return L("Weight", "Peso")
	elseif v == 3
		return L("Value per weight", "Valor por peso")
	endif
	return L("Name", "Nome")
EndFunction

string Function AnchorXName(int v)
	if v == 1
		return L("Center", "Centro")
	elseif v == 2
		return L("Right", "Direita")
	endif
	return L("Left", "Esquerda")
EndFunction

string Function AnchorYName(int v)
	if v == 1
		return L("Center", "Centro")
	elseif v == 2
		return L("Bottom", "Base")
	endif
	return L("Top", "Topo")
EndFunction

Function AddToggleRange(int first, int last)
	int i = first
	while i <= last
		tOpt[i] = AddToggleOption(L(tEN[i], tPT[i]), BetterLootingNative.GetInt(tKey[i], 0) != 0)
		i += 1
	endwhile
EndFunction

Function AddSliderRange(int first, int last)
	int i = first
	while i <= last
		sOpt[i] = AddSliderOption(L(sEN[i], sPT[i]), BetterLootingNative.GetInt(sKey[i], 0) as float, sFmt[i])
		i += 1
	endwhile
EndFunction

Event OnPageReset(string a_page)
	BuildTables()
	lang = BetterLootingNative.GetInt("iLanguage", 0)
	SetCursorFillMode(TOP_TO_BOTTOM)

	if a_page == Pages[0]
		AddHeaderOption(L("Language and look", "Idioma e visual"))
		oLang = AddMenuOption(L("Language", "Idioma"), LangName(lang))
		oVisual = AddMenuOption(L("Loot window look", "Visual da janela de loot"), VisualName(BetterLootingNative.GetInt("iVisual", 0)))
		AddHeaderOption(L("Behavior", "Comportamento"))
		AddToggleRange(0, 5)
		AddHeaderOption(L("Sorting", "Ordenacao"))
		AddToggleRange(10, 10)
		oSortBy = AddMenuOption(L("Sort by", "Ordenar por"), SortByName(BetterLootingNative.GetInt("iSortBy", 0)))
		AddToggleRange(11, 12)
	elseif a_page == Pages[1]
		AddHeaderOption(L("Size and lines", "Tamanho e linhas"))
		AddSliderRange(0, 3)
		AddHeaderOption(L("Position", "Posicao"))
		oAnchorX = AddMenuOption(L("Horizontal anchor", "Ancora horizontal"), AnchorXName(BetterLootingNative.GetInt("iAnchorX", 0)))
		oAnchorY = AddMenuOption(L("Vertical anchor", "Ancora vertical"), AnchorYName(BetterLootingNative.GetInt("iAnchorY", 0)))
		AddSliderRange(4, 5)
		AddHeaderOption(L("Opacity", "Opacidade"))
		AddSliderRange(6, 7)
		AddHeaderOption(L("Columns and icons", "Colunas e icones"))
		AddToggleRange(6, 9)
	elseif a_page == Pages[3]
		AddHeaderOption(L("Crosshair highlight", "Realce na mira"))
		AddToggleRange(13, 13)
		oHLColor = AddMenuOption(L("Highlight color", "Cor do realce"), ColorName(BetterLootingNative.GetInt("iHLColor", 0)))
		AddToggleRange(14, 15)
		sOpt[9] = AddSliderOption(L(sEN[9], sPT[9]), BetterLootingNative.GetInt(sKey[9], 800) as float, sFmt[9])
		AddHeaderOption(L("Object types", "Tipos de objeto"))
		AddToggleRange(16, 31)
		AddHeaderOption(L("Area scan", "Varrimento por raio"))
		AddToggleRange(32, 32)
		oScanKey = AddKeyMapOption(L("Scan key (hold)", "Tecla do varrimento (manter)"), BetterLootingNative.GetInt("iHLScanKey", 18))
		AddSliderRange(10, 13)
		AddHeaderOption(L("Lootable corpses", "Cadaveres com loot"))
		AddToggleRange(33, 34)
		oHLCorpseColor = AddMenuOption(L("Corpse color", "Cor dos cadaveres"), ColorName(BetterLootingNative.GetInt("iHLCorpseColor", 2)))
		AddSliderRange(14, 14)
	elseif a_page == Pages[2]
		AddHeaderOption(L("Keys (loot menu open)", "Teclas (menu de loot aberto)"))
		oKeyAll = AddKeyMapOption(L("Take all", "Pegar tudo"), BetterLootingNative.GetInt("iKeyTakeAll", 46))
		oKeySearch = AddKeyMapOption(L("Search (open the container)", "Procurar (abrir o contentor)"), BetterLootingNative.GetInt("iKeySearch", 19))
		sOpt[8] = AddSliderOption(L(sEN[8], sPT[8]), BetterLootingNative.GetInt(sKey[8], 0) as float, sFmt[8])
	endif
EndEvent

int Function FindToggle(int a_option)
	int i = 0
	while i < tOpt.Length
		if tOpt[i] == a_option
			return i
		endif
		i += 1
	endwhile
	return -1
EndFunction

int Function FindSlider(int a_option)
	int i = 0
	while i < sOpt.Length
		if sOpt[i] == a_option
			return i
		endif
		i += 1
	endwhile
	return -1
EndFunction

Event OnOptionSelect(int a_option)
	int i = FindToggle(a_option)
	if i >= 0
		int v = 1
		if BetterLootingNative.GetInt(tKey[i], 0) != 0
			v = 0
		endif
		BetterLootingNative.SetInt(tKey[i], v)
		SetToggleOptionValue(a_option, v != 0)
	endif
EndEvent

Event OnOptionDefault(int a_option)
	int i = FindToggle(a_option)
	if i >= 0
		int d = BetterLootingNative.GetDefault(tKey[i], 0)
		BetterLootingNative.SetInt(tKey[i], d)
		SetToggleOptionValue(a_option, d != 0)
		return
	endif
	i = FindSlider(a_option)
	if i >= 0
		int ds = BetterLootingNative.GetDefault(sKey[i], 0)
		BetterLootingNative.SetInt(sKey[i], ds)
		SetSliderOptionValue(a_option, ds as float, sFmt[i])
		return
	endif
	if a_option == oKeyAll
		SetKey("iKeyTakeAll", a_option, 0)
	elseif a_option == oKeySearch
		SetKey("iKeySearch", a_option, 0)
	elseif a_option == oAnchorX
		BetterLootingNative.SetInt("iAnchorX", 0)
		SetMenuOptionValue(a_option, AnchorXName(0))
	elseif a_option == oAnchorY
		BetterLootingNative.SetInt("iAnchorY", 0)
		SetMenuOptionValue(a_option, AnchorYName(0))
	elseif a_option == oSortBy
		BetterLootingNative.SetInt("iSortBy", 0)
		SetMenuOptionValue(a_option, SortByName(0))
	elseif a_option == oHLColor
		BetterLootingNative.SetInt("iHLColor", 0)
		SetMenuOptionValue(a_option, ColorName(0))
	elseif a_option == oHLCorpseColor
		BetterLootingNative.SetInt("iHLCorpseColor", 2)
		SetMenuOptionValue(a_option, ColorName(2))
	elseif a_option == oScanKey
		SetKey("iHLScanKey", a_option, 0)
	elseif a_option == oVisual
		BetterLootingNative.SetInt("iVisual", 0)
		BetterLootingNative.ApplyVisual()
		SetMenuOptionValue(a_option, VisualName(0))
	elseif a_option == oLang
		BetterLootingNative.SetInt("iLanguage", 0)
		lang = 0
		ForcePageReset()
	endif
EndEvent

Event OnOptionSliderOpen(int a_option)
	int i = FindSlider(a_option)
	if i >= 0
		SetSliderDialogStartValue(BetterLootingNative.GetInt(sKey[i], 0) as float)
		SetSliderDialogDefaultValue(BetterLootingNative.GetDefault(sKey[i], 0) as float)
		SetSliderDialogRange(sMin[i], sMax[i])
		SetSliderDialogInterval(sStep[i])
	endif
EndEvent

Event OnOptionSliderAccept(int a_option, float a_value)
	int i = FindSlider(a_option)
	if i >= 0
		BetterLootingNative.SetInt(sKey[i], a_value as int)
		SetSliderOptionValue(a_option, a_value, sFmt[i])
	endif
EndEvent

Event OnOptionMenuOpen(int a_option)
	string[] opts
	int cur = 0
	int dflt = 0
	if a_option == oLang
		opts = new string[2]
		opts[0] = "English"
		opts[1] = "Portugues"
		cur = lang
	elseif a_option == oVisual
		opts = new string[2]
		opts[0] = VisualName(0)
		opts[1] = VisualName(1)
		cur = BetterLootingNative.GetInt("iVisual", 0)
	elseif a_option == oAnchorX
		opts = new string[3]
		opts[0] = AnchorXName(0)
		opts[1] = AnchorXName(1)
		opts[2] = AnchorXName(2)
		cur = BetterLootingNative.GetInt("iAnchorX", 0)
	elseif a_option == oAnchorY
		opts = new string[3]
		opts[0] = AnchorYName(0)
		opts[1] = AnchorYName(1)
		opts[2] = AnchorYName(2)
		cur = BetterLootingNative.GetInt("iAnchorY", 0)
	elseif a_option == oHLCorpseColor
		opts = new string[5]
		opts[0] = ColorName(0)
		opts[1] = ColorName(1)
		opts[2] = ColorName(2)
		opts[3] = ColorName(3)
		opts[4] = ColorName(4)
		cur = BetterLootingNative.GetInt("iHLCorpseColor", 2)
		dflt = 2
	elseif a_option == oHLColor
		opts = new string[5]
		opts[0] = ColorName(0)
		opts[1] = ColorName(1)
		opts[2] = ColorName(2)
		opts[3] = ColorName(3)
		opts[4] = ColorName(4)
		cur = BetterLootingNative.GetInt("iHLColor", 0)
	elseif a_option == oSortBy
		opts = new string[4]
		opts[0] = SortByName(0)
		opts[1] = SortByName(1)
		opts[2] = SortByName(2)
		opts[3] = SortByName(3)
		cur = BetterLootingNative.GetInt("iSortBy", 0)
	else
		return
	endif
	SetMenuDialogOptions(opts)
	SetMenuDialogStartIndex(cur)
	SetMenuDialogDefaultIndex(dflt)
EndEvent

Event OnOptionMenuAccept(int a_option, int a_index)
	if a_index < 0
		return
	endif
	if a_option == oLang
		BetterLootingNative.SetInt("iLanguage", a_index)
		lang = a_index
		ForcePageReset()
	elseif a_option == oVisual
		BetterLootingNative.SetInt("iVisual", a_index)
		BetterLootingNative.ApplyVisual()
		SetMenuOptionValue(a_option, VisualName(a_index))
	elseif a_option == oAnchorX
		BetterLootingNative.SetInt("iAnchorX", a_index)
		SetMenuOptionValue(a_option, AnchorXName(a_index))
	elseif a_option == oAnchorY
		BetterLootingNative.SetInt("iAnchorY", a_index)
		SetMenuOptionValue(a_option, AnchorYName(a_index))
	elseif a_option == oHLCorpseColor
		BetterLootingNative.SetInt("iHLCorpseColor", a_index)
		SetMenuOptionValue(a_option, ColorName(a_index))
	elseif a_option == oHLColor
		BetterLootingNative.SetInt("iHLColor", a_index)
		SetMenuOptionValue(a_option, ColorName(a_index))
	elseif a_option == oSortBy
		BetterLootingNative.SetInt("iSortBy", a_index)
		SetMenuOptionValue(a_option, SortByName(a_index))
	endif
EndEvent

Function SetKey(string a_key, int a_option, int a_code)
	int code = a_code
	if code < 1
		code = BetterLootingNative.GetDefault(a_key, 1)
	endif
	BetterLootingNative.SetInt(a_key, code)
	SetKeyMapOptionValue(a_option, code)
EndFunction

Event OnOptionKeyMapChange(int a_option, int a_keyCode, string a_conflictControl, string a_conflictName)
	if a_option == oKeyAll
		SetKey("iKeyTakeAll", a_option, a_keyCode)
	elseif a_option == oKeySearch
		SetKey("iKeySearch", a_option, a_keyCode)
	elseif a_option == oScanKey
		SetKey("iHLScanKey", a_option, a_keyCode)
	endif
EndEvent

Event OnOptionHighlight(int a_option)
	if a_option == oVisual
		SetInfoText(L("Look of the loot window. The change applies the next time the loot menu opens.", "Visual da janela de loot. A mudanca aplica-se na proxima vez que o menu de loot abrir."))
	elseif a_option == oLang
		SetInfoText(L("Language of this menu.", "Idioma deste menu."))
	elseif a_option == oKeyAll
		SetInfoText(L("Takes every item. While the loot menu is open this key does nothing else.", "Pega todos os itens. Com o menu de loot aberto esta tecla nao faz mais nada."))
	elseif a_option == oKeySearch
		SetInfoText(L("Opens the container normally. While the loot menu is open this key does nothing else.", "Abre o contentor normalmente. Com o menu de loot aberto esta tecla nao faz mais nada."))
	elseif a_option == oAnchorX
		SetInfoText(L("Where on the screen the window is anchored, horizontally.", "Ponto do ecra onde a janela fica ancorada, na horizontal."))
	elseif a_option == oAnchorY
		SetInfoText(L("Where on the screen the window is anchored, vertically.", "Ponto do ecra onde a janela fica ancorada, na vertical."))
	elseif a_option == oScanKey
		SetInfoText(L("Hold this key to light up nearby objects and lootable corpses. Holding longer keeps refreshing the glow. A short tap still does its normal action.", "Mantenha esta tecla premida para realcar objetos e cadaveres com loot por perto. Manter mais tempo continua a refrescar o brilho. Um toque curto faz a acao normal."))
	elseif a_option == oHLCorpseColor
		SetInfoText(L("Color of the glow on lootable corpses.", "Cor do brilho nos cadaveres com loot."))
	elseif a_option == oHLColor
		SetInfoText(L("Color of the glow on the object you look at. What would be stealing uses red if that option is on.", "Cor do brilho no objeto para que olhas. O que seria roubo usa vermelho, se essa opcao estiver ligada."))
	elseif a_option == oSortBy
		SetInfoText(L("Order of the items inside each category (or of the whole list if grouping is off). Quest items always come first.", "Ordem dos itens dentro de cada categoria (ou de toda a lista, se o agrupamento estiver desligado). Itens de missao ficam sempre primeiro."))
	elseif a_option == sOpt[3]
		SetInfoText(L("Stops the list after this many items. 0 means all items are listed.", "Para a lista neste numero de itens. 0 lista todos os itens."))
	endif
EndEvent
