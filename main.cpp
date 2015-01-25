#include "engine.h"
#include <iostream>

int main(){
	
	Engine *eng = new Engine(1366, 768, true);
	
	eng->run();
	
	return 0;

}
