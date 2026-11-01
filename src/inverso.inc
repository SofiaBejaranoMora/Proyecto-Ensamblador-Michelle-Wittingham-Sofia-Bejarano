; Modo inverso: jugador crea la clave y computadora intenta descubrirla.
; Las rutinas auxiliares van primero; IniciarModoInverso queda al final.

; Compara el candidato con todas las pistas anteriores.
; Entrada: InversoCandidatoFicha. Salida: CF=1 si es compatible.
InversoCompatible PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    PUSH DI

    ; Probar el candidato como secreto temporal.
    XOR SI, SI
    MOV CX, 5
InversoCopiarCandidato:
    MOV AL, InversoCandidatoFicha[SI]
    MOV CodigoSecreto[SI], AL
    INC SI
    LOOP InversoCopiarCandidato

    MOV InversoEsCompatible, 1
    XOR SI, SI
InversoCompararPista:
    XOR AX, AX
    MOV AL, TurnoActual
    CMP SI, AX
    JAE InversoRestaurarSecreto

    ; Recuperar la combinacion de un intento anterior.
    MOV AX, SI
    MOV BX, 5
    MUL BX
    MOV DI, AX
    XOR BX, BX
InversoCopiarHistorico:
    MOV AL, Historial[DI]
    MOV IntentoActual[BX], AL
    INC DI
    INC BX
    CMP BX, 5
    JB InversoCopiarHistorico

    CALL EvaluarIntento
    MOV AL, AciertosExactos
    CMP AL, HistorialToros[SI]
    JNE InversoCandidatoDescartado
    MOV AL, AciertosOtraPosicion
    CMP AL, HistorialVacas[SI]
    JNE InversoCandidatoDescartado
    INC SI
    JMP InversoCompararPista

InversoCandidatoDescartado:
    MOV InversoEsCompatible, 0

InversoRestaurarSecreto:
    XOR SI, SI
    MOV CX, 5
InversoCopiarSecreto:
    MOV AL, InversoClaveJugador[SI]
    MOV CodigoSecreto[SI], AL
    INC SI
    LOOP InversoCopiarSecreto

    CMP InversoEsCompatible, 1
    JNE InversoNoCompatible
    STC
    JMP InversoFinCompatible
InversoNoCompatible:
    CLC
InversoFinCompatible:
    POP DI
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
InversoCompatible ENDP

; Devuelve CF=1 si el candidato cumple la regla de no repeticion.
InversoSinRepetidos PROC
    PUSH AX
    PUSH BX
    PUSH SI
    PUSH DI

    CMP PermitirRepetidos, 1
    JE InversoCandidatoValido
    XOR SI, SI
InversoRevisarPosicion:
    XOR DI, DI
InversoRevisarAnterior:
    CMP DI, SI
    JAE InversoSiguientePosicion
    MOV AL, InversoCandidatoFicha[SI]
    CMP AL, InversoCandidatoFicha[DI]
    JE InversoCandidatoRepetido
    INC DI
    JMP InversoRevisarAnterior
InversoSiguientePosicion:
    INC SI
    XOR AX, AX
    MOV AL, CantidadPosiciones
    CMP SI, AX
    JB InversoRevisarPosicion

InversoCandidatoValido:
    STC
    JMP InversoFinRepetidos
InversoCandidatoRepetido:
    CLC
InversoFinRepetidos:
    POP DI
    POP SI
    POP BX
    POP AX
    RET
InversoSinRepetidos ENDP

; Enumera claves posibles y descarta las incompatibles con las pistas.
; Salida: CF=1 y nuevo IntentoActual, o CF=0 si no queda ninguna.
InversoBuscarIntento PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI

InversoOtroCandidato:
    MOV AX, InversoIndiceCandidato
    CMP AX, InversoTotalCandidatos
    JAE InversoNoHayCandidato
    INC InversoIndiceCandidato

    ; Convertir numero de candidato a fichas 1..6 u 1..8.
    MOV InversoCandidatoFicha[4], 0
    XOR SI, SI
InversoCrearDigito:
    XOR DX, DX
    XOR BX, BX
    MOV BL, CantidadFichas
    DIV BX
    INC DL
    MOV InversoCandidatoFicha[SI], DL
    INC SI
    XOR CX, CX
    MOV CL, CantidadPosiciones
    CMP SI, CX
    JB InversoCrearDigito

    CALL InversoSinRepetidos
    JNC InversoOtroCandidato
    CALL InversoCompatible
    JNC InversoOtroCandidato

    ; Guardar como nuevo intento la clave que sobrevivio al filtro.
    XOR SI, SI
    MOV CX, 5
InversoCopiarIntento:
    MOV AL, InversoCandidatoFicha[SI]
    MOV IntentoActual[SI], AL
    INC SI
    LOOP InversoCopiarIntento
    STC
    JMP InversoFinBuscar

InversoNoHayCandidato:
    CLC
InversoFinBuscar:
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
InversoBuscarIntento ENDP

