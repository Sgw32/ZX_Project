;---------------------------------------------------------------;
;                                                               ;
;   SabreWulf                                                   ;
;                                                               ;
;  Original game by Ultimate Play The Game                      ;
;  Programmed by Chris and Tim Stamper                          ;
;                                                               ;
;       Programmed by Tony Thompson                             ;
;       Start Date  24 Apr 2003                                 ;
;       Last Update 30 Apr 2003                                 ;
;         Added tile and map drawing                            ;
;       Last Update 3  Apr 2004                                 ;
;         Added sprite drawing and animation                    ;
;                                                               ;
;---------------------------------------------------------------;
MAPWIDTH:   equ     16                  ; number of screens the map is wide
MAPHEIGHT:  equ     16                  ; number of screens the map is tall
SABRE_SCREEN:     equ     16384               ; spectrum display memory address
SABRE_SCREEN2:    equ     SABRE_SCREEN+64           ; address to draw the offset screen buffer
SABRE_ATTR:       equ     22528               ; spectrum attribute memory address
SABRE_SCRLEN:     equ     6144                ; size of the screen pixels in bytes
SABRE_ATTRLEN:    equ     768                 ; size of the SABRE_ATTR in bytes
SABRE_MEMSCRLEN:  equ     6335                ; size of the offscreen buffer
CHRSET:     equ     15616               ; address of the spectrum ROM character
;SCRWIDTH:   equ     32                  ; number of character the offscreen buffer is wide

;include "screen.inc"

MemScr:     defs 5632                   ; off screen memory aligned to 4 byte boundary
MemAttr:    defs 705

Sabre_Start:      ;ld sp,  63488
            ;jp Start
            call Sabre_ClrScr
            call Sabre_DrwPanel
            call Sabre_DrwMapPos
            call Sabre_DrawMap
            call Sabre_SM_Draw
            ;ret

Sabre_MoveLoop:   call Sabre_MM_Move                ; move the Monsters
            call Sabre_SM_Move                ; move SabreMan
            call Sabre_Pause                  ; pause a little while
            jr Sabre_MoveLoop                 ; move again


Sabre_Pause:      push bc                     ; save bc
            push de                     ; save de
            push hl                     ; hl
            ld bc, 5000                 ; define the pause length
            ld de, 0                    ; define block transfer destination
            ld hl, 0                    ; define block transfer source
            ldir                        ; do a block transfer (takes a little while)
            pop hl                      ; restore hl
            pop de                      ; restore de
            pop bc                      ; restore bc
            ret                         ; pause complete

Sabre_DrwPanel:   ld hl, SABRE_SCREEN+3
            ld de, UP1
            call DrwStr
            ld hl, SABRE_SCREEN+33
            ld de, SCORE1
            call DrwStr
            ld hl,  SABRE_SCREEN+27
            ld de,  UP2
            call DrwStr
            ld hl,  SABRE_SCREEN+57
            ld de,  SCORE2
            call DrwStr
            ld hl, SABRE_SCREEN+15
            ld de,  HI
            call DrwStr
            ld hl,  SABRE_SCREEN+45
            ld de,  SCOREH
            call DrwStr

            ld hl, SCOREC
            ld de, SABRE_ATTR
            ld bc, 64
            ldir
            ret

Sabre_DrwMapPos:  ld a, (MAP_Coord)
            ld de, MAPPOS
            call Num2Txt
            ld a, (MAP_Coord + 1)
            ld de, MAPPOS + 4
            call Num2Txt
            ld hl, SABRE_SCREEN+33
            ld de, MAPPOS
            call DrwStr
            ret

; converts a number less than 16 to text
Num2Txt:    add a, a
            ld b, 0
            ld c, a
            ld hl, TXT16
            add hl, bc
            ld bc, 2
            ldir
            ret



;---------------------------------------------------------------;
; Sabre_MM_Move                                                       ;
;                                                               ;
;   Moves the Monsters on the screen                            ;
;                                                               ;
;   Written by Tony Thompson                                    ;
;   Created         24th April 2003                             ;
;   Last Changed    4th April 2004                              ;
;   Added empty stub for later                                  ;
;                                                               ;
;   Inputs                                                      ;
;      none                                                     ;
;                                                               ;
;   Outputs                                                     ;
;       none                                                    ;
;                                                               ;
;   Regs Used                                                   ;
;       none                                                    ;
;                                                               ;
;   Regs Destoryed                                              ;
;       none                                                    ;
;                                                               ;
;---------------------------------------------------------------;
Sabre_MM_Move:    ret



;---------------------------------------------------------------;
; Sabre_SM_Move                                                       ;
;                                                               ;
;   Moves SabreMan by using the Interface II port 2             ;
;                                                               ;
;   Written by Tony Thompson                                    ;
;   Created         24th April 2003                             ;
;   Last Changed    1st May 2003                                ;
;     Made the movement change map screens                      ;
;   Last Changed    4th April 2004                              ;
;     Made the movement move and animate SabreMan               ;
;                                                               ;
;   Inputs                                                      ;
;       none                                                    ;
;                                                               ;
;   Outputs                                                     ;
;       none                                                    ;
;                                                               ;
;   Regs Used                                                   ;
;       af,  bc,  d                                             ;
;                                                               ;
;   Regs Destoryed                                              ;
;       af,  af',  bc',  de'. hl'                               ;
;                                                               ;
;---------------------------------------------------------------;
Sabre_SM_Move:    ld bc, 61438                ; load bc with Interface II port 2 address
            in a, (c)                   ; read the Interface II port
            and 191                     ; mask the 4 movement keys from the port value
            xor 191                     ; invert the keys so that if no keys are pressed
            ret z                       ; the value will be zero,  and ret if no key pressed
            ld d, a                     ; put keys pressed into d
            bit 3, d                    ; read the right key
            exx
            call nz, RIGHT              ; call right if pressed
            exx
            bit 4, d                    ; read the left key
            exx
            call nz, LEFT               ; call left if pressed
            exx
            ld a, d
            and 24
            ret nz                      ; ret if either the left or right are pressed
            bit 1, d                    ; read the up key
            exx
            call nz, UP                 ; call up if pressed
            exx
            bit 2, d                    ; read the down key
            exx
            call nz, DOWN               ; call down if pressed
            exx
            bit 0, d                    ; read the fire key
            exx
            call nz, TOGGLECOLLISONS
            exx
            ret                         ; finished moving
