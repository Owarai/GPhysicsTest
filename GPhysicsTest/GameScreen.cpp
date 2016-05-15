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

YVec& YVec::operator-=(const YVec& vec2) {
	this->x -= vec2.x;
	this->y -= vec2.y;
	return *this;
}

YVec& YVec::operator+=(const YVec& vec2) {
	this->x += vec2.x;
	this->y += vec2.y;
	return *this;
}

YVec& YVec::operator*=(double v) {
	this->x *= v;
	this->y *= v;
	return *this;
}

YVec& YVec::operator/=(double v) {
	this->x /= v;
	this->y /= v;
	return *this;
}

Ball::Ball()
{
	this->pos = YVec(rand() % 400, rand() % 300);
	this->vel = YVec((double)(rand() % 10) / 10, (double)(rand() % 10) / 10);
	this->rad = (rand() % 10) + 5;
	this->color = QColor(rand() % 256, rand() % 256, rand() % 256);

}

void Ball::move()
{
	pos += vel;
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
	for (int i = 0; i < 4; i++) {
		balls.push_back(Ball());
	}

	/*
	for (int i = 0; i < 3; i++) {
	balls.push_back(Ball());
	
	balls[i].rad = 10;
	balls[i].pos.y = height();
	balls[i].vel.y = 0;
	}
	*/
	/*
	balls[1].rad = 20;

	balls[0].pos.x = width() / 4;
	balls[1].pos.x = width() / 2;
	balls[2].pos.x = width() * 3/ 4;

	balls[0].vel.x = 1;
	balls[1].vel.x = 0.01;
	balls[2].vel.x = -1;
	*/

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
		painter.setPen(balls[i].color);
		painter.setBrush(QBrush(balls[i].color));
		painter.drawEllipse(QPointF(balls[i].pos.x, balls[i].pos.y), balls[i].rad, balls[i].rad);
		painter.drawEllipse(QPointF(balls[i].pos.x, balls[i].pos.y), balls[i].rad, balls[i].rad);
	}

	float beforeMomen = momentum;


	momentum = 0;
	for (int i = 0; i < balls.size(); i++) {
		momentum += pow(balls[i].rad, 2) * abs(length(balls[i].vel));
	}

	if (momentum > beforeMomen) {
		painter.setPen(Qt::red);
	}
	else {
		painter.setPen(Qt::black);
	}
	char str[256];
	sprintf(str, "%lf", momentum);
	painter.drawText(10, 10, str);

}

void GameScreen::mousePressEvent(QMouseEvent * event) {
	balls.push_back(Ball());
	balls.back().pos.x = event->pos().x();
	balls.back().pos.y = event->pos().y();
	balls.back().vel = YVec(0.0, 0.0);

}

void GameScreen::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_B) {
		for (int i = 0; i < balls.size(); i++) {
			balls[i].vel.y -= 5;
		}
	}
	else if (event->key() == Qt::Key_C) {
		for (int i = 0; i < balls.size(); i++) {
			balls[i].vel /= 2;
		}
	}
	else if (event->key() == Qt::Key_R) {
		for (int i = 0; i < balls.size(); i++) {
			balls[i].vel = YVec (0.0, 0.0) - balls[i].vel;
		}
	}
	else if (event->key() == Qt::Key_D) {
		for (int i = 0; i < balls.size(); i++) {
			balls[i].rad *= 0.5;
		}
	}
	else if (event->key() == Qt::Key_E) {
		for (int i = 0; i < balls.size(); i++) {
			balls[i].rad *= 2;
		}
	}
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
		else if (balls[i].pos.y < balls[i].rad) {
			balls[i].pos.y = balls[i].rad;
			balls[i].vel.y = -balls[i].vel.y;
		}

		if (balls[i].pos.x < balls[i].rad) {
			balls[i].pos.x = balls[i].rad;
			balls[i].vel.x = -balls[i].vel.x;
		}
		else if (balls[i].pos.x > width() - balls[i].rad) {
			balls[i].pos.x = width() - balls[i].rad;
			balls[i].vel.x = -balls[i].vel.x;
		}
		/*
		if (abs(balls[i].vel.x) < 0.1) {
			balls[i].vel.x = 0;
		}
		*/

		if (balls[i].pos.y > height() - (balls[i].rad + 0.1) && abs(balls[i].vel.y) < 0.1) {

		//if (abs(balls[i].vel.y) < 0.00001) {
			balls[i].vel.y = 0;

			//friction
			balls[i].vel.x *= 0.99;
			if (abs(balls[i].vel.x) < 0.1) {
				balls[i].vel.x = 0;
			}
		}
		/*if (balls[i].pos.y > height() - (balls[i].rad + 0.1) && balls[i].vel.y < 0.2) {
			balls[i].vel.x *= 0.99;
		}*/
		else {
			balls[i].vel.y += 0.1f;
		}


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
	YVec midpt = (ball1.pos * ball2.rad+ ball2.pos * ball1.rad) / (ball1.rad + ball2.rad);

	// compute the unit vector of perpendicular direction to the collision face
	YVec v2 = (ball2.pos - ball1.pos);
	v2 /= length(v2);

	ball1.pos = midpt - v2 * ball1.rad;
	
	v2 *= dot(ball1.vel, v2);
	YVec v3 = ball1.vel - v2;

	// 
	YVec v4 = (ball1.pos - ball2.pos);
	v4 /= length(v4);

	ball2.pos = midpt - v4 * ball2.rad;

	v4 *= dot(ball2.vel, v4);
	YVec v5 = ball2.vel - v4;

	ball1.vel = v4 * pow(ball2.rad, 2) / pow(ball1.rad, 2) + v3;
	ball2.vel = v2 * pow(ball1.rad, 2) / pow(ball2.rad, 2) + v5;


}
