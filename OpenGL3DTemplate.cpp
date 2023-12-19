#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <glut.h>
#include <iostream>
#include <windows.h>

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
#define INTERVAL 1000 
float playerPosX = 0;
float playerPosY = 0.5;
float playerPosZ = 0.5; // Added Z-coordinate
float playerRotation = 0.0f;
bool wasPreviousStepLeft = false;
bool wasPreviousStepR = false;
bool wasPreviousStepT = false;
bool wasPreviousStepD = false;
bool goalCollected = false;
const float PI = 3.14159265359;
int timer = 120;
GLfloat waterJetHeight = 0.0f;  // Initial height of the water jet
GLfloat waterJetSpeed = 0.002f;  // Speed of the water jet animation
GLfloat wheelRotationAngle = 0.0f;
float swingAngle = 0.0f;
float angle = 0.0f;
float teaAngle = 0.0f;
float scale_factor = 1.0f;
float scale_factorr = 1.0f;
bool rightt = true;
bool accelerating = true;
bool scale_up = true;
bool scale_upp = true;
bool pressed = false;
bool pressedonce = false;
int t = 0;

void Sound(int value) {
	const wchar_t* soundb = L"sss.wav";

	if (PlaySound(soundb, NULL, SND_ASYNC | SND_LOOP | SND_FILENAME)) {
		std::wcout << L"Sound played successfully." << std::endl;
	}
	else {
		std::wcerr << L"Failed to play the sound. Error code: " << GetLastError() << std::endl;
	}
}


void SoundLost() {
	const wchar_t* soundl = L"sad.wav";

	if (PlaySound(soundl, NULL, SND_ASYNC | SND_FILENAME)) {
		std::wcout << L"Sound played successfully." << std::endl;
	}
	else {
		std::wcerr << L"Failed to play the sound. Error code: " << GetLastError() << std::endl;
	}
}

void SoundPower() {
	const wchar_t* soundp = L"power.wav";

	if (PlaySound(soundp, NULL, SND_ASYNC | SND_LOOP | SND_FILENAME)) {
		std::wcout << L"Sound played successfully." << std::endl;
	}
	else {
		std::wcerr << L"Failed to play the sound. Error code: " << GetLastError() << std::endl;
	}

}


void SoundWon() {
	const wchar_t* soundw = L"won.wav";

	if (PlaySound(soundw, NULL, SND_ASYNC | SND_LOOP | SND_FILENAME)) {
		std::wcout << L"Sound played successfully." << std::endl;
	}
	else {
		std::wcerr << L"Failed to play the sound. Error code: " << GetLastError() << std::endl;
	}
}

void youLose(int value) {
	SoundLost();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glColor3f(0.8f, 0.6f, 0.2f);

	float textX = 0.0f;
	float textY = 1.0f;
	float textZ = 0.0f; // Set the z-coordinate to 0

	glColor3f(0.8f, 0.6f, 0.2f);

	glRasterPos3f(textX, textY, textZ);

	char loseText[] = "You Lose!";
	for (int i = 0; i < strlen(loseText); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, loseText[i]);
	}


	Sleep(5000);
}

void youWin() {
	SoundWon();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glColor3f(0.8f, 0.6f, 0.2f);

	float textX = 0.0f;
	float textY = 1.0f;
	float textZ = 0.0f; // Set the z-coordinate to 0

	glColor3f(0.8f, 0.6f, 0.2f);

	glRasterPos3f(textX, textY, textZ);

	char loseText[] = "You Win!";
	for (int i = 0; i < strlen(loseText); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, loseText[i]);
	}

	Sleep(5000);
}
void Timer(int value) {
	if (timer > 0) {
		timer--;
	}
	if (timer == 0) {

	}
	glutTimerFunc(1000, Timer, 0);

	glutPostRedisplay();
}
float wallColor[3] = { 1.0f, 1.0f, 1.0f };  // Initial color of the walls

