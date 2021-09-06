<<<<<<< HEAD
PROJ_NAME=Teste

# .c files
C_SOURCE=$(wildcard ./source/*.c)

# .h files
H_SOURCE=$(wildcard ./source/*.h)

# Object files
OBJ=$(subst .c,.o,$(subst source,objects,$(C_SOURCE)))

LIBS=$(wildcard ./lib/gcc/*.a)

# Compiler and linker
CC=gcc

# Flags for compiler
CC_FLAGS=-c         		\
         -W         		\
		 -g						


ifeq ($(OS),Windows_NT)
LIB=-lcsfml-graphics	\
	-lcsfml-audio		\
	-lcsfml-window		\
	-lcsfml-system		\
	-lmingw32  			
else
LIB=-lcsfml-graphics	\
	-lcsfml-audio		\
	-lcsfml-window		\
	-lcsfml-system		
endif
#	-Xlinker /subsystem:windows

# Command used at clean target
RM = rm -rf

#
# Compilation and linking
#
all: objects bin ./bin/$(PROJ_NAME)

./bin/$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ $(LIB) -o $@
	@ echo 'Finished building binary: $@'
	@ echo ' '

./objects/%.o: ./source/%.c ./source/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) $(LIB) -o $@
	@ echo ' '

./objects/main.o: ./source/main.c $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) $(LIB) -o $@
	@ echo ' '

objects:
	@ mkdir objects

bin:
	@ mkdir bin

clean:
ifeq ($(OS),Windows_NT)
	@ rd /s /q objects
	@ rd /s /q bin
else
	@ rm -rf objects
	@ rm -rf bin
endif

=======
PROJ_NAME=Teste

# .c files
C_SOURCE=$(wildcard ./source/*.c)

# .h files
H_SOURCE=$(wildcard ./source/*.h)

# Object files
OBJ=$(subst .c,.o,$(subst source,objects,$(C_SOURCE)))

LIBS=$(wildcard ./lib/gcc/*.a)

# Compiler and linker
CC=gcc

# Flags for compiler
CC_FLAGS=-c         		\
         -W         		\
		 -Wall				\
		 -g					\
         -std=c99			

LIB=-lmingw32  			\
	-Iinclude			\
	-lcsfml-graphics	\
	-lcsfml-audio		\
	-lcsfml-window		\
	-lcsfml-system		\
#	-Xlinker /subsystem:windows

# Command used at clean target
RM = rm -rf

#
# Compilation and linking
#
all: objects $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ $(LIB) -o $@.exe
	@ echo 'Finished building binary: $@.exe'
	@ echo ' '

./objects/%.o: ./source/%.c ./source/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) $(LIB) -o $@
	@ echo ' '

./objects/main.o: ./source/main.c $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) $(LIB) -o $@
	@ echo ' '

objects:
	@ mkdir objects

clean:
	@ rd /s /q objects

>>>>>>> 288559f1b14d788b6b0130864a5605111b3d1ab4
.PHONY: all clean