#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	canvas = new GameScreen();
	this->setCentralWidget(canvas);
}

MainWindow::~MainWindow()
{

}

void MainWindow::keyPressEvent(QKeyEvent* event) {
	canvas->keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {

}
