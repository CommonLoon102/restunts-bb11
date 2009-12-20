;extrn _abort:proc
;extrn _printf:proc
;extrn__fputn:proc
_TEXT segment byte public 'CODE' use16
;extrn _strcmp:proc
;extrn _strcpy:proc
;extrn _strcat:proc
;extrn _strlen:proc
;extrn _strrchr:proc
;extrn _stricmp:proc
;extrn _rand:proc
;extrn _srand:proc
;extrn _printf:proc
;extrn _sprintf:proc
;extrn _flushall:proc
;extrn _abort:proc
;extrn _abs:proc
_TEXT ends

extrn sin_fast:proc
extrn cos_fast:proc

extrn mmgr_path_to_name:proc
;extrn fatal_error:proc
extrn mmgr_alloc_pages:proc

