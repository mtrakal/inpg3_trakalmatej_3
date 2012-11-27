//---------------------------------------------------------------------
// Ukazkovy priklad osvìtleni kuzele
// Autor: Pavel Tisnovsky
// Modifikace: M.Fribert
// Jednoduchy program, ktery zobrazuje kuzel se zapnutym stinovanim. Je povolena ambientni i difuzni
// slozka svetla spolu s odlesky. Vzhledem k tomu, ze je pouzito
// konstantniho stinovani, jsou jasne viditelne plosky na povrchu telesa.
// Pomoci leveho tlacitka mysi lze telesem otacet, prave tlacitko slouzi
// k priblizeni nebo vzdaleni telesa.
//---------------------------------------------------------------------

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // skryje okno konzole
#include <GL/glut.h>                            // hlavickovy soubor funkci GLUTu a OpenGL

int   xnew=0, ynew=0, znew=20;                  // soucasna pozice mysi, ze ktere se pocitaji rotace a posuvy
int   xold=0, yold=0, zold=20;                  // minula pozice mysi, ze ktere se pocitaji rotace a posuvy
int   xx, yy, zz;                               // bod, ve kterem se nachazi kurzor mysi

int a=10;	// sirka kuzelove plochy
int b=10;	// vyska kuzelove plochy
int pocetPlosek = 100; // pocet plosek, ktere se vykresluji na osach
int m=0;	// x souradnice vrcholu kuzele
int n=0;	// y souradnice vrcholu kuzele
int p=0;	// z souradnice vrcholu kuzele
int krok=5;	// velikost kroku pri otaceni

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
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);       // barva pozadi obrazku
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
    glEnable(GL_LIGHTING);                      // globalni povoleni stinovani
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
void drawObjectNormal(void)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(m, n, p-b);					// posun pocatku vykreslovani objektu
	glutWireCone(a,b,pocetPlosek,pocetPlosek);	//vykreslení drátového modelu kuželové plochy
	glTranslatef(0, 0, 2*b);					// posun pocatku vykreslovani objektu
	glRotatef(180, 0.0f, 1.0f, 0.0f);
	glutWireCone(a,b,pocetPlosek,pocetPlosek);	//vykreslení drátového modelu kuželové plochy
}



//--------------------------------------------------------------------
// Tato funkce je volana pri kazdem prekresleni okna
//--------------------------------------------------------------------
void onDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// vymazani barvoveho bufferu i pameti hloubky
    setPerspectiveProjection();                 // nastaveni perspektivni kamery
    glTranslatef(0.0f, 0.0f, -50.0f);           // posun objektu dale od kamery
    glTranslatef(0.0f, 0.0f, znew);             // priblizeni ci vzdaleni objektu podle pohybu kurzoru mysi
    glRotatef(ynew, 1.0f, 0.0f, 0.0f);          // rotace objektu podle pohybu kurzoru mysi
    glRotatef(xnew, 0.0f, 1.0f, 0.0f);
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
		glutPostRedisplay();                    // prekresleni sceny
		break;
	case 'y':
		ynew=yold+krok-yy;
		yold=ynew;
		glutPostRedisplay();                    // prekresleni sceny
		break;
	case 'Y':
		ynew=yold-krok-yy;
		yold=ynew;
		glutPostRedisplay();                    // prekresleni sceny
		break;
	case 'z':
		znew=zold+krok-zz;
		zold=znew;
		glutPostRedisplay();                    // prekresleni sceny
		break;
	case 'Z':
		znew=zold-krok-zz;
		zold=znew;
		glutPostRedisplay();                    // prekresleni sceny
		break;
	case 'm':
		m=m+krok;
		glutPostRedisplay();                    // prekresleni sceny
		break;
	case 'M':
		m=m-krok;
		glutPostRedisplay();                    // prekresleni sceny
		break;
	case 'n':
		n=n+krok;
		glutPostRedisplay();                    // prekresleni sceny
		break;
	case 'N':
		n=n-krok;
		glutPostRedisplay();                    // prekresleni sceny
		break;
	case 'p':
		p=p+krok;
		glutPostRedisplay();                    // prekresleni sceny
		break;
	case 'P':
		p=p-krok;
		glutPostRedisplay();                    // prekresleni sceny
		break;
	case 'r':									// reset posuvu
		m=0;
		n=0;
		p=0;
		glutPostRedisplay();                    // prekresleni sceny
		break;
	default:                        break;
	}
}



//---------------------------------------------------------------------
// Hlavni funkce konzolove aplikace
//---------------------------------------------------------------------
int main(int argc, char **argv)
{
    glutInit(&argc, argv);                      // inicializace knihovny GLUT
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);// nastaveni dvou barvovych bufferu a pameti hloubky
    glutInitWindowPosition(30, 30);             // pocatecni pozice leveho horniho rohu okna
    glutInitWindowSize(400, 400);               // pocatecni velikost okna
    glutCreateWindow("Priklad na OpenGL cislo 42");// vytvoreni okna pro kresleni
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

