//---------------------------------------------------------------------
// Autor: Pavel Tisnovsky
// Modifikace: Matej Trakal (mtrakal.cz)
// Semestrální práce èíslo 3. na pøedmìt INPG3.
//---------------------------------------------------------------------

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // skryje okno konzole

#define _USE_MATH_DEFINES // kvùli M_PI, naèítá se z math.h

#include <GL/glut.h>                            // hlavickovy soubor funkci GLUTu a OpenGL
#include <math.h>

int   xnew=0, ynew=0, znew=0,zoomnew=-6;                  // soucasna pozice mysi, ze ktere se pocitaji rotace a posuvy
int   xold=0, yold=0, zold=0,zoomold=-6;                  // minula pozice mysi, ze ktere se pocitaji rotace a posuvy
int   xx, yy, zz,zoomz;                               // bod, ve kterem se nachazi kurzor mysi

int a=2;	// sirka kuzelove plochy
int b=2;	// vyska kuzelove plochy
float pocetPlosek = 100; // pocet plosek, ktere se vykresluji na osach
int m=0;	// x souradnice vrcholu kuzele
int n=0;	// y souradnice vrcholu kuzele
int p=0;	// z souradnice vrcholu kuzele
int krok=5;	// velikost kroku pri otaceni
int krokposuv=1; // krok pro posuv

int   windowWidth;                              // sirka okna
int   windowHeight;                             // vyska okna

// parametry, ktere ovlivnuji osvetleni
GLfloat materialAmbient[]={0.4f, 0.4f, 0.4f, 1.0f};  // ambientni slozka barvy materialu
GLfloat materialDiffuse[]={1.0f, 0.4f, 0.4f, 1.0f};  // difuzni slozka barvy materialu
GLfloat materialSpecular[]={1.0f, 1.0f, 1.0f, 1.0f}; // barva odlesku
GLfloat materialShininess[]={50.0f};                 // faktor odlesku
GLfloat light_position0[]={1.0f, 1.0f, -1.0f, 0.0f};   // pozice 1.svetla
GLfloat light_position1[]={-1.0f, -1.0f, 1.0f, 0.0f};   // pozice 2.svetla

//---------------------------------------------------------------------
// Funkce pro inicializaci vykreslovani
//---------------------------------------------------------------------
void onInit(void)
{
	//glClearColor(0.8f, 0.8f, 0.8f, 0.0f);       // barva pozadi obrazku
	glClearDepth(1.0f);                         // implicitni hloubka ulozena v pameti hloubky
	glEnable(GL_DEPTH_TEST);                    // povoleni funkce pro testovani hodnot v pameti hloubky
	glDepthFunc(GL_LESS);                       // funkce pro testovani fragmentu
	glShadeModel(GL_FLAT);                      // nastaveni stinovaciho rezimu
	glPolygonMode(GL_FRONT, GL_FILL);           // nastaveni rezimu vykresleni modelu
	glPolygonMode(GL_BACK, GL_FILL);            // jak pro predni tak pro zadni steny
	glDisable(GL_CULL_FACE);                    // zadne hrany ani steny se nebudou odstranovat
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);    // nastaveni ambientni slozky barvy materialu
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);    // nastaveni difuzni slozky barvy materialu
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);  // nastaveni barvy odlesku
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);// nastaveni faktoru odlesku
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);      // nastaveni pozice 1.svetla
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);      // nastaveni pozice 2.svetla
	//glEnable(GL_LIGHTING);                      // globalni povoleni stinovani
	glEnable(GL_LIGHT0);                        // povoleni prvniho svetla

}

//---------------------------------------------------------------------
// Nastaveni souradneho systemu v zavislosti na velikosti okna
//---------------------------------------------------------------------
void onResize(int w, int h)                     // argumenty w a h reprezentuji novou velikost okna
{
	glViewport(0, 0, w, h);                     // viditelna oblast pres cele okno
	windowWidth=w;                              // zapamatovat si velikost okna
	windowHeight=h;
}

//---------------------------------------------------------------------
// Nastaveni perspektivni projekce
//---------------------------------------------------------------------
void setPerspectiveProjection(void)
{
	glMatrixMode(GL_PROJECTION);                // zacatek modifikace projekcni matice
	glLoadIdentity();                           // vymazani projekcni matice (=identita)
	gluPerspective(75.0, (double)windowWidth/(double)windowHeight, 0.1f, 90.0f); // nastaveni perspektivni kamery
	glMatrixMode(GL_MODELVIEW);                 // bude se menit modelova matice
	glLoadIdentity();                           // nahrat jednotkovou matici
}

