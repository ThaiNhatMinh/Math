#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cmath>

static int gs_iWidth = 640;
static int gs_iHeight = 480;
static HDC gs_hWindowDC = (HDC)0;

static double gs_fLFrustum = -0.5500;
static double gs_fRFrustum = +0.5500;
static double gs_fBFrustum = -0.4125;
static double gs_fTFrustum = +0.4125;
static double gs_fNFrustum = +1.0;
static double gs_fFFrustum = +100.0;

static int gs_iXPort = 0;
static int gs_iYPort = 0;
static int gs_iWPort = gs_iWidth;
static int gs_iHPort = gs_iHeight;

static double gs_adEye[3] = { 0.0, 0.0, 4.0 };
static double gs_adDir[3] = { 0.0, 0.0, -1.0 };
static double gs_adUp[3] = { 0.0, 1.0, 0.0 };
static double gs_adRight[3] = { 1.0, 0.0, 0.0 };

static float gs_afVertex0[3] = { 1.0f, 0.0f, 0.0f };
static float gs_afVertex1[3] = { -1.0f, 1.0f, 0.0f };
static float gs_afVertex2[3] = { -1.0f, -1.0f, 0.0f };
static float gs_afColor0[3] = { 1.0f, 0.0f, 0.0f }; // red
static float gs_afColor1[3] = { 0.0f, 1.0f, 0.0f }; // green
static float gs_afColor2[3] = { 0.0f, 0.0f, 1.0f }; // blue

// translation vector for triangle
static float gs_afTranslate[3] =
{
	0.0f, 0.0f, 0.0f
};

// rotation matrix for triangle
static float gs_aafRotate[3][3] =
{
	{1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f}
};

// transformation matrix for triangle (in OpenGL format)
static float gs_afMatrix[16] =
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

// for incremental translations
static float gs_fDPosition = 0.1f;
// for incremental rotations
static float gs_afAngle = 0.1f;
static float gs_fCos = cosf(gs_afAngle);
static float gs_fSin = sinf(gs_afAngle);


void main()
{
	// register the window class
	static char s_acWindowClass[] = "Wild Magic Application";
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = s_acWindowClass;
	wc.lpszMenuName = NULL;
	RegisterClass(&wc);

	// require the window to have the specified client area
	RECT kRect = { 0, 0, gs_iWidth-1, gs_iHeight-1 };
	AdjustWindowRect(&kRect,WS_OVERLAPPEDWINDOW,false);
	// create the application window
	static char s_acWindowTitle[] = "Draw Triangle";
	int iXPos = 0, iYPos = 0;
	int iWidth = kRect.right - kRect.left + 1;
	int iHeight = kRect.bottom - kRect.top + 1;
	HWND hWnd = CreateWindow(s_acWindowClass,s_acWindowTitle,
	WS_OVERLAPPEDWINDOW,iXPos,iYPos,iWidth,iHeight,(HWND)0,
	(HMENU)0,hInstance,NULL);
	// create a window for rendering
	gs_hWindowDC = GetDC(hWnd);

		// select format for drawing surface
	PIXELFORMATDESCRIPTOR kPFD;
	memset(&kPFD,0,sizeof(PIXELFORMATDESCRIPTOR));
	kPFD.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	kPFD.nVersion = 1;
	kPFD.dwFlags =
	PFD_DRAW_TO_WINDOW |
	PFD_SUPPORT_OPENGL |
	PFD_GENERIC_ACCELERATED |
	PFD_DOUBLEBUFFER;
	kPFD.iPixelType = PFD_TYPE_RGBA;
	kPFD.cColorBits = 24; // 24-bit colors for front/back buffers
	kPFD.cDepthBits = 16; // 16-bit depth buffer
	kPFD.cStencilBits = 8; // 8-bit stencil buffer

	int iPixelFormat = ChoosePixelFormat(gs_hWindowDC,&kPFD);
	if ( iPixelFormat == 0 )
	{
		ReleaseDC(hWnd,gs_hWindowDC);
		return ;
	}
	BOOL bSuccess = SetPixelFormat(gs_hWindowDC,iPixelFormat,&kPFD);
	if ( !bSuccess )
	{
		ReleaseDC(hWnd,gs_hWindowDC);
		return ;
	}

	// create an OpenGL context
	HGLRC hWindowRC = wglCreateContext(gs_hWindowDC);
	if ( !hWindowRC )
	{
		ReleaseDC(hWnd,gs_hWindowDC);
		return ;
	}
	bSuccess = wglMakeCurrent(gs_hWindowDC,hWindowRC);
	if ( !bSuccess )
	{
		wglDeleteContext(hWindowRC);
		ReleaseDC(hWnd,gs_hWindowDC);
		return ;
	}