// Function to change the color of the walls
void changeWallColor(int value) {
	for (int i = 0; i < 3; ++i) {
		wallColor[i] = static_cast<float>(rand()) / RAND_MAX;  // Random color value between 0 and 1
	}
	glutPostRedisplay();
	glutTimerFunc(INTERVAL, changeWallColor, 0);  // Set the timer for the next color change
}

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 1.0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;

void drawWall(double thickness) {
	glPushMatrix();
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}


void setDefaultCameraPosition() {
	camera = Camera(-0.005504, 0.531817, 1.964828, -0.002969, 0.561812, 0.965281, -0.000000, 0.999550, 0.029994);
}

void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

void drawGround() {
	glColor3f(0.5f, 0.5f, 0.5f);  // Gray color for the ground
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, 0.0f, -1.0f);  // Bottom-left corner
	glVertex3f(-1.0f, 0.0f, 1.0f);   // Top-left corner
	glVertex3f(1.0f, 0.0f, 1.0f);    // Top-right corner
	glVertex3f(1.0f, 0.0f, -1.0f);   // Bottom-right corner
	glEnd();
}

void drawPlayer() {
	glPushMatrix();
	if (wasPreviousStepLeft || wasPreviousStepR || wasPreviousStepT) {
		glTranslatef(playerPosX, playerPosY, playerPosZ);
		glRotatef(playerRotation, 0, 1.0, 0);

		// Draw head
		glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glutSolidSphere(0.1, 50, 50);

		// Draw eyes
		glColor3f(0.0f, 0.0f, 0.0f);
		glTranslated(-0.03, 0.03, 0.08);
		glutSolidSphere(0.02, 50, 50);

		glTranslated(0.06, 0.0, 0.0);
		glutSolidSphere(0.02, 50, 50);

		glPopMatrix();

		// Draw body
		glPushMatrix();
		glColor3f(0.0f, 1.0f, 0.0f);
		glTranslated(0.0, -0.2, 0.0);  // Adjusted translation for the body
		glScaled(0.1, 0.2, 0.1);  // Adjusted scale to make the body longer
		glutSolidCube(1.0);  // Cube for the body
		glPopMatrix();

		// Draw left arm
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);
		glTranslated(-0.075, -0.18, 0.0);  // Adjusted translation for the left arm
		glScaled(0.04, 0.15, 0.04);
		glutSolidCube(1.0);
		glPopMatrix();

		// Draw right arm
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);
		glTranslated(0.075, -0.18, 0.0);  // Adjusted translation for the right arm
		glScaled(0.04, 0.15, 0.04);
		glutSolidCube(1.0);
		glPopMatrix();
		// Draw left leg
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);
		glTranslated(-0.0375, -0.4, 0.0);  // Adjusted translation for the left leg
		glScaled(0.04, 0.2, 0.04);
		glutSolidCube(1.0);  // Adjusted scale to make the leg smaller
		glPopMatrix();

		// Draw right leg
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);
		glTranslated(0.0375, -0.4, 0.0); // Corrected translation for the right leg
		glScaled(0.04, 0.2, 0.04);
		glutSolidCube(1.0);  // Adjusted scale to make the leg smaller
		glPopMatrix();

	}
	else {
		// Draw head
		glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f); // Red color
		glTranslated(playerPosX, playerPosY, playerPosZ); // Set player position
		glutSolidSphere(0.1, 50, 50); // Sphere for the head

		// Draw eyes
		glColor3f(0.0f, 0.0f, 0.0f); // Black color for eyes
		glTranslated(-0.03, 0.03, 0.08); // Adjust position for the left eye
		glutSolidSphere(0.02, 50, 50); // Left eye

		glTranslated(0.06, 0.0, 0.0); // Adjust position for the right eye
		glutSolidSphere(0.02, 50, 50); // Right eye

		glPopMatrix();

		// Draw body
		glPushMatrix();
		glColor3f(0.0f, 1.0f, 0.0f); // Green color
		glTranslated(playerPosX, playerPosY - 0.2, playerPosZ); // Set player position
		glScaled(0.1, 0.2, 0.1); // Adjusted scale to make the body longer
		glutSolidCube(1.0); // Cube for the body
		glPopMatrix();

		// Draw left arm
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f); // Blue color
		glTranslated(playerPosX - 0.075, playerPosY - 0.18, playerPosZ); // Set player position
		glScaled(0.04, 0.15, 0.04);
		glutSolidCube(1.0);
		glPopMatrix();

		// Draw right arm
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f); // Blue color
		glTranslated(playerPosX + 0.075, playerPosY - 0.18, playerPosZ); // Set player position
		glScaled(0.04, 0.15, 0.04);
		glutSolidCube(1.0);
		glPopMatrix();

		// Draw left leg
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f); // Blue color
		glTranslated(playerPosX - 0.0375, playerPosY - 0.4, playerPosZ); // Set player position

		// Adjusted scale to make the leg longer and narrower
		glScaled(0.04, 0.2, 0.04);
		glutSolidCube(1.0);
		glPopMatrix();

		// Draw right leg
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f); // Blue color
		glTranslated(playerPosX + 0.0375, playerPosY - 0.4, playerPosZ); // Set player position

		// Adjusted scale to make the leg longer and narrower
		glScaled(0.04, 0.2, 0.04);
		glutSolidCube(1.0);
		glPopMatrix();
	}
	glPopMatrix();
	glFlush();
	glPopMatrix();
	glPushMatrix();

	// Draw a ground plane to make it more explicit
	drawGround();

	glPopMatrix();
	glFlush();
}


