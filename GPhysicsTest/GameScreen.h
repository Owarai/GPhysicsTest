#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <QWidget>
#include <vector>
#include <utility>
#include "ui_GameScreen.h"
#include <QTimer>
#include <time.h>
#include <vector>
#include <QKeyEvent>
#include <QMouseEvent>

class YVec
{
public:
	double x;
	double y;

public:
	YVec();
	YVec(double x, double y);
	~YVec();

	YVec operator-(const YVec& vec2);
	YVec operator+(const YVec& vec2);
	YVec operator*(double v);
	YVec operator/(double v);

	YVec& operator-=(const YVec& vec2);
	YVec& operator+=(const YVec& vec2);
	YVec& operator*=(double v);
	YVec& operator/=(double v);

};

double length(const YVec& vec);
double dot(const YVec& vec1, const YVec& vec2);

class Ball
{
public:
	QColor color;
	YVec pos;
	YVec vel;
	double rad;
	double mass;

public:
	Ball();
	~Ball() {}

	void move();

};

class GameScreen : public QWidget
{
	Q_OBJECT

public:
	static enum { GRAVITY_NULL = 0, GRAVITY_DOWN, GRAVITY_UP, GRAVITY_LEFT, GRAVITY_RIGHT };

public:
	GameScreen(QWidget *parent = 0);
	~GameScreen();

	void keyPressEvent(QKeyEvent* pEvent);
	void keyReleaseEvent(QKeyEvent* pEvent);
	bool isCollided(const Ball& ball1, const Ball& ball2);
	void collision(Ball& ball1, Ball& ball2);

public slots:
	void onTimer();

protected:
	void paintEvent(QPaintEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);

private:
	Ui::GameScreen ui;

	int grav;

	std::vector<Ball> balls;
	double momentum;

	QTimer* timer;
	QPoint clickedPos;
};

#endif // GAMESCREEN_H
