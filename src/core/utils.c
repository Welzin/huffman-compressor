#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void affiche_type(Type type) {
    const char *types[5] = {
        "[\033[1;32mSuccès\033[0m] ",
        "[\033[1;36mInfo\033[0m] ",
        "[\033[1;33mAvertissement\033[0m] ",
        "[\033[1;31mErreur\033[0m] ",
        "[\033[1;34mDebug\033[0m] ",
    };

    printf("%s", types[type]);
}

int chaines_egales(const char* c1, const char* c2) {
    if (strlen(c1) != strlen(c2)) {
        return 0;
    }

    size_t i = 0;
    int identiques = 1;
    while (identiques && c1[i] != '\0' && c2[i] != '\0') {
        identiques = (c1[i] == c2[i]);
        ++i;
    }

    return identiques;
}

char *itoa(size_t entier) {
    size_t nbChiffres = (entier == 0), copy = entier;
  
    while (copy != 0) { 
        copy /= 10; 
        nbChiffres++; 
    }
  
    char *chaine = (char *) malloc((nbChiffres + 1) * sizeof(char));
    chaine[nbChiffres] = '\0';

    for (int i = nbChiffres - 1; i >= 0; --i) {
        chaine[i] = ('0' + entier % 10);
        entier /= 10;
    }
  
    return chaine;
}

int recuperer_occurrences(FILE *fd, size_t *out) {
    int good = 1, c;
    while (good && (c = fgetc(fd)) != EOF) {
        if (c >= '0' && c <= '9') {
            *out *= 10;
            *out += c - '0';
        }
        else if (c == DELIM) {
            break;
        }
        else {
            good = 0;
        }
    }

    return good;
}