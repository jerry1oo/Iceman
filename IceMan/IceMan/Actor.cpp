#include "Actor.h"
#include "StudentWorld.h"
#include<algorithm>
using namespace std;

bool Path::newStep(const Step& s) {

	pair<int, int> pt(s.point.first, s.point.second);
	for (auto history : m_steps) {
		if (pt == history.point)
			return false;
	}
	return true;
}


Path Actor::findShortestPath(int targX, int targY) {

	vector<shared_ptr<Path>> paths;
	StudentWorld* map = m_game->GetWorld();

	// Initial path check
	Step origin(none, getX(), getY());
	Step iU(up, getX(), getY() + 1);
	Step iR(right, getX() + 1, getY());
	Step iD(down, getX(), getY() - 1);
	Step iL(left, getX() - 1, getY());

	// Check up
	if (iU.point.second <= 60 && map->isSpaceEmpty(iU.point)) {
		paths.push_back(make_shared<Path>(origin));
		paths.back()->addInstr(iU);
	}
	// Check right
	if (iR.point.first <= 60 && map->isSpaceEmpty(iR.point)) {
		paths.push_back(make_shared<Path>(origin));
		paths.back()->addInstr(iR);
	}
	// Check down
	if (iD.point.second >= 0 && map->isSpaceEmpty(iD.point)) {
		paths.push_back(make_shared<Path>(origin));
		paths.back()->addInstr(iD);
	}
	// Check left
	if (iL.point.first >= 0 && map->isSpaceEmpty(iL.point)) {
		paths.push_back(make_shared<Path>(origin));
		paths.back()->addInstr(iL);
	}
	// This should never happen
	if (paths.empty()) {
		Path stuck(std::move(origin));
		return stuck;
	}
	// Path checker/maker
	for (;;) {

		vector<shared_ptr<Path>>::iterator itrP = paths.begin();
		vector<shared_ptr<Path>>::iterator endP = paths.end();

		// Inifinite loop catcher
		if (itrP == endP)
			assert(false);

		while (itrP != endP) {
			queue<Step> steps;
			int pathX = (*itrP)->viewLast().point.first;
			int pathY = (*itrP)->viewLast().point.second;

			// Finding the tunnel

			Step u(up, pathX, pathY + 1);
			Step r(right, pathX + 1, pathY);
			Step d(down, pathX, pathY - 1);
			Step l(left, pathX - 1, pathY);
			// Check up
			if (u.point.second <= 60 && map->isSpaceEmpty(u.point) && (*itrP)->newStep(u))
				steps.push(u);
			// Check right
			if (r.point.first <= 60 && map->isSpaceEmpty(r.point) && (*itrP)->newStep(r))
				steps.push(r);
			// Check down
			if (d.point.second >= 0 && map->isSpaceEmpty(d.point) && (*itrP)->newStep(d))
				steps.push(d);
			// Check left
			if (l.point.first >= 0 && map->isSpaceEmpty(l.point) && (*itrP)->newStep(l))
				steps.push(l);
			// No possible steps (Dead end)
			if (steps.empty())
				itrP = paths.erase(itrP);
			else {
				// One possible step
				if (steps.size() == 1) {
					(*itrP)->addInstr(steps.front());
					itrP++;
				}
				// Multiple possible steps
				else if (steps.size() > 1) {
					int pos = 0;
					// Iterator position tracker
					while (paths.begin() + pos != paths.end()) {
						if (paths.begin() + pos == itrP)
							break;
						pos++;
					}
					// Path splitter
					vector<shared_ptr<Path>> newPaths;
					while (steps.size() > 1) {
						Path temp = **itrP;
						temp.addInstr(steps.front());
						steps.pop();
						newPaths.emplace_back(make_shared<Path>(std::move(temp)));
					}
					// Puts newPaths into paths
					(*itrP)->addInstr(steps.front());
					steps.pop();
					for (auto p : newPaths)
						paths.emplace_back(std::move(p));
					// Repositions itrP
					itrP = paths.begin() + pos + 1;
				}
				// Test: this else statment should never be executed
				else
					assert(false);
			}
			endP = paths.end();
			// Checks all paths if any made it to the finish
			pair<int, int> goal(targX, targY);
			vector<shared_ptr<Path>> finalPaths;
			// Collects all paths that work
			for (auto k : paths)
				if (k->viewLast().point == goal)
					finalPaths.push_back(k);
			if (finalPaths.empty())
				continue;
			else {
				int numSteps = finalPaths.front()->findSize();
				Path shortestPth = *finalPaths.front();
				// Determines which final path is shortest
				for (auto j : finalPaths)
					if (j->findSize() < numSteps) {
						shortestPth = *j;
						numSteps = j->findSize();
					}
				return shortestPth;
			}
		}
	}
}




