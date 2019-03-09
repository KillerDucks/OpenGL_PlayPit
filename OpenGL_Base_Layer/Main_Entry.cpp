#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#pragma comment(lib, "opengl32.lib")  
#pragma comment(lib, "glu32.lib") 

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

float angle = 0.0f;
float X_angle = 1.0f, Y_angle = 0.0f, Z_angle = 0.0f;

PIXELFORMATDESCRIPTOR GetPixelFormatDescriptor();
BOOL InitOpenGL(HWND hWnd);
void GLResizeWindow(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
GLuint LoadTextureImageFile(const char* filename);
void FreeCreatedTexture(GLuint texture);
void DrawScene(HDC hdc);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR pCmdLine, int nCmdShow)

{

	    // Register the window class.

	const wchar_t CLASS_NAME[] = L"OpenGL_WND_Class";

	    

	WNDCLASS wc = { };



	wc.lpfnWndProc = WindowProc;

	wc.hInstance = hInstance;

	wc.lpszClassName = CLASS_NAME;



	RegisterClass(&wc);



	    // Create the window.



	HWND hwnd = CreateWindowEx(

		0,                              // Optional window styles.

		CLASS_NAME,                     // Window class

		L"OpenGl PlayPit",    // Window text

		WS_OVERLAPPEDWINDOW,            // Window style



		// Size and position

		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,



		NULL,       // Parent window    

		NULL,       // Menu

		hInstance,  // Instance handle

		NULL        // Additional application data

	);



	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//draw a scene on a window  
		DrawScene(GetDC(hwnd));
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;

}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (uMsg)
	{

	case WM_CREATE:
		if (!InitOpenGL(hwnd)) {
			MessageBox(hwnd, TEXT("Error: Cannot initialize OpenGL"), TEXT("ERROR"),
				MB_OK | MB_ICONERROR);
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_DOWN:
			angle += 3.0f;
			X_angle = 1.0f;
			Y_angle = 0.0f;
			Z_angle = 0.0f;
			break;
		case VK_RIGHT:
			angle += 3.0f;
			X_angle = 0.0f;
			Y_angle = 1.0f;
			Z_angle = 0.0f;
			break;
		case VK_LEFT:
			angle += 3.0f;
			X_angle = 0.0f;
			Y_angle = 0.0f;
			Z_angle = 1.0f;
			break;
		case VK_UP:
			angle += 3.0f;
			X_angle = 1.0f;
			Y_angle = 1.0f;
			Z_angle = 1.0f;
			break;
		}
		break;

	case WM_SIZE:
		GLResizeWindow(hwnd, uMsg, wParam, lParam);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		// TODO: Add any drawing code here...  
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

PIXELFORMATDESCRIPTOR GetPixelFormatDescriptor()
{
	PIXELFORMATDESCRIPTOR pfd;

	pfd.bReserved = 0;
	pfd.cAccumAlphaBits = 0;
	pfd.cAccumBits = 0;
	pfd.cAccumBlueBits = 0;
	pfd.cAccumGreenBits = 0;
	pfd.cAccumRedBits = 0;
	pfd.cAlphaBits = 0;
	pfd.cAlphaShift = 0;
	pfd.cAuxBuffers = 0;
	pfd.cBlueBits = 0;
	pfd.cBlueShift = 0;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.cGreenBits = 0;
	pfd.cGreenShift = 0;
	pfd.cRedBits = 0;
	pfd.cRedShift = 0;
	pfd.cStencilBits = 0;
	pfd.dwDamageMask = 0;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.dwLayerMask = 0;
	pfd.dwVisibleMask = 0;
	pfd.iLayerType = 0;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	return pfd;
}

BOOL InitOpenGL(HWND hWnd)
{
	PIXELFORMATDESCRIPTOR pfd;
	int pixelFormat;
	HGLRC glrc;
	HDC hdc;

	hdc = GetDC(hWnd);

	if (hdc == NULL) {
		MessageBox(hWnd, TEXT("Error: Can't Get Device Context for Window"), TEXT("Error"),
			MB_OK | MB_ICONERROR);
		return FALSE;
	}

	pfd = GetPixelFormatDescriptor();

	//choose pixel format for the current device context  
	pixelFormat = ChoosePixelFormat(hdc, &pfd);

	if (pixelFormat == 0) {
		MessageBox(hWnd, TEXT("Error: Can't Choose Pixel Format"), TEXT("Error"),
			MB_OK | MB_ICONERROR);
		ReleaseDC(hWnd, hdc);
		return FALSE;
	}

	//set pixel format for current device context  
	pixelFormat = SetPixelFormat(hdc, pixelFormat, &pfd);

	if (pixelFormat == 0) {
		MessageBox(hWnd, TEXT("Error: Can't Set The Pixel Format"), TEXT("Error"),
			MB_OK | MB_ICONERROR);
		ReleaseDC(hWnd, hdc);
		return FALSE;
	}

	//get handle to the GL of windows  
	glrc = wglCreateContext(hdc);

	if (glrc == NULL) {
		MessageBox(hWnd, TEXT("Error: Can't Create GL Context"), TEXT("Error"),
			MB_OK | MB_ICONERROR);
		ReleaseDC(hWnd, hdc);
		return FALSE;
	}

	//enable OpenGL for current device context  
	if (!wglMakeCurrent(hdc, glrc)) {
		MessageBox(hWnd, TEXT("Error: Can't Make Current GL Context"), TEXT("Error"),
			MB_OK | MB_ICONERROR);
		wglDeleteContext(glrc);
		ReleaseDC(hWnd, hdc);
		return FALSE;
	}

	return true;
}

void GLResizeWindow(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	GetClientRect(hwnd, &rect);

	GLsizei width = rect.right - rect.left;
	GLsizei height = rect.bottom - rect.top;

	glViewport(0, 0, width, height);
	gluPerspective(45, 1.0 * (width / height), 1.0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

GLuint LoadTextureImageFile(const char* filename)
{
	GLuint texture = 0;
	int width, height;
	BYTE* data = NULL;
	FILE* file;

	fopen_s(&file, filename, "rb");

	if (&file == NULL) return 0;

	width = 256;
	height = 256;
	data = (BYTE*)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_BGRA_EXT, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_TEXTURE_ENV_COLOR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	free(data);
	return texture;
}

void FreeCreatedTexture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}

void DrawScene(HDC hdc)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0.0f, 0.2f, 0.0f);
	glRotatef(angle, X_angle, Y_angle, Z_angle);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, LoadTextureImageFile("textures/blocks.bmp"));

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.25f, 0.10f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.05f, 0.10f, 0.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.05f, -0.40f, 0.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.25f, -0.40f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.8f, 0.8f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.25f, 0.10f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.25f, 0.10f, 0.30f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-0.25f, -0.40f, 0.30f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.25f, -0.40f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.05f, 0.10f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.05f, 0.10f, 0.30f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.05f, -0.40f, 0.30f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.05f, -0.40f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.25f, 0.10f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.05f, 0.10f, 0.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.05f, 0.10f, 0.30f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.25f, 0.10f, 0.30f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.25f, -0.40f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.05f, -0.40f, 0.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.05f, -0.40f, 0.30f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.25f, -0.40f, 0.30f);
	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, LoadTextureImageFile("textures/rocks.bmp"));
	glPushMatrix();
	glTranslatef(-0.5f, 0.4f, 0.0f);
	glRotatef(angle, X_angle, Y_angle, Z_angle);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0f, 0.3f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.15f, -0.1f, 0.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-0.15f, -0.1f, 0.0f);
	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	GLUquadric * quad = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, LoadTextureImageFile("textures/blocks5.bmp"));
	gluQuadricTexture(quad, 1);
	glPushMatrix();
	glTranslatef(0.4f, 0.5f, 0.0f);
	glRotatef(angle, X_angle, Y_angle, Z_angle);
	glColor3f(1.0f, 1.0f, 1.0f);
	gluCylinder(quad, 0.1f, 0.1f, 0.6f, 40, 40);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	GLUquadric * quad2 = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, LoadTextureImageFile("textures/blocks8.bmp"));
	gluQuadricTexture(quad2, 1);
	glPushMatrix();
	glTranslatef(0.4f, -0.5f, 0.0f);
	glRotatef(angle, X_angle, Y_angle, Z_angle);
	glColor3f(1.0f, 1.0f, 1.0f);
	gluCylinder(quad2, 0.1f, 0.1f, 0.6f, 40, 40);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	SwapBuffers(hdc);
}