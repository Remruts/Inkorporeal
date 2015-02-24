#include "control.h"

control::control(){
	esc = false;
	evLeft = false;
	evRight = false;
	evJump = false;
	evShoot = false;
	evDash = false;
	evMelee = false;
	evStart = false;
	evDown = false;
	anyKey = false;
	esc = false;
}

control::~control(){
	
}
	
void control::update(SDL_Event* ev){
	if ((*ev).type == SDL_KEYDOWN){
		
		anyKey = true;
		
		switch( ev->key.keysym.sym ){
		case SDLK_ESCAPE:
			esc = true;
		break;
		
		//jumping
		case SDLK_UP:
			evJump = true;
		break;
		case SDLK_w:
			evJump = true;
		break;
		case SDLK_SPACE:
			evJump = true;
		break;
		
		//dashing
		case SDLK_x:
			evDash = true;
		break;
		case SDLK_k:
			evDash = true;
		break;
		
		//moving left
		case SDLK_a:
			evLeft = true;
		break;
		case SDLK_LEFT:
			evLeft = true;
		break;
		
		//moving right
		case SDLK_d:
			evRight = true;
		break;
		case SDLK_RIGHT:
			evRight = true;
		break;
		
		//shooting
		case SDLK_c:
			evShoot = true;
		break;
		case SDLK_j:
			evShoot = true;
		break;
		
		//melee
		case SDLK_z:
			evMelee = true;
		break;
		case SDLK_l:
			evMelee = true;
		break;
		
		case SDLK_DOWN:
			evDown = true;
		break;
		case SDLK_s:
			evDown = true;
		break;
		
		case SDLK_RETURN:
			evStart = true;
		break;
		
		default:
		break;
		}
	} else if ((*ev).type == SDL_KEYUP){
		
		anyKey = false;
		
		switch( ev->key.keysym.sym ){
		case SDLK_ESCAPE:
			esc = false;
		break;
		
		//jumping
		case SDLK_UP:
			evJump = false;
		break;
		case SDLK_w:
			evJump = false;
		break;
		case SDLK_SPACE:
			evJump = false;
		break;
		
		//dashing
		case SDLK_x:
			evDash = false;
		break;
		case SDLK_k:
			evDash = false;
		break;
		
		//moving left
		case SDLK_a:
			evLeft = false;
		break;
		case SDLK_LEFT:
			evLeft = false;
		break;
		
		//moving right
		case SDLK_d:
			evRight = false;
		break;
		case SDLK_RIGHT:
			evRight = false;
		break;
		
		//shooting
		case SDLK_c:
			evShoot = false;
		break;
		case SDLK_j:
			evShoot = false;
		break;
		
		//melee
		case SDLK_z:
			evMelee = false;
		break;
		case SDLK_l:
			evMelee = false;
		break;
		
		case SDLK_DOWN:
			evDown = false;
		break;
		case SDLK_s:
			evDown = false;
		break;
		
		case SDLK_RETURN:
			evStart = false;
		break;
		
		default:
		break;
		}
	}
}
