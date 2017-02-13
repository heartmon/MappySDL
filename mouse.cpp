#pragma once
#include "mouse.h"
#include "mouse_render_component.h"
#include "mouse_input_component.h"

Mouse::Mouse() {
}

void Mouse::Init(AvancezLib* system, MouseRenderComponent* renderComponent, MouseInputComponent* inputComponent) {
	//Set up
	this->system = system;
	this->renderComponent = renderComponent;
	this->inputComponent = inputComponent;

	//renderComponent->SetMouse(this);

	//initial position
	horizontalPosition = 0;
	verticalPosition = 0;
	currentState = STATE_STAND;
	direction = 1;
}

void Mouse::Draw() {

}

void Mouse::Update(float dt) {
	inputComponent->Update(dt, this);
	renderComponent->Update(dt, this);
}

void Mouse::Destroy() {
	sprite->destroy();
}

int Mouse::getCurrentState() {
	return this->currentState;
}

void Mouse::setCurrentState(int state) {
	this->currentState = state;
}

