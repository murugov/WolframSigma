FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

COMMON_INCLUDES		= -I./COMMON/headers
CONFIG_INCLUDES 	= -I./CONFIG
STK_INCLUDES		= -I./STACK/headers
TREE_INCLUDES		= -I./TREE/headers
GEN_INCLUDES		= -I./GENERATOR/headers -I./GENERATOR/src -I./GENERATOR/ReportFiles
WOLFRAM_INCLUDES    = -I./WOLFRAM_SIGMA/headers

COMMON_FILES  = COMMON/HAshStr.cpp COMMON/IsBadPtr.cpp COMMON/LineCounter.cpp COMMON/logger.cpp COMMON/SizeFile.cpp COMMON/Speak.cpp COMMON/TXTreader.cpp
TREE_FILES 	  = TREE/TreeFunc.cpp
WOLFRAM_FILES = WOLFRAM_SIGMA/WolfFunc.cpp WOLFRAM_SIGMA/DataReader.cpp WOLFRAM_SIGMA/GenGraphs.cpp WOLFRAM_SIGMA/CalcFunc.cpp

all: help

wolf: WOLFRAM_SIGMA/main_wolf.cpp $(COMMON_FILES) $(TREE_FILES) $(WOLFRAM_FILES)
	@echo "-----------------------------------------------------------------------------------------"
	g++ -o wolf_program $(FLAGS) WOLFRAM_SIGMA/main_wolf.cpp $(COMMON_INCLUDES) $(CONFIG_INCLUDES) $(GEN_INCLUDES) $(STK_INCLUDES) $(TREE_INCLUDES) $(WOLFRAM_INCLUDES) $(COMMON_FILES) $(TREE_FILES) $(WOLFRAM_FILES)
	@echo "-----------------------------------------------------------------------------------------"

gen: GENERATOR/main_gen.cpp $(COMMON_FILES) $(TREE_FILES)
	@echo "-----------------------------------------------------------------------------------------"
	g++ -o gen_program $(FLAGS) GENERATOR/main_gen.cpp $(COMMON_INCLUDES) $(CONFIG_INCLUDES) $(STK_INCLUDES) $(TREE_INCLUDES) $(GEN_INCLUDES) $(COMMON_FILES) $(TREE_FILES)
	@echo "-----------------------------------------------------------------------------------------"

run-wolf: wolf
	./wolf_program

run-gen: gen
	./gen_program

run: run-wolf

clean:
	rm -f wolf_program

help:
	@echo "Available commands:"
	@echo ""
	@echo "  make wolf                     - compile a wolfram"
	@echo "  make run-wolf                 - compile and run wolfram"
	@echo "  make run                      - compile and run wolfram"
	@echo ""
	@echo "  make clean                    - remove compiled programs"

.PHONY: wolf run-wolfrun clean help