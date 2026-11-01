; Archivo binario CE01: firma de 4 bytes + nueve entradas de 3 bytes.
; Cada nivel tiene tres puestos: menos intentos, y en empate menos segundos.

; Sin parametros. Inicializa la tabla vacia; conserva BX,CX.
VaciarRecords PROC
    PUSH BX
    PUSH CX
    XOR BX, BX
    MOV CX, 27
RecordVaciarOtro:
    MOV Records[BX], 255
    INC BX
    LOOP RecordVaciarOtro
    POP CX
    POP BX
    RET
VaciarRecords ENDP

; Entrada: BufferRecords con firma y 9 entradas.
; Salida: CF=1 si no es valido. Conserva AX,SI.
ValidarRecords PROC
    PUSH AX
    PUSH SI
    CMP WORD PTR BufferRecords, 4543H
    JNE RecordInvalido
    CMP WORD PTR BufferRecords+2, 3130H
    JNE RecordInvalido
    XOR SI, SI
RecordValidarOtro:
    MOV AL, BufferRecords[SI+4]
    CMP AL, 255
    JE RecordValidarVacio
    CMP AL, 1
    JB RecordInvalido
    CMP AL, 12
    JA RecordInvalido
    CMP SI, 9
    JB RecordValidarTiempo
    CMP AL, 10
    JA RecordInvalido
RecordValidarTiempo:
    CMP WORD PTR BufferRecords[SI+5], 5999
    JA RecordInvalido
    JMP RecordValidarSiguiente
RecordValidarVacio:
    CMP WORD PTR BufferRecords[SI+5], 0FFFFH
    JNE RecordInvalido
RecordValidarSiguiente:
    ADD SI, 3
    CMP SI, 27
    JB RecordValidarOtro
    POP SI
    POP AX
    CLC
    RET
RecordInvalido:
    POP SI
    POP AX
    STC
    RET
ValidarRecords ENDP

; Entrada: RutaRecords. Salida: tabla cargada o vacia; EstadoArchivo=1 si fallo.
; Archivo inexistente es normal en la primera ejecucion. Conserva registros.
CargarRecords PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    CALL VaciarRecords
    MOV EstadoArchivo, 0
    MOV DX, RutaRecords
    MOV AX, 3D00H
    INT 21H
    JNC RecordLeer
    CMP AX, 2
    JE RecordCargarFin
    JMP RecordLecturaFallo
RecordLeer:
    MOV BX, AX
    MOV DX, OFFSET BufferRecords
    MOV CX, 32
    MOV AH, 3FH
    INT 21H
    JC RecordLeerCerrarFallo
    MOV SI, AX
    MOV AH, 3EH
    INT 21H
    JC RecordLecturaFallo
    CMP SI, 31
    JNE RecordLecturaFallo
    CALL ValidarRecords
    JC RecordLecturaFallo
    XOR SI, SI
RecordCopiarLeido:
    MOV AL, BufferRecords[SI+4]
    MOV Records[SI], AL
    INC SI
    CMP SI, 27
    JB RecordCopiarLeido
    JMP RecordCargarFin
RecordLeerCerrarFallo:
    MOV AH, 3EH
    INT 21H
RecordLecturaFallo:
    MOV EstadoArchivo, 1
RecordCargarFin:
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
CargarRecords ENDP

; Entrada: tabla y RutaRecords. Salida: EstadoArchivo=2 ante error de escritura.
; Comprueba bytes escritos y cierre del archivo. Conserva AX,BX,CX,DX.
GuardarRecords PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    MOV EstadoArchivo, 0
    MOV DX, RutaRecords
    XOR CX, CX
    MOV AH, 3CH
    INT 21H
    JC RecordEscrituraFallo
    MOV BX, AX
    MOV DX, OFFSET FirmaRecords
    MOV CX, 31
    MOV AH, 40H
    INT 21H
    JC RecordEscribirCerrarFallo
    CMP AX, 31
    JNE RecordEscribirCerrarFallo
    MOV AH, 3EH
    INT 21H
    JC RecordEscrituraFallo
    JMP RecordGuardarFin
RecordEscribirCerrarFallo:
    MOV AH, 3EH
    INT 21H
RecordEscrituraFallo:
    MOV EstadoArchivo, 2
RecordGuardarFin:
    POP DX
    POP CX
    POP BX
    POP AX
    RET
GuardarRecords ENDP

