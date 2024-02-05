
#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_FILE_READER_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_FILE_READER_H

#include "common.h"
#include "types.h"

PD_BEGIN_DECLS

typedef struct pd_file_reader_t {
        void *stream_data;
        void (*fn_close)(void*);
        void (*fn_rewind)(void*);
        size_t (*fn_read)(void *, void *, size_t);
        void (*fn_skip)(void *, long);
} pd_file_reader_t;

PD_INLINE size_t pd_file_reader_read(pd_file_reader_t *reader, void *buffer,
                                      size_t size)
{
        return reader->fn_read(reader->stream_data, buffer, size);
}

PD_INLINE void pd_file_reader_rewind(pd_file_reader_t *reader)
{
        reader->fn_rewind(reader->stream_data);
}

PD_INLINE void pd_file_reader_skip(pd_file_reader_t *reader, long bytes)
{
        reader->fn_skip(reader->stream_data, bytes);
}

PD_PUBLIC pd_file_reader_t *pd_file_reader_create(void);
PD_PUBLIC pd_file_reader_t *pd_file_reader_create_from_file(const char *filename);
PD_PUBLIC void pd_file_reader_destroy(pd_file_reader_t *reader);

PD_END_DECLS

#endif
