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

; ---------------- teclas (tecla + modificador) ----------------
string[] aKey
string[] aMod
string[] aEN
string[] aPT
int[] aOptKey
int[] aOptMod

; ---------------- menus ----------------
int oLang
int oVisual
int oAnchorX
int oAnchorY
int oSortBy
int oHLColor
int oHLCorpseColor

int Function GetVersion()
	return 2
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
	tKey = new string[34]
	tEN = new string[34]
	tPT = new string[34]
	tOpt = new int[34]
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
	tKey[5] = "bShowValue"
	tEN[5] = "Show value column"
	tPT[5] = "Mostrar coluna de valor"
	tKey[6] = "bShowWeight"
	tEN[6] = "Show weight column"
	tPT[6] = "Mostrar coluna de peso"
	tKey[7] = "bShowValuePerWeight"
	tEN[7] = "Show value per weight column"
	tPT[7] = "Mostrar coluna de valor por peso"
	tKey[8] = "bShowItemIcons"
	tEN[8] = "Show item icons"
	tPT[8] = "Mostrar icones dos itens"
	tKey[9] = "bSortByCategory"
	tEN[9] = "Group by category (gold, gems, potions, weapons...)"
	tPT[9] = "Agrupar por categoria (ouro, gemas, pocoes, armas...)"
	tKey[10] = "bSortDescending"
	tEN[10] = "Descending order"
	tPT[10] = "Ordem descendente"
	tKey[11] = "bSortWeightlessFirst"
	tEN[11] = "Weightless items first"
	tPT[11] = "Itens sem peso primeiro"
	tKey[12] = "bHLEnabled"
	tEN[12] = "Highlight objects under the crosshair"
	tPT[12] = "Realcar objetos na mira"
	tKey[13] = "bHLRedStealable"
	tEN[13] = "Red highlight for what would be stealing"
	tPT[13] = "Realce vermelho para o que seria roubo"
	tKey[14] = "bHLIgnoreUnnamed"
	tEN[14] = "Ignore objects without a name"
	tPT[14] = "Ignorar objetos sem nome"
	tKey[15] = "bHLActivator"
	tEN[15] = "Activators"
	tPT[15] = "Ativadores"
	tKey[16] = "bHLTalkingActivator"
	tEN[16] = "Talking activators"
	tPT[16] = "Ativadores falantes"
	tKey[17] = "bHLContainer"
	tEN[17] = "Containers"
	tPT[17] = "Contentores"
	tKey[18] = "bHLDoor"
	tEN[18] = "Doors"
	tPT[18] = "Portas"
	tKey[19] = "bHLFlora"
	tEN[19] = "Flora and trees"
	tPT[19] = "Flora e arvores"
	tKey[20] = "bHLFurniture"
	tEN[20] = "Furniture"
	tPT[20] = "Mobilia"
	tKey[21] = "bHLArmor"
	tEN[21] = "Armor and clothes"
	tPT[21] = "Armaduras e roupas"
	tKey[22] = "bHLWeapon"
	tEN[22] = "Weapons"
	tPT[22] = "Armas"
	tKey[23] = "bHLAmmo"
	tEN[23] = "Ammo"
	tPT[23] = "Municao"
	tKey[24] = "bHLBook"
	tEN[24] = "Books"
	tPT[24] = "Livros"
	tKey[25] = "bHLIngredient"
	tEN[25] = "Ingredients"
	tPT[25] = "Ingredientes"
	tKey[26] = "bHLKey"
	tEN[26] = "Keys"
	tPT[26] = "Chaves"
	tKey[27] = "bHLMisc"
	tEN[27] = "Misc items"
	tPT[27] = "Itens diversos"
	tKey[28] = "bHLPotion"
	tEN[28] = "Potions"
	tPT[28] = "Pocoes"
	tKey[29] = "bHLScroll"
	tEN[29] = "Scrolls"
	tPT[29] = "Pergaminhos"
	tKey[30] = "bHLSoulGem"
	tEN[30] = "Soul gems"
	tPT[30] = "Gemas de alma"
	tKey[31] = "bHLScanEnabled"
	tEN[31] = "Area scan (hold the key)"
	tPT[31] = "Varrimento por raio (manter a tecla)"
	tKey[32] = "bHLCorpses"
	tEN[32] = "Highlight lootable corpses (crosshair and scan)"
	tPT[32] = "Realcar cadaveres com loot (mira e varrimento)"
	tKey[33] = "bHLCorpseHideSearched"
	tEN[33] = "Hide corpses already searched"
	tPT[33] = "Esconder cadaveres ja revistados"
	sKey = new string[16]
	sEN = new string[16]
	sPT = new string[16]
	sMin = new float[16]
	sMax = new float[16]
	sStep = new float[16]
	sFmt = new string[16]
	sOpt = new int[16]
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
	sKey[15] = "iHLFillPercent"
	sEN[15] = "Glow strength"
	sPT[15] = "Intensidade do brilho"
	sMin[15] = 5.0
	sMax[15] = 100.0
	sStep[15] = 5.0
	sFmt[15] = "{0}%"
	aKey = new string[8]
	aMod = new string[8]
	aEN = new string[8]
	aPT = new string[8]
	aOptKey = new int[8]
	aOptMod = new int[8]
	aKey[0] = "iKeyTake"
	aMod[0] = "iModTake"
	aEN[0] = "Take"
	aPT[0] = "Pegar"
	aKey[1] = "iKeyTakeOne"
	aMod[1] = "iModTakeOne"
	aEN[1] = "Take only one"
	aPT[1] = "Pegar so um"
	aKey[2] = "iKeyUse"
	aMod[2] = "iModUse"
	aEN[2] = "Use / equip"
	aPT[2] = "Usar / equipar"
	aKey[3] = "iKeyTakeAll"
	aMod[3] = "iModTakeAll"
	aEN[3] = "Take all"
	aPT[3] = "Pegar tudo"
	aKey[4] = "iKeySearch"
	aMod[4] = "iModSearch"
	aEN[4] = "Search (open the container)"
	aPT[4] = "Procurar (abrir o contentor)"
	aKey[5] = "iKeyEquipRight"
	aMod[5] = "iModEquipRight"
	aEN[5] = "Equip in the right hand"
	aPT[5] = "Equipar na mao direita"
	aKey[6] = "iKeyEquipLeft"
	aMod[6] = "iModEquipLeft"
	aEN[6] = "Equip in the left hand"
	aPT[6] = "Equipar na mao esquerda"
	aKey[7] = "iHLScanKey"
	aMod[7] = ""
	aEN[7] = "Area scan key (hold)"
	aPT[7] = "Tecla do varrimento (manter)"

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
	n = 0
	while n < aOptKey.Length
		aOptKey[n] = -1
		aOptMod[n] = -1
		n += 1
	endwhile
	oLang = -1
	oVisual = -1
	oAnchorX = -1
	oAnchorY = -1
	oSortBy = -1
	oHLColor = -1
	oHLCorpseColor = -1
