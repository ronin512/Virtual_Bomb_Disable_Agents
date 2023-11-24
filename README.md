# Virtual_Bomb_Disable_Agents
#VIRTUAL BOMB DISABLE-ΣΥΝΕΡΓΑΣΙΑ ΕΥΦΥΩΝ ΠΡΑΚΤΟΡΩΝ ΓΙΑ ΤΗΝ  ΞΟΥΔΕΤΕΡΩΣΗ ΕΙΚΟΝΙΚΗΣ ΒΟΜΒΑΣ
#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <windows.h>

using namespace std;

const int n=60; 
const int m=60; 
static int map[n][m];
static int closed_nodes_map[n][m]; 
static int open_nodes_map[n][m]; 
static int dir_map[n][m]; 
const int dir=8; 
static int dx[dir]={1, 1, 0, -1, -1, -1, 0, 1};
static int dy[dir]={0, 1, 1, 1, 0, -1, -1, -1};

// Κλάση για τον κόμβο στον χάρτη
class node
{
    int xPos;
    int yPos;
    int level;
    int priority;

public:
    node(int xp, int yp, int d, int p) 
        {xPos=xp; yPos=yp; level=d; priority=p;}

    int getxPos() const {return xPos;}
    int getyPos() const {return yPos;}
    int getLevel() const {return level;}
    int getPriority() const {return priority;}

    // Ενημέρωση της προτεραιότητας με βάση τη συνάρτηση εκτίμησης
    void updatePriority(const int & xDest, const int & yDest)
    {
         priority=level+estimate(xDest, yDest)*10; // A*
    }

    // Αύξηση του επιπέδου κατά την επόμενη κίνηση
    void nextLevel(const int & i) 
    {
         level+=(dir==8?(i%2==0?10:14):10);
    }

    // Εκτίμηση του κόστους για την ευθεία απόσταση (Manhattan distance)
    const int & estimate(const int & xDest, const int & yDest) const
    {
        static int xd, yd, d;
        xd=xDest-xPos;
        yd=yDest-yPos;
        d=static_cast<int>(sqrt(xd*xd+yd*yd));
        // Μπορείτε να χρησιμοποιήσετε την Manhattan distance αντί για την Euclidean
        // d=abs(xd)+abs(yd);
        return(d);
    }
};

// Παρόμοια κλάση για άλλον κόμβο (φαίνεται ότι δεν χρησιμοποιείται στον κώδικα)
class node1
{
    // ... (ίδια λογική με την κλάση node)
};

// Συνάρτηση σύγκρισης για την ουρά προτεραιότητας
bool operator<(const node & a, const node & b)
{
    return a.getPriority() > b.getPriority();
}

// ... (παρόμοια για την node1)

