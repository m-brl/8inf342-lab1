# Labo 1

Ce projet vise à recréer une boucle REPL basique avec forking.

## Build

### Via script

Il est possible d'utiliser les scripts de build:
- debug.sh: génère un fichier executable de debug
- release.sh: génère un fichier executable de production
- clean.sh: supprimer les dossiers de build

Tout les executables sont générés dans leur dossier de build.


### A la main

Créer un dossier de build:

```
mkdir build && cd build
```

Générer les fichiers cmake:

```
cmake ..
```

Build le projet

```
cmake --build .
```
