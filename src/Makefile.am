#

# Debug

if CHK_DEBUG
CFLAGS_DEBUG = -p -DDEBUG
else
CFLAGS_DEBUG = -DNDEBUG
endif

#
AM_CFLAGS = $(CFLAGS_DEBUG) -Wall \
	-DD_GTK \
	`sdl-config --cflags `
AM_CXXFLAGS = $(CFLAGS_DEBUG) -Wall \
	-DD_GTK \
	`sdl-config --cflags `
AM_LDFLAGS = \
	`sdl-config --libs` -lSDL_image -lSDL_mixer \
	-lGL -lGLU -lglut

bin_PROGRAMS = star-snow-notes
star_snow_notes_SOURCES = \
	main.h main.cpp \
	FileList.h FileList.cpp \
	Star.h Star.cpp Space.h Space.cpp \
	ControlPanel.h ControlPanel.cpp \
	Button.h Button.cpp \
	Option.h Option.cpp \
	Music.h Music.cpp 

EXTRA_DIST = 
