//
//  main.cpp
//  RushHour
//
//  Created by YUQI ZHU on 9/25/16.
//  Copyright © 2016 YUQI ZHU. All rights reserved.
//

#include <iostream>
#include <vector>
#include <queue>
using namespace std;



//vector<vector<int>> board;
vector<vector<vector<int>>> used_queue;//for store the previous steps
vector<vector<vector<int>>> renew_queue;//for renew the previous steps
vector<vector<vector<int>>> solution;//for store the solution

int CAR=0;
int BUS=0;
int countVisitNode=0;
int d=0; //depth of solution node

class vehicle//class vehicle
{
public:
    vehicle(char t, char d, char r, char c)
    {
        if(t=='b'||t=='B')
        {
            type=3;
            size=3;
            id=++BUS;
        }
        else
        {
            size=2;
            if(t=='C'||t=='c')
            {
                id=++CAR;
                type=2;
            }
            else
            {
                type=9;
                id=9;
            }
        }
        dir=d;
        if(r<90)
            row=r-'A';
        else
            row=r-'a';
        column=c-'1';
    }
    vehicle &operator =(vehicle v)
    {
        type=v.type;
        dir=v.dir;
        row=v.row;
        column=v.column;
        size=v.size;
        id=v.id;
        return *this;
    }
    int type;
    int size;
    int id;
    char dir;
    int row;
    int column;
};

class node
{
public:
    vector<vehicle> v;  //for store the state of every vehicle
    vector<node> child;
    vector<vector<int>> b; //the board of every node
    int fx;//for store the value of f(x)
    node *father;//the father node
    node(vector<vehicle> v1)//update board when get a new node
    {
        v=v1;
        vector<int> temp(6,0);
        for(int i=0;i<6;i++)
            b.push_back(temp);
        
        for(auto temp:v)
        {
            if(temp.dir=='H'||temp.dir=='h')
            {
                int i;
                for(i=0;i<temp.size-1;i++)
                    b[temp.row][temp.column+i]=temp.type;
                b[temp.row][temp.column+i]=temp.id;
            }
            else
            {
                int i;
                for(i=0;i<temp.size-1;i++)
                    b[temp.row+i][temp.column]=temp.type;
                b[temp.row+i][temp.column]=temp.id;
            }
        }
    }
    node(){}
    
    node& operator = (node n)
    {
        b=n.b;
        child=n.child;
        father=n.father;
        v=n.v;
        return *this;
    }
};



void readInput(vector<vehicle> &returnv,string input)//read the input into vehicle
{
    if(input.size()>4)
    {
        cout<<"input size error";
        return;
    }
    vehicle v(input[0],input[1],input[2],input[3]);
    returnv.push_back(v);
    return;
}




