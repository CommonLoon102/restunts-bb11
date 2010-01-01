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

extrn file_find:proc
extrn file_find_next:proc
extrn file_paras:proc
extrn file_paras_fatal:proc
extrn file_paras_nofatal:proc
extrn file_decomp_paras:proc
extrn file_decomp_paras_fatal:proc
extrn file_decomp_paras_nofatal:proc
extrn file_read:proc
extrn file_read_fatal:proc
extrn file_read_nofatal:proc
extrn file_write_fatal:proc
extrn file_write_nofatal:proc
extrn file_decomp:proc
extrn file_decomp_fatal:proc
extrn file_decomp_nofatal:proc
extrn file_load_binary:proc
extrn file_load_binary_fatal:proc
extrn file_load_binary_nofatal:proc
extrn file_load_resource:proc
extrn file_load_resfile:proc
extrn file_load_3dres:proc
extrn file_load_audiores:proc

extrn mmgr_path_to_name:proc
extrn mmgr_alloc_pages:proc
extrn mmgr_alloc_resmem:proc
extrn mmgr_alloc_a000:proc
extrn mmgr_get_ofs_diff:proc
extrn mmgr_free:proc
extrn mmgr_copy_paras:proc
extrn copy_paras_reverse:proc
extrn mmgr_get_unk:proc
extrn mmgr_find_free:proc
extrn mmgr_op_unk2:proc
extrn mmgr_get_chunk_size:proc
extrn mmgr_resize_memory:proc
extrn mmgr_op_unk:proc
extrn mmgr_alloc_resbytes:proc

extrn locate_shape_fatal:proc
extrn locate_shape_nofatal:proc
extrn locate_shape_alt:proc
extrn locate_sound_fatal:proc
extrn locate_many_resources:proc

extrn stuntsmain:proc

;extrn fatal_error:proc
