#include<mysql/mysql.h>
#include<iostream>
using namespace std;


int main()
{

    MYSQL *mysql = mysql_init(NULL);
    if(mysql == nullptr) {
        cout<<"mysql init error"<<endl; 
    }
    else {
        cout<<"mysql init success "<<endl; 
    }
    if(!mysql_real_connect(mysql,"49.235.90.199","root","wqwq123","cashier",3306,NULL,0)) {
        cout<<mysql_error(mysql)<<endl;
        cout<<"connect failed"<<endl;
    }
    else {
        cout<<"connect success"<<endl;
    }
    mysql_set_character_set(mysql, "utf8");
    if(mysql_query(mysql,"insert into Employee values(1,'张三','nan','1997-01-02','12332','售货员','15700000001',8200);")) {
        cout<<mysql_error(mysql)<<endl;
        cout<<"add data failed"<<endl;
    }
    else {
        cout<<"add data success"<<endl;
    }



    return 0;

}

