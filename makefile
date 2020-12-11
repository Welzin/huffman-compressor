OUTPUT_HUF = build/huf
OUTPUT_DEHUF = build/dehuf
CC = @gcc
SOURCES_HUF = $(shell find src/huf/ -type f -name '*.c')
SOURCES_DEHUF = $(shell find src/dehuf/ -type f -name '*.c')
CORE = $(shell find src/core/ -type f -name '*.c')

DIR = $(shell if [ ! -d "build" ]; then mkdir "build"; fi)
DOC = $(shell make doc > /dev/null 2>&1)

MESSAGE = @echo "Build réussi ! Exécutables trouvables dans build/"
MESSAGE_D = $(MESSAGE)" (Debug)"
MSG_DOC = "La documentation a été bien générée dans projet_doc/"
MSG_PYT = "L'exécutable python (par) a bien été généré dans le dossier courant."

.PHONY: doc python

ifdef DEBUG
all: 
	$(DIR)
	$(CC) -g $(SOURCES_HUF) $(CORE) -o $(OUTPUT_HUF)
	$(CC) -g $(SOURCES_DEHUF) $(CORE) -o $(OUTPUT_DEHUF)
	$(MESSAGE_D)
else
all: 
	$(DIR)
	$(CC) $(SOURCES_HUF) $(CORE) -o $(OUTPUT_HUF)
	$(CC) $(SOURCES_DEHUF) $(CORE) -o $(OUTPUT_DEHUF)
	@if [ ! -f "par" ]; then make python > /dev/null 2>&1; echo $(MSG_PYT); fi
	$(MESSAGE)
	@if [ ! -d "projet_doc" ]; then make doc > /dev/null 2>&1; echo $(MSG_DOC); fi
endif

clean:
	@rm -f build/*
	@rm -f src/*~
	@rm -f src/*\#*
	@rm -rf projet_doc/*
	@echo "Nettoyage terminé. Les exécutables et la documentations ont bien été effacés."

doc:
	@doxygen doc/Doxyfile
	@echo $(MSG_DOC)

python:
	@pyinstaller --onefile src/python/par.py > /dev/null
	@mv dist/par .
	@rmdir dist
	@rm par.spec
	@echo $(MSG_PYT)
