#### Compiler and tool definitions shared by all build targets #####
CC = gcc
BASICOPTS = -g
CFLAGS = $(BASICOPTS)

CPPFLAGS_main.c = \
	-I.
OBJS_main.c =  \
	$(TARGETDIR)/main.o \
	$(TARGETDIR)/transform.o
USERLIBS_main.c = -lATerm $(SYSLIBS_main.c) 
DEPLIBS_main.c =   
LDLIBS_main.c = $(USERLIBS_main.c)

TARGETDIR=.



# Build the interpreter
all: $(TARGETDIR)/interpreter

# Link or archive
$(TARGETDIR)/interpreter: $(TARGETDIR) $(OBJS_main.c) $(DEPLIBS_main.c)
	$(LINK.c) $(CFLAGS_main.c) $(CPPFLAGS_main.c) -o $@ $(OBJS_main.c) $(LDLIBS_main.c)

# Compile source files into .o files
$(TARGETDIR)/main.o: $(TARGETDIR) main.c
	$(COMPILE.c) $(CFLAGS_main.c) $(CPPFLAGS_main.c) -o $@ main.c

$(TARGETDIR)/transform.o: $(TARGETDIR) transform.c
	$(COMPILE.c) $(CFLAGS_main.c) $(CPPFLAGS_main.c) -o $@ transform.c

# Clean target deletes all generated files
clean:
	rm -f $(TARGETDIR)/interpreter $(TARGETDIR)/*.o



# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux