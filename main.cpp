#include<bits/stdc+.h>
#include<operations.h>
using namespace std;
int main()
{

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
