#include <stdio.h>
#include <stdlib.h>
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "symboles.h"
#include "affiche_arbre_abstrait.h"
#include "dico.h"

char yytext[100];
FILE *yyin;

int uniteCourante;

int main(int argc, char **argv) {
    char nom[100];
    char valeur[100];
    yyin = fopen(argv[1], "r");
    if(yyin == NULL){
        fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[1]);
        exit(1);
    }

    uniteCourante = yylex();

    init();

    n_prog* pg = PG();

    printf("\nArbre abstrait ====>\n\n");
    affiche_n_prog(pg);
    printf("\nTable des symboles ======>\n\n");
    symbole_n_prog(pg);
    affiche_dico();
    return 0;
}
