#include "Player.h"

Player::Player(Bitmap figure, HBRUSH trackBrush, int x, int y) {
	this->figure = figure;
	this->trackBrush = trackBrush;
	this->X = x;
	this->Y = y;
}