void drawTableLeg(double thick, double len) {
	glColor3f(0.8f, 0.6f, 0.2f);
	glPushMatrix();
	glTranslated(0, len / 2, 0);
	glScaled(thick, len / 2, thick);
	glutSolidCube(2.0);
	glPopMatrix();
}

void drawTable(double topWid, double topThick, double legThick, double legLen) {
	glColor3f(0.2f, 0.2f, 0.2f); // Rope color
	glPushMatrix();
	glRotated(-90, 1.0, 0.0, 0.0);
	glTranslated(0, 0.35, 0.45);
	glPushMatrix();
	glScaled(topWid, topThick, topWid / 2);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, legLen, -0.2);
	glScaled(topWid, topThick, topWid / 2);
	glutSolidCube(1.0);
	glPopMatrix();

	double dist = 0.95 * topWid / 2.0 - legThick / 2.0;

	glPushMatrix();
	glTranslated(-0.2, 0, -0.2);
	drawTableLeg(legThick, legLen);
	glPopMatrix();
	glPushMatrix();
	glTranslated(-0.2, 0, -0.1);
	drawTableLeg(legThick, legLen);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.2, 0, -0.2);
	drawTableLeg(legThick, legLen);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.2, 0, -0.1);
	drawTableLeg(legThick, legLen);
	glPopMatrix();
}

void drawCylinder(float radius, float height, int slices, int stacks) {
	GLUquadric* quadric = gluNewQuadric();
	gluCylinder(quadric, radius, radius, height, slices, stacks);
	gluDeleteQuadric(quadric);
}

