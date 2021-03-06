#pragma once
#include "Objeto.h"

class Boton : public Objeto
{
public:
	
	//typedef void CallBack_t(Juego* jg);
	Boton(Juego* ptr, int px, int py, Juego::Texturas_t tex1, Juego::Texturas_t tex2, collision type);
	Boton(Juego* ptr, int px, int py, int pw, int ph, Juego::Texturas_t tex1, Juego::Texturas_t tex2, collision type);
	virtual ~Boton();
	
	void update();
	void draw() const;
	void changeText();
	bool onClick();
private:
	Textura *textura1, *textura2;
	//CallBack_t * cbOnClick;
};

