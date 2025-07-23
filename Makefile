ifeq ($(OS),Windows_NT)
# Windows
    CC = clang
    LN = lld-link
    RM = del /Q /F
    MKDIR = mkdir
    EXEC = bin/proda.exe
    DOCKER = docker
    PATHSEP = \\
else
# Linux
    CC = clang
    LN = ld
    RM = rm -f
    MKDIR = mkdir -p
    EXEC = bin/proda
    DOCKER = docker
    PATHSEP = /
endif


SRC = scripts/src
HEADERS_DIR = scripts/headers
BUILD = build
BIN = bin

# Исключение app.log и других не-C-файлов
SOURCES = $(filter-out $(SRC)/app.log, $(wildcard $(SRC)/*.c))
HEADERS = $(wildcard $(HEADERS_DIR)/*.h)
OBJ = $(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(SOURCES))

CFLAGS = -Wall -Wextra -std=c11 -I$(HEADERS_DIR)
LDFLAGS = 

.PHONY: all clean build-dir lint test docker-build docker-run ci deploy monitor

all: $(EXEC)

$(EXEC): $(OBJ)
    @$(MKDIR) $(BIN)
    $(CC) $(CFLAGS) $(OBJ) -o $(EXEC) $(LDFLAGS)

$(BUILD)/%.o: $(SRC)/%.c $(HEADERS)
    @$(MKDIR) $(BUILD)
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    $(RM) $(subst /,$(PATHSEP),$(BUILD)/*) $(subst /,$(PATHSEP),$(BIN)/*) *.i *.s *.o

lint:
    clang-tidy $(SOURCES) -- $(CFLAGS)

test: all
    $(subst /,$(PATHSEP),$(EXEC)) --test

docker-build:
    $(DOCKER) build -t proda-server -f dockerfile .

docker-run:
    $(DOCKER) run -p 8080:80 proda-server

ci: lint test docker-build

deploy: 
    python script.py --deploy

monitor:
    python script.py --monitor