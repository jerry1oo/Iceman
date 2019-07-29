#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <memory>
#include <algorithm>
#include <vector>
#include <queue>
#include <cassert>
using namespace std;


class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp



struct Step {

	GraphObject::Direction direct;
	std::pair<int, int> point;
	Step(GraphObject::Direction d, std::pair<int, int> p) : direct(d), point(p) {}
	Step(GraphObject::Direction d, int x, int y) : direct(d), point(x, y) {}
};


class Path {

private:
	std::vector<Step> m_steps;

public:
	//	Constructors
	///	Path(Step&& s) { m_steps.push_back(std::move(s)); }
	Path(Step s) { m_steps.push_back(s); }
	Path(GraphObject::Direction d, std::pair<int, int> p) {
		m_steps.emplace_back(Step(d, p));
	}
	Path(GraphObject::Direction d, int x, int y) {
		m_steps.emplace_back(Step(d, x, y));
	}
	//	Copy Constructor
	Path(const Path& rhs) {
		this->m_steps = rhs.m_steps;
	}
	//	Move Constructor
	Path(Path&& rhs) {
		this->m_steps = std::move(rhs.m_steps);
	}
	//	Assignment Operator
	Path operator=(Path& rhs) {
		if (!this->m_steps.empty())
			this->m_steps.clear();
		Path temp(rhs);
		this->m_steps.swap(temp.m_steps);
		return (*this);
	}
	//	Destructor
	~Path() {
		if (!this->m_steps.empty())
			this->m_steps.clear();
	}
	//	Add Instructions()
	void addInstr(Step s) { m_steps.push_back(s); }
	void addInstr(GraphObject::Direction d, std::pair<int, int> p) {
		m_steps.emplace_back(Step(d, p));
	}
	void addInstr(GraphObject::Direction d, int x, int y) {
		m_steps.emplace_back(Step(d, x, y));
	}
	//	Other functions
	bool newStep(const Step& s);
	const Step viewFirst() const { return m_steps.front(); }
	const Step viewLast() const { return m_steps.back(); }
	const Step viewAt(int i) const { return m_steps.at(i); }
	const int findSize() const { return m_steps.size(); }
	const bool isEmpty() const { return m_steps.empty(); }
	//Path operator+(const Path& rhs);
};






class Actor : public GraphObject {

private:
	StudentWorld* m_game;
	int ID;
	bool Alive;
	
	
public:
	Actor(int ID, int x, int y, Direction dir, double size, unsigned int depth) : GraphObject(ID, x, y, dir, size, depth) {}
	~Actor() {}
	virtual void doSomething() = 0;
	double getRadius(std::shared_ptr<GraphObject> other) {
		double xDist = abs(this->getX() - other->getX());
		double yDist = abs(this->getY() - other->getY());
		return sqrt((xDist *xDist) + (yDist * yDist));
	}
	double getRadius2(int x,int y) {
		double xDist = abs(this->getX() - x);
		double yDist = abs(this->getY() - y);
		return sqrt((xDist *xDist) + (yDist * yDist));
	}
	virtual double getRadius1(GraphObject* other) { return -1; }
	
	Path findShortestPath(int targX, int targY);
	virtual int getID() = 0;
	virtual bool isAlive() { return false; }
	virtual void gotbribed(){}
	virtual bool checkifStuned() { return false; }
	virtual void lowerHP(int hp) {}
	virtual int checkHp() { return -1; }
	
	
};

class Ice : public Actor {
	int ID = IID_ICE;
public:
	Ice(int x, int y) : Actor(IID_ICE, x, y, right, .25, 3) {
		this->setVisible(true);
	}
	~Ice() {}
	virtual void doSomething() override {}
	virtual int getID() override { return ID; }
};

