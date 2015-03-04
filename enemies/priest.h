#ifndef _PRIEST_
#define _PRIEST_

#include "enemy.h"

class demon;

class priest : public enemy{
public:
	priest(LTexture* sprt, int x, int y);
	~priest();	
	
	virtual void step(level*);
	virtual void draw(painter*);
private:
	void drawHalo(painter* p, LTexture* cube, int x, int y, double direction, int size, int counter, int colour);
	animation* movingAnim;
	double timer;
	double haloAngle;
	int haloColor;
	demon* shadowDevil;
};

class limb{
public:
	limb(LTexture* sprt, limb* parent);
	~limb();
	
	void getPos(int &X, int &Y); //posición relativa
	double getAngle(); //ángulo relativo en grados
	void getPivot(int &X, int &Y); //obtener pivote
	
	void setPos(int X, int Y); //setear posición
	void setAngle(double ang); //setear ángulo relativo;
	void setPivot(int X, int Y); //setear el pivote;
	void setSprite(int X, int Y, int tamX, int tamY); //qué sprite va a ser dibujado
	void mirror(int m); //espejar: 0=no, 1=horizontal, 2=vertical, 3=ambos.
	
	void getWorldPos(int &X, int &Y); //posición real en el mundo
	double getWorldAngle();	//ángulo real
	
	void draw(painter* pintor); //dibujar
		
private:
	LTexture* spritesheet; //Textura a dibujar
	int sprtX, sprtY, sprtSizeX, sprtSizeY; //posición y tamaño en el spritesheet
	int x, y; //posición relativa
	int pivotX, pivotY; //posición del pivote sobre el cual rotar
	double angle; //ángulo en grados
	bool mirroring;
	limb* parent; //padre si tuviera
};

class demon {
public:
	demon(LTexture* sprt, int X, int Y);
	~demon();
	
	void step(level*);
	void draw(painter*);
private:
	LTexture* spritesheet;
	int x, y;
	int face;
	
	double angBrazoIzq;
	
	limb *brazoIzq, *brazoDer; //brazos
	limb *antebrazoIzq, *antebrazoDer; //antebrazos
	limb *manoIzq, *manoDer; //manos
};

#endif