TOGGLECOLLISONS:
            ld a, (DoColTest)
            xor 1
            ld (DoColTest), a
            ret

UP:         ld hl, (SM_Sprite)          ; get the current sprite set
            ld (SM_OSprite), hl
            ld de, SabreManWalkUp       ; load de with what it should be
            and a                       ; reset the carry flag
            sbc hl, de                  ; compare them
            jr z, UPMOVE                ; zero if the same so goto next frame
            push de                     ; save the sprite pointer
            call Sabre_SM_Draw                ; erase the last sabreman
            pop de                      ; restore the sprite pointer
            ld (SM_Sprite), de          ; not the same,  so change the sprite set
            ld a, (SM_Frame)            ; get the frame index
            xor a                       ; reset the frame to zero
            ld (SM_OFrame), a           ; save the frame index
            ld (SM_Frame),  a           ; set the frame index
            call Sabre_SM_Draw                ; draw the new sabreman
            ret                         ; done
UPMOVE:	    call SM_Size
            ld bc, (SM_Pos)             ; load position into bc
            ld a, b
            sub 4
            ld b, a
            and 7
            jr z,  UPDOTEST
            inc d
UPDOTEST:   call ColTest
            inc b
            inc b
            inc b
            inc b
            and a
            ret nz
            ld (SM_OPos), bc
            ld a, b
            sub 4
            ld b, a
            cp 16
            jr z, UPSABRE_SCREEN
            ld (SM_Pos), bc
            jp NEXTFRAME
UPSABRE_SCREEN:   ld bc, (SM_Pos)
            ld b, 164
            ld (SM_Pos), bc
            ld bc, (MAP_Coord)          ; get the current map coords
            dec b                       ; move to the next map
            ld (MAP_Coord), bc          ; save the new map coord
            call Sabre_DrwMapPos
            call Sabre_DrawMap
            call FixSMPos
            call Sabre_SM_Draw
            call Sabre_SM_Draw
            ld hl, (SM_Pos)
            ld (SM_OPos), hl
            ld hl, (SM_Sprite)          ; get the current sprite set
            ld b, (hl)                  ; load b with the number of frames for the sprite set
            ld a, (SM_Frame)            ; get the current frame in a
            ld (SM_OFrame), a           ; save the old frame
            call SM_Erase
            jp NEXTFRAME

DOWN:       ld hl, (SM_Sprite)          ; get the current sprite set
            ld (SM_OSprite), hl
            ld de, SabreManWalkDown     ; load de with what it should be
            and a                       ; reset the carry flag
            sbc hl, de                  ; compare them
            jr z,  DOWNMOVE             ; zero if the same so goto next frame
            push de                     ; save the sprite pointer
            call Sabre_SM_Draw                ; erase the last sabreman
            pop de                      ; restore the sprite pointer
            ld (SM_Sprite), de          ; not the same,  so change the sprite set
            ld a, (SM_Frame)            ; get the frame index
            ld (SM_OFrame), a           ; save the frame index
            xor a                       ; reset the frame to zero
            ld (SM_Frame),  a           ; set the frame index
            call Sabre_SM_Draw                ; draw the new sabreman
            ;call SM_Erase              ; erase the old sabreman
            ret                         ; done
DOWNMOVE:   call SM_Size
            ld bc, (SM_Pos)             ; load position into bc
            ld a, 4
            add a, b
            ld b, a
            and 7
            jr z,  DOWNDOTEST
            inc d
DOWNDOTEST: call ColTest
            dec b
            dec b
            dec b
            dec b
            and a
            ret nz
            ld (SM_OPos), bc
            ld a, 4
            add a, b
            ld b, a
            ld a, 168
            cp b
            jr z,  DOWNSABRE_SCREEN
            ld (SM_Pos), bc
            jp NEXTFRAME
DOWNSABRE_SCREEN: ld bc, (SM_Pos)
            ld b, 20
            ld (SM_Pos), bc
            ld bc, (MAP_Coord)          ; get the current map coords
            inc b                       ; move to the next map
            ld (MAP_Coord), bc          ; save the new map coord
            call Sabre_DrwMapPos
            call Sabre_DrawMap
            call FixSMPos
            call Sabre_SM_Draw
            call Sabre_SM_Draw
            ld hl, (SM_Pos)
            ld (SM_OPos), hl
            ld hl, (SM_Sprite)          ; get the current sprite set
            ld b, (hl)                  ; load b with the number of frames for the sprite set
            ld a, (SM_Frame)            ; get the current frame in a
            ld (SM_OFrame), a           ; save the old frame
            call SM_Erase
            jp NEXTFRAME

