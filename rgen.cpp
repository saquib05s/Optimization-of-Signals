/*#include <iostream>
#include <urandom>
using namespace std;*/

// an example of reading random numbers from /dev/urandom
// https://stackoverflow.com/questions/35726331/c-extracting-random-numbers-from-dev-urandom
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
using namespace std;

int main(int argc, char *argv[])
{
	bool outer_flag = true;
	while (true){
	vector <vector <vector <int>>> store; 
	unsigned int s=10, n=5, l=5, c=20;	// s = No. of streets, n = No. of line segments;
	for (int i=1; i<argc; i++)	// l = wait seconds, c = Range of x and y in "(x,y)" coordinate.
	{
		string arg1(argv[i]);
		
		if ("-s" == arg1)
		{	
			try{
				if (stoi(argv[i+1])>=2){
					s=stoi(argv[i+1]);}
				else
				{
					cerr<<"Error: Invalid s"<<endl;				
				}
			}
			catch (exception e)
			{
				cerr<<"Error: in s"<<endl;
			}
		}
		if ("-n" == arg1)
		{
			try{
				if (stoi(argv[i+1])>=1){
					n=stoi(argv[i+1]);}
				else
				{
					cerr<<"Error: Invalid n"<<endl;
				}
			}
			catch(exception e)
			{
				cerr<<"Error: in n"<<endl;
			}
		}
			
		if ("-l" == arg1)
		{
			try{
				if (stoi(argv[i+1])>=5){
					l=stoi(argv[i+1]);}
				else
				{
					cerr<<"Error: Invalid l"<<endl;
				}
			}
			catch(exception e)
			{
				cerr<<"Error: in l"<<endl;
			}
		}
			
		if ("-c" == arg1)
		{
			try{
				if (stoi(argv[i+1])>=1){
					c=stoi(argv[i+1]);}
				else
				{
					cerr<<"Error: Invalid c"<<endl;
				}
			}
			catch (exception e)
			{
				cerr<<"Error: in c"<<endl;
			}
		}
	}	
	
	// open /dev/urandom to read
	ifstream urandom("/dev/urandom");

	// check that it did not fail
	if (urandom.fail()) {
	cerr << "Error: cannot open /dev/urandom\n";
	return 1;
	}
	
	unsigned int rs, rn, rl, rc; // rs = Random s;
	urandom.read((char *)&rs, sizeof(int));
	int streets = int ((rs%(s-1))+2);	// Range should be [2,s]. Therefore, [0,s-1-1] + 2 = [2,s] as range of n%k is [0,k-1]
	
	
	for (int i=0; i<streets; i++)
	{
		int failed_coor_attempt = 0, valid_attempt = 10, flag_s = 0;
		urandom.read((char *)&rn, sizeof(int));
		int ls = int ((rn%n)+1);	//ls=Line segments-> Range should be [1,n]. Therefore, [0,n-1] + 1 = [1,n]
		
		vector <vector <int>> temp2d;
		for (int j=0; j<=ls; j++)
		{
			int flag_c=0;
			while (failed_coor_attempt <=valid_attempt)
			{
				vector <int> temp;

				urandom.read((char *)&rc, sizeof(int));
				int a = int((rc%(2*c)+1)-c); // To set in range - [-10,10] = [0,20] - 10
				
				urandom.read((char *)&rc, sizeof(int));
				int b = int((rc%(2*c)+1)-c); // To set in range - [-10,10] = [0,20] - 10
				
				for(int i=0; i<temp2d.size(); i++)	//Check whether same coor are not there
				{
					if (a == temp2d[i][0] && b == temp2d[i][1])
					{
						failed_coor_attempt++;
						flag_c++;
						break;
					}
				}
				if (temp2d.size()>1){
					for (int i=0; i<temp2d.size()-1; i++) // Check whether line segments of a same street does not intersect
					{
						float x1 = temp2d[temp2d.size()-1][0]; //3
						float y1 = temp2d[temp2d.size()-1][1]; //3
						float x2 = a; //2
						float y2 = b;	//2
						float mc1 = (y2-y1)/(x2-x1);
						
						float x3 = temp2d[i][0];
						float y3 = temp2d[i][1];
						float x4 = temp2d[i+1][0];
						float y4 = temp2d[i+1][1];
						float mc2 = (y4-y3)/(x4-x3);
						
						if (mc1==mc2)
						{
							bool c = (min(x3, x4) <= x2 && x2 <= max(x3, x4)) && (min(y3, y4) <= y2 && y2 <= max(y3, y4));
							bool old_c = (min(x3, x4) <= x1 && x1 <= max(x3, x4)) && (min(y3, y4) <= y1 && y1 <= max(y3, y4));
							if (c || old_c)
							{
								failed_coor_attempt++;
								flag_c++;
								break;
							}
						}
						
					}
				}
				if (flag_c!=0){
					continue;}
					
				temp.push_back(a);
				temp.push_back(b);
				temp2d.push_back(temp);
				break;
			}
			if (failed_coor_attempt > valid_attempt)
			{
				cerr<<"Error: Failed inputing valid coordinates after "<<valid_attempt<<" attempts"<<endl;
				outer_flag = false;
			}
			
		}
		while(failed_coor_attempt <=valid_attempt)
		{
			if (store.size()>0)
			{
				for (int i=0; i<store.size(); i++)
				{
					int tempj_flag = 0;
					for (int j=0; j<store[i].size()-1; j++)
					{
						float sx1 = store[i][j][0];
						float sy1 = store[i][j][1];
						float sx2 = store[i][j+1][0];
						float sy2 = store[i][j+1][1];
						
						float smc1 = (sy2-sy1)/(sx2-sx1);
						
						int temp2d_flag=0;
						for (int k=0; k < temp2d.size()-1; k++)
						{
							float sx3 = temp2d[k][0];
							float sy3 = temp2d[k][1];
							float sx4 = temp2d[k+1][0];
							float sy4 = temp2d[k+1][1];
							
							float smc2 = (sy4-sy3)/(sx4-sx3);
							
							if (smc1==smc2)
							{
								bool sc = (min(sx1, sx2) <= sx3 && sx3 <= max(sx1, sx2)) && (min(sy1, sy2) <= sy3 && sy3 <= max(sy1, sy2));
								bool sold_c = (min(sx1, sx2) <= sx4 && sx4 <= max(sx1, sx2)) && (min(sy1, sy2) <= sy4 && sy4 <= max(sy1, sy2));
								if (sc || sold_c)
								{
									temp2d_flag++;
									failed_coor_attempt++;
									flag_s++;
									break;
								}
							}
						}
						if (temp2d_flag!=0)
						{
							tempj_flag++;
							break;
						}
					}
					if (tempj_flag!=0)
					{
						break;
					}
				}
			}
			if (flag_s!=0){
				continue;}
			store.push_back(temp2d);
			break;
		}
		if (failed_coor_attempt > valid_attempt)
			{
				cerr<<"Error: Failed inputing valid streets after "<<valid_attempt<<" attempts"<<endl;
				outer_flag = false;
			}
		
	}
	
	// Compute street strings
	string street = "";
	int namecount = 0;
	//char s_name = 'a';
	for (int i=0; i<store.size(); i++)
	{
		namecount++;
		street += "add \"";
		for (int alpha=0; alpha <= namecount/26; alpha++) 
		{
			street.push_back(char(namecount%26 + 97));
		}
		
		street += "\"";
		for (int j=0; j<store[i].size();j++)
		{
			street += " (";
			street += to_string(store[i][j][0]);
			street += ",";
			street += to_string(store[i][j][1]);
			street += ")";
		}
		street += "\n";
		//s_name++;
	}
	street += "gg\n";
	
	// Remove streets
	
	int rnamecount = 0;
	//char rs_name = 'a';
	for (int i=0; i<store.size(); i++)
	{
		rnamecount++;
		street += "rm \"";
		for (int alpha=0; alpha <= rnamecount/26; alpha++) 
		{
			street.push_back(char(rnamecount%26 + 97));
		}
		
		street += "\"";
		
		street += "\n";
		//s_name++;
	}
	
	if (outer_flag){
	cout<<street;}
	
	/*cout<<"add \"a\" (1,1) (3,3) (2,2) (4,4)\n";
	cout<<"add \"b\" (1,3) (3,1)\n";
	cout<<"gg\n";*/
	
	urandom.read((char *)&rl, sizeof(int));
	int sec = int ((rl%(l-4))+5);	// [5,l] = [0,l-4-1] + 5
	
	sleep(sec);
	
	cout<<flush;

	urandom.close();} // while ends
	return 0;
}