void Iceman::DecHP(int hp) {
	m_hp = m_hp - hp;

}
// Iceman member functions/methods
void Iceman::doSomething() {

	int keyPress = 0;
	if (m_hp <= 0) {
		return;
	}

	if (game->GetWorld()->getKey(keyPress) == true) {

		switch (keyPress) {
		case KEY_PRESS_DOWN:
			// Look down
			if (getDirection() != down) {
				setDirection(down);
			}
			// Move down
			else if (getY() > 0 && !game->GetWorld()->boulderInWay(getX(), getY() - 1)) {
				moveTo(getX(), getY() - 1);
				game->GetWorld()->breakIce(getX(), getY());
			}
			// Animate
			else {
				moveTo(getX(), getY());
			}
			break;
		case KEY_PRESS_UP:
			// Look up
			if (getDirection() != up) {
				setDirection(up);
			}
			// Move up
			else if ((getY() < VIEW_HEIGHT - 4) && !game->GetWorld()->boulderInWay(getX(), getY() + 1)) {
				moveTo(getX(), getY() + 1);
				game->GetWorld()->breakIce(getX(), getY());
			}
			// Animate
			else {
				moveTo(getX(), getY());
			}
			break;
		case KEY_PRESS_LEFT:
			// Look left
			if (getDirection() != left) {
				setDirection(left);
			}
			// Move left
			else if (getX() > 0 && !game->GetWorld()->boulderInWay(getX() - 1, getY())) {
				moveTo(getX() - 1, getY());
				game->GetWorld()->breakIce(getX(), getY());
			}
			// Animate
			else {
				moveTo(getX(), getY());
			}
			break;
		case KEY_PRESS_RIGHT:
			// Look right
			if (getDirection() != right) {
				setDirection(right);
			}
			// Move right
			else if (getX() < VIEW_WIDTH - 4 && !game->GetWorld()->boulderInWay(getX() + 1, getY())) {
				moveTo(getX() + 1, getY());
				game->GetWorld()->breakIce(getX(), getY());
			}
			// Animate
			else {
				moveTo(getX(), getY());
			}
			break;
		case KEY_PRESS_SPACE:
			if (getWtr() > 0) {
				if (getDirection() == up && getY() <= VIEW_HEIGHT - 8) {
					game->GetWorld()->makeSquirt(getX(), getY() + 4, getDirection());
					m_water--;
				}
				else if (getDirection() == down && getY() > 4) {
					game->GetWorld()->makeSquirt(getX(), getY() - 4, getDirection());
					m_water--;
				}
				else if (getDirection() == right && getX() <= VIEW_WIDTH - 8) {
					game->GetWorld()->makeSquirt(getX() + 4, getY(), getDirection());
					m_water--;
				}
				else if (getDirection() == left && getX() > 4) {
					game->GetWorld()->makeSquirt(getX() - 4, getY(), getDirection());
					m_water--;
				}

			}
			break;
		case KEY_PRESS_ESCAPE:
			m_hp = 0;
			break;
		case 'z':
		case 'Z':
			if (getSonar() > 0) {
				m_sonar--;
				game->GetWorld()->Sonarused();
			}
			break;
		case KEY_PRESS_TAB:
			if (getGld() > 0) {
				m_gold--;
				game->GetWorld()->Goldused();
			}
		}

	}
}

