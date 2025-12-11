[![GitHub build](https://github.com/LanguageMachines/foliatest/actions/workflows/foliatest.yml/badge.svg?branch=master)](https://github.com/LanguageMachines/foliatest/actions/)
[![Language Machines Badge](http://applejack.science.ru.nl/lamabadge.php/foliatest)](http://applejack.science.ru.nl/languagemachines/)

# Foliatest

  (c) CLST/TiCC/CLiPS 2026
  https://github.com/LanguageMachines/foliatest

    Centre for Language and Speech Technology, Radboud University Nijmegen
    Tilburg centre for Cognition and Communication, Tilburg University and
    Centre for Dutch Language and Speech, University of Antwerp

  This package provides series of tests for libfolia.

  foliatest is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Comments and bug-reports are welcome at our issue tracker at
  https://github.com/LanguageMachines/timbl/issues or by mailing
  lamasoftware (at) science.ru.nl.

-----------------------------------------------------------------------

This software has been tested on:
- Intel platforms running several versions of Linux, including Ubuntu, Debian,
  Arch Linux, Fedora (both 32 and 64 bits)
- MAC platform running OS X 10.10


Contents of this distribution:
- Sources
- Licensing information ( COPYING )
- Build system based on GNU Autotools

Dependencies:
To be able to succesfully build libfolia from source, you need the following dependencies:
- ticcutils (https://github.com/LanguageMachines/ticcutils)
- libfolia (https://github.com/LanguageMachines/libfolia)
- libicu-dev
- libxml2-dev
- A sane C++ build environment with autoconf, automake, autoconf-archive, make, gcc or clang, libtool, pkg-config

To install ticcutils, libfolia and ucto, first consult whether your
distribution's package manager has an up-to-date package.
If not, for easy installation of libfolia and all dependencies, it is included
as part of our software distribution LaMachine:
https://proycon.github.io/LaMachine .

To compile and install manually from source, provided you have all the
dependencies installed:

 - $ bash bootstrap.sh
 - $ ./configure
 - $ make
 - $ make install
