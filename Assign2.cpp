#include <bits/stdc++.h>
#include <ctype.h>

using namespace std;

unordered_map<char, int> nonmpt;
unordered_map<char, int> mpt;

bool isTerminal(char c){

	if(nonmpt.find(c) == nonmpt.end())
		return true;
	return false;
}

void findmpt(map<char , vector<string> > cfg){

	for(auto it1 = cfg.begin();it1!=cfg.end();it1++){

		for(auto it2 = it1->second.begin();it2!=it1->second.end();it2++){
		
			string prod = *it2;
			for(int i = 0;i<prod.size();i++){
			
				if(isTerminal(prod[i]))
					mpt[prod[i]] = 1;
			
			}
		
		}
	}


}

void prettyPrint(map<char, vector<char> > cfg){

	map<char , vector<char> >::iterator it1;
	vector<char>::iterator it2;
	
	for(it1 = cfg.begin();it1!=cfg.end();it1++){
	
		cout<<it1->first<<"={";
		int i = 0;
		for(it2 = it1->second.begin();it2!=it1->second.end();it2++){
		
			if(i++ != 0)
				cout<<",";
				
			cout<<*it2;
		
		}
		
		cout<<"}"<<endl;
	
	}

}

void printMap(map<char , vector<string> > cfg){

    for(auto it1 = cfg.begin();it1!=cfg.end();it1++){

        cout<<it1->first<<"=";
        int start = 0;

         unordered_map<string , int> tmp;
         
        for(auto it2 = it1->second.begin();it2!=it1->second.end();it2++){
        
            if(tmp.find(*it2) == tmp.end()){
            
                if(start++ != 0)
                    cout<<"/"; 
                cout<<*it2;
            }
                   
            tmp[*it2] = 1;
        }
        cout<<endl;
    }

}

vector<char> Combine(vector<char> s1, vector<char> s2){

	unordered_map<char, int> tmp;

	vector<char> res;

	for(auto it = s1.begin(); it != s1.end(); it++){
		if(tmp.find(*it) == tmp.end()){
		
			res.push_back(*it);
		
		}
		tmp[*it] = 1;
	
	}
	
	for(auto it = s2.begin(); it!= s2.end(); it++){

		if(tmp.find(*it) == tmp.end()){
		
			res.push_back(*it);
		
		}
	
		tmp[*it] = 1;
	}
	
	return res;


}

bool IsEqual(map<char, vector<char> > s1, map<char, vector<char> > s2){

	if(s1.size() == s2.size()){
	
		map<char, vector<char> >::iterator it;
		
		for(auto it = s1.begin(); it!= s1.end(); it++){
	
			if(it->second.size() != s2[it->first].size())
			
				return false;
	
		}

		return true;
	}
	
	return false;

}

vector<char> removeNull(vector<char> list){

	int index = -1;
	
	for(int i = 0;i < list.size();i++){
	
		if(list[i] == '~')
			index = i;
	
	}
	
	if(index == -1)
		return list;
	else{
	
		list.erase(list.begin() + index); 
	
		return list;
	}

}


bool findFollow(map<char, vector<char> > &followSet, map<char, vector<char> > firstSet, map<char, vector<string> > cfg, unordered_map<char, int> nullSet){

	map<char, vector<char> > newSet;

	for(auto it1 = cfg.begin();it1 != cfg.end();it1++){	
	
		vector<char> setTemp = followSet[it1->first];
		for(auto it = cfg.begin();it != cfg.end();it++){
			
			for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++){
			
				string prod = *it2;
				bool check = false;
				for(int i = 0; i <= prod.size(); i++){
				
					if(i == prod.size() && check){
						
							setTemp = Combine(setTemp, followSet[it->first]);
							break;
					}
						
					if(prod[i] == it1->first){
					
						check = true;
						
					
					}else if(check){
						
						if(isTerminal(prod[i])){
						
							vector<char> single;
							single.push_back(prod[i]);
							setTemp = Combine(setTemp, single);
							check = false;
							break;
					
						}else{
						
							setTemp = Combine(setTemp, removeNull(firstSet[prod[i]]));
					
							if(nullSet.find(prod[i]) == nullSet.end()){
								check = false;
								break;
							}
						}	
						
					}
				}
			
			}
		}
		
		newSet[it1->first] = setTemp;
	}
	
	if(!IsEqual(newSet, followSet)){
	
		followSet = newSet;
		return false;
	}
	
	return true;
}

bool findFirst(map<char, vector<char> > &firstSet, map<char, vector<string> > cfg, unordered_map<char, int> nullSet){

	bool check = false;

	map<char, vector<char> > newSet;

	for(auto it = cfg.begin();it != cfg.end();it++){
		
		vector<char> prev, setTemp = firstSet[it->first];
		
		for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++){
			
			string prod = *it2;
			for(int i = 0; i < prod.size(); i++){

				if(prod[i] == '~')
					continue;

				else if(isTerminal(prod[i])){
				
					vector<char> single;
					single.push_back(prod[i]);
					setTemp = Combine(setTemp, single);
					break;
				}

				else{
				
					setTemp = Combine(setTemp, firstSet[prod[i]]);

					if(nullSet.find(prod[i]) == nullSet.end())
						break;
						
					
				}
			}
			
			setTemp = Combine(setTemp, prev);
			prev = setTemp;
			
		}
		
		newSet[it->first] = setTemp;
	}
	
	if(!IsEqual(newSet, firstSet)){
	
		firstSet = newSet;
		return false;
	}
	
	return true;

}


