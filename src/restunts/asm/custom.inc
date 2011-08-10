; this empty segment is placed at the end of the executable.
; it is used by the hacked crt to determine how large the image is
endseg segment byte public 'ENDSEG' use16
endseg ends

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
extrn polarAngle:proc
extrn polarRadius3D:proc
extrn polarRadius2D:proc
extrn rect_compare_point:proc
extrn rect_adjust_from_point:proc
extrn mat_mul_vector:proc
extrn mat_multiply:proc
extrn mat_invert:proc
extrn mat_rot_x:proc
extrn mat_rot_y:proc
extrn mat_rot_z:proc
extrn mat_rot_zxy:proc
extrn vector_op_unk:proc
extrn vector_op_unk2:proc
extrn vector_to_point:proc
extrn set_projection:proc
extrn select_cliprect_rotate:proc
extrn polyinfo_reset:proc
extrn init_polyinfo:proc
extrn calc_sincos80:proc

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
extrn file_decomp_vle:proc
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
extrn mmgr_get_chunk_by_name:proc
extrn mmgr_find_free:proc
extrn mmgr_release:proc
extrn mmgr_get_chunk_size:proc
extrn mmgr_resize_memory:proc
extrn mmgr_op_unk:proc
extrn mmgr_alloc_resbytes:proc
extrn mmgr_get_res_ofs_diff_scaled:proc
extrn mmgr_get_chunk_size_bytes:proc

extrn locate_shape_fatal:proc
extrn locate_shape_nofatal:proc
extrn locate_shape_alt:proc
extrn locate_sound_fatal:proc
extrn locate_many_resources:proc
extrn locate_text_res:proc

extrn shape3d_load_all:proc
extrn shape3d_free_all:proc
extrn shape3d_init_shape:proc
extrn transformed_shape_op:proc
extrn transformed_shape_op_helper2:proc
extrn transformed_shape_op_helper3:proc
extrn get_a_poly_info:proc
extrn preRender_default:proc
extrn preRender_default_alt:proc
;extrn preRender_helper:proc
;extrn preRender_helper3:proc
extrn skybox_op_helper:proc
extrn preRender_wheel_helper4:proc
extrn preRender_patterned:proc
extrn preRender_unk:proc

extrn draw_line_related:proc
extrn draw_line_related_alt:proc

extrn sprite_make_wnd:proc
extrn sprite_free_wnd:proc
extrn sprite_set_1_from_argptr:proc
extrn sprite_copy_2_to_1:proc
extrn sprite_copy_2_to_1_2:proc
extrn sprite_copy_2_to_1_clear:proc
extrn sprite_copy_wnd_to_1:proc
extrn sprite_copy_wnd_to_1_clear:proc
extrn sprite_copy_both_to_arg:proc
extrn sprite_copy_arg_to_both:proc
extrn sprite_clear_1_color:proc
extrn sprite_putimage:proc
extrn sprite_putimage_and:proc
extrn sprite_putimage_or:proc
extrn file_load_shape2d:proc
extrn file_load_shape2d_fatal:proc
extrn file_load_shape2d_nofatal:proc
extrn file_load_shape2d_nofatal2:proc
extrn file_load_shape2d_res:proc
extrn file_load_shape2d_res_fatal:proc
extrn file_load_shape2d_res_nofatal:proc
extrn file_unflip_shape2d:proc
extrn file_get_unflip_size:proc
extrn file_get_shape2d:proc

extrn kb_init_interrupt:proc
extrn kb_exit_handler:proc
extrn kb_int9_handler:proc
extrn kb_int16_handler:proc
extrn kb_get_key_state:proc
extrn kb_call_readchar_callback:proc
extrn kb_read_char:proc
extrn kb_checking:proc
extrn flush_stdin:proc
extrn kb_check:proc

extrn init_kevinrandom:proc
extrn get_kevinrandom_seed:proc
extrn get_kevinrandom:proc
extrn get_super_random:proc

extrn stuntsmain:proc

;extrn fatal_error:proc
