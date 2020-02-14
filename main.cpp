#include<bits/stdc+.h>
using namespace std;

string file_path = "./data/";

const char separator = ' ';
const int nameWidth = 22;
const int numWidth = 2;

template<typename T> void printElement(T t, const int& width){
	cout << left << setw(width) << setfill(separator) << t;
}

struct data_type{
	char type;
	int intu; //represent integer
	string stru; //represent string 
	data_type(char type, string s){
		this->type =  type;
		switch(type){
			case '1':
				this->intu = atoi(s.c_str());
				break;
			case '2':
				this->stru = s;
				break;
		}
	}
	string data(){
		switch(this->type){
			case '1':
				return to_string(this->intu);
			case '2':
				return this->stru
		}
	}
};

struct Table_row{
	vector <data_type> row;
};

struct Table{
	string table_name;
	vector <string> table_attr;
	vector <char. table_attr_type;
	vector <Table_row> table_row;
	
	int no_of_attribute(){
		return this-> table_attr.size();
	}

	int no_of_record(){
		return this-> table_row.size();
	}
};

map<string , Table> Table_all;

struct operators{
	string type;
	operators(string s){
		this->type = s;	
	}
	bool cal(data_type a, data_type b){
        if (this->type==">=" && a.type=='1' && b.type=='1' && a.intu>=b.intu) return true;
        else if (this->type==">" && a.type=='1' && b.type=='1' && a.intu>b.intu) return true;
        else if (this->type=="<=" && a.type=='1' && b.type=='1' && a.intu<=b.intu) return true;
        else if (this->type=="<" && a.type=='1' && b.type=='1' && a.intu<b.intu) return true;
        else if (this->type=="=" && a.type=='1' && b.type=='1' && a.intu==b.intu) return true;
        else if (this->type=="!=" && a.type=='1' && b.type=='1' && a.intu!=b.intu) return true;
        else if (this->type=="=" && a.type=='2' && b.type=='2' && a.stru==b.stru) return true;
        else if (this->type=="!=" && a.type=='2' && b.type=='2' && a.stru!=b.stru) return true;

        else return false;
    }
};

typedef pair< pair <string, string>, oper> cond;

int int_parser(string s){
	return atoi(s.c_str());
}

bool string_comparer(string a, string b){
	if(a==b) return true;
	else return false;
}

string space_remover(string s){
	string temp;
	isstringstream iss(s);
	do{
		string sub;
		iss >> sub;
		temp+= sub;
	} while(iss);
	return temp;
}

vector<string> comma_separator(string s){
	vector<string> result;
	if(s.size()==0)return result;
	stringstream ss(s);
	while(ss.good()){
		string substr;
		getline(ss, substr, ',');
		result.push_back(substr);	
	}
	return result;
}

bool is_func(char c){
	if(c=='P'||c=='S'||c=='U'||c=='R' ||c=='C' || c=='D')return true;
    else return false;	
}

bool is_valid_operator(string s){

    if(s=="="||s==">"||s==">="||s=="<="||s=="<"||s=="!=")return true;
    else return false;
}


void print_table(Table table){
	cout << table.table_name<<endl;
	for(int i=0;i<table.no_of_attribute();i++){
		printElement(table.table_attr[i], nameWidth);
	}
	cout << endl;
	
	for(int i=0; i<table.no_of_record();i++){
		for(int j=0; j<table.no_of_attribute();j++){
			if(table.table_row[i].row[j].type==1)printElement(table.table_row[i].row[j].data9), numWidth);
else printElement(table.table_row[i].row[j].data(), nameWidth);
		}
		cout << endl;
	}
}

void generate_error(int code){
	cout<<"error_code: "<<code<<" : ";
    switch(code){
        case 0: cout<<"Table not present\n";
            break;
        case 1: cout<<"Column name not present in table\n";
            break;
        case 2: cout<<"No of column does not match\n";
            break;
        case 3: cout<<"Condition specified not present in table\n";
            break;
        case 4: cout<<"Wrong condition operation\n";
            break;
        case 5: cout<<"Insert semicolon at the end of parse_query\n";
            break;
        case 6: cout<<"Syntax error in query\n";
            break;
        case 7: cout<<"Multiple columns with same name please perform Rename operation\n";
            break;
        case 8: cout<<"Relations are Union incompatible\n";
            break;
    }
    exit(0);

}

bool condition_checker(vector<cond> cons,Table_row tr,vector<string> table_attr){
    bool flag=true;
    int i;
    for(auto f : cons){
        i=0;
        for(i=0;i<table_attr.size();i++){
            if((f).first.first == table_attr[i]){
                int j;
                 //if value is another attribute of relation
                for(j=0;j<table_attr.size();j++){
                        if((f).first.second == table_attr[j]){
                            if(!(f).second.cal(tr.row[i],tr.row[j]) ){
                                flag=false;
                                return flag;
                            }
                            break;
                        }
                }
                //if value is constant
                if(j==table_attr.size()){
                    data_type dt = data_type(tr.row[i].type,(f).first.second);
                    if(!(f).second.cal(tr.row[i],dt) ){
                                flag=false;
                                return flag;
                    }
                }
                break;
            }
        }
 }
        if(i==table_attr.size()){ 
            error_generate(3);
        }
    }
    return true;
}

vector<cond> multiple_condition_handler(vector<string> attr_list,vector<char> attr_type,vector<string> s){
    vector<cond> cons;
    char attr_code = '1';

    for(int i=0;i<s.size();i++){
        int idx,flag=-1;
        for(idx=0;idx<s[i].size()-1;idx++){
            if(is_valid_opt(s[i].substr(idx,2))){
                flag=2;
                break;
            }
            if(is_valid_opt(s[i].substr(idx,1))){
                flag=1;
                break;
            }
        }
        if(flag==-1){           
            error_generate(4);
        }
        string attr = s[i].substr(0,idx);
        string op = s[i].substr(idx,flag);
        string val = s[i].substr(idx+flag,s[i].size()-idx-flag);
        if(flag!=-1){
            operators op1(op);
            cond en1 = make_pair(make_pair(attr,val),op1);
            cons.push_back(en1);
        }
    }
    return cons;
}

bool data_type_comparer(data_type a, data_type b){
    if(a.type!=b.type)return 0;
    if(a.type=='1')return a.intu==b.intu;
    if(a.type=='2')return a.stru==b.stru;
}



