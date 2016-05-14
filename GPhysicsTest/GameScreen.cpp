#include "GameScreen.h"
#include <QPainter>
#include <random>

YVec::YVec(double x, double y)
{
	this->x = x;
	this->y = y;
}

YVec::YVec()
{
	this->x = 0.0f;
	this->y = 0.0f;
}

YVec::~YVec()
{

}

YVec YVec::operator-(const YVec& vec2) {
	YVec result;
	result.x = this->x - vec2.x;
	result.y = this->y - vec2.y;
	return result;
}

YVec YVec::operator+(const YVec& vec2) {
	YVec result;
	result.x = this->x + vec2.x;
	result.y = this->y + vec2.y;
	return result;
}

YVec YVec::operator*(double v) {
	YVec result;
	result.x = this->x * v;
	result.y = this->y * v;
	return result;
}

YVec YVec::operator/(double v) {
	YVec result;
	result.x = this->x / v;
	result.y = this->y / v;
	return result;
}

Ball::Ball()
{
	this->pos = YVec(rand() % 400, rand() % 300);
	this->vel = YVec((double)(rand() % 10) / 10, (double)(rand() % 10) / 10);
	this->rad = (rand() % 20) + 1;

}

void Ball::move()
{
	this->pos.x += this->vel.x;
	this->pos.y += this->vel.y;
}

double length(const YVec& vec) {
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

double dot(const YVec& vec1, const YVec& vec2) {
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

GameScreen::GameScreen(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	for (int i = 0; i < 10; i++) {
		balls.push_back(Ball());
	}

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));

	timer->start(10);
}

GameScreen::~GameScreen()
{

}

void GameScreen::paintEvent(QPaintEvent * event) {
	QPainter painter(this);
	painter.fillRect(0, 0, width(), height(), QColor(255, 255, 255));
	for (int i = 0; i < balls.size(); i++) {
		painter.drawEllipse(QPointF(balls[i].pos.x, balls[i].pos.y), balls[i].rad, balls[i].rad);
	}
}

void GameScreen::keyPressEvent(QKeyEvent* event) {

}

void GameScreen::keyReleaseEvent(QKeyEvent* event) {

}

void GameScreen::onTimer() {
	for (int i = 0; i < balls.size(); i++) {
		balls[i].move();

		for (int j = i + 1; j < balls.size(); j++) {
			if (isCollided(balls[i], balls[j])) {
				collision(balls[i], balls[j]);
			}
		}

		if (balls[i].pos.y > height() - balls[i].rad) {
			balls[i].pos.y = height() - balls[i].rad;
			balls[i].vel.y = -balls[i].vel.y * 0.9;
		}

		if (balls[i].pos.x < balls[i].rad) {
			balls[i].pos.x = balls[i].rad;
			balls[i].vel.x = -balls[i].vel.x;
		}
		else if (balls[i].pos.x > width() - balls[i].rad) {
			balls[i].pos.x = width() - balls[i].rad;
			balls[i].vel.x = -balls[i].vel.x;
		}

		if (balls[i].pos.y > height() - (balls[i].rad + 0.1) && balls[i].vel.y < 0.2) {
			balls[i].vel.x *= 0.9995;
		}

		balls[i].vel.y += 0.1f;
		/*
		if (abs(balls[i].vel.x) < 0.1) {
			balls[i].vel.x = 0;
		}
		if (abs(balls[i].vel.y) < 0.1) {
			balls[i].vel.y = 0;
		}
		*/
	}



	update();
}

bool GameScreen::isCollided(const Ball& ball1, const Ball& ball2) {
	double xDist = (ball1.pos.x - ball2.pos.x);
	double yDist = (ball1.pos.y - ball2.pos.y);

	if (sqrt(xDist * xDist + yDist * yDist) < ball1.rad + ball2.rad) {
		return true;
	}

	return false;

}

void GameScreen::collision(Ball& ball1, Ball& ball2) {
	YVec midpt = (ball1.pos + ball2.pos) / 2;

	// compute the unit vector of perpendicular direction to the collision face
	YVec v2 = (ball2.pos - ball1.pos);
	v2 = v2 / length(v2);

	ball1.pos = midpt - v2 * ball1.rad;
	
	v2 = v2 * dot(ball1.vel, v2);
	YVec v3 = ball1.vel - v2;

	// 
	YVec v4 = (ball1.pos - ball2.pos);
	v4 = v4 / length(v4);

	ball2.pos = midpt - v4 * ball2.rad;

	v4 = v4 * dot(ball2.vel, v4);
	YVec v5 = ball2.vel - v4;


	ball1.vel = v4 + v3;
	ball2.vel = v2 + v5;
}
