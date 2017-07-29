//
//  main.cpp
//  RushHourSMAStar
//
//  Created by YUQI ZHU on 10/9/16.
//  Copyright © 2016 YUQI ZHU. All rights reserved.
//

#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
#define MAX 999


vector<vector<vector<int>>> used_queue;////for store the nodes that have been generated
vector<vector<vector<int>>> badNode;//the node will never regenarate

int CAR=0;
int BUS=0;

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
    int fsv;//forgetten successor value
    int depth;//node's depth for indicate the depth of path
    node *father;//the father node
    node(vector<vehicle> v1)//update board when get a new node
    {
        fsv=MAX;
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
        father=NULL;
    }
    node(){fsv=MAX;father=NULL;}/////////////////////////////
    
    node& operator = (node n)
    {
        b=n.b;
        child=n.child;
        father=n.father;
        v=n.v;
        depth=n.depth;
        fx=n.fx;
        fsv=n.fsv;
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
    
    int a=0;
    for(auto A:used_queue)
    {
        if(A==child.b)
        {
            a=1;
            break;
        }
    }
    for(auto A:badNode)
    {
        if(A==child.b)
        {
            a=1;
            break;
        }
    }
    if(a==1)
        return true;
    else
        return false;
}


int computeH(node n)//compute h(n)
{
    int count=0;
    for(auto a:n.b[2])
        if(a!=0)
            count++;
    return count;
}
int computeG(node n)//compute g(n)
{
    return 5-n.v[0].column;
}

bool cmp(node *a,  node *b)
{
    if(a->fx==b->fx)
        return a->depth>b->depth;
    return a->fx<b->fx;
}


vector <node*> q;//priority queue



vector<node> findAllChildren(node n)
{
    vector<node> children;
    for(int i=0;i<n.v.size();i++)
    {
        if(n.v[i].dir=='h'||n.v[i].dir=='H')
        {
            if(n.v[i].column>0)//no left bound
            {
                if(n.b[n.v[i].row][n.v[i].column-1]==0)// can go left
                {
                    vector<vehicle> temp=n.v;
                    temp[i].column--;
                    node child(temp);
                    child.depth=n.depth+1;
                    child.fx=computeH(child)+computeG(child)+child.depth;
                    if(!ifUsed(child))
                        children.push_back(child);//add a child to children
                }
            }
            if(n.v[i].column+n.v[i].size-1<5)
            {
                if(n.b[n.v[i].row][n.v[i].column+n.v[i].size]==0)// can go right
                {
                    vector<vehicle> temp=n.v;
                    temp[i].column++;
                    node child(temp);
                    child.depth=n.depth+1;
                    child.fx=computeH(child)+computeG(child)+child.depth;
                    if(!ifUsed(child))
                        children.push_back(child);//add a child to children
                }
            }
            
        }
        if(n.v[i].dir=='v'||n.v[i].dir=='V')
        {
            if(n.v[i].row>0)//no top bound
            {
                if(n.b[n.v[i].row-1][n.v[i].column]==0)//can go up
                {
                    vector<vehicle> temp=n.v;
                    temp[i].row--;
                    node child(temp);
                    child.depth=n.depth+1;
                    child.fx=computeH(child)+computeG(child)+child.depth;
                    if(!ifUsed(child))
                        children.push_back(child);//add a child to children
                }
            }
            if(n.v[i].row+n.v[i].size-1<5)//no bottom bound
            {
                if(n.b[(n.v[i].row+n.v[i].size)][n.v[i].column]==0)
                {
                    vector<vehicle> temp=n.v;
                    temp[i].row++;
                    node child(temp);
                    child.depth=n.depth+1;
                    child.fx=computeH(child)+computeG(child)+child.depth;
                    if(!ifUsed(child))
                        children.push_back(child);//add a child to children
                }
            }
        }
    }
    return children;
}

void updateFx(node *n)//update f(x) of node's ancestors
{
    node *a=n;
    int temp=MAX;
    for(auto a:n->child)
    {
        if(temp>a.fx||temp==a.fx)
            temp=a.fx;
    }
    a->fx=temp;
    while(a->father!=NULL)
    {
        if(temp<a->father->fsv)
            a->father->fx=temp;
        else
            a->father->fx=a->fsv;
        a=a->father;
    }
}


