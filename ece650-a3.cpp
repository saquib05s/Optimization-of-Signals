#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char *argv[])
{
	vector <pid_t> kids = {};
	
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
					return 0;
				}
			}
			catch (exception e)
			{
				cerr<<"Error: in s"<<endl;
				return 0;
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
					return 0;
				}
			}
			catch(exception e)
			{
				cerr<<"Error: in n"<<endl;
				return 0;
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
					return 0;
				}
			}
			catch(exception e)
			{
				cerr<<"Error: in l"<<endl;
				return 0;
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
					return 0;
				}
			}
			catch (exception e)
			{
				cerr<<"Error: in c"<<endl;
				return 0;
			}
		}
	}
	//while (true){
	int fd1[2], fd2[2];
	if (pipe(fd1)<0)
            return 1;
	if (pipe(fd2)<0)
			return 11;
	
	int pid1 = fork();
	kids.push_back(pid1);
	if (pid1<0)
		return 12;
	
	if (pid1 == 0)
	{
		dup2(fd1[1], STDOUT_FILENO);
		close(fd1[0]);
		close(fd1[1]);
		execv("./rgen", argv);
		//execlp("./rgen", "", NULL);
	}
	
	int pid2 = fork();
	kids.push_back(pid2);
	if (pid2<0)
		return 13;
	if (pid2 == 0)
	{
		dup2(fd1[0], STDIN_FILENO);
		close(fd1[0]);
		close(fd1[1]);
		dup2(fd2[1], STDOUT_FILENO);
		close(fd2[0]);
		close(fd2[1]);
		//execv("./ece650-a1.py", argv);
		execlp("python3", "python3", "ece650-a1.py", NULL);
	}
	close(fd1[0]);
    close(fd1[1]);

    //waitpid(pid1, NULL, 0);
    //waitpid(pid2, NULL, 0);
	
	
	// Pipe for Python-a2

    /*int pid3 = fork();
    if(pid3<0)
            return 2;
    if (pid3==0)
    {
        dup2(fd2[1], STDOUT_FILENO);
        close(fd2[0]);
        close(fd2[1]);
        execlp("python3", "python3", "ece650-a1.py", NULL);
	}*/
	int pid3 = fork();
	kids.push_back(pid3);
    if (pid3<0)
            return 5;
    if (pid3 == 0)
    {
        dup2(fd2[1], STDOUT_FILENO);
        close(fd2[1]);
        close(fd2[0]);
        string line;
		
		/*struct timeval tmo;
        fd_set readfds;


        // wait only 5 seconds for user input 
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        tmo.tv_sec = 5;
        tmo.tv_usec = 0;

        switch (select(1, &readfds, NULL, NULL, &tmo)) {
            case -1:
                break;
            case 0:
                cout<<endl;
                exit(0);
        }*/
		while (!cin.eof())
		{
			getline(cin,line);
			if (line == "")
			{
				int status;
				kill(getppid(),SIGTERM);
				waitpid(getppid(), &status, 0);
				break;
			}
			cout<<line<<"\n";
		}
        //exit(0);
    }

    int pid4 = fork();
	kids.push_back(pid4);
    if (pid4<0)
            return 6;
    if (pid4 == 0)
    {
        dup2(fd2[0], STDIN_FILENO);
        close(fd2[0]);
        close(fd2[1]);
		execv("./ece650-a2", argv);
        //execlp("./ece650-a2","", NULL);
	}
	close(fd2[0]);
    close(fd2[1]);

	waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
    waitpid(pid4, NULL, 0);
	
	for (pid_t k : kids)
	{
		int status;
		kill (k, SIGTERM);
		waitpid(k, &status, 0);
	}
//}
    return 0;
}