; Entrada: NivelActual, TurnoActual y SegundosJuego de una victoria.
; Salida: inserta en el top 3 y marca NuevoRecord=1 si clasifica. Conserva registros.
InsertarRecord PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH SI
    PUSH DI
    MOV NuevoRecord, 0
    XOR AX, AX
    MOV AL, NivelActual
    MOV BL, 9
    MUL BL
    MOV DI, AX
    MOV SI, DI
    ADD SI, 9
RecordBuscarPuesto:
    MOV AL, TurnoActual
    CMP AL, Records[DI]
    JB RecordInsertar
    JA RecordSiguientePuesto
    MOV AX, SegundosJuego
    CMP AX, WORD PTR Records[DI+1]
    JB RecordInsertar
RecordSiguientePuesto:
    ADD DI, 3
    CMP DI, SI
    JB RecordBuscarPuesto
    JMP RecordInsertarFin
RecordInsertar:
    MOV BX, SI
    SUB BX, 3
RecordDesplazar:
    CMP BX, DI
    JBE RecordPonerNuevo
    MOV AL, Records[BX-3]
    MOV Records[BX], AL
    MOV AX, WORD PTR Records[BX-2]
    MOV WORD PTR Records[BX+1], AX
    SUB BX, 3
    JMP RecordDesplazar
RecordPonerNuevo:
    MOV AL, TurnoActual
    MOV Records[DI], AL
    MOV AX, SegundosJuego
    MOV WORD PTR Records[DI+1], AX
    MOV NuevoRecord, 1
RecordInsertarFin:
    POP DI
    POP SI
    POP CX
    POP BX
    POP AX
    RET
InsertarRecord ENDP

; Solo las victorias normales participan; pruebas y claves manuales no cuentan.
; Sin parametros; conserva registros mediante las rutinas llamadas.
RegistrarVictoria PROC
    CMP PartidaTerminada, 1
    JNE RecordRegistrarFin
    CMP ClaveManual, 0
    JNE RecordRegistrarFin
    CMP ModoPruebas, 0
    JNE RecordRegistrarFin
    CALL InsertarRecord
    CMP NuevoRecord, 0
    JE RecordRegistrarFin
    CALL GuardarRecords
RecordRegistrarFin:
    RET
RegistrarVictoria ENDP

; Sin parametros. Mensaje de resultado; conserva AX.
MostrarAvisoRecord PROC
    PUSH AX
    CMP ClaveManual, 0
    JNE RecordAvisoManual
    CMP NuevoRecord, 0
    JE RecordAvisoFin
    CMP EstadoArchivo, 2
    JE RecordAvisoError
    ESCRIBIR_TEXTO 19, 12, 'NUEVO RECORD!'
    JMP RecordAvisoFin
RecordAvisoError:
    ESCRIBIR_TEXTO 19, 4, 'NO SE PUDO GUARDAR EL RECORD'
    JMP RecordAvisoFin
RecordAvisoManual:
    ESCRIBIR_TEXTO 19, 10, 'PRUEBA: SIN RECORD'
RecordAvisoFin:
    POP AX
    RET
MostrarAvisoRecord ENDP

 ; Tarjeta: DX=coordenada superior y CL=color del acento. Conserva registros.
DibujarTarjetaRecord PROC
    PUSH AX
    PUSH CX
    PUSH DX
    PINTAR_RECT 16, DX, 288, 44, 22
    XOR CH, CH
    PINTAR_RECT 16, DX, 2, 44, CX
    INC DX
    PINTAR_RECT 108, DX, 60, 42, 25
    ADD DX, 4
    ; Una pequena corona distingue el primer puesto.
    PINTAR_RECT 148, DX, 2, 3, 20
    PINTAR_RECT 156, DX, 2, 3, 20
    DEC DX
    PINTAR_RECT 152, DX, 2, 4, 20
    ADD DX, 5
    PINTAR_RECT 148, DX, 10, 2, 20
    POP DX
    POP CX
    POP AX
    RET
DibujarTarjetaRecord ENDP

