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
	Pages = new string[3]
	Pages[0] = "General / Geral"
	Pages[1] = "Window / Janela"
	Pages[2] = "Keys / Teclas"
EndFunction

string Function L(string en, string pt)
	if lang == 1
		return pt
	endif
	return en
EndFunction

Function BuildTables()
	tKey = new string[10]
	tEN = new string[10]
	tPT = new string[10]
	tOpt = new int[10]
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

	sKey = new string[9]
	sEN = new string[9]
	sPT = new string[9]
	sMin = new float[9]
	sMax = new float[9]
	sStep = new float[9]
	sFmt = new string[9]
	sOpt = new int[9]
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
	elseif a_option == sOpt[3]
		SetInfoText(L("Stops the list after this many items. 0 means all items are listed.", "Para a lista neste numero de itens. 0 lista todos os itens."))
	endif
EndEvent
