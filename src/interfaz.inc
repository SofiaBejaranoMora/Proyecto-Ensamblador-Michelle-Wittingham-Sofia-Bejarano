DibujarPersonaje PROC
    PUSH AX
    PUSH SI
    PUSH DI
    XOR AH, AH
    DEC AX
    MOV SI, AX
    SHL SI, 1
    MOV DI, Fichas[SI]
    CMP CuadroAnimacion, 0
    JE PersonajeDibujar
    MOV DI, FichasAnimadas[SI]
PersonajeDibujar:
    PUSH BX
    PUSH DX
    PUSH CX
    PUSH CX
    MOV AX, 16
    PUSH AX
    PUSH AX
    PUSH DI
    CALL DibujarSprite
    POP DI
    POP SI
    POP AX
    RET
DibujarPersonaje ENDP

CalcularMarcaFicha PROC
    PUSH AX
    MOV AL, FichaSeleccionada
    DEC AL
    XOR AH, AH
    MOV BL, 3
    DIV BL
    PUSH AX
    MOV AL, AH
    XOR AH, AH
    MOV BX, 32
    MUL BX
    ADD AX, 212
    MOV BX, AX
    POP AX
    XOR AH, AH
    MOV DL, 30
    MUL DL
    ADD AX, 30
    MOV DX, AX
    POP AX
    RET
CalcularMarcaFicha ENDP

RedibujarPanelFichas PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    PUSH DI
    XOR AX, AX
    MOV AL, FichaSeleccionada
    PUSH AX
    MOV SI, 1
PanelOtraFicha:
    MOV AX, SI
    MOV FichaSeleccionada, AL
    CALL CalcularMarcaFicha

    PINTAR_RECT BX, DX, 28, 2, TAB_CASILLA
    ADD DX, 2
    PINTAR_RECT BX, DX, 6, 16, TAB_CASILLA
    ADD BX, 22
    PINTAR_RECT BX, DX, 6, 16, TAB_CASILLA
    SUB BX, 22
    ADD DX, 16
    PINTAR_RECT BX, DX, 28, 8, TAB_CASILLA
    SUB DX, 18
    ADD BX, 6
    ADD DX, 2
    MOV AX, SI
    MOV CX, 16
    MOV FondoSprite, TAB_CASILLA
    CALL DibujarPersonaje
    MOV FondoSprite, 0

    ADD BX, 14
    ADD DX, 18
    MOV AX, SI
    ADD AL, '0'
    MOV CL, 8
    CALL DibujarDigito
    INC SI
    XOR AX, AX
    MOV AL, CantidadFichas
    CMP SI, AX
    JB PanelOtraFicha
    JE PanelOtraFicha
    POP AX
    MOV FichaSeleccionada, AL
    CALL CalcularMarcaFicha
    ADD DX, 25
    PINTAR_RECT BX, DX, 28, 1, 7
    POP DI
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
RedibujarPanelFichas ENDP

DibujarCursorJuego PROC
    PUSH AX
    PUSH BX
    PUSH DX
    MOV AL, PosicionActual
    MOV BL, 22
    MUL BL
    ADD AX, 24
    MOV BX, AX
    MOV DX, FilaJuego
    ADD DX, 11
    PINTAR_RECT BX, DX, 16, 1, 7
    POP DX
    POP BX
    POP AX
    RET
DibujarCursorJuego ENDP

BorrarCursorJuego PROC
    PUSH AX
    PUSH BX
    PUSH DX
    MOV AL, PosicionActual
    MOV BL, 22
    MUL BL
    ADD AX, 24
    MOV BX, AX
    MOV DX, FilaJuego
    ADD DX, 11
    PINTAR_RECT BX, DX, 16, 1, TAB_CASILLA
    POP DX
    POP BX
    POP AX
    RET
BorrarCursorJuego ENDP

DibujarFichaActual PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    XOR AX, AX
    MOV AL, PosicionActual
    MOV SI, AX
    MOV BL, 22
    MUL BL
    ADD AX, 24
    MOV BX, AX
    MOV DX, FilaJuego
    PINTAR_RECT BX, DX, 16, 11, TAB_CASILLA
    MOV AL, IntentoActual[SI]
    CMP AL, 0
    JE FichaActualFin
    ADD BX, 2
    MOV CX, 12

    CALL DibujarPersonaje
    CALL DibujarCursorJuego
FichaActualFin:
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
DibujarFichaActual ENDP

EscribirNumero PROC
    PUSH AX
    PUSH BX
    PUSH SI
    PUSH DS
    XOR AH, AH
    MOV BL, 10
    DIV BL
    ADD AX, 3030H
    XOR BX, BX
    PUSH BX
    PUSH AX
    MOV SI, SP
    PUSH SS
    POP DS
    CALL EscribirTextoDificultad
    ADD SP, 4
    POP DS
    POP SI
    POP BX
    POP AX
    RET
