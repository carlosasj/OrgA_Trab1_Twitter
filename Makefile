#	Carlos Alberto Schneider Junior
#	NUSP: 9167910
#	Turma: 2 (terça)
#	Data de entrega: 24/03/2015
#	Data Limite: 24/03/2015

EXEC=twttr
SOURCES= main.c

RM=rm -rf

ZIP_NAME=Trab1-Twitter-0000000-9167910-2222222-3333333.zip

all:
	gcc -std=c99 $(SOURCES) -o $(EXEC) -lm

run:
	./$(EXEC)

zip:
	@$(RM) $(ZIP_NAME);zip $(ZIP_NAME) *.h *.c Makefile