#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "symboles.h"
#include "analyseur_lexical.h"
#include "util.h"

#define YYTEXT_MAX 100
#define is_num(c)(('0' <= (c)) && ((c) <= '9'))
#define is_maj(c)(('A' <= (c)) && ((c) <= 'Z'))
#define is_min(c)(('a' <= (c)) && ((c) <= 'z'))
#define is_alpha(c)(is_maj(c) || is_min(c) || (c) == '_' || (c) == '$')
#define is_alphanum(c)(is_num((c)) || is_alpha((c)))
 
extern FILE *yyin;

char *tableMotsClefs[] = {
	"si", 
};

int codeMotClefs[] = { 
	SI, 
};

char yytext[YYTEXT_MAX];
int yyleng;
int yylval;
int nbMotsClefs = 1;
/* Compter les lignes pour afficher les messages d'erreur avec numero ligne */
int nb_ligne = 1;

//int operateur_multichar( char c1, char c2, int token_multichar );

/*******************************************************************************
 * Fonction qui ignore les espaces et commentaires. 
 * Renvoie -1 si arrivÃ© Ã  la fin du fichier, 0 si tout va bien 
 ******************************************************************************/
int mangeEspaces()
{ 
	char c = fgetc(yyin);
	int comment = 0;
	while( comment || (c == ' ') || (c == '\n') || (c == '\t') || (c == '#' ) ) {
		if( c == '#' ) {
			comment = 1;
		}
		if( c == '\n' ) {
			nb_ligne++;
			comment = 0;
		}
		c = fgetc(yyin);    
	}
	if ( feof(yyin) ) {
		return -1;
	}    
	ungetc(c, yyin);
	return 0;
}

/*******************************************************************************
 * Lit un caractÃ¨re et le stocke dans le buffer yytext 
 ******************************************************************************/
int lireCar(void)
{
	yytext[yyleng++] = fgetc(yyin);
	yytext[yyleng] = '\0';
	return yytext[yyleng - 1];
}

/*******************************************************************************
 * Remet le dernier caractÃ¨re lu au buffer clavier et enlÃ¨ve du buffer yytext 
 ******************************************************************************/
void delireCar()
{
	char c;
	c = yytext[yyleng - 1];
	yytext[--yyleng] = '\0';
	ungetc(c, yyin);
}
/*******************************************************************************
 * Fonction principale de l'analyseur lexical, lit les caractÃ¨res de yyin et
 * renvoie les tokens sous forme d'entier.
 * Pour les tokens de type ID_FCT, ID_VAR et NOMBRE la
 * valeur du token est dans yytext, visible dans l'analyseur syntaxique.
 ******************************************************************************/
int yylex(void)
{
    // char c;
    int i;
    yytext[yyleng = 0] = '\0';
    
    while(!feof(yyin))
    {
        i = mangeEspaces();
        
        if(i == -1)
            return FIN;
        
        i = lireCar();
        
        // c = (char) i;
        
        // 1) Symboles simples
        
        switch((char) i)
        {
            case '+':
                return PLUS;
                break;
            case '-':
                return MOINS;
                break;
            case '*':
                return FOIS;
                break;
            case '/':
                return DIVISE;
                break;
            case '(':
                return PARENTHESE_OUVRANTE;
                break;
            case ')':
                return PARENTHESE_FERMANTE;
                break;
            case '[':
                return CROCHET_OUVRANT;
                break;
            case ']':
                return CROCHET_FERMANT;
                break;
            case '{':
                return ACCOLADE_OUVRANTE;
                break;
            case '}':
                return ACCOLADE_FERMANTE;
                break;
            case '=':
                return EGAL;
                break;
            case '<':
                return INFERIEUR;
                break;
            case '&':
                return ET;
                break;
            case '|':
                return OU;
                break;
            case '!':
                return NON;
                break;
            case ',':
                return VIRGULE;
                break;
            case ';':
                return POINT_VIRGULE;
                break;
        }
        
        // 2) NOMBRES
        
        if(is_num(i))
        {   
            while(is_num(i))
                i = lireCar();

            delireCar();
            return NOMBRE;
        }
        
        // 3) MOT CLEF DU LANGAGE
                
        if(is_alpha(i))
        {   
            while(is_alpha(i))
            {
                i = lireCar();
            }
            
            delireCar();
            
            if(yytext[0] == '$')
                return ID_VAR;

            if(strcmp(yytext, "si") == 0)
                return SI;
            else if(strcmp(yytext, "alors") == 0)
                return ALORS;
            else if(strcmp(yytext, "sinon") == 0)
                return SINON;
            else if(strcmp(yytext, "tantque") == 0)
                return TANTQUE;
            else if(strcmp(yytext, "faire") == 0)
                return FAIRE;
            else if(strcmp(yytext, "retour") == 0)
                return RETOUR;
            else if(strcmp(yytext, "lire") == 0)
                return LIRE;
            else if(strcmp(yytext, "ecrire") == 0)
                return ECRIRE;
            else 
                return ID_FCT;
        }
    }
    
    return -1;
}