void Squirt::doSomething() {
	if (Alive == false) {
		setVisible(false);
		return;
	}
	if (game1->GetWorld()->IceHere3(getX(), getY())) {
		
		setVisible(false);
		Alive = false;
		Hp = 0;

	}

	else if (game1->GetWorld()->checkforP(getX(), getY(), this)) {
		Alive = false;
		
		return;
	}

	else if (getDirection() == up && getY() < VIEW_HEIGHT) {
		if (game1->GetWorld()->IceHere3(getX(), getY() + 1)) {
			
			Alive = false;
		}
		else {
					
			moveTo(getX(), getY() + 1);
		}
		Hp--;
	}
	else if (getDirection() == down && getY() >= 0) {
		if (game1->GetWorld()->IceHere3(getX(), getY() - 1)) {
			
			Alive = false;
		}
		else {
			
			moveTo(getX(), getY() - 1);
		}
		Hp--;
	}
	else if (getDirection() == right && getX() < VIEW_WIDTH) {
		if (game1->GetWorld()->IceHere3(getX() + 1, getY())) {
			
			Alive = false;
		}
		else {
			
			moveTo(getX() + 1, getY());
		}
		Hp--;
	}
	else if (getDirection() == left && getX() >= 0) {
		if (game1->GetWorld()->IceHere3(getX() - 1, getY())) {
			
			Alive = false;
		}
		else {
			
			moveTo(getX() - 1, getY());
		}
		Hp--;
	}
	if (Hp <= 0) {
		Alive = false;
	}
}

