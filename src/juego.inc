
; Logica principal del juego.

IniciarPartida PROC
    PUSH AX

    ; Reiniciar el estado del jugador.
    MOV TurnoActual, 0
    MOV PosicionActual, 0

    ; Revisar la dificultad seleccionada.
    CMP NivelActual, 0
    JNE RevisarNormal

    ; FACIL
    MOV CantidadPosiciones, 4
    MOV CantidadFichas, 6
    MOV IntentosMaximos, 12
    MOV PermitirRepetidos, 0
    JMP FinIniciarPartida

RevisarNormal:
    CMP NivelActual, 1
    JNE ConfigurarDificil

    ; NORMAL
    MOV CantidadPosiciones, 4
    MOV CantidadFichas, 6
    MOV IntentosMaximos, 10
    MOV PermitirRepetidos, 1
    JMP FinIniciarPartida

ConfigurarDificil:
    ; DIFICIL
    MOV CantidadPosiciones, 5
    MOV CantidadFichas, 8
    MOV IntentosMaximos, 10
    MOV PermitirRepetidos, 1

FinIniciarPartida:
    POP AX
    RET
IniciarPartida ENDP


; Dibujar el cursor debajo de la casilla seleccionada.
DibujarCursorJuego PROC
    PUSH AX
    PUSH BX

    MOV AL, PosicionActual
    MOV BL, 22
    MUL BL
    ADD AX, 24
    MOV BX, AX

    PINTAR_RECT BX, 42, 16, 1, 7

    POP BX
    POP AX
    RET
DibujarCursorJuego ENDP


; Borrar la posicion anterior del cursor.
BorrarCursorJuego PROC
    PUSH AX
    PUSH BX

    MOV AL, PosicionActual
    MOV BL, 22
    MUL BL
    ADD AX, 24
    MOV BX, AX

    PINTAR_RECT BX, 42, 16, 1, TAB_CASILLA

    POP BX
    POP AX
    RET
BorrarCursorJuego ENDP


; Controlar el movimiento del jugador.
ControlarJuego PROC
    PUSH AX

    CALL DibujarCursorJuego

JuegoEsperar:
    CALL ActualizarSonido

    ; Comprobar si hay alguna tecla.
    MOV AH, 01H
    INT 16H
    JZ JuegoEsperar

    ; Leer la tecla.
    MOV AH, 10H
    INT 16H

    ; ESC: regresar al menu.
    CMP AL, 27
    JNE JuegoRevisarTeclas
    JMP JuegoSalir

JuegoRevisarTeclas:
    ; Flecha izquierda.
    CMP AH, 4BH
    JE JuegoIzquierda

    ; Flecha derecha.
    CMP AH, 4DH
    JE JuegoDerecha

    JMP JuegoEsperar

JuegoIzquierda:
    ; Evitar salir por el lado izquierdo.
    CMP PosicionActual, 0
    JNE JuegoMoverIzquierda
    JMP JuegoEsperar

JuegoMoverIzquierda:
    CALL BorrarCursorJuego

    DEC PosicionActual

    CALL DibujarCursorJuego
    CALL SonidoSeleccion

    JMP JuegoEsperar

JuegoDerecha:
    ; Ultima posicion permitida.
    MOV AL, CantidadPosiciones
    DEC AL

    CMP PosicionActual, AL
    JB JuegoMoverDerecha
    JMP JuegoEsperar

JuegoMoverDerecha:
    CALL BorrarCursorJuego

    INC PosicionActual

    CALL DibujarCursorJuego
    CALL SonidoSeleccion

    JMP JuegoEsperar

JuegoSalir:
    CALL SonidoSeleccion

    POP AX
    RET
ControlarJuego ENDP