void drawChairgame() {
	// Draw the central structure
	glColor3f(0.8f, 0.6f, 0.2f); // Seat color
	glPushMatrix();
	glTranslated(0.0, -0.2, 0.0);
	glScaled(0.1, 0.9, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glColor3f(0.0f, 0.8f, 1.0f);
	glPushMatrix();
	glRotated(wheelRotationAngle, 0.0, 1.0, 0.0);
	glTranslated(0.0, waterJetHeight, 0.0);
	for (int i = 0; i < 8; ++i) {
		glRotated(45.0, 0.0, 1.0, 0.0);
		glPushMatrix();
		glTranslated(0.0, 0.0, 0.05);
		glutSolidCone(0.05, 0.3, 20, 20);

		// Draw truly vertical lines at the top of each cone
		glColor3f(0.0f, 0.8f, 1.0f);
		glPushMatrix();
		glTranslated(0.0, -0.2, 0.3);  // Move to the top of the cone
		glRotated(-90.0, 1.0, 0.0, 0.0);
		drawCylinder(0.02, 0.2, 20, 20);
		glPopMatrix();

		glPopMatrix();
	}
	glPopMatrix();
}


void drawFerrisWheel() {
	// Draw the central structure
	glColor3f(0.0f, 0.8f, 1.0f);// Gray color for the central structure
	glPushMatrix();
	glTranslated(0.0, -0.2, 0.0);
	glScaled(0.1, 1.2, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glRotated(wheelRotationAngle, 0.0, 0.0, 1.0);

	glColor3f(0.8f, 0.6f, 0.2f); // Seat color
	for (int i = 0; i < 8; ++i) {
		glPushMatrix();
		double angle = i * 45.0;
		double x = 0.3 * cos(DEG2RAD(angle));
		double y = 0.3 * sin(DEG2RAD(angle));
		glTranslated(x, y, 0.3);
		glScaled(0.1, 0.1, 0.1);
		glutSolidCube(1.0);
		glPopMatrix();
	}

	glPushMatrix();
	glTranslated(0.0, 0.0, 0.3);
	glScaled(0.05, 0.05, 0.5);  // Adjust dimensions as needed
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw the lines connecting the central structure to the cabins
	glColor3f(0.5f, 0.5f, 0.5f);  // Gray color for the lines
	for (int i = 0; i < 8; ++i) {
		glPushMatrix();
		double angle = i * 45.0;
		double x = 0.3 * cos(DEG2RAD(angle));
		double y = 0.3 * sin(DEG2RAD(angle));
		glLineWidth(2.0);  // Set line width as needed
		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.5);
		glVertex3d(x, y, 0.3);
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();
}

void drawSwing() {
	// Set swing color
	glColor3f(0.5f, 0.5f, 0.5f);

	// Draw the horizontal beam of the swing
	glPushMatrix();
	glTranslated(0.1, 0.7, 0.5);
	glScaled(0.6, 0.02, 0.02);
	glutSolidCube(1);
	glPopMatrix();

	// Draw the two vertical ropes for the swing
	glColor3f(0.2f, 0.2f, 0.2f); // Rope color

	glPushMatrix();
	// Move the rotation center to the top of the beam
	glTranslatef(0.1, 0.7, 0.5);
	glRotatef(swingAngle, 1.0f, 0.0f, 0.0f);
	glTranslatef(-0.1, -0.7, -0.5);

	// Draw the two vertical ropes for the swing
	glColor3f(0.2f, 0.2f, 0.2f); // Rope color

	// Left rope
	glPushMatrix();
	glTranslated(0.2, 0.5, 0.5);
	glScaled(0.02, 0.4, 0.02);
	glutSolidCube(1);
	glPopMatrix();

	// Right rope
	glPushMatrix();
	glTranslated(0.0, 0.5, 0.5);
	glScaled(0.02, 0.4, 0.02);
	glutSolidCube(1);
	glPopMatrix();

	// Draw the swing seat
	glColor3f(0.8f, 0.6f, 0.2f); // Seat color

	glPushMatrix();
	glTranslated(0.1, 0.3, 0.5);
	glScaled(0.1, 0.01, 0.1);
	glutSolidCube(1.9);
	glPopMatrix();


	glPopMatrix();

	glPushMatrix();
	glTranslated(0.4, 0.4, 0.5);
	glScaled(0.02, 0.7, 0.02);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.2, 0.4, 0.5);
	glScaled(0.02, 0.7, 0.02);
	glutSolidCube(1);
	glPopMatrix();
}


void drawTeapot() {
	glutSolidTeapot(0.08);
}

void drawCubeWithTeapots() {
	// Draw the cube
	glColor3f(0.8f, 0.6f, 0.2f); // Seat color
	glPushMatrix();
	glTranslated(0, 0.04, 0);
	glScaled(2.0, 0.2, 2.0);
	glutSolidCube(0.35);
	glPopMatrix();
	glColor3f(0.2f, 0.2f, 0.2f); // Rope color
	// Draw four teapots on the cube
	glPushMatrix();
	glTranslated(0.2, 0.14, 0.2);
	glRotated(teaAngle, 0.00, 1.0, 0.0);
	drawTeapot();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.2, 0.14, 0.2);
	glRotated(teaAngle, 0.00, 1.0, 0.0);
	drawTeapot();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.2, 0.14, -0.2);
	glRotated(teaAngle, 0.00, 1.0, 0.0);
	drawTeapot();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.2, 0.14, -0.2);
	glRotated(teaAngle, 0.00, 1.0, 0.0);
	drawTeapot();
	glPopMatrix();
}