void RegPro::doSomething() {
	bool yelled = false;
	bool move = false;


	if (Alive == false) {
		return;
	}

	if (checkifStuned() && Leave == false) {
		if (wait == 0) {
			stuned = false;
		}
		wait--;
		return;
	}

	if (bribed == true) {
		Leave = true;
		wait = 0;
		bribed = false;
		return;

	}

	else if (wait > 0) {
		wait--;
		return;
	}

	else if (Leave == true) {
		Alive = false; // for now
		setVisible(false);
		if (getX() == 60 && getY() == 60) {
			Alive = false;
		}
		/*else {
			Path shortestPth = findShortestPath(60, 60);
			queue<Step> steps;
			for (int i = 0; i < shortestPth.findSize(); i++)
				steps.push(shortestPth.viewAt(i));
			while (!steps.empty()) {
				GraphObject::Direction faceDir = steps.front().direct;
				int goX = steps.front().point.first;
				int goY = steps.front().point.second;
				setDirection(faceDir);
				moveTo(goX, goY);
				steps.pop();
			}
		}*/
	}
	else if (Hp <= 0) {
		Leave = true;
		wait = 0;
		game1->GetWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
	}
	else if ((game1->GetWorld()->NearIceMan(this->getX(), this->getY(), this) == true) && yell <= 0) {
		
		game1->GetWorld()->playSound(SOUND_PROTESTER_YELL);
		game1->GetWorld()->DecHP(2);
		yelled = true;
		yell = 15;//reset		
	}
	else if ((game1->GetWorld()->IcemanVorHofRP(this->getX(), this->getY(), this) == true)) {
		see = true;
		if (game1->GetWorld()->getIcemanX() > getX()) {
			if (getDirection() != right) {
				setDirection(right);
			}
			else if (game1->GetWorld()->NearIceMan(this->getX(), this->getY(), this) != true) {
				moveTo(getX() + 1, getY());
			}
		}
		else if (game1->GetWorld()->getIcemanX() < getX()) {
			if (getDirection() != left) {
				setDirection(left);
			}
			else if (game1->GetWorld()->NearIceMan(this->getX(), this->getY(), this) != true) {
				moveTo(getX() - 1, getY());
			}
		}
		else if (game1->GetWorld()->getIcemanY() > getY()) {
			if (getDirection() != up) {
				setDirection(up);
			}
			else if (game1->GetWorld()->NearIceMan(this->getX(), this->getY(), this) != true) {
				moveTo(getX(), getY() + 1);
			}
		}
		else if (game1->GetWorld()->getIcemanY() < getY()) {
			if (getDirection() != down) {
				setDirection(down);
			}
			else if (game1->GetWorld()->NearIceMan(this->getX(), this->getY(), this) != true) {
				moveTo(getX(), getY() - 1);
			}
		}
		move = true;
	}
	else if ((game1->GetWorld()->IcemanVorHofRP(this->getX(), this->getY(), this) != true) && (numSquaresToMoveInCurrentDirection <= 0 || see == true) && (game1->GetWorld()->AtInter(this->getX(), this->getY(), this) != true)) {

		bool done = true;
		if (see == true) {
			numSquaresToMoveInCurrentDirection = 0;
			see = false;
		}
		if (numSquaresToMoveInCurrentDirection <= 0) {
			bool found = false;
			while (found == false) {
				int randomN = rand() % 4 + 1;
				switch (randomN) {
				case down:
					if (getY() > 0 && !game1->GetWorld()->IceHere(getX(), getY() - 1)) {
						setDirection(down);
						found = true;
						if (numSquaresToMoveInCurrentDirection <= 0) {
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
						}
					}
					break;
				case up:
					if (getY() < VIEW_HEIGHT - 4 && !game1->GetWorld()->IceHere(getX(), getY() + 1)) {
						setDirection(up);
						found = true;
						if (numSquaresToMoveInCurrentDirection <= 0) {
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
						}
					}
					break;
				case left:
					if (getX() > 0 && !game1->GetWorld()->IceHere(getX() - 1, getY())) {
						setDirection(left);
						found = true;
						if (numSquaresToMoveInCurrentDirection <= 0) {
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
						}
					}
					break;
				case right:
					if (getX() < VIEW_WIDTH - 4 && !game1->GetWorld()->IceHere(getX() + 1, getY())) {
						setDirection(right);
						found = true;
						if (numSquaresToMoveInCurrentDirection <= 0) {
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
						}
					}
					break;

				}
			}
		}
	}
	else if ((perp <= 0) && (game1->GetWorld()->AtInter1(this->getX(), this->getY(), this) == true)) {
		game1->GetWorld()->AtInter(this->getX(), this->getY(), this);

		perp1 = true;
		perp = 200;
		see = false;
		numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
	}
	else if (move == false) {
		see = false;
		numSquaresToMoveInCurrentDirection--;
		switch (getDirection()) {
		case up:
			if (getY() < VIEW_HEIGHT - 4 && !game1->GetWorld()->IceHere(getX(), getY() + 1)) {
				moveTo(getX(), getY() + 1);
			}
			else {
				bool found = false;
				while (found == false) {
					int randomN = rand() % 4 + 1;
					switch (randomN) {
					case 1:
						if (getY() > 0 && !game1->GetWorld()->IceHere(getX(), getY() - 1)) {
							setDirection(down);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;

						}
						break;

					case 2:
						if (getX() > 0 && !game1->GetWorld()->IceHere(getX() - 1, getY())) {
							setDirection(left);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;

						}
						break;
					case 3:
						if (getX() < VIEW_WIDTH - 4 && !game1->GetWorld()->IceHere(getX() + 1, getY())) {
							setDirection(right);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;

						}
						break;

					}
				}
			}

			break;
		case down:
			if (getY() > 0 && !game1->GetWorld()->IceHere(getX(), getY() - 1)) {
				moveTo(getX(), getY() - 1);
			}
			else {
				bool found = false;
				while (found == false) {
					int randomN = rand() % 4 + 1;
					switch (randomN) {

					case 1:
						if (getY() < VIEW_HEIGHT - 4 && !game1->GetWorld()->IceHere(getX(), getY() + 1)) {
							setDirection(up);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
						}
						break;
					case 2:
						if (getX() > 0 && !game1->GetWorld()->IceHere(getX() - 1, getY())) {
							setDirection(left);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;
						}
						break;
					case 3:
						if (getX() < VIEW_WIDTH - 4 && !game1->GetWorld()->IceHere(getX() + 1, getY())) {
							setDirection(right);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;
						}
						break;

					}
				}
			}

			break;
		case left:
			if (getX() > 0 && !game1->GetWorld()->IceHere(getX() - 1, getY())) {
				moveTo(getX() - 1, getY());
			}
			else {
				bool found = false;
				while (found == false) {
					int randomN = rand() % 4 + 1;
					switch (randomN) {
					case 1:
						if (getY() > 0 && !game1->GetWorld()->IceHere(getX(), getY() - 1)) {
							setDirection(down);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;
						}
						break;
					case 2:
						if (getY() < VIEW_HEIGHT - 4 && !game1->GetWorld()->IceHere(getX(), getY() + 1)) {
							setDirection(up);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;
						}
						break;

					case 3:
						if (getX() < VIEW_WIDTH - 4 && !game1->GetWorld()->IceHere(getX() + 1, getY())) {
							setDirection(right);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
						}
						break;

					}
				}
			}

			break;
		case right:
			if (getX() < VIEW_WIDTH - 4 && !game1->GetWorld()->IceHere(getX() + 1, getY())) {
				moveTo(getX() + 1, getY());
			}
			else {
				bool found = false;
				while (found == false) {
					int randomN = rand() % 4 + 1;
					switch (randomN) {
					case 1:
						if (getY() > 0 && !game1->GetWorld()->IceHere(getX(), getY() - 1)) {
							setDirection(down);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;
						}
						break;
					case 2:
						if (getY() < VIEW_HEIGHT - 4 && !game1->GetWorld()->IceHere(getX(), getY() + 1)) {
							setDirection(up);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;
						}
						break;
					case 3:
						if (getX() > 0 && !game1->GetWorld()->IceHere(getX() - 1, getY())) {
							setDirection(left);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
						}
						break;


					}
				}
			}

			break;
		}
		move = true;
	}

	if (yelled == true) {
		wait = rest;
		perp--;
		return;
	}
	if (perp1 == true) {
		yell--;
		perp1 = false;
		return;
	}
	else if (wait <= 0 && move == true) {
		wait = rest;
		yell--;
		perp--;
		return;
	}
	//else if (yelled == false) {
	
	//	yell--;	
	//	perp--;
	//	return;
	//}
	else if (wait == 0) {
		yell--;
		perp--;
		return;
	}
	else {
		wait--;
		return;
	}

}

