#include "Play.h"
#include "ObjetoJuego.h"
#include "objetoDecorativo.h"
#include "GameOver.h"
#include "Player.h"
#include "BalaPlayer.h"
#include "BalaEnemigo.h"
#include "EnemigoGuardia.h"

#include "BossRino.h"
#include "Spider.h"

#include "EnemigoPlanta.h"
#include <iostream>

#include "BarraVida.h"
#include "BarraVidaVacia.h"
#include "Cargador.h"
#include "EnergiaDisponible.h"
#include "Checkpoint.h"
#include "EnemigoPlanta.h"
#include "Dialogo.h"
#include "Aura.h"
#include "Botiquin.h"
#include "Bobina.h"
#include "EnemigoBomba.h"

#include "Key.h"
#include "Puerta.h"

#include <time.h>
using namespace std;

Play::Play(Juego* ptr) : Estado(ptr)
{
	type = NIVEL_1;

	init();
}

Play::~Play()
{
	for (int i = 0; i < musicFiles.size(); i++){
		delete musicFiles[i];
	}
}

void Play::init() {

	//TUTORIAL
	juego->arrayObjetos.push_back(new Aura(juego, 200, 350, 400, 50, "Para moverte, utiliza WASD"));
	juego->arrayObjetos.push_back(new Aura(juego, 1450, 300, 440, 50, "Click izquierdo para disparar"));
	juego->arrayObjetos.push_back(new Aura(juego, 450, 1090, 460, 50, "Necesitas una llave para pasar"));
	juego->arrayObjetos.push_back(new Aura(juego, 980, 1090, 400, 50, "SPACE para hacer dash"));

	//MUSICA//
	musicNames.push_back("..\\bmps\\city.mp3");
	musicNames.push_back("..\\bmps\\synd.mp3");
	musicNames.push_back("..\\bmps\\DieHistoric.mp3");

	for (int j = 0; j < musicNames.size(); j++) {
		cancion = new Musica();
		cancion->load(musicNames[j]);
		musicFiles.push_back(cancion);
	}

	//La cancion del nivel es aleatoria
	srand(time(NULL));
	int rnd = rand() % musicFiles.size();
	musicFiles[rnd]->play();

	///		PUSH DE OBJETOS DECORATIVOS		///
	//Los pusheamos antes para que el jugador pase por delante de los objetos
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 450, 110, "MesaRota2"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 520, 110, "MesaRota"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 590, 110, "MesaPeque"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 660, 110, "MesaCorazon"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 800, 50, "VitrinaLib"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 900, 50, "VitrinaFeto"));

	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 350, 420, "MesaRota"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 420, 420, "MesaTentaculo"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 490, 420, "MesaPeque"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 560, 420, "MesaPeque"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 900, 350, "Planta3"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1520, 570, "Cafetera"));

	//Sala 1
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2100, 370, "Hojas"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1800, 130, "VitrinaLib"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1880, 130, "VitrinaLib"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1960, 130, "VitrinaFeto"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2040, 130, "VitrinaLib"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2300, 180, "MesaTentaculo"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2365, 180, "MesaCorazon"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2035, 320, "MesaDoble"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2035, 490, "MesaDoble"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1800, 500, "Planta2"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2080, 380, "papelera3"));

	//Sala 2
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1900, 1060, "Hojas"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2130, 1270, "Hojas"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2200, 900, "Hojas"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1850, 850, "VitrinaFeto"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1930, 850, "VitrinaLib"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2010, 850, "VitrinaFeto"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2190, 980, "papelera3"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2000, 1060, "MesaTentaculo"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2065, 1060, "MesaPeque"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2130, 1060, "MesaRota"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2250, 1150, "Planta1"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2290, 1210, "Planta3"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1800, 1180, "VitrinaFeto"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1880, 1180, "VitrinaLib"));

	//Pasillos
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 360, 1380, "MesaPeque"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 425, 1380, "MesaRota"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 300, 1000, "VitrinaFeto"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 125, 2360, "VitrinaFeto"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 220, 2360, "Cafetera"));

	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 180, 1300, "BancoDcha"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1250, 180, "BancoArriba"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1395, 840, "BancoDcha"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 825, 3300, "BancoDcha"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 550, 2600, "BancoIzda"));

	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 670, 2970, "MesaRota"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 735, 2970, "MesaTentaculo"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 860, 3150, "Planta3"));

	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1100, 4640, "Cafetera"));

	//Sala 5
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1000, 1560, "Hojas"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1200, 1660, "Hojas"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 890, 1340, "VitrinaFeto"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 975, 1340, "VitrinaFeto"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1200, 1560, "Planta2"));	

	//Sala 6
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 935, 2600, "Hojas"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1560, 2430, "VitrinaFeto"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1640, 2430, "VitrinaLib"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1450, 2800, "MesaDoble"));

	//Sala 7	
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 380, 3550, "Hojas"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 380, 3750, "Planta3"));

	//Sala 8
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 210, 5730, "Hojas"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 145, 5500, "VitrinaFeto"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 405, 5500, "VitrinaLib"));

	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 140, 4900, "Planta3"));

	//Sala 9
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1700, 5340, "Servers"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1850, 5340, "Servers"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1950, 5560, "Hojas"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2180, 5530, "MesaRota2"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 2245, 5530, "MesaRota"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1670, 5700, "Planta2"));

	//Humo
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, -50, -200, "Humo"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 0, 1200, "Humo"));

	//Random
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1470, 2600, "Hojas"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1100, 3670, "Hojas"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 480, 3400, "VitrinaFeto"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1385, 4800, "BancoIzda"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 570, 4140, "mesaEE"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 450, 4450, "Caja"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1240, 3570, "Caja"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1840, 6227, "Caja"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 330, 2960, "Servers"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego,1980, 6459, "Hojas"));
	juego->arrayObjetos.push_back(new ObjetoDecorativo(juego, 1895, 6819, "Hojas"));

	juego->arrayObjetos.push_back(new Checkpoint(juego, 1100, 5650));

	//LLAVES Y PUERTAS
	//1
	juego->arrayObjetos.push_back(new Key(juego, 2050, 1200));
	juego->arrayObjetos.push_back(new Puerta(juego, 150, 1250, 600, 200));
	//2
	//Llave en la ara�a
	juego->arrayObjetos.push_back(new Puerta(juego, 1200, 2350, 150, 150));
	//3
	juego->arrayObjetos.push_back(new Key(juego, 595, 4360));
	juego->arrayObjetos.push_back(new Puerta(juego, 700, 5360, 150, 150));

	///		*PLAYER		///
	if (juego->getCheck()){
		juego->player = new Player(juego, 1100, 5650);
		//juego->player = new Player(juego, 570, 6700);

		juego->arrayObjetos.push_back(juego->player);
	}
	else{
		juego->player = new Player(juego, 200, 300);
		//juego->player = new Player(juego, 2000, 6700);
		juego->arrayObjetos.push_back(juego->player);
	}

					///      BOBINAS     ///
	juego->arrayObjetos.push_back(new Bobina(juego, 500, 6425));
	juego->arrayObjetos.push_back(new Bobina(juego, 1320, 6425));
	juego->arrayObjetos.push_back(new Bobina(juego, 500, 6975));
	juego->arrayObjetos.push_back(new Bobina(juego, 1320, 6975));


					///		ENEMIGOS	///	
	//ENEMIGOS GUARDIA
	juego->arrayObjetos.push_back(new EnemigoGuardia(juego, 1260, 1750));
	juego->arrayObjetos.push_back(new EnemigoGuardia(juego, 430, 2300));
	juego->arrayObjetos.push_back(new EnemigoGuardia(juego, 380, 3690));
	//zona de la mesa con boton
	juego->arrayObjetos.push_back(new EnemigoGuardia(juego, 753, 4484));
	juego->arrayObjetos.push_back(new EnemigoGuardia(juego, 469, 4700));
	juego->arrayObjetos.push_back(new EnemigoGuardia(juego, 569, 4323)); //quitar si hay muchos

	//ENEMIGOS PLANTA
	juego->arrayObjetos.push_back(new EnemigoPlanta(juego, 1350, 1150));
	juego->arrayObjetos.push_back(new EnemigoPlanta(juego, 580, 1150));
	juego->arrayObjetos.push_back(new EnemigoPlanta(juego, 320, 1800));
	juego->arrayObjetos.push_back(new EnemigoPlanta(juego, 1220, 2700));
	juego->arrayObjetos.push_back(new EnemigoPlanta(juego, 350, 3500));
	juego->arrayObjetos.push_back(new EnemigoPlanta(juego, 600, 5250));
	juego->arrayObjetos.push_back(new EnemigoPlanta(juego, 1260, 1500));
	juego->arrayObjetos.push_back(new EnemigoPlanta(juego, 1350, 3100));
	juego->arrayObjetos.push_back(new EnemigoPlanta(juego, 1680, 3100));
	juego->arrayObjetos.push_back(new EnemigoPlanta(juego, 2214, 6857));
	juego->arrayObjetos.push_back(new EnemigoPlanta(juego, 1818, 6369));
	//plantas de la zona del boss


	//ENEMIGOS BOMBA
	juego->arrayObjetos.push_back(new EnemigoBomba(juego, 2020, 6500));
	juego->arrayObjetos.push_back(new EnemigoBomba(juego, 2113, 5651));

	//ENEMIGOS ARA�A
	juego->arrayObjetos.push_back(new Spider(juego, 320, 2600, true)); //LLAVE
	juego->arrayObjetos.push_back(new Spider(juego, 1222, 3900, false));
	juego->arrayObjetos.push_back(new Spider(juego, 271, 5973, false)); 

					///		INTERFAZ	///	
	juego->elemInterfaz.push_back(new BarraVidaVacia(juego, juego->player, 128, 32, 0, 0));
	juego->elemInterfaz.push_back(new BarraVida(juego, juego->player, 32, 32, 0, 0));
	juego->elemInterfaz.push_back(new Cargador(juego, juego->player, 75, 75, juego->SCREEN_WIDTH - 75, juego->SCREEN_HEIGHT - 85));
	juego->elemInterfaz.push_back(new EnergiaDisponible(juego, juego->player, 32, 32, 0, 0));

}

