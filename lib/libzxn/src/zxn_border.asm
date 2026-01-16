SECTION code_user
PUBLIC _zxn_border_fastcall
EXTERN _SYSVAR_BORDCR

IO_BORDER   equ $FE
COLOR_MASK  equ $07

; ==============================================================================
; void zxn_border_fastcall(uint8_t uiColor)
; ------------------------------------------------------------------------------
; set color of border
; ------------------------------------------------------------------------------
; first (single) argument in (DEH)L (__z88dk_fastcall)
; ==============================================================================
_zxn_border_fastcall:
  push af
  push bc

  ld a, l
  and COLOR_MASK
  ld b, a
  rlca
  rlca
  rlca
  ld (_SYSVAR_BORDCR), a  ; Set sysvar BORDCR (0x5C48); color in BIT3:5
  in a, (IO_BORDER)
  and ~COLOR_MASK
  or b
  out (IO_BORDER), a

  pop bc
  pop af
  ; call 0x2297   ; BORDER-Routine im 48K-ROM: 0x2294 - omitting parameter check
  ret
