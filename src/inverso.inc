
IniciarModoInverso PROC
    PUSH AX
    PUSH BX
    PUSH SI

    CALL IniciarPartida

    CALL LimpiarPantalla

    ESCRIBIR_TEXTO 3, 10, 'MODO INVERSO'
    ESCRIBIR_TEXTO 8, 2, 'ENTER: ACEPTAR  BS: BORRAR'
    ESCRIBIR_TEXTO 12, 3, 'TU CLAVE:'
    ESCRIBIR_TEXTO 20, 3, 'ESC: CANCELAR'

    CMP CantidadFichas, 8
    JNE InversoSeisFichas

    ESCRIBIR_TEXTO 6, 3, 'ELIGE FICHAS DEL 1 AL 8'
    JMP InversoReglas

InversoSeisFichas:
    ESCRIBIR_TEXTO 6, 3, 'ELIGE FICHAS DEL 1 AL 6'

InversoReglas:
    CMP PermitirRepetidos, 0
    JNE InversoPermiteRepetidos

    ESCRIBIR_TEXTO 10, 3, 'SIN FICHAS REPETIDAS'
    JMP InversoIniciarEntrada

InversoPermiteRepetidos:
    ESCRIBIR_TEXTO 10, 3, 'SE PERMITEN REPETICIONES'

InversoIniciarEntrada:
    XOR SI, SI

InversoEsperarTecla:
    MOV AH, 00H
    INT 16H

    CMP AL, 27
    JE InversoSalir

    CMP AL, 8
    JE InversoBorrar

    CMP AL, 13
    JE InversoConfirmar

    CMP AL, '1'
    JB InversoEsperarTecla

    CMP AL, '8'
    JA InversoEsperarTecla

    SUB AL, '0'

    CMP AL, CantidadFichas
    JA InversoEsperarTecla

    XOR BX, BX
    MOV BL, CantidadPosiciones

    CMP SI, BX
    JAE InversoEsperarTecla

    CMP PermitirRepetidos, 0
    JNE InversoGuardarFicha

    XOR BX, BX

InversoBuscarRepetida:
    CMP BX, SI
    JAE InversoGuardarFicha

    CMP AL, CodigoSecreto[BX]
    JE InversoEsperarTecla

    INC BX
    JMP InversoBuscarRepetida

InversoGuardarFicha:
    MOV CodigoSecreto[SI], AL
    INC SI

    CALL MostrarClaveManual
    JMP InversoEsperarTecla

InversoBorrar:
    CMP SI, 0
    JE InversoEsperarTecla

    DEC SI
    MOV CodigoSecreto[SI], 0

    CALL MostrarClaveManual
    JMP InversoEsperarTecla

InversoConfirmar:
    XOR BX, BX
    MOV BL, CantidadPosiciones

    CMP SI, BX
    JNE InversoEsperarTecla

    CALL LimpiarPantalla

    ESCRIBIR_TEXTO 8, 11, 'CLAVE GUARDADA'
    ESCRIBIR_TEXTO 12, 4, 'PREPARADA PARA MODO INVERSO'
    ESCRIBIR_TEXTO 20, 9, 'ESC: VOLVER'

InversoEsperarSalida:
    MOV AH, 00H
    INT 16H

    CMP AL, 27
    JNE InversoEsperarSalida

InversoSalir:
    POP SI
    POP BX
    POP AX
    RET
IniciarModoInverso ENDP
