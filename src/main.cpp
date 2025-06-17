#include <GL/glut.h>
#include <windows.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float cameraX = 149.6f, cameraY = 0.0f, cameraZ = 1.0f; // Position de la caméra ajustée pour commencer à côté de la Terre
float cameraYaw = 0.0f, cameraPitch = 0.0f;
bool rightMouseButtonDown = false;
int lastMouseX, lastMouseY;
float scaleFactor = 1.0f / 1000000.0f;               // Facteur d'échelle pour réduire les dimensions
float earthOrbitRadius = 149600000.0f * scaleFactor; // Rayon de l'orbite de la Terre (km)
float sunRadius = 696340.0f * scaleFactor;           // Rayon du Soleil (km)
float earthRadius = 6371.0f * scaleFactor;           // Rayon de la Terre (km)
float earthOrbitSpeed = 29.78f * scaleFactor;        // Vitesse orbitale de la Terre (km/s)
float earthOrbitAngle = 0.0f;                        // Angle actuel de l'orbite de la Terre
float lightSpeed = 299792.458f * scaleFactor;        // Vitesse de la lumière (km/s)
float cameraSpeed = lightSpeed;                      // Utiliser la vitesse de la lumière pour la caméra

void updateCameraPosition()
{
    float yawRad = cameraYaw * M_PI / 180.0f;
    float pitchRad = cameraPitch * M_PI / 180.0f;

    if (GetAsyncKeyState('Z') & 0x8000)
    {
        cameraX += cameraSpeed * sin(yawRad);
        cameraZ -= cameraSpeed * cos(yawRad);
    }
    if (GetAsyncKeyState('S') & 0x8000)
    {
        cameraX -= cameraSpeed * sin(yawRad);
        cameraZ += cameraSpeed * cos(yawRad);
    }
    if (GetAsyncKeyState('Q') & 0x8000)
    {
        cameraX -= cameraSpeed * cos(yawRad);
        cameraZ -= cameraSpeed * sin(yawRad);
    }
    if (GetAsyncKeyState('D') & 0x8000)
    {
        cameraX += cameraSpeed * cos(yawRad);
        cameraZ += cameraSpeed * sin(yawRad);
    }
    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        cameraY += cameraSpeed;
    }
    if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
    {
        cameraY -= cameraSpeed;
    }
}

void mouseMotion(int x, int y)
{
    if (rightMouseButtonDown)
    {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;
        cameraYaw += dx * 0.1f;
        cameraPitch -= dy * 0.1f;
        if (cameraPitch > 89.0f)
            cameraPitch = 89.0f;
        if (cameraPitch < -89.0f)
            cameraPitch = -89.0f;
    }
    lastMouseX = x;
    lastMouseY = y;
}

void mouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            rightMouseButtonDown = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else if (state == GLUT_UP)
        {
            rightMouseButtonDown = false;
        }
    }
}

void configureCamera()
{
    glLoadIdentity();
    float yawRad = cameraYaw * M_PI / 180.0f;
    float pitchRad = cameraPitch * M_PI / 180.0f;
    float lookX = sin(yawRad) * cos(pitchRad);
    float lookY = sin(pitchRad);
    float lookZ = -cos(yawRad) * cos(pitchRad);
    gluLookAt(cameraX, cameraY, cameraZ, cameraX + lookX, cameraY + lookY, cameraZ + lookZ, 0, 1, 0);
}

void configureProjection(int width, int height)
{
    if (height == 0) // Pour éviter la division par zéro
        height = 1;
    float aspect = (float)width / (float)height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.001f, 1000.0f); // Ajustement du plan de coupe lointain
    glMatrixMode(GL_MODELVIEW);
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    configureProjection(width, height);
}

void renderBitmapString(float x, float y, float z, void *font, const char *string)
{
    const char *c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void drawOrbit(float radius)
{
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f); // Couleur blanche pour l'orbite
    for (int i = 0; i < 360; i++)
    {
        float theta = i * M_PI / 180.0f;
        float x = radius * cos(theta);
        float z = radius * sin(theta);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();
}

void updateEarthPosition()
{
    earthOrbitAngle += earthOrbitSpeed; // Vitesse de rotation de la Terre
    if (earthOrbitAngle >= 360.0f)
        earthOrbitAngle -= 360.0f;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCameraPosition();
    configureCamera();

    // Dessiner le Soleil
    glColor3f(1.0f, 1.0f, 0.0f); // Couleur jaune pour le Soleil
    GLUquadric *quad = gluNewQuadric();
    gluSphere(quad, sunRadius, 100, 100); // Rayon du Soleil
    renderBitmapString(0.0f, sunRadius + 0.1f, 0.0f, GLUT_BITMAP_HELVETICA_18, "Soleil");

    // Dessiner l'orbite de la Terre
    drawOrbit(earthOrbitRadius);

    // Mettre à jour la position de la Terre
    updateEarthPosition();
    float earthX = earthOrbitRadius * cos(earthOrbitAngle * M_PI / 180.0f);
    float earthZ = earthOrbitRadius * sin(earthOrbitAngle * M_PI / 180.0f);

    // Dessiner la Terre
    glPushMatrix();
    glTranslatef(earthX, 0.0f, earthZ);
    glColor3f(0.0f, 0.0f, 1.0f);          // Couleur bleue pour la Terre
    gluSphere(quad, earthRadius, 50, 50); // Rayon de la Terre
    renderBitmapString(0.0f, earthRadius + 0.01f, 0.0f, GLUT_BITMAP_HELVETICA_18, "Terre");
    glPopMatrix();

    gluDeleteQuadric(quad);

    glutSwapBuffers();
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    configureProjection(500, 500);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    int argc = 1;
    char *argv[] = {(char *)""};

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sphere OpenGL");

    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseButton);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}