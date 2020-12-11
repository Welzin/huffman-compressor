import os, subprocess, printer, re

def decompresser(archive) :
    """!Décompresse le fichier donné

    Décompresse le fichier donné grâce au décompresseur Huffman fait en C.

    @param archive Archive a décompresser
    @return Contenu de la décompression et résultat (booléen) du programme.
    """
    dehuf = "./build/dehuf"

    if not os.path.isfile(dehuf) :
        raise RuntimeError("L'exécutable de décompression n'a pas été trouvé.")

    # Création d'un fichier temporaire dans lequel mettre le contenu du fichier compressé
    r = subprocess.Popen("{} \"{}\" 2> /dev/null".format(dehuf, archive), shell=True, stdout=subprocess.PIPE)
    #returncode = r.wait()

    # Si le code retourné n'est pas 0, c'est qu'il y a eu une erreur lors de la compression.
    return (r.communicate(), r.returncode == 0)

def normalise_chemin(chemin, dstDir) :
    """!Normalise le chemin donné en mettant dstDir devant

    Pour un chemin quelconque, ajoute dstDir devant. Marche aussi pour des 
    chemins farfelus, comme ../, ./ ou /

    @param chemin Chemin a normaliser
    @param dstDir Répertoire de destination de l'archive
    @return Chemin normalisé.
    """
    if chemin[0] == '/' or chemin[0:2] == "./" or chemin[0:3] == "../" :
        chemin = chemin[chemin.find('/') + 1:]

    return os.path.join(dstDir, chemin)


def extraire(archive) :
    """!Extrait l'archive donnée.

    Décompresse l'archive et extrait tout son contenu dans un dossier du nom de
    l'archive (sans l'extension). Si un dossier de ce nom existe déjà, ajoute un
    _i avec 1 <= i < n tant que le dossier existe déjà.

    @param archive Archive à extraire
    """
    # Étape 1 : Décompresser le fichier d'archive
    contenu, etat = decompresser(archive)

    if not etat :
        contenu = open(archive, "rb").read()
    else :
        contenu = contenu[0]

    # Étape 2 : Récupérer toutes les en-têtes
    fichiers = re.findall(b"\[F : ([^\]]+)\]", contenu)

    # Étape 3 : Créer le dossier d'extraction
    dir = os.path.splitext(archive)[0]
    i = 1

    while os.path.isdir(dir) :
        if i == 1 :
            dir += "_1"
        else :
            dir = dir[:-1]
            dir += str(i)
        i += 1

    os.mkdir(dir)

    # Étape 4 : Créer les fichiers et mettre le contenu dedans
    if len(fichiers) > 0 :
        for i in range(len(fichiers) - 1) :
            deb = contenu.find(fichiers[i]) + len(fichiers[i]) + 1
            fin = contenu.find(fichiers[i + 1]) - 5
            fichier = normalise_chemin(fichiers[i].decode("utf-8"), dir)

            print(fichier)

            if not os.path.isdir(os.path.dirname(fichier)) :
                os.makedirs(os.path.dirname(fichier), 0o755)

            with open(fichier, "wb") as f :
                f.write(contenu[deb:fin])

        # Dernier fichier
        with open(normalise_chemin(fichiers[-1].decode("utf-8"), dir), "wb") as f :
            f.write(contenu[contenu.find(fichiers[-1]) + len(fichiers[-1]) + 1:])

        printer.print_success("Extraction réussie. Les fichiers ont été extraits dans le répertoire suivant : {}".format(dir))
    else :
        printer.print_error("Ce fichier n'est pas une archive !")
