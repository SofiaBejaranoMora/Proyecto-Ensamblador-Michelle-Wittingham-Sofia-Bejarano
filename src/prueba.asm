.MODEL SMALL
.STACK 100H

INCLUDE macros.inc

.DATA
ModoAnterior DB ?
OpcionMenu DB 0

INCLUDE paleta.inc
INCLUDE figuras.inc
INCLUDE inicio.inc
INCLUDE menuDato.inc


; Textos de CREDITOS.
CredDesarrollo DB 'DESARROLLADO POR', 0
CredMichelle   DB 'MICHELLE', 0
CredSofia      DB 'SOFIA', 0
CredVolver     DB 'ESC PARA VOLVER', 0

; Textos de DIFICULTAD.
DifFacil   DB 'FACIL', 0
DifNormal  DB 'NORMAL', 0
DifDificil DB 'DIFICIL', 0
DifVolver  DB 'ESC PARA VOLVER', 0

; Textos de INSTRUCCIONES.
InstObjetivo    DB 'ADIVINA LA CLAVE', 0
InstCombinacion DB 'FORMA UNA COMBINACION', 0
InstIntento DB 'TIENES VARIOS ', 'INTENTOS', 0
InstPistas      DB 'UTILIZA LAS PISTAS', 0
InstPosicion    DB 'REVISA LAS POSICIONES', 0
InstGanar       DB 'DESCUBRE LA CLAVE', 0
InstVolver      DB 'ESC PARA VOLVER', 0


.CODE
INCLUDE video.inc
INCLUDE pantalla.inc
INCLUDE menu.inc
INCLUDE tablero.inc
INCLUDE credit.inc
INCLUDE dific.inc
INCLUDE instruc.inc

MAIN PROC
    MOV AX, @DATA
    MOV DS, AX
    CLD
    ; Guardar el modo original antes de activar graficos.
    MOV AH, 0FH
    INT 10H
    MOV ModoAnterior, AL
    MOV AX, 0013H
    INT 10H
    CALL CargarPaleta
    MOV AX, 0A000H
    MOV ES, AX
    CALL LimpiarPantalla
    CALL PantallaInicio

    EsperarInicio:
        MOV AH, 00H
        INT 16H

        CMP AL, 13
        JE AbrirMenu

        CMP AL, 27
        JNE SeguirEsperandoInicio
        JMP Terminar

    SeguirEsperandoInicio:
        JMP EsperarInicio

    AbrirMenu:
        CALL LimpiarPantalla
        CALL PantallaMenu

    EsperarMenu:
        MOV AH, 10H
        INT 16H

        ; Salir con ESC.
        CMP AL, 27
        JNE RevisarEnter
        JMP Terminar

    RevisarEnter:
        ; Detectar ENTER.
        CMP AL, 13
        JE SeleccionarOpcion

        ; Detectar flecha abajo.
        CMP AH, 50H
        JE MenuAbajo

        ; Detectar flecha arriba.
        CMP AH, 48H
        JE MenuArriba

        JMP EsperarMenu

    SeleccionarOpcion:
        CMP OpcionMenu, 0
        JE AbrirJuego

        CMP OpcionMenu, 1
        JE AbrirDificultad

        CMP OpcionMenu, 2
        JE AbrirInstrucciones

        CMP OpcionMenu, 3
        JE AbrirCreditos

        CMP OpcionMenu, 4
        JNE OpcionNoSalir
        JMP Terminar

    OpcionNoSalir:
        JMP EsperarMenu

    MenuArriba:
        ; Borrar el selector de la posicion actual.
        CALL BorrarSelector

        ; Comprobar si estamos en JUGAR.
        CMP OpcionMenu, 0
        JE VolverASalir

        ; Pasar a la opcion anterior.
        DEC OpcionMenu
        JMP ActualizarMenu

    VolverASalir:
        MOV OpcionMenu, 4
        JMP ActualizarMenu

    MenuAbajo:
        ; Borrar el selector de la posicion actual.
        CALL BorrarSelector

        ; Comprobar si estamos en SALIR.
        CMP OpcionMenu, 4
        JE VolverAJugar

        ; Pasar a la siguiente opcion.
        INC OpcionMenu
        JMP ActualizarMenu

    VolverAJugar:
        MOV OpcionMenu, 0

    ActualizarMenu:
        CALL DibujarSelector
        JMP EsperarMenu

    AbrirJuego:
        CALL PantallaJuego
        JMP EsperarJuego

    AbrirDificultad:
        CALL PantallaDificultad
        JMP EsperarJuego

    AbrirInstrucciones:
        CALL PantallaInstrucciones
        JMP EsperarJuego

    AbrirCreditos:
        CALL PantallaCreditos
        JMP EsperarJuego

    EsperarJuego:
        MOV AH, 00H
        INT 16H

        CMP AL, 27
        JNE EsperarJuego

        JMP AbrirMenu

    Terminar:
        ; ENTER y ESC salen de esta prueba. El menu se agregara despues.
        MOV AL, ModoAnterior
        MOV AH, 00H
        INT 10H
        MOV AX, 4C00H
        INT 21H
MAIN ENDP

END MAIN
