#pragma once
#include "SDL.h"
#include "Juego.h"

#include <iostream>
#include <vector>
#include <random>
#include <typeinfo>
#include <stack>
#include <iomanip>
#include <string>
#include <fstream>

#include "MenuPausa.h"
#include "Play.h"
#include "MenuPrincipal.h"
#include "GameOver.h"
#include "Pausa.h"
#include <exception>
#include "Error.h"
#include "MapEditor.h"
#include "Aura.h"
#include "Botiquin.h"
#include "enemy.h"
#include "EnemigoPlanta.h"

#include <SDL_mixer.h>
#include <SDL_ttf.h>


using namespace std;

//The level tiles
Tilemap::Tile* tileSet[TOTAL_TILES];
vector<Tilemap::Tile*> tileVisible;
vector<Tilemap::Tile*> wallsArray;	//Elementos del mapa de tiles que son colisionables

Juego::Juego()
{

	//Música
	Mix_Init(27);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Volume(1, MIX_MAX_VOLUME / 2);
	//Texto
	TTF_Init();

	srand(SDL_GetTicks());

	pWindow = nullptr;
	pRenderer = nullptr;
	pcolor = { 0, 0, 0 }; //Color del fondo negro;

	exit = false;
	GO = false;
	if (!initSDL())
	{
		printf("Failed to initialize!\n");
	}
	else if (!initMedia())
	{
		printf("Failed to load media!\n");
	}
	else
	{
		//camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		changeState(new MenuPrincipal(this));
		estado = topEstado(); //primer estado: MENU
	}
	camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	contDash = 80;

	for (auto i : wallsArray) {
		if (isInScreen(i->getBox())) {
			tileVisible.push_back(i);
			//i->setInside();
		}
	}

	for (auto j : arrayObjetos) {
		addToScreen(j);
	}


}

Juego::~Juego()
{
	estado = nullptr;
	freeMedia();
	Mix_Quit(); //Musica
	TTF_Quit(); //Texto
	closeSDL();
}

void Juego::cleanArrays() {

	for (int i = 0; i < arrayObjetos.size(); ++i)
		arrayObjetos.erase(arrayObjetos.begin() + i);

	for (int j = 0; j < elemInterfaz.size(); ++j)
		elemInterfaz.erase(elemInterfaz.begin() + j);
	
	for (int aux1 = 0; aux1 < enemyArray.size(); ++aux1)
		enemyArray.erase(enemyArray.begin() + aux1);
	
	for (int aux2 = 0; aux2 < playerBullets.size(); ++aux2)
		playerBullets.erase(playerBullets.begin() + aux2);

	for (int aux3 = 0; aux3 < enemyBullets.size(); ++aux3)
		enemyBullets.erase(enemyBullets.begin() + aux3);

	for (int aux4 = 0; aux4 < objVisible.size(); ++aux4)
		objVisible.erase(objVisible.begin() + aux4);

	for (int aux5 = 0; aux5 < arrayMenu.size(); ++aux5)
		arrayMenu.erase(arrayMenu.begin() + aux5);

	if(arrayObjetos.size() > 0)
		arrayObjetos.erase(arrayObjetos.begin());
	if (enemyArray.size() > 0)
		enemyArray.erase(enemyArray.begin());
	if (elemInterfaz.size() > 0)
		elemInterfaz.erase(elemInterfaz.begin());
	if (playerBullets.size() > 0)
		playerBullets.erase(playerBullets.begin());
	if (enemyBullets.size() > 0)
		enemyBullets.erase(enemyBullets.begin());
	if (objVisible.size() > 0)
		objVisible.erase(objVisible.begin());
	while (arrayMenu.size() > 0)
		arrayMenu.erase(arrayMenu.begin());

}
void Juego::run()
{
	Uint32 lastUpdate;

	while (!exit) {

		switch (estado->getCurrentState()) {
		case MENU_PRINCIPAL:
			
			changeState(new MenuPrincipal(this));
			estado = topEstado(); 
				
			//Render menu
			estado->draw();
			//Update menu
			estado->update();

			cleanArrays();
			break;

		case NIVEL_1:

			//GO = false;

			changeState(new Play(this));
			estado = topEstado(); 

			pause = new MenuPausa(this);
			cout << "NIVEL 1 (PLAY) \n";
			
			//Clear screen	
			SDL_RenderClear(pRenderer);

			lastUpdate = SDL_GetTicks();
			//Render level
			for (int i = 0; i < TOTAL_TILES; ++i)
			{
				tileSet[i]->render(pRenderer, camera);
			}

			//Update screen
			SDL_RenderPresent(pRenderer);

			estado->draw();
			handle_events();

			while (estado->getCurrentState() == NIVEL_1) {

				for (auto t : wallsArray) {
					if (!t->isInside()) {
						if (isInScreen(t->getBox())) {
							tileVisible.push_back(t);
							//t->setInside();
						}
					}
				}

				estado = topEstado();
				//if (SDL_GetTicks() - lastUpdate >= MSxUpdate){ //while(elapsed >= MSxUpdate)
				delta = (SDL_GetTicks() - lastUpdate) / 10.0f;
				estado->update(delta);
				lastUpdate = SDL_GetTicks();
				//}
				//estado = topEstado();
				//Render level
				for (int i = 0; i < TOTAL_TILES; ++i)
				{
					tileSet[i]->render(pRenderer, camera);
				}

				for (int i = 0; i < tileVisible.size(); ++i) {
					if (isInScreen(tileVisible[i]->getBox())) {
						tileVisible.erase(tileVisible.begin(), tileVisible.begin() + i);
					}
				}
				//cout << delta << "\n";
				estado->draw();
				handle_events();
				contDash++;
			}

			cleanArrays();
			delete player;
			break;
			
		case GAME_OVER:

			changeState(new GameOver(this));
			estado = topEstado();

			camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			estado->draw();
			estado->update();
			break;

		}

	}
	SDL_Delay(500); //cin.get();
}
/*

//Render level
//Clear screen
SDL_SetRenderDrawColor(pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
SDL_RenderClear(pRenderer);

//Render level
for (int i = 0; i < TOTAL_TILES; ++i)
{
tileSet[i]->render(camera);
}

//Update screen
SDL_RenderPresent(pRenderer);

*/

