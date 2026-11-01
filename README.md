# Clave Espectral

Mastermind grafico para DOS, ensamblador x86 de 16 bits, VGA 13h (320 x 200).
Integrantes: Michelle Wittingham y Sofia Bejarano.

## Compilar y comprobar

Requiere DOSBox y Borland TASM/TLINK. No se distribuyen las herramientas comerciales.
Desde PowerShell en la raiz del repositorio:

```powershell
powershell -ExecutionPolicy Bypass -File .\herramientas\compilar.ps1 -Pruebas
```

Las rutas predeterminadas corresponden al equipo de desarrollo. Para otro equipo:

```powershell
powershell -ExecutionPolicy Bypass -File .\herramientas\compilar.ps1 -DosBox "C:\DOSBox\DOSBox.exe" -Tasm "C:\TASM" -Pruebas
```

El script monta una copia temporal, ensambla con `tasm /m5 /w2 prueba.asm`, enlaza con
`tlink prueba.obj` y ejecuta `prueba.exe /T`. Publica el ejecutable en `bin/CLAVE.EXE`
y guarda los registros. El modo /T usa las mismas rutinas de evaluacion del juego.

Para ensamblar manualmente, dentro de DOSBox, montar el repositorio como C y TASM como T:

```dos
mount c "C:\GITHUB\Proyecto-Ensamblador-Michelle-Wittingham-Sofia-Bejarano"
mount t "C:\TASM"
c:
cd src
t:\tasm /m5 /w2 prueba.asm
t:\tlink prueba.obj
prueba.exe
```

Para ejecutar la entrega: abrir DOSBox desde la raiz con `-conf dosbox.conf`.
En el IDE tambien se puede compilar y ejecutar `src/prueba.asm`.

## Controles

- Portada: Enter continua; Esc sale.
- Menu: arriba/abajo eligen, Enter abre, Esc sale.
- Dificultad: arriba/abajo eligen; Enter guarda; Esc cancela.
- Juego: izquierda/derecha cambian casilla; arriba/abajo cambian y colocan ficha.
- Teclas 1..C colocan directamente un personaje en la casilla activa.
- Enter evalua; Backspace vacia la casilla; Esc vuelve al menu.
- Resultado: Enter inicia otra partida; M o Esc vuelve al menu.
- F2 desde el menu: editor de clave secreta para la demostracion del algoritmo.

Las pistas son numeros: verde = toros (ficha y posicion exactas), naranja = vacas
(ficha correcta en otra posicion). Cada ficha del secreto se cuenta una sola vez.

## Dificultad

| Nivel | Posiciones | Fichas | Repetidos | Intentos |
|---|---:|---:|---|---:|
| Facil | 4 | 6 | No | 12 |
| Normal | 4 | 6 | Si | 10 |
| Dificil | 5 | 8 | Si | 10 |

El editor F2 admite repetidos tambien en Facil, exclusivamente para las pruebas del
PDF; las partidas aleatorias respetan las restricciones de la dificultad elegida.

## Casos del docente

Usar Normal y F2. Equivalencias de simbolos: R=1, V=2, A=3, M=4, N=5, B=6.

| Secreto | Intento | Toros | Vacas |
|---|---|---:|---:|
| 1234 | 1324 | 2 | 2 |
| 1123 | 1211 | 1 | 2 |
| 1234 | 5656 | 0 | 0 |
| 3333 | 3311 | 2 | 0 |

Introducir la clave con F2 y Enter. En el tablero colocar cada digito y mover a la
siguiente casilla con derecha; Enter evalua. `/T` ejecuta estos cuatro casos sin
interaccion, ademas de comprobar generacion y borrado/validacion.

## Archivos

`src/`: ensamblador y tablas de sprites. `bin/`: entrega compilada y registros.
`sprites/`: referencias previas. `herramientas/`: scripts auxiliares de construccion.
`doc/`: documento tecnico, auditoria, paleta, hoja de sprites y plantilla de bitacora.

## Uso de IA y autoria

Se utilizo Codex para explicar ensamblador, revisar errores y generar/modificar
partes del codigo, pruebas y documentacion, incluidos controles, evaluacion,
animaciones y compilacion. Las autoras deben revisar, comprender y adaptar todo
lo entregado y completar la declaracion personal en el documento tecnico.
La procedencia y autoria original de los sprites existentes debe ser confirmada
por las autoras; este repositorio no prueba por si solo ese requisito.
