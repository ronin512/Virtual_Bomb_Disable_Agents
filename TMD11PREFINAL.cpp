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

        void updatePriority(const int & xDest, const int & yDest)
        {
             priority=level+estimate(xDest, yDest)*10; //A*
        }

        
        void nextLevel(const int & i) // i: direction
        {
             level+=(dir==8?(i%2==0?10:14):10);
        }
        
        
        const int & estimate(const int & xDest, const int & yDest) const
        {
            static int xd, yd, d;
            xd=xDest-xPos;
            yd=yDest-yPos;         


            d=static_cast<int>(sqrt(xd*xd+yd*yd));

            // Manhattan distance
            //d=abs(xd)+abs(yd);
            
            
            return(d);
        }
};




class node1
{
    
    int x1Pos;
    int y1Pos;
    
    int level1;
    
    int priority1;  

    public:
        node1(int xp, int yp, int d, int p) 
            {x1Pos=xp; y1Pos=yp; level1=d; priority1=p;}
    
        int getx1Pos() const {return x1Pos;}
        int gety1Pos() const {return y1Pos;}
        int getLevel1() const {return level1;}
        int getPriority1() const {return priority1;}

        void updatePriority1(const int & x1Dest, const int & y1Dest)
        {
             priority1=level1+estimate(x1Dest, y1Dest)*10; //A*
        }

        
        void nextLevel1(const int & i) // i: direction
        {
             level1+=(dir==8?(i%2==0?10:14):10);
        }
        
        
        const int & estimate(const int & x1Dest, const int & y1Dest) const
        {
            static int x1d, y1d, d1;
            x1d=x1Dest-x1Pos;
            y1d=y1Dest-y1Pos;         


            d1=static_cast<int>(sqrt(x1d*x1d+y1d*y1d));

            // Manhattan distance
            //d=abs(xd)+abs(yd);
            
            
            return(d1);
        }
};

bool operator<(const node & a, const node & b)
{
  return a.getPriority() > b.getPriority();
}

bool operator<(const node1 & a1, const node1 & b1)
{
  return a1.getPriority1() > b1.getPriority1();
}