LEFT:       ld hl, (SM_Sprite)          ; get the current sprite set
            ld (SM_OSprite), hl
            ld de, SabreManWalkLeft     ; load de with what it should be
            and a                       ; reset the carry flag
            sbc hl, de                  ; compare them
            jr z,  LEFTMOVE             ; zero if the same so goto next frame
            push de                     ; save the sprite pointer
            call Sabre_SM_Draw                ; erase the last sabreman
            pop de                      ; restore the sprite pointer
            ld (SM_Sprite), de          ; not the same,  so change the sprite set
            ld a, (SM_Frame)            ; get the frame index
            ld (SM_OFrame), a           ; save the frame index
            xor a                       ; reset the frame to zero
            ld (SM_Frame), a            ; set the frame index
            call Sabre_SM_Draw                ; draw the new sabreman
            ;call SM_Erase              ; erase the old sabreman
            ret                         ; done
LEFTMOVE:   call SM_Size                ; get the size of the current sprite
            ld bc, (SM_Pos)             ; load position into bc
            ld a, c
            sub 4
            ld c, a
            and 7
            jr z,  LEFTDOTEST
            inc e                       ; inc the width for non boundary position
LEFTDOTEST: call ColTest                ; will the new position collide?
            inc c                       ;
            inc c                       ;
            inc c                       ;
            inc c                       ;
            and a                       ; test for collision
            ret nz                      ; non zero is a collision
            ld (SM_OPos), bc            ; no collision,  so save the old position
            ld a, c
            sub 4
            ld c, a
            jr z, LEFTSABRE_SCREEN
            ld (SM_Pos), bc             ; save the new position
            jp NEXTFRAME                ; go to the next frame
LEFTSABRE_SCREEN: ld bc, (SM_Pos)
            ld c, 232
            ld (SM_Pos), bc
            ld bc, (MAP_Coord)          ; get the current map coords
            dec c                       ; move to the next map
            ld (MAP_Coord), bc          ; save the new map coord
            call Sabre_DrwMapPos
            call Sabre_DrawMap
            call Sabre_SM_Draw
            call Sabre_SM_Draw
            ld hl, (SM_Pos)
            ld (SM_OPos), hl
            ld hl, (SM_Sprite)          ; get the current sprite set
            ld b, (hl)                  ; load b with the number of frames for the sprite set
            ld a, (SM_Frame)            ; get the current frame in a
            ld (SM_OFrame), a           ; save the old frame
            call SM_Erase
            jp NEXTFRAME

RIGHT:      ld hl, (SM_Sprite)          ; get the current sprite set
            ld (SM_OSprite), hl         ; save the sprite set
            ld de, SabreManWalkRight    ; load de with what it should be
            and a                       ; reset the carry flag
            sbc hl, de                  ; compare them
            jr z, RIGHTMOVE             ; zero if the same so move sprite
            push de                     ; save the sprite pointer
            call Sabre_SM_Draw                ; erase the last sabreman
            pop de                      ; restore the sprite pointer
            ld (SM_Sprite), de          ; not the same,  so change the sprite set
            ld a, (SM_Frame)            ; get the frame index
            ld (SM_OFrame), a           ; save the frame index
            xor a                       ; reset the frame to zero
            ld (SM_Frame), a            ; set the frame index
            call Sabre_SM_Draw                ; draw the new sabreman
            ret                         ; done
RIGHTMOVE:  call SM_Size
            ld bc, (SM_Pos)             ; load position into bc
            ld a, 4
            add a, c
            ld c, a
            and 7
            jr z, RIGHTDOTEST
            inc e                       ; inc the width for non boundary position
RIGHTDOTEST:call ColTest
            dec c
            dec c
            dec c
            dec c
            and a
            ret nz
            ld (SM_OPos), bc            ; save the old position
            ld a, 4
            add a, c
            ld c, a
            cp 240
            jr z, RIGHTSABRE_SCREEN
            ld (SM_Pos), bc             ; save the new position
            jr NEXTFRAME                ; go to the next frame
RIGHTSABRE_SCREEN:ld bc, (SM_Pos)
            ld c, 4
            ld (SM_Pos), bc
            ld bc, (MAP_Coord)          ; get the current map coords
            inc c                       ; move to the next map
            ld (MAP_Coord), bc          ; save the new map coord
            call Sabre_DrwMapPos
            call Sabre_DrawMap
            call Sabre_SM_Draw
            call Sabre_SM_Draw
            ld hl, (SM_Pos)
            ld (SM_OPos), hl
            ld hl, (SM_Sprite)          ; get the current sprite set
            ld b, (hl)                  ; load b with the number of frames for the sprite set
            ld a, (SM_Frame)            ; get the current frame in a
            ld (SM_OFrame), a           ; save the old frame
            call SM_Erase

NEXTFRAME:  ld hl, (SM_Sprite)          ; get the current sprite set
            ld b, (hl)                  ; load b with the number of frames for the sprite set
            ld a, (SM_Frame)            ; get the current frame in a
            ld (SM_OFrame), a           ; save the old frame
            inc a                       ; move to the next frame
            ld (SM_Frame), a            ; store the result
            cp b                        ; cp to b
            jr nz, NEXTFRAME1           ; skip if the result is not zero
            xor a                       ; reset the frame index to zero (ie. loop around)
            ld (SM_Frame), a            ; store the new frame index
NEXTFRAME1: call Sabre_SM_Draw                ; draw the new sabreman
            call SM_Erase               ; erase the old sabreman
            ret