bool Juego::initSDL()
{

	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		pWindow = SDL_CreateWindow("GS-14", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (pWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (pRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool Juego::initMedia()
{
	bool success = true;
	//MUSICA
	musicNames.push_back("..\\Sonidos\\Musica\\Come.mp3");

	for (int j = 0; j < musicNames.size(); j++) {
		cancion = new Musica;
		cancion->load(musicNames[j]);
		musicFiles.push_back(cancion);
	}

	//musicFiles[Cancion1]->play();

	//TEXTO
	nombreFuentes.push_back("..\\Fuentes\\ARIAL.ttf");
	nombreFuentes.push_back("..\\Fuentes\\NES-Chimera.ttf");

	for (int j = 0; j < nombreFuentes.size(); j++) {
		textoAux.load(nombreFuentes[j], 50); //fuente, size
		arrayFuentes.push_back(textoAux);
	}

	texturas.push_back("..\\bmps\\personaje.png");
	texturas.push_back("..\\bmps\\bala.png");

	//Elementos del menu
	texturas.push_back("..\\bmps\\BotonPlayE.png");
	texturas.push_back("..\\bmps\\BotonPlayA.png");
	texturas.push_back("..\\bmps\\BotonOptionsE.png");
	texturas.push_back("..\\bmps\\BotonOptionsA.png");
	texturas.push_back("..\\bmps\\BotonExitE.png");
	texturas.push_back("..\\bmps\\BotonExitA.png");
	texturas.push_back("..\\bmps\\Fondo.png");

	texturas.push_back("..\\bmps\\enemy.png");
	texturas.push_back("..\\bmps\\balaEnemigo.png");

	//Interfaz
	texturas.push_back("..\\bmps\\VidaLlena.png");
	texturas.push_back("..\\bmps\\VidaVacia.png");
	texturas.push_back("..\\bmps\\DashLleno.png");
	texturas.push_back("..\\bmps\\DashVacio.png");
	texturas.push_back("..\\bmps\\Cargador.png");
	texturas.push_back("..\\bmps\\CuadroDialogo.png");
	//
	texturas.push_back("..\\bmps\\Checkpoint.png");
	texturas.push_back("..\\bmps\\enemyPlanta.png");
	texturas.push_back("..\\bmps\\BarraVida.png");
	texturas.push_back("..\\bmps\\Humo.png");
	texturas.push_back("..\\bmps\\Botiquin1.png");
	//Decorativos
	texturas.push_back("..\\bmps\\papelera1.png");
	texturas.push_back("..\\bmps\\papelera2.png");
	texturas.push_back("..\\bmps\\papelera3.png");
	texturas.push_back("..\\bmps\\mesaeasteregg.png");

	texturas.push_back("..\\bmps\\MesaPequena.png");
	texturas.push_back("..\\bmps\\MesaCorazon.png");
	texturas.push_back("..\\bmps\\MesaTentaculo.png");
	texturas.push_back("..\\bmps\\MesaDoble.png");
	texturas.push_back("..\\bmps\\MesaRota.png");
	texturas.push_back("..\\bmps\\MesaRota2.png");

	texturas.push_back("..\\bmps\\Bobina.png");
	texturas.push_back("..\\bmps\\Aura.png");

	//Plantas (decorativos)
	texturas.push_back("..\\bmps\\Planta1.png");
	texturas.push_back("..\\bmps\\Planta2.png");
	texturas.push_back("..\\bmps\\Planta3.png");

	texturas.push_back("..\\bmps\\VitrinaLibreria.png");
	texturas.push_back("..\\bmps\\VitrinaFeto.png");

	//Load Assets Textures
	for (int j = 0; j < texturas.size(); ++j) {
		pTexture = new Textura;
		pTexture->load(pRenderer, texturas[j]);
		arrayTex.push_back(pTexture);
	}

	//Load tile texture
	if (!gTileTexture.loadFromFile(pRenderer, "..\\bmps\\sheet3.png"))
	{
		printf("Failed to load tile set texture!\n");
		success = false;
	}

	//Load tile map
	if (!setTiles(tileSet))
	{
		printf("Failed to load tile set!\n");
		success = false;
	}

	//Load tile colliders to new array
	for (auto i : tileSet) {
		if (i->isWall())
			wallsArray.push_back(i);
	}

	ifstream file;
	file.open("..\\bmps\\almas.txt", ios::in);
	auto cinbuf = std::cin.rdbuf(file.rdbuf()); //

	if (file.is_open())
	{
		string leeFraseI;
		int tam = -1;
		std::cin >> tam;

		for (int i = 0; i < tam + 1; ++i)
		{
			getline(file, leeFraseI);

			VectTextosAlma.push_back(leeFraseI);
		}
	}

	return success;
}

void Juego::freeMedia()
{
	//Objetos
	for (size_t aux = 0; aux < arrayMenu.size(); ++aux) {
		delete arrayMenu[aux];
		arrayMenu[aux] = nullptr;
	}
	//Interfaz
	for (int i = 0; i < elemInterfaz.size(); i++){
		delete elemInterfaz[i];
		elemInterfaz[i] = nullptr;
	}

	//juego = nullptr;
	//textura = nullptr;
	pRenderer = nullptr;

	for (size_t aux = 0; aux < arrayTex.size(); ++aux) {
		delete arrayTex[aux];
		//arrayTex[aux] = nullptr;
	}

	//close(tileSet, pRenderer, pWindow);

	for (int i = 0; i < musicFiles.size(); i++){
		delete musicFiles[i];
	}

}

void Juego::closeSDL()
{
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		if (tileSet[i] == NULL)
		{
			delete tileSet[i];
			tileSet[i] = NULL;
		}
	}

	//Free loaded images
	gTileTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
	pWindow = NULL;
	pRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


void Juego::handle_events()
{
	if (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) setSalir();
		else if (e.type == SDL_MOUSEBUTTONUP) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				//std::cout << "CLICK";
				x = e.button.x + (camera.x + camera.w / 2) - SCREEN_WIDTH / 2;
				y = e.button.y + (camera.y + camera.h / 2) - SCREEN_HEIGHT / 2;
				//estado->onClick();
				player->onClick();
			}
			else if (e.button.button == SDL_BUTTON_RIGHT) {
				pause->draw();
				pause->update();
			 }
		}
		/*else if (e.type == SDLK_ESCAPE) {
			//pause->active = true;
			pause->draw();
			pause->update();
		}*/
		updateDirection();
		//std::cout << contDash << "\n";
		if (contDash >= 80){ //Timer del Dash
			dashAux = true;
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_SPACE) {
					contDash = 0;
					dashing = true;		
					dashAux = false; //Para la interfaz
				}
			}	
		}
		/*if (e.type == SDL_KEYUP) {
			if (e.key.keysym.sym == SDLK_ESCAPE && dynamic_cast<Play*>(topEstado()) != nullptr) {
				pushState(new Pausa(this));
			}
		}*/


	}
}

