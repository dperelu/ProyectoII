#pragma once
#include "EstadoJuego.h"
#include "Juego.h"
#include "HUD.h"
using namespace std;

class Estado : public EstadoJuego
{
public:

	Estado(Juego* ptr);
	virtual ~Estado();

	void draw();
	void update(int delta) {};
	void update();
	bool onClick();
	
	//STATE CHANGER
	void play();
	void menu();
	void salir();

	GAME_STATES getCurrentState() { return type; };
	void changeCurrentState(GAME_STATES newType) { type = newType; };
	ObjetoJuego* getObjeto(int i) const { return juego->arrayObjetos[i]; }; //Getter del objeto i del array
	int getSizeArray() const { return juego->arrayObjetos.size(); }; //Getter del tama�o del array

protected:

	Juego* juego;
	SDL_Event e;

	SDL_Renderer* pRenderer;
	Textura* textura;
	
	void resetCamera();
	
	GAME_STATES type;

	int mouse_x, mouse_y;
	int height, width;
};

