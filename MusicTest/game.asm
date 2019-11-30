;------------------------------------------------------------------------;
;                         Sochi Nights for Yandex Contest' 2019          ;
;   Based on the Gem Machine - A Bejewelled clone for the ZX Spectrum    ;
;   Based on the Sabre Wolf clone (TommyGun) for the ZX Spectrum         ;
;                                                                        ;
;                                                                        ;
;------------------------------------------------------------------------;

include "isr.inc"
main:
                call SND_SETSFXM    ; turn off sound fx
                call SFX_INIT
                ld hl, TITLE_TUNE
        	    call SND_INIT_HL
        	    call ISR_Start
;---------------------------------------------------------------;
; Programs main loop                                            ;
;   Controls the flow of the program                            ;
;---------------------------------------------------------------;
MainLoop:       
                jp MainLoop

include "ay_player.inc"

end start


