CC=g++

CFLAGS = 	-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef 				\
			-Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations 			\
			-Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ 				\
			-Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion 		\
			-Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2	 			\
			-Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers 			\
			-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo 		\
			-Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits 	\
			-Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

OBJ = obj
SRC = src

COMMON_LIB_FOLDER = ./../../mylibs/
LIB_NAMES 			= tree onegin
LIB_SUBST 			= -L$(COMMON_LIB_FOLDER)$(lib_name)/bin -l$(lib_name)
LIB_HEADERS_SUBST 	= -I $(COMMON_LIB_FOLDER)$(lib_name)/headers

LIBS 		= $(foreach lib_name,$(LIB_NAMES),$(LIB_SUBST))
LIB_HEADERS	= $(foreach lib_name,$(LIB_NAMES),$(LIB_HEADERS_SUBST))
SOURCES 	= $(wildcard $(SRC)/*.cpp)
OBJFILES 	= $(patsubst $(SRC)/%,$(OBJ)/%,$(SOURCES:.cpp=.o))
OUT 		= main.exe
DUMP_FOLDER = .\dumps

$(OUT) : $(OBJFILES)
	@$(CC) -o $@ $(CFLAGS) $^ $(LIB_HEADERS) $(LIBS)

$(OBJ)/%.o : $(SRC)/%.cpp
	@$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f $(OBJFILES) $(OUT)

.PHONY: clean_dumps
clean_dumps:
	rm -r -f $(DUMP_FOLDER)
	mkdir $(DUMP_FOLDER)