void printBoard(vector<vector<int>>board)//display board
{
    for(auto iter1:board)
    {
        for(auto iter2:iter1)
        {
            cout<<iter2<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

bool ifReach(vector<vehicle> v)//test if the node is reach the end
{
    if(v[0].column==4)
        return true;
    else return false;
}



bool ifUsed(node child)//to avoid DLS created the same child
{
    for(auto A:used_queue)
    {
        if(A==child.b)
            return true;
    }
    used_queue.push_back(child.b);
    return false;
}



node* DLS(node &root, int depth)//DLS algorithm
{
    if(ifReach(root.v))
    {
        while(root.father)
        {
            solution.push_back(root.b);
            root=*root.father;
        }
        return &root;
    }
    else
    {
        if(depth==0)
            return NULL;
        
        if(depth>0)
        {
            for(int i=0;i<root.v.size();i++)
            {
                if(root.v[i].dir=='H'||root.v[i].dir=='h')//direction is horizontal
                {
                    if(root.v[i].column>0)//no left bound
                    {
                        if(root.b[root.v[i].row][root.v[i].column-1]==0)//go left
                        {
                            vector<vehicle> temp=root.v;
                            temp[i].column--;
                            node child(temp);
                            if(!ifUsed(child))
                            {
                                countVisitNode++;
                                root.child.push_back(child);
                                child.father=&root;//add a child
                                //printBoard(child.b);///////////////////
                                node*r=new node();
                                r=DLS(child, depth-1);
                                if(r)
                                    return r;
                            }
                        }
                    }
                    if(root.v[i].column+root.v[i].size-1<5)//no right bound
                    {
                        if(root.b[root.v[i].row][root.v[i].column+root.v[i].size]==0)//go right
                        {
                            vector<vehicle> temp=root.v;
                            temp[i].column++;
                            node child(temp);
                            if(!ifUsed(child))
                            {
                                countVisitNode++;
                                root.child.push_back(child);
                                child.father=&root;//add a child
                                //printBoard(child.b);///////////////////
                                node*r=new node();
                                r=DLS(child, depth-1);
                                if(r)
                                    return r;
                            }
                        }
                    }
                }
                else//direction is vertical
                {
                    if(root.v[i].row>0)//no top bound
                    {
                        if(root.b[root.v[i].row-1][root.v[i].column]==0)//go up
                        {
                            vector<vehicle> temp=root.v;
                            temp[i].row--;
                            node child(temp);
                            if(!ifUsed(child))
                            {
                                countVisitNode++;
                                root.child.push_back(child);
                                child.father=&root;//add a child
                                //printBoard(child.b);///////////////////
                                node*r=new node();
                                r=DLS(child, depth-1);
                                if(r)
                                    return r;
                                
                            }
                        }
                    }
                    
                    if(root.v[i].row+root.v[i].size-1<5)//no bottom bound
                    {
                        if(root.b[(root.v[i].row+root.v[i].size)][root.v[i].column]==0)//go down
                        {
                            vector<vehicle> temp=root.v;
                            temp[i].row++;
                            node child(temp);
                            if(!ifUsed(child))
                            {
                                countVisitNode++;
                                root.child.push_back(child);
                                child.father=&root;//add a child
                                //printBoard(child.b);///////////////////
                                node*r=new node();
                                r=DLS(child, depth-1);
                                if(r)
                                    return r;
                            }
                        }
                    }
                }// added one child there
            }
        }

        return NULL;
    }
}

node *IDDFS(node &root)//IDDFS algorithm
{
    node *found;
    int i=1;
    while(i)
    {
        used_queue.push_back(root.b);//initial used queue
        found=DLS(root, i);
        if (found)
        {
            d=i;
            return found;
        }
        i++;
        used_queue=renew_queue;
    }
    return NULL;
}

int main(int argc, const char * argv[])
{
    vector<vehicle> v;
    //input here, then run the program
    cout<<"Easy 1: input 1"<<endl<<"Easy 5: input 5"<<endl;
    cout<<"Plz input: ";
    int input;
    cin>>input;
    //Input here and then run the program
    switch (input)
    {
        case 1:
        {
            readInput(v,"IHC3");
            readInput(v,"CVA3");
            readInput(v,"BHB4");
            readInput(v,"CVC5");
            readInput(v,"CVE5");
        }
            break;
            
        case 5:
        {
            readInput(v,"IHC4");
            readInput(v,"CVA4");
            readInput(v,"BVA6");
            readInput(v,"CVD4");
            readInput(v,"CHE5");
        }
            break;
            
        default:
            cout<<"Wrong input!"<<endl;
            
            return 0;
    }

     
    node root(v);
    root.father=NULL;
    printBoard(root.b);
    IDDFS(root);
    cout<<"The number of visited nodes is: "<<countVisitNode<<endl;
    cout<<"Depth is: "<<d<<endl<<endl;
    cout<<"Solution"<<endl;
    
    while(solution.size())
    {
        printBoard(solution.back());
        solution.pop_back();
    }
    
    

    
    
    return 0;
}













































