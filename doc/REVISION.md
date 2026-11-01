# Revision contra Proyecto II - V01

## Implementado y comprobado en DOSBox

- RF-01: portada con integrantes y mascota de dos cuadros.
- RF-02: menu de cinco opciones por teclado.
- RF-03: instrucciones y ejemplo grafico de pistas.
- RF-04/05: generacion desde INT 1Ah; restricciones por dificultad y rechazo de clave consecutiva identica.
- RF-06: flechas, numeros 1..C, Enter evalua, Backspace borra.
- RF-07: aviso de intento incompleto; no consume turno.
- RF-08: toros/vacas con reserva de coincidencias; cuatro casos oficiales comprobados por /T.
- RF-09: historial visible y almacenado en arreglos (cinco bytes por intento).
- RF-10: dificultad, numero de intento y restantes.
- RF-11: victoria/derrota y revelacion de la clave.
- RF-12: resultado con calabaza que salta o fantasma que se sacude.
- RF-13: Enter reinicia; M/Esc vuelve al menu.
- RF-14: restauracion de modo y salida DOS; altavoz apagado.
- RG-01/02/03: modo 13h, DibujarSprite generico, transparencia cero.
- RG-04: ocho fichas de 16x16 en DATA. Autoria original debe confirmarla la pareja.
- RG-05/06: portada, seleccion y fin con dos cuadros; intervalo de cuatro ticks BIOS.
- RG-07: 27 colores DAC y tabla doc/PALETA.md. Se corrigio el indice 164 no definido del buho a dorado 14.
- RG-08: se redibuja solo el cursor/ficha/selector; no se limpia el tablero en cada espera.
- RT-01..06: mas de 12 PROC, menos de 200 lineas por PROC, encabezados, pila y arreglos.
- RT-07: TASM 3.2 y TLINK 3.01 sin errores ni advertencias; registros en bin.
- RT-08: logica y dibujo en ensamblador; Python/PowerShell solo exportan o construyen.

## Entregables

- E1: fuentes organizadas en src, bin, sprites, herramientas, doc.
- E2: bin/CLAVE.EXE y dosbox.conf.
- E3: README con comandos y controles, script de compilacion probado.
- E4: doc/Documento-tecnico.pdf, 10 paginas; revisar y personalizar antes de entregar.
- E5: doc/hoja-sprites.png y tablas de bytes en figuras.inc/cuadros.inc.
- E6: doc/BITACORA.md es una plantilla pendiente de completar por semana con hechos reales.
- E7: video opcional de 3 a 5 minutos pendiente de grabar por las autoras.

## Pendiente de las autoras

Confirmar que los sprites originales son propios, completar la bitacora real y revisar
la declaracion de IA y todo el codigo para la defensa individual. No se inventaron
aportes, horas, firmas ni historias de trabajo. Confirmar tambien si el docente exige
una ficha lateral de 16px de alto por fila: Facil requiere 12 filas y se usan filas de
13px para conservar todas dentro de la zona de 168px del propio enunciado.

No se implementaron los opcionales de puntajes en archivo, cronometro, modo inverso,
dos jugadores ni fade. Si se conserva el sonido existente, ya es una funcion opcional.
