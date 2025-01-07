# Client TFTP : `tp2.c`

Ce dépôt contient une implémentation d'un client TFTP, permettant de télécharger des fichiers depuis un serveur TFTP via le protocole UDP. Ce client est basé sur le fichier source `tp2.c`.

## Fonctionnalités

- Récupération de l'adresse du serveur et du nom du fichier à télécharger via les arguments de ligne de commande.
- Résolution de l'adresse du serveur avec `getaddrinfo`.
- Construction et envoi de requêtes de lecture (RRQ) conformes à [RFC1350](https://www.rfc-editor.org/rfc/rfc1350).
- Gestion des transferts de fichiers, qu'ils soient constitués d'un seul ou de plusieurs paquets.
- Envoi automatique d'acquittements (ACK) pour chaque paquet reçu.

## Prérequis

- Un serveur TFTP doit être en cours d'exécution sur `127.0.0.1` au port `1069`.
- Le fichier à télécharger doit être disponible sur le serveur sous le nom `ones512`.
- Utilisez le script `./go.sh` si nécessaire pour préparer l'environnement (par exemple, décompresser des fichiers).

## Utilisation

### Compilation

```bash
gcc tp2.c -o tp2
```

### Exécution

```bash
./tp2 <hôte> <fichier>
```

- `<hôte>` : Adresse du serveur (par exemple : `127.0.0.1`).
- `<fichier>` : Nom du fichier à télécharger (par exemple : `ones512`).

Exemple :

```bash
./tp2 127.0.0.1 ones512
```

## Détails d'implémentation

- **Analyse des arguments** : Le programme récupère les arguments de ligne de commande pour obtenir l'adresse du serveur (`argv[1]`) et le nom du fichier (`argv[3]`).
- **Résolution d'adresse avec `getaddrinfo`** : La fonction `getaddrinfo` permet de convertir l'adresse du serveur en une structure utilisable pour les communications UDP.
- **Construction des requêtes RRQ** : Une requête de lecture conforme au protocole TFTP est construite avec les informations du fichier et du mode de transfert (`octet`).
- **Réception et traitement des paquets** :
  - Les paquets de données (DATA) sont reçus avec `recvfrom`, puis traités pour séparer les en-têtes et le contenu.
  - Les acquittements (ACK) sont envoyés après chaque réception de paquet.
- **Gestion des fichiers volumineux** : Le programme gère les fichiers divisés en plusieurs paquets, en utilisant une boucle pour traiter les données jusqu'à la réception complète.

## Difficultés rencontrées

1. **Gestion des paquets multiples** : 
   - Une attention particulière a été nécessaire pour gérer les transmissions de fichiers de grande taille, où plusieurs paquets sont nécessaires.
   - Par exemple, pour un fichier `ones512`, un second paquet vide est parfois envoyé pour signaler la fin du transfert.

2. **Débogage réseau avec Wireshark** :
   - L'utilisation de Wireshark pour analyser les paquets TFTP a mis en évidence des changements de port côté serveur pendant la communication, nécessitant une gestion correcte des adresses et ports dans le code.

3. **Conformité au protocole TFTP** :
   - Respecter le format précis des en-têtes TFTP (codes opération, numéro de bloc, etc.) a exigé une lecture attentive de la RFC1350.

## Tests effectués

- **Fichier `ones512`** :
  - Transfert réussi en deux étapes :
    - Premier paquet contenant les données.
    - Second paquet contenant uniquement un en-tête, signalant la fin du transfert.
- **Fichier de petite taille** :
  - Transfert réussi en un seul paquet, avec un acquittement correct.

## Conclusion

Le fichier `tp2.c` implémente un client TFTP fonctionnel et conforme au protocole. Ce projet met en lumière les défis liés aux transmissions basées sur UDP et le respect des spécifications des protocoles réseau.
