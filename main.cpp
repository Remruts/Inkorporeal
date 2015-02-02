#include "engine.h"
#include <iostream>

int main(){
	
	Engine *eng = NULL;
	eng = new Engine(1366, 768, true);
	
	eng->run();
	
	if (eng != NULL)
		delete eng;
	
	return 0;

}
