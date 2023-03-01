/*
0 �������ݼ�
1 �β�����ݼ�
2 �����������ݼ�
3 �����������ݼ�

*/

#include<bits/stdc++.h>
#include <windows.h>
#include <stdio.h>
#define debug(x)	cout<<"debug	"<<x<<"\n";

using namespace std;

struct AttrTable{//�� 
	string FarEdg;//���ڸ��׽ڵ��׼��ȡֵ 	����/�Ժ�/ģ�� 
	vector<string> AttrNameList;//������
	vector<int> AttrNameUsed;//�����Ƿ�ʹ�� 0δʹ�ù� 1ʹ�ù�  2����ֵ
	vector<vector<string> > RowData;//�����һ�α� 
	vector<vector<string> > ColData;//�����һ�α� 
	
	map<string, vector<string> > AttrAllVal;//ĳ���������п��ܵ�ȡֵ 
	
	//������ 
	void restore(){
		int ii =  RowData.size();
		int jj = AttrNameList.size();
		for(int j = 0;j < jj; ++j){
			vector<string> now;
			for(int i = 0;i < ii; ++i){
				now.push_back(RowData[i][j]);
			}
			ColData.push_back(now);
		}
	}
	
	//��ȡ�������п���ȡֵ 
	void getallval(){
		//��ͬ����ֵ������ͬmap 
		map<string, map<string,int > >  MpAttrJudge;
		for(int i = 0;i < ColData.size() - 1; ++i){//�����Ϣ���� 
			for(int j = 0;j < ColData[i].size(); ++j){ 
				//���ԣ�����ֵ�� ûȡ�� 
				if(MpAttrJudge[ AttrNameList[i] ][ ColData[i][j] ] == 0){
					MpAttrJudge[ AttrNameList[i] ][ColData[i][j]] = 1;
					AttrAllVal[ AttrNameList[i] ].push_back(ColData[i][j]);
				}
			}
		}
	}
};

struct node{//��� 
	int num;//���
	
	vector<int>	child; //���ӽڵ��б� 
	map<string,int > way;//���ڸ���������ֵ��ѡ��ĳ�����ӵݹ� 
	
	bool flag = false;//�Ƿ�Ҷ���
	string catg;//Ҷ�ӽڵ��¼��� 
	
	int SuitAttr;//���ֵı�׼�ǵڼ���
	string StrSuitAttr;//���ֵı�׼ 
	string Edge;//���ڸ��׽ڵ��׼��ȡֵ 	����/�Ժ�/ģ�� 
};

struct Find{
	int type;//0��ɢ 1���� 
	int idx;//��idx������
	string Sattr;//�����ַ��� 
	double divi;//����ֵ�Ļ��ֵ� 
}; 

AttrTable OrigiTable;//��ʼ��ѵ����D 
vector<node> tr;//��
int tridx = -1; 
double ContiJudge = 0.0; 
 
void test(AttrTable tab){
	cout<<"t1\n";
	for(int i = 0;i < tab.RowData.size(); ++i){
		for(int j = 0;j < tab.RowData[i].size(); ++j){
			cout<< tab.RowData[i][j]<<"	";
		}
		cout<<"\n";
	}
//	cout<<"t2\n";
//	for(int i = 0;i < tab.ColData.size(); ++i){
//		for(int j = 0;j < tab.ColData[i].size(); ++j){
//			cout<< tab.ColData[i][j]<<"	";
//		}
//		cout<<"\n";
//	}
	cout<<"t3\n";
	for(int i = 0;i < tab.AttrNameList.size() - 1; ++i){
		cout << "==" << tab.AttrNameList[i] <<"\n";
		for(int j = 0;j < tab.AttrAllVal[ tab.AttrNameList[i] ].size(); ++j){
			cout<< tab.AttrAllVal[ tab.AttrNameList[i] ][j]<<" ";
		}
		cout << "\n";
	} 
}
void read(){//������
	SetConsoleOutputCP(65001);
	ifstream in("train3.txt");
	string s="";
	
	bool flag = true;//�����б�־ 
	while(getline(in,s)){
		vector<string> now;
		//ÿ�ζ���һ�� 
		int pre = 0;
		for(int i=0;i < (int)s.size(); ++i){
			if(s[i] == '\t'){
				string blo = s.substr(pre,i - pre);//�и���� 
				now.push_back(blo);
				pre = i + 1; 
			}
		}
		string blo = s.substr(pre,(int)s.size() - pre);//��ĩ�� 
		now.push_back(blo);
		
		if(flag){
			OrigiTable.AttrNameList = now;
			flag = false;
		}
		else{
			OrigiTable.RowData.push_back(now);
		}
	}
	
	OrigiTable.restore();
	OrigiTable.getallval();
	
	for(int i=0;i < OrigiTable.AttrNameList.size() ; ++i){
		if(OrigiTable.AttrNameList[i][0] == '*'){//����ֵ 
			OrigiTable.AttrNameUsed.push_back(2);
		}
		else{
			OrigiTable.AttrNameUsed.push_back(0);
		}
	}
}

