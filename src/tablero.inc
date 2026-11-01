TAB_PANEL EQU 22
TAB_CASILLA EQU 23
TAB_BORDE EQU 24

; Entrada: reglas de partida. Salida: tablero completo; conserva registros.
PantallaJuego PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    CALL IniciarTransicion
    CALL LimpiarPantalla
    ; Zonas del enunciado: encabezado, tablero, lateral y barra inferior.
    PINTAR_RECT 0, 0, 320, 20, 21
    PINTAR_RECT 4, 22, 196, 168, TAB_PANEL
    PINTAR_RECT 204, 22, 112, 168, TAB_PANEL
    PINTAR_RECT 0, 190, 320, 10, 21
    PINTAR_RECT 4, 22, 196, 1, TAB_BORDE
    PINTAR_RECT 4, 189, 196, 1, TAB_BORDE
    PINTAR_RECT 4, 22, 1, 168, TAB_BORDE
    PINTAR_RECT 199, 22, 1, 168, TAB_BORDE
    PINTAR_RECT 204, 22, 112, 1, TAB_BORDE
    PINTAR_RECT 315, 22, 1, 168, TAB_BORDE
    PINTAR_IMAGEN 8, 3, 48, 14, TextoClave, TextoClaveAncho, TextoClaveAlto
    PINTAR_IMAGEN 62, 3, 82, 14, TextoEspectral, TextoEspectralAncho, TextoEspectralAlto
    MOV SI, 24
    XOR CX, CX
    MOV CL, IntentosMaximos
TabFila:
    PUSH CX
    MOV BX, 24
    XOR CX, CX
    MOV CL, CantidadPosiciones
TabCasilla:
    PINTAR_RECT BX, SI, 16, 12, TAB_CASILLA
    ADD BX, 22
    LOOP TabCasilla
    PINTAR_RECT 144, SI, 20, 12, TAB_CASILLA
    PINTAR_RECT 176, SI, 20, 12, TAB_CASILLA
    ADD SI, 13
    POP CX
    LOOP TabFila
    CALL RedibujarPanelFichas
    CALL MostrarEstado
    CALL AyudaJuego
    CALL DibujarCursorJuego
    CALL TerminarTransicion
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
PantallaJuego ENDP
