#ifndef __THE_KING_OF_LIMBS__
#define __THE_KING_OF_LIMBS__

#include "LTexture.h"
#include "painter.h"
#include <cmath>
#include <vector>
#include <map>

using std::vector;
using std::map;

// clase miembro
// unidad constituyente de un esqueleto 
// Es importante notar que el sprite tiene que ser dibujado "hacia abajo"
class limb{
public:
	limb(LTexture* sprt, limb* parent);
	~limb();
	
	void getPos(int &X, int &Y); //posición relativa
	double getAngle(); //ángulo relativo en grados
	void getPivot(int &X, int &Y); //obtener pivote
	void getMaxAngles(double &min, double &max); //obtiene los ángulos máximo y mínimo
	
	void setPos(int X, int Y); //setear posición
	void setAngle(double ang); //setear ángulo relativo;
	void setMaxAngles(double min, double max); //setea los ángulos máximo y mínimo. Debería ser (min <= max)
	void setPivot(int X, int Y); //setear el pivote;
	void setSprite(int X, int Y, int tamX, int tamY); //qué sprite va a ser dibujado
	void setFlip(int f); // espejar: 0=no, 1=horizontal, 2=vertical, 3=ambos.
	void mirror(bool m); // Si debe dibujarse para abajo [en el "espejo"]
	
	void getWorldPos(int &X, int &Y); //posición real en el mundo
	double getWorldAngle();	//ángulo real
	
	void draw(painter* pintor); //dibujar
		
private:
	LTexture* spritesheet; //Textura a dibujar
	int sprtX, sprtY, sprtSizeX, sprtSizeY; //posición y tamaño en el spritesheet
	
	int x, y; //posición relativa
	
	int pivotX, pivotY; //posición del pivote sobre el cual rotar
	double angle; //ángulo en grados
	double maxAngle, minAngle; //ángulos máximo y mínimo 
	
	bool mirroring;
	int flip;
	
	limb* parent; //padre si tuviera
};

//Clase esqueleto [contenedora de miembros]
class limbSkeleton{
public:
	limbSkeleton();
	~limbSkeleton();
	
	// Agrega un miembro y devuelve el puntero al mismo
	// "parent" es es el índice del padre
	// Si parent es -1, no tiene padre. 
	limb* addLimb(LTexture *sprt, int parent); 
	//Obtiene el miembro de índice "limbNum"
	limb* getLimb(int limbNum);
	
	// Obtiene la cantidad de miembros que posee el esqueleto
	int getLimbNum();
	
	// dibuja
	void draw(painter*);
		
private:
	vector<limb*> limbs;
};

// clase para animaciones por esqueleto
// no agregué distintas velocidades, porque no me pareció necesario
// Nota: Antes sólo se podía animar un miembro por keyframe, pero whatever.
class limbAnim {
public:
	limbAnim(limbSkeleton* lskel, bool repeat);
	~limbAnim();
	
	// Lo que hace esta función es determinar el ángulo de un miembro 
	// en un frame determinado
	//
	// Ej: 
	//
	// addKeyframe(100, 45, 0);
	// addKeyframe(100, 90, 1);
	//
	// dice que en el frame 100, el miembro 0 está a 45 grados y
	// el miembro 1 a 90
	void addKeyframe(int frame, double angle, int limbNum);
	// indica si hay loop
	void setRepeat(bool repeat);
	
	// play cuando algo está en pausa
	void play();
	// pausar
	void pause();
	// resetear la animación
	void reset();
	// indica si la animación terminó
	// obviamente que si loopea esto siempre es false
	bool isFinished();
	
	// función step, como siempre
	void step();
			
private:
	
	map<int , std::map<int, double> > frames;
	limbSkeleton* skel;
	
	map<int, double> startAngles;
	
	map<int , std::map<int, double> >::iterator currentKeyframe;
	int currentFrame; //frame actual
	int startTime; 
	bool loop;
	
	bool paused;
};

// lerp es útil, pero también se puede hacer en una línea.
// Por esto último, consideré que no era necesario tener una función, pero bue
float lerp(float v0, float v1, float t); 

#endif
