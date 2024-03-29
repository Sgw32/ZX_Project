;------------------------------------------------------------------------;
;                         Sochi Nights for Yandex Contest' 2019          ;
;   Slideshow                                                            ;
;                                                                        ;
;                                                                        ;
;------------------------------------------------------------------------;

NEXTTAP_ADR:    equ     24317
NEXTTAP_SIZE:   equ     39524

include "isr.inc"
include "startuplogo_rt.inc"

;Test codes (commented)
main:
                
main_gs:        push af             ; save the registers
                push bc
                push de
                push  hl
                xor a               ; clear accum
                call 8859           ; set border colour to black
                xor a
                call SND_SETSFXM    ; turn off sound fx
                call ClrScr         ; clear the screen
                call SFX_INIT
                ld a, GF_MENU       ; set the game flags to show the menu
                ld (GameFlags), a   ; clear the game flags
                ld hl, TUNE_A
                call SND_INIT_HL
                call ISR_Start
                call Screen1
                call Screen2
                call Screen3

MainLoop:       
                call TapLoad   ; continue playing game if set
                jp MainLoop

TapLoad:        call ISR_Stop
                xor a               ; reset accum
                ld (CursorLive), a  ; reset the cursorlive variable
                call ClrScr         ; clear the screen
                pop hl              ; restore the registers
                pop de
                pop bc
                pop af
                jp load_and_run_file
                ret                 ; return to BASIC

load_and_run_file:
                ld ix, NEXTTAP_ADR
                ld de, NEXTTAP_SIZE
                push ix
                scf 
                ld a,#ff
                inc d
                exa
                dec d
                jp $0556
                jp 24317
                ret
                
;---------------------------------------------------------------;
; Pause                                                         ;
;   A generic pause function                                    ;
;---------------------------------------------------------------;
Pause:          push bc
                push de
                push hl
                ld bc, PAUSETIME
                ld de, 0
                ld hl, 0
                ldir
                pop hl
                pop de
                pop bc
                ret

;---------------------------------------------------------------;
;                                                               ;
; LIBRARY SOURCE CODE                                           ;
;                                                               ;
;---------------------------------------------------------------;
;include "isr.inc"
include "gameplay.inc"
include "control.inc"
include "screen.inc"
include "ay_player.inc"
include "corridors_screen.inc"              ; include the screens and tiles
;---------------------------------------------------------------;
;                                                               ;
; VARIABLES                                                     ;
;                                                               ;
;---------------------------------------------------------------;
;Cursor:         defb 0
CursorPos:      defb 136, 96            ; the current position of the gem cursor
CursorPosOld:   defb 136, 96            ; the previous position of the gem cursor
CursorLive:     defb 0                  ; cursor is being displayed on the screen
PointerPos:     defb 128, 96            ; the currently valid position of the mouse pointer
MovePosLast:    defb 0  ,  0            ; the last position of the mouse
MouseHasMoved:  defb 0                  ; indicates whether the kempstom mouse has moved
MouseStartPos:  defb 0  ,  0            ; the start position of the kempston mouse
GameFlags:      defb GF_MENU            ; start with the main menu
;ControlFlags:   defb CF_INTERFACE_II    ; default to the Interface II joystick
;ControlFlags:   defb CF_KEM_JOYSTICK    ; default to the Interface II joystick
ControlFlags:   defb CF_KEYBOARD    ; default to the Interface II joystick
MusicFlags:     defb GF_THEME_1         ; default is theme 1
;GameTune:       defw TUNE_A             ; the default game music
;GemTheme:       defb GF_THEME_1         ; default is theme 1
CursorFlags:    defb CF_NONE            ; default is no buttons pressed
SwapFlags:      defb 0                  ; 1 - left/right, 2 up/down
MatchesSoFar:   defb 0                  ; number of matches for the level
Level:          defb 0                  ; the current level

;---------------------------------------------------------------;
;                                                               ;
; FLAG DEFINITIONS                                              ;
;                                                               ;
;---------------------------------------------------------------;

;---------------------------------------------------------------;
; Game Flags                                                    ;
;---------------------------------------------------------------;
GF_QUIT         equ  1          ; user has requested to quit
GF_MENU         equ  2          ; we are currently in the menu system
GF_START_GAME   equ  4          ; we are to start a gane
GF_PLAYING      equ  8          ; we are playing a game
GF_PLAY_MODE    equ 16          ; 1 - normal game, 0 - timed game
GF_GAME_OVER    equ 32          ; game has finished

;---------------------------------------------------------------;
; Control Keys                                                  ;
;---------------------------------------------------------------;
CF_KEYBOARD     equ  1          ; QAOP and Space
CF_INTERFACE_II equ  2          ; interface II ports 1 & 2
CF_KEM_JOYSTICK equ  4          ; kempston joystick
CF_KEM_MOUSE    equ  8          ; kempston mouse

CF_MOUSE_RIGHT  equ  1          ; the kempston right mouse button bit
CF_MOUSE_LEFT   equ  2          ; the kempston left mouse button bit

;---------------------------------------------------------------;
; Graphics Themes                                               ;
;---------------------------------------------------------------;
GF_THEME_1      equ  1          ; Programmer Art
GF_THEME_2      equ  2          ; Square Gems
GF_THEME_3      equ  4          ; Different Shaped Gems

;---------------------------------------------------------------;
; Cursor Flags                                                  ;
;---------------------------------------------------------------;
CF_NONE         equ  0          ; nothing set
CF_ACTION       equ  1          ; fire button pressed or left mouse button
CF_LEFT         equ  2          ; cursor moved left
CF_RIGHT        equ  4          ; cursor moved right
CF_UP           equ  8          ; cursor moved up
CF_DOWN         equ 16          ; cursor moved down


;---------------------------------------------------------------;
; Swap Flags                                                    ;
;---------------------------------------------------------------;
SF_NONE         equ 0           ; nothing to swap
SF_UP           equ 1           ; swap up
SF_DOWN         equ 2           ; swap down
SF_LEFT         equ 4           ; swap left
SF_RIGHT        equ 8           ; swap right

AAA_END:        defb 0
;---------------------------------------------------------------;
; Program start location                                        ;
;---------------------------------------------------------------;

end start


