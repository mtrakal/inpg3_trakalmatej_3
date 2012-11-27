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

#include <GL/glut.h>                            // hlavickovy soubor funkci GLUTu a OpenGL



enum {                                          // operace, ktere se mohou provadet s mysi:
    ROTATE,                                     // rotace objektu
    TRANSLATE,                                  // posun objektu
} operation=ROTATE;

int   xnew=0, ynew=0, znew=20;                  // soucasna pozice mysi, ze ktere se pocitaji rotace a posuvy
int   xold=0, yold=0, zold=20;                  // minula pozice mysi, ze ktere se pocitaji rotace a posuvy
int   xx, yy, zz;                               // bod, ve kterem se nachazi kurzor mysi

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
    glutSolidCone(10,10,32,32);
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
    if (key>='A' && key<='Z')                   // uprava velkych pismen na mala
        key+='a'-'A';                           // pro zjednoduseni prikazu switch

    switch (key) {
        case 27:    exit(0);            break;  // ukonceni aplikace
        case 'q':   exit(0);            break;  // ukonceni aplikace
        default:                        break;
    }
}



//---------------------------------------------------------------------
// Tato funkce je volana pri stisku ci pusteni tlacitka mysi
//---------------------------------------------------------------------
void onMouseButton(int button, int state, int x, int y)
{
    if (button==GLUT_LEFT_BUTTON) {             // pri zmene stavu leveho tlacitka
        operation=ROTATE;
        if (state==GLUT_DOWN) {                 // pri stlaceni tlacitka
            xx=x;                               // zapamatovat pozici kurzoru mysi
            yy=y;
        }
        else {                                  // pri pusteni tlacitka
            xold=xnew;                          // zapamatovat novy pocatek
            yold=ynew;
        }
        glutPostRedisplay();                    // prekresleni sceny
    }
    if (button==GLUT_RIGHT_BUTTON) {
        operation=TRANSLATE;
        if (state==GLUT_DOWN) zz=y;             // pri stlaceni tlacitka zapamatovat polohu kurzoru mysi
        else zold=znew;                         // pri pusteni tlacitka zapamatovat novy pocatek
        glutPostRedisplay();                    // prekresleni sceny
    }
}



//---------------------------------------------------------------------
// Tato funkce je volana pri pohybu mysi se stlacenym tlacitkem.
// To, ktere tlacitko je stlaceno si musime predem zaznamenat do
// globalni promenne stav ve funkci onMouseButton()
//---------------------------------------------------------------------
void onMouseMotion(int x, int y)
{
    switch (operation) {
        case ROTATE:                            // stav rotace objektu
            xnew=xold+x-xx;                     // vypocitat novou pozici
            ynew=yold+y-yy;
            glutPostRedisplay();                // a prekreslit scenu
            break;
        case TRANSLATE:                         // stav priblizeni/oddaleni objektu
            znew=zold+y-zz;                     // vypocitat novou pozici
            glutPostRedisplay();                // a prekreslit scenu
            break;
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
    glutMouseFunc(onMouseButton);               // registrace funkce volane pri stlaceni ci pusteni tlacitka
    glutMotionFunc(onMouseMotion);              // registrace funkce volane pri pohybu mysi se stlacenym tlacitkem
    onInit();                                   // inicializace vykreslovani
    glutMainLoop();                             // nekonecna smycka, kde se volaji zaregistrovane funkce
    return 0;                                   // navratova hodnota vracena operacnimu systemu
}



//---------------------------------------------------------------------
// Konec zdrojoveho souboru
//---------------------------------------------------------------------

