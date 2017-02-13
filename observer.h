#pragma once

class Observer {
public:
	virtual ~Observer() {}
	virtual void onNotify() = 0;
};