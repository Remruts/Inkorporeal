#ifndef _LEVEL_LAYOUT_
#define _LEVEL_LAYOUT_

class levelLayout{
public:
	levelLayout(bool*, player*); //, vector<enemy*>
	~levelLayout();

	//determina si un punto está sobre el jugador
	bool isPlayer(int x, int y);
	
	//determina si un punto está sobre algo sólido
	bool isSolid(int x, int y);
	
	// Determina si hay algo sólido en una línea horizontal del punto x1 al x2
	// Si hubiese, devuelve la posición de contacto, sino -1
	int hRaySolid(int x1, int x2, int y);
	// Determina si hay algo sólido en una línea vertical del punto y1 al y2
	// Si hubiese, devuelve la posición de contacto, sino -1
	int vRaySolid(int y1, int y2, int x);
	
	//chequea colisión del jugador con un enemigo
	bool playerBumpsEnemy();
	
	//bool bulletBumpsEnemy();
	//bool bulletBumpsPlayer();
	
	void update();
	
private:
	bool solid[420]; // mapa de 30x14 (=420). Determina si es sólido (true) o no (false)
	player* jugador;
	//vector<playerBullet*>
	//vector<enemy*>;
	//vector<enemyBullet*>
};

#endif
