#! /usr/bin/python3 

import sys
from archivage import parcourir, archiver
from printer import print_info, print_error
from extraction import extraire

if len(sys.argv) == 2 and (sys.argv[1] == "--help" or sys.argv[1] == "-h") :
    print_info("""Utilisation de {} :\n\t{} source dest\tCompresse le
                          répertoire source dans l'archive dest\n\t{} -x archive\t
                          Extrait l'archive dans le répertoire courant.""".format(sys.argv[0], sys.argv[0], sys.argv[0]))
    sys.exit(0)

if len(sys.argv) != 3 :
    print_error("Impossible d'appeler {} avec {} arguments. Essayez {} --help | -h".format(sys.argv[0], len(sys.argv), sys.argv[0]))
    sys.exit(1)

if sys.argv[1] != "-x" :
    # Compresser et enregistrer les fichiers dans l'archive.
    archiver(parcourir(sys.argv[1]), sys.argv[2])
else :
    # Extraire les fichiers de l'archive
    extraire(sys.argv[2])