EndFunction

string Function VisualName(int v)
	if v == 1
		return "Original (QuickLootIE)"
	endif
	return "Redesigned"
EndFunction

string Function LangName(int v)
	if v == 1
		return "Portugues"
	endif
	return "English"
EndFunction

string Function ModName2(int v)
	if v == 1
		return "Shift"
	elseif v == 2
		return "Ctrl"
	elseif v == 3
		return "Alt"
	endif
	return L("None", "Nenhum")
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

Function AddKeyRow(int i)
	aOptKey[i] = AddKeyMapOption(L(aEN[i], aPT[i]), BetterLootingNative.GetInt(aKey[i], 18))
	if aMod[i] != ""
		aOptMod[i] = AddMenuOption(L(aEN[i], aPT[i]) + " - " + L("modifier", "modificador"), ModName2(BetterLootingNative.GetInt(aMod[i], 0)))
	else
		AddEmptyOption()
	endif
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
		AddToggleRange(0, 4)
		AddHeaderOption(L("Sorting", "Ordenacao"))
		AddToggleRange(9, 9)
		oSortBy = AddMenuOption(L("Sort by", "Ordenar por"), SortByName(BetterLootingNative.GetInt("iSortBy", 0)))
		AddToggleRange(10, 11)
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
		AddToggleRange(5, 8)
	elseif a_page == Pages[2]
		SetCursorFillMode(LEFT_TO_RIGHT)
		AddHeaderOption(L("Loot window keys", "Teclas da janela de loot"))
		AddHeaderOption(L("Modifiers", "Modificadores"))
		int i = 0
		while i < 7
			AddKeyRow(i)
			i += 1
		endwhile
		AddHeaderOption(L("Timing", "Tempos"))
		AddHeaderOption("")
		AddSliderRange(8, 8)
		AddEmptyOption()
		AddHeaderOption(L("Area scan (outside the loot window)", "Varrimento (fora da janela de loot)"))
		AddHeaderOption("")
		AddKeyRow(7)
		AddSliderRange(10, 10)
	elseif a_page == Pages[3]
		AddHeaderOption(L("Crosshair highlight", "Realce na mira"))
		AddToggleRange(12, 12)
		oHLColor = AddMenuOption(L("Highlight color", "Cor do realce"), ColorName(BetterLootingNative.GetInt("iHLColor", 0)))
		AddToggleRange(13, 14)
		AddSliderRange(9, 9)
		AddSliderRange(15, 15)
		AddHeaderOption(L("Object types", "Tipos de objeto"))
		AddToggleRange(15, 30)
		AddHeaderOption(L("Area scan", "Varrimento por raio"))
		AddToggleRange(31, 31)
		AddSliderRange(11, 13)
		AddHeaderOption(L("Lootable corpses", "Cadaveres com loot"))
		AddToggleRange(32, 33)
		oHLCorpseColor = AddMenuOption(L("Corpse color", "Cor dos cadaveres"), ColorName(BetterLootingNative.GetInt("iHLCorpseColor", 2)))
		AddSliderRange(14, 14)
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