string FindAttrMax(AttrTable tab){//�ҵ���ǰD������������ 
	int las = (int)tab.ColData.size() - 1;
	map<string,int> mp;
	for(int i=0;i < tab.ColData[las].size(); ++i){
		mp[tab.ColData[las][i]] ++ ;
	} 
	int maxx = -1;string ans = "";
    for(auto &it : mp){
    	if(maxx < it.second){
    		maxx = it.second;
    		ans = it.first;
		}
    }
	return ans; 
}

double Gini(AttrTable tab){//���� Gini(D) 
	double GiniD = 0.0;
	int sum = tab.RowData.size();
	int las = (int)tab.ColData.size() - 1;//��� 
	map<string,int> mp;//�������� 
	for(int i=0;i < tab.ColData[las].size(); ++i){
		mp[tab.ColData[las][i]] ++ ;
	} 
	for(auto &u:mp){
		//u.second ÿ�����ĸ���
		//	cout<<"pi"<<" "<<u.second<<"\n";
		double pk = (double)u.second / (double)sum;
		GiniD = GiniD + pk * pk;
	}
	GiniD = 1.0 - GiniD;
	return GiniD;
}

Find FindSuitAttr(AttrTable tab){//�ҵ�D���ŵĻ������� 
	Find ans;double MinGini = 1e8;
	for(int i = 0;i < tab.AttrNameList.size() - 1; ++i){//����ÿһ��û�ù�������a ɫ�� 
		if(tab.AttrNameUsed[i] == 1)	continue;
		cout<<"now try use " << tab.AttrNameList[i] <<"\n";
		
		if(tab.AttrNameUsed[i] == 2){//���Ե�����������ֵ 
			vector<double> T,Ta;//��ʱ�����ֵ� 
			for(int j = 0;j < tab.AttrAllVal[ tab.AttrNameList[i] ].size(); ++j){//���ԵĲ�ֵͬav �ܶ� 
				string NowAttr =  tab.AttrAllVal[ tab.AttrNameList[i] ][j];//���� 0.814 
				T.push_back(stod(NowAttr));
			}
			sort(T.begin() , T.end());
			for(int j = 0;j < T.size() - 1; ++ j)	Ta.push_back( (T[j] + T[j+1])/2.0 );
			
			//����ÿһ�����ֵ�
			for(int j = 0;j < Ta.size() ; ++ j){
				double Tjudge = Ta[j];//���ֵ� 
				cout << "now Ta" << Tjudge <<"	";
				
				AttrTable NewtabP,NewtabN;//�� �� 
				NewtabP.AttrNameList = tab.AttrNameList;
				NewtabP.AttrNameUsed = tab.AttrNameUsed;
				NewtabN.AttrNameList = tab.AttrNameList;
				NewtabN.AttrNameUsed = tab.AttrNameUsed;
				
				for(int jj = 0 ;jj < tab.RowData.size(); ++jj){
					if(stod(tab.RowData[jj][i]) < Tjudge){
						NewtabN.RowData.push_back(tab.RowData[jj]);
					}
					else{
						NewtabP.RowData.push_back(tab.RowData[jj]);
					}
				}
				
				NewtabP.restore();
				NewtabP.getallval();
				NewtabN.restore();
				NewtabN.getallval();
				
				double Gini_index = 0.0;
				Gini_index =  (double)NewtabP.RowData.size()/(double)tab.RowData.size() * Gini(NewtabP);
				Gini_index = Gini_index + (double)NewtabN.RowData.size()/(double)tab.RowData.size() * Gini(NewtabN);
				
				cout  << Gini_index <<"\n"; 
				if(Gini_index < MinGini){
					MinGini = Gini_index;
					ans = {1,i,tab.AttrNameList[i],Tjudge};
				}
			}
		}
		else{//���Ե���������ɢֵ 
			double Gini_index =  0.0;
			for(int j = 0;j < tab.AttrAllVal[ tab.AttrNameList[i] ].size(); ++j){//���ԵĲ�ֵͬav ��ɫ 
				string NowAttr =  tab.AttrAllVal[ tab.AttrNameList[i] ][j];//���� �ں� 
				
				//��ĳ������ֵav ���ֳ��±� 
				AttrTable Newtab;
				Newtab.AttrNameList = tab.AttrNameList;
				Newtab.AttrNameUsed = tab.AttrNameUsed;
				for(int tt = 0; tt < tab.RowData.size(); ++tt){
					if(tab.RowData[tt][i] == NowAttr){
						Newtab.RowData.push_back(tab.RowData[tt]);
					}
				}
				Newtab.restore();
				Newtab.getallval();
				
				// ��� |Dv|/|D| * Gini(Dv)  
				Gini_index = Gini_index + (double)Newtab.RowData.size() / (double)tab.RowData.size() * Gini(Newtab);
			}
			if(Gini_index < MinGini){
				MinGini = Gini_index;
				ans = {0,i,tab.AttrNameList[i],0.0};
			}
			cout << "	" << Gini_index <<"\n"; 
		}
	}
	cout <<"==	"<< ans.type <<"	"<<ans.Sattr <<"	"<<ans.divi<<"\n";
	return ans;
}

