int pd_bmp_reader_init(pd_image_reader_t* reader);

int pd_bmp_reader_read_header(pd_image_reader_t* reader);

int pd_bmp_reader_read_data(pd_image_reader_t* reader, pd_canvas_t *graph);
