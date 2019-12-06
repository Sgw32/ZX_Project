;---------------------------------------------------------------;
;                                                               ;
;   GamePlay.inc                                                ;
;                                                               ;
;   This file contains routines for dealing with playing the    ;
;   Game                                                        ;
;                                                               ;
;---------------------------------------------------------------;

Screen1:    
             ld hl, CNT1
             ld (hl), 2
             ld hl, Moscow2        ; display the title screen
SN1_PGLoop1:      
             call DrawScr            ; draw the screen    
             call Pause1000              ; pause between key presses  
             
             call ClrScr
             call Pause1000              ; pause between key presses
             
             ld a,(CNT1)
             dec a
             ld (CNT1),a
             jr nz, SN1_PGLoop1               ; keep going still the game is finished
             ld hl, CNT1
             ld (hl), 2
             ld hl, Moscow2        ; display the title screen
SN1_PGLoop2:      
             dec bc
             call DrawScr            ; draw the screen    
             call Pause60000              ; pause between key presses  
             
             call ClrScr
             call Pause60000              ; pause between key presses
             ld a,(CNT1)
             dec a
             ld (CNT1),a
             jr nz, SN1_PGLoop2               ; keep going still the game is finished
             
SN1_ExitFromSplash:
             ld hl, Moscow2        ; display the title screen
             call DrawScr            ; draw the screen 
             call WaitForSpace
             call ClrScr
             call Pause60000
             ret             
             


Screen2:    
             ld hl, CNT1
             ld (hl), 2
             ld hl, NII1        ; display the title screen
SN2_PGLoop1:      
             call DrawScr            ; draw the screen    
             call Pause1000              ; pause between key presses  
             
             call ClrScr
             call Pause1000              ; pause between key presses
             
             ld a,(CNT1)
             dec a
             ld (CNT1),a
             jr nz, SN2_PGLoop1               ; keep going still the game is finished
             ld hl, CNT1
             ld (hl), 2
             ld hl, NII1        ; display the title screen
SN2_PGLoop2:      
             dec bc
             call DrawScr            ; draw the screen    
             call Pause60000              ; pause between key presses  
             
             call ClrScr
             call Pause60000              ; pause between key presses
             ld a,(CNT1)
             dec a
             ld (CNT1),a
             jr nz, SN2_PGLoop2               ; keep going still the game is finished
             
SN2_ExitFromSplash:
             ld hl, NII1        ; display the title screen
             call DrawScr            ; draw the screen 
             call WaitForSpace
             call ClrScr
             call Pause60000
             ret             
             
Screen3:    
             ld hl, CNT1
             ld (hl), 2
             ld hl, Corridors1        ; display the title screen
SN3_PGLoop1:      
             call DrawScr            ; draw the screen    
             call Pause1000              ; pause between key presses  
             
             call ClrScr
             call Pause1000              ; pause between key presses
             
             ld a,(CNT1)
             dec a
             ld (CNT1),a
             jr nz, SN3_PGLoop1               ; keep going still the game is finished
             ld hl, CNT1
             ld (hl), 2
             ld hl, Corridors1        ; display the title screen
SN3_PGLoop2:      
             dec bc
             call DrawScr            ; draw the screen    
             call Pause60000              ; pause between key presses  
             
             call ClrScr
             call Pause60000              ; pause between key presses
             ld a,(CNT1)
             dec a
             ld (CNT1),a
             jr nz, SN3_PGLoop2               ; keep going still the game is finished
             
SN3_ExitFromSplash:
             ld hl, Corridors1        ; display the title screen
             call DrawScr            ; draw the screen 
             call WaitForSpace
             call ClrScr
             call Pause60000
             ret             
             

