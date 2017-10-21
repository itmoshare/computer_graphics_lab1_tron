#include "Player.h"

Player::Player(Bitmap figure, HBRUSH trackBrush, RECT position) {
	this->figure = figure;
	this->trackBrush = trackBrush;
	this->position = position;
}