int Function FindKey(int a_option)
	int i = 0
	while i < aOptKey.Length
		if aOptKey[i] == a_option
			return i
		endif
		i += 1
	endwhile
	return -1
EndFunction

int Function FindMod(int a_option)
	int i = 0
	while i < aOptMod.Length
		if aOptMod[i] == a_option
			return i
		endif
		i += 1
	endwhile
	return -1
EndFunction

Function SetKey(string a_key, int a_option, int a_code)
	int code = a_code
	if code < 1
		code = BetterLootingNative.GetDefault(a_key, 1)
	endif
	BetterLootingNative.SetInt(a_key, code)
	SetKeyMapOptionValue(a_option, code)
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
	i = FindKey(a_option)
	if i >= 0
		SetKey(aKey[i], a_option, 0)
		return
	endif
	i = FindMod(a_option)
	if i >= 0
		int dm = BetterLootingNative.GetDefault(aMod[i], 0)
		BetterLootingNative.SetInt(aMod[i], dm)
		SetMenuOptionValue(a_option, ModName2(dm))
		return
	endif
	if a_option == oAnchorX
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
	int k = FindMod(a_option)
	if k >= 0
		opts = new string[4]
		opts[0] = ModName2(0)
		opts[1] = ModName2(1)
		opts[2] = ModName2(2)
		opts[3] = ModName2(3)
		cur = BetterLootingNative.GetInt(aMod[k], 0)
		dflt = BetterLootingNative.GetDefault(aMod[k], 0)
	elseif a_option == oLang
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
	elseif a_option == oSortBy
		opts = new string[4]
		opts[0] = SortByName(0)
		opts[1] = SortByName(1)
		opts[2] = SortByName(2)
		opts[3] = SortByName(3)
		cur = BetterLootingNative.GetInt("iSortBy", 0)
	elseif a_option == oHLColor
		opts = new string[5]
		opts[0] = ColorName(0)
		opts[1] = ColorName(1)
		opts[2] = ColorName(2)
		opts[3] = ColorName(3)
		opts[4] = ColorName(4)
		cur = BetterLootingNative.GetInt("iHLColor", 0)
	elseif a_option == oHLCorpseColor
		opts = new string[5]
		opts[0] = ColorName(0)
		opts[1] = ColorName(1)
		opts[2] = ColorName(2)
		opts[3] = ColorName(3)
		opts[4] = ColorName(4)
		cur = BetterLootingNative.GetInt("iHLCorpseColor", 2)
		dflt = 2
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
	int k = FindMod(a_option)
	if k >= 0
		BetterLootingNative.SetInt(aMod[k], a_index)
		SetMenuOptionValue(a_option, ModName2(a_index))
	elseif a_option == oLang
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
	elseif a_option == oSortBy
		BetterLootingNative.SetInt("iSortBy", a_index)
		SetMenuOptionValue(a_option, SortByName(a_index))
	elseif a_option == oHLColor
		BetterLootingNative.SetInt("iHLColor", a_index)
		SetMenuOptionValue(a_option, ColorName(a_index))
	elseif a_option == oHLCorpseColor
		BetterLootingNative.SetInt("iHLCorpseColor", a_index)
		SetMenuOptionValue(a_option, ColorName(a_index))
	endif