void HardPro::doSomething() {
	bool yelled = false;
	bool move = false;
	

	if (Alive == false) {
		return;
	}
	if (checkifStuned() && Leave == false) {
		if (wait == 0) {
			stuned = false;
			if (bribed == true) {
				bribed = false;
			}
		}
		wait--;
		return;
	}
	if (bribed == true) {
		
		wait = stun;
		stuned = true;
		return;

	}

	else if (wait > 0) {
		wait--;
		return;
	}

	else if (Leave == true) {
		Alive = false; // for now
		setVisible(false);
		if (getX() == 60 && getY() == 60) {
			Alive = false;
		}
		else {
			// find fastest way out
		}
	}
	else if (Hp <= 0) {
		Leave = true;
		wait = 0;
		game1->GetWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
	}
	else if ((game1->GetWorld()->NearIceMan(this->getX(), this->getY(), this) == true) && yell <= 0) {
		
		game1->GetWorld()->playSound(SOUND_PROTESTER_YELL);
		game1->GetWorld()->DecHP(2);
		yelled = true;
		yell = 15;//reset		
	}

	// else if (within radius) ;
	// findShortestPath()	<------------------------------------------------------------------------------------------------O
	// Refer to 5th instruction for HardcoreProtestor::doSomething() or every tick!

	else if ((game1->GetWorld()->IcemanVorHofRP(this->getX(), this->getY(), this) == true)) {
		see = true;
		if (game1->GetWorld()->getIcemanX() > getX()) {
			if (getDirection() != right) {
				setDirection(right);
			}
			else if (game1->GetWorld()->NearIceMan(this->getX(), this->getY(), this) != true) {
				moveTo(getX() + 1, getY());
			}
		}
		else if (game1->GetWorld()->getIcemanX() < getX()) {
			if (getDirection() != left) {
				setDirection(left);
			}
			else if (game1->GetWorld()->NearIceMan(this->getX(), this->getY(), this) != true) {
				moveTo(getX() - 1, getY());
			}
		}
		else if (game1->GetWorld()->getIcemanY() > getY()) {
			if (getDirection() != up) {
				setDirection(up);
			}
			else if (game1->GetWorld()->NearIceMan(this->getX(), this->getY(), this) != true) {
				moveTo(getX(), getY() + 1);
			}
		}
		else if (game1->GetWorld()->getIcemanY() < getY()) {
			if (getDirection() != down) {
				setDirection(down);
			}
			else if (game1->GetWorld()->NearIceMan(this->getX(), this->getY(), this) != true) {
				moveTo(getX(), getY() - 1);
			}
		}
		move = true;
	}
	else if ((game1->GetWorld()->IcemanVorHofRP(this->getX(), this->getY(), this) != true) && (numSquaresToMoveInCurrentDirection <= 0 || see == true) && (game1->GetWorld()->AtInter(this->getX(), this->getY(), this) != true)) {

		bool done = true;
		if (see == true) {
			numSquaresToMoveInCurrentDirection = 0;
			see = false;
		}
		if (numSquaresToMoveInCurrentDirection <= 0) {
			bool found = false;
			while (found == false) {
				int randomN = rand() % 4 + 1;
				switch (randomN) {
				case down:
					if (getY() > 0 && !game1->GetWorld()->IceHere(getX(), getY() - 1)) {
						setDirection(down);
						found = true;
						if (numSquaresToMoveInCurrentDirection <= 0) {
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
						}
					}
					break;
				case up:
					if (getY() < VIEW_HEIGHT - 4 && !game1->GetWorld()->IceHere(getX(), getY() + 1)) {
						setDirection(up);
						found = true;
						if (numSquaresToMoveInCurrentDirection <= 0) {
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
						}
					}
					break;
				case left:
					if (getX() > 0 && !game1->GetWorld()->IceHere(getX() - 1, getY())) {
						setDirection(left);
						found = true;
						if (numSquaresToMoveInCurrentDirection <= 0) {
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
						}
					}
					break;
				case right:
					if (getX() < VIEW_WIDTH - 4 && !game1->GetWorld()->IceHere(getX() + 1, getY())) {
						setDirection(right);
						found = true;
						if (numSquaresToMoveInCurrentDirection <= 0) {
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
						}
					}
					break;

				}
			}
		}
	}
	else if ((perp <= 0) && (game1->GetWorld()->AtInter1(this->getX(), this->getY(), this) == true)) {
		game1->GetWorld()->AtInter(this->getX(), this->getY(), this);

		perp1 = true;
		perp = 200;
		see = false;
		numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
	}
	else if (move == false) {
		see = false;
		numSquaresToMoveInCurrentDirection--;
		switch (getDirection()) {
		case up:
			if (getY() < VIEW_HEIGHT - 4 && !game1->GetWorld()->IceHere(getX(), getY() + 1)) {
				moveTo(getX(), getY() + 1);
			}
			else {
				bool found = false;
				while (found == false) {
					int randomN = rand() % 4 + 1;
					switch (randomN) {
					case 1:
						if (getY() > 0 && !game1->GetWorld()->IceHere(getX(), getY() - 1)) {
							setDirection(down);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;

						}
						break;

					case 2:
						if (getX() > 0 && !game1->GetWorld()->IceHere(getX() - 1, getY())) {
							setDirection(left);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;

						}
						break;
					case 3:
						if (getX() < VIEW_WIDTH - 4 && !game1->GetWorld()->IceHere(getX() + 1, getY())) {
							setDirection(right);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;

						}
						break;

					}
				}
			}

			break;
		case down:
			if (getY() > 0 && !game1->GetWorld()->IceHere(getX(), getY() - 1)) {
				moveTo(getX(), getY() - 1);
			}
			else {
				bool found = false;
				while (found == false) {
					int randomN = rand() % 4 + 1;
					switch (randomN) {

					case 1:
						if (getY() < VIEW_HEIGHT - 4 && !game1->GetWorld()->IceHere(getX(), getY() + 1)) {
							setDirection(up);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
						}
						break;
					case 2:
						if (getX() > 0 && !game1->GetWorld()->IceHere(getX() - 1, getY())) {
							setDirection(left);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;
						}
						break;
					case 3:
						if (getX() < VIEW_WIDTH - 4 && !game1->GetWorld()->IceHere(getX() + 1, getY())) {
							setDirection(right);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;
						}
						break;

					}
				}
			}

			break;
		case left:
			if (getX() > 0 && !game1->GetWorld()->IceHere(getX() - 1, getY())) {
				moveTo(getX() - 1, getY());
			}
			else {
				bool found = false;
				while (found == false) {
					int randomN = rand() % 4 + 1;
					switch (randomN) {
					case 1:
						if (getY() > 0 && !game1->GetWorld()->IceHere(getX(), getY() - 1)) {
							setDirection(down);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;
						}
						break;
					case 2:
						if (getY() < VIEW_HEIGHT - 4 && !game1->GetWorld()->IceHere(getX(), getY() + 1)) {
							setDirection(up);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;
						}
						break;

					case 3:
						if (getX() < VIEW_WIDTH - 4 && !game1->GetWorld()->IceHere(getX() + 1, getY())) {
							setDirection(right);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
						}
						break;

					}
				}
			}

			break;
		case right:
			if (getX() < VIEW_WIDTH - 4 && !game1->GetWorld()->IceHere(getX() + 1, getY())) {
				moveTo(getX() + 1, getY());
			}
			else {
				bool found = false;
				while (found == false) {
					int randomN = rand() % 4 + 1;
					switch (randomN) {
					case 1:
						if (getY() > 0 && !game1->GetWorld()->IceHere(getX(), getY() - 1)) {
							setDirection(down);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;
						}
						break;
					case 2:
						if (getY() < VIEW_HEIGHT - 4 && !game1->GetWorld()->IceHere(getX(), getY() + 1)) {
							setDirection(up);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
							perp = 200;
						}
						break;
					case 3:
						if (getX() > 0 && !game1->GetWorld()->IceHere(getX() - 1, getY())) {
							setDirection(left);
							found = true;
							numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
						}
						break;


					}
				}
			}

			break;
		}
		move = true;
	}

	if (yelled == true) {
		wait = rest;
		perp--;
		return;
	}
	if (perp1 == true) {
		yell--;
		perp1 = false;
		return;
	}
	else if (wait <= 0 && move == true) {
		wait = rest;
		yell--;
		perp--;
		return;
	}
	//else if (yelled == false) {
	
	//	yell--;	
	//	perp--;
	//	return;
	//}
	else if (wait == 0) {
		yell--;
		perp--;
		return;
	}
	else {
		wait--;
		return;
	}

}

