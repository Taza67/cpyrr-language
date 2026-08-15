<a id="readme-top"></a>

[![Contributors][contributors-shield]][contributors-url]
[![Issues][issues-shield]][issues-url]
[![License][license-shield]][license-url]
[![C][c-shield]][c-url]

<div align="center">

<h3 align="center">cpyrr-language</h3>

  <p align="center">
    Compilateur et machine virtuelle pour le langage CPYRR, écrits en C. Analyse lexicale et syntaxique avec Lex et Yacc, puis exécution d'un programme CPYRR dans une machine virtuelle à pile.
    <br />
    <br />
    <a href="https://github.com/Taza67/cpyrr-language/issues/new?labels=bug">Report Bug</a>
    &middot;
    <a href="https://github.com/Taza67/cpyrr-language/issues/new?labels=enhancement">Request Feature</a>
  </p>
</div>

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>

## About The Project

`cpyrr-language` est un compilateur et une machine virtuelle pour le langage **CPYRR**. Le programme `cpyrr-cvm` analyse un programme source CPYRR — grâce à un analyseur lexical (`flex`) et une grammaire (`bison`) — construit les tables de compilation (table lexicographique, table des déclarations, table de représentation des types, table des régions), puis exécute le programme dans une machine virtuelle à pile.

Le compilateur gère les types de base, les structures, les tableaux, les fonctions et procédures, les instructions conditionnelles et les boucles, ainsi que les opérations de lecture et d'écriture.

### Built With

* [C](https://fr.wikipedia.org/wiki/C_(langage)) C89
* [Make](https://fr.wikipedia.org/wiki/Make)
* [Flex](https://fr.wikipedia.org/wiki/Flex_(analyseur_lexical))
* [Bison](https://fr.wikipedia.org/wiki/GNU_bison)

<p align="right"><a href="#readme-top" title="Retour en haut">↑</a></p>

## Getting Started

### Prerequisites

* `gcc`
* `make`
* `flex`
* `bison`

### Installation

```bash
git clone https://github.com/Taza67/cpyrr-language.git
cd cpyrr-language
make build
```

<p align="right"><a href="#readme-top" title="Retour en haut">↑</a></p>

## Usage

Afficher l'aide :

```sh
./bin/cpyrr-cvm -h
```

Compiler un programme CPYRR dans un fichier de sortie (ou un fichier par défaut) :

```sh
./bin/cpyrr-cvm -c programme.cpy
./bin/cpyrr-cvm -c programme.cpy sortie.txt
```

La compilation affiche les tables construites, puis exécute le programme dans la machine virtuelle et affiche la pile d'exécution.

<p align="right"><a href="#readme-top" title="Retour en haut">↑</a></p>

## Contributing

Voir [CONTRIBUTING.md](CONTRIBUTING.md). Veuillez lire [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) avant de participer.

<p align="right"><a href="#readme-top" title="Retour en haut">↑</a></p>

## License

Distribué sous la licence MIT. Voir [LICENSE](LICENSE) pour plus d'informations.

<p align="right"><a href="#readme-top" title="Retour en haut">↑</a></p>

## Contact

Taza67 - [tazaakil67@gmail.com](mailto:tazaakil67@gmail.com)

Lien du projet : [https://github.com/Taza67/cpyrr-language](https://github.com/Taza67/cpyrr-language)

<p align="right"><a href="#readme-top" title="Retour en haut">↑</a></p>

<!-- MARKDOWN LINKS & IMAGES -->
[contributors-shield]: https://img.shields.io/github/contributors/Taza67/cpyrr-language.svg
[contributors-url]: https://github.com/Taza67/cpyrr-language/graphs/contributors
[issues-shield]: https://img.shields.io/github/issues/Taza67/cpyrr-language.svg
[issues-url]: https://github.com/Taza67/cpyrr-language/issues
[license-shield]: https://img.shields.io/badge/License-MIT-blue.svg
[license-url]: https://github.com/Taza67/cpyrr-language/blob/main/LICENSE
[c-shield]: https://img.shields.io/badge/C89-00599C.svg?logo=c&logoColor=white
[c-url]: https://fr.wikipedia.org/wiki/C_(langage)
