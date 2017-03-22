#ifndef H_HOLLYHIGH
#define H_HOLLYHIGH

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <dirent.h>
#include <algorithm>

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

	void testoutput()
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

	void loaddata()
	{
		//for (int i = 3; i < ISEfilelist.size(); ++i)
		{
			readfile(ISEfilelist[3], 1);
			readfile(ISEfilelist[4], 1);
			readfile(ISEfilelist[5], 1);
			readfile(CRSPfilelist[2], 2);
		}
	}

	void readfile(string filename, int flag)
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
	    	for (int i = 0; i < (9+flag-1); i++){
		        getline(file,entry,',');
		        newline.push_back(entry);
		        }
	        getline(file,entry,'\n');
	        newline.push_back(entry);// read one line

	        //printline(newline);
	        if (!newline[0].empty() && !newline[9].empty())
			{
	        	add2Y(newline,flag);
	        }
	        
	        //cout << newline[0].empty() << endl;
	        //cout << k++ << endl;
	        newline.clear();
	  	}
	    file.close();
	}

	void add2Y(vector<string> newline, int flag)
	{
		int today,yy,mm,dd;
		string uu;
		if (flag == 1)
		{
			uu = newline[UNDLY];
			uu.erase(remove_if(uu.begin(), uu.end(), ::isdigit),uu.end());
			today = stoi(newline[DATE],nullptr,10);
		}
		else if (flag == 2)
		{
			uu = newline[6];
			today = stoi(newline[1],nullptr,10);
		}

		yy = today / 10000;
		yy = yy % 100;
		dd = today % 100;
		mm = today / 100;
		mm = mm % 100;

		//cout << "yy/mm/dd\n";
		//cout << yy << "/" << mm << "/" << dd << "    " << flag <<endl;

		//if (flag == 1)
		{
			while (Y.size() < (yy-4) )
			{
				Y.push_back(Year());
			}
			
			while (Y[yy-5].M.size() < mm)
			{
				Y[yy-5].M.push_back(Month());
			}
		}

		


		//cout << "before i\n";
		int i = day2i(yy,mm,dd,flag);
		//cout << "i = " << i << endl;
		//cout << "before j\n";
		int j = 0;
		if (i != -1)
		{
			j = undly2i(yy,mm,i,uu,flag);
		}
		
		//cout << "j = " << j << endl;

		if (i != -1 && j != -1)
		{
			for (int k = 0; k < newline.size(); ++k)
			{
				if (k == UNDLY && flag == 1)
				{
					newline[k].erase(remove_if(newline[k].begin(), newline[k].end(), ::isdigit),newline[k].end());
				}
				Y[yy-5].M[mm-1].D[i].U[j].data.push_back(newline[k]);
			}
		}

	}

	int day2i(int yy,int mm,int dd, int flag)
	{
		int i = 0;
		if (Y[yy-5].M[mm-1].D.size() == 0)
		{
			if (flag == 2)
			{
				return -1;
			}
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
					if (flag == 2)
					{
						return -1;
					}
					Y[yy-5].M[mm-1].D.push_back(Day());
					Y[yy-5].M[mm-1].D[i].day = dd;
					break;
				}
				
			}
		}
		return i;
	}
	int undly2i(int yy,int mm,int i, string uu, int flag)
	{
		int j = 0;
		if (Y[yy-5].M[mm-1].D[i].U.size() == 0)
		{
			Y[yy-5].M[mm-1].D[i].U.push_back(Undly());
			Y[yy-5].M[mm-1].D[i].U[0].undly = uu;
		}
		else
		{
			while (Y[yy-5].M[mm-1].D[i].U[j].undly != uu)// find undly
			{
				j++;
				if (j == Y[yy-5].M[mm-1].D[i].U.size())// cant find, create a new undly
				{
					if (flag == 2)
					{
						return -1;
					}
					Y[yy-5].M[mm-1].D[i].U.push_back(Undly());
					Y[yy-5].M[mm-1].D[i].U[j].undly = uu;

					break;
				}
				
			}
		}
		return j;
	}

	void output()
	{
		ofstream file;
		char filename[20];
	    //file.open ("testoutput.csv");
	    
	    
	   
	    for (int yy = 0; yy < Y.size(); ++yy)
	    {
	    	for (int mm = 0; mm < Y[yy].M.size(); ++mm)
	    	{
	    		sprintf(filename,"./output/IC_20%02d_%02d.csv",yy+5,mm+1);
	    		file.open (filename);
	    		file << "TRADE_DT,"<<"UNDLY,"<<"TICKER,"<<"TSYMBOL,"<<"CUSIP,"<<"PERMNO,";
			    file <<"COMNAM,"<<"SHRCD,"<<"SHRCLS,"<<"PRICEL1,";
			    file <<"OBC,"<<"OSC,"<<"CBC,"<<"CSC,"<<"OBP,"<<"OSP,"<<"CBP,"<<"CSP,";
			    file <<"OBCL1,"<<"OSCL1,"<<"CBCL1,"<<"CSCL1,"<<"OBPL1,"<<"OSPL1,"<<"CBPL1,"<<"CSPL1,";
			    file <<"OBCL2,"<<"OSCL2,"<<"CBCL2,"<<"CSCL2,"<<"OBPL2,"<<"OSPL2,"<<"CBPL2,"<<"CSPL2,";
			    file <<"OBCL3,"<<"OSCL3,"<<"CBCL3,"<<"CSCL3,"<<"OBPL3,"<<"OSPL3,"<<"CBPL3,"<<"CSPL3,";
			    file <<"OBCL4,"<<"OSCL4,"<<"CBCL4,"<<"CSCL4,"<<"OBPL4,"<<"OSPL4,"<<"CBPL4,"<<"CSPL4,";
			    file <<"VOL,"<<"VOLL1,"<<"VOLL2,"<<"VOLL3,"<<"VOLL4,"<<"RETL1,"<<"RET,";
			    file <<"RETP1,"<<"RETP2,"<<"RETP3,"<<"REPT4,"<<"RETP5"<< endl;
	    		for (int dd = 0; dd < Y[yy].M[mm].D.size(); ++dd)
	    		{
	    			for (int uu = 0; uu < Y[yy].M[mm].D[dd].U.size(); ++uu)
	    			{
	    				cout << yy+5 << "/" << mm+1 << "/" << Y[yy].M[mm].D[dd].day <<endl;
	    				file << Y[yy].M[mm].D[dd].U[uu].data[DATE] << ",";
	    				file << Y[yy].M[mm].D[dd].U[uu].data[UNDLY] << ",";
	    				if (Y[yy].M[mm].D[dd].U[uu].data.size() > 10)
	    				{
		    				file << Y[yy].M[mm].D[dd].U[uu].data[TICKER] << ",";
		    				file << Y[yy].M[mm].D[dd].U[uu].data[TSYMBOL] << ",";
		    				file << Y[yy].M[mm].D[dd].U[uu].data[CUSIP] << ",";
		    				file << Y[yy].M[mm].D[dd].U[uu].data[PERMNO] << ",";
		    				file << Y[yy].M[mm].D[dd].U[uu].data[COMNAM] << ",";
		    				file << Y[yy].M[mm].D[dd].U[uu].data[SHRCD] << ",";
		    				file << Y[yy].M[mm].D[dd].U[uu].data[SHRCLS] << ",";
		    				file << finddata(yy,mm,dd,uu,Y[yy].M[mm].D[dd].U[uu].undly,PRC,-1) << ",";
	    				}
	    				else
	    				{
	    					for (int i = 0; i < 8; ++i)
	    					{
	    						file << "" << ",";
	    					}
	    				}
	    				

		    			for (int j = 0; j <= 4; ++j)
	    				{
		    				for (int i = 2; i <= 9; ++i)
		    				{
		    					file << finddata(yy,mm,dd,uu,Y[yy].M[mm].D[dd].U[uu].undly,i,-j) << ",";
		    					//file << "" << ",";
		    				}
		    			}
						if (Y[yy].M[mm].D[dd].U[uu].data.size() > 10)
	    				{
			    			for (int j = 0; j <= 4; ++j)
		    				{
			    				file << finddata(yy,mm,dd,uu,Y[yy].M[mm].D[dd].U[uu].undly,VOL,-j) << ",";
			    				//file << "" << ",";
			    			}

			    			for (int j = -1; j <= 5; ++j)
		    				{
			    				file << finddata(yy,mm,dd,uu,Y[yy].M[mm].D[dd].U[uu].undly,RET,j) << ",";
			    			}
			    		}

	    				file << endl;	    				
	    			}
	    		}
	    		file.close();
	    	}
	    }

	    //file.close();
	}

	string finddata(int yy, int mm, int dd, int uu, string undly, int i, int offset)
	{

		dd = dd + offset;
		//cout << dd << endl;
		if (dd < 0)// pre month
		{
			mm = mm - 1;
			if (mm < 0)
			{
				yy = yy - 1;
				mm = 11;
			}
			if (Y[yy].M.size() == 0 || Y[yy].M[mm].D.size() == 0)
			{
				return "";
			}
			dd = Y[yy].M[mm].D.size() + dd;
		}
		else if (dd >= Y[yy].M[mm].D.size())// next month
		{
			//cout << "next" << endl;
			dd = dd - Y[yy].M[mm].D.size();
			mm = mm + 1;
			if (mm >= 11)
			{
				yy = yy + 1;
				mm = 0;
			}
			if (Y[yy].M.size() == 0 || Y[yy].M[mm].D.size() == 0)
			{
				return "";
			}
		}
			//cout << Y[yy].M[mm].D[dd].U[uu].data[i] << endl;
		int j = 0;
		while (1)
		//for (int j = 0; j < 5; ++j)
		{
			if ( (uu-j) < 0 && (uu+j) >= Y[yy].M[mm].D[dd].U.size())
			{
				break;
			}
			if ((uu+j) < Y[yy].M[mm].D[dd].U.size())
			{
				if (Y[yy].M[mm].D[dd].U[uu+j].undly == undly)
				{
					return Y[yy].M[mm].D[dd].U[uu+j].data[i];
				}
			}
			if ((uu-j) >= 0)
			{
				if (Y[yy].M[mm].D[dd].U[uu-j].undly == undly)
				{
					return Y[yy].M[mm].D[dd].U[uu-j].data[i];
				}
			}
			j++;
		}

		return "";
		
	}


	void test()
	{
		printf("hello world!\n");

		vector<int> *v = new vector<int>;

		int i = 1;

		v->push_back(1);
		v->push_back(2);
		v->push_back(3);
		v->push_back(4);

		cout << (*v)[0] << endl;

		delete v;
		
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
		loaddata();
		//testoutput();
		output();
	}
};
#endif