// Συνάρτηση A* για τον πρώτο agent
string pathFind(const int & xStart, const int & yStart, 
                const int & xFinish, const int & yFinish)
{
    static priority_queue<node> pq[2];
    static int pqi;
    static node *n0;
    static node *m0;
    static int i, j, x, y, xdx, ydy;
    static char c;
    pqi = 0;

    // Αρχικοποίηση των χαρτών
    for (y = 0; y < m; y++)
    {
        for (x = 0; x < n; x++)
        {
            map[x][y] = 0;
            closed_nodes_map[x][y] = 0;
            open_nodes_map[x][y] = 0;
        }
    }

    n0 = new node(xStart, yStart, 0, 0);
    n0->updatePriority(xFinish, yFinish);
    pq[pqi].push(*n0);
    open_nodes_map[xStart][yStart] = n0->getPriority();

    // Κύριος βρόγχος A* αλγορίθμου
    while (!pq[pqi].empty())
    {
        n0 = new node(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
                      pq[pqi].top().getLevel(), pq[pqi].top().getPriority());
        x = n0->getxPos();
        y = n0->getyPos();
        pq[pqi].pop();
        open_nodes_map[x][y] = 0;
        closed_nodes_map[x][y] = 1;

        if (x == xFinish && y == yFinish)
        {
            string path = "";
            while (!(x == xStart && y == yStart))
            {
                j = dir_map[x][y];
                c = '0' + (j + dir / 2) % dir;
                path = c + path;
                x += dx[j];
                y += dy[j];
            }
            delete n0;
            while (!pq[pqi].empty())
                pq[pqi].pop();
            return path;
        }

        // Έλεγχος κινήσεων σε όλες τις δυνατές κατευθύνσεις
        for (i = 0; i < dir; i++)
        {
            xdx = x + dx[i];
            ydy = y + dy[i];

            if (!(xdx < 0 || xdx > n - 1 || ydy < 0 || ydy > m - 1 || map[xdx][ydy] == 1 ||
                  closed_nodes_map[xdx][ydy] == 1))
            {
                m0 = new node(xdx, ydy, n0->getLevel(), n0->getPriority());
                m0->nextLevel(i);
                m0->updatePriority(xFinish, yFinish);

                if (open_nodes_map[xdx][ydy] == 0)
                {
                    open_nodes_map[xdx][ydy] = m0->getPriority();
                    pq[pqi].push(*m0);
                    dir_map[xdx][ydy] = (i + dir / 2) % dir;
                }
                else if (open_nodes_map[xdx][ydy] > m0->getPriority())
                {
                    open_nodes_map[xdx][ydy] = m0->getPriority();
                    dir_map[xdx][ydy] = (i + dir / 2) % dir;

                    while (!(pq[pqi].top().getxPos() == xdx &&
                             pq[pqi].top().getyPos() == ydy))
                    {
                        pq[1 - pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pq[pqi].pop();

                    if (pq[pqi].size() > pq[1 - pqi].size())
                        pqi = 1 - pqi;
                    while (!pq[pqi].empty())
                    {
                        pq[1 - pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pqi = 1 - pqi;
                    pq[pqi].push(*m0);
                }
                else
                    delete m0;
            }
        }
        delete n0;
    }
    return "";
}

// ... (Παρόμοια συνάρτηση για τον δεύτερο agent)

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{
    srand((unsigned int)time(NULL));

    // Αρχικοποίηση του χάρτη
    for (int y = 0; y < m; y++)
    {
        for (int x = 0; x < n; x++)
            map[x][y] = 0;
    }

    // Τοποθέτηση εμποδίων
    for (int x = n / 8; x < n * 7 / 8; x++)
    {
        map[x][m / 2] = 1;
    }
    for (int y = m / 8; y < m * 7 / 8; y++)
    {
        map[n / 2][y] = 1;
    }

    // Ορισμός αρχικών και τελικών θέσεων για τους agents
    int xA, yA, xB, yB;
    switch (rand() % 8)
    {
    // ... (Ορισμός τυχαίων αρχικών και τελικών θέσεων)
    }

    // ... (Παρόμοια για τον δεύτερο agent)

    cout << "Map Size (X,Y): " << n << "," << m << endl;
    cout << "Start1: " << xA << "," << yA << endl;
    cout << "Start2: " << xA1 << "," << yA1 << endl;
    cout << "Finish: " << xB << "," << yB << endl;

    clock_t start = clock();
    string route = pathFind(xA, yA, xB, yB);
    string route1 = pathFind(xA1, yA1, xB1, yB1);
    if (route == "")
        cout << "An empty route generated!" << endl;
    clock_t end = clock();
    double time_elapsed = double(end - start);
    cout << "Time to calculate the route (ms): " << time_elapsed << endl;
    cout << "Route:" << endl;
    cout << route << endl
         << endl;

    // ... (Παρόμοια για τον δεύτερο agent)

    // Εμφάνιση του χάρτη
    for (int y = 0; y < m; y++)
    {
        for (int x = 0; x < n; x++)
            if (map[x][y] == 0)
                cout << ".";
            else if (map[x][y] == 1)
                cout << "A1";
            else if (map[x][y] == 2)
                cout << "A2";
            else if (map[x][y] == 3)
                cout << "A3";
            else if (map[x][y] == 5)
                cout << "A4";
            else if (map[x][y] == 6)
                cout << "A5";
            else if (map[x][y] == 4)
            {
                cout << "D";
                cout << " ";
                cout << "BOMB DISABLED!";
            }
        cout << endl;
    }

    // Εκτύπωση μηνύματος και αλλαγή χρώματος κειμένου
    while (true)
    {
        gotoxy(0, 0);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        cout << "BOMB DISABLED!";
        Sleep(1000);
        gotoxy(0, 0);
        cout << "                    ";
        Sleep(500);
    }

    cin.get();
    return (0