void Play::update(double delta) {	
	
	for (auto i : juego->arrayObjetos) {
		if (!i->isInside()) {
			juego->addToScreen(i);
		}
		else if (i->isInside() && !juego->isInScreen(i->getRect()))
			i->putInside(false);
	}

	//COLISIONES CON OBJETOS
	for (int i = 0; i < juego->objVisible.size(); ++i) {
		for (int j = 0; j < juego->objVisible.size(); ++j) {
			if (juego->checkCollision(juego->objVisible[i], juego->objVisible[j])) {
				juego->objVisible[i]->onCollision(juego->objVisible[j]->getType());
				juego->objVisible[j]->onCollision(juego->objVisible[i]->getType());
			}
		}
	}


	if (juego->player->isDead()){		
		juego->estado->changeCurrentState(GAME_OVER);
	}

	else {
		
		//UPDATE
		for (int i = 0; i < juego->arrayObjetos.size(); ++i) {
			juego->arrayObjetos[i]->update(delta);
		}

		for (int i = 0; i < juego->elemInterfaz.size(); i++) {
			juego->elemInterfaz[i]->update();
		}
		
	}
	//LIMPIEZA DE VECTOR DE OBJETOS
	for (int aux = 0; aux < juego->arrayObjetos.size(); ++aux) {
		if (juego->arrayObjetos[aux]->isDead()) {
			juego->arrayObjetos.erase(juego->arrayObjetos.begin() + aux);
		}
	}
	for (int aux2 = 0; aux2 < juego->playerBullets.size(); ++aux2) {
		if (juego->playerBullets[aux2]->isDead())
			juego->playerBullets.erase(juego->playerBullets.begin() + aux2);
	}
	for (int aux3 = 0; aux3 < juego->enemyBullets.size(); ++aux3) {
		if (juego->enemyBullets[aux3]->isDead())
			juego->enemyBullets.erase(juego->enemyBullets.begin() + aux3);
	}
	for (int aux4 = 0; aux4 < juego->objVisible.size(); ++aux4) {
		if (!juego->isInScreen(juego->objVisible[aux4]->getRect()) || juego->objVisible[aux4]->isDead())
			juego->objVisible.erase(juego->objVisible.begin() + aux4);
	}
		
	
}