;
; checks the new position is ok
; or needs to be adjusted by -8
; finds the correct x position of the sprite when moving vertically
; between screens.
;
FixSMPos:   ld bc, (SM_Pos)             ; get the current sprite position
            push bc                     ; save it
            call MAttr2                 ; get the attr addr for it
            pop bc                      ; restore position
            jr z, Fixed                 ; zero, position is ok
            ld c, 120                   ; check pos 15,16
            push bc                     ; save position
            call MAttr2                 ; get the attr addr
            pop bc                      ; restore pos
            call FixCheck               ; check position
            jr z, Fixed                 ; zero if position is ok
            ld c, 128                   ; check pos 16, 17
            push bc                     ; save position
            call MAttr2                 ; get attr addr
            pop bc                      ; restore position
            call FixCheck               ; check attr addr
            jr z, Fixed                 ; zero if position is ok
            ld c, 112                   ; check pos 14, 15
            push bc                     ; save position
            call MAttr2                 ; get attr addr
            pop bc                      ; restore position
            call FixCheck               ; check attr addr
            jr z, Fixed                 ; zero if position ok
            ld c, 104                   ; check pos 13, 14
            push bc                     ; save position
            call MAttr2                 ; get attr addr
            pop bc                      ; restore position
            call FixCheck               ; check attr addr
            jr z, Fixed                 ; zero if position is ok
            ld c, 168                   ; last possible position is 21,22
Fixed:      ld (SM_Pos), bc             ; store the new position
            ret

;
; checks the 2 attributes for the position are both zero
; this indicates the sprite can be placed here
;
FixCheck    ld a, (hl)                  ; get attr at hl
            and a                       ; set flags
            ret nz                      ; non zero then return
            inc l                       ; get next attr position
            ld a, (hl)                  ; get attr at hl
            and a                       ; check is zero
            ret                         ; return set flags

;---------------------------------------------------------------;
; Sabre_SM_Draw                                                       ;
;                                                               ;
;   Draws the SabreMan sprite at it current position,  frame and;
;   color in the off screen buffer                              ;
;   This function uses the DrwSprNC function which XOR's the    ;
;   sprite onto the screen. So you call the draw function again ;
;   to erase the sprite                                         ;
;                                                               ;
;   Written by Tony Thompson                                    ;
;   Created         3rd April 2004                              ;
;   Last Changed    3rd April 2004                              ;
;                                                               ;
;   Inputs                                                      ;
;       none                                                    ;
;                                                               ;
; Outputs                                                       ;
;       none                                                    ;
;                                                               ;
;   Regs Used                                                   ;
;       af,  bc,  de,  hl                                       ;
;                                                               ;
;   Regs destoryed                                              ;
;       af,  bc,  de,  hl                                       ;
;---------------------------------------------------------------;
Sabre_SM_Draw:    ld hl, (SM_Sprite)          ; load the sprite set into hl
            inc hl                      ; move pass the number of frames to the sprite pointers
            ld a, (SM_Frame)            ; load the current frame index into a
            ld c, a                     ; and place into c
            sla c                       ; multiply c by 2
            ld b,  0                    ; and reset b to zero (ie. bc is frame index * 2)
            add hl, bc                  ; point hl to the correct frame pointer to draw
            ld e, (hl)                  ; load de with the frame pointer
            inc hl
            ld d, (hl)
            ex de, hl                   ; place the frame pointer in hl
            ld bc, (SM_Pos)             ; load bc with the position of the sprite
            ld a, (SM_Color)            ; load a with the color of sabreman
            call DrwSprNC               ; draw the sabreman frame in the correct color
            ret

SM_Erase:   ld hl,  (SM_OSprite)        ; load the sprite set into hl
            inc hl                      ; move pass the number of frames to the sprite pointers
            ld a, (SM_OFrame)           ; load the current frame index into a
            ld c, a                     ; and place into c
            sla c                       ; multiply c by 2
            ld b,  0                    ; and reset b to zero (ie. bc is frame index * 2)
            add hl,  bc                 ; point hl to the correct frame pointer to draw
            ld e, (hl)                  ; load de with the frame pointer
            inc hl
            ld d, (hl)
            ex de, hl                   ; place the frame pointer in hl
            ld bc,  (SM_OPos)           ; load bc with the position of the sprite
            xor a                       ; indicate we don't want to change the SABRE_ATTR
            call DrwSprNC               ; erase the old sabreman frame
            ret

SM_Size:    ld hl, (SM_OSprite)         ; load the sprite set into hl
            inc hl                      ; move pass the number of frames to the sprite pointers
            ld a, (SM_OFrame)           ; load the current frame index into a
            ld c, a                     ; and place into c
            sla c                       ; multiply c by 2
            ld b,  0                    ; and reset b to zero (ie. bc is frame index * 2)
            add hl, bc                  ; point hl to the correct frame pointer to draw
            ld e, (hl)                  ; load de with the frame pointer
            inc hl
            ld d, (hl)
            ex de, hl                   ; place the frame pointer in hl
            ld e, (hl)                  ; load the width of the sprite into e
            inc hl                      ; move to the height
            ld d, (hl)                  ; and load it into d
            srl d
            srl d
            srl d
            srl e
            srl e
            srl e
            ret

;
; bc - sprite position in pixels
; returns new x position for sprite based on the collision attributes
;
Sabre_DrawMap:    ld bc, (MAP_Coord)          ; get the map coords
            call Map2Scr                ; convert map cell coords to tile screen address
            call DrwScr                 ; draw the screen
            call BltMemScr              ; show the screen
            ret

