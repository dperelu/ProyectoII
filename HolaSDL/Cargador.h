#pragma once
#include "Interfaz.h"
class Cargador :public Interfaz
{
public:
	Cargador(Juego* pJuego, SDL_Rect Camera, int alto, int ancho, int posX, int posY);
	virtual ~Cargador();
	void update(SDL_Rect Camera, int contador);
};

