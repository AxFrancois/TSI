# Projet de jeu video : TSI

ATTENTION SI IL MANQUE LES LIBRAIRIES GLUT ET GLEW, COPIER COLLER LE DOSSIER EXTERN DES TUTORIAUX DANS LE DOSSIER PRINCIPAL !

## Compilation

Pour compiler et executer à partir du CMakeLists.txt en ligne de commande (Linux et macOs)

```sh
mkdir build
cd build
cmake ..
make
cd ..
./build/projet
```

ou 

```sh
mkdir build
cmake . -B build
make -C ./build && ./build/projet
```

**Testé sur windows avec Visual Studio Community 2019 (MSVC2019)**

**Note sur l'utilisation des IDE (QtCreator, etc)**

Le repertoire d'execution doit être tp_tsi.
C'est a dire que le repertoire data/ doit être accessible.         

Image à usage libre : https://www.pexels.com/photo/boston-terrier-wearing-unicorn-pet-costume-1564506/