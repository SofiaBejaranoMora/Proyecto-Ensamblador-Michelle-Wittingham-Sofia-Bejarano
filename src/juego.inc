
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