void DecisionTreerBuild(AttrTable tab,int fa,string edg){//ѵ���� ���׽ڵ� ���ڸ��׽ڵ��׼��ȡֵ 
	test(tab);
	
	//�½�һ���ڵ�
	node Now;
	Now.num = ++ tridx;
	tr.push_back(Now);
	if(fa != -1){
		tr[Now.num].Edge = edg;
		tr[fa].child.push_back(Now.num);
		tr[fa].way[edg] = Now.num;
	}
	
	//�жϵ�ǰ�ڵ�ѵ�����ǲ��ǰ�������������ͬһ�� 
	bool judge = true;
	for(int i = 1;i < (int)tab.RowData.size(); ++i){
		if(tab.RowData[0].back() != tab.RowData[i].back()){
			judge = false;	break;
		}
	}
	if(judge){//��Ϊͬ�� 
		tr[Now.num].flag = true;
		tr[Now.num].catg = tab.RowData[0].back();
		return ;
	}
	
	//���Լ�Ϊ�ռ� ���� ��ǰѵ�����ڵ�ǰ�����Լ���ȡֵ��ͬ
	bool judge2 = true , judge3 = true;//2���D��������A��ȡֵ��ͬ 3���AΪ�� 
	for(int i = 0;i < (int)tab.ColData.size() - 1; ++i){//ǰ���Ѿ��й��˷��࣬�ض�����ͬ 
		for(int j = 1;j < (int)tab.ColData[i].size(); ++j){
			if(tab.ColData[i][j] != tab.ColData[i][0]){
				judge2 = false;	break;
			}
		}
	}
	for(int i = 0;i < tab.AttrNameUsed.size(); ++i){
		if(tab.AttrNameUsed[i] != 1){//����û�õ� 
			judge3 = false;	break;  
		}
	}
	if(judge2||judge3){
		tr[Now.num].flag = true;
		tr[Now.num].catg = FindAttrMax(tab);
		return ;
	}
	
	//Ѱ�Һ��ʵĻ��ֱ�׼ 
	Find get = FindSuitAttr(tab);
	
	if(get.type == 0){//��ɢֵ 
		//���ʵĻ���ֵ 
		tr[Now.num].SuitAttr = get.idx;
		tr[Now.num].StrSuitAttr = get.Sattr;
		
		//��ͬ����ֵ ����ͬһvector 
		map<string, vector<int> > MpAttr;
		for(int i = 0;i < (int)tab.RowData.size() ; ++i){
			MpAttr[ tab.RowData[i][get.idx] ].push_back(i);
		}
		
		//��ͬ������ֵ���첻ͬ�ķ�֧��ע����������ֵΪ��ʼD�п��ܵ�����ȡֵ Ϊ���п��ܵ����Խ��� 
		cout<<"qu chu shuxing	"<< get.Sattr <<"\n";
		
		for(int i = 0;i < OrigiTable.AttrAllVal[get.Sattr].size(); ++i){//����ֵ�Ĳ�ͬȡֵ 
			string NowAttrVal =  OrigiTable.AttrAllVal[get.Sattr][i];
			cout<<"shuxing zhi "<< NowAttrVal <<"\n";
			
			AttrTable Newtab;
			Newtab.AttrNameList = tab.AttrNameList;
			Newtab.AttrNameUsed = tab.AttrNameUsed;
			Newtab.AttrNameUsed[ get.idx ] = 1;//�����ѱ�ʹ�� 
			Newtab.FarEdg = NowAttrVal;//av
			for(int j = 0;j < MpAttr[NowAttrVal].size(); ++j){//�����±� 
				int rownum = MpAttr[NowAttrVal][j];
				Newtab.RowData.push_back(tab.RowData[rownum]);//��������ֵ��Ӧ�� 
			}
			
			//���±�Ϊ�գ�˵����ʱD��û�ж�Ӧ����
			//Ϊ�˱�֤���ɵ������ж�D��û�е����ݵķ�������������һ��Ҷ�ӽڵ� 
			if(Newtab.RowData.empty()){
				node Next;
				Next.num = ++ tridx;
				tr.push_back(Next);
				tr[Now.num].child.push_back(Next.num);
				tr[Now.num].way[NowAttrVal] = Next.num;
				tr[Next.num].flag = true;
				tr[Next.num].Edge = Newtab.FarEdg;
				tr[Next.num].catg = FindAttrMax(tab);
				return ;
			}
			else{
				Newtab.restore();//Ϊ���򲻱�Ҫִ�� 
				Newtab.getallval();
				DecisionTreerBuild(Newtab,Now.num,Newtab.FarEdg);
			} 
		}
	}
	else{//����ֵ 
		tr[Now.num].SuitAttr = get.idx;
		tr[Now.num].StrSuitAttr = get.Sattr + "<" + to_string(get.divi);
		
		//С�� 
		AttrTable Newtab;
		Newtab.AttrNameList = tab.AttrNameList;
		Newtab.AttrNameUsed = tab.AttrNameUsed;
	
		//����ֵ���Է���ʹ�� 
		
		Newtab.FarEdg = "<" + to_string(get.divi);//�� 0.824 
		
		for(int i = 0; i < tab.RowData.size() ; ++i){
			if(stod(tab.RowData[i][get.idx]) < get.divi)
				Newtab.RowData.push_back( tab.RowData[i] );
		}
		
		//ѡ���˻��ֵ㣬��ôһ�������ű����ֵ�����
		Newtab.restore();//Ϊ���򲻱�Ҫִ�� 
		Newtab.getallval();
		DecisionTreerBuild(Newtab,Now.num,Newtab.FarEdg);
		
		//���� 
		AttrTable Newtab2;
		Newtab2.AttrNameList = tab.AttrNameList;
		Newtab2.AttrNameUsed = tab.AttrNameUsed;
	
		//����ֵ���Է���ʹ�� 
		
		Newtab2.FarEdg = ">" + to_string(get.divi);//�� 0.824 
		
		for(int i = 0; i < tab.RowData.size() ; ++i){
			if(stod(tab.RowData[i][get.idx]) > get.divi)
				Newtab2.RowData.push_back( tab.RowData[i] );
		}
		
		//ѡ���˻��ֵ㣬��ôһ�������ű����ֵ�����
		Newtab2.restore();//Ϊ���򲻱�Ҫִ�� 
		Newtab2.getallval();
		DecisionTreerBuild(Newtab2,Now.num,Newtab2.FarEdg);
	}
}
void print(){
	for(int i = 0;i < tr.size(); ++ i){
		cout<<"---start---\n";
		cout<<"now node num:"<<tr[i].num<<"\n";
		if(tr[i].flag){
			cout<<"is leaf\n";
			cout<<"judge by far Edge"<<tr[i].Edge<<"\n";
			cout<<"categ :"<<tr[i].catg<<"\n";
		}
		else{
			cout<<"is not leaf\n";
			cout<<"judge by far Edge:"<<tr[i].Edge<<"\n";
			cout<<"node StrSuitAttr "<<tr[i].StrSuitAttr<<"\n";
			cout<<"his child	";
			for(int j = 0;j < tr[i].child.size();++j){
				cout<<tr[i].child[j]<<"	";
			}
			cout<<"\n";
			for(auto &it :tr[i].way){
				cout<<it.first<<"	"<<it.second<<"\n";
			}
		}	
		cout<<"---end---\n";
	}
}