class RegPro : public Actor {
	int ID = IID_PROTESTER;
	StudentWorld* game1;
	bool Leave = false;
	int rest;
	int wait;
	int yell;
	int Hp = 5;
	bool Alive = true;
	int numSquaresToMoveInCurrentDirection;
	bool see = false;
	int perp;
	bool perp1;
	bool stuned = false;
	int AnnoyedStunT;
	bool bribed = false;
	bool done;
	queue<Step> stepsDeath;
public:
	RegPro(int ticks, int stunT, StudentWorld* gameset) : Actor(IID_PROTESTER, 60, 60, left, 1.0, 0) {

		setVisible(true);
		wait = 0;
		rest = ticks;
		yell = 0;
		perp = 200;
		perp1 = false;
		game1 = gameset;
		AnnoyedStunT = stunT;
		bool bribed = false;
		numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
		done = false;
	}
	virtual int getID() override { return ID; }
	virtual void doSomething() override;
	bool checkifStuned() override { return stuned; }
	int getperp() { return perp; }
	int checkwait() { return wait; }
	void lowerwait() { wait--; }
	void resetwait() { wait = rest; }
	void lowerHP(int hp) override { Hp = Hp - hp; stuned = true; if (Hp > 0) { wait = AnnoyedStunT; }; }
	void resetperp() { perp = 200; }
	int checkHp() override { return Hp; }
	void setDead() { Alive = false; }
	void gotbribed() override { bribed = true; }
	virtual bool isAlive() override { return Alive; }
	/*double getRadius1(Squirt* other) override {
		double xDist = abs(this->getX() - other->getX());
		double yDist = abs(this->getY() - other->getY());
		return sqrt((xDist *xDist) + (yDist * yDist));
	}*/
};
class HardPro : public Actor {
	int ID = IID_HARD_CORE_PROTESTER;
	StudentWorld* game1;
	bool Leave = false;
	int rest;
	int wait;
	int yell;
	int Hp = 20;
	bool Alive = true;
	int numSquaresToMoveInCurrentDirection;
	bool see = false;
	int perp;
	bool perp1;
	bool stuned = false;
	int AnnoyedStunT;
	int HardProM;
	bool bribed = false;
	int stun;
	bool done;
	queue<Step> stepsDeath;
	queue<Step> stepsIceman;
public:
	HardPro(int ticks, int a, int stunT, int HPM, StudentWorld* gameset) : Actor(IID_HARD_CORE_PROTESTER, 60, 60, left, 1.0, 0) {

		setVisible(true);
		wait = 0;
		stun = a;
		rest = ticks;
		yell = 0;
		perp = 200;
		perp1 = false;
		game1 = gameset;
		AnnoyedStunT = stunT;
		HardProM = HPM;
		numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
		done = false;
	}
	virtual int getID() override { return ID; }
	virtual void doSomething() override;
	bool checkifStuned() override { return stuned; }
	int getperp() { return perp; }
	int checkwait() { return wait; }
	void lowerwait() { wait--; }
	void resetwait() { wait = rest; }
	void lowerHP(int hp) override { Hp = Hp - hp; stuned = true; if (Hp > 0) { wait = AnnoyedStunT; };	}
	void resetperp() { perp = 200; }
	int checkHp() override { return Hp; }
	void setDead() { Alive = false; }
	void gotbribed() override { bribed = true; }
	virtual bool isAlive() override { return Alive; }
	/*double getRadius1(Squirt* other) {
		double xDist = abs(this->getX() - other->getX());
		double yDist = abs(this->getY() - other->getY());
		return sqrt((xDist *xDist) + (yDist * yDist));
	}*/
};

class Squirt : public Actor {
private:
	int ID = IID_WATER_SPURT;
	bool Alive;
	StudentWorld* game1;
	int Hp = 3;

public:
	Squirt(int x, int y, Direction Dir, StudentWorld* game) : Actor(IID_WATER_SPURT, x, y, Dir, 1.0, 1) {
		setVisible(true);
		game1 = game;
		Alive = true;
	}

