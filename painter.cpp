#include "painter.h"

painter::painter(SDL_Renderer *screen){
	canvas = screen;
	rect.x = 0;
	rect.y = 0;
	
	textColor.r = 0;
	textColor.g = 0;
	textColor.b = 0;
	
	eightbit8 = TTF_OpenFont( "fonts/PressStart2P.ttf", 8 ); 
	if (eightbit8 == NULL) { 
		std::cout <<  "Failed to load font 8! SDL_ttf Error: " << TTF_GetError() << std::endl;
		exit(1);
	}
	
	ubuntuFont24 = TTF_OpenFont( "fonts/Ubuntu-L.ttf", 24 ); 
	if (ubuntuFont24 == NULL) { 
		std::cout <<  "Failed to load font 24! SDL_ttf Error: " << TTF_GetError() << std::endl;
		exit(1);
	}
	
	ubuntuFont32 = TTF_OpenFont( "fonts/Ubuntu-L.ttf", 32 ); 
	if (ubuntuFont32 == NULL) { 
		std::cout <<  "Failed to load font 32! SDL_ttf Error: " << TTF_GetError() << std::endl;
		exit(1);
	}
	
	ubuntuFont48 = TTF_OpenFont( "fonts/Ubuntu-L.ttf", 48 ); 
	if (ubuntuFont48 == NULL) { 
		std::cout <<  "Failed to load font 48! SDL_ttf Error: " << TTF_GetError() << std::endl;
		exit(1);
	}
	
	//Empezamos con un color random
	randomHue = (rand()%360)/double(360);
	randomSaturation = 0.8;
	randomValue = 0.9;
	
	//shake
	maxShakeFactor = 0;
	shakeFactor = 0;
	shakeX = 0;
	shakeY = 0;
}

painter::~painter(){
	if (eightbit8 != NULL){
		TTF_CloseFont(eightbit8);
		eightbit8 = NULL;
	}
	
	if (ubuntuFont24 != NULL){
		TTF_CloseFont(ubuntuFont24);
		ubuntuFont24 = NULL; 
	}
	
	if (ubuntuFont32 != NULL){
		TTF_CloseFont(ubuntuFont32);
		ubuntuFont32 = NULL; 
	}
	
	if (ubuntuFont48 != NULL){
		TTF_CloseFont(ubuntuFont48);
		ubuntuFont48 = NULL;
	}
	
}

void painter::draw(LTexture *tex, int srcX, int srcY, int srcW, int srcH, int screenX, int screenY){

	rect.x = srcX;
	rect.y = srcY;
	if (srcW == 0 || srcH == 0){
		rect.w = tex->getWidth();
		rect.h = tex->getHeight();
	} else{
		rect.w = srcW;
		rect.h = srcH;
	}
	
	screen_rect.x = screenX+shakeX;
	screen_rect.y = screenY+shakeY;
	screen_rect.w = rect.w;
	screen_rect.h = rect.h;
	
	//debug
	//std::cout << "x: " << screen_rect.x << ", y: " << screen_rect.y << std::endl;
	//std::cout << "w: " << rect.w << ", h: " << rect.h << std::endl;
	
	SDL_RenderCopy(canvas, tex->getTexture(), &rect, &screen_rect);
}

void painter::drawEx(LTexture *tex, int srcX, int srcY, int srcW, int srcH, 
	int screenX, int screenY, int screenW, int screenH, 
	const double angle, int flip){
	
	SDL_RendererFlip flp;
	if (flip == 1){
		flp = SDL_FLIP_HORIZONTAL;
	} else if (flip == 2){
		flp = SDL_FLIP_VERTICAL;
	} else if (flip ==3){
		flp = SDL_RendererFlip(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);
	} else {
		flp = SDL_FLIP_NONE;
	}
	
	rect.x = srcX;
	rect.y = srcY;
	if (srcW == 0 || srcH == 0){
		rect.w = tex->getWidth();
		rect.h = tex->getHeight();
	} else{
		rect.w = srcW;
		rect.h = srcH;
	}
	
	screen_rect.x = screenX+shakeX;
	screen_rect.y = screenY+shakeY;
	if (screenW == 0 || screenH == 0){
		screen_rect.w = tex->getWidth();
		screen_rect.h = tex->getHeight();
	} else{
		screen_rect.w = screenW;
		screen_rect.h = screenH;
	}
	
	
	SDL_RenderCopyEx(canvas, tex->getTexture(), &rect, &screen_rect, angle, NULL, flp);
	
}

