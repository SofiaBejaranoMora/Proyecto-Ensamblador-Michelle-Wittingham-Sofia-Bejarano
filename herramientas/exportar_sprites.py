"""Exporta los cuadros de src a una hoja PNG y documenta la paleta. Requiere Pillow."""
from pathlib import Path
import re
from PIL import Image, ImageDraw
raiz = Path(__file__).resolve().parents[1]
colores = []
for linea in (raiz / 'src/paleta.inc').read_text().splitlines():
    if linea.startswith('DB '):
        colores.append(tuple(int(x.strip()) for x in linea[3:].split(';')[0].split(',')))
rgb = [tuple(v * 255 // 63 for v in c) for c in colores]
hoja = Image.new('RGB', (720, 600), '#202632')
dibujo = ImageDraw.Draw(hoja)
nombres = ['Gato', 'Calabaza', 'Caldero', 'Dulce', 'Sombrero', 'Cuervo', 'Buho', 'Molly', 'Fantasma']
for cuadro, archivo in enumerate(['figuras.inc', 'cuadros.inc']):
    texto = (raiz / 'src' / archivo).read_text()
    for indice, nombre in enumerate(nombres):
        etiqueta = 'Sprite' + nombre + ('2' if cuadro else '')
        bloque = texto.split(etiqueta + ' LABEL BYTE')[1]
        valores = []
        for linea in bloque.splitlines():
            if linea.startswith('DB '):
                for numero in linea[3:].split(';')[0].split(','):
                    numero = numero.strip()
                    valores.append(int(numero[:-1], 16) if numero.lower().endswith('h') else int(numero))
                if len(valores) >= 256:
                    break
        assert len(valores) == 256
        imagen = Image.new('RGB', (16, 16), '#aca690')
        imagen.putdata([rgb[v] if v else (172,166,144) for v in valores])
        x = (indice % 3) * 240 + cuadro * 96 + 14
        y = (indice // 3) * 195 + 28
        hoja.paste(imagen.resize((80, 80), Image.Resampling.NEAREST), (x, y))
        dibujo.text((x, y + 88), nombre + ' / ' + str(cuadro + 1), fill='white')
(raiz / 'doc').mkdir(exist_ok=True)
hoja.save(raiz / 'doc/hoja-sprites.png')
lineas = ['# Tabla de paleta VGA', '', 'Componentes DAC de 0 a 63. Indice 0 transparente en sprites.', '', '| Indice | R | G | B | Vista RGB |', '|---|---:|---:|---:|---|']
for i, ((r,g,b), c) in enumerate(zip(colores,rgb)):
    lineas.append(f'| {i} | {r} | {g} | {b} | #{c[0]:02X}{c[1]:02X}{c[2]:02X} |')
(raiz / 'doc/PALETA.md').write_text('\n'.join(lineas) + '\n')
print('Hoja de sprites y paleta exportadas.')