void Juego::updateDirection() {

	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY -= VPLAYER; break;
		case SDLK_w: mVelY -= VPLAYER; break;

		case SDLK_DOWN: mVelY += VPLAYER; break;
		case SDLK_s: mVelY += VPLAYER; break;

		case SDLK_LEFT: mVelX -= VPLAYER; break;
		case SDLK_a: mVelX -= VPLAYER; break;

		case SDLK_RIGHT: mVelX += VPLAYER; break;
		case SDLK_d: mVelX += VPLAYER; break;

		case SDLK_r: setRecargar(true); break;
		}
	}

	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY += VPLAYER; break;
		case SDLK_w: mVelY += VPLAYER; break;

		case SDLK_DOWN: mVelY -= VPLAYER; break;
		case SDLK_s: mVelY -= VPLAYER; break;

		case SDLK_LEFT: mVelX += VPLAYER; break;
		case SDLK_a: mVelX += VPLAYER; break;

		case SDLK_RIGHT: mVelX -= VPLAYER; break;
		case SDLK_d: mVelX -= VPLAYER; break;

		case SDLK_r: setRecargar(false); break;		
		}
	}
}

//////////////////// GETTERS /////////////////////
void Juego::getMousePos(int & mpx, int & mpy) const
{
	mpx = x;
	mpy = y;
}