SDL_Surface* painter::loadImage(const string & path ){
	return IMG_Load( path.c_str() );
}

//Robado de Lazy Foo, pero cambiado para adecuarse a mi painter
LTexture* painter::loadTexture(const string & path ) { 
	//The final texture 
	LTexture* finalTex = NULL;
	SDL_Texture* newTexture = NULL; 
	//Load image at specified path 
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() ); 
	if( loadedSurface == NULL ) { 
		std::cout <<  "Unable to load image! " << path << "\nError: " << IMG_GetError() << std::endl; 
		exit(1);
	} else { 
		//Create texture from surface pixels 
		newTexture = SDL_CreateTextureFromSurface( canvas, loadedSurface ); 
		if( newTexture == NULL ) { 
			std::cout <<  "Unable to create texture from " << path << "\nError: " << SDL_GetError() << std::endl;
			exit(1);
		}
		 
		finalTex = new LTexture(loadedSurface->w, loadedSurface->h, newTexture);
		//Get rid of old loaded surface 
		SDL_FreeSurface( loadedSurface ); 
		loadedSurface = NULL;
	} 
	
	return finalTex; 
}

LTexture* painter::textureFromText(const string& textureText, int size, unsigned char r, unsigned char g, unsigned char b){
	
	textColor.r = r;
	textColor.g = g;
	textColor.b = b;
	
	SDL_Texture* mTexture = NULL;
	LTexture* finalTex = NULL;
	SDL_Surface* textSurface = NULL;
	
	//Render text surface 
	if (size == 0){
		textSurface = TTF_RenderUTF8_Blended( eightbit8, textureText.c_str(), textColor); 
	} else if (size == 1){
		textSurface = TTF_RenderUTF8_Blended( ubuntuFont24, textureText.c_str(), textColor); 
	} else if (size == 2){
		textSurface = TTF_RenderUTF8_Blended( ubuntuFont32, textureText.c_str(), textColor); 
	} else{
		textSurface = TTF_RenderUTF8_Blended( ubuntuFont48, textureText.c_str(), textColor); 
	}
	
	if(textSurface == NULL) { 
		std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
		exit(1);
	} else { 
		//Create texture from surface pixels 
		mTexture = SDL_CreateTextureFromSurface( canvas, textSurface ); 
		if( mTexture == NULL ) { 
			std::cout << "Unable to create texture from rendered text! SDL Error: \n" << SDL_GetError() << std::endl; 
			exit(1);
		} else { 
			//create finalTex
			finalTex = new LTexture(textSurface->w, textSurface->h, mTexture); 
		} 
		//Get rid of old surface 
		SDL_FreeSurface(textSurface); 
		textSurface = NULL;
	}
	//if everything went right, return finalTex
	return finalTex;
}

void painter::freeTexture(LTexture* tex){
	delete tex;
	//LTexture hace SDL_DestroyTexture(tex);
}

void painter::freeImage(SDL_Surface* sur){
	SDL_FreeSurface(sur);
}

LTexture* painter::textureFromSurface(SDL_Surface* sur){
	LTexture* finalTex = NULL;
	SDL_Texture* newTexture = NULL;
	newTexture = SDL_CreateTextureFromSurface( canvas, sur);
	SDL_Texture* copy = SDL_CreateTexture(canvas, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, sur->w, sur->h);

	finalTex = new LTexture(sur->w, sur->h, newTexture);
	SDL_SetRenderTarget(canvas, copy);
	draw(finalTex, 0, 0, 0, 0, 0, 0);
	resetRenderTarget();
	delete finalTex;
	
	SDL_SetTextureBlendMode(copy, SDL_BLENDMODE_BLEND);
	finalTex = new LTexture(sur->w, sur->h, copy);
	return finalTex;
}