;---------------------------------------------------------------;
; MPix                                                          ;
;                                                               ;
;   Converts a screen pixel coord into an off screen address    ;
;   Written by Tony Thompson                                    ;
;   Created         1984                                        ;
;   Last Changed    1st May 2003                                ;
;                                                               ;
;   Inputs                                                      ;
;       b - y position in pixels                                ;
;       c - x position in pixels                                ;
;                                                               ;
;   Outputs                                                     ;
;       hl - the buffer address for the off screen location     ;
;                                                               ;
;   Regs Used                                                   ;
;       af,  bc,  de,  hl                                       ;
;                                                               ;
;   Regs destoryed                                              ;
;       af,  bc,  de                                            ;
;---------------------------------------------------------------;
MPix:       push de
            srl c
            srl c
            srl c                   ; div x pos by 8 into col
            ld l, b                 ; multiply y pos * 32
            ld h, 0
            add hl, hl
            add hl, hl
            add hl, hl
            add hl, hl
            add hl, hl
            ld a, l
            or c                    ; add c to de
            ld l, a
            ld de, MemScr
            add hl, de              ; add de to hl to get MemScr to offset
            pop de
            ret

;---------------------------------------------------------------;
; MAttr                                                         ;
;                                                               ;
;   Converts a screen pixel coord into a off screen attr address;
;   Written by Tony Thompson                                    ;
;   Created         1984                                        ;
;   Last Changed    1st May 2003                                ;
;                                                               ;
;   Inputs                                                      ;
;       b - y position in pixels                                ;
;       c - x position in pixels                                ;
;                                                               ;
;   Outputs                                                     ;
;       hl - the attribute address for the off screen location  ;
;                                                               ;
;   Regs Used                                                   ;
;       af,  bc,  de,  hl                                       ;
;                                                               ;
;   Regs destoryed                                              ;
;       af,  bc,  hl                                            ;
;---------------------------------------------------------------;
MAttr:      push de
            srl c
            srl c
            srl c                   ; div x pos by 8 into col
            ld l, b                 ; put row into hl
            ld h, 0
            add hl, hl
            add hl, hl
            ld a, l
            or c                    ; add c to hl
            ld l, a
            ld de,  MemAttr
            add hl, de
            pop de
            ret

MAttr2:     push de
            srl c
            srl c
            srl c                   ; div x pos by 8 into col
            srl b
            srl b
            srl b
            dec b
            dec b
            ld l, b                 ; multiply y pos * 32
            ld h, 0
            add hl, hl
            add hl, hl
            add hl, hl
            add hl, hl
            add hl, hl
            ld a, l
            or c                    ; add c to de
            ld l, a
            ld de, MemAttr
            add hl, de              ; add de to hl to get MemScr to offset
            pop de
            ret

;---------------------------------------------------------------;
; Map2Scr                                                       ;
;   Converts a Screen grid coords to an address of a Tiled      ;
;   Screen address that can be passed to DrwScr                 ;
;                                                               ;
;   Inputs                                                      ;
;       b - y grid position of tile screen                      ;
;       c - x grid position of tile screen                      ;
;                                                               ;
; MapIdx2Scr                                                    ;
;   Converts an index into the ScreenTable into an address      ;
;   of a Tiled Screen address that can be passed to DrwScr      ;
;                                                               ;
;   Inputs                                                      ;
;       bc - index of the screen                                ;
;                                                               ;
;   Written by Tony Thompson                                    ;
;   Created         1st May 2003                                ;
;   Last Changed    1st May 2003                                ;
;                                                               ;
;   Outputs                                                     ;
;       hl - the address of the tiled screen                    ;
;                                                               ;
;   Regs Used                                                   ;
;       af,  bc,  de,  hl                                       ;
;                                                               ;
;   Regs destoryed                                              ;
;       af,  de                                                 ;
;---------------------------------------------------------------;
Map2Scr:    push bc                     ; save the coords
            sla b                       ; multiply y cell coord by 16
            sla b
            sla b
            sla b
            ld a, c                     ; get the x cell coord
            or b                        ; combine the two
            ld c, a
            ld b, 0
MapIdx2Scr: sla c                       ; multiply bc by 2
            rl b                        ;
            ld hl, MapTable             ;
            add hl, bc
            pop bc                      ; restore the coords
            ld e, (hl)                  ; put the address of the tile screen into de
            inc hl                      ; from the screentable entry
            ld d, (hl)                  ; stored in hl
            ex de, hl                   ; hl contains tile screen address
            ret


;---------------------------------------------------------------;
; Sabre_ClrScr                                                        ;
;                                                               ;
;   Clears the screen                                           ;
;                                                               ;
;   Written by Tony Thompson                                    ;
;   Created         1st May 2003                                ;
;   Last Changed    1st May 2003                                ;
;                                                               ;
;   Inputs                                                      ;
;       none                                                    ;
;                                                               ;
;   Outputs                                                     ;
;       screen is cleared                                       ;
;                                                               ;
;   Regs Used                                                   ;
;       bc,  hl                                                 ;
;                                                               ;
;   Regs destoryed                                              ;
;       bc,  de,  hl                                            ;
;---------------------------------------------------------------;
Sabre_ClrScr:     push bc                     ; save bc
            push de                     ; save de
            push hl                     ; save hl
                                        ; clear the SABRE_ATTR first - makes for a smoother clear
            ld hl,  SABRE_ATTR                ; point hl to screen address
            ld de,  SABRE_ATTR+1              ; point de to screen address + 1
            ld bc,  SABRE_ATTRLEN-1           ; load bc with the size of the screen - 1
            ld (hl),  0                 ; clear the first element of the screen
            ldir                        ; block transfer zeroes to all of the screen
                                        ; clear the screen data
            ld hl,  SABRE_SCREEN              ; point hl to screen address + 1
            ld de,  SABRE_SCREEN+1            ; point de to screen address
            ld bc,  SABRE_SCRLEN-1            ; load bc with the size of the screen - 1
            ld (hl),  0                 ; clear the first element of the screen
            ldir                        ; block transfer zeroes to all of the screen
            pop hl                      ; restore hl
            pop de                      ; restore de
            pop bc                      ; restore bc
            ret                         ; screen cleared

