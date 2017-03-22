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
	vector<Undly> *U;// = new vector<Undly>;
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
	    			for (int uu = 0; uu < Y[yy].M[mm].D[dd].U->size(); ++uu)
	    			{
	    				for (i = 0; i < Y[yy].M[mm].D[dd].U->at(uu).data.size()-1; ++i)
	    				{
	    					file << Y[yy].M[mm].D[dd].U->at(uu).data[i] << ",";
	    				}
	    				file << Y[yy].M[mm].D[dd].U->at(uu).data[i] << "\n";	    				
	    			}
	    			//delete Y[yy].M[mm].D[dd].U;
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
		  // print all the files and directories within directory 
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
		//cout << ISEfilelist[90] << endl;
		//for (int i = 3; i < ISEfilelist.size(); ++i)
		for (int i = 2; i <= 2; ++i)
		{
			readfile(CRSPfilelist[i], 1);
			//readfile(ISEfilelist[90], 1);
			//readfile(ISEfilelist[91], 1);
		}
		//cout << CRSPfilelist[9] << endl;
		//readfile(CRSPfilelist[9], 2);
		//readfile(CRSPfilelist[3], 2);

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
	    //for (int n = 0; n < 750; ++n)
	    {
	    	for (int i = 0; i < (10); i++){
		        getline(file,entry,',');
		        newline.push_back(entry);
		        }
	        getline(file,entry,'\n');
	        newline.push_back(entry);// read one line

	        //printline(newline);
	        if (!newline[0].empty() && !newline[6].empty())
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
		{
			uu = newline[6];
			uu.erase(remove_if(uu.begin(), uu.end(), ::isdigit),uu.end());
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
				if (k == 6 && flag == 1)
				{
					newline[k].erase(remove_if(newline[k].begin(), newline[k].end(), ::isdigit),newline[k].end());
				}
				Y[yy-5].M[mm-1].D[i].U->at(j).data.push_back(newline[k]);
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
			Y[yy-5].M[mm-1].D[0].U = new vector<Undly>;
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
					Y[yy-5].M[mm-1].D[i].U = new vector<Undly>;
					break;
				}
				
			}
		}
		return i;
	}
	int undly2i(int yy,int mm,int i, string uu, int flag)
	{
		//cout << "now is in undly2i" << endl;
		int j = 0;
		if (Y[yy-5].M[mm-1].D[i].U->size() == 0)
		{
			Y[yy-5].M[mm-1].D[i].U->push_back(Undly());
			Y[yy-5].M[mm-1].D[i].U->at(0).undly = uu;
		}
		else
		{
			while (Y[yy-5].M[mm-1].D[i].U->at(j).undly != uu)// find undly
			{
				//cout << "j = " << j << endl;
				j++;
				if (j == Y[yy-5].M[mm-1].D[i].U->size())// cant find, create a new undly
				{
					if (flag == 2)
					{
						return -1;
					}
					Y[yy-5].M[mm-1].D[i].U->push_back(Undly());
					Y[yy-5].M[mm-1].D[i].U->at(j).undly = uu;

					break;
				}
				
			}
		}
		//cout << "leaveing unly2i" << endl;
		return j;
	}

	void output(int yy, int mm)
	{
		ofstream file;
		char filename[20];
	    //file.open ("testoutput.csv");
	    
	    
	   
	    //for (int yy = 0; yy < Y.size(); ++yy)
	    {
	    	//for (int mm = 0; mm < Y[yy].M.size(); ++mm)
	    	{
	    		sprintf(filename,"./output/CRSP_20%02d_%02d.csv",yy+5,mm+1);
	    		cout << "writing " << filename << "...\n";
	    		file.open (filename);
	    		file << "PERMNO,"<<"DATE,"<<"SHRCD,"<<"TICKER,"<<"COMNAM,"<<"SHRCLS,"<<"TSYMBOL,"<<"CUSIP,"<<"PRC,"<<"VOL," << "RET,";
	    		file <<"PRICEL1,";
			    file <<"VOL,"<<"VOLL1,"<<"VOLL2,"<<"VOLL3,"<<"VOLL4,"<<"RETL1,"<<"RET,";
			    file <<"RETP1,"<<"RETP2,"<<"RETP3,"<<"REPT4,"<<"RETP5"<< endl;
	    		for (int dd = 0; dd < Y[yy].M[mm].D.size(); ++dd)
	    		{
	    			for (int uu = 0; uu < Y[yy].M[mm].D[dd].U->size(); ++uu)
	    			{
	    				//cout << yy+5 << "/" << mm+1 << "/" << Y[yy].M[mm].D[dd].day <<endl;

	    				for (int j = 0; j < 11; ++j)
	    				{
	    					file << Y[yy].M[mm].D[dd].U->at(uu).data[j] << ",";
	    				}
	    	
		    			file << finddata(yy,mm,dd,uu,Y[yy].M[mm].D[dd].U->at(uu).undly,8,-1) << ",";
	    				
						//if (Y[yy].M[mm].D[dd].U->at(uu).data.size() > 10)
	    				{
			    			for (int j = 0; j <= 4; ++j)
		    				{
			    				file << finddata(yy,mm,dd,uu,Y[yy].M[mm].D[dd].U->at(uu).undly,9,-j) << ",";
			    				//file << "" << ",";
			    			}

			    			for (int j = -1; j <= 5; ++j)
		    				{
			    				file << finddata(yy,mm,dd,uu,Y[yy].M[mm].D[dd].U->at(uu).undly,10,j) << ",";
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
		//cout << "now in find data" << endl;

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
				return "NA";
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
			if (Y.size() == yy || Y[yy].M.size() == mm || Y[yy].M[mm].D.size() == 0)
			{
				return "NA";
			}
		}
		
		//cout <<"in finddata " << yy+5 << "/" << mm+1 << "/" << Y[yy].M[mm].D[dd].day <<endl;
		int j = uu;
		int k = uu;
		while (1)
		//for (int j = 0; j < 5; ++j)
		{
			if ( j < 0 && k >= Y[yy].M[mm].D[dd].U->size())
			{
				break;
			}
			if (k < Y[yy].M[mm].D[dd].U->size() && k >= 0)
			{
				if (Y[yy].M[mm].D[dd].U->at(k).undly == undly)
				{
					return Y[yy].M[mm].D[dd].U->at(k).data[i];
				}
			}
			k++;
			if (j < Y[yy].M[mm].D[dd].U->size() && j >= 0)
			{
				if (Y[yy].M[mm].D[dd].U->at(j).undly == undly)
				{
					return Y[yy].M[mm].D[dd].U->at(j).data[i];
				}
			}
			j--;
		}

		return "0";
		
	}

	void freenew(int yy, int mm)
	{
		//for (int yy = 0; yy < Y.size(); ++yy)
	    {
	    	//for (int mm = 0; mm < Y[yy].M.size(); ++mm)
	    	{
	    		cout << "deleteint " << yy+5 << "/" << mm+1 << "...\n";
	    		for (int dd = 0; dd < Y[yy].M[mm].D.size(); ++dd)
	    		{
	    			delete Y[yy].M[mm].D[dd].U;
	    		}
	    		
	    	}
	    }
	}


	void test()
	{
		printf("hello world!\n");
		vector<Day> Dtest;
		string stest = "test";
		Dtest.push_back(Day());
		Dtest[0].day = 502;
		Dtest[0].U = new vector<Undly>;
		Dtest[0].U->push_back(Undly());
		Dtest[0].U->at(0).undly = stest;


		cout << Dtest[0].U->at(0).undly << endl;

		delete Dtest[0].U;
		
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
		int yy = 5,mm = 5;
		int ly = 0, lm = 0;
		readfile(CRSPfilelist[2], 1);
		readfile(CRSPfilelist[3], 1);
		output(yy-5,mm-1);
		ly = yy;
		lm = mm;
		mm++;

		for (int i = 4; i < CRSPfilelist.size(); ++i)
		//for (int i = 4; i <=4 ; i++)
		{
			

			readfile(CRSPfilelist[i], 1);
			//cout << CRSPfilelist[i] << endl;
			//cout << yy << "/" << mm << endl;
			output(yy-5,mm-1);
			freenew(ly-5,lm-1);
			ly = yy;
			lm = mm;
			yy = yy + (mm + 1 - 1) / 12;
			mm = (mm + 1 - 1) % 12 + 1;
		}
		output(yy-5,mm-1);
		freenew(ly-5,lm-1);
		freenew(yy-5,mm-1);


		//loaddata();
		//testoutput();
		
	}
};
#endif