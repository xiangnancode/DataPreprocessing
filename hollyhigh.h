#ifndef H_HOLLYHIGH
#define H_HOLLYHIGH

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <dirent.h>

using namespace std;
#define DATE 0
#define UNDLY 1
#define OBC 2
#define OSC 3
#define CBC 4
#define CSC 5
#define OBP 6
#define OSP 7
#define CBP 8
#define CSP 9
#define PERMNO 10
#define CRSPDATE 11
#define SHRCD 12
#define TICKER 13
#define COMNAM 14
#define SHRCLS 15
#define TSYMBOL 16
#define CUSIP 17
#define PRC 18
#define VOL 19
#define RET 20

struct Undly
{
	string undly;
	vector<string> data;
};

struct Day
{
	int day;
	vector<Undly> U;
};

struct Month
{
	vector<Day> D;
};

struct Year
{
	vector<Month> M;
};


class Data
{
	vector<string> ISEfilelist;
	vector<string> CRSPfilelist;
	vector<Year> Y;
	vector<int> date;

	void output()
	{
		ofstream file;
		int i = 0;
	    file.open ("testoutput.csv");
	    
	    file << "Date,"<<"Ticker,"<<"OBC,"<<"OSC,"<<"CBC,"<<"CSC,"<<"OBP,"<<"OSP,"<<"CBP,"<<"CSP," << endl;
	   
	    for (int yy = 0; yy < Y.size(); ++yy)
	    {
	    	for (int mm = 0; mm < Y[yy].M.size(); ++mm)
	    	{
	    		for (int dd = 0; dd < Y[yy].M[mm].D.size(); ++dd)
	    		{
	    			for (int uu = 0; uu < Y[yy].M[mm].D[dd].U.size(); ++uu)
	    			{
	    				for (i = 0; i < Y[yy].M[mm].D[dd].U[uu].data.size()-1; ++i)
	    				{
	    					file << Y[yy].M[mm].D[dd].U[uu].data[i] << ",";
	    				}
	    				file << Y[yy].M[mm].D[dd].U[uu].data[i] << "\n";	    				
	    			}
	    		}
	    		
	    	}
	    }

	    file.close();
	}

	void printline(vector<string> example)
	{
	    for (int i = 0; i < example.size(); ++i)
	    {
	    	cout << example[i]+",";
	    }
	    cout<< endl;
	}

	void getfilelist()
	{
		DIR *dir;
		struct dirent *ent;
		string name;
		if ((dir = opendir ("./ISEdata/")) != NULL) {
		  while ((ent = readdir (dir)) != NULL) {
		    name = ent->d_name;
		    //cout << "./ISEdata/"+name << endl;
		    ISEfilelist.push_back("./ISEdata/"+name);
		  }
		  closedir (dir);
		} else {
		  cout << "could not open ISE directory" << endl;
		  
		}
		if ((dir = opendir ("./CRSPdata/")) != NULL) {
		  /* print all the files and directories within directory */
		  while ((ent = readdir (dir)) != NULL) {
		    name = ent->d_name;
		    //cout << "./CRSPdata/"+name << endl;
		    CRSPfilelist.push_back("./CRSPdata/"+name);
		  }
		  closedir (dir);
		} else {
		  cout << "could not open CRSP directory" << endl;
		  
		}
	}

	void loadISE()
	{
		//for (int i = 3; i < ISEfilelist.size(); ++i)
		{
			readISEfile(ISEfilelist[3]);
			readISEfile(ISEfilelist[4]);
		}
	}

	void readISEfile(string filename)
	{
		//cout << filename << endl;
		ifstream file (filename);
		cout << "reading " << filename << "...\n";
		vector<string> newline;
	    string entry;
	   
	    getline(file,entry,'\n');
	    //cout << "entry: " << entry << endl;
	    int k = 0;

	    while(file.good())
	    //for (int n = 0; n < 14986; ++n)
	    {
	    	for (int i = 0; i < 9; i++){
		        getline(file,entry,',');
		        newline.push_back(entry);
		        }
	        getline(file,entry,'\n');
	        newline.push_back(entry);// read one line

	        //printline(newline);
	        if (!newline[0].empty())
	        {
	        	add2Y(newline);
	        }
	        
	        //cout << newline[0].empty() << endl;
	        //cout << k++ << endl;
	        newline.clear();
	  	}
	    file.close();
	}

	void add2Y(vector<string> newline)
	{
		int today,yy,mm,dd;
		string uu = newline[UNDLY];
		today = stoi(newline[DATE],nullptr,10);
		yy = today / 10000;
		yy = yy % 100;
		dd = today % 100;
		mm = today / 100;
		mm = mm % 100;

		//cout << "yy/mm/dd\n";
		cout << yy << "/" << mm << "/" << dd <<endl;

		if (Y.size() < (yy-4) )
		{
			Y.push_back(Year());
		}
		
		while (Y[yy-5].M.size() < mm)
		{
			Y[yy-5].M.push_back(Month());
		}


		//cout << "before i\n";
		int i = day2i(yy,mm,dd);
		//cout << "i = " << i << endl;
		//cout << "before j\n";
		int j = undly2i(yy,mm,i,uu);
		//cout << "j = " << j << endl;

		for (int k = 0; k < newline.size(); ++k)
		{
			Y[yy-5].M[mm-1].D[i].U[j].data.push_back(newline[k]);
		}

		
	}

	int day2i(int yy,int mm,int dd)
	{
		int i = 0;
		if (Y[yy-5].M[mm-1].D.size() == 0)
		{
			Y[yy-5].M[mm-1].D.push_back(Day());
			Y[yy-5].M[mm-1].D[0].day = dd;
		}
		else
		{
			while (Y[yy-5].M[mm-1].D[i].day != dd)// find day
			{
				i++;
				if (i == Y[yy-5].M[mm-1].D.size())// cant find, create a new day
				{
					Y[yy-5].M[mm-1].D.push_back(Day());
					Y[yy-5].M[mm-1].D[i].day = dd;
					break;
				}
				
			}
		}
		return i;
	}
	int undly2i(int yy,int mm,int i,string uu)
	{
		int j = 0;
		if (Y[yy-5].M[mm-1].D[i].U.size() == 0)
		{
			Y[yy-5].M[mm-1].D[i].U.push_back(Undly());
			Y[yy-5].M[mm-1].D[i].U[0].undly = uu;
		}
		else
		{
			while (Y[yy-5].M[mm-1].D[i].U[j].undly != uu)// find day
			{
				j++;
				if (j == Y[yy-5].M[mm-1].D[i].U.size())// cant find, create a new day
				{
					Y[yy-5].M[mm-1].D[i].U.push_back(Undly());
					Y[yy-5].M[mm-1].D[i].U[j].undly = uu;

					break;
				}
				
			}
		}
		return j;
	}

	void test()
	{
		printf("hello world!\n");
		vector<string> s;
		string a = "apple";

		s.push_back(a);

		cout << s.size() << endl;
		
	}

public:

	void calltest()
	{

		test();
		//printline(ISEfilelist);
	}

	void process()
	{
		getfilelist();
		loadISE();
		output();
	}
};
#endif