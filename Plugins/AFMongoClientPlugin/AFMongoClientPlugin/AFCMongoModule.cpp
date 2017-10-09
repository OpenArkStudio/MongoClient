#include "AFCMongoModule.h"
#include "AFCMongoDriver.h"
#include <iostream>
#include "bsoncxx/types.hpp"

std::unique_ptr<mongocxx::instance> AFCMongoModule::m_pMongoInstance = nullptr;

AFCMongoModule::AFCMongoModule(AFIPluginManager* p)
{
    pPluginManager = p;

    if(!m_pMongoInstance.get())
    {
        m_pMongoInstance = std::make_unique<mongocxx::instance>();
    }

    m_pMongoDriver = new AFCMongoDriver();
}

AFCMongoModule::~AFCMongoModule()
{
    delete(m_pMongoDriver);
}

bool AFCMongoModule::Init()
{
    return true;
}

bool AFCMongoModule::Shut()
{
    return true;
}

bool AFCMongoModule::Execute()
{
    return true;
}

bool AFCMongoModule::AfterInit()
{
    return true;
}

AFIMongoDriver* AFCMongoModule::GetMongoDriver()
{
    return m_pMongoDriver;
}

bool AFCMongoModule::AddMongoDBInfo(const std::string& strDBName, const std::string& strIpPort, const std::string& strUserName, const std::string&strPassword)
{
    if(m_pMongoDriver->Connect(strDBName, strIpPort, strUserName, strPassword))
    {
        const std::string strURl = m_pMongoDriver->GetMongoUrl();
        std::cout << "connect uri: " << strURl << " succes " << std::endl;
        return true;
    }
    else
    {
        const std::string strURl = m_pMongoDriver->GetMongoUrl();
        std::cout << "connect uri: " << strURl << " fail" << std::endl;
        return false;
    }

    return true;
}

bool AFCMongoModule::FindOne(const std::string& strCollection, const std::pair<std::string, AFCData>& xKeyValue, std::map<std::string, AFCData>& xFiledValue)
{
    if(xFiledValue.size() == 0)
    {
        std::cout << "field size = 0" << std::endl;
        return true;
    }

    mongocxx::options::find xOpts{};
    const AFCData xValue = xKeyValue.second;
    bsoncxx::builder::stream::document xOptsDocument;
    bsoncxx::builder::stream::document xFilter;

    if(!FillValueToDoc(xFilter, xKeyValue.first, xValue))
    {
        std::cout << "FillValueToDoc error" << std::endl;
        return false;
    }

    bool bHaveID = false;
    for(auto &it : xFiledValue)
    {
        xOptsDocument << it.first << 1;
    }

    if(xFiledValue.find("_id") == xFiledValue.end())
    {
        xOptsDocument << "_id" << 0;
    }

    xOpts.projection(xOptsDocument.view());
    //opts.max_time();  超时时间 毫秒，默认1s钟

    std::list<bsoncxx::document::value> listRusult;
    if(!m_pMongoDriver->Query(strCollection, xFilter, xOpts, listRusult))
    {
        std::cout << "mongo Query error" << std::endl;
        return false;
    }

    for(auto &it : listRusult)
    {
        std::cout << bsoncxx::to_json(it.view()) << std::endl;
        for(auto &itField : xFiledValue)
        {
            if(!GetValueFromBsonDoc(it.view(), itField.first, itField.second))
            {
                std::cout << "GetValueFromBsonDoc error" << std::endl;
                return false;
            }
        }

        //peek one
        break;
    }

    return true;
}

bool AFCMongoModule::Insert(const std::string& strCollection, const std::list<std::pair<std::string, AFCData>>& listFieldValue)
{
    bsoncxx::builder::stream::document xDocument{};
    for(auto &it : listFieldValue)
    {
        if(!FillValueToDoc(xDocument, it.first, it.second))
        {
            std::cout << "FillValueToDoc error" << std::endl;
            return false;
        }
    }
    return m_pMongoDriver->InsertOne(strCollection, xDocument.view(), mongocxx::options::insert());
    //return true;
}

bool AFCMongoModule::UnInsertSet(const std::string& strCollection, const std::pair<std::string, AFCData>& xKeyValue, const std::list<std::pair<std::string, AFCData>>& listFieldValue)
{
    bsoncxx::builder::stream::document xFilter;
    if(!FillValueToDoc(xFilter, xKeyValue.first, xKeyValue.second))
    {
        std::cout << "FillValueToDoc error" << std::endl;
        return false;
    }

    bsoncxx::builder::stream::document xDocument;
    xDocument << "$set" << bsoncxx::builder::stream::open_document;
    for(auto &it : listFieldValue)
    {
        if(!FillValueToDoc(xDocument, it.first, it.second))
        {
            std::cout << "FillValueToDoc error" << std::endl;
            return false;
        }
    }
    xDocument << bsoncxx::builder::stream::close_document;

    mongocxx::options::update xOptions;
    xOptions.upsert(false);

    return m_pMongoDriver->UpdateOne(strCollection, xFilter.view(), xDocument.view(), xOptions);
}