;---------------------------------------------------------------;
; ClrMemScr                                                     ;
;                                                               ;
;   Clears the off screen memory buffer                         ;
;                                                               ;
;   Written by Tony Thompson                                    ;
;   Created         1st May 2003                                ;
;   Last Changed    1st May 2003                                ;
;                                                               ;
;   Inputs                                                      ;
;       none                                                    ;
;                                                               ;
;   Outputs                                                     ;
;       off screen is cleared                                   ;
;                                                               ;
;   Regs Used                                                   ;
;       bc,  hl                                                 ;
;                                                               ;
;   Regs destoryed                                              ;
;       none                                                    ;
;---------------------------------------------------------------;
ClrMemScr:  push bc                     ; save bc
            push de                     ; save de
            push hl                     ; save hl
            ld hl,  MemScr              ; point hl to the offscreen buffer
            ld de,  MemScr              ; point de to the offscreen buffer
            inc de
            ld bc,  SABRE_MEMSCRLEN           ; set bc to the size of the buffer
            ld (hl), 0                  ; clear the first byte of the buffer
            ldir                        ; clear all of the buffer
            pop hl                      ; restore hl
            pop de                      ; restore de
            pop bc                      ; restore bc
            ret                         ; done

;---------------------------------------------------------------;
; BltMemScr                                                     ;
;                                                               ;
;   Blits the off screen memory buffer to the screen            ;
;                                                               ;
;   Written by Tony Thompson                                    ;
;   Created         1st May 2003                                ;
;   Last Changed    1st May 2003                                ;
;                                                               ;
;   Inputs                                                      ;
;       none                                                    ;
;                                                               ;
;   Outputs                                                     ;
;       off screen is copied to the screen                      ;
;                                                               ;
;   Regs Used                                                   ;
;       af,  bc,  de,  hl                                       ;
;                                                               ;
;   Regs destoryed                                              ;
;       af,  bc,  de,  hl                                       ;
;---------------------------------------------------------------;
BltMemScr:  ;di
            ld hl,  MemAttr             ; copy the SABRE_ATTR first
            ld de,  SABRE_ATTR + 64           ; this makes for a smoother copy
            ld bc,  704                 ; onto the screen
            ldir                        ; copy attribute in 1 block transfer
                                        ; copy the screen data
            ld b, 176                   ; lines to copy
            ld hl, MemScr               ; off screen buffer
            ld de, SABRE_SCREEN2              ; destination on screen
bms1:       push bc                     ; save lines to copy
            ld bc,  32                  ; set size of line to block transfer
            push de                     ; save screen address
            ldir                        ; block transfer the line
            ex de, hl                   ; put new buffer address into de
            pop hl                      ; get screen address
            inc h                       ; try to move down 1 line in a character                1M  4T
            ld a, h                     ; get h into a                                          1M  4T
            and 7                       ; test if still inside character                        2M  7T
            jr nz, bms2                 ; ret if in character square                            1M  5T  3M  5T  7M  20T
            ld a, l                     ; no,  get lower byte of address                         1M  4T
            add a, 32                   ; and move it to the next character block               2M  7T
            ld l, a                     ; store the result                                      1M  4T
            jr c, bms2                  ; return if we are still in the same segment?           1M  5T  3M  5T  13M 40T
            ld a, h                     ; no,  so need to adjust high order byte of address      1M  4T
            sub 8                       ; adjust screen segment                                 2M  7T
            ld h, a                     ; store the correction                                  1M  4T
bms2:       ex de, hl                   ; swap addresses
            pop bc                      ; get the lines to copy
            djnz bms1                  ; dec and repeat until all lines are copied
            ;ei
            ret

;---------------------------------------------------------------;
; DrwScr                                                        ;
;                                                               ;
;   Draws a Tiled Screen                                        ;
;                                                               ;
;   Written by Tony Thompson                                    ;
;   Created         1st May 2003                                ;
;   Last Changed    1st May 2003                                ;
;                                                               ;
;   Inputs                                                      ;
;       hl - the address of the tiled screen to draw            ;
;                                                               ;
;   Outputs                                                     ;
;       none                                                    ;
;                                                               ;
;   Regs Used                                                   ;
;       af,  bc,  de,  hl                                       ;
;                                                               ;
;   Regs destoryed                                              ;
;       af,  bc,  de,  hl                                       ;
;---------------------------------------------------------------;
DrwScr:     ;di
            call ClrMemScr
            ld a, l                     ; check to see
            xor h                       ; if the screen address is 0
            ret z                       ; and return if it is
            ld a, (hl)                  ; b contains no of tiles on screen
            and a                       ; check if a is zero
            ret z                       ; ret if it is
            ld b, a                     ; put no of tiles in b
