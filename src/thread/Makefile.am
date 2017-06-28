AUTOMAKE_OPTIONS=foreign subdir-objects
AM_CFLAGS = -I$(abs_top_srcdir)/include $(CODE_COVERAGE_CFLAGS)
noinst_LTLIBRARIES = libthread.la
libthread_la_SOURCES = pthread/thread.c pthread/mutex.c pthread/cond.c \
win32/thread.c win32/mutex.c win32/cond.c