bool AFCMongoModule::InsertSet(const std::string& strCollection, const std::pair<std::string, AFCData>& xKeyValue, const std::list<std::pair<std::string, AFCData>>& listFieldValue)
{
    bsoncxx::builder::stream::document xFilter;
    if(!FillValueToDoc(xFilter, xKeyValue.first, xKeyValue.second))
    {
        std::cout << "FillValueToDoc error" << std::endl;
        return false;
    }

    bsoncxx::builder::stream::document xDocument;
    xDocument << "$set" << bsoncxx::builder::stream::open_document;
    for(auto &it : listFieldValue)
    {
        if(!FillValueToDoc(xDocument, it.first, it.second))
        {
            std::cout << "FillValueToDoc error" << std::endl;
            return false;
        }
    }
    xDocument << bsoncxx::builder::stream::close_document;

    mongocxx::options::update xOptions;
    xOptions.upsert(true);

    return m_pMongoDriver->UpdateOne(strCollection, xFilter.view(), xDocument.view(), xOptions);
}

bool AFCMongoModule::Delete(const std::string& strCollection, const std::pair<std::string, AFCData>& xKeyValue, bool bDelOne/* = true*/)
{
    bsoncxx::builder::stream::document xFilter;
    if(!FillValueToDoc(xFilter, xKeyValue.first, xKeyValue.second))
    {
        std::cout << "FillValueToDoc error" << std::endl;
        return false;
    }
    mongocxx::options::delete_options xOptions;

    if(bDelOne)
    {
        if(!m_pMongoDriver->DeleteOne(strCollection, xFilter.view(), xOptions))
        {
            std::cout << "delete one failed" << std::endl;
            return false;
        }
    }
    else
    {
        if(!m_pMongoDriver->DeleteMany(strCollection, xFilter.view(), xOptions))
        {
            std::cout << "delete many failed" << std::endl;
            return false;
        }
    }
    return true;
}

bool AFCMongoModule::GetValueFromBsonDoc(const bsoncxx::document::view& xDoc, const std::string& strField, AFIData& xValue)
{
    auto xDataType = xDoc[strField].type();
    switch(xDataType)
    {

    case bsoncxx::type::k_bool:
        xValue.SetBool(xDoc[strField].get_bool().value);
        break;
    case bsoncxx::type::k_timestamp:
        xValue.SetInt64((int64_t)(xDoc[strField].get_timestamp().timestamp));
        break;
    case bsoncxx::type::k_int64:
        {
            if(xValue.GetType() == DT_OBJECT)
            {
                xValue.SetObject(xDoc[strField].get_int64().value);
            }
            else
            {
                xValue.SetInt64(xDoc[strField].get_int64().value);
            }
        }
        break;
    case bsoncxx::type::k_int32:
        xValue.SetInt(xDoc[strField].get_int32().value);
        break;
    case bsoncxx::type::k_double:
        xValue.SetDouble(xDoc[strField].get_double().value);
        break;
    case bsoncxx::type::k_utf8:
        xValue.SetString(xDoc[strField].get_utf8().value.to_string().c_str());
        break;
    //case bsoncxx::type::k_binary:
    //    xDoc[strField].get_binary();
    //    break;
    default:
        std::cout << "unsupport value type :" << (int64_t)xDataType <<  std::endl;
        return false;
    }
    return true;
}

bool AFCMongoModule::FillValueToDoc(bsoncxx::builder::stream::document& xDoc, const std::string& strField, const AFIData& xValue)
{
    switch(xValue.GetType())
    {
    case AF_DATA_TYPE::DT_BOOLEAN:
        {
            xDoc << strField << xValue.GetBool();
        }
        break;
    case AF_DATA_TYPE::DT_INT:
        {
            xDoc << strField << (int32_t)xValue.GetInt();
        }
        break;
    case AF_DATA_TYPE::DT_INT64:
        {
            xDoc << strField << xValue.GetInt64();
        }
        break;
    case AF_DATA_TYPE::DT_FLOAT:
        {
            xDoc << strField << xValue.GetFloat();
        }
        break;
    case AF_DATA_TYPE::DT_DOUBLE:
        {
            xDoc << strField << xValue.GetDouble();
        }
        break;
    case AF_DATA_TYPE::DT_STRING:
        {
            xDoc << strField << xValue.GetString();
        }
        break;
    case AF_DATA_TYPE::DT_OBJECT:
        {
            xDoc << strField << (int64_t)xValue.GetObject().n64Value;
        }
        break;
    case AF_DATA_TYPE::DT_POINTER:
    case AF_DATA_TYPE::DT_USERDATA:
    case AF_DATA_TYPE::DT_TABLE:
        {
            std::cout << "Cannot save key value type " << strField  << std::endl;
            return false;
        }
    default:
        {
            std::cout << "Unknow key value type " << strField << std::endl;
            return false;
        }
        break;
    }
    return true;
}