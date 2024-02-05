void pd_png_reader_create(pd_image_reader_t *reader);
void pd_png_reader_destroy(pd_image_reader_t *reader);
jmp_buf *pd_png_reader_jmpbuf(pd_image_reader_t *reader);
pd_error_t pd_png_reader_read_header(pd_image_reader_t *reader);
void pd_png_reader_start(pd_image_reader_t *reader);
void pd_png_reader_read_row(pd_image_reader_t *reader, pd_canvas_t *graph);
void pd_png_reader_finish(pd_image_reader_t *reader);