	~Squirt() { this->setVisible(false); }
	virtual int getID() override { return ID; }
	virtual void doSomething() override;
	void SetDead() { Alive = true; }
	virtual bool isAlive() override { return Alive; }


};


class Iceman : public Actor {

private:
	int ID = IID_PLAYER;
	StudentWorld* game;
	int m_hp;
	int m_water = 3;
	int m_sonar = 0;
	int m_gold = 0;
public:
	Iceman(StudentWorld* gameset)
		: Actor(IID_PLAYER, 30, 60, right, 1.0, 0), m_hp(10), m_water(5), m_sonar(1), m_gold(0), game(gameset) {
		this->setVisible(true);
	}
	~Iceman() {}
	virtual int getID() override { return ID; }
	virtual void doSomething() override;
	int getHP() { return m_hp; }
	int getWtr() { return m_water; }
	int getGld() { return m_gold; }
	int getSonar() { return m_sonar; }
	void DecHP(int hp);
	Iceman* getIceman() { return this; }
	void gotGold() { m_gold++; }
	void gotSonar() { m_sonar++; }
	void gotWater() { m_water += 3; }



};

class GoldNug : public Actor {
	int ID = IID_GOLD;
	StudentWorld* game;
	bool ForIceman;
	bool ForPro;
	bool perm;
	bool Alive = true;
	int wait;
	int bribed;
public:
	GoldNug(int x, int y, int who, StudentWorld* game1) : Actor(IID_GOLD, x, y, right, 1.0, 2) {
		game = game1;
		if (who == IID_PLAYER) {
			setVisible(true);
			ForPro = true;
			ForIceman = false;
			perm = false;
			wait = 100;
		}
		else {
			setVisible(false);
			ForIceman = true;
			ForPro = false;
			perm = true;

		}
	}
	virtual int getID() override { return ID; }
	virtual void doSomething() override;
	
	virtual bool isAlive() override { return Alive; }
	void gotbribed() override { bribed = true; }
};

class Sonar : public Actor {
	int ID = IID_SONAR;
	StudentWorld* game;
	bool Alive = true;
	int wait;
public:
	Sonar(int x, int y, int wait1, StudentWorld* game1) : Actor(IID_SONAR, x, y, right, 1.0, 2) {
		setVisible(true);
		wait = wait1;
		game = game1;
	}
	virtual int getID() override { return ID; }
	virtual void doSomething() override;
	
	virtual bool isAlive() override { return Alive; }
};

class Waterpool : public Actor {
	int ID = IID_WATER_POOL;
	StudentWorld* game;
	bool Alive = true;
	int wait;
public:
	Waterpool(int x, int y, int wait1, StudentWorld* game1) : Actor(IID_WATER_POOL, x, y, right, 1.0, 2) {
		setVisible(true);
		wait = wait1;
		game = game1;
	}
	virtual int getID() override { return ID; }
	virtual void doSomething() override;
	
	virtual bool isAlive() override { return Alive; }
};

class Boulder : public Actor {
	int ID = IID_BOULDER;
	StudentWorld* game;
	bool Alive;
	bool stable = true;
	bool falling = false;
	int wait = 30;
public:
	Boulder(int x, int y, StudentWorld* game1) : Actor(IID_BOULDER, x, y, down, 1.0, 1) {
		setVisible(true);
		game = game1;
		Alive = true;
	}
	virtual int getID() override { return ID; }
	virtual void doSomething() override;
	
	
	virtual bool isAlive() override { return Alive; }

};


class Barrel : public Actor {

private:
	int ID = IID_BARREL;
	StudentWorld* m_game;
	Iceman* m_iceMan;
	bool Alive = true;

public:
	Barrel(int x, int y, StudentWorld* game1) : Actor(IID_BARREL, x, y, right, 1.0, 2) {
		this->setVisible(false);
		m_game = game1;
	}
	~Barrel() {}
	virtual int getID() override { return ID; }
	virtual void doSomething() override;
	virtual bool isAlive() override { return Alive; }
	
};




#endif // ACTOR_H_