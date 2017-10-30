//#include <bits/stdc++.h>
#include <iostream>
#include <set>
#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>
#include <map>
#include <queue>
#define MAX 10000
#define pb push_back 
using namespace std;

char tempstore[1000];
int one[MAX],zero[MAX];
vector<int>data[MAX];
vector<set<int> >frequentItemset[20];
vector<set<int> >candidateSet;
vector< pair< set<int>,set<int> > >finalRules[20];

map<set<int>,int>mapset;
map<set<int>,int>finalmap[20];
map<int,string> decode;

void dcode()
{
	decode[1]=" handicapped-infants=y ";
	decode[-1]=" handicapped-infants=n ";
	decode[2]="water-project-cost-sharing=y ";
	decode[-2]="water-project-cost-sharing=n ";
	decode[3]="adoption-of-the-budget-resolution=y ";
	decode[-3]="adoption-of-the-budget-resolution=n ";
	decode[4]="physician-fee-freeze=y ";
	decode[-4]="physician-fee-freeze=n ";
	decode[5]="el-salvador-aid=y ";
	decode[-5]="el-salvador-aid=y ";
	decode[6]="religious-groups-in-schools=y ";
	decode[-6]="religious-groups-in-schools=n ";
	decode[7]="anti-satellite-test-ban=y ";
	decode[-7]="anti-satellite-test-ban=n ";
	decode[8]="aid-to-nicaraguan-contras=y ";
	decode[-8]="aid-to-nicaraguan-contras=n ";
	decode[9]="mx-missile=n";
	decode[-9]="mx-missile=n ";
	decode[10]="immigration=y ";
	decode[-10]="immigration=n ";
	decode[11]="synfuels-corporation-cutback=y ";
	decode[-11]="synfuels-corporation-cutback=n ";
	decode[12]="education-spending=y ";
	decode[-12]="education-spending=n ";
	decode[13]="superfund-right-to-sue=y ";
	decode[-13]="superfund-right-to-sue=n ";
	decode[14]="crime =y ";
	decode[-14]="crime=n ";
	decode[15]="duty-free-exports=y ";
	decode[-15]="duty-free-exports=n ";
	decode[16]="export-administration-act-south-africa=y ";
	decode[-16]="export-administration-act-south-africa=n ";
	decode[17]="democrat ";
	decode[-17]="republican ";
	
}





float min_threshold = 0.3;
float min_conf_threshold = 0.5;
int line  = 0;
int ct = 0;


struct node{
	int index;
	vector<set<int> >nodeData;
	int isLeaf;
	struct node* arr[20];
};


queue<node*>q;
// void print(){

// 	for(int i = 0; i<line;i++){
// 	    for(int j = 0; j<data[i].size();j++){
//     		cout<<data[i][j]<<" ";
//     	}

//     cout<<endl;
// 	}
// }



//Pass transaction


void trans(struct node* root,vector<int>currentTrans,vector<int>&vec,int cardinality){
if(currentTrans.size()<cardinality || root == NULL)return;

	if(root->isLeaf == 1){
		//if(cardinality > 3 && vec.size() == cardinality){
		//for(int i = 0; i<vec.size();i++)cout<<vec[i]<<"  ";
		//	cout<<endl;
		//}
		//cout<<vec[0]<<" "<<vec[1]<<" "<<vec.size()<<endl;
		
		ct++;
		for(int i=0;i<root->nodeData.size();i++){
			set<int>temp = root->nodeData[i];
			set<int>::iterator it = temp.begin();

			//print
			// while(it!=temp.end()){cout<<*it<<" ";++it;}
			// cout<<endl;
			// it = temp.begin();
			//
			int flag = 0;
			sort(vec.begin(),vec.end());
			for(int j=0;j<vec.size();j++){
				//if(vec[j] != *it)
				//cout<<vec[j]<<"  "<<*it<<endl;
					 if(vec[j] != *it){
					 	flag = 1;
						break;
					 }
					it++;
			}

			//cout<<endl<<endl;
			if(!flag){mapset[temp]++;}
		}

		//cout<<endl;
		return;

	}
	else{

			int temp = currentTrans[0];
			currentTrans.erase(currentTrans.begin());
			vec.pb(temp);

			trans(root->arr[(temp%cardinality + cardinality)%cardinality],currentTrans,vec,cardinality);
			vec.pop_back();
			trans(root,currentTrans,vec,cardinality);
	}
}


     
void rules_generator(set<int> FI)
{

	int sc;
	double conf;
	sc=finalmap[FI.size()][FI];
	set<int> temp;
	vector<set<int> > cr[20],fr[20];
	//for(int j=0;j<FI.size();j++)
		for(set<int>::iterator it=FI.begin();it!=FI.end();it++)
		{
			set<int>golu;
			golu.insert(*it);	
			cr[1].push_back(golu);
		}
	
	int x=0;
	for(int i=0;i<cr[1].size();i++)
	{
		temp=FI;
		for(set<int>::iterator it=cr[1][i].begin();it!=cr[1][i].end();it++)
		if(temp.find(*it)!=temp.end())
			temp.erase(*it);
		//cout<<"sc= "<<sc<<" "<<"fmap = "<<finalmap[temp.size()][temp]<<endl;
		conf=(double)sc/(double)finalmap[temp.size()][temp];

		//cout<<"confidence="<<conf<<endl ;


		if(conf>=min_conf_threshold)
		{
			fr[1].push_back(cr[1][i]);
			finalRules[FI.size()].push_back(make_pair(temp,cr[1][i]));
		}

	}
	
	for(int i=2;i<FI.size();i++)
	{
		// cout<<"csmdcsc";
		int x=0;
		for(int j=0;j<fr[i-1].size();j++)
		{
			for(int k=0;k<fr[1].size();k++)
			{
				if(fr[i-1][j].find(*fr[1][k].begin())==fr[i-1][j].end())
				{
					cr[i].push_back(fr[i-1][j]);
					cr[i][cr[i].size()-1].insert(*fr[1][k].begin());
					
				}
			}
		}

		for(int j=0;i<cr[i].size();i++)
		{
			temp=FI;
			for(set<int>::iterator it=cr[i][j].begin();it!=cr[i][j].end();it++)
			if(temp.find(*it)!=temp.end())
				temp.erase(*it);
			conf=(double)sc/(double)finalmap[temp.size()][temp];
			if(conf>=min_conf_threshold)
			{
				fr[i].push_back(cr[i][j]);
				finalRules[FI.size()].push_back(make_pair(temp,cr[i][j]));
			}
		}


	}
}


