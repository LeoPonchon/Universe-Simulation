# Universe Simulation

Petite simulation 3D **OpenGL / FreeGLUT** écrite en C++ pour expérimenter le rendu, les transformations, une orbite simple et une caméra libre.

La scène actuelle représente le **Soleil** et la **Terre** avec des dimensions mises à l'échelle, une orbite terrestre animée et des labels.

> Le nom du dépôt décrit l'objectif d'apprentissage ; l'implémentation actuelle se concentre sur le système Soleil–Terre, pas sur une simulation physique complète de l'univers.

## Fonctionnalités

- rendu 3D OpenGL ;
- Soleil et Terre dessinés comme sphères ;
- orbite terrestre ;
- animation de la position de la Terre ;
- valeurs de rayon/distance mises à l'échelle ;
- caméra libre ;
- rotation caméra à la souris ;
- affichage de labels.

## Contrôles

- `Z` / `S` : avancer / reculer
- `Q` / `D` : déplacement latéral
- `Espace` : monter
- `Shift gauche` : descendre
- clic droit + mouvement souris : orienter la caméra

## Stack

- C++
- OpenGL
- GLU
- FreeGLUT
- CMake 3.10+
- Windows

Le projet utilise actuellement `windows.h` et `GetAsyncKeyState`, donc le code est orienté Windows.

## Build

FreeGLUT est fourni dans le dépôt et le `CMakeLists.txt` configure ses dossiers `include`, `lib/x64` et `bin/x64`.

Depuis le dossier de build adapté à votre installation :

```bash
cmake -S src -B build
cmake --build build --config Release
```

Le build copie automatiquement `freeglut.dll` à côté de l'exécutable.

## Limites

La simulation n'est pas un modèle astrophysique à pas de temps réaliste : elle utilise des constantes mises à l'échelle et une animation simplifiée à but visuel/pédagogique.
