;extrn _abort:proc
;extrn _printf:proc
;extrn__fputn:proc
_TEXT segment byte public 'CODE' use16
extrn _strcmp:proc
extrn _strcpy:proc
_TEXT ends
extrn sin_fast:proc
extrn cos_fast:proc
