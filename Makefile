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
clean:
	rm -rf *~ pl4 depends

