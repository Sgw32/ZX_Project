;---------------------------------------------------------------;
;                                                               ;
;   GamePlay.inc                                                ;
;                                                               ;
;   This file contains routines for dealing with playing the    ;
;   Game                                                        ;
;                                                               ;
;---------------------------------------------------------------;

GameStartNew:   ld a, GF_GAME_OVER
                call SetGameFlag
                ret

GameOver:       ld b, 24                ; show the hi score table
                ld c, 72
                ld d, 10
                ld e, 22
                call WindowOpen
                ld hl, GameOverTxt1
                call PrintLineAt
                ld hl, GameOverTxt2
                call PrintLineAt
                ld hl, GameOverTxt3
                call PrintLineAt
                ld hl, GameOverTxt4
                call PrintLineAt
                call WaitForSpace       ; wait for space to be pressed
                call WindowClose
                call TransferScore      ; transfer the player score
                call IsNewHighScore     ; check to see if its a hi score
                call c, GetHiscoreName  ; if so, get the players name
                ld b, 24                ; show the hi score table
                ld c, 72
                ld d, 16
                ld e, 21
                call WindowOpen         ; open a window 21x17 at 72,24
                call DisplayHiScore     ; display the hiscore table
                ld hl, GameOverTxt4
                ld a, 136
                ld (GameOverTxt4+1), a
                call PrintLineAt
                ld a, 88
                ld (GameOverTxt4+1), a
                call WaitForSpace       ; wait for space to be pressed
                call WindowClose        ; close the window
                ld a, GF_GAME_OVER + GF_PLAYING
                call ResetGameFlag      ; reset the game over and playing flags
                ld a, GF_MENU
                call SetGameFlag        ; enter the main menu
                ret

GamePlay:       call MoveGameCursor
                ;ld a, 255
                ;call IncrementScore
                ;ld a, GF_GAME_OVER
                ;call SetGameFlag
                ret



;---------------------------------------------------------------;
; RandomNumber                                                  ;
;                                                               ;
; Chooses a random number from 0-a                              ;
;                                                               ;
;---------------------------------------------------------------;
RandomNumber:   push hl                 ; save registers
                push bc                 ; save registers
                ld c, a                 ; merge to C
                dec c                   ; computers does 0 n-1
                ld hl, (seed)           ; fetch RND pointer
                inc hl                  ; point to next number
                ld a, h
                and 63
                ld h, a                 ; but keep it in ROM
                ld a, r
                xor l
                ld l, a
                ld (seed), hl           ; store next RND-pointer
                ld a, (hl)              ; fetch RND number
RNmarge:        sub c                   ; calculate within marge
                jr nc, RNmarge
                adc a, c                ; 1 sub to much + range (1 - n) restore
                pop bc                  ; retrieve registers
                pop hl
                ret                     ; A holds RND value
seed            defw 0


