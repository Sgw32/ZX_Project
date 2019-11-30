org $8000

; Define some ROM routines
cls EQU $0D6B
opench EQU $1601
print EQU $203C
;clear EQU $1A0D
clear EQU $1EAC
loader2 EQU $8100
; Define our string to print
string:
db 'Loading!',8

start:
; Clear screen
call cls

; Open upper screen channel
ld a,2
call opench

; Print string
ld de,string
ld bc,8
call print

; Print string
;call loader2

;call WaitForSpace
;jp $1af0

load_and_run_file:
    ld ix, $8000
    ld de,  29
    push ix
    scf 
    ld a,#ff
    inc d
    exa
    dec d
    jp $0556
    ;jp $28С7
    ;jp $1af1
    ;jp load_and_run_file
    ;jp $8000

ret
;call loader2

end start 