EndEvent

Event OnOptionKeyMapChange(int a_option, int a_keyCode, string a_conflictControl, string a_conflictName)
	int i = FindKey(a_option)
	if i >= 0
		SetKey(aKey[i], a_option, a_keyCode)
	endif
EndEvent

Event OnOptionHighlight(int a_option)
	int i = FindKey(a_option)
	if i >= 0
		if i == 7
			SetInfoText(L("Hold this key to light up nearby objects and lootable corpses. Holding longer keeps refreshing the glow. A short tap still does its normal action.", "Mantenha esta tecla premida para realcar objetos e cadaveres com loot por perto. Manter mais tempo continua a refrescar o brilho. Um toque curto faz a acao normal."))
		else
			SetInfoText(L("Key of this action while the loot window is open. Keyboard keys and mouse buttons work. The game does not do anything else with this key while the window is open.", "Tecla desta acao com a janela de loot aberta. Funcionam teclas do teclado e botoes do rato. O jogo nao faz mais nada com esta tecla enquanto a janela esta aberta."))
		endif
		return
	endif
	i = FindMod(a_option)
	if i >= 0
		SetInfoText(L("Modifier that must be held together with the key (None, Shift, Ctrl or Alt).", "Modificador que tem de estar premido junto com a tecla (Nenhum, Shift, Ctrl ou Alt)."))
		return
	endif
	if a_option == oVisual
		SetInfoText(L("Look of the loot window. The change applies the next time the loot menu opens.", "Visual da janela de loot. A mudanca aplica-se na proxima vez que o menu de loot abrir."))
	elseif a_option == oLang
		SetInfoText(L("Language of this menu.", "Idioma deste menu."))
	elseif a_option == oAnchorX
		SetInfoText(L("Where on the screen the window is anchored, horizontally.", "Ponto do ecra onde a janela fica ancorada, na horizontal."))
	elseif a_option == oAnchorY
		SetInfoText(L("Where on the screen the window is anchored, vertically.", "Ponto do ecra onde a janela fica ancorada, na vertical."))
	elseif a_option == oHLCorpseColor
		SetInfoText(L("Color of the glow on lootable corpses.", "Cor do brilho nos cadaveres com loot."))
	elseif a_option == oHLColor
		SetInfoText(L("Color of the glow on the object you look at. What would be stealing uses red if that option is on.", "Cor do brilho no objeto para que olhas. O que seria roubo usa vermelho, se essa opcao estiver ligada."))
	elseif a_option == oSortBy
		SetInfoText(L("Order of the items inside each category (or of the whole list if grouping is off). Quest items always come first.", "Ordem dos itens dentro de cada categoria (ou de toda a lista, se o agrupamento estiver desligado). Itens de missao ficam sempre primeiro."))
	elseif a_option == sOpt[3]
		SetInfoText(L("Stops the list after this many items. 0 means all items are listed.", "Para a lista neste numero de itens. 0 lista todos os itens."))
	elseif a_option == sOpt[15]
		SetInfoText(L("How strong the glow is. It applies in 5 steps (100, 60, 35, 18 and 8%). Lower it to see the details of the objects.", "Define quanto forte e o brilho. Aplica-se em 5 niveis (100, 60, 35, 18 e 8%). Reduza para ver os detalhes dos objetos."))
	endif
EndEvent
