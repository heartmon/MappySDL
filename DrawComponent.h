#pragma once

template <class T>
class DrawComponent {
public:
	virtual ~DrawComponent() {};
	virtual Update(int dt, T* entity);
private:

};