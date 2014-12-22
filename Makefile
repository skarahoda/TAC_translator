translator_lex.yy.c: translator/scanner.flx
	mkdir -p depends
	flex -o depends/translator_lex.yy.c translator/scanner.flx
translator_parser.tab.c: translator/parser.y
	mkdir -p depends
	bison -d translator/parser.y -o depends/translator_parser.tab.c
translator.o : translator/translator.c translator/translator.h
	mkdir -p depends
	gcc -c translator/translator.c -o depends/translator.o
pl4: translator_lex.yy.c translator_parser.tab.c translator.o
	gcc -o pl4 depends/translator_lex.yy.c depends/translator_parser.tab.c depends/translator.o -lfl
optimizer_lex.yy.c: optimizer/scanner.flx
	mkdir -p depends
	flex -o depends/optimizer_lex.yy.c optimizer/scanner.flx
optimizer_parser.tab.c: optimizer/parser.y
	mkdir -p depends
	bison -d optimizer/parser.y -o depends/optimizer_parser.tab.c
tac_data.o : optimizer/tac_data.c optimizer/tac_data.h
	mkdir -p depends
	gcc -c optimizer/tac_data.c -o depends/tac_data.o
tacopt: optimizer_lex.yy.c optimizer_parser.tab.c tac_data.o
	gcc -o tacopt depends/optimizer_lex.yy.c depends/optimizer_parser.tab.c depends/tac_data.o -lfl
all: pl4 tacopt
	$(info ==============================================================)
	$(info Done for all programs.)
clean:
	rm -rf *~ .*~ pl4 depends tacopt