// remember to clear candidate set at the end and map
int main()
{




	FILE *inputfile = fopen("vote.arff","r+");   //trainingdata file


	

    while(fscanf(inputfile, "%[^\n]s",tempstore)!=EOF)
    { 															 //storing training data in training data vector
        int len = strlen(tempstore),j = 0;
 		

 		for(int i = 0; i<len ;i++)
 		{
 			if(tempstore[i] == 'y')data[line].pb(1);
 			else if(tempstore[i] == 'n')data[line].pb(0);
 			else if(tempstore[i] == 'd'){data[line].pb(1);break;} // democrat = 1
 			else if(tempstore[i]== 'r'){data[line].pb(0);break;}
 			else if(tempstore[i] =='?')data[line].pb(-1);
 		}
 		line++;
       	fgetc(inputfile);

    }
    fclose(inputfile);

    //cout<<line<<endl;

    for(int i = 0; i<line;i++)
    for(int j = 0; j<data[i].size();j++)
    {
    	if(data[i][j] == 1)one[j]++;
    	else if(data[i][j] == 0)zero[j]++;
    }

    for(int i = 0; i<line;i++)
    for(int j = 0; j<data[i].size();j++)
    {
    	if(data[i][j] == -1 && one[j]>zero[j]){data[i][j] = 1;one[j]++;}
    	else if(data[i][j] == -1 && one[j]<=zero[j]){data[i][j]  = 0;zero[j]++;}
    }

//computation of F1

	for(int i = 0; i<17; i++)
	{
		set<int>temp;
		// cout<<one[i]+zero[i]<<endl;
		if(one[i]>zero[i] && one[i]>=min_threshold*line)
		{
			temp.insert(i+1);
			frequentItemset[1].pb(temp);
			mapset[temp]=one[i];
		}
		else if(one[i]<=zero[i] && zero[i]>=min_threshold*line)
		{
			temp.insert(-(i+1));
			frequentItemset[1].pb(temp);
			mapset[temp] = zero[i];
		}
			
	}
	finalmap[1] = mapset;

	// for(int i = 0; i<frequentItemset[1].size();i++)cout<<*frequentItemset[1][i].begin()<<" ";

	//to pass transactions change data
	for(int p = 0; p<line;p++)
	{
		for(int q = 0; q<data[p].size();q++)
		{
			if(data[p][q])
			data[p][q] = (q+1);
			else
			data[p][q] = -(q+1);
		}
	}

	for(int p = 0; p<line;p++)
	{
		sort(data[p].begin(),data[p].end());
	}


	//TREES OF i CARDINALITY

	for(int i = 2;i<=17;i++)
	{

		node* root = new node;
		root->index = 0;
		root->isLeaf = 0;

		map<set<int>,int>tempMap;  //to store unique sets
		for(int t = 0; t<20;t++)root->arr[t] = NULL;

			for(int j = 0;j<frequentItemset[i-1].size();j++)
			{     //crossproduct of (i-1)*1
				set<int>temp = frequentItemset[i-1][j];
				
				for(int k = 0; k<frequentItemset[1].size();k++)
				{
					set<int>val = frequentItemset[1][k];
					set<int>ans;
					ans.insert(*val.begin());
					for(set<int>::iterator it= temp.begin();it!=temp.end();++it)ans.insert(*it);
					
					if(tempMap.find(ans)==tempMap.end()&& ans.size() == i)
					{
						candidateSet.pb(ans);
						tempMap[ans] = 1;

					}
				}
			}

		// tempMap.clear();
		
		//if(i == 3)cout<<candidateSet.size();
		// for(int j = 0; j<candidateSet.size();j++)
		// cout<<*candidateSet[j].begin()<<" "<<*candidateSet[j].rbegin()<<endl;
		// cout<<candidateSet.size()<<endl;


		//pruning

		for(int j = 0; j<candidateSet.size();j++)
		{
			//cout<<*candidateSet[j].begin()<<" "<<*candidateSet[j].rbegin()<<endl;
			set<int>temp = candidateSet[j];

			for(set<int>::iterator it = temp.begin();it!=temp.end();it++)
			{
				temp.erase(*it);

				if(find(frequentItemset[i-1].begin(),frequentItemset[i-1].end(),temp)==frequentItemset[i-1].end())
				{
					candidateSet.erase(candidateSet.begin() + j);
					break;
				}
				temp.insert(*it);

			}
		}


		

		//CREATE HASH TREE
		
		for(int j = 0; j<candidateSet.size();j++)
		{
			
			root->nodeData.pb(candidateSet[j]);		
		}
		q.push(root);

		while(!q.empty())
		{
			
			struct node* currentNode = q.front();
			q.pop();
			int currentIndex = currentNode->index;

			vector<set<int> >currentNodedata = currentNode->nodeData;

			if(currentNodedata.size() <= 1 || currentIndex == i)
			{
				currentNode->isLeaf = 1;
				continue;
			} // can't separate further

			for(int j = 0; j< currentNodedata.size();j++)
			{
				set<int>tempset = currentNodedata[j];
				
				set<int>::iterator it = tempset.begin();
				int pos = 0;
				while(pos<currentIndex){++it;pos++;}

				int modevalue = ((*it)%i+i)%i; 

				

				if(currentNode->arr[modevalue] == NULL)
				{
					node* newNode = new node;
					newNode->isLeaf = 0;
					newNode->index = currentIndex+1;
					for(int k = 0; k<20; k++)newNode->arr[k] = NULL;
					currentNode->arr[modevalue] = newNode;
					q.push(newNode);
				}

				struct node* datapushinNode = currentNode->arr[modevalue];

				datapushinNode->nodeData.pb(tempset);
			
			}

		}

		//HASH TREE DONE

		for(int t = 0; t<line; t++)
		{
			vector<int>emptySet;
			trans(root,data[t],emptySet,i);
		}

		//cout<<mapset.size()<<endl;
		int ttt = 0;
		for(map<set<int>,int>::iterator it = mapset.begin();it!=mapset.end();it++)
		{
			set<int>temp = (*it).first;
			int county = (*it).second;
		 	if((double)county/line>min_threshold)
		 	{
		 		//cout<<ttt<<":";
		 		ttt++;
		 		frequentItemset[i].pb(temp);
				// for(set<int>::iterator pd = temp.begin();pd!=temp.end();pd++){
				// 	cout<<*pd<<" ";
		 	//  	}
		 	//  	cout<<endl;
			}
		}
		

		// cout<<ct<<endl;

		candidateSet.clear();
		finalmap[i] = mapset;
		mapset.clear();


		//cout<<"\n\n\n\ni:"<<i<<" ct:"<<ct<<"\n";
		ct = 0;
	}

	//Rule Generation

	for(int i=2;i<20;i++)
	{
		for(int j=0;j<frequentItemset[i].size();j++)
		{
			 // for(set<int>::iterator it=frequentItemset[2][j].begin();it!=frequentItemset[2][j].end();it++)
			 // cout<<*it<<" "<<endl;
			rules_generator(frequentItemset[i][j]);
		}
		//cout<<"\n";
	}
	cout<<"min support threshhold="<<min_threshold<<"\n\n\n\nmin confidence threshold ="<<min_conf_threshold<<endl<<"\n\n\n\n\n";
	dcode();	
	int cnt=0;
	for(int i=2;i<20;i++)
	{
		int count =0;
		cout<<"Rules for "<<i<<" cardinality frequentItemset\n";
		for(int j=0;j<finalRules[i].size();j++)
		{
			cnt++;
			cout<<++count<<" : { ";
			for(set<int>::iterator it=finalRules[i][j].first.begin();it!=finalRules[i][j].first.end();it++)
			 cout<<decode[*it]<<" ,";
			cout<<"} --->  {";
			for(set<int>::iterator it=finalRules[i][j].second.begin();it!=finalRules[i][j].second.end();it++)
			 cout<<decode[*it]<<" ,";
			cout<<"}\n";
		}
		cout<<"\n\n\n\n";

		cout<<"Total no of rules generated ="<<cnt<<"\n\n\n";
	}
	
}




