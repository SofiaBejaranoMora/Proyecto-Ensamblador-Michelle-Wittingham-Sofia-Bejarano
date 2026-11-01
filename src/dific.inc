
; Pantalla de dificultad.
; Requiere macros.inc, video.inc, menuDato.inc y audio.inc.

; Entrada: OpcionDificultad. Salida: pantalla y selector; conserva registros.
PantallaDificultad PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI

    CALL LimpiarPantalla

    ; Fondo y bordes.
    PINTAR_RECT 4, 4, 309, 192, 1

    PINTAR_RECT 4, 4, 309, 1, 4
    PINTAR_RECT 4, 195, 309, 1, 4
    PINTAR_RECT 4, 4, 1, 192, 4
    PINTAR_RECT 312, 4, 1, 192, 4

    ; Titulo.
    PINTAR_IMAGEN 81, 18, 158, 18, TextoDificultad, TextoDificultadAncho, TextoDificultadAlto

    PINTAR_RECT 24, 48, 268, 1, 5

    ; Cuadros de las opciones.
    PINTAR_RECT 88, 62, 144, 24, 2
    PINTAR_RECT 88, 94, 144, 24, 2
    PINTAR_RECT 88, 126, 144, 24, 2

    ; Texto FACIL.
    ESCRIBIR_TEXTO 9, 17, 'FACIL'

    ; Texto NORMAL.
    ESCRIBIR_TEXTO 13, 17, 'NORMAL'

    ; Texto DIFICIL.
    ESCRIBIR_TEXTO 17, 17, 'DIFICIL'

    ; Indicacion inferior.
    ESCRIBIR_TEXTO 22, 9, 'ESC: VOLVER AL MENU'

    ; Dibujar la opcion seleccionada.
    CALL DibujarSelectorDificultad

    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
PantallaDificultad ENDP


; Escribir textos en pantalla.
; Entrada: DS:SI=cadena terminada en cero, DH=fila, DL=columna.
; Salida: texto blanco. Conserva AX,BX,CX,DX,SI; requiere DF=0.
EscribirTextoDificultad PROC
    PUSH AX
    PUSH BX
    PUSH DX
    PUSH SI

DifSiguienteLetra:
    LODSB
    CMP AL, 0
    JE DifFinTexto

    PUSH AX
    MOV AH, 02H
    XOR BH, BH
    INT 10H
    POP AX

    MOV AH, 09H
    XOR BH, BH
    MOV BL, 3

    PUSH CX
    MOV CX, 1
    INT 10H
    POP CX

    INC DL
    JMP DifSiguienteLetra

DifFinTexto:
    POP SI
    POP DX
    POP BX
    POP AX
    RET
EscribirTextoDificultad ENDP


; Dibujar la flecha de seleccion.
; Entrada: OpcionDificultad. Salida: flecha dibujada; conserva AX,BX.
DibujarSelectorDificultad PROC
    PUSH AX
    PUSH BX

    MOV AL, OpcionDificultad
    MOV BL, 32
    MUL BL
    ADD AX, 70
    MOV BX, AX

    PINTAR_IMAGEN 73, BX, 8, 8, SpriteFlecha, 8, 8

    POP BX
    POP AX
    RET
DibujarSelectorDificultad ENDP


; Borrar la flecha anterior.
; Entrada: OpcionDificultad anterior. Salida: flecha borrada; conserva AX,BX.
BorrarSelectorDificultad PROC
    PUSH AX
    PUSH BX

    MOV AL, OpcionDificultad
    MOV BL, 32
    MUL BL
    ADD AX, 70
    MOV BX, AX

    PINTAR_RECT 73, BX, 8, 8, 1

    POP BX
    POP AX
    RET
BorrarSelectorDificultad ENDP


; Control de seleccion de dificultad.
; Entrada: NivelActual. Salida: Enter actualiza el nivel; Esc cancela; conserva AX.
SeleccionarDificultad PROC
    PUSH AX

    ; Iniciar con la dificultad actual.
    MOV AL, NivelActual
    MOV OpcionDificultad, AL

    CALL PantallaDificultad

DifEsperar:
    CALL ActualizarSonido

    ; Comprobar si hay una tecla.
    MOV AH, 01H
    INT 16H
    JZ DifEsperar

    ; Leer la tecla.
    MOV AH, 00H
    INT 16H

    ; ESC: regresar sin guardar.
    CMP AL, 27
    JNE DifRevisarEnter
    JMP DifSalir

DifRevisarEnter:
    ; ENTER: confirmar dificultad.
    CMP AL, 13
    JNE DifRevisarArriba
    JMP DifConfirmar

DifRevisarArriba:
    CMP AH, 48H
    JNE DifRevisarAbajo
    JMP DifArriba

DifRevisarAbajo:
    CMP AH, 50H
    JNE DifEsperar
    JMP DifAbajo


; Mover hacia arriba.
DifArriba:
    CALL SonidoSeleccion
    CALL BorrarSelectorDificultad

    CMP OpcionDificultad, 0
    JE DifUltima

    DEC OpcionDificultad
    JMP DifActualizar

DifUltima:
    MOV OpcionDificultad, 2
    JMP DifActualizar


; Mover hacia abajo.
DifAbajo:
    CALL SonidoSeleccion
    CALL BorrarSelectorDificultad

    CMP OpcionDificultad, 2
    JE DifPrimera

    INC OpcionDificultad
    JMP DifActualizar

DifPrimera:
    MOV OpcionDificultad, 0


; Actualizar solamente la flecha.
DifActualizar:
    CALL DibujarSelectorDificultad
    JMP DifEsperar


; Guardar la dificultad elegida.
DifConfirmar:
    MOV AL, OpcionDificultad
    MOV NivelActual, AL

    CALL SonidoSeleccion

DifSalir:
    POP AX
    RET
SeleccionarDificultad ENDP