//--------------------------------------------------------------------
// Vykresleni objektu
//--------------------------------------------------------------------
void kuzelPlocha() {
	float x,y,z,kruznice,radius;

	float pocetBodu = 1/pocetPlosek;

	glBegin(GL_POINTS); // kreslení bodu
	//glBegin(GL_LINE_STRIP); // kreslení èáry

	for (z = -b; z < b; z=z+pocetBodu)
	{
		radius=a*(-z/b);
		for(kruznice = 0; kruznice < 360; kruznice=kruznice+0.5f)
		{
			glColor3f(sin(kruznice * M_PI/90),1/((z*z)/b),((kruznice-180)*(kruznice-180))/(180*180)); // barvy libovolnì nìjak spoèítané
			x = radius * cos(kruznice * M_PI/180.0f); // výpoèet pozice bodu x
			y = radius * sin(kruznice * M_PI/180.0f); // výpoèet pozice bodu y
			glVertex3f(x,y,z);
		}
	}
	glEnd();
}

void drawObjectNormal(void)
{
	glTranslatef(m, n, p);					// posun pocatku vykreslovani objektu
	kuzelPlocha();
}

//--------------------------------------------------------------------
// Tato funkce je volana pri kazdem prekresleni okna
//--------------------------------------------------------------------
void onDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// vymazani barvoveho bufferu i pameti hloubky
	setPerspectiveProjection();                 // nastaveni perspektivni kamery
	glTranslatef(0.0f, 0.0f, zoomnew);             // priblizeni ci vzdaleni objektu podle +/-
	glRotatef(ynew, 1.0f, 0.0f, 0.0f);          // rotace objektu podle pohybu kurzoru mysi
	glRotatef(xnew, 0.0f, 1.0f, 0.0f);
	glRotatef(znew, 0.0f, 0.0f, 1.0f);
	glClearColor(0.0, 0.0, 0.0, 0.0);           // nastaveni mazaci barvy na cernou
	glColor3f(0.0f, 1.0f, 0.0f);                // nastaveni barvy pro kresleni
	drawObjectNormal();                         // objekt vykresleny se zapnutou mlhou
	glFlush();                                  // provedeni a vykresleni vsech zmen
	glutSwapBuffers();                          // a prohozeni predniho a zadniho bufferu
}

//---------------------------------------------------------------------
// Tato funkce je volana pri stlaceni ASCII klavesy
//---------------------------------------------------------------------
void onKeyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:										// ukonceni aplikace
		exit(0); 
		break;  
	case 'q':									// ukonceni aplikace
		exit(0);
		break;
	case 'x':
		xnew=xold+krok-xx;
		xold=xnew;
		glutPostRedisplay();
		break;
	case 'X':
		xnew=xold-krok-xx;
		xold=xnew;
		break;
	case 'y':
		ynew=yold+krok-yy;
		yold=ynew;
		break;
	case 'Y':
		ynew=yold-krok-yy;
		yold=ynew;
		break;
	case 'z':
		znew=zold+krok-zz;
		zold=znew;
		break;
	case 'Z':
		znew=zold-krok-zz;
		zold=znew;
		break;
	case '+': // pøiblížení objektu
		zoomnew=zoomold+krokposuv-zoomz;
		zoomold=zoomnew;
		break;
	case '-': // oddálení objektu
		zoomnew=zoomold-krokposuv-zoomz;
		zoomold=zoomnew;
		break;
	case 'm': // posuny objektu od støedu otáèení
		m=m+krokposuv;
		break;
	case 'M':
		m=m-krokposuv;
		break;
	case 'n':
		n=n+krokposuv;
		break;
	case 'N':
		n=n-krokposuv;
		break;
	case 'p':
		p=p+krokposuv;
		break;
	case 'P':
		p=p-krokposuv;
		break;
	case 'r':									// reset posuvu
		m=0;
		n=0;
		p=0;
		break;
	default:  
		break;
	}
	glutPostRedisplay();                    // prekresleni sceny
}

//---------------------------------------------------------------------
// Hlavni funkce konzolove aplikace
//---------------------------------------------------------------------
int main(int argc, char **argv)
{
	glutInit(&argc, argv);                      // inicializace knihovny GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);// nastaveni dvou barvovych bufferu a pameti hloubky
	glutInitWindowPosition(30, 30);             // pocatecni pozice leveho horniho rohu okna
	glutInitWindowSize(600, 600);               // pocatecni velikost okna
	glutCreateWindow("inpg3 - semestrálka, zadání èíslo 3.");// vytvoreni okna pro kresleni
	glutDisplayFunc(onDisplay);                 // registrace funkce volane pri prekreslovani okna
	glutReshapeFunc(onResize);                  // registrace funkce volane pri zmene velikosti okna
	glutKeyboardFunc(onKeyboard);               // registrace funkce volane pri stlaceni klavesy
	onInit();                                   // inicializace vykreslovani
	glutMainLoop();                             // nekonecna smycka, kde se volaji zaregistrovane funkce
	return 0;                                   // navratova hodnota vracena operacnimu systemu
}

//---------------------------------------------------------------------
// Konec zdrojoveho souboru
//---------------------------------------------------------------------

