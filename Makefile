CC = gcc 

SRCDIR = ./src
OBJDIR = ./build

EXECNAME = cell-noise

CCFLAGS =	-I/usr/include/gdk-pixbuf-2.0 \
			-I/usr/include/libpng16 \
			-I/usr/include/glib-2.0 \
			-I/usr/lib/glib-2.0/include \
			-I/usr/include/SDL2 \

LIBSFLAGS = -lgdk_pixbuf-2.0 \
			-lgobject-2.0    \
			-lglib-2.0 		 \
			-lSDL2    		 \
			-lm			 	 \

C_SRCFILES = $(shell find $(SRCDIR) -type f -name "*.c")
C_OBJFILES = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(C_SRCFILES))

#@echo $(C_OBJFILES)

all: mk_obj_dir voxelspace 

voxelspace: $(C_OBJFILES) 
	$(CC) $(CCFLAGS) $^ $(LIBSFLAGS) -g -o $(EXECNAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CCFLAGS) -c -g $< -o $@

.PHONY: clean mk_obj_dir

clean:
	rm -rf $(EXECNAME) 
	rm -rf $(OBJDIR)

mk_obj_dir:
	mkdir -p $(OBJDIR)
