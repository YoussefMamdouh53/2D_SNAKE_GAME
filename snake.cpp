#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <cmath>
#include <vector>
#include <GL/freeglut.h>

int WinHeight = 1920;
int WinWidth = 1080;
const int MAX_FPS = 15;
const int SCALE = 60;
const float PI = 3.14f;
int score = 0, best = 0;
bool gameover = false, wait = false;
using namespace std;

float get_random_number() {
    return (float)rand() / RAND_MAX;
}
int pow2(int n) {
	return n * n;
}

struct Vector2 {
	int x;
	int y;
	Vector2() : x(0) , y(0) {}
	Vector2(double x, double y) : x(x), y(y) {}
};

struct Color {
	float r;
	float g;
	float b;
};

class Tail {
public:
	Vector2 v;
	Vector2 dir;
	Color color;
};

class Food {
private:
	Vector2 pos;
	Color color;
public:
	Food() {
		update();
	}
	void update() {
		pos.x = WinWidth * get_random_number();
		pos.x -= pos.x % SCALE;
		pos.y = WinHeight * get_random_number();
		pos.y -= pos.y % SCALE;
		color.r = get_random_number();
		color.g = get_random_number();
		color.b = get_random_number();
	}
	bool check(Tail const *v) {
		if (pow2(v[0].v.x - pos.x) + pow2(v[0].v.y - pos.y) < pow2(SCALE)) {
			update();
			return true;
		}
		return false;
	}
	void show() {
		glColor3f(color.r, color.g, color.b);
		float r = SCALE / 2;
		glBegin(GL_POLYGON);
		for (int i = 0;i < 360; i += 60){
			glVertex2f(r * sin(i * PI / 180.0) + pos.x + r, r * cos(i * PI / 180.0) + pos.y + r);
		}
		glEnd();
	}
};

Food f;

class Snake {
private:
	vector<Tail> v;
public:
	Snake() {
		Tail x;
		x.v.x = 0;
		x.v.y = 0;
		x.dir.x = SCALE;
		x.dir.y = 0;
		x.color.r = 0;
		x.color.g = 0.4117;
		x.color.b = 0;
		v.push_back(x);
	}
	void moveUp(){
		if (v[0].dir.x == 0) return;
		v[0].dir.x = 0;
		v[0].dir.y = -SCALE;
	}
	void moveDown(){
		if (v[0].dir.x == 0) return;
		v[0].dir.x = 0;
		v[0].dir.y = SCALE;
	}
	void moveRight(){
		if (v[0].dir.y == 0) return;
		v[0].dir.x = SCALE;
		v[0].dir.y = 0;
	}
	void moveLeft(){
		if (v[0].dir.y == 0) return;
		v[0].dir.x = -SCALE;
		v[0].dir.y = 0;
	}

