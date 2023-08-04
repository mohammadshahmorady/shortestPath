#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

class node;

struct adj
{
	node* adjacent;
	int weight;
	adj* next;
};

struct edge
{
	node* v1;
	node* v2;
	int weight;
	edge* next;
};

struct way
{
	int weight;
	short Node;
	way* next;
};

class node
{
	private:
		adj* adjList;
		int key;
		node* parent;
		bool visited;
		
	public:
		node()
		{
			key = INT_MAX;
			parent = NULL;
			adjList = NULL;
			visited = false;
		}
		~node()
		{
			adj* flag = adjList;
			while(flag)
			{
				adjList = adjList->next;
				delete flag;
				flag = adjList;
			}
		}
		void setKey(int Key){key = Key;}
		
		int getKey(){return key;}
		
		void setVisited(bool Visited){visited = Visited;}
		
		int getVisited(){return visited;}
		
		void setParent(node* Parent){parent = Parent;}
		
		node* getParent(){return parent;}
		
		void addAdj(adj* Adj)
		{
			if(!adjList)
				adjList = Adj;
			else
			{
				adj* flag = adjList;
				while(flag->next)
					flag = flag->next;
				flag->next = Adj;
			}
		}
		
		adj* getAdjs(){return adjList;}
};

int main()
{
	char a = -1;
	while( true )
	{
		ifstream input;
		cout<<"1.Prim\n2.Kruskal\n3.Dijkestra\n4.BellmanFord\n5.Floyd\n\n0.Exit\n\nEnter an option : ";
		cin>>a;
		system("cls");
		
		if (a == '1')
			input.open("Prim.txt");
		else if (a == '2')
			input.open("Kruskal.txt");
		else if (a == '3')
			input.open("Dijkestra.txt");
		else if (a == '4')
			input.open("BellmanFord.txt");
		else if (a == '5')
			input.open("Floyd.txt");
		else if ( a == '0' )
			break;
		else
		{
			cout<<"Wrong input!\n\n";
			a = -1;
		}
		if (!input)
		{
			cout<<"Couldn't read the input file\n\n";
			a = -1;
		}
		
		else if( a < '5' && a > '0')
		{
			int n;
			input>>n;
			node* graph;
			graph = new node[n];
			node* k;
			char Weight[10];
			int weight;
			for( int i = 0 ; i < n ; i++ )
			{
				k = graph + i;
				for( int j = 0 ; j < n ; j++ )
				{
					input>>Weight;
					weight = atoi(Weight);
					if(weight)
					{
						adj* newAdj = new adj;
						newAdj->weight = weight;
						newAdj->next = NULL;
						newAdj->adjacent = graph + j;
						k->addAdj(newAdj);
					}
				}
			}
			
			if (a == '1')
			{
				char b[4];
				cout<<"Choose the start node : ";
				cin>>b;
				int startNode = atoi(b);
				while( 1 > startNode || n < startNode )
				{
					cout<<endl<<endl<<"Wrong input! Choose a number between 1 and nodes number : ";
					cin>>b;
					startNode = atoi(b);
				}
				cout<<endl<<endl;
				bool queue = true;
				graph[startNode - 1].setKey(0);
				while (queue)
				{
					node* u = NULL;
					int min = INT_MAX;
					queue = false;
					for(int i = 0 ; i < n ; i++ )
						if(!graph[i].getVisited())
						{
							queue = true;
							if(graph[i].getKey() < min)
							{
								min = graph[i].getKey();
								u = graph + i;
							}
						}
					if(u)
					{
						u->setVisited(true);
						adj* Adj = u->getAdjs();
						while(Adj)
						{
							if ( !(Adj->adjacent->getVisited()) && Adj->weight < Adj->adjacent->getKey())
							{
								Adj->adjacent->setParent(u);
								Adj->adjacent->setKey(Adj->weight);
							}
							Adj = Adj->next;
						}
					}
				}
				for( int i = 0 ; i < n ; i++ )
				{
					for( int j = 0 ; j < n ; j++ )
					{
						if( graph[i].getParent() == graph + j )
						{
							adj* temp = graph[j].getAdjs();
							while(temp->adjacent != graph + i)
								temp = temp->next;
							cout<<temp->weight<<" ";
						}
						else if( graph[j].getParent() == graph + i )
						{
							adj* temp = graph[i].getAdjs();
							while(temp->adjacent != graph + j)
								temp = temp->next;
							cout<<temp->weight<<" ";
						}
						else
							cout<<"* ";
					}
					cout<<endl;
				}
				for( int i = 0 ; i < n ; i++ )
					delete (graph + i);
				a = -1;
				cout<<endl;
			}
			else if (a == '2')
			{
				edge* head = NULL;
				for( int i = 0 ; i < n ; i++ )
				{
					graph[i].setParent(graph + i);
					adj* temp = graph[i].getAdjs();
					while(temp)
					{
						edge* flag = new edge;
						flag->v1 = graph + i;
						flag->v2 = temp->adjacent;
						flag->weight = temp->weight;
						flag->next = NULL;
						temp = temp->next;
						edge* prev = head;
						if ( !head || flag->weight < head->weight)
						{
							prev = head;
							head = flag;
							head->next = prev;
						}
						else
						{
							edge* next = head->next;
							while( next && flag->weight > next->weight )
							{
								prev = next;
								next = next->next;
							}
							prev->next = flag;
							flag->next = next;
						}
					}
				}
							
				int** matrix;
				matrix = new int*[n];
				for(int i = 0 ; i < n ; i++)
				{
					matrix[i] = new int[n];
					for(int j = 0 ; j < n ; j++)
						matrix[i][j] = 0;
				}
				
				edge* flag;
				while( head )
				{
					node* p1 = head->v1;
					node* p2 = head->v2;
					while(p1->getParent() != p1)
						p1 = p1->getParent();
					while(p2->getParent() != p2)
						p2 = p2->getParent();
					if(p1 != p2)
					{
						node* i = head->v1;
						node* j = head->v2;
						int k1 = 0;
						int k2 = 0;
						while(graph + k1 != i)
							k1++;
						while(graph + k2 != j)
							k2++;
						matrix[k1][k2] = matrix[k2][k1] = head->weight;
						while(i->getParent() != i)
							i = i->getParent();
						while(j->getParent() != j)
							j = j->getParent();
						i->setParent(j);
					}
					flag = head;
					head = head->next;
					delete flag;
				}
				
				for(int i = 0 ; i < n ; i++)
				{
					for(int j = 0 ; j < n ; j++)
						if(matrix[i][j])
							cout<<matrix[i][j]<<" ";
						else
							cout<<"* ";
					cout<<endl;
				}
				cout<<endl;
				
				for( int i = 0 ; i < n ; i++ )
					delete (graph + i);
				a = -1;
				cout<<endl;
			}
			else if (a == '3')
			{
				char b[4];
				cout<<"Choose the start node : ";
				cin>>b;
				int startNode = atoi(b);
				while( 1 > startNode || n < startNode )
				{
					cout<<endl<<endl<<"Wrong input! Choose a number between 1 and nodes number : ";
					cin>>b;
					startNode = atoi(b);
				}
				cout<<endl<<endl;
				bool queue = true;
				graph[startNode - 1].setKey(0);
				while (queue)
				{
					node* u = NULL;
					int min = INT_MAX;
					queue = false;
					for(int i = 0 ; i < n ; i++ )
						if(!graph[i].getVisited())
						{
							queue = true;
							if(graph[i].getKey() < min)
							{
								min = graph[i].getKey();
								u = graph + i;
							}
						}
					if(u)
					{
						u->setVisited(true);
						adj* Adj = u->getAdjs();
						while(Adj)
						{
							if ( !(Adj->adjacent->getVisited()) && Adj->weight + u->getKey() < Adj->adjacent->getKey())
							{
								Adj->adjacent->setParent(u);
								Adj->adjacent->setKey(Adj->weight + u->getKey());
							}
							Adj = Adj->next;
						}
					}
				}
				for( int i = 0 ; i < n ; i++ )
				{
					for( int j = 0 ; j < n ; j++ )
					{
						if( graph[j].getParent() == graph + i )
						{
							adj* temp = graph[j].getAdjs();
							while(temp->adjacent != graph + i)
								temp = temp->next;
							cout<<temp->weight<<" ";
						}
						else
							cout<<"* ";
					}
					cout<<endl;
				}
				for( int i = 0 ; i < n ; i++ )
					delete (graph + i);
				a = -1;
				cout<<endl;
			}
			else if (a == '4')
			{
				char b[4];
				cout<<"Choose the start node : ";
				cin>>b;
				int startNode = atoi(b);
				while( 1 > startNode || n < startNode )
				{
					cout<<endl<<endl<<"Wrong input! Choose a number between 1 and nodes number : ";
					cin>>b;
					startNode = atoi(b);
				}
				cout<<endl;
				bool ans = false;
				graph[startNode - 1].setKey(0);
				for (int k = 1 ; k < n ; k++)
				{
					ans = true;
					for(int i = startNode - 1 ; i < startNode - 1 + n ; i++ )
					{
						adj* u = graph[i % n].getAdjs();
						while(u)
						{
							int key = graph[i % n].getKey();
							if( key != INT_MAX && key + u->weight < u->adjacent->getKey())
							{
								u->adjacent->setKey(key + u->weight);
								u->adjacent->setParent(graph + (i % n));
								ans = false;
							}
							u = u->next;
						}
					}
					if(ans)
						break;
				}
				if(ans)
					for( int i = 0 ; i < n ; i++ )
					{
						for( int j = 0 ; j < n ; j++ )
						{
							if( graph[j].getParent() == graph + i )
							{
								adj* temp = graph[j].getAdjs();
								while(temp->adjacent != graph + i)
									temp = temp->next;
								cout<<temp->weight<<" ";
							}
							else
								cout<<"* ";
						}
						cout<<endl;
					}
				else
					cout<<"The input matrix has a negative circuit"<<endl<<endl;
				
				for( int i = 0 ; i < n ; i++ )
					delete (graph + i);
				a = -1;
				cout<<endl;
			}
			
		}
		else if(a == '5')
		{
			int** graph;
			int n;
			input>>n;
			graph = new int*[n];
			
			way** links;
			links = new way*[n];
			
			char Weight[10];
			int weight;
			for( int i = 0 ; i < n ; i++ )
			{
				links[i] = new way[n];
				graph[i] = new int[n];
				for( int j = 0 ; j < n ; j++)
				{
					input>>Weight;
					weight = atoi(Weight);
					links[i][j].Node = i;
					if(Weight[0] == '*')
					{
						graph[i][j] = INT_MAX;
						links[i][j].next = NULL;
					}
					else
					{
						graph[i][j] = weight;
						way* New;
						New = new way;
						New->next = NULL;
						New->weight = weight;
						New->Node = j;
						links[i][j].next = New;
					}
				}
			}
			for(int k = 0 ; k < n ; k++)
				for(int i = 0 ; i < n ; i++)
					for(int j = 0 ; j < n ; j++)
						if(graph[i][k] + graph[k][j] < graph[i][j])
						{
							graph[i][j] = graph[i][k] + graph[k][j];
							way* prev = links[i][j].next;
							way* next;
							while(prev)
							{
								next = prev->next;
								delete prev;
								prev = next;
							}
							next = new way;
							next->Node = links[i][k].next->Node;
							next->weight = links[i][k].next->weight;
							next->next = links[i][k].next->next;
							links[i][j].next = next;
							prev = next;
							next = next->next;
							while(next)
							{
								way* temp;
								temp = next;
								next = new way;
								next->next = temp->next;
								next->Node = temp->Node;
								next->weight = temp->weight;
								prev->next = next;
								prev = next;
								next = next->next;
							}
							next = links[k][j].next;
							while(next)
							{
								way* temp;
								temp = next;
								next = new way;
								next->next = temp->next;
								next->Node = temp->Node;
								next->weight = temp->weight;
								prev->next = next;
								prev = next;
								next = next->next;
							}
						}
			for(int i = 0 ; i < n ; i++)
			{
				for(int j = 0 ; j < n ; j++)
				{
					if(graph[i][j] == INT_MAX)
						cout<<"* ";
					else
						cout<<graph[i][j];
					cout<<" ";
				}
				cout<<endl;
			}
			while(a != -1)
			{
				cout<<endl<<"1.Show shortest path between node i and node j\n2.Back\n\nChoose an option : ";
				cin>>a;
				cout<<endl;
				if(a == '2')
					a = -1;
				else if(a == '1')
				{
					int i = 0;
					int j = 0;
					while( i < 1 || i > n )
					{
						cout<<endl<<"Enter i (enter a number between 1 and nodes number): ";
						cin>>i;
					}
					while( j < 1 || j > n || j == i )
					{
						cout<<endl<<"Enter j (enter a number between 1 and nodes number different than i): ";
						cin>>j;
					}
					cout<<endl<<"Shortest path between node "<<i<<" and node "<<j<<" :\n";
					i--;
					j--;
					way* flag = links[i] + j;
					while(flag->next)
					{
						cout<<"( "<<(flag->Node + 1)<<" , ";
						flag = flag->next;
						cout<<(flag->Node + 1)<<" )     weight = "<<flag->weight<<endl;
					}
				}
				else
					cout<<"Wrong entry !"<<endl;
			}
			cout<<endl;
		}
		input.close();
	}
	
	return 0;
}
