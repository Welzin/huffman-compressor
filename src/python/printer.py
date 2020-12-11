from enum import Enum

class PrintableType(Enum) :
    """!Enumération des types affichables.

    Pour un affichage sympatique, définition des différents types d'affichages
    possibles. Chaque type a une couleur définie et s'affiche avant la chaîne
    envoyée.
    """
    Success = 0     #!< Succès, affiché en vert
    Info = 1        #!< Infos, affiché en bleu clair
    Warning = 2     #!< Avertissement, affiché en jaune
    Error = 3       #!< Erreur, affiché en rouge
    Debug = 4       #!< Debug, affiché en bleu foncé

def print_aux(string, type) :
    """!Affiche `type`, puis `string`.

    Définie une couleur pour chaque type affichable :
        - Vert pour succès
        - Bleu clair pour info
        - Jaune pour avertissement
        - Rouge pour erreur
        - Bleu foncé pour debug

    @param string La chaîne a afficher.
    @param type Le type d'affichage à avoir.
    """
    types = {
        PrintableType.Success: "[\033[1;32mSuccès\033[0m]",
        PrintableType.Info: "[\033[1;36mInfo\033[0m]",
        PrintableType.Warning: "[\033[1;33mAvertissement\033[0m]",
        PrintableType.Error: "[\033[1;31mErreur\033[0m]",
        PrintableType.Debug: "[\033[1;34mDebug\033[0m]",
    }

    print("{} {}".format(types[type], string))
 
def print_success(string) :
    """!Affiche la chaîne donnée avec [Succès] devant.

    @param string Chaîne à afficher.
    """
    print_aux(string, PrintableType.Success)

def print_info(string) :
    """!Affiche la chaîne donnée avec [Info] devant.

    @param string Chaîne à afficher.
    """
    print_aux(string, PrintableType.Info)

def print_error(string) :
    """!Affiche la chaîne donnée avec [Erreur] devant.

    @param string Chaîne à afficher.
    """
    print_aux(string, PrintableType.Error)

def print_warning(string) :
    """!Affiche la chaîne donnée avec [Avertissement] devant.

    @param string Chaîne à afficher.
    """
    print_aux(string, PrintableType.Warning)

def print_debug(string) :
    """!Affiche la chaîne donnée avec [Debug] devant.

    @param string Chaîne à afficher.
    """
    print_aux(string, PrintableType.Debug)