string pathFind( const int & xStart, const int & yStart, 
                 const int & xFinish, const int & yFinish )
{
    static priority_queue<node> pq[2]; // list of open (not-yet-tried) nodes
    static int pqi; // pq index
    static node* n0;
    static node* m0;
    static int i, j, x, y, xdx, ydy;
    static char c;
    pqi=0;

    
    for(y=0;y<m;y++)
    {
        for(x=0;x<n;x++)
        {
            closed_nodes_map[x][y]=0;
            open_nodes_map[x][y]=0;
        }
    }

    
    n0=new node(xStart, yStart, 0, 0);
    n0->updatePriority(xFinish, yFinish);
    pq[pqi].push(*n0);
    open_nodes_map[x][y]=n0->getPriority(); // mark it on the open nodes map

    
    while(!pq[pqi].empty())
    {
        
        n0=new node( pq[pqi].top().getxPos(), pq[pqi].top().getyPos(), 
                     pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

        x=n0->getxPos(); y=n0->getyPos();

        pq[pqi].pop(); // remove the node from the open list
        open_nodes_map[x][y]=0;
        
        closed_nodes_map[x][y]=1;

       
        if(x==xFinish && y==yFinish) 
        {
            
            // by following the directions
            string path="";
            while(!(x==xStart && y==yStart))
            {
                j=dir_map[x][y];
                c='0'+(j+dir/2)%dir;
                path=c+path;
                x+=dx[j];
                y+=dy[j];
            }

            
            delete n0;
            
            while(!pq[pqi].empty()) pq[pqi].pop();           
            return path;
        }

        
        for(i=0;i<dir;i++)
        {
            xdx=x+dx[i]; ydy=y+dy[i];

            if(!(xdx<0 || xdx>n-1 || ydy<0 || ydy>m-1 || map[xdx][ydy]==1 
                || closed_nodes_map[xdx][ydy]==1))
            {
                
                m0=new node( xdx, ydy, n0->getLevel(), 
                             n0->getPriority());
                m0->nextLevel(i);
                m0->updatePriority(xFinish, yFinish);

                
                if(open_nodes_map[xdx][ydy]==0)
                {
                    open_nodes_map[xdx][ydy]=m0->getPriority();
                    pq[pqi].push(*m0);
                    
                    dir_map[xdx][ydy]=(i+dir/2)%dir;
                }
                else if(open_nodes_map[xdx][ydy]>m0->getPriority())
                {
                    
                    open_nodes_map[xdx][ydy]=m0->getPriority();
                    
                    dir_map[xdx][ydy]=(i+dir/2)%dir;

                    
                    while(!(pq[pqi].top().getxPos()==xdx && 
                           pq[pqi].top().getyPos()==ydy))
                    {                
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();       
                    }
                    pq[pqi].pop(); // remove the wanted node
                    
                    
                    if(pq[pqi].size()>pq[1-pqi].size()) pqi=1-pqi;
                    while(!pq[pqi].empty())
                    {                
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();       
                    }
                    pqi=1-pqi;
                    pq[pqi].push(*m0); 
                }
                else delete m0; 
            }
        }
        delete n0; 
    }
    return ""; 
}


string pathFind1( const int & x1Start, const int & y1Start, 
                 const int & x1Finish, const int & y1Finish )
{
    static priority_queue<node1> pq1[2]; // list of open (not-yet-tried) nodes
    static int pqi1; // pq index
    static node1* n10;
    static node1* m10;
    static int i1, j1, x1, y1, x1dx, y1dy;
    static char c1;
    pqi1=0;

    
    for(y1=0;y1<m;y1++)
    {
        for(x1=0;x1<n;x1++)
        {
            closed_nodes_map[x1][y1]=0;
            open_nodes_map[x1][y1]=0;
        }
    }

    
    n10=new node1(x1Start, y1Start, 0, 0);
    n10->updatePriority1(x1Finish, y1Finish);
    pq1[pqi1].push(*n10);
    open_nodes_map[x1][y1]=n10->getPriority1(); // mark it on the open nodes map

    
    while(!pq1[pqi1].empty())
    {
        
        n10=new node1( pq1[pqi1].top().getx1Pos(), pq1[pqi1].top().gety1Pos(), 
                     pq1[pqi1].top().getLevel1(), pq1[pqi1].top().getPriority1());

        x1=n10->getx1Pos(); y1=n10->gety1Pos();

        pq1[pqi1].pop(); // remove the node from the open list
        open_nodes_map[x1][y1]=0;
        
        closed_nodes_map[x1][y1]=1;

       
        if(x1==x1Finish && y1==y1Finish) 
        {
            
            // by following the directions
            string path="";
            while(!(x1==x1Start && y1==y1Start))
            {
                j1=dir_map[x1][y1];
                c1='0'+(j1+dir/2)%dir;
                path=c1+path;
                x1+=dx[j1];
                y1+=dy[j1];
            }

            
            delete n10;
            
            while(!pq1[pqi1].empty()) pq1[pqi1].pop();           
            return path;
        }

        
        for(i1=0;i1<dir;i1++)
        {
            x1dx=x1+dx[i1]; y1dy=y1+dy[i1];

            if(!(x1dx<0 || x1dx>n-1 || y1dy<0 || y1dy>m-1 || map[x1dx][y1dy]==1 
                || closed_nodes_map[x1dx][y1dy]==1))
            {
                
                m10=new node1( x1dx, y1dy, n10->getLevel1(), 
                             n10->getPriority1());
                m10->nextLevel1(i1);
                m10->updatePriority1(x1Finish, y1Finish);

                
                if(open_nodes_map[x1dx][y1dy]==0)
                {
                    open_nodes_map[x1dx][y1dy]=m10->getPriority1();
                    pq1[pqi1].push(*m10);
                    
                    dir_map[x1dx][y1dy]=(i1+dir/2)%dir;
                }
                else if(open_nodes_map[x1dx][y1dy]>m10->getPriority1())
                {
                    
                    open_nodes_map[x1dx][y1dy]=m10->getPriority1();
                    
                    dir_map[x1dx][y1dy]=(i1+dir/2)%dir;

                    
                    while(!(pq1[pqi1].top().getx1Pos()==x1dx && 
                           pq1[pqi1].top().gety1Pos()==y1dy))
                    {                
                        pq1[1-pqi1].push(pq1[pqi1].top());
                        pq1[pqi1].pop();       
                    }
                    pq1[pqi1].pop(); // remove the wanted node
                    
                    
                    if(pq1[pqi1].size()>pq1[1-pqi1].size()) pqi1=1-pqi1;
                    while(!pq1[pqi1].empty())
                    {                
                        pq1[1-pqi1].push(pq1[pqi1].top());
                        pq1[pqi1].pop();       
                    }
                    pqi1=1-pqi1;
                    pq1[pqi1].push(*m10); 
                }
                else delete m10; 
            }
        }
        delete n10; 
    }
    return ""; 
}










 void gotoxy (int x, int y)
	{
	    COORD coord; // coordinates
	    coord.X = x; coord.Y = y; // X and Y coordinates
	    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // moves to the coordinates
	}                
   








int main()
{
    srand((unsigned int)time(NULL));

    
    for(int y=0;y<m;y++)
    {
        for(int x=0;x<n;x++) map[x][y]=0;
    }

    
    for(int x=n/8;x<n*7/8;x++)
    {
        map[x][m/2]=1;
    }
    for(int y=m/8;y<m*7/8;y++)
    {
        map[n/2][y]=1;
    }
    
    
    int xA, yA, xB, yB;
    switch(rand()%8)
    {
        case 0: xA=0;yA=0;xB=n-1;yB=m-1; break;
        case 1: xA=0;yA=m-1;xB=n-1;yB=0; break;
        case 2: xA=n/2-1;yA=m/2-1;xB=n/2+1;yB=m/2+1; break;
        case 3: xA=n/2-1;yA=m/2+1;xB=n/2+1;yB=m/2-1; break;
        case 4: xA=n/2-1;yA=0;xB=n/2+1;yB=m-1; break;
        case 5: xA=n/2+1;yA=m-1;xB=n/2-1;yB=0; break;
        case 6: xA=0;yA=m/2-1;xB=n-1;yB=m/2+1; break;
        case 7: xA=n-1;yA=m/2+1;xB=0;yB=m/2-1; break;
    }
    
    int xA1, yA1, xB1, yB1;
    switch(rand()%8)
    {
        case 0: xA1=0;yA1=0;xB1=n-1;yB1=m-1; break;
        case 1: xA1=0;yA1=m-1;xB1=n-1;yB1=0; break;
        case 2: xA1=n/2-1;yA1=m/2-1;xB1=n/2+1;yB1=m/2+1; break;
        case 3: xA1=n/2-1;yA1=m/2+1;xB1=n/2+1;yB1=m/2-1; break;
        case 4: xA1=n/2-1;yA1=0;xB1=n/2+1;yB1=m-1; break;
        case 5: xA1=n/2+1;yA1=m-1;xB1=n/2-1;yB1=0; break;
        case 6: xA1=0;yA1=m/2-1;xB1=n-1;yB1=m/2+1; break;
        case 7: xA1=n-1;yA1=m/2+1;xB1=0;yB1=m/2-1; break;
    }
    
    
    
    
    
cout << "                                                 \n";    
cout << "*************************************************\n";
cout << "          BOMB DISABLER                         *\n";
cout << " BY CHRIS VYZANTIOS & JERRY MOSCHOPOULOS        *\n";
cout << "*************************************************\n"; 
cout << "                                                 \n";
cout << "                                                 \n";
cout << "A1-A5: agents                                    \n";
cout << "D: Deminer                                       \n";
cout << "NOW SEE THE AGENTS IN ACTION!!!!!                \n";

char program_start;

do
{ 
  cout << endl; 
  cout << "Type s and press enter so the program starts: ";
  cin >> program_start;

  if (program_start!='s')
  {
   cout << endl;
   cout << "You may only type s and then the program will start!";   
   cout << endl;
  }


} while (program_start!='s');

    
    cout<<"Map Size (X,Y): "<<n<<","<<m<<endl;
    cout<<"Start1: "<<xA<<","<<yA<<endl;
    cout<<"Start2: "<<xA1<<","<<yA1<<endl;
    cout<<"Finish: "<<xB<<","<<yB<<endl;
    
    clock_t start = clock();
    string route=pathFind(xA, yA, xB, yB);
    string route1=pathFind(xA1, yA1, xB1, yB1);
    if(route=="") cout<<"An empty route generated!"<<endl;
    clock_t end = clock();
    double time_elapsed = double(end - start);
    cout<<"Time to calculate the route (ms): "<<time_elapsed<<endl;
    cout<<"Route:"<<endl;
    cout<<route<<endl<<endl;

     
    if (route.length()>0)
    {
        int j; char c;
        int x=xA;
        int y=yA;
        map[x][y]=2;
        for(int i=0;i<route.length();i++)
        {
            c =route.at(i);
            j=atoi(&c); 
            x=x+dx[j];
            y=y+dy[j];
            map[x][y]=3;
        }
        map[x][y]=4;
    }
    if (route1.length()>0)
    {
        int j1; char c1;
        int x1=xA1;
        int y1=yA1;
        map[x1][y1]=5;
        for(int i=0;i<route1.length();i++)
        {
            c1 =route1.at(i);
            j1=atoi(&c1); 
            x1=x1+dx[j1];
            y1=y1+dy[j1];
            map[x1][y1]=6;
        }
        map[x1][y1]=7;
    
    }
    
    
    
    
    
    
    
    
    
        
        for(int y=0;y<m;y++)
        {
            for(int x=0;x<n;x++)
                if(map[x][y]==0)
                    cout<<".";
                else if(map[x][y]==1)
                    cout<<"A1";
                
                else if(map[x][y]==2)
                    cout<<"A2"; 
                else if(map[x][y]==3)
                    cout<<"A3"; 
                else if (map[x][y]==5)
                     cout << "A4";
                else if (map[x][y]==6)
                     cout << "A5";     
                    else if(map[x][y]==4)
                    {
                    cout << "D";
                    cout << " ";
                    cout << "BOMB DISABLED!";
                    }
            cout<<endl;
        }
        
        
       while (true)
	   {
	        gotoxy(0,0); // move to where we want to output
	        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	        cout << "BOMB DISABLED!"; // overwrite the current output
	        Sleep(1000);
	        gotoxy(0,0); // move back to the start of output
	        cout << "                    "; // this will reset the output to blank
	        Sleep(500);
	    }
	 
	    cin.get();

  /*cout << endl;
  cout << "Press any key to continue..";	
  std::cin.get();
  std::cin.ignore();*/	
  
    return (0);
}