ds1:        inc hl                      ; move to first tile description
            push bc                     ; save the no of tiles
            ld e, (hl)                  ; e contains the tile index
            inc hl                      ; move hl to x position
            ld c, (hl)                  ; c contains the x position of tile
            inc hl                      ; move hl to y position
            ld b, (hl)                  ; b contains the y position of tile
            push hl                     ; save tile screen index
            ld d, 0                     ; convert tile index into a tile address
            sla e                       ; by multiplying de by 2
            rl d                        ;
            ld hl,  TileTable           ; load hl with tile table base address
            add hl, de                  ; get the correct tile address index
            ld e, (hl)                  ; load the tile address into de
            inc hl                      ;
            ld d, (hl)                  ;
            ex de, hl                   ; put the tile address in hl
            call DrwTileSabre                ; draw the tile
            pop hl                      ; get current index address into screen
            pop bc                      ; get no of tiles
            djnz ds1                    ; draw all tiles
            ;ei
            ret                         ; all tiles are drawn


;---------------------------------------------------------------;
; DrwTile                                                       ;
;                                                               ;
;   Draws a tile object on the screen                           ;
;                                                               ;
;   Written by Tony Thompson                                    ;
;   Created         24th April 2003                             ;
;   Last Changed    1st May 2003                                ;
;                                                               ;
;   Inputs                                                      ;
;       hl - the address of the tile to draw                    ;
;       b  - the y position of the tile on the screen           ;
;       c  - the x position of the tile on the screen           ;
;                                                               ;
;Outputs                                                        ;
;       none                                                    ;
;                                                               ;
;   Regs Used                                                   ;
;       hl,  de,  bc,  af                                       ;
;                                                               ;
;   Regs destoryed                                              ;
;       af,  bc,  de,  hl                                       ;
;---------------------------------------------------------------;
DrwTileSabre:    ld e, (hl)                  ; read the width of the tile
            inc hl                      ; move to height
            ld d, (hl)                  ; read the height of the tile
            inc hl                      ; hl now points to the tile data,  and de contains the size of the tile
            push bc                     ; save the position of the tiles
            push hl                     ; save tile address
            call MPix                   ; convert bc position into screen address
            push de                     ; push size onto stack
            pop bc                      ; and put size into bc
            pop de                      ; put tile address into de
            push hl                     ; save the screen address
            push bc                     ; save the size
            sla b                       ; convert character height to
            sla b                       ; pixel height by muliplying the
            sla b                       ; character height by 8
dt1_sabre:        push bc                     ; save the size
            push hl                     ; save the screen address
            ex de, hl                   ; swap screen addr and tile addr (hl - tile)
            ld b, 0                     ; load b with zero,  now bc contains width
            ldir                        ; block transfer the tile to the screen
            ex de, hl                   ; swap screen addr and tile addr back again (de - tile)
            pop hl                      ; get the 1st column screen address
            ld c, 32                    ; size of a line in the buffer
            add hl, bc                  ; move down 1 line in the screen buffer
            pop bc                      ; get the size
            djnz dt1_sabre                    ; draw all lines of the tile
            pop bc                      ; get the size
            pop hl                      ; get the top/left screen address
            pop af                      ; put position into af
            push bc                     ; save size
            push af                     ; save position
            pop bc                      ; put position back into bc
            call MAttr                  ; convert tile pos to attribute address
            pop bc                      ; put size back into bc
dt2_sabre:        push bc                     ; save the size
            push hl                     ; save the attr address
            ld b, 0                     ; make bc contain only the width
            ex de, hl                   ; swap attr addr and tile addr
            ldir                        ; block transfert the attr data
            ex de, hl                   ; swap attr addr and tile addr back again
            pop hl                      ; get 1st column attr address
            ld c, 32                    ; ld bc with width of screen
            add hl, bc                  ; goto the next line down
            pop bc                      ; put currently drawn size into bc
            djnz dt2_sabre                    ; do all the lines of attr's
            ret                         ; finished drawing tile

;---------------------------------------------------------------;
; DrwChr                                                        ;
;                                                               ;
;   Draws a character on the screen                             ;
;                                                               ;
;   Written by Tony Thompson                                    ;
;   Created         24th April 2003                             ;
;   Last Changed    1st May 2003                                ;
;                                                               ;
;   Inputs                                                      ;
;       a  - the character to draw                              ;
;       hl - the screen address to draw the character           ;
;                                                               ;
; Outputs                                                       ;
;       none                                                    ;
;                                                               ;
;   Regs Used                                                   ;
;       af,  b,  de,  hl                                        ;
;                                                               ;
;   Regs destoryed                                              ;
;       af,  b,  de,  hl                                        ;
;---------------------------------------------------------------;
DrwChr:     sub 32                      ; character set starts at space
            push hl                     ; save screen address
            ld h, 0                     ; use hl to calculate character address
            ld l, a                     ;
            add hl, hl                  ; multiply by 2
            add hl, hl                  ; multiply by 4
            add hl, hl                  ; multiply by 8
            ld de, CHRSET               ; de has character set address
            add hl, de                  ; hl contains address of character data
            ex de, hl                   ; now put it into de
            pop hl                      ; get screen address
            ld b, 8                     ; characters are 8 pixels high
dc1:        ld a, (de)                  ; get character data
            ld (hl), a                  ; put it on screen
            inc h                       ; move down 1 line within a character on screen
            inc de                      ; move to next character data
            djnz dc1                    ; count the lines drawn
            ret                         ; done