void Play::draw()
{
	//Limpia el buffer
	//SDL_RenderClear(pRenderer);
	SDL_SetRenderDrawColor(pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Dibuja los objetos

	for (int aux = 0; aux < juego->arrayObjetos.size(); ++aux) {
		juego->arrayObjetos[aux]->draw();
	}
	for (int aux2 = 0; aux2 < juego->playerBullets.size(); ++aux2) {
		juego->playerBullets[aux2]->draw();
	}
	for (int aux3 = 0; aux3 < juego->enemyBullets.size(); ++aux3) {
		juego->enemyBullets[aux3]->draw();
	}

	//Dibuja interfaz, por encima de los objetos
	for (int i = 0; i < juego->elemInterfaz.size(); i++) {
		juego->elemInterfaz[i]->draw();
	}
	/*
	//Pintado de texto (cargador)
	if (juego->stats[2] > 0 && !juego->player->isDead()){
	fuenteCargador->load(juego->getTexto(1), 18);
	mensaje->loadFromText(pRenderer, to_string(juego->stats[2]), *fuenteCargador, Black);
	if (juego->stats[2] < 10){
	mensaje->render(pRenderer, juego->SCREEN_WIDTH - 44, juego->SCREEN_HEIGHT - 50);
	}
	else{ mensaje->render(pRenderer, juego->SCREEN_WIDTH - 54, juego->SCREEN_HEIGHT - 50); }
	}
	else{
	fuenteCargador->load(juego->getTexto(1), 18);
	mensaje->loadFromText(pRenderer, "Recargando", *fuenteCargador, Red);
	mensaje->render(pRenderer, ((juego->SCREEN_WIDTH / 2) - 100), ((juego->SCREEN_HEIGHT / 2) - 100));
	}
	*/

	SDL_RenderPresent(pRenderer);
}


void Play::newDisparo(ObjetoJuego * po, int posX, int posY) {

	//posiciones del rat�n
	int mX, mY;
	juego->getMousePos(mX, mY);

	int distance = sqrt((mX - posX)*(mX - posX) + (mY - posY)*(mY - posY));

	int vX = 75 * (mX - posX) / distance;
	int vY = 75 * (mY - posY) / distance;

	//Disparo
	juego->playerBullets.push_back(new BalaPlayer(juego, posX, posY, vX, vY));
}
/*
void Play::posPlayer(int& x, int& y) {
	static_cast <Player*>(juego->player)->getPos(x, y);
}*/

void Play::newDisparoEnemigo(int posEx, int posEy, int targetX, int targetY, int velDis) {
	//desde donde dispara //hacia donde dispara     //velocidad disparo


	int distance = sqrt((targetX - posEx)*(targetX - posEx) + (targetY - posEy)*(targetY - posEy));

	if (distance == 0) //He puesto esto para que no salga la excepci�n de que divide entre 0
		distance = 5;

	int vX = velDis * (targetX - posEx) / distance;
	int vY = velDis * (targetY - posEy) / distance;

	if (vX == 0 && vY == 0){ // Para que con lo de antes no se quede la bala flotando
		vX = vY = 10; // Si se hace lo de que al tocar al jugador haga da�o y te empuje un poco igual no hace falta
	}

	//float vX = velDis * (targetX - posEx) / distance + 0.01; //Arreglad esto para que no se haga 0
	//float vY = velDis * (targetY - posEy) / distance + 0.01;

	//Disparo
	juego->enemyBullets.push_back(new BalaEnemigo(juego, posEx, posEy, vX, vY));
}
