
; Pantalla de dificultad.
; Requiere macros.inc, video.inc, menuDato.inc y audio.inc.

; Entrada: OpcionDificultad. Salida: pantalla y selector; conserva registros.
PantallaDificultad PROC
    PUSH AX
    PUSH CX
    CALL AbrirLienzo
    ESCRIBIR_TEXTO 1, 3, 'A TU RITMO'
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 3, 3, 'Elige que tan traviesa sera la clave.'
    XOR AX, AX
    MOV AL, OpcionDificultad
    PUSH AX
    MOV OpcionDificultad, 0
    MOV CX, 3
DificultadPintarTarjetas:
    MOV FondoTexto, 22
    CALL PintarTarjetaDificultad
    INC OpcionDificultad
    LOOP DificultadPintarTarjetas
    POP AX
    MOV OpcionDificultad, AL
    MOV FondoTexto, 21
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 22, 3, 'Flechas elegir - ENTER guardar'
    ESCRIBIR_TEXTO 24, 3, 'ESC vuelve sin cambiar tu nivel'
    CALL DibujarSelectorDificultad
    CALL CerrarEstilo
    CALL TerminarTransicion
    POP CX
    POP AX
    RET
PantallaDificultad ENDP


; Escribir textos en pantalla.
; Entrada: DS:SI=cadena terminada en cero, DH=fila, DL=columna.
; Salida: texto blanco. Conserva AX,BX,CX,DX,SI; requiere DF=0.
EscribirTextoDificultad PROC
    ; Las cadenas pueden venir de CODE o de la pila; el estilo vive en DATA.
    PUSH AX
    PUSH DS
    MOV AX, SEG FondoTexto
    MOV DS, AX
    CMP FondoTexto, 0
    POP DS
    POP AX
    JE TextoBIOSNormal
    CALL EscribirTextoEnPanel
    RET
TextoBIOSNormal:
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
    MOV FondoTexto, 25
    CALL PintarTarjetaDificultad
    MOV AL, OpcionDificultad
    MOV BL, 40
    MUL BL
    ADD AX, 60
    MOV BX, AX
    PINTAR_IMAGEN 8, BX, 8, 8, SpriteFlecha, 8, 8
    POP BX
    POP AX
    RET
DibujarSelectorDificultad ENDP

; Quita el resaltado sin redibujar el resto. Conserva AX,BX.
BorrarSelectorDificultad PROC
    PUSH AX
    PUSH BX
    MOV FondoTexto, 22
    CALL PintarTarjetaDificultad
    MOV AL, OpcionDificultad
    MOV BL, 40
    MUL BL
    ADD AX, 60
    MOV BX, AX
    PINTAR_RECT 8, BX, 8, 8, 21
    POP BX
    POP AX
    RET
BorrarSelectorDificultad ENDP

; Entrada: opcion y fondo de la tarjeta. Conserva registros.
PintarTarjetaDificultad PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    MOV AL, OpcionDificultad
    MOV BL, 40
    MUL BL
    ADD AX, 48
    MOV BX, AX
    XOR DX, DX
    MOV DL, FondoTexto
    PINTAR_RECT 24, BX, 272, 36, DX
    CMP FondoTexto, 25
    JNE DificultadSinAcento
    PINTAR_RECT 24, BX, 2, 36, 7
DificultadSinAcento:
    ADD BX, 8
    MOV AX, BX
    MOV CL, 3
    SHR AX, CL
    MOV DH, AL
    MOV ColorTexto, 3
    CMP OpcionDificultad, 0
    JNE DificultadTarjetaNormal
    ESCRIBIR_TEXTO DH, 5, 'Facil - sin repetir'
    MOV ColorTexto, 26
    ADD DH, 2
    ESCRIBIR_TEXTO DH, 5, '4 lugares / 6 fichas / 12 turnos'
    PINTAR_IMAGEN 272, 52, 16, 16, SpriteGato, 16, 16
    JMP DificultadTarjetaFin
DificultadTarjetaNormal:
    CMP OpcionDificultad, 1
    JNE DificultadTarjetaDificil
    ESCRIBIR_TEXTO DH, 5, 'Normal - con repeticiones'
    MOV ColorTexto, 26
    ADD DH, 2
    ESCRIBIR_TEXTO DH, 5, '4 lugares / 6 fichas / 10 turnos'
    PINTAR_IMAGEN 272, 92, 16, 16, SpriteCalabaza, 16, 16
    JMP DificultadTarjetaFin
DificultadTarjetaDificil:
    ESCRIBIR_TEXTO DH, 5, 'Dificil - con repeticiones'
    MOV ColorTexto, 26
    ADD DH, 2
    ESCRIBIR_TEXTO DH, 5, '5 lugares / 8 fichas / 10 turnos'
    PINTAR_IMAGEN 272, 132, 16, 16, SpriteFantasma, 16, 16
DificultadTarjetaFin:
    CALL CerrarEstilo
    POP DX
    POP CX
    POP BX
    POP AX
    RET
PintarTarjetaDificultad ENDP


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
