#pragma once

class TESObjectREFR;

// Realce de objetos: aplica um efeito de shader do jogo (BetterLooting.esp) aos objetos. A chamada usa
// a funcao nativa EffectShader.Play diretamente, sem scripts Papyrus.
//  - objeto na mira (evento da mira)
//  - varrimento por raio e cadaveres lootaveis (tecla mantida premida)
namespace Highlight
{
	// procura os shaders do ESP e regista o evento da mira
	void Init();

	// estado da tecla do varrimento (chamado pelo gancho de input; nao consome a tecla)
	void OnScanKey(bool a_down, bool a_up, float a_heldSeconds, bool a_lootMenuVisible);

	// o cadaver deixa de ser realcado (foi revistado)
	void MarkSearched(TESObjectREFR *a_ref);
}