; Evalua, registra y dibuja un intento de la computadora.
; Actualiza PartidaTerminada al ganar (1) o agotar turnos (2).
InversoEjecutarIntento PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI

    CALL EvaluarIntento
    CALL GuardarHistorial

    XOR SI, SI
    XOR CX, CX
    MOV CL, CantidadPosiciones
    MOV BX, 26
    MOV DX, FilaJuego
InversoDibujarPersonaje:
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    MOV AL, IntentoActual[SI]
    MOV CX, 12
    MOV FondoSprite, TAB_CASILLA
    CALL DibujarPersonaje
    MOV FondoSprite, 0
    POP SI
    POP DX
    POP CX
    POP BX
    INC SI
    ADD BX, 22
    LOOP InversoDibujarPersonaje
    CALL DibujarPistas
    CALL SonidoSeleccion
    INC TurnoActual

    MOV AL, AciertosExactos
    CMP AL, CantidadPosiciones
    JNE InversoRevisarLimite
    MOV PartidaTerminada, 1
    MOV RelojActivo, 0
    PINTAR_RECT 0, 190, 320, 10, 21
    ESCRIBIR_TEXTO 24, 1, 'COMPUTADORA DESCUBRIO TU CLAVE  ESC'
    JMP InversoFinEjecutar

InversoRevisarLimite:
    MOV AL, TurnoActual
    CMP AL, IntentosMaximos
    JB InversoPrepararSiguiente
    MOV PartidaTerminada, 2
    MOV RelojActivo, 0
    PINTAR_RECT 0, 190, 320, 10, 21
    ESCRIBIR_TEXTO 24, 1, 'GANASTE: NO DESCUBRIO TU CLAVE  ESC'
    JMP InversoFinEjecutar

InversoPrepararSiguiente:
    ADD FilaJuego, 13
    CALL MostrarEstado
    PINTAR_RECT 0, 190, 320, 10, 21
    ESCRIBIR_TEXTO 24, 1, 'ENTER: SIGUIENTE INTENTO  ESC: MENU'

InversoFinEjecutar:
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
InversoEjecutarIntento ENDP

; Controla los intentos de la computadora hasta ganar, perder o ESC.
InversoJugarComputadora PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH SI

    ; Guardar fuera del evaluador la clave real del jugador.
    XOR SI, SI
    MOV CX, 5
InversoGuardarClave:
    MOV AL, CodigoSecreto[SI]
    MOV InversoClaveJugador[SI], AL
    INC SI
    LOOP InversoGuardarClave

    MOV InversoIndiceCandidato, 0
    MOV InversoTotalCandidatos, 1296  ; 6^4
    CMP NivelActual, 2
    JNE InversoPrepararPrimero
    MOV InversoTotalCandidatos, 32768 ; 8^5

InversoPrepararPrimero:
    CALL PantallaJuego
    CALL BorrarCursorJuego
    CALL IniciarCronometro

    ; Primer intento fijo para poder verificar el algoritmo.
    XOR SI, SI
    XOR CX, CX
    MOV CL, CantidadPosiciones
    MOV AL, 1
InversoFichaInicial:
    MOV IntentoActual[SI], AL
    INC SI
    INC AL
    LOOP InversoFichaInicial
    CALL InversoEjecutarIntento

InversoEsperarComputadora:
    CALL ActualizarSonido
    CALL ActualizarCronometro
    MOV AH, 01H
    INT 16H
    JZ InversoEsperarComputadora
    MOV AH, 00H
    INT 16H
    CMP AL, 27
    JE InversoTerminarComputadora
    CMP PartidaTerminada, 0
    JNE InversoEsperarComputadora
    CMP AL, 13
    JNE InversoEsperarComputadora

    CALL InversoBuscarIntento
    JC InversoCandidatoEncontrado
    ; No deberia suceder con pistas consistentes.
    MOV PartidaTerminada, 2
    MOV RelojActivo, 0
    PINTAR_RECT 0, 190, 320, 10, 21
    ESCRIBIR_TEXTO 24, 1, 'NO HAY CANDIDATOS COMPATIBLES  ESC'
    JMP InversoEsperarComputadora

InversoCandidatoEncontrado:
    CALL InversoEjecutarIntento
    JMP InversoEsperarComputadora

InversoTerminarComputadora:
    MOV RelojActivo, 0
    POP SI
    POP CX
    POP BX
    POP AX
    RET
InversoJugarComputadora ENDP

; Entrada de la clave por parte del jugador.
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
    JMP InversoMostrarRegla
InversoSeisFichas:
    ESCRIBIR_TEXTO 6, 3, 'ELIGE FICHAS DEL 1 AL 6'
InversoMostrarRegla:
    CMP PermitirRepetidos, 0
    JNE InversoReglaRepeticiones
    ESCRIBIR_TEXTO 10, 3, 'SIN FICHAS REPETIDAS'
    JMP InversoIniciarEntrada
InversoReglaRepeticiones:
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
    CALL InversoJugarComputadora
InversoSalir:
    POP SI
    POP BX
    POP AX
    RET
IniciarModoInverso ENDP