int c=0;
double parent=0;
double son=0;

node *SMA(node &root, int memory)
{
    root.depth=0;
    root.fx=computeH(root)+computeG(root)+root.depth;
    q.push_back(&root);
    vector<node> children=findAllChildren(root);//for store chirdren
    vector<vector<int>> last=root.b;//use for judge if the node has been changed
    
    
    while(1)
    {
        if(c==13)
            son=double(used_queue.size()+badNode.size());
        if(c==11)
            parent=double(used_queue.size()+badNode.size());
            
            
            
        node *n=new node();
        if(q.empty())
        {
            return NULL;
        }
        n=q[0];
        used_queue.push_back(n->b);//to avoid get used node
        if(ifReach(n->v))
        {
            c++;//use for compute EBF
            return n;
        }
        if(n->b!=last)//if current n has been changed
        {
            c++;//use for compute EBF
            last=n->b;
            children=findAllChildren(*n);//create a new children set
        }
        if(children.size()==0)//there are no child, which means this way will never goin to distination
        {
            n->fx=MAX;
            badNode.push_back(n->b);//join the bad nodes vector, then this node will never regenerate
            sort(q.begin(), q.end(), cmp);
            continue;
        }
        
        node *s=new node();
        *s=children.back();
        children.pop_back();
        s->father=n;//generate a child
        n->child.push_back(*s);
        
        
        
        if((!ifReach(s->v))&&(s->depth==memory||s->depth>memory))//modify f(s) if necessary
        {
            s->fx=MAX;
            sort(q.begin(), q.end(), cmp);
        }
        
        if(children.size()==0)//if all all of n's children has been generated
        {
            updateFx(n);
            sort(q.begin(), q.end(), cmp);
        }
        
        if(q.size()==memory||q.size()>memory)
        {
            node *dump=q.back();
            q.pop_back();//delete the highest-f-cost node in queue
            node *dumpFather=dump->father;
            
            if(dumpFather!=NULL)
            {
                if(dump->fx<dumpFather->fsv)
                    dumpFather->fsv=dump->fx;
                vector <node>::iterator Iter;
                for(Iter = dumpFather->child.begin(); Iter != dumpFather->child.end(); Iter++)
                {
                    if(Iter->b==dump->b)
                    {
                        dumpFather->child.erase(Iter);
                        break;
                    }
                }
            }
            
            vector<vector<vector<int>>>::iterator Iter1;
            for(Iter1=used_queue.begin();Iter1!=used_queue.end();Iter1++)//delete the node form used_queue
            {
                if(*Iter1==dump->b)
                {
                    used_queue.erase(Iter1);
                    break;
                }
            }
        }
        
        q.push_back(s);
        sort(q.begin(), q.end(), cmp);
        
    }
    
    return NULL;
}






int main(int argc, const char * argv[]) {
    
    vector<vehicle> v;
    
    cout<<"Easy 1: input 1"<<endl<<"Easy 5: input 5"<<endl<<"Easy 9: input 9"<<endl<<"Medium 11: input 11"<<endl;
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
            
        case 9:
        {
            readInput(v, "IHC4");
            readInput(v, "BHD1");
            readInput(v, "BHF4");
            readInput(v, "BVC6");
            readInput(v, "CVE3");
            readInput(v, "CVD5");
        }
            break;
            
        case 11:
        {
            readInput(v, "IHC1");
            readInput(v, "BHE1");
            readInput(v, "BHA2");
            readInput(v, "CVC3");
            readInput(v, "CHA5");
            readInput(v, "CHB5");
            readInput(v, "CVC6");
            readInput(v, "CVE6");
        }
            break;
            
        default:
            cout<<"Wrong input!"<<endl;
            
            return 0;
            
    }
    //given the memory
    node root(v);
    printBoard(root.b);
    node *result=SMA(root, 50);
    
    
    
    cout<<"The depth of best solution is: "<<result->depth<<endl;
    cout<<"Visited nodes: "<<c<<endl<<endl<<"Solution:"<<endl;
    cout<<"EBF is: "<<sqrt(son/parent)<<endl;
    vector<vector<vector<int>>> solution;
    while(result)
    {
        solution.push_back(result->b);
        result=result->father;
    }
    while(solution.size())
    {
        printBoard(solution.back());
        solution.pop_back();
    }
    
    return 0;
}
























