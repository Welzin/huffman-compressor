import sys, os, printer, subprocess

def parcourir(repertoire) :
    """!Parcours le répertoire donné pour lister tous les fichiers.

    Inclus les fichiers des sous-dossiers.

    @param repertoire Répertoire à parcourir.
    """
    fichiers = []

    if os.access(repertoire, os.X_OK) and os.access(repertoire, os.R_OK) :
        liste = os.listdir(repertoire)    
        for fichier in liste :
            if os.path.isdir(repertoire + '/' + fichier) :
                fichiers.extend(parcourir(repertoire + '/' + fichier))
            else :
                fichiers.append(repertoire + '/' + fichier)

    return fichiers

def compresser(nomFichier, dst) :
    """!Compresse le fichier donné

    Compresse le fichier donné grâce au compresseur Huffman fait en C.

    @param fichier Le fichier à compresser
    @param dst Fichier de destination
    @return Résultat de la compression. Si la compression a réussi, retourne Vrai
            Sinon, si le fichier compressé est plus gros que celui de base ou que
            la compression a raté, retourne Faux.
    """
    huf = "./build/huf"

    if not os.path.isfile(huf) :
        raise RuntimeError("L'exécutable de compression n'a pas été trouvé.")

    # Création d'un fichier temporaire dans lequel mettre le contenu du fichier compressé
    r = subprocess.Popen("{} --silent --skip \"{}\" {} > /dev/null 2>&1".format(huf, nomFichier, dst), shell=True)
    r.wait()

    # Si le code retourné n'est pas 0, c'est qu'il y a eu une erreur lors de la compression.
    return r.returncode == 0

def creer_entete(nomFichier) :
    """!Crée l'en-tête du fichier 

    Prend le nom de base du fichier et sa taille, et l'écrit dans l'en-tête.

    @param nomFichier Nom du fichier à concaténer
    @return L'en-tête à écrire dans le fichier d'archive
    """
    # Pour l'en-tête, on fait un truc simple avec le nom du fichier et la taille
    return "[F : {}]".format(nomFichier)

def ajouter(tete, fichier, archive) :  
    """!Ajoute le fichier compressé à l'archive.

    Écrit l'en-tête dans le fichier archive, puis le contenu du fichier compressé dans le fichier d'archive.

    @param tete En-tête à mettre avant le contenu du fichier.
    @param fichier Chemin vers le fichier.
    @param archive Chemin vers l'archive (fichier de destination)
    """ 
    with open(archive, "a") as dst :
        dst.write(tete)
    with open(archive, "ab") as dst :
        with open(fichier, "rb") as f :
            dst.write(f.read())

def archiver(fichiers, dst) :
    """!Archive les fichiers dans le chemin de destination

    @param fichiers Tous les fichiers à compresser et archiver
    @param dst Fichier dans lequel archiver
    """ 
    if os.path.isfile(dst) :
        os.remove(dst)

    result = True

    buildFile = dst + ".build"
    open(buildFile, "w").close()
    
    for fichier in fichiers :
        try :
            print(fichier)
            # Créer l'en-tête grâce à ça
            tete = creer_entete(fichier)
            # Ajouter le contenu du fichier compressé à la fin de l'archive
            ajouter(tete, fichier, buildFile)
        except RuntimeError as e :
            printer.print_error(*e.args)
            result = False
            break
        except FileNotFoundError :
            printer.print_error("Il y a eu un problème lors de la localisation du fichier {}. Arrêt.".format(fichier))
            result = False
            break
        except PermissionError :
            printer.print_error("Vous n'avez pas les permissions sur tous les fichiers. Arrêt.")
            result = False
            break

    reussite = compresser(buildFile, dst)

    if reussite :
        os.remove(buildFile)
    else :
        os.rename(buildFile, dst)

    if result :
        printer.print_success("Archivage réussi.")
    else :
        printer.print_error("Il y a eu un problème lors de l'archivage. Ce dernier n'a pas pu se terminer.")