////////////////// PILA ///////////////////////////
void Juego::changeState(EstadoJuego* newSt)
{
	if (!states.empty()) popState(); //si no está vacía, se vacía
	pushState(newSt); //se mete el nuevo estado
}

void Juego::pushState(EstadoJuego* newState)
{
	states.push(newState);
}

void Juego::popState()
{
	delete states.top();
	states.pop();
}

EstadoJuego* Juego::topEstado() { return states.top(); }


////////////////// COLISIONES ////////////////////
bool Juego::checkCollision(ObjetoJuego * a, ObjetoJuego * b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	bool colisiona = false; //previo GoodToGo. Nos costaba enterderlo con ese nombre

	//Cambiado a que revise los que queremos (los que son true) en vez de lo que queremos obviar
	switch (a->getType()) {
	case PJ:												//Hemos puesto la colision con el aura para que siga funcionando
		//pero hay que quitarla para que no reste vida cuando se hagan bien los arrays
		if (b->getType() == ENEMY_WEAPON || b->getType() == ENEMY || b->getType() == BOSS || b->getType() == AURA || b->getType() == BOTIQUIN)
			colisiona = true;

		break;
	case ENEMY:
		if (b->getType() == PJ_WEAPON)
			colisiona = true;
		break;
	case CHECK:
		if (b->getType() == PJ)
			colisiona = true;
		break;
	case PJ_WEAPON:
		if (b->getType() == ENEMY || b->getType() == BOSS || b->getType() == ENEMY_WEAPON)
			colisiona = true;
		break;
	case ENEMY_WEAPON:
		if (b->getType() == PJ || b->getType() == PJ_WEAPON)
			colisiona = true;
		break;
	case AURA:
		if (b->getType() == PJ)
			colisiona = true;
		break;
	case DECORATIVO:
		if (b->getType() == PJ || b->getType() == ENEMY){
			//
		}
	}

	if (!colisiona)
		return false;

	//Calculate the sides of rect A
	leftA = a->getRect().x;
	rightA = a->getRect().x + a->getRect().w;
	topA = a->getRect().y;
	bottomA = a->getRect().y + a->getRect().h;

	//Calculate the sides of rect B
	leftB = b->getRect().x;
	rightB = b->getRect().x + b->getRect().w;
	topB = b->getRect().y;
	bottomB = b->getRect().y + b->getRect().h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//printf("Enemy touched!\n");

	//If none of the sides from A are outside B
	return true;
}