/*******************************************************************************
 * Fonction auxiliaire appelÃ©e par l'analyseur syntaxique tout simplement pour 
 * afficher des messages d'erreur et l'arbre XML 
 ******************************************************************************/
void nom_token( int token, char *nom, char *valeur ) {
	int i;
	  
	strcpy( nom, "symbole" );
	if(token == POINT_VIRGULE) strcpy( valeur, "POINT_VIRGULE");
	else if(token == PLUS) strcpy(valeur, "PLUS");
	else if(token == MOINS) strcpy(valeur, "MOINS");
	else if(token == FOIS) strcpy(valeur, "FOIS");
	else if(token == DIVISE) strcpy(valeur, "DIVISE");
	else if(token == PARENTHESE_OUVRANTE) strcpy(valeur, "PARENTHESE_OUVRANTE");
	else if(token == PARENTHESE_FERMANTE) strcpy(valeur, "PARENTHESE_FERMANTE");
	else if(token == CROCHET_OUVRANT) strcpy(valeur, "CROCHET_OUVRANT");
	else if(token == CROCHET_FERMANT) strcpy(valeur, "CROCHET_FERMANT");
	else if(token == ACCOLADE_OUVRANTE) strcpy(valeur, "ACCOLADE_OUVRANTE");
	else if(token == ACCOLADE_FERMANTE) strcpy(valeur, "ACCOLADE_FERMANTE");
	else if(token == EGAL) strcpy(valeur, "EGAL");
	else if(token == INFERIEUR) strcpy(valeur, "INFERIEUR");
	else if(token == ET) strcpy(valeur, "ET");
	else if(token == OU) strcpy(valeur, "OU");
	else if(token == NON) strcpy(valeur, "NON");
	else if(token == SI) strcpy(valeur, "SI");
	else if(token == ALORS) strcpy(valeur, "ALORS");
	else if(token == SINON) strcpy(valeur, "SINON");
	else if(token == TANTQUE) strcpy(valeur, "TANTQUE");
	else if(token == FAIRE) strcpy(valeur, "FAIRE");
	else if(token == ENTIER) strcpy(valeur, "ENTIER");
	else if(token == RETOUR) strcpy(valeur, "RETOUR");
	else if(token == LIRE) strcpy(valeur, "LIRE");
	else if(token == ECRIRE) strcpy(valeur, "ECRIRE");
	else if(token == FIN) strcpy(valeur, "FIN");
	else if(token == VIRGULE) strcpy(valeur, "VIRGULE");

	else if( token == ID_VAR ) {
		strcpy( nom, "id_variable" );  
		strcpy( valeur, yytext );        
	}
	else if( token == ID_FCT ) {
		strcpy( nom, "id_fonction" );    
		strcpy( valeur, yytext );    
	}
	else if( token == NOMBRE ) {
		strcpy( nom, "nombre" );
		strcpy( valeur, yytext ); 
	}
	else {
		strcpy( nom, "mot_clef" );
		for(i=0; i < nbMotsClefs; i++){
			if( token ==  codeMotClefs[i] ){
				strcpy( valeur, tableMotsClefs[i] );
				break;
			}
		}
	}  
}
/*******************************************************************************
 * Fonction auxiliaire appelÃ©e par le compilo en mode -l, pour tester l'analyseur
 * lexical et, Ã©tant donnÃ© un programme en entrÃ©e, afficher la liste des tokens.
 ******************************************************************************/

void test_yylex_internal(FILE *yyin) {
	int uniteCourante;
	uniteCourante = yylex();
	while (uniteCourante != FIN) {
		printf("%s %d\n", yytext, uniteCourante);
		uniteCourante = yylex();
	}
	printf("%s %d\n", yytext, uniteCourante);
}
