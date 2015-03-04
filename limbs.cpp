#include "limbs.h"


//LIMBS

limb::limb(LTexture* sprt, limb* padre){
	
	spritesheet = sprt;
	
	sprtX = 0;
	sprtY = 0;
	
	// default 32
	sprtSizeX = 32;
	sprtSizeY = 32;
	
	x = 0;
	y = 0;
	
	pivotX = 0;
	pivotY = 0;
	
	angle = 0; 
	parent  = padre;
	
	mirroring = 0;
	
	minAngle = 0;
	maxAngle = 360;
}

limb::~limb(){
	
}

//posición relativa
void limb::getPos(int &X, int &Y){
	X = x;
	Y = y;
}
//ángulo relativo en grados
double limb::getAngle(){
	return angle;
}

//ángulo relativo en grados
void limb::getMaxAngles(double &min, double &max){
	min = minAngle;
	max = maxAngle;
}

//obtener pivote
void limb::getPivot(int &X, int &Y){
	X = pivotX;
	Y = pivotY;
}

//setear ángulo relativo;
void limb::setAngle(double ang){
	if (ang < minAngle){
		ang = minAngle;
	} else if (ang >= maxAngle){
		ang = maxAngle;
	}
	angle = ang;
}

//setea los ángulos máximo y mínimo
void limb::setMaxAngles(double min, double max){
	minAngle = min;
	maxAngle = max; 
}

//setear el pivote;
void limb::setPivot(int X, int Y){
	pivotX = X;
	pivotY = Y;
}

//qué sprite va a ser dibujado
void limb::setSprite(int X, int Y, int tamX, int tamY){
	sprtX = X;
	sprtY = Y;
	sprtSizeX = tamX;
	sprtSizeY = tamY;
}

//espejar: 0=no, 1=horizontal, 2=vertical, 3=ambos.
void limb::mirror(int m) {
	if (m<0){
		m = 0;
	} else if (m>3){
		m = m%4;
	}
	
	mirroring = m;
}

//setear posición
void limb::setPos(int X, int Y){
	x = X;
	y = Y;
}

//posición real en el mundo
void limb::getWorldPos(int &X, int &Y){
	X = 0;
	Y = 0;
	
	limb* nuevoPadre = parent;
	limb* miembroActual = this;
	
	int pivX, pivY;
	double parentAngle;
	int dstX, dstY;
	double magnitud;
	
	//Funciona, pero los sprites tienen que estar para abajo y coincidir los pivotes
	while(nuevoPadre != NULL){
		
		
		nuevoPadre->getPivot(pivX, pivY);
		
		//obtengo el ángulo real del padre 
		parentAngle = nuevoPadre->getWorldAngle();
		
		//primero calculo ángulo entre pivote padre y pivote hijo, y magnitud
		dstX = (miembroActual->x - miembroActual->pivotX)-pivX;
		dstY = (miembroActual->y - miembroActual->pivotY)-pivY;
		magnitud = sqrt(dstX*dstX+dstY*dstY);
		
		//Luego calculo la posición real
		X += cos(parentAngle/180.0*3.1415)*magnitud;
		Y += sin(parentAngle/180.0*3.1415)*magnitud;
		
		miembroActual = nuevoPadre;
		nuevoPadre = miembroActual->parent;
	}
	
	X += miembroActual->x;
	Y += miembroActual->y;	
	
}

//ángulo real
double limb::getWorldAngle(){
	limb* nuevoPadre = parent;
	double res = angle;
	while (nuevoPadre != NULL){
		res += nuevoPadre->angle;
		nuevoPadre = nuevoPadre->parent;
	}
	if (res>360){
		res = int(res)%360;
	}
	return res;
}

//dibujar
void limb::draw(painter* pintor){
	limb* miembro = this;
	int X, Y;
	while(miembro != NULL){
		
		miembro->getWorldPos(X, Y);
		
		pintor->setPivot(miembro->pivotX, miembro->pivotY);
		pintor->drawEx(miembro->spritesheet, miembro->sprtX, miembro->sprtY, miembro->sprtSizeX, miembro->sprtSizeY, X, Y, 
			miembro->sprtSizeX, miembro->sprtSizeY, miembro->getWorldAngle()-90, miembro->mirroring);
			
		miembro = miembro->parent;
	}
	pintor->defaultPivot();
}
//FIN LIMBS
