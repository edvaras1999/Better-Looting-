# -*- coding: utf-8 -*-
"""Gera BetterLooting.esp (so a quest do MCM) e BetterLooting.seq, sem Creation Kit.
   Uso: python tools/make_esp.py  ->  dist/Data/BetterLooting.esp e dist/Data/SEQ/BetterLooting.seq
   Mestre: Skyrim.esm. A quest arranca com o jogo ("Start Game Enabled") e liga o script BetterLootingMCM."""
import os
import struct


FORM_VERSION = 43


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


OWN = 0x01000000                      # indice 01: o unico mestre e Skyrim.esm (00)
ID_QUST = OWN | 0x800

script = b'BetterLootingMCM'
vmad = struct.pack('<hhH', 5, 2, 1) + struct.pack('<H', len(script)) + script + b'\x00' + struct.pack('<H', 0)
qust = rec('QUST', ID_QUST, [
    sub('EDID', z('BetterLooting_MCMQuest')),
    sub('VMAD', vmad),
    sub('DNAM', bytes.fromhex('110100300000000000000000')),   # Start Game Enabled
    sub('NEXT'),
    sub('ANAM', struct.pack('<I', 0)),
])

n_total = 1 + 1                       # 1 registo + 1 grupo (o jogo conta os grupos)
tes4 = rec('TES4', 0, [
    sub('HEDR', struct.pack('<fII', 1.7, n_total, 0x801)),
    sub('CNAM', z('BetterLooting')),
    sub('MAST', z('Skyrim.esm')),
    sub('DATA', struct.pack('<Q', 0)),
    sub('INTV', struct.pack('<I', 1)),
])

esp = tes4 + grup('QUST', [qust])

root = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dist', 'Data')
os.makedirs(os.path.join(root, 'SEQ'), exist_ok=True)
with open(os.path.join(root, 'BetterLooting.esp'), 'wb') as f:
    f.write(esp)
with open(os.path.join(root, 'SEQ', 'BetterLooting.seq'), 'wb') as f:
    f.write(struct.pack('<I', ID_QUST))
print('BetterLooting.esp: %d bytes; BetterLooting.seq: quest %08X' % (len(esp), ID_QUST))