;---------------------------------------------------------------;
; DrwStr                                                        ;
;                                                               ;
;   Draws a string of characters on the screen                  ;
;                                                               ;
;   Written by Tony Thompson                                    ;
;   Created         24th April 2003                             ;
;   Last Changed    1st May 2003                                ;
;                                                               ;
;   Inputs                                                      ;
;       de - the address of the string                          ;
;       hl - the screen address to draw the string              ;
;                                                               ;
; Outputs                                                       ;
;       none                                                    ;
;                                                               ;
;   Regs Used                                                   ;
;       af,  de,  hl                                            ;
;                                                               ;
;   Regs destoryed                                              ;
;       af,  de,  hl                                            ;
;---------------------------------------------------------------;

DrwStr:     ld a, (de)                  ; get character from string
            or a                        ; check character is not end of string
            ret z                       ; return if string is finished
            push de                     ; save string address
            push hl                     ; save screen address
            call DrwChr                 ; draw the character
            pop hl                      ; get the screen address
            pop de                      ; get the string address
            inc de                      ; move to next character in the string
            inc hl                      ; move to next screen position
            jr DrwStr


;---------------------------------------------------------------;
; DrwSprNC                                                      ;
;                                                               ;
;   Draws a sprite with no color SABRE_ATTR onto the screen           ;
;   off screen buffer                                           ;
;                                                               ;
;   Written by Tony Thompson                                    ;
;   Created         3rd April 2004                              ;
;   Last Changed    3rd April 2004                              ;
;                                                               ;
;   Inputs                                                      ;
;       a  - the color of the sprite                            ;
;       b  - the y position of the sprite                       ;
;       c  - the x position of the sprite                       ;
;       hl - the address of the sprite data                     ;
;                                                               ;
; Outputs                                                       ;
;       none                                                    ;
;                                                               ;
;   Regs Used                                                   ;
;       af,  bc,  de,  hl                                       ;
;                                                               ;
;   Regs destoryed                                              ;
;       af,  bc,  de,  hl                                       ;
;---------------------------------------------------------------;
;DrwSprNC:   ex af, af'                  ; save the sprite color
;            ld a,  c                    ; do we need to scroll the sprite into position?
;            and 7                       ; set z flags
;            jr z, DSNCfast              ; if zero if set,  then we can draw the sprite the fast way
                                        ; need to draw the sprite the slow way

; bc - position in pixels
; de - size of characters to test
; returns a not zero collision,  a is zero no collision
;
ColTest:    ld a,  (DoColTest)          ; do the collision test?
            and a
            ret z                       ; zero = no
                                        ; yep,  so continue
            push bc                     ; (pos)
            push hl                     ; (pos, hl)
            push de                     ; (pos, hl,  size)
            call MAttr2
            pop de                      ; (pos, hl)
            xor a
ColTest1:   push hl                     ; (pos, hl, attr)
            push de                     ; (pos, hl, attr, size)
ColTest2:   ld b, (hl)
            or b
            inc hl
            dec e
            jr nz,  ColTest2
            pop de                      ; (pos, hl, attr)
            pop hl                      ; (pos, hl)
            ld bc, SCRWIDTH
            add hl, bc
            dec d
            jr nz,  ColTest1
ColTestEnd: pop hl                      ;(pos)
            pop bc                      ;()
            ret

LastCode:   nop
;---------------------------------------------------------------;
;                                                               ;
; Sprites and Sprite Table used in the game                     ;
;                                                               ;
;---------------------------------------------------------------;
include "sprites_sabre.inc"

;---------------------------------------------------------------;
;                                                               ;
; Tiles used in the Map                                         ;
;                                                               ;
;---------------------------------------------------------------;
;include "tiles_sabre.inc"

;---------------------------------------------------------------;
;                                                               ;
; Map Table and Screens used in the game                        ;
;                                                               ;
;---------------------------------------------------------------;
include "map_sabre.inc"

;---------------------------------------------------------------;
;                                                               ;
; String Table                                                  ;
;                                                               ;
;---------------------------------------------------------------;
UP1:    defm "P1"
        defb 0
UP2:    defm "P2"
        defb 0
HI:     defm "HI"
        defb 0
SCORE1: defm "000000"
        defb 0
SCORE2: defm "000000"
        defb 0
SCOREH: defm "100000"
        defb 0
MAPPOS: defm "08, 10"
        defb 0
; SABRE_ATTR for the score panel at the top of the screen
SCOREC: defb 69, 69, 69, 69, 69, 69, 71, 71, 71,  7,  7,  7,  7,  7,  7, 87, 87,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 69, 69, 69, 69, 69
        defb 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70

TXT16:  defm "0001020304050607080910111213141516"

;---------------------------------------------------------------;
;                                                               ;
;   Global Variables                                            ;
;                                                               ;
;---------------------------------------------------------------;
;Scrolls:        defb 0                  ; the number of scrolls to do when drawing a sprite
TileTable:      defb 0
 
SM_Sprite:      defw SabreManWalkLeft   ; current sprite set to use for sabreman
SM_Frame:       defb 0                  ; current frame of the sprite set
SM_Pos:         defb 120,  80           ; current position of sabreman
SM_Color:       defb 71                 ; default color is Bright white on black
; old versions of the above
SM_OSprite:     defw SabreManWalkLeft   ; old sprite set to use for sabreman
SM_OFrame:      defb 0                  ; old frame of the sprite set
SM_OPos:        defb 128,  24           ; old position of sabreman
MAP_Coord:      defb 8, 10              ; the coordinates of the curren screen in the map
DoColTest:      defb 1                  ; state whether to do the collision test (zero - no,  non-zero yes)