	void Gameover() {
		v.erase(v.begin() + 1, v.end());
		best = max(score, best);
		score = 0;
		moveRight();
		v[0].v.x = v[0].v.y = 0;
		increment();
		increment();
		gameover = true;
	}
	void increment() {
		Tail tail;
		tail.dir.x = v[v.size()-1].dir.x;
		tail.dir.y = v[v.size()-1].dir.y;
		tail.v.x = v[v.size()-1].v.x - tail.dir.x;
		tail.v.y = v[v.size()-1].v.y - tail.dir.y;
		tail.color.r = v[v.size()-1].color.r + 0.015;
		tail.color.g = v[v.size()-1].color.g + 0.015;
		tail.color.b = v[v.size()-1].color.b + 0.015;
		v.push_back(tail);
	}
	void update() {
		//if (v[0].v.y < 0 || v[0].v.y > WinHeight - SCALE || v[0].v.x > WinWidth - SCALE || v[0].v.x < 0) Gameover(); // if the snake hits the border the gameover!
		if (v[0].v.x > WinWidth - SCALE)
				v[0].v.x = 0;
			else if (v[0].v.x < 0)
				v[0].v.x = WinWidth - SCALE;
			else if (v[0].v.y > WinHeight - SCALE)
				v[0].v.y = 0;
			else if (v[0].v.y < 0)
				v[0].v.y = WinHeight - SCALE;
		if (f.check(&v[0])) {
			increment();
			f.update();
			score++;
		}
		if (death()) {
			Gameover();
		}
		for (int i = v.size() - 1;i > 0; i--){
			v[i].v.x += v[i].dir.x;
			v[i].v.y += v[i].dir.y;
			v[i].dir.x = v[i-1].dir.x;
			v[i].dir.y = v[i-1].dir.y;
			if (v[i].v.x > WinWidth - SCALE)
				v[i].v.x = 0;
			else if (v[i].v.x < 0)
				v[i].v.x = WinWidth - SCALE;
			else if (v[i].v.y > WinHeight - SCALE)
				v[i].v.y = 0;
			else if (v[i].v.y < 0)
				v[i].v.y = WinHeight - SCALE;
		}
		v[0].v.x += v[0].dir.x;
		v[0].v.y += v[0].dir.y;
	}
	bool death() {
		for (int i = 1; i<v.size();i++){
			if (v[0].v.x == v[i].v.x && v[0].v.y == v[i].v.y) return true;
		}
		return false;
	}
	void show() {
		// glColor3f(34/255.0,139/255.0,34/255.0);
		for (int i = 0;i<v.size();i++){
			glColor3f(v[i].color.r,v[i].color.g,v[i].color.b);
			glBegin(GL_QUADS);
			glVertex2f(v[i].v.x, v[i].v.y);
			glVertex2f(v[i].v.x + SCALE, v[i].v.y);
			glVertex2f(v[i].v.x + SCALE, v[i].v.y + SCALE);
			glVertex2f(v[i].v.x, v[i].v.y + SCALE);
			glEnd();
		}
	}
	Tail const *get_pos() {
		return &v[0];
	}
};

Snake s;

void Draw() {
    glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.1,0.1,0.1, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(0, WinWidth, WinHeight, 0,  -1, 1);
	glColor3f(1,1,1);
	glRasterPos2f(WinWidth/2 - 75, 50);
  	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)("Score: " + to_string(score) + ", Best: " + to_string(best)).c_str());
	glRasterPos2f(25, WinHeight - 25);
  	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Use 'W S D A' to Move and ESC to exit.");
	glRasterPos2f(WinWidth - 450, WinHeight - 20);
  	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"SNAKE GAME MADE BY YOUSSEF MAMDOUH");
	if (gameover)
	{
		glColor3f(1,0,0);
		glRasterPos2f(WinWidth/2-75, WinHeight/2);
  		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"GAME OVER!");
	}
	else {
		s.update();
		s.show();
		f.show();		
	}
    glFlush();
	wait = false;
}

void KeyPressed(unsigned char key, int x, int y) {
	if (wait) return;
	wait = true;
	if (key == 'w' || key == 'W')
		s.moveUp();
	else if (key == 's' || key == 'S')
		s.moveDown();
	else if (key == 'd' || key == 'D')
		s.moveRight();
	else if (key == 'a' || key == 'A')
		s.moveLeft();
	else if (key == 27) exit(0);
	else if (key == '+') s.increment();
	gameover = false;
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000/MAX_FPS, timer, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	srand(time(0));
	WinWidth = glutGet(GLUT_SCREEN_WIDTH);
	WinHeight = glutGet(GLUT_SCREEN_HEIGHT);
	f.update();
	s.increment();
	s.increment();
	glutInitDisplayMode(GLUT_SINGLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(WinWidth, WinHeight);
	glutCreateWindow("Snake");
	glutFullScreen();
	glutDisplayFunc(Draw);
	glutKeyboardFunc(KeyPressed);
	timer(0);
	glutMainLoop();
	return 0;
}