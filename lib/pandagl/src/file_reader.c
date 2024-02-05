#include <stdio.h>
#include <stdlib.h>
#include <pandagl/file_reader.h>

static size_t pd_file_stream_on_read(void *data, void *buffer, size_t size)
{
        return fread(buffer, 1, size, data);
}

static void pd_file_stream_on_skip(void *data, long offset)
{
        fseek(data, offset, SEEK_CUR);
}

static void pd_file_stream_on_rewind(void *data)
{
        rewind(data);
}

static void pd_file_stream_on_close(void *data)
{
        fclose(data);
}

pd_file_reader_t *pd_file_reader_create(void)
{
	return calloc(1, sizeof(pd_file_reader_t));
}

pd_file_reader_t *pd_file_reader_create_from_file(const char *filename)
{
        FILE *fp;
        pd_file_reader_t *reader;

        fp = fopen(filename, "rb");
	if (fp == NULL) {
		return NULL;
	}
	reader = pd_file_reader_create();
        reader->stream_data = fp;
        reader->fn_skip = pd_file_stream_on_skip;
        reader->fn_read = pd_file_stream_on_read;
        reader->fn_rewind = pd_file_stream_on_rewind;
        reader->fn_close = pd_file_stream_on_close;
	return reader;
}

void pd_file_reader_destroy(pd_file_reader_t *reader)
{
	reader->fn_close(reader->stream_data);
	free(reader);
}
