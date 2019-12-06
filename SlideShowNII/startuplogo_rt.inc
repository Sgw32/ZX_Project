ATTRIBUTES   equ 22528
CNT1         equ  23675
 
;---------------------------------------------------------------;
; PlayGame                                                      ;
;   The games main loop                                         ;
;---------------------------------------------------------------;
SplashScreen:    
             ld hl, CNT1
             ld (hl), 2
             ld hl, StartupLogo        ; display the title screen
PGLoop1:      
             call DrawScr            ; draw the screen    
             call Pause1000              ; pause between key presses  
             
             call ClrScr
             call Pause1000              ; pause between key presses
             
             ld a,(CNT1)
             dec a
             ld (CNT1),a
             jr nz, PGLoop1               ; keep going still the game is finished
             ld hl, CNT1
             ld (hl), 2
             ld hl, StartupLogo        ; display the title screen
PGLoop2:      
             dec bc
             call DrawScr            ; draw the screen    
             call Pause60000              ; pause between key presses  
             
             call ClrScr
             call Pause60000              ; pause between key presses
             ld a,(CNT1)
             dec a
             ld (CNT1),a
             jr nz, PGLoop2               ; keep going still the game is finished
             
ExitFromSplash:
             ld hl, StartupLogo        ; display the title screen
             call DrawScr            ; draw the screen 
             call WaitForSpace
             call ClrScr
             call Pause60000
             ret             

;---------------------------------------------------------------;
; Pause                                                         ;
;   pause briefly                                               ;
;---------------------------------------------------------------;
Pause1000:       push bc                 ; save bc
             push de                 ; save de
             push hl                 ; hl
             ld bc, 100            ; define the pause length
             ld de, 0                ; define block transfer destination
             ld hl, 0                ; define block transfer source
             ldir                    ; do a block transfer (takes a little while) 
             pop hl                  ; restore hl
             pop de                  ; restore de
             pop bc                  ; restore bc
             ret                     ; pause complete   
             

;---------------------------------------------------------------;
; Pause2                                                         ;
;   pause briefly                                               ;
;---------------------------------------------------------------;
Pause60000:       push bc                 ; save bc
             push de                 ; save de
             push hl                 ; hl
             ld bc, 6000            ; define the pause length
             ld de, 0                ; define block transfer destination
             ld hl, 0                ; define block transfer source
             ldir                    ; do a block transfer (takes a little while) 
             pop hl                  ; restore hl
             pop de                  ; restore de
             pop bc                  ; restore bc
             ret                     ; pause complete                         

;---------------------------------------------------------------;
; DrawScr                                                       ;
;                                                               ;
;   Draws a screen object onto the display                      ;
;   Written by Tony Thompson                                    ;
;   Created         2006                                        ;
;   Last Changed    18 March 2006                               ;
;                                                               ;
;   Inputs                                                      ;
;       hl - the address of a screen to draw                    ;
;                                                               ;
;   Regs Used                                                   ;
;       af,  bc, de, hl                                         ;
;                                                               ;
;   Regs destoryed                                              ;
;       none                                                    ;
;---------------------------------------------------------------;
DrawScr:    push af
            push bc
            push de
            push hl            ; save all the registers
            push bc
            push hl
            ld bc, 768        ; clear the attributes to black
            ld hl, ATTRIBUTES              
DrwScrClr:  ld (hl), 0
            inc hl
            dec bc
            ld a, c
            or b
            jr nz, DrwScrClr
            pop hl
            pop bc
            ld b, 192          ; transfer 192 lines
            ld de, SCREEN      ; point de at the screen address  
DrwScrLp:   push bc            ; save bc
            ld bc, 32          ; transfer 1 line of pixels  
            push de            ; save the screen position
            ldir               ; do the transfer
            pop de             ; restore the screen position
            ex de, hl          ; swap screen address for off screen address
            call Incy          ; get next line of the physical screen
            ex de, hl          ; put screen address back into de
            pop bc             ; get the line count
            djnz DrwScrLp      ; decrement it and keep draw lines if not zero
            ld bc, 768         ; transfer the attributes to the screen
            ldir               ; do the transfer
            pop hl             ; restore all the registers
            pop de
            pop bc
            pop af
            ret                ; return to caller

include "startuplogo.inc"              ; include the screens and tiles
include "nii_screen.inc"              ; include the screens and tiles
