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
db 'Hello world 1!',14

start:
; Clear screen
call cls

; Open upper screen channel
ld a,2
call opench

; Print string
ld de,string
ld bc,14
call print

; Print string
;call loader2
ret
;call loader2

end start 


org $8100

; Define some ROM routines
cls EQU $0D6B
opench EQU $1601
print EQU $203C
;clear EQU $1A0D
clear EQU $1EAC
loader2 EQU $8100
; Define our string to print
string:
db 'Hello world 1!',14

start:
; Clear screen
call cls

; Open upper screen channel
ld a,2
call opench

ld de,string
ld bc,14
call print
ret

;clear 24316
;poke 23610,255
;load "" code
;randomize usr 24317