void GoldNug::doSomething() {
	if (Alive == false) {
		return;
	}

	if (ForIceman) {


		if (!isVisible() && game->GetWorld()->close(this, 4, true)) {
			setVisible(true);
			return;
		}
		else if (isVisible() && game->GetWorld()->close(this, 3, true)) {
			setVisible(false);
			Alive = false;
			game->GetWorld()->addemptyspace(getX(), getY());
			game->GetWorld()->playSound(SOUND_GOT_GOODIE);
			game->GetWorld()->increaseScore(10);
			game->GetWorld()->getIceman()->gotGold();
			return;
		}
	}

	if (ForPro) {
		if (isVisible() && game->GetWorld()->close(this, 3, false)) {
			Alive = false;
			game->GetWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
			setVisible(false);
			return;
		}
		else if (wait <= 0) {
			game->GetWorld()->addemptyspace(getX(), getY());
			Alive = false;
			setVisible(false);
		}
		else {
			wait--;
		}

	}

	return;
}

void Sonar::doSomething() {
	if (Alive == false) {
		return;
	}
	else if (game->GetWorld()->closeIceman(this)) {
		setVisible(false);
		Alive = false;
		game->GetWorld()->playSound(SOUND_GOT_GOODIE);
		game->GetWorld()->getIceman()->gotSonar();
		game->GetWorld()->increaseScore(75);
	}
	if (wait <= 0) {
		Alive = false;
		game->GetWorld()->addemptyspace(getX(), getY());
		setVisible(false);
	}
	wait--;
}

