
Screen1:    
             ld a,1
             ld (NEXTTAP),a
             ret             
             
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
            ld hl, ATTR              
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


;---------------------------------------------------------------;
; WaitForSpace                                                  ;
;   wait for space key to be pressed                            ;
;---------------------------------------------------------------;
WaitForSpace:   ld bc, 32766           ; check keys from Space-B
                in a, (c)              ; get keys
                bit 0, a               ; if space pressed
                jr nz, WaitForSpace    ; no, then check again
                ret                    ; yes, return to caller

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

    