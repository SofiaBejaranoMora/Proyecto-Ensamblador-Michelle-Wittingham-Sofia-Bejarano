.MODEL SMALL
.STACK 100H

INCLUDE macros.inc


.DATA
ModoAnterior DB ?
OpcionMenu DB 0
InicioSonido DW 0
SonidoActivo DB 0
DifFacil   DB 'FACIL', 0
DifNormal  DB 'NORMAL', 0
DifDificil DB 'DIFICIL', 0
DifVolver  DB 'ESC: VOLVER AL MENU', 0
CredDesarrollo DB 'DESARROLLADO POR', 0
CredMichelle   DB 'MICHELLE WITTINGHAM', 0
CredSofia      DB 'SOFIA BEJARANO', 0
CredVolver     DB 'ESC: VOLVER AL MENU', 0

InstObjetivo    DB 'ADIVINA LA CLAVE SECRETA.', 0
InstCombinacion DB 'FORMA UNA COMBINACION DE 4 FICHAS.', 0
InstIntento     DB 'CADA FILA REPRESENTA UN INTENTO.', 0
InstPistas      DB 'USA LAS PISTAS PARA ACERTAR.', 0
InstPosicion    DB 'IMPORTAN LA FICHA Y SU POSICION.', 0
InstGanar       DB 'DESCUBRE LA CLAVE PARA GANAR.', 0
InstVolver      DB 'ESC: VOLVER AL MENU', 0

INCLUDE paleta.inc
INCLUDE figuras.inc
INCLUDE inicio.inc
INCLUDE menuDato.inc

.CODE
INCLUDE video.inc
INCLUDE pantalla.inc
INCLUDE menu.inc
INCLUDE dific.inc
INCLUDE audio.inc
INCLUDE tablero.inc
INCLUDE instruc.inc
INCLUDE credit.inc

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
   ; CALL LimpiarPantalla
    CALL PantallaInicio

    EsperarInicio:
        MOV AH, 00H
        INT 16H

        CMP AL, 13
        JE AbrirMenu

        CMP AL, 27
        JNE EsperarInicio
        JMP Terminar

        JMP EsperarInicio

    AbrirMenu:
        ;CALL LimpiarPantalla
        CALL PantallaMenu

    EsperarMenu:
        CALL ActualizarSonido
        MOV AH, 01H
        INT 16H
        JZ EsperarMenu

        MOV AH, 00H
        INT 16H

        CMP AL, 27
        JNE RevisarTeclaMenu
        JMP Terminar

RevisarTeclaMenu:

        CMP AH, 50H
        JE MenuAbajo

        CMP AL,13
        JE ElegirOpcion

        JMP EsperarMenu

           ElegirOpcion:
        CMP OpcionMenu, 0
        JE AbrirJuego

        CMP OpcionMenu, 1
        JE AbrirDificultad

        CMP OpcionMenu, 2
        JE AbrirInstrucciones

        CMP OpcionMenu, 3
        JE AbrirCreditos

        JMP Terminar

    AbrirJuego:
        CALL PantallaJuego
        JMP VistaAbierta

    AbrirDificultad:
        CALL PantallaDificultad
        JMP VistaAbierta

    AbrirInstrucciones:
        CALL PantallaInstrucciones
        JMP VistaAbierta

    AbrirCreditos:
        CALL PantallaCreditos
        JMP VistaAbierta

    VistaAbierta:
        CALL SonidoSeleccion

    EsperarVista:
        CALL ActualizarSonido

        MOV AH, 01H
        INT 16H
        JZ EsperarVista

        MOV AH, 00H
        INT 16H

        CMP AL, 27
        JE SalirVista

        JMP EsperarVista

    SalirVista:
        CALL SonidoSeleccion
        JMP ActualizarMenu

      MenuAbajo:
        CALL SonidoSeleccion

        ; Borrar antes de cambiar OpcionMenu.
        CALL BorrarSelector

        CMP OpcionMenu, 4
        JE VolverAJugar

        INC OpcionMenu
        JMP ActualizarSelector

    VolverAJugar:
        MOV OpcionMenu, 0

    ActualizarSelector:
        CALL DibujarSelector
        JMP EsperarMenu

    ; Redibujar todo solo al regresar de otra vista.
    ActualizarMenu:
        CALL PantallaMenu
        JMP EsperarMenu

    Terminar:
        CALL ApagarSonido
        
        MOV AL, ModoAnterior
        MOV AH, 00H
        INT 10H
        MOV AX, 4C00H
        INT 21H
MAIN ENDP

END MAIN
