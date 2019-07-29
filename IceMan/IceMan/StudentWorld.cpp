#include "StudentWorld.h"
#include <string>
#include<iostream>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}
StudentWorld::~StudentWorld() {
	m_ice.clear();
	m_iceman = nullptr;
}

int StudentWorld::init() {
	barrels = 0;
	m_iceman = std::make_shared<Iceman>(this);
	

	int T = 200 - getLevel();
	SpawnPtime = max(25, T);
	LastSpawn = 0;
	int P = 2 + getLevel() * 1.5;
	MaxP = min(15, P);
	int W = 3 - getLevel() / 4;
	WaitRPMove = max(0, W);
	int A = 100 - getLevel() * 10;
	AnnoyedStunT = max(50, A);
	HardProM = 16 + getLevel() * 2;
	int L = getLevel() * 10 + 30;
	probabilityOfHardcore = min(90, L);

	int m = 100 - getLevel() * 10;
	tickstare = max(50,m );

	//barrel
	int B = getLevel() / 2 + 2;
	Barrelspawn = min(B, 9);
	int i = 0;
	while (i < Barrelspawn) {
		int randX = 30;
		bool made = false;
		
		while (made == false) {
			bool close = false;
			while (randX <= ((VIEW_WIDTH / 2) + 2) && randX >= ((VIEW_WIDTH / 2) - 6)) {
				randX = rand() % 60 + 0;
			}
			int randY = rand() % 56 + 0;
			//m_barrel.push_back(new Barrel(randX, randY,this));
			for (auto it : m_actor) {
				int ID = it->getID();
				if ((ID == IID_BARREL || ID == IID_GOLD || ID == IID_BOULDER) && close == false) {
					if (it->getRadius2(randX,randY) < 4) {
						close = true;
					}
				}
			}
			if (close == false) {
				m_actor.push_back(std::make_shared<Barrel>(randX, randY, this));
				barrels++;
				//emptysquares.erase(std::remove(emptysquares.begin(), emptysquares.end(), std::make_pair(randX, randY)), emptysquares.end());
				made = true;
			}
		}
		i++;
	}
	
	//Gold
	int G = 5 - getLevel() / 2;
	MaxG = max(G, 2);
	i = 0;
	while (i < MaxG) {
		int randX = 30;
		bool made = false;
		
		while (made == false) {
			bool close = false;
			while (randX <= ((VIEW_WIDTH / 2) + 2) && randX >= ((VIEW_WIDTH / 2) - 6)) {
				randX = rand() % 60 + 0;
			}
			int randY = rand() % 56 + 20;
			for (auto it : m_actor) {
				int ID = it->getID();
				//m_gold.push_back(new GoldNug(randX, randY, 1, this));
				if ((ID == IID_BARREL || ID == IID_GOLD || ID == IID_BOULDER) && close == false) {
					if (it->getRadius2(randX, randY) < 4) {
						close = true;						
						//emptysquares.erase(std::remove(emptysquares.begin(), emptysquares.end(), std::make_pair(randX, randY)), emptysquares.end());
					}
				}
			}
			if (close == false) {
				m_actor.push_back(std::make_shared<GoldNug>(randX, randY, 1, this));
				made = true;
			}
		}
		i++;
	}
	//Sonar
	int Z = 300 - 10 * getLevel();
	SonarandPoolTick = max(100, Z);
	GoodieRate = getLevel() * 25 + 300;



	for (int i = 0; i < VIEW_WIDTH; i++) {
		std::vector<std::shared_ptr<Ice>> col;
		for (int k = 0; k < VIEW_HEIGHT; k++) {
			col.push_back(std::shared_ptr<Ice>(make_shared<Ice>(i, k)));
		}
		m_ice.push_back(std::move(col));
	}

	// Makes top row of ice invisible
	for (int i = 0; i < VIEW_WIDTH; i++) {
		for (int k = VIEW_HEIGHT - 4; k < VIEW_HEIGHT; k++) {
			m_ice.at(i).at(k)->setVisible(false);
		}
	}

	// Creates middle tunnel
	for (int i = (VIEW_WIDTH / 2) - 2; i < (VIEW_WIDTH / 2) + 2; i++) {
		for (int k = 4; k < VIEW_HEIGHT; k++) {
			m_ice.at(i).at(k)->setVisible(false);
		}
	}
	for (int i = 0; i <= VIEW_HEIGHT - 4; i++) {
		if (i >= 4) {
			emptysquares.push_back(std::make_pair(30, i));
		}
		emptysquares.push_back(std::make_pair(i, 60));
	}

	//Boulder
	int barrel = getLevel() / 2 + 2;
	Boulderspawn = min(barrel, 9);
	i = 0;
	while (i < Boulderspawn) {
		int randX = 30;
		bool made = false;
		
		while (made == false) {
			bool close1 = false;
			while (randX <= ((VIEW_WIDTH / 2) + 2) && randX >= ((VIEW_WIDTH / 2) - 6)) {
				randX = rand() % 60 + 0;
			}
			int randY = rand() % 36 + 20;
			for (auto it : m_actor) {
				int ID = it->getID();
				if ((ID == IID_BARREL || ID == IID_GOLD || ID == IID_BOULDER) && close1 == false) {
					if (it->getRadius2(randX, randY) < 4) {
						close1 = true;
					}
				}
			}
			if (close1 == false) {
				
				//m_boulder.push_back(new Boulder(randX, randY, this));
				m_actor.push_back(std::make_shared<Boulder>(randX, randY, this));
				replaceIce(randX, randY);
				made = true;
			}
		}
		i++;
	}
	
	string Lvl = "Lvl: " + to_string(getLevel());
	string Lives = " Lives: " + to_string(getLives());
	string Hlth = " Hlth: " + to_string((m_iceman->getHP()) * 10) + "%";
	string Wtr = " Wtr: " + to_string(m_iceman->getWtr());
	string Gld = " Gld: " + to_string(m_iceman->getGld());
	string OilLeft = " Oil Left: " + to_string(barrels);//change after make oil class
	string Sonar = " Sonar: " + to_string(m_iceman->getSonar());
	string realscore = to_string(getScore());
	string zeros = "";
	


	for (int i = realscore.size(); i < 6; i++) {
		zeros += "0";
	}
	string score = " Scr: " + zeros + realscore;
	setGameStatText(Lvl + Lives + Hlth + Wtr + Gld + OilLeft + Sonar + score);

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {

	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	string Lvl = "Lvl: " + to_string(getLevel());
	string Lives = " Lives: " + to_string(getLives());
	string Hlth = " Hlth: " + to_string((m_iceman->getHP()) * 10) + "%";
	string Wtr = " Wtr: " + to_string(m_iceman->getWtr());
	string Gld = " Gld: " + to_string(m_iceman->getGld());
	string OilLeft = " Oil Left: " + to_string(barrels);//change after make oil class
	string Sonar1 = " Sonar: " + to_string(m_iceman->getSonar());
	string realscore = to_string(getScore());
	string zeros = "";
	for (int i = realscore.size(); i < 6; i++) {
		zeros += "0";
	}
	string score = " Scr: " + zeros + realscore;
	setGameStatText(Lvl + Lives + Hlth + Wtr + Gld + OilLeft + Sonar1 + score);

	//Goodiespawn;
	int random = rand() % GoodieRate + 1;
	if (random == GoodieRate) {
		int random1 = rand() % 5 + 1;
		if (random1 > 4) {
			//m_sonar.push_back(new Sonar(0, 60, SonarandPoolTick, this));
			m_actor.push_back(std::make_shared<Sonar>(0, 60, SonarandPoolTick, this));
			//emptysquares.erase(std::remove(emptysquares.begin(), emptysquares.end(), std::make_pair(0, 60)), emptysquares.end());
		}
		else {
			int randomx = rand() % emptysquares.size() + 0;

			//m_pool.push_back(new Waterpool(emptysquares.at(randomx).first, emptysquares.at(randomx).second, SonarandPoolTick, this));
			m_actor.push_back(std::make_shared<Waterpool>(emptysquares.at(randomx).first, emptysquares.at(randomx).second, SonarandPoolTick, this));
			//emptysquares.erase(std::remove(emptysquares.begin(), emptysquares.end(), std::make_pair(emptysquares.at(randomx).first, emptysquares.at(randomx).second)), emptysquares.end());
		}
	}

	m_iceman->doSomething();
	if (LastSpawn == 0 && (currentpro <= MaxP)) {
		int chance = 100 / probabilityOfHardcore;
		int random = rand() % chance + 1;
		if (random == chance) {
			//Hprotesters.push_back(new HardPro(WaitRPMove, AnnoyedStunT, HardProM, this));
			m_actor.push_back(std::make_shared<HardPro>(WaitRPMove, tickstare, AnnoyedStunT, HardProM, this));
			currentpro++;
		}
		else {
			//protesters.push_back(new RegPro(WaitRPMove, AnnoyedStunT, this));
			m_actor.push_back(std::make_shared<RegPro>(WaitRPMove, AnnoyedStunT, this));
			currentpro++;
		}
		LastSpawn = SpawnPtime;
	}
	for (auto it : m_actor) {		
		it->doSomething();
		if (!it->isAlive()) {
			if (it->getID() == IID_HARD_CORE_PROTESTER || it->getID() == IID_PROTESTER) {
				currentpro--;
			}
			it.reset();
		}
	}

	/*for (auto it : protesters) {
		it->doSomething();

	}
	for (auto it : Hprotesters) {
		it->doSomething();

	}
	for (auto it : m_gold) {
		it->doSomething();
	}
	for (auto it : m_barrel) {
		it->doSomething();

	}
	for (auto it : m_sonar) {
		it->doSomething();
	}

	for (auto it : m_squirt) {
		it->doSomething();
	}
	for (auto it : m_pool) {
		it->doSomething();
	}
	for (auto it : m_boulder) {
		it->doSomething();
	}*/

	
	if (barrels <= 0) {
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	if (m_iceman->getHP() <= 0) {
		playSound(SOUND_PLAYER_GIVE_UP);
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	else {
		LastSpawn--;
		return GWSTATUS_CONTINUE_GAME;
	}
	LastSpawn--;

}



void StudentWorld::cleanUp() {
	m_ice.clear();
	m_iceman.reset();

	m_iceman = nullptr;
	currentpro = 0;
	for (auto it : m_actor) {
		it.reset();
	}
	m_actor.clear();
	
	emptysquares.clear();
}

void StudentWorld::Sonarused() {
	playSound(SOUND_SONAR);
	for (auto it : m_actor) {
		if (it->getID() == IID_GOLD || it->getID() == IID_BARREL) {
			if (it->isAlive()) {
				if (it->getRadius(m_iceman) <= 12) {
					it->setVisible(true);
				}
			}
		}
	}
	
}
void StudentWorld::Goldused() {
	m_actor.push_back(std::make_shared<GoldNug>(getIcemanX(), getIcemanY(), IID_PLAYER, this));
}

bool StudentWorld::close(GoldNug* gold, int pick, bool forIceman) {
	if (forIceman) {


		if (gold->getRadius(m_iceman) <= pick) {
			return true;
		}
		return false;
	}
	else {
		for (auto it : m_actor) {
			int ID = it->getID();
			if (ID == IID_PROTESTER || ID == IID_HARD_CORE_PROTESTER) {
				if (gold->getRadius(it) <= pick) {
					it->gotbribed();
					if (ID == IID_PROTESTER) {
						increaseScore(25);
					}
					if (ID == IID_HARD_CORE_PROTESTER) {
						increaseScore(50);
					}
					return true;

				}
			}
		}
		
		
	}
	return false;
}
bool StudentWorld::checkforP(int x, int y, Squirt* squirt) {
	bool annoyed = false;
	for (auto it : m_actor){ 
		int ID = it->getID();
		if (ID == IID_PROTESTER || ID == IID_HARD_CORE_PROTESTER) {
			int radi = squirt->getRadius(it);
			if (radi <= 3) {
				if (!it->checkifStuned()) {
					if (it->checkHp() > 2) {
						it->lowerHP(2);
						playSound(SOUND_PROTESTER_ANNOYED);
						annoyed = true;
					}
					else {
						it->lowerHP(2);
						
						if (ID == IID_HARD_CORE_PROTESTER) {
							increaseScore(250);
						}
						else if (ID == IID_PROTESTER) {
							increaseScore(100);
						}

					}
				}

			}
		}
	}
	
	
	if (annoyed) {
		return true;
	}
	return false;
}
bool StudentWorld::IceHere2(int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++)
		{
			if (((x + i) <= VIEW_WIDTH - 4 && (y + k) <= VIEW_HEIGHT - 4)) {
				if ((x + i) >= 0 && (y + k) >= 0) {
					if (m_ice.at(x + i).at(y + k)->isVisible()) {
						return true;
					}
				}
			}
		}
	}
	return false;
}


bool StudentWorld::IceHere3(int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++)
		{
			if (((x + i) > VIEW_WIDTH || (y + k) > VIEW_HEIGHT) || ((x + i) < 0 || (y + k) < 0)) {
			
				return true;
			}
			if (((x + i) < VIEW_WIDTH && (y + k) < VIEW_HEIGHT)) {
				if ((x + i) >= 0 && (y + k) >= 0) {
					if (m_ice.at(x + i).at(y + k)->isVisible()) {
						return true;
					}
				}
			}

		}
	}
	return false;
}