EscribirNumero ENDP

MostrarEstado PROC
    PUSH AX
    PUSH DX
    PINTAR_RECT 160, 0, 160, 20, 21
    CMP NivelActual, 0
    JNE EstadoNormal
    ESCRIBIR_TEXTO 1, 21, 'NIVEL: FACIL'
    JMP EstadoContadores
EstadoNormal:
    CMP NivelActual, 1
    JNE EstadoDificil
    ESCRIBIR_TEXTO 1, 21, 'NIVEL: NORMAL'
    JMP EstadoContadores
EstadoDificil:
    ESCRIBIR_TEXTO 1, 21, 'NIVEL: DIFICIL'
EstadoContadores:
    PINTAR_RECT 208, 124, 104, 36, TAB_PANEL
    ESCRIBIR_TEXTO 16, 26, 'INTENTO:'
    MOV AL, TurnoActual
    INC AL
    MOV DH, 16
    MOV DL, 35
    CALL EscribirNumero
    ESCRIBIR_TEXTO 18, 26, 'QUEDAN:'
    MOV AL, IntentosMaximos
    SUB AL, TurnoActual
    MOV DH, 18
    MOV DL, 35
    CALL EscribirNumero
    CMP ClaveManual, 0
    JE EstadoFin
    ESCRIBIR_TEXTO 22, 26, 'PRUEBA MANUAL'
EstadoFin:
    CALL MostrarJugadorMulti
    CALL MostrarTiempoJuego
    POP DX
    POP AX
    RET
MostrarEstado ENDP

DibujarPistas PROC
    PUSH BX
    PUSH CX
    PUSH AX
    PUSH DX

    MOV AL, AciertosExactos
    ADD AL, '0'
    MOV BX, 150
    MOV DX, FilaJuego
    ADD DX, 2
    MOV CL, 7
    CALL DibujarDigito
    MOV AL, AciertosOtraPosicion
    ADD AL, '0'
    MOV BX, 182
    MOV CL, 12
    CALL DibujarDigito
    POP DX
    POP AX
    POP CX
    POP BX
    RET
DibujarPistas ENDP

AyudaJuego PROC
    PUSH AX
    PINTAR_RECT 0, 190, 320, 10, 21
    ESCRIBIR_TEXTO 24, 0, '1-8:FICHA ENTER:PROBAR BS:BORRA ESC:MENU'
    POP AX
    RET
AyudaJuego ENDP

AvisarIncompleto PROC
    PUSH AX
    PINTAR_RECT 0, 190, 320, 10, 21
    ESCRIBIR_TEXTO 24, 1, 'COMPLETA TODAS LAS CASILLAS PRIMERO'
    POP AX
    RET
AvisarIncompleto ENDP

AvisarRepetida PROC
    PUSH AX
    PINTAR_RECT 0, 190, 320, 10, 21
    ESCRIBIR_TEXTO 24, 1, 'FACIL: NO SE PERMITEN FICHAS REPETIDAS'
    POP AX
    RET
AvisarRepetida ENDP

DibujarDigito PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    PUSH DI
    PUSH BP
    SUB AL, '0'
    XOR AH, AH
    MOV SI, 5
    PUSH DX
    MUL SI
    POP DX
    MOV SI, AX
    XOR CH, CH
    MOV BP, 5
DigitoFila:
    MOV AL, Digitos[SI]
    MOV DI, 3
DigitoPixel:
    TEST AL, 4
    JZ DigitoTransparente
    PUSH AX
    PINTAR_RECT BX, DX, 1, 1, CX
    POP AX
DigitoTransparente:
    SHL AL, 1
    INC BX
    DEC DI
    JNZ DigitoPixel
    SUB BX, 3
    INC DX
    INC SI
    DEC BP
    JNZ DigitoFila
    POP BP
    POP DI
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
DibujarDigito ENDP

PantallaFin PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    CALL AbrirLienzo
    CMP PartidaTerminada, 1
    JNE FinDerrota
    MOV ColorTexto, 7
    ESCRIBIR_TEXTO 1, 3, 'LA MAGIA ERA TUYA!'
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 3, 3, 'Descubriste la clave. Bien jugado.'
    JMP FinMostrarClave
FinDerrota:
    MOV ColorTexto, 18
    ESCRIBIR_TEXTO 1, 3, 'OTRA RONDA, OTRO MISTERIO'
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 3, 3, 'La proxima clave puede ser la tuya.'
FinMostrarClave:
    PINTAR_RECT 16, 44, 288, 48, 22
    MOV FondoTexto, 22
    ESCRIBIR_TEXTO 6, 12, 'La clave secreta'

    MOV AL, CantidadPosiciones
    MOV BL, 28
    MUL BL
    SUB AX, 4
    MOV BX, 320
    SUB BX, AX
    SHR BX, 1
    MOV DX, 64
    XOR SI, SI
