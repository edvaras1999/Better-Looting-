#pragma once

// Realce de objetos: aplica um efeito de shader do jogo (BetterLooting.esp) ao objeto que esta
// na mira. A chamada usa a funcao nativa EffectShader.Play diretamente, sem scripts Papyrus.
namespace Highlight
{
	// procura os shaders do ESP e regista o evento da mira
	void Init();
}
