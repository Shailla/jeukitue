# Introduction #

Indique comment installer l'environnement de développement pour le Moteur écrit en C++.

# Installation #
  * Installer MinGW :
    * Télécharger et exécuter l'installeur "mingw-get-inst-XXX.exe" ici http://sourceforge.net/projects/mingw/files/MinGW/
    * Laisser les options par défaut (l'installation dans C:/MinGW est importante sinon il y aura des problèmes de configuration avec CMake) sauf :
      * Cocher Download latest repository catalogues
      * Choisir l'installation des éléments suivants : C Compiler, C++ Compiler, MSYS Basic System et MinGW Developper Toolkit
    * L'installeur va télécharger les packages
    * Une fois l'installation terminée, mettre C:/MinGW/bin dans le PATH
  * Installer Eclipse CDT
  * Installer CMake disponible ici http://www.cmake.org/cmake/resources/software.html, laisser les options d'installation par défaut
  * Lancer CMake et saisir les informations :
    * Where is the source code = D:/JC/jkt/svn/trunk/moteur/sources
    * Where to build the binaries = D:/JC/jkt/svn/trunk/moteur/project/EclipseCDT4-MinGW-Makefiles
    * Cliquer sur "Configure" et choisir le générateur "Eclipse CDT - MinGW Makefiles" en laissant "Use default native compilers"
    * En cas d'erreur de CMake indiquant que la librairie "libgmp-10.dll" est manquant assurez-vous d'avoir bien mis "C:/MinGW/bin" dans le PATH
    * CMake demande alors des informations, saisir celles-ci :
      * CMAKE\_BUILD\_TYPE = Debug
      * CMAKE\_ECLIPSE\_EXECUTABLE = Chercher "eclipse.exe" dans votre système de fichiers
      * CMAKE\_ECLIPSE\_VERSION = 3.7 (Indigo), ou la vôtre
      * CMAKE\_INSTALL\_PREFIX = D:/JC/jkt/svn/trunk/moteur/deploy
    * Re-cliquer sur "Configure", plus rien ne devrait apparaître en rouge
    * Cliquer alors sur "Generate"
  * Ouvrir Eclipse
  * Dans le project explorer, choisir "Import..."/"Existing Projects into Workspace"
  * Choisir la root directory "D:\JC\jkt\svn\trunk\moteur\project\EclipseCDT4-MinGW-Makefiles"
  * Cliquer sur "Finish"
  * Compiler le projet ("Build project" en clic-droit sur le projet)
  * Clic-droit sur le projet, puis "Run As.../"Run Configurations..." puis :
    * Clic-droit "New" sur "C/C++ Application"
    * Dans "Project", sélectionner le projet
    * Dans "C/C++ Application" mettre "D:\JC\jkt\svn\trunk\moteur\deploy\Debug\jkt.exe"
    * Dans l'onglet "Arguments", mettre "D:\JC\jkt\svn\trunk\moteur\deploy" dans "Working directory"
    * Et enfin "Run" et enjoy...