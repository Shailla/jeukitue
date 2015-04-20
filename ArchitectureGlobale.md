# Introduction #

Description des composants logiciels qui composent l'application et de leurs rôles et interactions.


# Details #

L'application se compose de 2 composants : le Moteur et le Centralisateur.

Le Moteur est le client de jeu sur lequel le joueur joue, c'est un client lourd écrit en C++ (OpenGL, SDL, FMOD, Agar, ...).

Le Centralisateur est un seuveur d'application J2EE (Spring, Spring MVC, Hibernate, MySQL) accessible sur internet.

Le Centralisateur est accessible par 2 façons :
> - le Moteur peut s'y connecter en UDP/TCP, voir ci-dessous
> - comme un site web

Le Moteur peur se connecter sur le Centralisateur (UDP/TCP) pour :
> - connaître les parties en cours et s'y connecter
> - chatter avec d'autres joueurs
> - télécharger de nouvelles Map
> - tenir à jour les scores de jeu du joueur (consultation ou upload du rapport d'une partie)
> - optenir des mises à jour

La connection site web du Centralisateur permet :
**l'uitlisateur peut:**
> - s'inscrire
> - chatter avec d'autres joueurs
> - suivre les parties en cours
> - consulter les scores des joueurs
> - télécharger des mises à jour ou des Map
**l'administrateur peut :**
> - administrer/monitorer les connections UDP/TCP
> - administrer les news et les fichiers téléchargeables (mises à jour, Map)
> - administrer la liste des utilisateurs