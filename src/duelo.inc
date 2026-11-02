GuardarClaveMulti PROC
    PUSH AX
    PUSH CX
    PUSH SI
    XOR SI, SI
    MOV CX, 5
MultiCopiarSecreto:
    MOV AL, CodigoSecreto[SI]
    MOV ClaveMulti[SI], AL
    INC SI
    LOOP MultiCopiarSecreto
    POP SI
    POP CX
    POP AX
    RET
GuardarClaveMulti ENDP

RestaurarClaveMulti PROC
    PUSH AX
    PUSH CX
    PUSH SI
    XOR SI, SI
    MOV CX, 5
MultiRecuperarSecreto:
    MOV AL, ClaveMulti[SI]
    MOV CodigoSecreto[SI], AL
    INC SI
    LOOP MultiRecuperarSecreto
    POP SI
    POP CX
    POP AX
    RET
RestaurarClaveMulti ENDP

MostrarJugadorMulti PROC
    CMP ModoMulti, 0
    JE MultiIndicadorFin
    PUSH AX
    MOV FondoTexto, 22
    MOV ColorTexto, 7
    CMP JugadorMulti, 0
    JNE MultiIndicadorDos
    ESCRIBIR_TEXTO 22, 26, 'JUGADOR 1'
    JMP MultiIndicadorListo
MultiIndicadorDos:
    ESCRIBIR_TEXTO 22, 26, 'JUGADOR 2'
MultiIndicadorListo:
    CALL CerrarEstilo
    POP AX
MultiIndicadorFin:
    RET
MostrarJugadorMulti ENDP

GuardarTurnoMulti PROC
    PUSH AX
    PUSH BX
    PUSH SI
    XOR BX, BX
    MOV BL, JugadorMulti
    MOV AL, PartidaTerminada
    MOV ResultadoMulti[BX], AL
    MOV AL, TurnoActual
    MOV IntentosMulti[BX], AL
    MOV SI, BX
    SHL SI, 1
    MOV AX, SegundosJuego
    MOV TiempoMulti[SI], AX
    POP SI
    POP BX
    POP AX
    RET
GuardarTurnoMulti ENDP

CompararDuelo PROC
    PUSH AX
    MOV GanadorMulti, 0
    CMP ResultadoMulti[0], 1
    JNE MultiPrimeroNoGano
    CMP ResultadoMulti[1], 1
    JNE MultiGanaUno
    MOV AL, IntentosMulti[0]
    CMP AL, IntentosMulti[1]
    JB MultiGanaUno
    JA MultiGanaDos
    MOV AX, TiempoMulti[0]
    CMP AX, TiempoMulti[2]
    JB MultiGanaUno
    JA MultiGanaDos
    JMP MultiCompararFin
MultiPrimeroNoGano:
    CMP ResultadoMulti[1], 1
    JE MultiGanaDos
    JMP MultiCompararFin
MultiGanaUno:
    MOV GanadorMulti, 1
    JMP MultiCompararFin
MultiGanaDos:
    MOV GanadorMulti, 2
MultiCompararFin:
    POP AX
    RET
CompararDuelo ENDP

VaciarTeclasMulti PROC
    PUSH AX
MultiVaciarOtra:
    MOV AH, 01H
    INT 16H
    JZ MultiTeclasVacias
    MOV AH, 00H
    INT 16H
    JMP MultiVaciarOtra
MultiTeclasVacias:
    POP AX
    RET
VaciarTeclasMulti ENDP

PantallaTurnoMulti PROC
    PUSH AX
    CALL AbrirLienzo
    ESCRIBIR_TEXTO 1, 3, 'DOS PERSONAS, UNA CLAVE'
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 3, 3, 'Misma dificultad. Un tablero nuevo.'
    PINTAR_RECT 24, 56, 272, 80, 22
    MOV FondoTexto, 22
    MOV ColorTexto, 3
    CMP JugadorMulti, 0
    JNE MultiEntregaDos
    ESCRIBIR_TEXTO 8, 7, 'TURNO DEL JUGADOR 1'
    PINTAR_IMAGEN 136, 80, 40, 40, SpriteGato, 16, 16
    JMP MultiEntregaAyuda
MultiEntregaDos:
    ESCRIBIR_TEXTO 8, 7, 'TURNO DEL JUGADOR 2'
    PINTAR_IMAGEN 136, 80, 40, 40, SpriteCalabaza, 16, 16
MultiEntregaAyuda:
    MOV FondoTexto, 21
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 18, 3, 'Que mire solo quien va a jugar.'
    PINTAR_RECT 32, 164, 256, 20, 25
    MOV FondoTexto, 25
    MOV ColorTexto, 3
    ESCRIBIR_TEXTO 21, 6, 'ENTER  Estoy listo'
    MOV FondoTexto, 21
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 24, 3, 'ESC  Cancelar el duelo'
    CALL CerrarEstilo
    CALL TerminarTransicion
    POP AX
    RET
PantallaTurnoMulti ENDP

EntregarTurnoMulti PROC
    PUSH AX
    CALL PantallaTurnoMulti
    CALL VaciarTeclasMulti
MultiEntregaEsperar:
    MOV AH, 00H
    INT 16H
    CMP AL, 13
    JE MultiEntregaAceptar
    CMP AL, 27
    JNE MultiEntregaEsperar
    POP AX
    STC
    RET
MultiEntregaAceptar:
    POP AX
    CLC
    RET
EntregarTurnoMulti ENDP

ControlarTurnoMulti PROC
    PUSH AX
    CALL IniciarCronometro
