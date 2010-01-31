#ifndef __JKT__CONFIGURATIONVIDEOVIEW_H
#define __JKT__CONFIGURATIONVIDEOVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class ConfigurationVideoView : public View
{
	int _fullScreen;		// >0 sur mode plein �cran, =0 sinon
	AG_Combo* _resoCombo;	// Combo pr�sentant les r�solutions vid�o disponibles
public:
	ConfigurationVideoView(const AG_EventFn controllerCallback);
	~ConfigurationVideoView(void);

	void show(void);
};

#endif

