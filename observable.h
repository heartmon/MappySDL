#pragma once
#include <vector>
#include "observer.h"

class Observable {
public:
	void addObserver(Observer* observer) {}
	void removeObserver(Observer* observer) {}
	void notify() {}
private:
	std::vector<Observer> observers_;
	//Observer* observers_;
	int numObservers_;
};