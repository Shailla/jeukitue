
#ifndef __JKT__JKT_H
#define __JKT__JKT_H

class CPlayer;
namespace JktMoteur {
	class GraphicObject;
}

int main(int argc, char** argv);
void quit_tutorial();								// Quitte sans code d'erreur
void quit_game( int code );							// Quitte avec code d'erreur
void gravitePlayer(CPlayer *player);				//fonction impl�mentant la gravit�
void initMenu(void);
void keyboard(unsigned char key, int x, int y);		// agit en fonction du clavier
void display(void);									// Affiche l'�cran
unsigned int timer(unsigned int nbr, void *fct);	//focntion qui s'ex�cute p�riodiquement et qui provoque l'affichage
void play_handle_key_down( SDL_Event* event );
void menu_agar_handle_key_down( SDL_Event* event );
Uint32 getPixel(SDL_Surface *surface, int x, int y);

void addGraphicObjectToInitialize(JktMoteur::GraphicObject* graphicObject);
void addGraphicObjectToDestruct(JktMoteur::GraphicObject* graphicObject);

#endif