MultiTurnoEsperar:
    CALL ActualizarCronometro
    CALL ActualizarSonido
    CALL AnimarSeleccion
    MOV AH, 01H
    INT 16H
    JZ MultiTurnoEsperar
    MOV AH, 00H
    INT 16H
    CMP AL, 27
    JE MultiTurnoCancelar
    CALL EditarIntento
    CMP PartidaTerminada, 0
    JE MultiTurnoEsperar
    CALL GuardarTurnoMulti
    CALL ApagarSonido
    POP AX
    CLC
    RET
MultiTurnoCancelar:
    MOV RelojActivo, 0
    CALL ApagarSonido
    POP AX
    STC
    RET
ControlarTurnoMulti ENDP

DibujarResultadoMulti PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PINTAR_RECT 16, DX, 288, 48, 22
    PINTAR_RECT 16, DX, 2, 48, 7
    ADD DX, 8
    CMP SI, 0
    JNE MultiResultadoIconoDos
    PINTAR_IMAGEN 32, DX, 24, 24, SpriteGato, 16, 16
    JMP MultiResultadoTexto
MultiResultadoIconoDos:
    PINTAR_IMAGEN 32, DX, 24, 24, SpriteCalabaza, 16, 16
MultiResultadoTexto:
    MOV AX, DX
    MOV CL, 3
    SHR AX, CL
    MOV DH, AL
    MOV FondoTexto, 22
    MOV ColorTexto, 3
    CMP SI, 0
    JNE MultiResultadoNombreDos
    ESCRIBIR_TEXTO DH, 10, 'Jugador 1'
    JMP MultiResultadoEstado
MultiResultadoNombreDos:
    ESCRIBIR_TEXTO DH, 10, 'Jugador 2'
MultiResultadoEstado:
    ADD DH, 2
    MOV ColorTexto, 26
    CMP ResultadoMulti[SI], 1
    JNE MultiResultadoPerdio
    ESCRIBIR_TEXTO DH, 10, 'Clave encontrada'
    JMP MultiResultadoMetricas
MultiResultadoPerdio:
    ESCRIBIR_TEXTO DH, 10, 'Sin intentos'
MultiResultadoMetricas:
    ADD DH, 2
    ESCRIBIR_TEXTO DH, 10, 'Intentos'
    MOV ColorTexto, 3
    MOV AL, IntentosMulti[SI]
    MOV DL, 19
    CALL EscribirNumero
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO DH, 23, 'Tiempo'
    MOV BX, SI
    SHL BX, 1
    MOV AX, TiempoMulti[BX]
    MOV DL, 30
    MOV ColorTexto, 3
    CALL EscribirTiempo
    POP DX
    POP CX
    POP BX
    POP AX
    RET
DibujarResultadoMulti ENDP

PantallaDuelo PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    CALL AbrirLienzo
    MOV ColorTexto, 7
    CMP GanadorMulti, 0
    JE MultiTituloEmpate
    CMP GanadorMulti, 1
    JNE MultiTituloDos
    ESCRIBIR_TEXTO 1, 3, 'GANA EL JUGADOR 1!'
    JMP MultiResumenTarjetas
MultiTituloDos:
    ESCRIBIR_TEXTO 1, 3, 'GANA EL JUGADOR 2!'
    JMP MultiResumenTarjetas
MultiTituloEmpate:
    ESCRIBIR_TEXTO 1, 3, 'ESTA VEZ, EMPATE!'
MultiResumenTarjetas:
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 3, 3, 'Una clave, dos maneras de resolverla.'
    MOV DX, 48
    XOR SI, SI
    CALL DibujarResultadoMulti
    MOV DX, 104
    INC SI
    CALL DibujarResultadoMulti
    MOV FondoTexto, 21
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 20, 3, 'La clave'
    MOV BX, 112
    MOV DX, 156
    XOR SI, SI
MultiRevelarFicha:
    PINTAR_RECT BX, DX, 20, 20, 23
    MOV AL, ClaveMulti[SI]
    ADD BX, 2
    ADD DX, 2
    MOV CX, 16
    CALL DibujarPersonaje
    SUB DX, 2
    ADD BX, 20
    INC SI
    XOR AX, AX
    MOV AL, CantidadPosiciones
    CMP SI, AX
    JB MultiRevelarFicha
    ESCRIBIR_TEXTO 23, 3, 'ENTER  Revancha'
    ESCRIBIR_TEXTO 24, 3, 'ESC  Volver al menu'
    CALL CerrarEstilo
    CALL TerminarTransicion
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
PantallaDuelo ENDP

IniciarMultijugador PROC
    PUSH AX
MultiNuevaPartida:
    MOV ModoMulti, 0
    CALL IniciarPartida
    CALL GuardarClaveMulti
    MOV ModoMulti, 1
    MOV JugadorMulti, 0
    MOV ResultadoMulti[0], 0
    MOV ResultadoMulti[1], 0
    MOV IntentosMulti[0], 0
    MOV IntentosMulti[1], 0
    MOV TiempoMulti[0], 0
    MOV TiempoMulti[2], 0
MultiSiguienteJugador:
    CALL EntregarTurnoMulti
    JC MultiSalir
    CALL IniciarPartida
    CALL PantallaJuego
    CALL ControlarTurnoMulti
    JC MultiSalir
    CMP JugadorMulti, 0
    JNE MultiTerminarDuelo
    INC JugadorMulti
    JMP MultiSiguienteJugador
MultiTerminarDuelo:
    CALL CompararDuelo
    CALL PantallaDuelo
    CALL VaciarTeclasMulti
MultiEsperarResultado:
    MOV AH, 00H
    INT 16H
    CMP AL, 13
    JE MultiNuevaPartida
    CMP AL, 27
    JNE MultiEsperarResultado
MultiSalir:
    MOV ModoMulti, 0
    MOV JugadorMulti, 0
    MOV RelojActivo, 0
    CALL ApagarSonido
    POP AX
    RET
IniciarMultijugador ENDP
