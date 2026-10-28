
SonidoSeleccion PROC
    PUSH AX
    PUSH CX
    PUSH DX

    ; CONFIGURAR CANAL 2 DEL TEMPORIZADOR
    MOV AL, 0B6h
    OUT 43h, AL

    ; CONFIGURAR FRECUENCIA DE 1000 Hz APROX.
    MOV AX, 1193

    OUT 42h, AL
    MOV AL, AH
    OUT 42h, AL

    ; ENCENDER ALTAVOZ
    IN AL, 61h
    OR AL, 03h
    OUT 61h, AL

    ; GUARDAR EL TIEMPO DE INICIO
    MOV AH, 00h
    INT 1Ah

    MOV InicioSonido, DX
    MOV SonidoActivo, 1

    POP DX
    POP CX
    POP AX
    RET
SonidoSeleccion ENDP

; ACTUALIZAR DURACION DEL SONIDO

ActualizarSonido PROC
    PUSH AX
    PUSH CX
    PUSH DX

    ; VERIFICAR SI HAY SONIDO ACTIVO
    CMP SonidoActivo, 0
    JE FinActualizarSonido

    MOV AH, 00h
    INT 1Ah
    SUB DX, InicioSonido

    CMP DX, 2
    JB FinActualizarSonido

    CALL ApagarSonido

FinActualizarSonido:
    POP DX
    POP CX
    POP AX
    RET
ActualizarSonido ENDP


; APAGAR ALTAVOZ

ApagarSonido PROC
    PUSH AX

    IN AL, 61h
    AND AL, 0FCh
    OUT 61h, AL

    MOV SonidoActivo, 0

    POP AX
    RET
ApagarSonido ENDP