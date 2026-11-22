#pragma once

class TESObjectREFR;

// Apanhar automatico: apanha (ativa) sozinho os objetos soltos, a flora e (opcionalmente) os filoes de minerio
// que passam nas regras do jogador, e da brilho ao que conta. Nunca toca em cadaveres, contentores, portas,
// alavancas nem em nada que tenha dono.
// As regras por tipo, os filtros de nome e de forma e as listas de proibicao estao em
// BetterLooting_AutoLoot.ini (ao lado da DLL).
namespace AutoLoot
{
	const int kTypeCount = 30;

	// le BetterLooting_AutoLoot.ini (chamar depois de os dados do jogo carregarem)
	void Load();

	// chamado a cada lote de eventos de input; trabalha no maximo duas vezes por segundo
	void Tick();

	// tecla ou botao que liga/desliga o apanhar automatico (so observa; nao consome a tecla)
	void OnToggleKey(bool a_lootMenuVisible);

	// tecla ou botao mantido premido (so observa; nao consome a tecla). a_pad: false = teclado/rato, true = comando
	void OnHoldKey(bool a_down, bool a_up, float a_heldSeconds, bool a_lootMenuVisible, bool a_pad);

	// regras lidas/alteradas pelo MCM. section "Type.<Nome>" com as chaves Pick, Glow, Color, MinValue,
	// MinValuePerWeight, MinArmor, MinDamage.
	bool GetRule(const char *a_section, const char *a_key, int &a_out);
	bool SetRule(const char *a_section, const char *a_key, int a_value);
}