string zj = "";//�Ҳ�֪��Ϊɶ dfs����ֵ�Ļ��ͻ�ݹ����
//�����������һ����̬ 
void dfs(int u,vector<string> now){//���ı�� ��ǩ���  
	if(tr[u].flag){
		cout<<"		"<<tr[u].catg<<"\n";
		zj = tr[u].catg;
		return ;
	}
	
	string wait = now[tr[u].SuitAttr];//�����������ֵ 
//	cout<<"----------"<<wait<<"\n";
	if(OrigiTable.AttrNameUsed[ tr[u].SuitAttr ] == 2){//��ǰ�ڵ�����ֵΪ���� 
		double waitd = stod(wait);
		int pos = tr[u].StrSuitAttr.find('<');
		string judges = tr[u].StrSuitAttr.substr(pos + 1);
		double judged = stod(judges);
		
		string go = "";
		if(waitd < judged){
			go = "<" + judges;
		}
		else{
			go = ">" + judges;
		}
		dfs(tr[u].way[ go ],now);
	}
	else{
		dfs(tr[u].way[ wait ],now);
	}
}
void op(){
	//����
	ifstream in("test3.txt");
	string s="";
	int tot = 0;//��������
	int right_num = 0; 

	while(getline(in,s)){
		++tot;
		vector<string> now;
		
		int pre = 0;
		for(int i=0;i < (int)s.size(); ++i){
			if(s[i] == '\t'){
				string blo = s.substr(pre,i - pre);//�и���� 
				now.push_back(blo);
				pre = i + 1; 
			}
		}
		string blo = s.substr(pre,(int)s.size() - pre);//��ĩ�� 
		now.push_back(blo);
		
		cout<<tot<<"	";
		for(auto q:now)	cout<<q<<"	";
		
		dfs(0,now);
//		cout << "1debug" << zj <<"\n";
//		cout << "2debug" << blo <<"\n";
		if(zj == blo)	right_num ++;
	}
	cout<<"test num = " << tot<<"\n";
	cout<<"right num = " << right_num<<"\n";
	cout<<"Correct rate = " << ((double)right_num / (double)tot);
	system("pause");
}
void DecisionTree(){
	DecisionTreerBuild(OrigiTable,-1,"");
}  
int main(){
	read();
	DecisionTree();
	print();
	op();
	return 0;
}
