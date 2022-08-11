// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>		//For numeric limits of dist vector

using namespace std;

void add_edge(vector<int> edges[], int s, int d)
{
    edges[s].push_back(d);
    edges[d].push_back(s);
}

void compute(int v, int src, int dst, vector<int> edges[], vector<int> path[], int dist[])
{
	vector <int> q;
	vector <int> visited;
	q.push_back(src);
	path[src].push_back(src);
	dist[q[0]]=0;
	while (q.size()!=0)
	{
		for (auto it=edges[q[0]].begin(); it!=edges[q[0]].end();it++)
		{
			bool flag=false;
			for (int i=0; i<visited.size(); i++)
			{
				if (*it==visited[i])
					flag=true;
			}
			if (flag==false)
			{
				for (int i=0; i<q.size();i++)
				{
					if(*it==q[i])
						flag=true;
				}
				if (flag==false)
				{
					q.push_back(*it);
					if (dist[*it]>dist[q[0]]+1)
					{
						dist[*it]=dist[q[0]]+1;
						path[*it].push_back(*it);
						for (auto jt=path[q[0]].begin(); jt!=path[q[0]].end();jt++)
						{
							path[*it].push_back(*jt);
						}
					}
					
				}
			}
		}
		visited.push_back(q[0]);
		q.erase(q.begin());
	}
	
}

void computefun(vector<unsigned> e, int n, int src, int dst)
{
	int v=n+1;
	int dist[v];
	
	for (int i=0;i<v;i++)
	{
		dist[i]=numeric_limits<int>::max();
	}
	
	vector<int> edges[v];	//To store edges
	vector<int> path[v];	//To store paths
	
	for(int i=0; i<e.size(); i+=2)
	{
		add_edge(edges, e[i], e[i+1]);
	}
	
	compute(v, src, dst, edges, path, dist);
	
	if (path[dst].empty())
	{
		cerr<<"Error: No path exists";
		cout<<flush;
	}
	else
	{
	    cout<<path[dst][path[dst].size()-1];
		cout<<flush;
	    for(int it=path[dst].size()-2; it>=0; it--)
		{
			cout<<"-"<<path[dst][it];
			cout<<flush;
		}
	}
	cout<<endl;
	
}

bool checkinput(string line, vector<unsigned> &e, int &n, int &s, int &d)
{
	bool innerflag = false;
	if (line[0] == 'V' && line[1] == ' ')
	{
		string v_number="";
		for (int i=2; i<line.length(); i++)
		{
			if (int(line[i]) > 57 || int(line[i]) < 48)
			{
				//cout<<"Error: 1"<<endl;
				return false;
			}
			else
				v_number += line[i];
				innerflag = true;
		}
		n=stoi(v_number);	//Converting to int
		/*if (n==0){
			cout<<"Error: 2"<<endl;
			return false;}*/
	}
	else if (line[0] == 's' && line[1] == ' ')
	{
		string src_dst="";
		string delimiter = " ";
		for (int i=2; i<line.length();i++)
		{
			if ((int(line[i]) > 57 || int(line[i]) < 48) && (line[i] != 32))
			{
				//cout<<"Error: 3"<<endl;
				return false;
			}
			else
				src_dst += line[i];
				innerflag = true;
		}
		string src = src_dst.substr(0, src_dst.find(delimiter));
		string dst = src_dst.substr(src_dst.find(delimiter)+1,src_dst.length());
		
		s=stoi(src);
		if (s==0 || s>n){
			//cout<<"Error: 4"<<endl;
			return false;}
		
		d=stoi(dst);
		if (d==0 || d>n){
			//cout<<"Error: 5"<<endl;
			return false;}
		
		computefun(e, n, s, d);
	}
	else if (line[0] == 'E' && line[1] == ' ')
	{
		if (e.size()>0)
			e.clear();
		for (int i=2; i<line.length(); i++)
		{
			if (int(line[i]) <= 57 && int(line[i]) >= 48)	//ASCII value of 1 and 9 are 49 and 57 respectively
			{	
				string edge_string_value="";
				while (int(line[i]) <= 57 && int(line[i]) >= 48)
				{
					edge_string_value +=line[i];
					i++;
				}
				int edge_value = stoi(edge_string_value);
				if (edge_value > n || edge_value == 0)
				{
					e.clear();
					//cout<<"Error: 6"<<endl;
					return false;
				}
				else
				{
					e.push_back(edge_value);
					innerflag = true;
				}
			}
		}
	}
	if (innerflag == true)
		return true;
	else{
		//cout<<"Error: 7"<<endl;
		return false;}
}
int main(int argc, char** argv) 
	{
	int n=0, s=0, d=0; 	//No of vertices, source, destination respectively
	vector<unsigned> e; // Storing edges not in adjacency list

    // read from stdin until EOF
    while (!std::cin.eof()) 
	{
        // print a promt
        //std::cout << "Enter numbers separated by space: ";					//R

        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);
		if (line == ""){break;}
        // create an input stream based on the line
        // we will use the input stream to parse the line
        std::istringstream input(line);
		
		if (line.rfind('V', 0) == 0 || line.rfind('E', 0) == 0)
				cout<<line<<endl;
		
		bool mainflag = false;
		mainflag = checkinput(line, e, n, s, d);
		if (mainflag == false)
		{
			cerr<<"Error: in mainflag"<<endl;
			cout<<flush;
		}
    }
	return 0;
}