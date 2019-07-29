#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "Actor.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <memory>
#include <vector>
#include <utility>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Ice;

class StudentWorld : public GameWorld {

public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir) {}
	~StudentWorld();

	virtual int init();
	virtual int move();
	virtual void cleanUp();

	StudentWorld* GetWorld() { return this; }
	void breakIce(int x, int y);
	bool IceHere(int x, int y);
	bool IceHere2(int x, int y);
	bool IceHere3(int x, int y);
	bool IceHere1(int x, int y, GraphObject::Direction dir);
	bool NearIceMan(int x, int y, RegPro* bot);
	bool NearIceMan(int x, int y, HardPro* bot);
	void DecHP(int hp) { m_iceman->DecHP(hp); }
	bool IcemanVorHofRP(int x, int y, RegPro* bot);
	bool IcemanVorHofRP(int x, int y, HardPro* bot);
	int getIcemanY() { return m_iceman->getY(); }
	int getIcemanX() { return m_iceman->getX(); }
	bool AtInter(int x, int y, RegPro* bot);
	bool AtInter1(int x, int y, RegPro* bot);
	bool AtInter(int x, int y, HardPro* bot);
	bool AtInter1(int x, int y, HardPro* bot);
	void makeSquirt(int x, int y, GraphObject::Direction dir);
	bool checkforP(int x, int y, Squirt* squirt);
	bool close(GoldNug* gold, int pick, bool forIceman);
	bool HitPlayer(Boulder* gold);
	void Sonarused();
	void Goldused();
	std::shared_ptr<Iceman> getIceman() { return m_iceman; }
	bool Icebelow(int x, int y);
	bool CheckBolBelow(int x, int y);
	void replaceIce(int x, int y);
	bool boulderInWay(int x, int y);
	void addemptyspace(int x, int y);
	bool closeIceman(Sonar* pointer);
	bool closeIceman(Waterpool* pointer);
	bool closeIceman(Barrel* pointer, int radius);
	void barrelfound() { barrels--; }
	void prodied() { currentpro--; }
	bool isSpaceEmpty(std::pair<int, int> target);

private:
	std::shared_ptr<Iceman> m_iceman;
	
	std::vector<std::shared_ptr<Actor>>m_actor;
	std::vector<std::vector<std::shared_ptr<Ice>>> m_ice;
	std::vector<std::pair <int, int>> emptysquares;
	int MaxP;
	int SpawnPtime;
	int LastSpawn;
	int WaitRPMove;
	int AnnoyedStunT;
	int HardProM;
	int probabilityOfHardcore;
	int MaxG;
	int SonarandPoolTick;
	int GoodieRate;
	int Boulderspawn;
	int Barrelspawn;
	int currentpro = 0;
	int barrels;
	int tickstare;
	
};

#endif // STUDENTWORLD_H_