void drawSphere(float x, float y, float z, float radius, int slices, int stacks) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glutSolidSphere(radius, slices, stacks);
	glPopMatrix();
}

void drawCylinderr(float x, float y, float z, float radius, float height, int slices) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	drawCylinder(radius, height, slices, 1);
	glPopMatrix();
}

void drawSmileyFace() {
	// Draw face (yellow sphere)
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glutSolidSphere(0.1, 50, 50);

	// Draw eyes
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslated(-0.03, 0.03, 0.08);
	glutSolidSphere(0.02, 50, 50);

	glTranslated(0.06, 0.0, 0.0);
	glutSolidSphere(0.02, 50, 50);

	glPopMatrix();
	glPushMatrix();
	glRotated(-90, 0, 0, 1.0);
	glTranslated(-0.02, 0.05, 0);
	glColor3f(0.0f, 0.0f, 0.0f);  // Black color
	glLineWidth(2.0);
	glBegin(GL_LINE_STRIP);
	for (int i = -45; i <= 45; ++i) {
		float theta = i * 3.14159 / 180.0;
		float xPos = 0.05 * cos(theta);
		float yPos = -0.05 + 0.03 * sin(theta);
		glVertex3f(xPos, yPos, 0.1);
	}
	glEnd();
	glPopMatrix();
}

bool checkCollision(float playerX, float playerY, float playerZ,
	float smileyX, float smileyY, float smileyZ, float radius) {

	float distance = sqrt(pow(playerX - smileyX, 2) + pow(playerY - smileyY, 2) + pow(playerZ - smileyZ, 2));

	// Check if the distance is less than the radius
	if (distance / 1.4 - 0.1 < radius) {
		return true;
	}
	return false;
}

void anim(int value) {
	if (pressed) {
		waterJetHeight = 0.2 * sin(glutGet(GLUT_ELAPSED_TIME) * waterJetSpeed);
		if (accelerating) {
			wheelRotationAngle += 0.1 * glutGet(GLUT_ELAPSED_TIME) / 1000.0;  // Speed up
			if (wheelRotationAngle >= 600) {
				accelerating = false;
			}
		}
		else {
			wheelRotationAngle -= 0.1 * glutGet(GLUT_ELAPSED_TIME) / 1000.0;  // Slow down
			if (wheelRotationAngle <= 100) {
				accelerating = true;
			}
		}
		if (rightt) {
			swingAngle += 0.1 * glutGet(GLUT_ELAPSED_TIME) / 1000.0;  // Speed up
			if (swingAngle >= 20) {
				rightt = false;
			}
		}
		else {
			swingAngle -= 0.1 * glutGet(GLUT_ELAPSED_TIME) / 1000.0;  // Slow down
			if (swingAngle <= -20) {
				rightt = true;
			}
		}
		if (scale_up) {
			scale_factor += 0.01f;
			if (scale_factor >= 1.2f) {
				scale_up = false;  // Change direction to scale down
			}
		}
		else {
			scale_factor -= 0.01f;
			if (scale_factor <= 0.7f) {
				scale_up = true;  // Change direction to scale up
			}
		}
		angle += 0.1 * glutGet(GLUT_ELAPSED_TIME) / 1000.0;  // Speed up
		teaAngle -= 0.1 * glutGet(GLUT_ELAPSED_TIME) / 1000.0;  // Speed up
		// Redisplay

		// Set up the next animation frame
	}
	if (scale_upp) {
		scale_factorr += 0.01f;
		if (scale_factorr >= 1.2f) {
			scale_upp = false;  // Change direction to scale down
		}
	}
	else {
		scale_factorr -= 0.01f;
		if (scale_factorr <= 0.7f) {
			scale_upp = true;  // Change direction to scale up
		}
	}
	glutPostRedisplay();
	glutTimerFunc(32, anim, 0);  // 60 frames per second
}

void displayCameraPosition() {
	printf("Eye: (%f, %f, %f) Center: (%f, %f, %f) Up: (%f, %f, %f)\n",
		camera.eye.x, camera.eye.y, camera.eye.z,
		camera.center.x, camera.center.y, camera.center.z,
		camera.up.x, camera.up.y, camera.up.z);
}

void Display() {
	setupCamera();
	setupLights();
	if (timer == 0 && !goalCollected) {
		glClear(GL_COLOR_BUFFER_BIT);
		youLose(0);
		glFlush();
	}
	else if (goalCollected && t == 1) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		youWin();
		glFlush();
	}
	else {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//ground
		glColor3f(0.8f, 0.6f, 0.2f);
		glPushMatrix();
		glTranslated(0, 0.5 * 0.02, 0);
		glScaled(2.0, 0.02, 2.0);
		glutSolidCube(1);
		glPopMatrix();
		glColor3fv(wallColor);
		//walls
		//wall1
		glPushMatrix();
		glRotated(90, 0, 0, 1.0);
		glPushMatrix();
		glTranslated(1, 1, 0);
		glScaled(2, 0.02, 2);
		glutSolidCube(1);
		glPopMatrix();
		glPopMatrix();
		glPushMatrix();
		glRotated(90, 0, 0, 1.0);
		glPushMatrix();
		glTranslated(1, 0.98, 0);
		glScaled(0.2, 0.02, 2);
		glutSolidCube(1);
		glPopMatrix();
		glPopMatrix();
		//wall2

		glPushMatrix();
		glRotated(-90, 1.0, 0.0, 0.0);
		glTranslated(0, 1, 0);
		glPushMatrix();
		glTranslated(0, 0, 1);
		glScaled(2.0, 0.02, 2.0);
		glutSolidCube(1);
		glPopMatrix();
		glPopMatrix();
		glPushMatrix();
		glRotated(-90, 1.0, 0.0, 0.0);
		glTranslated(0, 1, 0);
		glPushMatrix();
		glRotated(-90, 0.0, 1.0, 0.0);
		glTranslated(1, -0.015, 0.02);
		glScaled(0.2, 0.02, 2.0);
		glutSolidCube(1);
		glPopMatrix();
		glPopMatrix();

		//wall3
		glPushMatrix();
		glRotated(90, 0, 0, 1.0);
		glTranslated(0, -2, 0);
		glPushMatrix();
		glTranslated(1, 1, 0);
		glScaled(2, 0.02, 2);
		glutSolidCube(1);
		glPopMatrix();
		glPopMatrix();
		glPushMatrix();
		glRotated(90, 0, 0, 1.0);
		glTranslated(0, -2, 0);
		glPushMatrix();
		glTranslated(1, 1.02, 0);
		glScaled(0.2, 0.02, 2);
		glutSolidCube(1);
		glPopMatrix();
		glPopMatrix();

		drawPlayer();

		glPushMatrix();
		glScaled(1, 0.7, 1);
		glScalef(scale_factor, scale_factor, 1.0f);
		glTranslated(-0.5, 0.0, -0.5);
		drawTable(0.6, 0.02, 0.02, 0.3);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.5, 0.68, 0.5);  // Adjust the position of the water fountain
		drawChairgame();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.5, 0.8, -0.8);  // Adjust the position of the water fountain
		drawFerrisWheel();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-1.2, 0, 0.4);
		glRotated(90, 0.00, 1.0, 0.0);
		drawSwing();
		glPopMatrix();

		glPushMatrix();
		glRotated(angle, 0.00, 1.0, 0.0);
		drawCubeWithTeapots();
		glPopMatrix();
		if (goalCollected) {
			t = 1;
		}
		if (!goalCollected) {
			glPushMatrix();
			glTranslated(0.7, 0.18, -0.1);
			glScalef(scale_factorr, scale_factorr, 1.5f);
			drawSmileyFace();
			glPopMatrix();
			if (checkCollision(playerPosX, playerPosY, playerPosZ,
				0.7, 0.18, -0.1, 0.15)) {
				goalCollected = true;
				// Optionally, you can play a sound, display a message, or perform other actions
			}
		}
		if (pressed && !pressedonce) {
			anim(0);
			pressedonce = true;
		}

		glColor3f(1.0f, 1.0f, 1.0f);

		// Flush drawing commands
		glFlush();
	}
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;
	float step = 0.1;

	float angle = -90.f;
	float angleI = 180.0f;
	float angleR = 90.0f;
	switch (key) {
	case 'w':
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;

	case 'f':
		camera = Camera(-0.005504, 0.531817, 1.964828, -0.002969, 0.561812, 0.965281, -0.000000, 0.999550, 0.029994);
		break;
	case 't':
		camera = Camera(-0.084667, 2.702397, 0.121020, -0.050605, 1.702993, 0.126574, 0.017239, -0.004970, -0.999839);
		break;
	case 'c':
		camera = Camera(0.969772, 1.038606, -0.254380, 0.090417, 0.565897, -0.311667, -0.476134, 0.871488, 0.117492);
		break;
	case 'i':
		if (playerPosZ - step >= -0.95) {
			playerPosZ -= step;
			wasPreviousStepLeft = false;
			wasPreviousStepR = false;
			wasPreviousStepD = false;
			if (!wasPreviousStepT) {
				playerRotation = angleI;
				wasPreviousStepT = true;
			}
		}
		break;
	case 'k':
		if (playerPosZ + step <= 0.95) {
			playerPosZ += step;
			wasPreviousStepLeft = false;
			wasPreviousStepR = false;
			wasPreviousStepT = false;
			if (!wasPreviousStepD) {
				wasPreviousStepD = true;
			}
		}
		break;
	case 'j':
		// Rotate the player to the left
		if (playerPosX - step >= -0.95) {
			wasPreviousStepR = false;
			wasPreviousStepT = false;
			wasPreviousStepD = false;
			if (!wasPreviousStepLeft) {
				playerRotation = angle;
				wasPreviousStepLeft = true;
			}
			playerPosX -= step;
		}
		break;
	case 'l':
		if (playerPosX + step <= 0.95) {
			wasPreviousStepT = false;
			wasPreviousStepD = false;
			wasPreviousStepLeft = false;
			if (!wasPreviousStepR) {
				playerRotation = angleR;
				wasPreviousStepR = true;
			}
			playerPosX += step;
		}
		break;
	case 'm':
		if (pressed) {
			pressed = false;
			pressedonce = false;
			Sound(0);
		}
		else {
			pressed = true;
			SoundPower();
		}
		break;
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}
void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}




void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Park");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	Sound(0);

	glShadeModel(GL_SMOOTH);
	setDefaultCameraPosition();
	glutTimerFunc(INTERVAL, changeWallColor, 0);
	glutTimerFunc(1000, Timer, 0);
	glutTimerFunc(0, anim, 0);
	glutMainLoop();
}
