# Compresseur de fichiers & Archiveur

Réalisation d'un compresseur - décompresseur de fichiers en C grâce à l'algorithme d'Huffman dans le cadre d'une unité d'enseignement

## Pré-requis

Ce petit programme est développé en C et en python, il est donc nécessaire d'avoir installé `gcc` et `python3`. Vous pouvez vérifier l'existence de ces exécutables avec la commande `which`. 

De plus, afin de pouvoir réussir la première compilation, les exécutables suivants sont nécessaires :
  
  - Doxygen, qui sert à documenter tout le projet. Vous pouvez l'installer avec `sudo apt-get install doxygen`.
  - PyInstaller, qui sert à compiler le code python pour avoir un exécutable. Vous pouvez le récupérer avec `pip3 install pyinstaller`. 

## Installation du projet
Pour installer le projet et l'exécutable, suivez les étapes suivantes :

 - Clonez le projet : 
	 - En **ssh** : `git@github.com:Welzin/huffman-compressor.git`
	 - En **https** : `https://github.com/Welzin/huffman-compressor.git`
 - Entrez dans le répertoire et exécutez `make`. Cette étape compile automatiquement tout ce qui vous manque :
   - Les exécutables `huf` et `dehuf` dans le dossier `build`,
   - La documentation du projet dans `projet_doc`,
   - L'exécutable python `par`.

**S'il y a un problème de compilation, voici les paramètre de la machine où la compilation à réussi :**

  - `uname -a` : **Linux ubuntu 5.4.0-52-generic x86_64**
  - `lsb-release -a` : **Ubuntu 20.04.1 LTS**
  - `gcc -v` : **gcc version 9.3.0**
 
Seule **la librairie standard** est utilisée dans le programme.

## Résumé des commandes

Vous ne voulez pas lire la suite, mais quand même tester le programme ? Cette partie est faite pour vous (dans le cas inverse, vous pouvez directement passer à la section suivante, Fonctionnalités implémentées).

  - **Compression d'un fichier** : `./build/huf source.txt dest.huf`
  - **Décompression d'un fichier** :
    - *Envoi sur stdout* : `./dehuf dest.huf`
    - *Envoi dans un fichier* : `./dehuf dest.huf out.txt`
  - **Archivage d'un dossier** : `./par dossier dossier.par`
  - **Extraction d'une archive** : `./par -x dossier.par`

## Fonctionnalités implémentées

### Compression

Pour compresser un fichier, de nom `source.txt`, dans un autre fichier de nom `dest.huf`, depuis la racine du projet, exécutez la commande suivante :
> `./build/huf source.txt dest.huf`

Il y a trois résultats possibles :

  - **Succès**, la compression c'est bien déroulée. Il existe un nouveau fichier : `dest.huf` qui est **moins lourd** que `fichier.txt`. Le taux de compression est affiché à l'écran sur la console.
  - **Échec**. Erreur lors de la compression. **L'erreur spécifique sera affichée à l'écran**.

  - **Problème de taille**. Le fichier compressé **est plus lourd** que le fichier non compressé. Un **avertissement s'affiche** à l'écran et demande à l'utilisateur si, oui ou non, il veut quand même garder le fichier généré. *En ajoutant l'option `--skip` avant les fichiers source / destination, cette étape ne s'affiche pas et le fichier est automatiquement supprimé.*

> Si vous voulez faire le malin et entrer `./build/huf source.txt source.txt`, le programme vous demandera si oui ou non vous voulez vraiment écraser le fichier !

De base, beaucoup d'informations sont affichées dans la console. Cependant, si vous ne souhaitez pas avoir ces informations, vous pouvez ajouter l'option `--silent`.

> Vous pouvez utiliser les options `--skip` et `--silent` en même temps ! Veillez cependant à ce qu'elles soient toutes deux **avant** les fichiers sources / destination.

### Décompression

Pour décompresser notre fichier `dest.huf`, il y a deux façons de faire :

- `./build/dehuf dest.huf`, qui affichera le fichier décompressé sur la sortie standard
- `./dehuf dest.huf out.txt`, un petit script utilitaire *bash* qui automatise la création d'un nouveau fichier `out.txt` avec pour contenu les données décompressées.

> Pour utiliser le script *bash*, n'oubliez pas d'exécuter `chmod +x dehuf` avant d'exécuter la commande.

### Archivage

Pour archiver un dossier `foo`, dans un archive `foo.par`, exécutez la commande suivante :

> `./par foo foo.par`

Il y a plusieurs résultats possibles :

  - **Succès**, l'archivage a réussi, les données ont été sauvegardées et compressées.
  - **Erreur**, qui peut avoir deux sources :
    - **Permission** : l'utilisateur n'a pas les permissions de lecture sur un fichier qu'il veut compresser. De ce fait, le programme ne peut pas lire ce fichier non plus, et l'archivage ne peut pas continuer. 
    - **Fichier non trouvé** : comme son nom l'indique, le fichier n'a pas été trouvé. Il peut s'agir d'un fichier phantôme sur votre disque.

Si l'archivage est un succès, vous pouvez toujours accéder à vos données. Il suffit d'extraire l'archive avec la commande suivante :

> `./par -x foo.par`

Dans ce cas, un dossier `foo_1` va être créé (je rappelle que le dossier `foo` est celui que vous avez compressé, il vaut mieux ne pas l'écraser) avec le dossier `foo` et tout son contenu à l'intérieur. Si l'extraction ne marche pas, c'est que le fichier donné n'est pas une archive compressée par notre algorithme.

### Documentation

La documentation se génère automatiquement la première fois que vous exécutez `make`. Cependant, si vous voulez mettre à jour la documentation, il suffit d'exécuter la commande suivante : `make doc`. La page d'accueil de la documentation se trouve ensuite au chemin suivant :

> projet_doc/html/index.html

## Complexité

On pose les deux variables suivantes :

- n = taille(fichier)
- BUFFER = 256, car on considère qu'on travaille sur des caractères codés sur 8 bits.

Dans le pire des cas, cet algorithme de compression a les complexités suivantes :

- Calcul de la fréquence : O(n) + O( BUFFER^2 )
- Création de l'arbre : O(2 * ( BUFFER^2 ))
- Encodage : O(BUFFER)
- Compression : O(n / moyenne(encodage))

## Ce qui peut être amélioré

Il y a deux optimisations évidentes qui peuvent être faite :

  - Au niveau du tableau de fréquences, l'enregistrement dans le fichier compressé peut être lourd. En effet, il est possible qu'il y ait 256 caractères avec plusieurs milliers d'occurrences, ce qui fait vite grossir l'en-tête du fichier compressé. Une amélioration peut-être réalisée en prenant plutôt un pourcentage des occurrences, et non le nombre total. Cette manière de faire est moins précise, il faut trouver un bon moyen de représenter les chiffres après la virgule pour être le plus précis possible sans prendre trop d'octets pour coder ces valeurs.
  - Le programme d'archivage et d'extraction en python est un programme *d'apprentissage*. Il ne doit pas être utilisé en réalité, pour une bonne raison : lors de l'extraction, le contenu de l'archive (non compressé) est stockée en mémoire. J'ai cherché à faire un générateur, pour justement ne pas garder ça en mémoire, mais je n'ai pas trouvé comment faire. C'est un point d'optimisation important.