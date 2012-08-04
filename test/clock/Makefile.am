AUTOMAKE_OPTIONS=foreign 
INCLUDES = -I$(top_builddir)/include 
noinst_PROGRAMS = clock
EXTRA_DIST= README new_daytime_hour.png new_night_cup.png \
new_daytime_background.png  new_daytime_minute.png  night_hour.png \
new_daytime_cup.png  new_night_bg.png  night_min.png 

clock_SOURCES = clock.c 
clock_LDADD   = $(top_builddir)/src/libLCUI.la 

