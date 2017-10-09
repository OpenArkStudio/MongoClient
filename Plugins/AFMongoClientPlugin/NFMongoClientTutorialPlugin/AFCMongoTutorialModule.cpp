#include "AFCMongoTutorialModule.h"
#include "SDK/Interface/AFIPluginManager.h"


AFCMongoTutorialModule::AFCMongoTutorialModule(AFIPluginManager* p)
{
    pPluginManager = p;
}

AFCMongoTutorialModule::~AFCMongoTutorialModule()
{
}

bool AFCMongoTutorialModule::Init()
{
    m_pMongoModule = pPluginManager->FindModule<AFIMongoModule>();
    return true;
}


bool AFCMongoTutorialModule::Execute()
{
    return true;
}

bool AFCMongoTutorialModule::AfterInit()
{
    m_pMongoModule->AddMongoDBInfo("ta", "localhost:27017", "", "");
    const std::string strCollection = "testCol";
    std::list<std::pair<std::string, AFCData>> listFieldValue;

    //insert
    listFieldValue.push_back(std::make_pair("name", AFCData(ArkFrame::DT_STRING, "syz3")));
    listFieldValue.push_back(std::make_pair("age", AFCData(ArkFrame::DT_INT64, (int64_t)19)));
    if(!m_pMongoModule->Insert(strCollection, listFieldValue))
    {
        std::cout << "insert failed" << std::endl;
    }
    else
    {
        std::cout << "insert success" << std::endl;
    }

    //set
    listFieldValue.clear();
    listFieldValue.push_back(std::make_pair("age", AFCData(ArkFrame::DT_INT64, (int64_t)77)));
    if(!m_pMongoModule->InsertSet(strCollection, std::make_pair("name", AFCData(ArkFrame::DT_STRING, "syz2")), listFieldValue))
    {
        std::cout << "InsertSet failed" << std::endl;
    }
    else
    {
        std::cout << "InsertSet success" << std::endl;
    }
    //delete
    if(!m_pMongoModule->Delete(strCollection, std::make_pair("age", AFCData(ArkFrame::DT_INT64, (int64_t)66)), true))
    {
        std::cout << "delete failed" << std::endl;
    }
    else
    {
        std::cout << "delete success" << std::endl;
    }
    //find
    std::map< std::string, AFCData > xFindlistFieldValue;
    xFindlistFieldValue["age"] = AFCData(ArkFrame::DT_INT64, 0);

    if(!m_pMongoModule->FindOne(strCollection, std::make_pair("name", AFCData(ArkFrame::DT_STRING, "syz2")), xFindlistFieldValue))
    {
        std::cout << "Find failed" << std::endl;
    }
    else
    {
        std::cout << "Find success" << std::endl;
        for(auto &it : xFindlistFieldValue)
        {
            std::cout << it.second.GetInt64() << std::endl;
        }
    }

    return true;
}


bool AFCMongoTutorialModule::Shut()
{
    return true;
}
