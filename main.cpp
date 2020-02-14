#include<bits/stdc++.h>
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
				return this->stru;
		}
	}
};

struct Table_row{
	vector <data_type> row;
};

struct Table{
	string table_name;
	vector <string> table_attr;
	vector <char> table_attr_type;
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

typedef pair< pair <string, string>, operators> cond;

int int_parser(string s){
	return atoi(s.c_str());
}

bool string_comparer(string a, string b){
	if(a==b) return true;
	else return false;
}

string space_remover(string s){
	string temp;
	istringstream iss(s);
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
			 if(table.table_row[i].row[j].type==1)printElement(table.table_row[i].row[j].data(),numWidth);
            else printElement(table.table_row[i].row[j].data(),nameWidth);
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

bool condition_checker(vector<cond> cons, Table_row tr, vector<string> table_attr){
    bool flag=true;
    int i;
    for(auto f : cons){
        i=0;
        for(i=0;i<table_attr.size();i++){
            if((f).first.first == table_attr[i]){
                int j;
                for(j=0;j<table_attr.size();j++){
                        if((f).first.second == table_attr[j]){
                            if(!(f).second.cal(tr.row[i],tr.row[j]) ){
                                flag=false;
                                return flag;
                            }
                            break;
                        }
                }
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
		if(i==table_attr.size()){ 
            generate_error(3);
        }
    }
	return true;
}

vector<cond> multiple_condition_handler(vector<string> attr_list, vector<char> attr_type, vector<string> s){
    vector<cond> cons;
    char attr_code = '1';

    for(int i=0;i<s.size();i++){
        int idx,flag=-1;
        for(idx=0;idx<s[i].size()-1;idx++){
            if(is_valid_operator(s[i].substr(idx,2))){
                flag=2;
                break;
            }
            if(is_valid_operator(s[i].substr(idx,1))){
                flag=1;
                break;
            }
        }
        if(flag==-1){           
            generate_error(4);
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

Table create_table(string table_name)
{

    Table new_table;
    string file_path1 = file_path+table_name+"_desc.txt"; 
    string file_path2 = file_path+table_name+"_data.txt"; 

    ifstream infile(file_path1);
    if(infile.fail()){
        generate_error(0);
    }
    int cnt=0;
    while(infile)
    {
        string s;
        if(!getline(infile,s)) break;

        istringstream ss( s );
        while(ss){
            string s;
            if(!getline(ss,s,'\t')) break;
            if(cnt==0){
                new_table.table_name=s;

            }
            else if(cnt==1){
               new_table.table_attr.push_back(s);

            }
            else if(cnt==2){
                new_table.table_attr_type.push_back(s[0]);
            }
         }
	 cnt++;
        }
	ifstream infile2(file_path2);
    if(infile2.fail()){
        generate_error(0);
    }
    while(infile2)
    {
        string s;
        if(!getline(infile2,s)) break;

        istringstream ss( s );
        Table_row record;
        cnt=0;
        while(ss){
            string s;
            if(!getline(ss,s,'\t')) break;

            data_type dt(new_table.table_attr_type[cnt],s);
            record.row.push_back(dt);
            cnt++;
        }

        new_table.table_row.push_back(record);

    }

    Table_all[table_name] = new_table;
    return new_table;

}

Table load_table(string rel_name){

    if( Table_all.find( rel_name ) != Table_all.end()){
        return Table_all[rel_name];
    }
    Table_all[rel_name] = create_table(rel_name);
    return Table_all[rel_name];
}

Table query_parser(string );


Table Project(Table table1,vector<string> sets_attr){

    Table res_table;
    vector<bool> attribute_checker(table1.no_of_attribute(),false); 
    for(int i=0;i<sets_attr.size();i++){
        int j=0;
        for(;j<table1.no_of_attribute();j++){
            if(string_comparer(sets_attr[i],table1.table_attr[j])){
                attribute_checker[j]=true;
                break;
            }
        }
        if(j==table1.no_of_attribute()){
            generate_error(1);
        }
    }
    for(int i=0;i<table1.no_of_record();i++){
        Table_row record;
        for(int j=0;j<table1.no_of_attribute();j++){
            if(attribute_checker[j]){                      
                record.row.push_back(table1.table_row[i].row[j]); 
            }
        }
        res_table.table_row.push_back(record); 
    }
	 for(int j=0;j<table1.no_of_attribute();j++){  
            if(attribute_checker[j]){
                res_table.table_attr.push_back(table1.table_attr[j]);
                res_table.table_attr_type.push_back(table1.table_attr_type[j]);
            }
        }

    res_table.table_name = "";
    return res_table;
}


Table Select(Table table1,vector<cond> cons){
    Table new_table;

    for(int i=0;i<table1.no_of_record();i++){
        if(condition_checker(cons,table1.table_row[i],table1.table_attr )){ 
            new_table.table_row.push_back(table1.table_row[i]);
        }
    }
    for(int j=0;j<table1.no_of_attribute();j++){       
        new_table.table_attr.push_back(table1.table_attr[j]);
        new_table.table_attr_type.push_back(table1.table_attr_type[j]);
    }

    new_table.table_name = "";
    return new_table;
}

Table Rename(Table table1,vector<string> sets_attr,string new_name){

    Table new_table;
    bool flag=1;
    if(sets_attr.size()==0)flag=0;
    if(flag && sets_attr.size()!=table1.table_attr.size()){
        generate_error(2);
    }
    for(int i=0;i<table1.no_of_record();i++){
        Table_row record;
        for(int j=0;j<table1.no_of_attribute();j++){
            if(i==0){
                if(flag){                      
                    new_table.table_attr.push_back(sets_attr[j]);
                    new_table.table_attr_type.push_back(table1.table_attr_type[j]);
                }
                else{                          
                    new_table.table_attr.push_back(table1.table_attr[j]);
                    new_table.table_attr_type.push_back(table1.table_attr_type[j]);
                }
            }
            record.row.push_back(table1.table_row[i].row[j]);

        }
        new_table.table_row.push_back(record);  
    }

    new_table.table_name = new_name;
    return new_table;
}


Table Union(Table table1, Table table2){

    if(table1.no_of_attribute()!=table2.no_of_attribute()){
        generate_error(1);
    }

    vector<bool> attribute_checker(false,table1.no_of_attribute()); 
    for(int i=0;i<table1.no_of_attribute();i++){

        if(!string_comparer(table1.table_attr[i],table2.table_attr[i]) || table1.table_attr_type[i]!=table2.table_attr_type[i]){
            generate_error(8);
        }
    }

    for(int i=0;i<table2.no_of_record();i++){  
        Table_row record;
        bool flag=1;
		for(int j=0;j<table1.no_of_record();j++){
            int cnt=0;
            for(int k=0;k<table2.no_of_attribute();k++){
                if(data_type_comparer(table2.table_row[i].row[k],table1.table_row[j].row[k])){
                    cnt++;
                }
            }
            if(cnt==table2.no_of_attribute()){ 
                flag=0;
                break;
            }
        }

        for(int j=0;j<table1.no_of_attribute();j++){   
            record.row.push_back(table2.table_row[i].row[j]);
        }

        if(flag)table1.table_row.push_back(record);

    }

    return table1;
}

Table CartesianProduct(Table table1, Table table2){

    Table new_table;

    vector<bool> attribute_checker(false,table1.no_of_attribute()); 
    for(int i=0;i<table1.no_of_record();i++){ 
        for(int j=0;j<table2.no_of_record();j++){
            Table_row record;
            for(int k=0;k<table1.no_of_attribute();k++){
                record.row.push_back(table1.table_row[i].row[k]);
            }
            for(int k=0;k<table2.no_of_attribute();k++){
                record.row.push_back(table2.table_row[j].row[k]);
            }

            new_table.table_row.push_back(record);
        }

    }
    for(int k=0;k<table1.no_of_attribute();k++){
        new_table.table_attr.push_back(table1.table_name+"."+table1.table_attr[k]);
        new_table.table_attr_type.push_back(table1.table_attr_type[k]);
    }

    for(int k=0;k<table2.no_of_attribute();k++){
        new_table.table_attr.push_back(table2.table_name+"."+table2.table_attr[k]);
        new_table.table_attr_type.push_back(table2.table_attr_type[k]);
	 }

    for(int k=0;k<table2.no_of_attribute();k++){
        new_table.table_attr.push_back(table2.table_name+"."+table2.table_attr[k]);
        new_table.table_attr_type.push_back(table2.table_attr_type[k]);
    }
    for(int i=0;i<new_table.no_of_attribute();i++)
        for(int j=0;j<new_table.no_of_attribute();j++)if(i!=j && string_comparer(new_table.table_attr[i],new_table.table_attr[j])){
            generate_error(7);
        }
    return new_table;
}

Table SetDifference(Table table1, Table table2){

    if(table1.no_of_attribute()!=table2.no_of_attribute()){
        generate_error(1);
    }
    Table new_table;

    vector<bool> attribute_checker(false,table1.no_of_attribute()); 
    for(int i=0;i<table1.no_of_attribute();i++){
        if(!string_comparer(table1.table_attr[i],table2.table_attr[i]) || table1.table_attr_type[i]!=table2.table_attr_type[i]){
            generate_error(8);
        }
    }
    for(int i=0;i<table1.no_of_record();i++){  
        Table_row record;
        bool flag=1;
        for(int j=0;j<table2.no_of_record();j++){
            int cnt=0;
            for(int k=0;k<table2.no_of_attribute();k++){
                if(data_type_comparer(table2.table_row[j].row[k],table1.table_row[i].row[k])){ 
                    cnt++;
                }
            }
			if(cnt==table2.no_of_attribute()){ 
                flag=0;
                break;
            }
        }

        record = table1.table_row[i];
        if(flag)new_table.table_row.push_back(record);

    }
    for(int k=0;k<table1.no_of_attribute();k++){
            new_table.table_attr.push_back(table1.table_attr[k]);
            new_table.table_attr_type.push_back(table1.table_attr_type[k]);
    }
    new_table.table_name="";
    return new_table;
}

Table project_query_parser(string s){
    Table table1;
    vector<string> attr_list;
    if(s[2]=='['){
        int i;
        for(i=3;s[i]!=';' && s[i]!=']';i++);
        if(s[i]==']'){
            string attr = s.substr(3,i-3);
            attr_list = comma_separator(attr); 
            if(s[i+1]==',' && (is_func(s[i+2]))&&(s[i+3]=='(')){ 
                string s2 = s.substr(i+2,s.size()-i-4)+';';
                Table res = query_parser(s2);  
                if(attr=="*"){       
                    attr_list = res.table_attr;
                }

                table1 = Project(res,attr_list); 
            }
			 else{
                string rel_name = s.substr(i+2,s.size()-i-4);
                Table table2 = load_table(rel_name);
                if(attr=="*"){
                    attr_list = table2.table_attr;
                }
                table1 = Project(table2,attr_list);  
             }
        }
        else{
            generate_error(6);
        }
    }
    else{
        generate_error(6);
    }
    return table1;
}

Table select_query_parser(string s){
    Table table1;
    vector<cond> cons;
    vector<string> cndn_list;
    if(s[2]=='['){
        int i;
        for(i=3;s[i]!=';' && s[i]!=']';i++);
        if(s[i]==']'){
            string attr = s.substr(3,i-3);
            if(attr=="*"){   
                string new_q = s;
                new_q[0]='P';
                return query_parser(new_q);
            }
            cndn_list = comma_separator(attr); 
            if(s[i+1]==',' && (is_func(s[i+2])) &&(s[i+3]=='(')){ 
                string s2 = s.substr(i+2,s.size()-i-4)+';';
                Table res = query_parser(s2);
                cons = multiple_condition_handler(res.table_attr,res.table_attr_type,cndn_list); 
                table1 = Select(res,cons); 
            }
			 else{
                string rel_name = s.substr(i+2,s.size()-i-4);
                Table table2 = load_table(rel_name); 
                cons = multiple_condition_handler(table2.table_attr,table2.table_attr_type,cndn_list); // 
                table1 = Select(table2,cons);  
             }
        }
        else{
            generate_error(6);
        }
    }
    else{
        generate_error(6);
    }
    return table1;
}

Table rename_query_parser(string s){
    Table table1;
    vector<string> attr_list;
    int i,i_old;

    for(i=3;s[i]!=';' && s[i]!=',';i++);
    string new_name = s.substr(2,i-2);
    if(s[i+1]=='[' || (is_func(s[i+1]) &&(s[i+2]=='(')) ){
        i_old=i;
        if(s[i+1]=='[')for(i=i+2;s[i]!=';' && s[i]!=']';i++);
        string attr;    
        if(i!=i_old){
            attr = s.substr(i_old+2,i-i_old-2);
            attr_list = comma_separator(attr);
         }
         else{
            i--;
         }

        if(s[i+1]==',' && (is_func(s[i+2])) &&(s[i+3]=='(')){ 
            string s2 = s.substr(i+2,s.size()-i-4)+';';
            Table res = query_parser(s2);
            table1 = Rename(res,attr_list,new_name); 
        }
		 else{
            string rel_name = s.substr(i+2,s.size()-i-4);

            Table table2 = load_table(rel_name);   

            table1 = Rename(table2,attr_list,new_name);  
         }
    }
    else{
        generate_error(6);
    }
    return table1;
}

Table union_query_parser(string s){
    Table table1;
    vector<string> attr_list;   
    if(is_func(s[2]) &&s[3]=='('){   
        int cnt=1;
        int idx=4;
        while(cnt!=0){         
            if(s[idx]=='(')cnt++;
            if(s[idx]==')')cnt--;
            idx++;
        }
        string s2 = s.substr(2,idx-2)+';';

        Table table1 = query_parser(s2);  
        Table table2;
        if(is_func(s[idx+1])&&s[idx+2]=='('){ 
            string s2 = s.substr(idx+1,s.size()-idx-3)+';';
            table2 = query_parser(s2);  
        }
        else{
            string rel_name = s.substr(idx+1,s.size()-idx-3);
            table2 = load_table(rel_name);  
        }
        Table res = Union(table1,table2); 
        return res;
    }
	 else{
        int idx=2;
        while(s[idx]!=','){
            idx++;
        }
        string rel_name = s.substr(2,idx-2);
        Table table1 = load_table(rel_name);  
        Table table2;

        if(is_func(s[idx+1])&&s[idx+2]=='('){  
            string s2 = s.substr(idx+1,s.size()-idx-3)+';';
            table2 = query_parser(s2); 
        }
        else{
            string rel_name = s.substr(idx+1,s.size()-idx-3);
            table2 = load_table(rel_name);  
        }
        Table res = Union(table1,table2);  
        return res;
    }

    return table1;
}

Table cartesianProduct_query_parser(string s){
    Table table1;
    vector<string> attr_list;   
    if(is_func(s[2]) &&s[3]=='('){   
        int cnt=1;
        int idx=4;
        while(cnt!=0){         
            if(s[idx]=='(')cnt++;
            if(s[idx]==')')cnt--;
            idx++;
        }
        string s2 = s.substr(2,idx-2)+';';
        Table table1 = query_parser(s2); 
        Table table2;
        if(is_func(s[idx+1])&&s[idx+2]=='('){  
            string s2 = s.substr(idx+1,s.size()-idx-3)+';';
            table2 = query_parser(s2); 
        }
        else{
            string rel_name = s.substr(idx+1,s.size()-idx-3);
            table2 = load_table(rel_name);  
        }
		Table res = CartesianProduct(table1,table2);  
        return res;
    }
    else{
        int idx=2;
        while(s[idx]!=','){
            idx++;
        }
        string rel_name = s.substr(2,idx-2);
        Table table1 = load_table(rel_name);   
        Table table2;
        if(is_func(s[idx+1])&&s[idx+2]=='('){  
            string s2 = s.substr(idx+1,s.size()-idx-3)+';';
            table2 = query_parser(s2); 
        }
        else{
            string rel_name = s.substr(idx+1,s.size()-idx-3);
            table2 = load_table(rel_name);  
        }
        Table res = CartesianProduct(table1,table2);  
        return res;
    }

    return table1;
}

Table setDifference_query_parser(string s){
    Table table1;
    vector<string> attr_list;  
    if(is_func(s[2]) &&s[3]=='('){   
        int cnt=1;
        int idx=4;
        while(cnt!=0){          
            if(s[idx]=='(')cnt++;
            if(s[idx]==')')cnt--;
            idx++;
        }

        string s2 = s.substr(2,idx-2)+';';
        Table table1 = query_parser(s2); 
        Table table2;
        if(is_func(s[idx+1])&&s[idx+2]=='('){  
            string s2 = s.substr(idx+1,s.size()-idx-3)+';';
            table2 = query_parser(s2);  
        }
        else{
            string rel_name = s.substr(idx+1,s.size()-idx-3);
            table2 = load_table(rel_name);  
        }	
		Table res = SetDifference(table1,table2); 
        return res;
    }
    return table1;
}

Table query_parser(string s){

    if(s[s.size()-1]!=';'){  
        generate_error(5);
    }

    if(s[0]=='P' && s[1]=='('){   // Project query
        return project_query_parser(s);
    }
    else if(s[0]=='S' && s[1]=='('){ // Select query
        return select_query_parser(s);
    }
    else if(s[0]=='R' && s[1]=='('){ // Rename query
        return rename_query_parser(s);
    }
    else if(s[0]=='U' && s[1]=='('){  // Union query
        return union_query_parser(s);
    }
    else if(s[0]=='C' && s[1]=='('){   // CartesianProduct query
        return cartesianProduct_query_parser(s);
    }
    else if(s[0]=='D' && s[1]=='('){   // SetDifference query
        return setDifference_query_parser(s);
    }
    else{
        generate_error(6);  // no such query exist
    }
}

int main(){
    string proj_parse_query;
    cout<<"Welcome to SQL..\n\n";
    cout<<">> ";
    while(getline(cin,proj_parse_query)){  
        string par_proj_parse_query = space_remover(proj_parse_query); 
        if(par_proj_parse_query=="exit"){
            cout<<"\n\nGood bye...\n";
            break;
        }
        else if(par_proj_parse_query!=""){
            Table res = query_parser(par_proj_parse_query);
            cout<<"\n";
            print_table(res);
        }
        cout<<"\n>> ";                    
    }
    return 0;
}
