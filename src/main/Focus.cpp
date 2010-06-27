

#include "SDL.h"

#include "util/Trace.h"

#include "Focus.h"

namespace JKT_PACKAGE_MENU
{

CFocus::CFocus(	void (*play)(SDL_Event *event),
				void (*menu)(SDL_Event *event),
				void (*menuAgar)(SDL_Event *event) )
{
TRACE().p( TRACE_MENU, "CFocus::CFocus(play=%x,menu=%x)%T", play, menu, this );
	play_handle_key_down = play;	// Impl�mente le focus d�di� � une partie en cours
	menu_handle_key_down = menu;	// Impl�mente le focus d�di� au menu
	menu_agar_handle_key_down = menuAgar;	// Impl�mente le focus d�di� au menu

	focus_actif_handle_key_down = menu;	// Choix du focus actif par d�faut
}

void CFocus::SetPlayFocus()	// Choisi la fonction focus d�di�e � une partie en cours
{
TRACE().p( TRACE_MENU, "CFocus::SetPlayFocus()%T", this );
	if( focus_actif_handle_key_down != play_handle_key_down )
	{
		SDL_WarpMouse( 250, 250 );			//positionnement du curseur de la souris
		SDL_ShowCursor( SDL_DISABLE );				//Cache le curseur de la souris
		SDL_WM_GrabInput( SDL_GRAB_ON );	//Active le mode d�placement relatif de la souris

		SDL_EnableKeyRepeat( 500, 500 );	// R�p�tition des touches clavier

		focus_actif_handle_key_down = play_handle_key_down;
	}
}

void CFocus::SetMenuFocus()	// Choisi la fonction focus d�di�e au menu
{
TRACE().p( TRACE_MENU, "CFocus::SetMenuFocus()%T", this );
	if( focus_actif_handle_key_down != menu_handle_key_down )
	{
		SDL_WarpMouse( 250, 250 );			//positionnement du curseur de la souris
		SDL_ShowCursor( SDL_ENABLE );		//Cache le curseur de la souris
		SDL_WM_GrabInput( SDL_GRAB_ON );	//Active le mode d�placement relatif de la souris

		SDL_EnableKeyRepeat( 500, 500 );	// R�p�tition des touches clavier

		focus_actif_handle_key_down = menu_handle_key_down;
	}
}

void CFocus::SetMenuAgarFocus()	// Choisi la fonction focus d�di�e au menu
{
TRACE().p( TRACE_MENU, "CFocus::SetMenuFocus()%T", this );
	if( focus_actif_handle_key_down != menu_handle_key_down )
	{
		SDL_WarpMouse( 250, 250 );			//positionnement du curseur de la souris
		SDL_ShowCursor( SDL_ENABLE );		//Cache le curseur de la souris
		SDL_WM_GrabInput( SDL_GRAB_ON );	//Active le mode d�placement relatif de la souris

		SDL_EnableKeyRepeat( 500, 500 );	// R�p�tition des touches clavier

		focus_actif_handle_key_down = menu_agar_handle_key_down;
	}
}

void CFocus::SwitchPlayOrConsoleFocus()
{
TRACE().p( TRACE_MENU, "CFocus::ChangePlayOrConsoleFocus()%T", this );
	if( focus_actif_handle_key_down == play_handle_key_down ) {
		SetMenuAgarFocus();	
	}
	else if(focus_actif_handle_key_down == menu_agar_handle_key_down) {
		SetPlayFocus();
	}
}

void CFocus::ExecFocus( SDL_Event *event )	// Ex�cute la fonction de focus pour l'�v�nement 'event'
{
	focus_actif_handle_key_down( event );
}

}	// JKT_PACKAGE_MENU
