; Fondo y texto compartidos por las pantallas de navegacion. Conserva AX.
AbrirLienzo PROC
    PUSH AX
    CALL IniciarTransicion
    PINTAR_RECT 0, 0, 320, 200, 21
    MOV FondoTexto, 21
    MOV ColorTexto, 3
    POP AX
    RET
AbrirLienzo ENDP

; Devuelve el texto al estilo normal del tablero; conserva registros y flags.
CerrarEstilo PROC
    MOV FondoTexto, 0
    MOV ColorTexto, 3
    RET
CerrarEstilo ENDP

; Entrada: reglas y ClaveManual. Prepara el editor; conserva AX.
DibujarEditorClave PROC
    PUSH AX
    CALL AbrirLienzo
    CMP ClaveManual, 0
    JE EditorTituloInverso
    ESCRIBIR_TEXTO 1, 3, 'TU CLAVE, TUS PRUEBAS'
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 3, 3, 'Prueba pistas con tu propia clave.'
    JMP EditorMostrarReglas
EditorTituloInverso:
    ESCRIBIR_TEXTO 1, 3, 'AHORA TU GUARDAS EL SECRETO'
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 3, 3, 'Yo intentare descubrir tu clave.'
EditorMostrarReglas:
    PINTAR_RECT 16, 48, 288, 40, 22
    PINTAR_RECT 16, 48, 2, 40, 7
    MOV FondoTexto, 22
    MOV ColorTexto, 3
    CMP CantidadFichas, 8
    JNE EditorSeisFichas
    ESCRIBIR_TEXTO 7, 3, 'Usa 1-8 para elegir tus personajes.'
    JMP EditorRepeticiones
EditorSeisFichas:
    ESCRIBIR_TEXTO 7, 3, 'Usa 1-6 para elegir tus personajes.'
EditorRepeticiones:
    MOV ColorTexto, 26
    CMP PermitirRepetidos, 0
    JE EditorSinRepeticiones
    ESCRIBIR_TEXTO 9, 3, 'Puedes repetir personajes.'
    JMP EditorMostrarEntrada
EditorSinRepeticiones:
    ESCRIBIR_TEXTO 9, 3, 'En este nivel, sin repetir.'
EditorMostrarEntrada:
    MOV FondoTexto, 21
    ESCRIBIR_TEXTO 12, 3, 'Tu clave'
    PINTAR_IMAGEN 264, 108, 24, 24, SpriteFantasma, 16, 16
    PINTAR_RECT 32, 164, 256, 20, 25
    MOV FondoTexto, 25
    MOV ColorTexto, 3
    ESCRIBIR_TEXTO 21, 6, 'ENTER  Ya tengo mi clave'
    MOV FondoTexto, 21
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 24, 3, 'Backspace borra   ESC cancela'
    PUSH SI
    XOR SI, SI
    CALL MostrarClaveManual
    POP SI
    CALL TerminarTransicion
    POP AX
    RET
DibujarEditorClave ENDP
