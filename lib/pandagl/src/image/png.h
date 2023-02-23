int pd_png_reader_init(pd_image_reader_t* reader);

int pd_png_reader_read_header(pd_image_reader_t* reader);

int pd_png_reader_read_data(pd_image_reader_t* reader, pd_canvas_t *graph);

int pd_write_png_file(const char *file_name, const pd_canvas_t *graph);
