/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj¹ osi 
//  uk³adu wspó³rzêdnych dla rzutowania perspektywicznego

/*************************************************************************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>


typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
// inicjalizacja po³o¿enia obserwatora

static GLfloat theta = 0.0;   
// k¹t obrotu obiektu wokó³ osi x

static GLfloat gamma = 0.0;
// k¹t obrotu obiektu wokó³ osi y

static GLfloat pix2angle;     
static GLfloat piy2angle;
// przelicznik pikseli na stopnie

static GLint status_left = 0;    
static GLint status_right = 0;
// stan klawiszy myszy
// 0 - nie naciœniêto ¿adnego klawisza
// 1 - naciœniêty zostaæ lewy klawisz

static int delta_x = 0;        
// ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
// i poprzedni¹ kursora myszy (x)

static int delta_y = 0;
// ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
// i poprzedni¹ kursora myszy (y)

static int delta_zoom = 0;
// ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
// i poprzedni¹ zoom

static int x_pos_old = 0;
// poprzednia pozycja kursora myszy (x)

static int y_pos_old = 0;
// poprzednia pozycja kursora myszy (y)

static int zoom_pos_old = 0;
// poprzednia pozycja zoom


/*************************************************************************************/

// Funkcja rysuj¹ca osie uk³adu wspó?rz?dnych
void Axes(void)
{

    point3  x_min = { -5.0, 0.0, 0.0 };
    point3  x_max = { 5.0, 0.0, 0.0 };
    // pocz?tek i koniec obrazu osi x

    point3  y_min = { 0.0, -5.0, 0.0 };
    point3  y_max = { 0.0,  5.0, 0.0 };
    // pocz?tek i koniec obrazu osi y

    point3  z_min = { 0.0, 0.0, -5.0 };
    point3  z_max = { 0.0, 0.0,  5.0 };
    //  pocz?tek i koniec obrazu osi y
    glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
    glBegin(GL_LINES); // rysowanie osi x
    glVertex3fv(x_min);
    glVertex3fv(x_max);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
    glBegin(GL_LINES);  // rysowanie osi y

    glVertex3fv(y_min);
    glVertex3fv(y_max);
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
    glBegin(GL_LINES); // rysowanie osi z

    glVertex3fv(z_min);
    glVertex3fv(z_max);
    glEnd();

}

/*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia wartoœci odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{


    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        x_pos_old = x;       
        y_pos_old = y;
        // przypisanie aktualnie odczytanej pozycji kursora
        // jako pozycji poprzedniej
        status_left = 1;          
        // wciêniêty zosta³ lewy klawisz myszy
    }
    else if(btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        zoom_pos_old = x;
        // przypisanie aktualnie odczytanej pozycji kursora
        // jako pozycji poprzedniej
        status_right = 1;
        // wciêniêty zosta³ prawy klawisz myszy
    }
    else
    {
        status_left = 0;
        status_right = 0;
    }
    // nie zosta³ wciêniêty ¿aden klawisz
}

/*************************************************************************************/
// Funkcja "monitoruje" po³o¿enie kursora myszy i ustawia wartoœci odpowiednich
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{

    delta_x = x - x_pos_old;     
    delta_y = y - y_pos_old;
    delta_zoom = x - x_pos_old;
    // obliczenie ró¿nicy po³o¿enia kursora myszy
    
    x_pos_old = x;  
    y_pos_old = y;
    zoom_pos_old = x;
    // podstawienie bie¿¹cego po³o¿enia jako poprzednie

    glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/

/*************************************************************************************/

// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana, gdy trzeba
// przerysowaæ scenê)



void RenderScene(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszcz¹cym

    glLoadIdentity();
    // Czyszczenie macierzy bie??cej

    gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // Zdefiniowanie po³o¿enia obserwatora
    Axes();
    // Narysowanie osi przy pomocy funkcji zdefiniowanej powy¿ej

    if (status_left == 1)                     // jeœli lewy klawisz myszy wciêniêty
    {
        theta += delta_x * pix2angle;    
        gamma += delta_y * piy2angle;
        // modyfikacja k¹ta obrotu o kat proporcjonalny
        // do ró¿nicy po³o¿eñ kursora myszy
    }                                 
    if (status_right == 1)                     // jeœli prawy klawisz myszy wciêniêty
    {
        viewer[2] += delta_zoom * pix2angle;
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    // Ustawienie koloru rysowania na bia³y

    glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k¹t (x)
    glRotatef(gamma, 1.0, 0.0, 0.0);  //obrót obiektu o nowy k¹t (y)

    glutWireTeapot(3.0);
    // Narysowanie czajnika
    glFlush();
    // Przekazanie poleceñ rysuj¹cych do wykonania
    glutSwapBuffers();



}
/*************************************************************************************/

// Funkcja ustalaj¹ca stan renderowania



void MyInit(void)
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Kolor czyszcz¹cy (wype³nienia okna) ustawiono na czarny

}

/*************************************************************************************/


// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹ 
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.



void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
    pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie (x)
    piy2angle = 360.0 / (float)vertical; // przeliczenie pikseli na stopnie (y)

    glMatrixMode(GL_PROJECTION);
    // Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

    glLoadIdentity();
    // Czyszcznie macierzy bie¿¹cej

    gluPerspective(70, 1.0, 1.0, 30.0);
    // Ustawienie parametrów dla rzutu perspektywicznego


    if (horizontal <= vertical)
        glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

    else
        glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
    // Ustawienie wielkoœci okna okna widoku (viewport) w zale¿noœci
    // relacji pomiêdzy wysokoœci¹ i szerokoœci¹ okna

    glMatrixMode(GL_MODELVIEW);
    // Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu  

    glLoadIdentity();
    // Czyszczenie macierzy bie¿¹cej

}

/*************************************************************************************/

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli



void main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(300, 300);

    glutCreateWindow("Rzutowanie perspektywiczne");

    glutDisplayFunc(RenderScene);
    // Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
    // (callback function).  Bêdzie ona wywo³ywana za ka¿dym razem
    // gdy zajdzie potrzeba przerysowania okna


    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
    // za zmiany rozmiaru okna                       


    glutMouseFunc(Mouse);
    // Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie stanu myszy

    glutMotionFunc(Motion);
    // Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie ruchu myszy
    
    glEnable(GL_DEPTH_TEST);
    // W³¹czenie mechanizmu usuwania niewidocznych elementów sceny

    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/