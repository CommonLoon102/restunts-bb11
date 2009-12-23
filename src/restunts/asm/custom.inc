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

extrn load_binary_file:proc
extrn load_binary_file_fatal:proc
extrn load_binary_file_nofatal:proc

extrn mmgr_path_to_name:proc
extrn mmgr_alloc_pages:proc
extrn mmgr_alloc_resmem:proc
extrn mmgr_alloc_a000:proc
extrn mmgr_get_ofs_diff:proc
extrn mmgr_free:proc
extrn mmgr_copy_paras:proc


;extrn fatal_error:proc