bool Juego::touchesWall(SDL_Rect objRect)
{
	//Go through the tiles

	for (int i = 0; i < tileVisible.size(); ++i)
	{
		//If the collision box touches the wall tile
		if (checkWallCollisions(objRect, tileVisible[i]->getBox())) {
			return true;
		}

	}
	//If no wall tiles were touched
	return false;



			//If the tile is a wall type tile
			/*//(tileSet[i]->getType() >= TILE_CENTER) && (tileSet[i]->getType() <= TILE_TOPLEFT)) <<-- EXTRAÍDO DE LAZYFOO
			if (tileSet[i]->getType() == TILE_TOPLEFT || tileSet[i]->getType() == TILE_TOP ||
			tileSet[i]->getType() == TILE_LEFT || tileSet[i]->getType() == TILE_DOWN ||
			tileSet[i]->getType() == TILE_RIGHT || tileSet[i]->getType() == TILE_DOWNRIGHT ||
			tileSet[i]->getType() == TILE_DOWNLEFT)
			{
			//If the collision box touches the wall tile
			if (checkWallCollisions(a, tileSet[i]->getBox()))
			{
			return true;
			}
			}*/
			/*
			if (tileSet[i]->isWall()) {
				//If the collision box touches the wall tile
				if (checkWallCollisions(object, tileSet[i]->getBox()))
				{
					return true;
				}
			}

			*/


	
}

bool Juego::checkWallCollisions(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;
	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

/// CREAR UNIDADES ///
//Cuando los enemigos mueren, crean almas y/o objetos según un random. Aquí se recogen los métodos que
//añaden cosas al juego

void Juego::creaAlmas(int posEnemigoX, int posEnemigoY, string msj){

	if (msj != ""){
		//Selecciona una frase aleatoria en el vector de frases y la metes en msj.
		//Si hay una que QUIERES que aparezca, la metes en el parámetro msj
	}

	arrayObjetos.push_back(new Aura(this, posEnemigoX, posEnemigoY, 420, 50, msj));

}

//Creamos un objeto dado por el id que se introduzca en la llamada a función, según el random deseado en el objeto desde el que se
//llame a este método.
void Juego::spawnObjetos(char id, int posEnemigoX, int posEnemigoY, string msj){

	int v2 = rand() % 70 + 30;				//Rango: [30,70]
	int direction = 1 - 2 * (rand() % 2);	//Decide el signo del random

	switch (id)
	{

	case 'a':	//alma

		while (msj == ""){
			//Selecciona una frase aleatoria en el vector de frases y la metes en msj.
			//Si hay una que QUIERES que aparezca, la metes en el parámetro msj al llamar a la funcion spawnObjetos.
			int miniRnd = rand() % VectTextosAlma.size() + 0;				
			msj = VectTextosAlma.at(miniRnd);
		}

		arrayObjetos.push_back(new Aura(this, posEnemigoX, posEnemigoY, 420, 50, msj));
		break;

	case 'b':	//botiquín

		//Mini random aqui para que el objeto no caiga justo encima del enemigo sino ligeramente desplazado
		
		arrayObjetos.push_back(new Botiquin(this, posEnemigoX + (30 * direction), posEnemigoY + (30 * direction)));
		break;

	case 'e':   //Spawn de enemigo que camina

		arrayObjetos.push_back(new enemy(this, posEnemigoX + (30 * direction), posEnemigoY));
		break;

	case 'p':	//Spawn de enemigos de plantas
		arrayObjetos.push_back(new EnemigoPlanta(this, posEnemigoX, posEnemigoY + (30 * direction)));
		break;
	}
}


/*
void Juego::addToScreen(Tilemap::Tile * t) { //ONLY ARRAY OF WALLS
	if (isInScreen(t->getBox())) {
		tileVisible.push_back(t);
	}
}
*/
void Juego::addToScreen(ObjetoJuego * obj) {
	if (isInScreen(obj->getRect())) {
		objVisible.push_back(obj);
		obj->putInside();
	}
}
bool Juego::isInScreen(SDL_Rect rect) {

	//SDL_Rect * pantalla;	//CAMERA

	if (rect.x < camera.w + camera.x && rect.y < camera.h + camera.y) {
		if (rect.x > camera.x && rect.y > camera.y) {
			//objVisible.push_back = obj;
			return true;
		}
	}	
	return false;
}