FinOtraFicha:
    PINTAR_RECT BX, DX, 24, 24, TAB_CASILLA
    PUSH BX
    ADD BX, 4
    ADD DX, 4
    MOV AL, CodigoSecreto[SI]
    MOV CX, 16
    CALL DibujarPersonaje
    SUB DX, 4
    POP BX
    ADD BX, 28
    INC SI
    XOR AX, AX
    MOV AL, CantidadPosiciones
    CMP SI, AX
    JB FinOtraFicha
    PINTAR_RECT 24, 96, 272, 16, 22
    ESCRIBIR_TEXTO 12, 4, 'Intentos'
    MOV ColorTexto, 3
    MOV AL, TurnoActual
    MOV DH, 12
    MOV DL, 14
    CALL EscribirNumero
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 12, 21, 'Tiempo'
    MOV ColorTexto, 3
    MOV AX, SegundosJuego
    MOV DH, 12
    MOV DL, 29
    CALL EscribirTiempo
    MOV FondoTexto, 21
    MOV ColorTexto, 7
    CALL MostrarAvisoRecord
    PINTAR_RECT 32, 164, 256, 20, 25
    PINTAR_RECT 32, 164, 2, 20, 7
    MOV FondoTexto, 25
    MOV ColorTexto, 3
    ESCRIBIR_TEXTO 21, 6, 'ENTER  Una partida mas'
    MOV FondoTexto, 21
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 24, 3, 'ESC Menu'
    ESCRIBIR_TEXTO 24, 23, 'F3 Tus records'
    CALL DibujarCuadroFinal
    CALL CerrarEstilo
    CALL TerminarTransicion
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
PantallaFin ENDP

IntroducirCodigo PROC
    PUSH AX
    PUSH BX
    PUSH DX
    PUSH SI
    MOV ClaveManual, 1

    MOV PermitirRepetidos, 1
    CALL DibujarEditorClave
    XOR SI, SI
    CALL MostrarClaveManual
ManualEsperar:
    CALL ActualizarSonido
    MOV AH, 01H
    INT 16H
    JZ ManualEsperar
    MOV AH, 00H
    INT 16H
    CMP AL, 27
    JE ManualCancelar
    CMP AL, 8
    JE ManualBorrar
    CMP AL, 13
    JE ManualConfirmar
    CMP AL, '1'
    JB ManualEsperar
    SUB AL, '0'
    CMP AL, CantidadFichas
    JA ManualEsperar
    XOR BX, BX
    MOV BL, CantidadPosiciones
    CMP SI, BX
    JAE ManualEsperar
    MOV CodigoSecreto[SI], AL
    INC SI
    CALL MostrarClaveManual
    JMP ManualEsperar
ManualBorrar:
    CMP SI, 0
    JE ManualEsperar
    DEC SI
    MOV CodigoSecreto[SI], 0
    CALL MostrarClaveManual
    JMP ManualEsperar
ManualConfirmar:
    XOR BX, BX
    MOV BL, CantidadPosiciones
    CMP SI, BX
    JNE ManualEsperar
    CLC
    JMP ManualFin
ManualCancelar:
    STC
ManualFin:
    CALL CerrarEstilo
    POP SI
    POP DX
    POP BX
    POP AX
    RET
IntroducirCodigo ENDP

MostrarClaveManual PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH DI
    MOV FondoTexto, 25
    MOV ColorTexto, 3
    MOV AL, CantidadPosiciones
    MOV BL, 32
    MUL BL
    MOV DI, 320
    SUB DI, AX
    SHR DI, 1
    XOR BX, BX
ManualMostrarOtro:
    PINTAR_RECT DI, 108, 28, 36, 25
    MOV AX, DI
    ADD AX, 8
    MOV CL, 3
    SHR AX, CL
    MOV DL, AL
    MOV DH, 14
    CMP BX, SI
    JAE ManualMostrarVacio
    MOV AL, CodigoSecreto[BX]
    CALL EscribirNumero
    MOV AL, CodigoSecreto[BX]
    PUSH BX
    MOV BX, DI
    ADD BX, 6
    MOV DX, 124
    MOV CX, 16
    CALL DibujarPersonaje
    POP BX
    JMP ManualMostrarSiguiente
ManualMostrarVacio:
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO DH, DL, '--'
    MOV ColorTexto, 3
ManualMostrarSiguiente:
    ADD DI, 32
    INC BL
    CMP BL, CantidadPosiciones
    JB ManualMostrarOtro
    MOV FondoTexto, 21
    MOV ColorTexto, 26
    POP DI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
MostrarClaveManual ENDP