void painter::setRenderTarget(LTexture* tex){
	if (tex != NULL){
		SDL_SetRenderTarget(canvas, tex->getTexture());
	}
}

void painter::resetRenderTarget(){
	SDL_SetRenderTarget(canvas, NULL);
}

//Dibujar cosas lindas
// setea el color para dibujar estas cosas
void painter::setColor(int r, int g, int b, int a){
	SDL_SetRenderDrawColor(canvas, r, g, b, a);
}
// 0=none, 1=blend, 2=add, 3=modulate;
void painter::setBlendMode(int blend){
	switch (blend){
	case 0:
		SDL_SetRenderDrawBlendMode(canvas, SDL_BLENDMODE_NONE);
	break;
	case 1:
		SDL_SetRenderDrawBlendMode(canvas, SDL_BLENDMODE_BLEND);
	break;
	case 2:
		SDL_SetRenderDrawBlendMode(canvas, SDL_BLENDMODE_ADD);
	break;
	case 3:
		SDL_SetRenderDrawBlendMode(canvas, SDL_BLENDMODE_MOD);
	break;
	default: 
		SDL_SetRenderDrawBlendMode(canvas, SDL_BLENDMODE_NONE);
	break;
	}
}
//dibuja un rectángulo
void painter::drawRect(int x, int y, int w, int h, bool fill){
	rect.x = x+shakeX;
	rect.y = y+shakeY;
	rect.w = w;
	rect.h = h;
	
	if (fill){
		SDL_RenderFillRect(canvas, &rect);
	} else{
		SDL_RenderDrawRect(canvas, &rect);
	}
	
}

//dibuja una línea
void painter::drawLine(int x1, int y1, int x2, int y2){
	SDL_RenderDrawLine(canvas, x1+shakeX, y1+shakeY, x2+shakeX, y2+shakeY);
}

void painter::setShake(double shk){
	maxShakeFactor = shk;
	if (shakeFactor < 1)
		shakeFactor = shk/2;
}

double painter::getShake(){
	return maxShakeFactor;
}

void painter::clear(){
	if (shakeFactor < maxShakeFactor){
		if (shakeFactor < 1){
			shakeFactor = 1;
		}else{
			shakeFactor *= 1.1;
		}
	} else if(shakeFactor > maxShakeFactor){
		shakeFactor *= 0.9;
	}
	
	if (fabs(shakeFactor) > 0.5){
		int shk = int(shakeFactor);
		shakeX = 2*(rand()%(shk+1))-shk;
		shakeY = 2*(rand()%(shk+1))-shk;
	}
	
	SDL_RenderClear(canvas);
}

void painter::setSaturation(unsigned int S){
	randomSaturation = S%101;
}

void painter::setValue(unsigned int V){
	randomValue = V%101;
}

void painter::getRandomColor(unsigned int &R, unsigned int &G, unsigned int &B){
	randomHue += GOLDEN_RATIO_CONJUGATE;
	randomHue = fmod(randomHue, 1);
	hsvToRgb((unsigned int)(randomHue*359), randomSaturation, randomValue, R, G, B);
}

//conversión hsv a rgb, porque sí
//sacado de Wikipedia
void painter::hsvToRgb(unsigned int H, double S, double V, 
						unsigned int &R, unsigned int &G, unsigned int &B){
	
	//gris;
	if(S == 0) {
		R = V;
		G = V;
		B = V;
		return;
	}
	
	unsigned int Hi = (H/60)%6;
	double f = fmod((double(H)/60), 6) - Hi;
	unsigned int p = (V*(1-S))*255.0;
	unsigned int q = (V*(1-f*S))*255.0;
	unsigned int t = (V*(1-(1-f)*S))*255.0;
	
	switch (Hi){
	case 0:
		R = V*255;
		G = t;
		B = p;
	break;
	
	case 1:
		R = q;
		G = V*255;
		B = p;
	break;
	
	case 2:
		R = p;
		G = V*255;
		B = t;
	break;
	
	case 3:
		R = p;
		G = q;
		B = V*255;
	break;
	
	case 4:
		R = t;
		G = p;
		B = V*255;
	break;
	
	case 5:
		R = V*255;
		G = p;
		B = q;
	break;
	}
}