void Waterpool::doSomething() {
	if (Alive == false) {
		return;
	}
	else if (game->GetWorld()->closeIceman(this)) {
		setVisible(false);
		Alive = false;
		game->GetWorld()->playSound(SOUND_GOT_GOODIE);
		game->GetWorld()->getIceman()->gotWater();
		game->GetWorld()->increaseScore(100);
	}
	if (wait <= 0) {
		Alive = false;
		game->GetWorld()->addemptyspace(getX(), getY());
		setVisible(false);
	}
	wait--;
}

void Boulder::doSomething() {
	if (Alive == false) {
		return;
	}
	if (falling) {
		moveTo(getX(), getY() - 1);
		if (getY() <= 0) {
			setVisible(false);
			this->Alive = false;
			game->GetWorld()->addemptyspace(getX(), getY());
		}
		else if (game->GetWorld()->Icebelow(getX(), getY() - 1)) {
			setVisible(false);
			this->Alive = false;
			game->GetWorld()->addemptyspace(getX(), getY());
		}
		else if (game->GetWorld()->CheckBolBelow(getX(), getY() - 1)) {
			setVisible(false);
			this->Alive = false;
			game->GetWorld()->addemptyspace(getX(), getY());
		}
		if (game->GetWorld()->HitPlayer(this)) {
			setVisible(false);
			this->Alive = false;
			game->GetWorld()->addemptyspace(getX(), getY());
			return;
		}
	}
	else if (stable) {
		if (!game->GetWorld()->Icebelow(getX(), getY() - 1)) {
			stable = false;
		}
	}
	else if (!stable) {
		if (wait <= 0) {
			game->GetWorld()->playSound(SOUND_FALLING_ROCK);
			falling = true;
		}
		else {
			wait--;
		}
	}
}


// Barrel member functions/methods
void Barrel::doSomething() {

	if (!Alive)
		return;
	if (!isVisible() && m_game->GetWorld()->closeIceman(this,4)) {
		setVisible(true);
		return;
	}
	if (m_game->GetWorld()->closeIceman(this,2)) {
		m_game->GetWorld()->barrelfound();
		setVisible(false);
		Alive = false;
		m_game->playSound(SOUND_FOUND_OIL);
	}
}

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp