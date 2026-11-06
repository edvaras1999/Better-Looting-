# -*- coding: utf-8 -*-
"""Gera BetterLooting.esp e BetterLooting.seq, sem Creation Kit.
   Uso: python tools/make_esp.py  ->  dist/Data/BetterLooting.esp e dist/Data/SEQ/BetterLooting.seq

   Conteudo (mestre: Skyrim.esm):
   - 1 quest ("Start Game Enabled") que liga o script do MCM, BetterLootingMCM  (forma 0x800)
   - 300 efeitos de shader (EFSH) para o realce de objetos (formas 0x801 a 0x92C):
       forma = 0x801 + (nivel * 5 + cor) * 12 + posicao ; 5 niveis de intensidade x 5 cores x 12 posicoes
       posicoes 0 a 3 = realce na mira (rodam) ; 4 a 11 = varrimento por raio
     A intensidade esta gravada em cada shader (textura de preenchimento cinzenta + alfa), porque o jogo
     nao le alteracoes feitas aos dados do shader depois de carregado.
     Os dados de base (BASE_DATA) vem do HighlightObjects.esp do Highlight Objects (EdgeLight Edition);
     aqui aplicam-se os mesmos valores que esse mod definia em tempo de execucao, ja gravados no ESP."""
import os
import struct

BASE_DATA = bytes.fromhex(
    '4e0000000500000001000000030000001f1f1f000000000000000000000000000000003f0000403f'
    'cdcccc3d00000000000000000000c03fd4af37000000000000000000000000000000000000000000'
    '0000803f0000803f0000000002000000050000000100000004000000020000000000803ecdcccc3d'
    '00000000000048420000a0410000c04000000000000000000000000000000000000000c000000000'
    '000080c00000c0c0000040c00000004000001041000000000000803f24242400ffffff0086868600'
    '000000000000803e00000000000000003333b33e0000803f0000000000000000000034430000a041'
    '0000204100000000000000000000204100007f430000000000000000ffffff000000000002000000'
    '02000000cdcccc3dcdcccc3d0000803f0000803f0000803f0000803f00000000ffffff00a3a3a300'
    '0000803f0000803f0000803f000000000000003f0000c03f0000803f000000000000000000000000'
    '0000000000000000000000000000000000000000000000004e000000000040400000004000000000'
)


COLORS = [('Gold', (255, 215, 0)), ('White', (255, 255, 255)), ('Green', (0, 255, 102)),
          ('Cyan', (0, 200, 255)), ('Red', (255, 40, 40))]
SLOTS = 12
CROSSHAIR_SLOTS = 4
# (percentagem, alfa) por nivel de intensidade
LEVELS = [(100, 255), (70, 178), (45, 115), (25, 64), (10, 26)]
FORM_VERSION = 43


def palette_dds(rgb):
    """DDS (DXT1, 256x4) de cor solida: a textura de paleta do shader passa a dar a cor escolhida.
    Com o bit "cinzento para cor" ligado, a cor do brilho vem desta paleta e nao das chaves de cor."""
    r, g, b = rgb
    c565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)
    width, height = 256, 4
    data = struct.pack('<HHI', c565, c565, 0) * ((width // 4) * (height // 4))
    header = (b'DDS ' + struct.pack('<7I', 124, 0x81007, height, width, len(data), 0, 0) + b'\x00' * 44 +
              struct.pack('<2I4s5I', 32, 4, b'DXT1', 0, 0, 0, 0, 0) + struct.pack('<5I', 0x1000, 0, 0, 0, 0))
    return header + data


def fill_dds(gray):
    """DDS (DXT1, 4x4) cinzento uniforme: textura de preenchimento. Com "cinzento para alfa" ligado,
    o tom de cinzento define a opacidade do brilho."""
    c565 = ((gray >> 3) << 11) | ((gray >> 2) << 5) | (gray >> 3)
    data = struct.pack('<HHI', c565, c565, 0)
    header = (b'DDS ' + struct.pack('<7I', 124, 0x81007, 4, 4, len(data), 0, 0) + b'\x00' * 44 +
              struct.pack('<2I4s5I', 32, 4, b'DXT1', 0, 0, 0, 0, 0) + struct.pack('<5I', 0x1000, 0, 0, 0, 0))
    return header + data


def sub(tag, data=b''):
    return tag.encode() + struct.pack('<H', len(data)) + data


def z(s):
    return s.encode('latin1') + b'\x00'


def rec(tag, form_id, subs, flags=0):
    body = b''.join(subs)
    return (tag.encode() + struct.pack('<I', len(body)) + struct.pack('<I', flags) +
            struct.pack('<I', form_id) + struct.pack('<I', 0) + struct.pack('<HH', FORM_VERSION, 0) + body)


def grup(label, recs):
    body = b''.join(recs)
    return (b'GRUP' + struct.pack('<I', len(body) + 24) + label.encode() + struct.pack('<I', 0) +
            struct.pack('<HH', 0, 0) + struct.pack('<HH', 0, 0) + body)


def shader_data(rgb, slot, alpha):
    d = bytearray(BASE_DATA)
    r, g, b = rgb
    d[0x10:0x14] = bytes((r, g, b, alpha))                    # preenchimento: cor 1 (alfa do nivel)
    struct.pack_into('<f', d, 0x20, 0.01)                     # preenchimento: razao de alfa persistente
    struct.pack_into('<f', d, 0x34, 1.0)                      # contorno: queda
    d[0x38:0x3C] = bytes((r, g, b, alpha))                    # contorno: cor
    struct.pack_into('<f', d, 0x3C, 0.1)                      # contorno: entrada
    struct.pack_into('<f', d, 0x40, 0.4 if slot < CROSSHAIR_SLOTS else 2.6)   # contorno: tempo com alfa total
    struct.pack_into('<f', d, 0x44, 0.3)                      # contorno: saida
    struct.pack_into('<f', d, 0x48, 0.0)                      # contorno: razao de alfa persistente
    struct.pack_into('<f', d, 0x54, 1.0)                      # preenchimento: razao de alfa total
    struct.pack_into('<f', d, 0x58, 1.0)                      # contorno: razao de alfa total
    # Outras cores do shader que no original eram brancas/cinzentas e deixavam o brilho branco:
    # 0x10C (cor do contorno), 0x138 e 0x13C (chaves de cor 2 e 3 do preenchimento). Mantem-se o byte de alfa.
    for off in (0x10C, 0x138, 0x13C):
        d[off:off + 3] = bytes((r, g, b))
    return bytes(d)


OWN = 0x01000000                      # indice 01: o unico mestre e Skyrim.esm (00)
ID_QUST = OWN | 0x800
FIRST_EFSH = 0x801

script = b'BetterLootingMCM'
vmad = struct.pack('<hhH', 5, 2, 1) + struct.pack('<H', len(script)) + script + b'\x00' + struct.pack('<H', 0)
qust = rec('QUST', ID_QUST, [
    sub('EDID', z('BetterLooting_MCMQuest')),
    sub('VMAD', vmad),
    sub('DNAM', bytes.fromhex('110100300000000000000000')),   # Start Game Enabled
    sub('NEXT'),
    sub('ANAM', struct.pack('<I', 0)),
])

efsh = []
for li, (percent, alpha) in enumerate(LEVELS):
    for ci, (cname, rgb) in enumerate(COLORS):
        for slot in range(SLOTS):
            form_id = OWN | (FIRST_EFSH + (li * len(COLORS) + ci) * SLOTS + slot)
            efsh.append(rec('EFSH', form_id, [
                sub('EDID', z('BetterLooting_FX_L%d_%s_%02d' % (li, cname, slot))),
                sub('ICON', (u'Effects\\Gradients\\BetterLooting_Fill%d.dds' % li).encode('latin1') + b'\x00'),
                sub('ICO2', b'\x00'),
                sub('NAM7', b'\x00'),
                sub('NAM8', (u'Effects\\Gradients\\BetterLooting_%s.dds' % cname).encode('latin1') + b'\x00'),
                sub('NAM9', b'\x00'),
                sub('DATA', shader_data(rgb, slot, alpha)),
            ]))

n_total = 1 + len(efsh) + 2           # registos + 2 grupos (o jogo conta os grupos)
next_id = FIRST_EFSH + len(efsh)
tes4 = rec('TES4', 0, [
    sub('HEDR', struct.pack('<fII', 1.7, n_total, next_id)),
    sub('CNAM', z('BetterLooting')),
    sub('MAST', z('Skyrim.esm')),
    sub('DATA', struct.pack('<Q', 0)),
    sub('INTV', struct.pack('<I', 1)),
])

esp = tes4 + grup('EFSH', efsh) + grup('QUST', [qust])

root = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dist', 'Data')
os.makedirs(os.path.join(root, 'SEQ'), exist_ok=True)
with open(os.path.join(root, 'BetterLooting.esp'), 'wb') as f:
    f.write(esp)
with open(os.path.join(root, 'SEQ', 'BetterLooting.seq'), 'wb') as f:
    f.write(struct.pack('<I', ID_QUST))
tex_dir = os.path.join(root, 'Textures', 'Effects', 'Gradients')
os.makedirs(tex_dir, exist_ok=True)
for cname, rgb in COLORS:
    with open(os.path.join(tex_dir, 'BetterLooting_%s.dds' % cname), 'wb') as f:
        f.write(palette_dds(rgb))
for li, (percent, alpha) in enumerate(LEVELS):
    with open(os.path.join(tex_dir, 'BetterLooting_Fill%d.dds' % li), 'wb') as f:
        f.write(fill_dds(alpha))
print('BetterLooting.esp: %d bytes, %d shaders; BetterLooting.seq: quest %08X' % (len(esp), len(efsh), ID_QUST))
