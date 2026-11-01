; Dibujo comun de cualquier personaje.
; Entrada: AL=1..8, BX=X, DX=Y, CX=tamano cuadrado. Conserva registros.
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

; Entrada: FichaSeleccionada. Salida: BX=X, DX=Y; conserva AX.
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

; Dibuja las 6 u 8 fichas sin cambiar la seleccion. Conserva registros.
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
    ; Pintar los margenes sin borrar el personaje que ya se ve.
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
    ; Etiqueta 1..8 segun el orden de las fichas.
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

; Cursor de posicion: BX=X, DX=Y calculados del estado. Conserva AX,BX,DX.
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

; Borra el cursor anterior. Conserva AX,BX,DX.
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

; Dibuja el contenido real de la posicion, incluso si esta vacia. Conserva registros.
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
    ; Altura 12: dejar el borde inferior para el cursor.
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

; Numero decimal de dos cifras. AL=0..99; DH=fila, DL=columna. Conserva registros.
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

; Indicadores persistentes en encabezado y lateral. Conserva AX,DX.
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
    ESCRIBIR_TEXTO 21, 26, 'PRUEBA MANUAL'
EstadoFin:
    POP DX
    POP AX
    RET
MostrarEstado ENDP

; Dos numeros en la fila evaluada; verde=toros, naranja=vacas. Conserva AX,DX.
DibujarPistas PROC
    PUSH BX
    PUSH CX
    PUSH AX
    PUSH DX
    ; El texto BIOS usa filas de 8: dibujar numeros a pixel exacto con la fuente ROM.
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

; Barra inferior: solo se actualiza ante una accion, no en cada espera.
; Sin parametros ni retorno; conserva AX mediante la macro.
AyudaJuego PROC
    PUSH AX
    PINTAR_RECT 0, 190, 320, 10, 21
    ESCRIBIR_TEXTO 24, 0, '1-8:FICHA ENTER:PROBAR BS:BORRA ESC:MENU'
    POP AX
    RET
AyudaJuego ENDP

; Sin parametros. Muestra aviso sin consumir turno; conserva AX.
AvisarIncompleto PROC
    PUSH AX
    PINTAR_RECT 0, 190, 320, 10, 21
    ESCRIBIR_TEXTO 24, 1, 'COMPLETA TODAS LAS CASILLAS PRIMERO'
    POP AX
    RET
AvisarIncompleto ENDP

; Sin parametros. Muestra aviso de ficha repetida; conserva AX.
AvisarRepetida PROC
    PUSH AX
    PINTAR_RECT 0, 190, 320, 10, 21
    ESCRIBIR_TEXTO 24, 1, 'FACIL: NO SE PERMITEN FICHAS REPETIDAS'
    POP AX
    RET
AvisarRepetida ENDP

; Digito de 3x5 pixeles. AL=ASCII, BX=X, DX=Y, CL=color. Conserva registros.
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

; Pantalla de resultado y clave revelada. Conserva registros.
PantallaFin PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    CALL LimpiarPantalla
    PINTAR_RECT 4, 4, 312, 184, 21
    CMP PartidaTerminada, 1
    JNE FinDerrota
    ESCRIBIR_TEXTO 3, 12, 'CLAVE DESCUBIERTA!'
    JMP FinMostrarClave
FinDerrota:
    ESCRIBIR_TEXTO 3, 13, 'SIN INTENTOS'
FinMostrarClave:
    ESCRIBIR_TEXTO 6, 12, 'LA CLAVE ERA:'
    MOV BX, 92
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
    ESCRIBIR_TEXTO 12, 10, 'INTENTOS USADOS:'
    MOV AL, TurnoActual
    MOV DH, 12
    MOV DL, 27
    CALL EscribirNumero
    ESCRIBIR_TEXTO 20, 8, 'ENTER: JUGAR OTRA VEZ'
    ESCRIBIR_TEXTO 22, 10, 'M / ESC: MENU'
    CALL DibujarCuadroFinal
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
PantallaFin ENDP

; Editor de secreto para la revision: F2 desde el menu.
; Devuelve CF=1 si se cancela, CF=0 si confirma. Conserva registros.
IntroducirCodigo PROC
    PUSH AX
    PUSH BX
    PUSH DX
    PUSH SI
    MOV ClaveManual, 1
    ; Se permiten repeticiones para probar los casos de la tabla 2.
    MOV PermitirRepetidos, 1
    CALL LimpiarPantalla
    ESCRIBIR_TEXTO 3, 7, 'CLAVE MANUAL PARA PRUEBAS'
    ESCRIBIR_TEXTO 6, 3, 'ESCRIBE 1-6 (1-8 EN DIFICIL)'
    ESCRIBIR_TEXTO 8, 3, 'ENTER: ACEPTAR  BACKSPACE: BORRAR'
    ESCRIBIR_TEXTO 10, 3, 'ESC: CANCELAR. ADMITE REPETIDOS.'
    XOR SI, SI
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
    POP SI
    POP DX
    POP BX
    POP AX
    RET
IntroducirCodigo ENDP

; SI=cantidad de digitos del editor; conserva registros.
MostrarClaveManual PROC
    PUSH AX
    PUSH BX
    PUSH DX
    PINTAR_RECT 80, 112, 128, 8, 8
    XOR BX, BX
ManualMostrarOtro:
    CMP BX, SI
    JAE ManualMostrarFin
    MOV AX, BX
    MOV DL, 3
    MUL DL
    ADD AL, 10
    MOV DL, AL
    MOV DH, 14
    MOV AL, CodigoSecreto[BX]
    CALL EscribirNumero
    INC BX
    JMP ManualMostrarOtro
ManualMostrarFin:
    POP DX
    POP BX
    POP AX
    RET
MostrarClaveManual ENDP
