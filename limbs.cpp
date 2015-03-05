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
	flip = 0;
	
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
void limb::setFlip(int f) {
	if (f < 0){
		f = 0;
	} else if (f > 3){
		f = f%4;
	}
	
	flip = f;
}

//Si debe dibjarse en el "espejo"
void limb::mirror(bool m){
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
		dstX = ((miembroActual->x - miembroActual->pivotX)-pivX);
		dstY = ((miembroActual->y - miembroActual->pivotY)-pivY);
		
		if (mirroring){
			dstX = dstX*0.7143+sin(parentAngle/180.0*3.1415)*0.2857;
			dstY = dstY*0.7143+sin(parentAngle/180.0*3.1415)*0.2857;
		}
				
		magnitud = sqrt(dstX*dstX+dstY*dstY);
		
		//Luego calculo la posición real
		X += cos(parentAngle/180.0*3.1415)*magnitud;
		Y += sin(parentAngle/180.0*3.1415)*magnitud;
		
		miembroActual = nuevoPadre;
		nuevoPadre = miembroActual->parent;
	}
	X += miembroActual->x;
	// 432 funciona para los brazos del demonio. Número pseudo-arbitrario.
	// en realidad debería hacerse todo un cálculo loco con el tamaño del sprite y etc.
	Y += (mirroring ? 432+(320-miembroActual->y*0.7143) : miembroActual->y);  
	
	
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
	
	//ángulo invertido
	if (mirroring){
		double radAngle = (res/180.0)*(3.1415);
		double X = cos(radAngle);
		double Y = -sin(radAngle);
		res = 180*(atan2(Y, X)/3.1415);
	} 
		
	return res;
}

//dibujar
void limb::draw(painter* pintor){
	
	int X, Y;
	getWorldPos(X, Y);
	
	pintor->setPivot(pivotX, pivotY);
	
	if (mirroring){
		//dibujar en el "espejo"
		double WorldAng = getWorldAngle()-90;
		pintor->drawEx(spritesheet, sprtX, sprtY, sprtSizeX, sprtSizeY, X, Y, 
			sprtSizeX*0.7143+sin(WorldAng/180.0*3.1415)*0.2857, 
			sprtSizeY*0.7143+cos(WorldAng/180.0*3.1415)*0.2857, WorldAng, !flip);
			// NO VOY A ACHICAR LA ESCALA, porque es horrible
			// Update: Eso hice :/
	} else {
		pintor->drawEx(spritesheet, sprtX, sprtY, sprtSizeX, sprtSizeY, X, Y, 
			sprtSizeX, sprtSizeY, getWorldAngle()-90, flip);
	}
	
	pintor->defaultPivot();
		
}
//FIN LIMBS

//Esqueleto
limbSkeleton::limbSkeleton(){
	
}

limbSkeleton::~limbSkeleton(){
	vector<limb*>::iterator it = limbs.begin();
	while (it != limbs.end()){
		if (*it != NULL){
			delete *it;
			*it = NULL;
			it = limbs.erase(it);
		}else{
			it++;	
		}
		
	}
}
	
limb* limbSkeleton::addLimb(LTexture *sprt, int parent){
	limb* res;
	if ((parent == -1) || (limbs.size() == 0)){
	
		res = new limb(sprt, NULL);
					
	} else {
		
		if (parent > int(limbs.size())-1){
			parent = int(limbs.size())-1;
		} else if (parent < 0){
			parent = 0;
		}
		
		res = new limb(sprt, limbs[parent]);
	}
	
	limbs.push_back(res);
	return res;
}

limb* limbSkeleton::getLimb(int limbNum){
	if (limbNum >= 0 && limbNum < int(limbs.size())){
		return limbs[limbNum];
	} else {
		return NULL;
	}
}

int limbSkeleton::getLimbNum(){
	return limbs.size();
}
	
void limbSkeleton::draw(painter* pintor){
	vector<limb*>::iterator it = limbs.begin();;
	while (it != limbs.end()){
		if (*it != NULL)
			(*it)->draw(pintor);
		it++;
	}
}

//fin esqueleto

//animaciones
limbAnim::limbAnim(limbSkeleton* limbskel, bool repeat){
	skel = limbskel;
	
	currentKeyframe = frames.begin(); //Esto no sirve de nada
	currentFrame = 0;
	startTime = 0;
	loop = repeat;
	
	paused = 0;
}

limbAnim::~limbAnim(){
	
}
	
void limbAnim::addKeyframe(int frame, double angle, int limbNum){
	if (frame < 0){
		frame = 0;
	}
	
	if (limbNum < 0){
		limbNum = 0;
	} else if (limbNum > skel->getLimbNum()-1){
		limbNum = skel->getLimbNum()-1;
	}
	
	(frames[frame])[limbNum] = angle;
	
	reset();
	
}

void limbAnim::setRepeat(bool repeat){
	loop = repeat;
}
	
void limbAnim::play(){
	paused = 0;
}

void limbAnim::pause(){
	paused = 1;
}

void limbAnim::reset(){
	startTime = 0;
	currentFrame = 0;
	currentKeyframe = frames.begin();
	
	for (int i = 0; i < skel->getLimbNum(); ++i){
		startAngles[i] = skel->getLimb(i)->getAngle();
	}
}

bool limbAnim::isFinished(){
	return (!loop && (currentKeyframe == frames.end()) && (currentFrame == currentKeyframe->first));
}

void limbAnim::step(){

	if ((skel != NULL) && (skel->getLimbNum() > 0) && (frames.size() > 0)){
		
		if (currentFrame > currentKeyframe->first){
			if (currentKeyframe != frames.end()){
				startTime = currentKeyframe->first;
				currentKeyframe++;
				
				for (int i = 0; i < skel->getLimbNum(); i++){
					startAngles[i] = skel->getLimb(i)->getAngle();
				}
				
			} else {
				if (loop){
					reset();
				} else {
					currentFrame = currentKeyframe->first;
				}
			}
			
		}
		
		if (currentKeyframe != frames.end()){
			limb* currentLimb;
			double newAngle;
			double proportion;
		
			map<int, double>::iterator it = (currentKeyframe->second).begin();
			while(it != (currentKeyframe->second).end()){
				currentLimb = skel->getLimb(it->first); //limb
				newAngle = it->second; //angle
				if (currentKeyframe->first - startTime > 0){
					proportion = (currentFrame - startTime)/double(currentKeyframe->first - startTime);
				} else {
					proportion = 1;
				}
				
				
				/*
				//debug
				std::cout << "keyframe: " << currentKeyframe->first << std::endl;
				std::cout << "currentLimb: " << (long int)(currentLimb) << std::endl;
				std::cout << "limb: " << it->first << std::endl;
				std::cout << "angle: " << it->second << std::endl;
				std::cout << proportion << std::endl;
				std::cout << std::endl;
				*/
					
				currentLimb->setAngle(lerp(startAngles[it->first], newAngle, proportion));
			
				it++;
			}
		}
		
		//Acá abajo, para soporte de frame 0
		currentFrame += 1;
		
	}	
		
}

//fin animaciones

//LERP
float lerp(float v0, float v1, float t){
  return (1-t)*v0 + t*v1;
}
