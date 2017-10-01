
#include <iostream>
//#include "AFIMongoModule.h"
#include "AFCMongoModule.h"

int main(int, char**)
{
    AFCMongoModule::Instance()->Init();
    const std::string strCollection = "testCol";
    std::list<std::pair<std::string, AFIMongoModule::value_type>> listFieldValue;
    //insert
    listFieldValue.push_back(std::make_pair("name", "syz3"));
    listFieldValue.push_back(std::make_pair("age", (int64_t)19));
    if(!AFCMongoModule::Instance()->Insert(strCollection, listFieldValue))
    {
        std::cout << "insert failed" << std::endl;
    }
    else
    {
        std::cout << "insert success" << std::endl;
    }

    //set
    listFieldValue.clear();
    listFieldValue.push_back(std::make_pair("age", (int64_t)77));
    if(!AFCMongoModule::Instance()->InsertSet(strCollection, std::make_pair("name", "syz2"), listFieldValue))
    {
        std::cout << "InsertSet failed" << std::endl;
    }
    else
    {
        std::cout << "InsertSet success" << std::endl;
    }
    //delete
    if(!AFCMongoModule::Instance()->Delete(strCollection, std::make_pair("age", 66), true))
    {
        std::cout << "delete failed" << std::endl;
    }
    else
    {
        std::cout << "delete success" << std::endl;
    }
    //find
    std::list<std::string> listField;
    listField.push_back("age");
    std::list<AFIMongoModule::value_type> listValue;
    if(!AFCMongoModule::Instance()->Find(strCollection, std::make_pair("name", "syz2"), listField, listValue))
    {
        std::cout << "Find failed" << std::endl;
    }
    else
    {
        std::cout << "Find success" << std::endl;
        for(auto &it : listValue)
        {
            std::cout << boost::get<int64_t>(it) << std::endl;
        }
    }


    while(true)
    {

    }
    return 0;
}