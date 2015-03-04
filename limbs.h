#ifndef __THE_KING_OF_LIMBS__
#define __THE_KING_OF_LIMBS__

#include "LTexture.h"
#include "painter.h"
#include <cmath>

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
	double maxAngle, minAngle; //ángulos máximo y mínimo 
	bool mirroring;
	limb* parent; //padre si tuviera
};


#endif
