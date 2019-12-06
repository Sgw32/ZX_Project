
;---------------------------------------------------------------;
; WaitForSpace                                                  ;
;   wait for space key to be pressed                            ;
;---------------------------------------------------------------;
WaitForSpace:   ld bc, &BFFE           ; check keys from Space-B
                in a, (c)              ; get keys
                bit 0, a               ; if space pressed
                jr nz, WaitForSpace    ; no, then check again
                ret                    ; yes, return to caller