; Sin parametros. Tres tarjetas de dificultad y tres puestos por tarjeta.
; Conserva registros. Restablece el estilo normal al salir.
PantallaRecords PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    CALL IniciarTransicion
    CALL LimpiarPantalla
    PINTAR_RECT 0, 0, 320, 200, 21
    MOV FondoTexto, 21
    MOV ColorTexto, 3
    ESCRIBIR_TEXTO 1, 3, 'TUS MEJORES HECHIZOS'
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 3, 3, 'Menos intentos, mejor puesto.'
    PINTAR_RECT 24, 40, 272, 1, 24
    PINTAR_IMAGEN 272, 8, 24, 24, SpriteCalabaza, 16, 16
    MOV DX, 46
    MOV CL, 7
    CALL DibujarTarjetaRecord
    MOV DX, 94
    MOV CL, 12
    CALL DibujarTarjetaRecord
    MOV DX, 142
    MOV CL, 18
    CALL DibujarTarjetaRecord
    MOV FondoTexto, 22
    MOV ColorTexto, 7
    ESCRIBIR_TEXTO 7, 3, 'FACIL'
    MOV ColorTexto, 12
    ESCRIBIR_TEXTO 13, 3, 'NORMAL'
    MOV ColorTexto, 18
    ESCRIBIR_TEXTO 19, 3, 'DIFICIL'
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 9, 3, '12 turnos'
    ESCRIBIR_TEXTO 15, 3, '10 turnos'
    ESCRIBIR_TEXTO 21, 3, '8 fichas'
    PINTAR_IMAGEN 84, 54, 16, 16, SpriteGato, 16, 16
    PINTAR_IMAGEN 84, 102, 16, 16, SpriteCalabaza, 16, 16
    PINTAR_IMAGEN 84, 150, 16, 16, SpriteFantasma, 16, 16
    XOR SI, SI
    XOR BX, BX
RecordDibujarOtro:
    CALL DibujarPuestoRecord
    ADD BX, 3
    INC SI
    CMP SI, 9
    JB RecordDibujarOtro
    MOV FondoTexto, 21
    MOV ColorTexto, 26
    CMP EstadoArchivo, 0
    JNE RecordAvisoArchivo
    ESCRIBIR_TEXTO 24, 3, 'ESC  Volver'
    ESCRIBIR_TEXTO 24, 23, 'Sigue la magia'
    JMP RecordPantallaFin
RecordAvisoArchivo:
    ESCRIBIR_TEXTO 24, 2, 'No se pudo leer o guardar el archivo.'
RecordPantallaFin:
    MOV FondoTexto, 0
    MOV ColorTexto, 3
    CALL TerminarTransicion
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
PantallaRecords ENDP

; SI=puesto global 0..8, BX=offset de entrada. Conserva registros.
DibujarPuestoRecord PROC
    PUSH AX
    PUSH CX
    PUSH DX
    PUSH DI
    MOV AX, SI
    MOV CL, 3
    DIV CL
    ; AL=nivel, AH=puesto dentro de su tarjeta.
    MOV CL, AH
    XOR AH, AH
    MOV DL, 6
    MUL DL
    ADD AL, 6
    MOV DH, AL
    XOR AX, AX
    MOV AL, CL
    MOV DI, AX
    MOV DL, 8
    MUL DL
    ADD AL, 14
    MOV DL, AL
    MOV FondoTexto, 22
    MOV ColorTexto, 26
    CMP DI, 0
    JNE RecordPuestoTitulo
    MOV FondoTexto, 25
    MOV ColorTexto, 20
RecordPuestoTitulo:
    CMP DI, 0
    JNE RecordSegundoPuesto
    ESCRIBIR_TEXTO DH, DL, '#1'
    JMP RecordPuestoDatos
RecordSegundoPuesto:
    CMP DI, 1
    JNE RecordTercerPuesto
    ESCRIBIR_TEXTO DH, DL, '#2'
    JMP RecordPuestoDatos
RecordTercerPuesto:
    ESCRIBIR_TEXTO DH, DL, '#3'
RecordPuestoDatos:
    ADD DH, 2
    MOV AL, Records[BX]
    CMP AL, 255
    JE RecordPuestoVacio
    MOV ColorTexto, 3
    CALL EscribirNumero
    ADD DL, 3
    ESCRIBIR_TEXTO DH, DL, 'int'
    SUB DL, 3
    ADD DH, 2
    MOV ColorTexto, 26
    MOV AX, WORD PTR Records[BX+1]
    CALL EscribirTiempo
    JMP RecordPuestoFin
RecordPuestoVacio:
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO DH, DL, 'Sin'
    ADD DH, 2
    ESCRIBIR_TEXTO DH, DL, 'marca'
RecordPuestoFin:
    POP DI
    POP DX
    POP CX
    POP AX
    RET
DibujarPuestoRecord ENDP

; F3 abre la tabla desde menu o resultado. ESC vuelve al origen; conserva AX.
ConsultarRecords PROC
    PUSH AX
    CALL PantallaRecords
RecordEsperarTecla:
    CALL ActualizarSonido
    MOV AH, 01H
    INT 16H
    JZ RecordEsperarTecla
    MOV AH, 00H
    INT 16H
    CMP AL, 27
    JNE RecordEsperarTecla
    POP AX
    RET
ConsultarRecords ENDP
