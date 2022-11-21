// Group Members

//  Muhammad Arfat 21I-0554
//  Sufian Sajjad 21I-0688
//  Muhammad Abdullah Hassan 21I-0778
//  Asjad Rauf 21i-0493




//BONUS DATASETS USED

//Wiki votes : https://snap.stanford.edu/data/wiki-Vote.html
//email-EuAll https://snap.stanford.edu/data/email-EuAll.html



//Including the required libraries

#include <iostream>
#include <map>
#include <list>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>



// Setting Preferences
std::string OUTPUT_FILE_NAME = "output.txt";
const bool OUTPUT_TO_FILE = true;
const bool OUTPUT_TO_CONSOLE = true;

std::string DATASET_FILE="twitter_combined.txt";
bool fileFound=true;





using namespace std;

//Global Variables
map<int, list<int> > graph;
map<int,list<int> > reverseGraph;
map<int,bool> visited;
bool writeToFile = false;

//funtion to append to file on the same line
void appendToFile(string text)
{
    ofstream file;
    file.open("./"+OUTPUT_FILE_NAME, ios::app);
    file << text;
    file.close();
}
//function to append to add new line ie new component to file
void appendLine()
{
    ofstream file;
    file.open("./"+OUTPUT_FILE_NAME, ios::app);
    file << endl;
    file.close();

}




//Function to read the graph from the file
void getGraph(string filename)
{
    ifstream file(filename);
    if(!file)
    {
        cout<<"File not found !!"<<endl;
        fileFound=false;
        return;
    }
    
    if(OUTPUT_TO_FILE)
    {
        OUTPUT_FILE_NAME="["+filename+"]{output}.txt";
    }

    string line;
    int node1,node2;
    while(file>>node1>>node2)
    {
        // cout<<int(node1)<<" "<<int(node2)<<endl;
        (graph)[node1].push_back(node2);
        // (*graph)[node2].push_back(node1);
    }
    
}


int DepthFirstTraversal(int& n, int V)
{
    
    visited[V] = true;
   
    n++;
    
    // Recursion for SCC
    list<int>::iterator i;
    for (i = reverseGraph[V].begin(); i != reverseGraph[V].end(); ++i)
    {
        if (!visited[*i])
        {
            DepthFirstTraversal(n,*i);
        }
           
    }
    if(OUTPUT_TO_FILE && writeToFile)
    {
        appendToFile(to_string(V)+" ");
    }
    return n;
}

void recursiveDFS(int start, list<int> &stack)
{
    visited[start] = true;
    
    list<int>::iterator it;
    for (it = graph[start].begin(); it != graph[start].end(); it++)
    {
        if(!visited[*it])
        {
            recursiveDFS(*it,stack);
        }
    }

    stack.push_back(start);


   
}

void FindSSC()
{
    //Setting all the nodes as unvisited
    for(auto i:graph)
    {
        visited[i.first]=false;
    }
    



    //Stack for topological Order
    list<int> stack;
    for(auto it : graph)
    {
        if(!visited[it.first])
        {
            recursiveDFS(it.first,stack);
        }
    }


     //Reverse the graph [Transpose]
     for(auto it : graph )
     {
         list<int>::iterator it2;
         for(it2 = graph[it.first].begin(); it2 != graph[it.first].end(); it2++)
         {
             reverseGraph[*it2].push_back(it.first);
         }
     }

    //Clear visited [marking all nodes as not visited]
    visited.clear();

    //allowing to write to file
    writeToFile = true;

     //DFS on reverse graph
    int count;
    int countOfUniqueSSC=0;
    int largest=0;
     while(!stack.empty())
     {
         count=0;
         int node = stack.back();
         stack.pop_back();
         if(!visited[node])
         {
             count = DepthFirstTraversal(count,node);
             if(count>largest)
             {
                 largest=count;
             }
             
            countOfUniqueSSC+=1;
            if (OUTPUT_TO_FILE && writeToFile)
            {
                appendLine();
            }
          
            

         }
     }
    


    cout<<"   Total number of Connected Components "<<countOfUniqueSSC<<endl;
    cout<<"   Number of Nodes in the  Largest SSC: " << largest << endl;


    
}




int main()
{
    string dataset="email-EuAll.txt";


    cout<<"\n ____________________________________________________________________\n";
    cout<<"|                                                                     |\n";
    cout<<"|               DISCERETE SEMESTER PROJECT                            |\n";
    cout<<"             Running Program on : "<<dataset<< "                          \n";
    cout<<"|_____________________________________________________________________|\n\n";



    getGraph(dataset);

    if(!fileFound)
    {
        return 0;
    }


    //to calculate time taken using time.h
    clock_t t;
    t = clock();
    FindSSC(); //function to find the strongly connected components
    t = clock() - t;


    cout << "   Time taken:  " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;


    if(OUTPUT_TO_FILE)
    {
        cout<<"   Output written to file "<<OUTPUT_FILE_NAME<<endl;
    }


    cout<<endl;
}
