
; Sin parametros. Activa pitido y guarda tick inicial; conserva AX,CX,DX.
SonidoSeleccion PROC
    PUSH AX
    PUSH CX
    PUSH DX

    MOV AL, 0B6h
    OUT 43h, AL

    MOV AX, 1193

    OUT 42h, AL
    MOV AL, AH
    OUT 42h, AL

    IN AL, 61h
    OR AL, 03h
    OUT 61h, AL

    MOV AH, 00h
    INT 1Ah

    MOV InicioSonido, DX
    MOV SonidoActivo, 1

    POP DX
    POP CX
    POP AX
    RET
SonidoSeleccion ENDP

; Sin parametros. Apaga tras dos ticks si esta activo; conserva AX,CX,DX.
ActualizarSonido PROC
    PUSH AX
    PUSH CX
    PUSH DX

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


; Sin parametros. Desactiva altavoz y SonidoActivo; conserva AX.
ApagarSonido PROC
    PUSH AX

    IN AL, 61h
    AND AL, 0FCh
    OUT 61h, AL

    MOV SonidoActivo, 0

    POP AX
    RET
ApagarSonido ENDP