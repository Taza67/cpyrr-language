# Contribuer

Merci de votre intérêt pour **cpyrr-language**. Le projet est un compilateur et une machine virtuelle pour le langage CPYRR, construit avec `make` (norme C89), l'analyse lexicale et la grammaire étant générées par `flex` et `bison`.

## Avant de commencer

- Recherchez dans les [issues existantes](https://github.com/Taza67/cpyrr-language/issues) pour éviter de faire un travail en double.
- Pour les changements importants ou structurels, ouvrez d'abord une issue.

## Environnement de développement

### Prérequis

- `gcc`
- `make`
- `flex`
- `bison`

### Cloner et compiler

```bash
git clone https://github.com/Taza67/cpyrr-language.git
cd cpyrr-language
make build
```

## Pull requests

1. Forkez le dépôt et créez une branche depuis `main`.
2. Faites des changements ciblés ; gardez les PR faciles à relire.
3. Vérifiez que `make build` se compile sans erreur ni avertissement.
4. Ouvrez une pull request avec une description claire et liez les issues concernées.

## Messages de commit

Suivez [Conventional Commits](https://www.conventionalcommits.org/).

- **Types :** `feat`, `fix`, `refactor`, `docs`, `test`, `chore`
- **Description :** à l'impératif, en minuscules, sans point final
- **Corps :** facultatif ; ligne vide après la description, puis des puces `-` — en minuscules sauf noms propres, à l'impératif, sans point final

## Organisation du code

| Fichier | Rôle |
|---------|------|
| `lex_yacc.l` / `lex_yacc.y` | Analyseur lexical et grammaire syntaxique du langage |
| `src/association.c` | Association des déclarations et vérification des types |
| `src/machine_virtuelle.c` | Exécution du programme dans la machine virtuelle |
| `src/table_lexicographique.c` | Table des lexèmes et table de hashage |
| `src/table_declarations.c` | Table des déclarations |
| `src/table_representation.c` | Représentation des types et entêtes de sous-programmes |
| `src/table_regions.c` | Table des régions |
| `src/gestion_arbres.c` | Arbres abstraits de la compilation |
| `src/pile_execution.c` | Pile d'exécution de la machine virtuelle |
| `src/pile.c` | TAD pile |
| `src/gestion_fichiers.c` | Vérification et gestion des fichiers |
| `src/erreur.c` | Messages d'erreur et d'avertissement |
| `src/allocation.c` | Allocation et libération mémoire |

## Code de conduite

Ce projet suit le [Contributor Covenant](CODE_OF_CONDUCT.md). En y participant, vous acceptez de le respecter.