bool StudentWorld::IceHere(int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++)
		{
			if (((x + i) < VIEW_WIDTH && (y + k) < VIEW_HEIGHT)) {
				if ((x + i) >= 0 && (y + k) >= 0) {
					if (m_ice.at(x + i).at(y + k)->isVisible()) {
						return true;
					}
				}
			}
		}
	}
	return false;
}



bool StudentWorld::IceHere1(int x, int y, GraphObject::Direction dir) {
	if (dir == GraphObject::up) {
		if ((y + 4) >= VIEW_HEIGHT) {
			return true;
		}
	}
	else if (dir == GraphObject::down) {
		if ((y - 4) <= 0) {
			return true;
		}
	}
	else if (dir == GraphObject::right) {
		if ((x + 4) >= VIEW_WIDTH) {
			return true;
		}
	}
	else if (dir == GraphObject::left) {
		if ((y - 4) <= 0) {
			return true;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++)
		{
			if ((((x + i) <= VIEW_WIDTH - 4) && ((y + k) <= VIEW_HEIGHT - 4))) {
				if ((x + i) >= 0 && (y + k) >= 0) {
					if (m_ice.at(x + i).at(y + k)->isVisible()) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

//	>> Where You left off <<--------------------------------------------------------O

bool StudentWorld::IcemanVorHofRP(int x, int y, RegPro* bot) {
	int icemanx = m_iceman->getX();
	int icemany = m_iceman->getY();
	if (icemany == y) {
		if (icemanx > x) {
			for (int i = x + 1; i < icemanx; i++) {
				if (IceHere(i, y)) {
					return false;
				}
			}
			return true;
		}
		else if (icemanx < x) {
			for (int i = x - 1; i > icemanx; i--) {
				if (IceHere(i, y)) {
					return false;
				}
			}
			return true;
		}
	}
	else if (icemanx == x) {
		if (icemany > y) {
			for (int i = y + 1; i < icemany; i++) {
				if (IceHere(x, i)) {
					return false;
				}
			}
			return true;
		}
		else if (icemany < y) {
			for (int i = y - 1; i >icemany; i--) {
				if (IceHere(x, i)) {
					return false;
				}
			}
			return true;
		}
	}
	return false;
}

//	>> Where You left off <<--------------------------------------------------------O
///___________________________________________________________________________________

bool StudentWorld::IcemanVorHofRP(int x, int y, HardPro* bot) {
	int icemanx = m_iceman->getX();
	int icemany = m_iceman->getY();
	if (icemany == y) {
		if (icemanx > x) {
			for (int i = x + 1; i < icemanx; i++) {
				if (IceHere(i, y)) {
					return false;
				}
			}
			return true;
		}
		else if (icemanx < x) {
			for (int i = x - 1; i > icemanx; i--) {
				if (IceHere(i, y)) {
					return false;
				}
			}
			return true;
		}
	}
	else if (icemanx == x) {
		if (icemany > y) {
			for (int i = y + 1; i < icemany; i++) {
				if (IceHere(x, i)) {
					return false;
				}
			}
			return true;
		}
		else if (icemany < y) {
			for (int i = y - 1; i > icemany; i--) {
				if (IceHere(x, i)) {
					return false;
				}
			}
			return true;
		}
	}
	return false;
}



bool StudentWorld::NearIceMan(int x, int y, RegPro* bot) {
	int m_x = m_iceman->getX();
	int m_y = m_iceman->getY();
	/*if (bot->getRadius(m_iceman) <= 4) {
	if ((m_x > x) && (bot->getDirection() == GraphObject::right) && (y >= m_y + 4 && y <= m_y - 4)) {
	return true;
	}
	if ((m_x < x) && (bot->getDirection() == GraphObject::left) && (y >= m_y + 4 && y <= m_y - 4)) {
	return true;
	}
	if ((m_y > y) && (bot->getDirection() == GraphObject::up) && (x >= m_x + 4 && x <= m_x - 4)) {
	return true;
	}
	if ((m_y < y) && (bot->getDirection() == GraphObject::down) && (x >= m_x + 4 && x <= m_x - 4)) {
	return true;
	}
	}*/
	for (int i = 0; i <= 4; i++) {
		if ((m_y == (y + i)) && (bot->getDirection() == GraphObject::up)) {
			for (int k = 0; k < 4; k++) {
				if (m_x == x + k || m_x == x - k) {
					return true;
				}
			}
		}
		else if ((m_y == (y - i)) && (bot->getDirection() == GraphObject::down)) {
			for (int k = 0; k < 4; k++) {
				if (m_x == x + k || m_x == x - k) {
					return true;
				}
			}
		}
		else if ((m_x == (x + i)) && (bot->getDirection() == GraphObject::right)) {
			for (int k = 0; k < 4; k++) {
				if (m_y == y + k || m_y == y - k) {
					return true;
				}
			}
		}
		else if ((m_x == (x - i)) && (bot->getDirection() == GraphObject::left)) {
			for (int k = 0; k < 4; k++) {
				if (m_y == y + k || m_y == y - k) {
					return true;
				}
			}
		}
	}
	return false;
}
bool StudentWorld::NearIceMan(int x, int y, HardPro* bot) {
	int m_x = m_iceman->getX();
	int m_y = m_iceman->getY();
	for (int i = 0; i < 4; i++) {
		if ((m_y == (y + i)) && (bot->getDirection() == GraphObject::up)) {
			for (int k = 0; k <= 4; k++) {
				if (m_x == x + k || m_x == x - k) {
					return true;
				}
			}
		}
		else if ((m_y == (y - i)) && (bot->getDirection() == GraphObject::down)) {
			for (int k = 0; k < 4; k++) {
				if (m_x == x + k || m_x == x - k) {
					return true;
				}
			}
		}
		else if ((m_x == (x + i)) && (bot->getDirection() == GraphObject::right)) {
			for (int k = 0; k < 4; k++) {
				if (m_y == y + k || m_y == y - k) {
					return true;
				}
			}
		}
		else if ((m_x == (x - i)) && (bot->getDirection() == GraphObject::left)) {
			for (int k = 0; k < 4; k++) {
				if (m_y == y + k || m_y == y - k) {
					return true;
				}
			}
		}
	}
	return false;
}
void StudentWorld::replaceIce(int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++)
		{
			if (m_ice.at(x + i).at(y + k)->isVisible()) {
				m_ice.at(x + i).at(y + k)->setVisible(false);
			}
		}
	}
}


void StudentWorld::breakIce(int x, int y) {
	bool dug = false;
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++)
		{
			if (m_ice.at(x + i).at(y + k)->isVisible()) {
				m_ice.at(x + i).at(y + k)->setVisible(false);
				emptysquares.push_back(std::make_pair(x, y));
				
				dug = true;
			}
		}
	}
	if (dug) {
		playSound(SOUND_DIG);
	}
}

bool StudentWorld::AtInter(int x, int y, RegPro* bot) {
	if (bot->getperp() > 0) {
		return false;
	}

	if ((bot->getDirection() == GraphObject::up && x < VIEW_HEIGHT - 4) || (bot->getDirection() == GraphObject::down && x>0)) {

		if (!IceHere1(x + 1, y, GraphObject::right) && !IceHere1(x - 1, y, GraphObject::left)) {
			int randN = rand() % 2 + 1;
			switch (randN) {
			case 1:
				bot->setDirection(GraphObject::right);

				return true;
				break;
			case 2:
				bot->setDirection(GraphObject::left);

				return true;
				break;
			}
		}
		else if (!IceHere1(x - 1, y, GraphObject::left)) {
			bot->setDirection(GraphObject::left);

			return true;
		}
		else if (!IceHere1(x + 1, y, GraphObject::right)) {
			bot->setDirection(GraphObject::right);

			return true;
		}
	}
	else if ((bot->getDirection() == GraphObject::left && y > 0) || (bot->getDirection() == GraphObject::right && y < VIEW_WIDTH - 4)) {
		if (!IceHere1(x, y + 1, GraphObject::up) && !IceHere1(x, y - 1, GraphObject::down)) {

			int randN = rand() % 2 + 1;
			switch (randN) {
			case 1:
				bot->setDirection(GraphObject::up);

				return true;
				break;
			case 2:
				bot->setDirection(GraphObject::down);

				return true;
				break;
			}
		}
		else if (!IceHere1(x, y + 1, GraphObject::up)) {

			bot->setDirection(GraphObject::up);

			return true;
		}
		else if (!IceHere1(x, y - 1, GraphObject::down)) {

			bot->setDirection(GraphObject::down);

			return true;
		}
	}
	return false;
}

bool StudentWorld::AtInter1(int x, int y, RegPro* bot) {
	if ((bot->getDirection() == GraphObject::up && x < VIEW_HEIGHT - 4) || (bot->getDirection() == GraphObject::down && x>0)) {

		if (!IceHere1(x + 1, y, GraphObject::right) && !IceHere1(x - 1, y, GraphObject::left)) {
			return true;
		}
		else if (!IceHere1(x - 1, y, GraphObject::left)) {
			return true;
		}
		else if (!IceHere1(x + 1, y, GraphObject::right)) {
			return true;
		}
	}
	else if ((bot->getDirection() == GraphObject::left && y > 0) || (bot->getDirection() == GraphObject::right && y < VIEW_WIDTH - 4)) {
		if (!IceHere1(x, y + 1, GraphObject::up) && !IceHere1(x, y - 1, GraphObject::down)) {
			return true;
		}
		else if (!IceHere1(x, y + 1, GraphObject::up)) {
			return true;
		}
		else if (!IceHere1(x, y - 1, GraphObject::down)) {
			return true;
		}
	}
	return false;
}
bool StudentWorld::AtInter(int x, int y, HardPro* bot) {
	if (bot->getperp() > 0) {
		return false;
	}

	if ((bot->getDirection() == GraphObject::up && x < VIEW_HEIGHT - 4) || (bot->getDirection() == GraphObject::down && x>0)) {

		if (!IceHere1(x + 1, y, GraphObject::right) && !IceHere1(x - 1, y, GraphObject::left)) {
			int randN = rand() % 2 + 1;
			switch (randN) {
			case 1:
				bot->setDirection(GraphObject::right);

				return true;
				break;
			case 2:
				bot->setDirection(GraphObject::left);

				return true;
				break;
			}
		}
		else if (!IceHere1(x - 1, y, GraphObject::left)) {
			bot->setDirection(GraphObject::left);

			return true;
		}
		else if (!IceHere1(x + 1, y, GraphObject::right)) {
			bot->setDirection(GraphObject::right);

			return true;
		}
	}
	else if ((bot->getDirection() == GraphObject::left && y > 0) || (bot->getDirection() == GraphObject::right && y < VIEW_WIDTH - 4)) {
		if (!IceHere1(x, y + 1, GraphObject::up) && !IceHere1(x, y - 1, GraphObject::down)) {

			int randN = rand() % 2 + 1;
			switch (randN) {
			case 1:
				bot->setDirection(GraphObject::up);

				return true;
				break;
			case 2:
				bot->setDirection(GraphObject::down);

				return true;
				break;
			}
		}
		else if (!IceHere1(x, y + 1, GraphObject::up)) {

			bot->setDirection(GraphObject::up);

			return true;
		}
		else if (!IceHere1(x, y - 1, GraphObject::down)) {

			bot->setDirection(GraphObject::down);

			return true;
		}
	}
	return false;
}

bool StudentWorld::AtInter1(int x, int y, HardPro* bot) {
	if ((bot->getDirection() == GraphObject::up && x < VIEW_HEIGHT - 4) || (bot->getDirection() == GraphObject::down && x>0)) {

		if (!IceHere1(x + 1, y, GraphObject::right) && !IceHere1(x - 1, y, GraphObject::left)) {
			return true;
		}
		else if (!IceHere1(x - 1, y, GraphObject::left)) {
			return true;
		}
		else if (!IceHere1(x + 1, y, GraphObject::right)) {
			return true;
		}
	}
	else if ((bot->getDirection() == GraphObject::left && y > 0) || (bot->getDirection() == GraphObject::right && y < VIEW_WIDTH - 4)) {
		if (!IceHere1(x, y + 1, GraphObject::up) && !IceHere1(x, y - 1, GraphObject::down)) {
			return true;
		}
		else if (!IceHere1(x, y + 1, GraphObject::up)) {
			return true;
		}
		else if (!IceHere1(x, y - 1, GraphObject::down)) {
			return true;
		}
	}
	return false;
}
void StudentWorld::makeSquirt(int x, int y, GraphObject::Direction dir) {

	//m_squirt.push_back(new Squirt(x, y, dir, this));
	m_actor.push_back(std::make_shared<Squirt>(x, y, dir, this));
	playSound(SOUND_PLAYER_SQUIRT);

}

bool StudentWorld::Icebelow(int x, int y) {
	for (int i = 0; i < 4; i++) {
		if ((((x + i) < VIEW_WIDTH - 4) && (y  < VIEW_HEIGHT - 4))) {
			if ((x + i) >= 0 && y >= 0) {
				if (m_ice.at(x + i).at(y)->isVisible()) {
					return true;
				}
			}
		}
	}
	return false;
}

bool StudentWorld::CheckBolBelow(int x, int y) {
	for (auto it : m_actor){
		if (it->getID() == IID_BOULDER) {
			if (y == it->getY() + 3) {
				for (int i = 0; i < 4; i++) {
					if (x == (it->getX() + i)) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool StudentWorld::HitPlayer(Boulder* rock) {
	if (rock->getRadius(m_iceman) <= 3) {
		m_iceman->DecHP(100);
		return true;
	}
	for (auto it : m_actor) {
		int ID = it->getID();
		if (ID == IID_HARD_CORE_PROTESTER || ID == IID_PROTESTER) {
			if (rock->getRadius(it) <= 3) {
				it->lowerHP(100);				
				increaseScore(500);
			}
		}
	}
	
	return false;
}
bool StudentWorld::boulderInWay(int x, int y) {	
	for (auto it : m_actor) {
		if (it->getID() == IID_BOULDER) {
			if (it->isAlive()) {
				int xb = it->getX();
				int yb = it->getY();

				for (int i = 0; i < 4; i++) {
					for (int k = 0; k < 4; k++)
					{
						if ((yb + i) == y || (yb + i) == y + 1 || (yb + i) == y + 2 || (yb + i) == y + 3) {
							if ((xb + k) == x || (xb + k) == x + 1 || (xb + k) == x + 2 || (xb + k) == x + 3) {
								return true;
							}
						}
					}
				}


			}
		}

	}
	return false;
}

void StudentWorld::addemptyspace(int x, int y) {
	emptysquares.push_back(std::make_pair(x, y));
}

bool StudentWorld::closeIceman(Sonar* pointer) {
	if (pointer->getRadius(m_iceman) <= 3) {
		return true;
	}
	return false;
}
bool StudentWorld::closeIceman(Waterpool* pointer) {
	if (pointer->getRadius(m_iceman) <= 3) {
		return true;
	}
	return false;
}
bool StudentWorld::closeIceman(Barrel* pointer, int radius) {
	if (pointer->getRadius(m_iceman) <= radius) {
		return true;
	}
	return false;
}
bool StudentWorld::isSpaceEmpty(pair<int, int> target) {
	
	//return (std::find(emptysquares.begin(), emptysquares.end(), target) != emptysquares.end());
	for (auto k : emptysquares)
		if (k == target)
			return true;
	return false;
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp