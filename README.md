# **Projet Enseash - Création d'un Micro-Shell**

*Ce projet a pour objectif de développer un micro-shell, appelé **enseash**, capable d'afficher les codes de sortie et les temps d'exécution des programmes lancés. Il se déroule sur deux séances et comporte plusieurs étapes.*

---

## **Question 1 : Affichage du Message d’Accueil et du Prompt**

Dans cette première étape, l’objectif est d’afficher un **message d’accueil** suivi d’un **prompt**, qui indique que le terminal est prêt à recevoir une commande.

---

## **Fonctions utilisées**

Pour réaliser cette étape, nous utilisons les fonctions suivantes de la librairie `unistd.h` :

- **`write(int fd, const void *buf, size_t buf_size)`**  
  Permet d'écrire des données dans un fichier ou dans le terminal.

- **`read(int fd, const void *buf, size_t buf_size)`**  
  Permet de lire des données saisies au clavier ou depuis un fichier.


### **Paramètres des fonctions**

1. **`fd`** : le descripteur de fichier, qui définit la source ou la destination des données. Les descripteurs courants sont :  
   - `STDIN_FILENO (0)` : lecture depuis le clavier.  
   - `STDOUT_FILENO (1)` : écriture dans le terminal.  
   - `STDERR_FILENO (2)` : écriture des messages d'erreur.  

2. **`buf`** : un pointeur vers une zone mémoire contenant :  
   - Les données à écrire (pour `write`).  
   - Une zone mémoire vide pour stocker les données lues (pour `read`).  

3. **`buf_size`** : la taille maximale (en octets) de la zone mémoire pointée par `buf`.


## **Utilisation dans le Projet**

- La fonction `write` est utilisée pour afficher le message d’accueil. Ici, la taille de `buf` correspond exactement à la longueur du texte à afficher.
- La fonction `read` est utilisée pour lire les commandes utilisateur saisies au clavier. Le tampon de stockage doit être suffisamment grand pour contenir ces commandes.
- 
## **Fonction Supplémentaire : strlen**

Nous utilisons également la fonction `strlen` de la librairie `string.h` pour obtenir la longueur (de type `int`) d’une chaîne de caractères. Cela est particulièrement utile pour déterminer la taille de `buf` avant d’écrire les données.


---

# **Question 2 : Lecture et Exécution des Commandes**

Dans cette étape, l'objectif est de lire une commande saisie dans le terminal, l'exécuter et revenir au prompt sans quitter le shell.  

## **Fonctionnement et Solutions**
1. **Lecture avec `read` :**
   if ((commande_size = read(0, buf, BUFSIZE)) == -1) {
    perror("read");
    exit(EXIT_FAILURE);
}
   La fonction `read` lit la commande saisie et la stocke dans un tableau `CommandLine`. Le caractère `\n` (fin de ligne) est remplacé par `\0` pour indiquer la fin de la chaîne.  

2. **Exécution avec `execvp` :**
   pid = fork();
if (pid == 0) {
    execvp(buf[0], buf);
    exit(EXIT_SUCCESS);
}

   La commande lue est exécutée avec `execvp`. Les arguments incluent le nom de la commande et ses options, suivis d'un `NULL`.  
    **Problème** : `execvp` interrompt le processus en cours, ce qui fait quitter le shell.  

3. **Solution avec `Retour au prompt et attente de la commande suivante` :**  
  La boucle while(1) maintient le cycle lecture → exécution → affichage.

# **Question 3 : Gestion de la sortie du shell**
Code associé :
if (strcmp("exit", buf[0]) == 0 || commande_size == 0) {
    write(1, "Bye bye...\n", strlen("Bye bye...\n"));
    exit(EXIT_FAILURE);
}
**Explication** :

Si l'utilisateur tape exit, ou si un <Ctrl+D> (EOF) est détecté, le programme affiche "Bye bye..." et termine.
Cela satisfait les conditions de sortie demandées.

---

