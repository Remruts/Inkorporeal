#ifndef _CONTROL_
#define _CONTROL_

#include <SDL2/SDL.h>

// Esto es muy trucho, pero me aburrí y no quería copiar el código de Nahuel. 
// No hacer estas cosas en casa. Copiar código de Nahuel

class control{
public:
	
	control();
	~control();
	
	void update(SDL_Event*);

	bool evLeft;
	bool evRight;
	bool evJump;
	bool evShoot;
	bool evDash;
	bool evMelee;
	bool esc;

private:
	
};

#endif