void findnullProductions(map<char, vector<char> > &firstSet, unordered_map<char, int> &nullSet, map<char, vector<string> > cfg){

	map<char , vector<string> >::iterator it;
	vector<string>::iterator it2;

	bool check = true;
	while(check){	
		check = false;
		for(it = cfg.begin();it != cfg.end();it++){

			if(nullSet.find(it->first) == nullSet.end()){
			
				for(it2 = it->second.begin(); it2 != it->second.end(); it2++){
				
					string prod = *it2;
					bool check2 = true;
					for(int i = 0; i < prod.size(); i++){
					
						if(nullSet.find(prod[i]) == nullSet.end()){
						
							check2 = false;
							break;
						}
					}
					
					if(check2){
						nullSet[it->first] = 1;
						firstSet[it->first].push_back('~');
						check = true;
						break;
					}
				}
			
			}
		}
		
		
	}

}

vector<char> findliterals(map<char, vector<char> > firstSet, map<char, vector<char> > followSet, unordered_map<char, int> nullSet, char p, string prod){
	vector<char> res;
	
	if(prod[0] == '~'){
		res = followSet[p];
		return res;
	}
	
	int cnt= 0;
	for(int i = 0;i < prod.size();i++){
		
		if(isTerminal(prod[i])){
			vector<char> temp;
			temp.push_back(prod[i]);
			cnt=1;
			res = Combine(res, temp);
			break;
		
		}
		
		res = Combine(res, firstSet[prod[i]]);
		
		if(find(firstSet[prod[i]].begin(), firstSet[prod[i]].end(), '~') == firstSet[prod[i]].end()){
		
		cnt=1;
			break;
			
		}
	
	}
	
	if(!cnt)
		res = Combine(res, followSet[p]);
	
	return res;
}
void createLLTable(map<char, vector<char> > firstSet, map<char, vector<char> > followSet, unordered_map<char, int> nullSet, map<char, vector<string> > cfg){

	map< pair<char, char>, string > ParseTable;
	mpt['$'] = 1;
	
	for(auto it1 = nonmpt.begin(); it1 != nonmpt.end(); it1++){
	
		for(auto it2 = mpt.begin(); it2 != mpt.end(); it2++){
		
			ParseTable[make_pair(it1->first, it2->first)] = "";
		}
	
	}
	 
	for(auto it = cfg.begin(); it != cfg.end(); it++){
		for(auto it3 = it->second.begin(); it3 != it->second.end();it3++){
		
			vector<char> setChar = findliterals(firstSet, followSet, nullSet, it->first, *it3);

			for(int i = 0;i < setChar.size();i++){
			
				if(setChar[i] == '~')
					continue;
					
				if(ParseTable[make_pair(it->first, setChar[i])].size() == 0)
					{
						ParseTable[make_pair(it->first, setChar[i])] = *it3;
					}
				else{
				
					cout<<"The given grammar is not LL(1) grammar"<<endl;
					return;
				}
			}
		}
	}
	cout << "Parse Table :-\n";
	cout << " \t\t";
	for(auto x:mpt)
	 cout << x.first << "\t\t";
		cout << '\n';
	for(auto x:nonmpt)
	{
		cout << x.first << "\t\t";
		for(auto y:mpt)
		{
			if(ParseTable[{x.first,y.first}]!="")
            {   
                cout << x.first<<"="<<ParseTable[{x.first,y.first}];
            }
			 
			 cout <<"\t\t";
		}
		cout << '\n';
	}
		cout<<"The given grammar is LL(1) grammar\n";
	
}

int main(){

		map<char, vector<char> > firstSet, followSet;
		unordered_map<char, int> nullSet;
		map<char, vector<string> > cfg, purecfg;

		string inpfile;
		
		cout<<"Original Grammar : "<<endl;
		
		int index = 0;
		while(getline(cin, inpfile)){

		    cout<<inpfile<<endl;
		    cfg[inpfile[0]] = vector<string>();
		    cfg[inpfile[0]] = vector<string>();
		 	firstSet[inpfile[0]] = vector<char>();
		 	followSet[inpfile[0]] = vector<char>();
		 	
		 	if(index++ == 0)
		 		followSet[inpfile[0]].push_back('$');
		 		
		    nonmpt[inpfile[0]] = 1;
		    stringstream check(inpfile.substr(2));
				
			string nxt; 
	  
			while(getline(check, nxt, '/')){       	    
		    	purecfg[inpfile[0]].push_back(nxt);
		        if(nxt.compare("~") == 0){
		            nullSet[inpfile[0]] = 1;
		            firstSet[inpfile[0]].push_back('~');
		            continue;
		        }
		        cfg[inpfile[0]].push_back(nxt);

		    
		    }
		     

		}

		cout<<"---------------------------"<<endl;
		findmpt(cfg);
		
		findnullProductions(firstSet, nullSet, cfg);
		while(true){
		
			if(findFirst(firstSet, cfg, nullSet)){
			
				break;
			}
			
		}
		
		cout<<"First Set :- "<<endl;
		prettyPrint(firstSet);
		
		cout<<"----------------------------------\n";

		while(true){
		
			if(findFollow(followSet, firstSet, cfg, nullSet)){
			
				break;
			}

		}
		
		cout<<"Follow Set :- "<<endl;
		prettyPrint(followSet);
		
		cout<<"----------------------------------\n";
		
		createLLTable(firstSet, followSet, nullSet, purecfg);
		
		return 0;
		
}