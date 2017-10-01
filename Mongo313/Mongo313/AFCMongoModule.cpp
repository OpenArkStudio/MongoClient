#include "AFCMongoModule.h"
#include "AFCMongoConfig.h"
#include "AFCMongoDriver.h"
#include <iostream>
#include "bsoncxx/types.hpp"

std::unique_ptr<mongocxx::instance> AFCMongoModule::m_pMongoInstance = nullptr;

AFCMongoModule::AFCMongoModule()
{
    if(!m_pMongoInstance.get())
    {
        m_pMongoInstance = std::make_unique<mongocxx::instance>();
    }
    m_pMongoConfig = new AFCMongoConfig();
    m_pMongoDriver = new AFCMongoDriver();
}

AFCMongoModule::~AFCMongoModule()
{
    delete(m_pMongoDriver);
    delete(m_pMongoConfig);
}

bool AFCMongoModule::Init()
{
    if(!m_pMongoConfig->LoadConfig())
    {
        return false;
    }
    const std::string& strURI = m_pMongoConfig->GetMongoConnectionUri(TEST_DB_NAME);
    std::cout << "connect uri: " << strURI << std::endl;
    return m_pMongoDriver->Connect(TEST_DB_NAME, strURI);
}

AFIMongoDriver* AFCMongoModule::GetMongoDriver()
{
    return m_pMongoDriver;
}

bool AFCMongoModule::Find(const std::string& strCollection, const std::pair<std::string, value_type>& xKeyValue, const std::list<std::string>& listField, std::list<value_type>& listValue)
{
    if(listField.size() == 0)
    {
        std::cout << "field size = 0" << std::endl;
        return true;
    }

    mongocxx::options::find xOpts{};
    const value_type& xValue = xKeyValue.second;
    bsoncxx::builder::stream::document xOptsDocument;

    bsoncxx::builder::stream::document xFilter;

    if(!FillValueToDoc(xFilter, xKeyValue.first, xValue))
    {
        std::cout << "FillValueToDoc error" << std::endl;
        return false;
    }

    for(auto &it : listField)
    {
        xOptsDocument << it << 1;
    }
    xOptsDocument << "_id" << 0;

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
        for(auto &itField : listField)
        {
            value_type xValue;
            if(!GetValueFromBsonDoc(it.view(), itField, xValue))
            {
                std::cout << "GetValueFromBsonDoc error" << std::endl;
                return false;
            }
            listValue.push_back(xValue);
        }
    }

    return true;
}

bool AFCMongoModule::Insert(const std::string& strCollection, const std::list<std::pair<std::string, value_type>>& listFieldValue)
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

bool AFCMongoModule::UnInsertSet(const std::string& strCollection, const std::pair<std::string, value_type>& xKeyValue, const std::list<std::pair<std::string, value_type>>& listFieldValue)
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

bool AFCMongoModule::InsertSet(const std::string& strCollection, const std::pair<std::string, value_type>& xKeyValue, const std::list<std::pair<std::string, value_type>>& listFieldValue)
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

bool AFCMongoModule::Delete(const std::string& strCollection, const std::pair<std::string, value_type>& xKeyValue, bool bDelOne/* = true*/)
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

bool AFCMongoModule::GetValueFromBsonDoc(const bsoncxx::document::view& xDoc, const std::string& strField, value_type& xValue)
{
    auto xDataType = xDoc[strField].type();
    switch(xDataType)
    {

    //case bsoncxx::type::k_bool:
    //    xValue = xDoc[strField].get_bool().value;
    //    break;
    //case bsoncxx::type::k_timestamp:
    //    xValue = (int64_t)(xDoc[strField].get_timestamp().timestamp);
    //    break;
    case bsoncxx::type::k_int64:
        xValue = xDoc[strField].get_int64().value;
        break;
    case bsoncxx::type::k_int32:
        xValue = (int64_t)xDoc[strField].get_int32().value;
        break;
    case bsoncxx::type::k_double:
        xValue = xDoc[strField].get_double().value;
        break;
    case bsoncxx::type::k_utf8:
        xValue = xDoc[strField].get_utf8().value.to_string();
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

bool AFCMongoModule::FillValueToDoc(bsoncxx::builder::stream::document& xDoc, const std::string& strField, const value_type& xValue)
{
    if(xValue.type() == typeid(int64_t))
    {
        xDoc << strField << boost::get<int64_t>(xValue);
    }
    else if(xValue.type() == typeid(double))
    {
        xDoc << strField << boost::get<double>(xValue);
    }
    else if(xValue.type() == typeid(std::string))
    {
        xDoc << strField << boost::get<std::string>(xValue);
    }
    else if(xValue.type() == typeid(int))
    {
        xDoc << strField << boost::get<int>(xValue);
    }
    //else if(xValue.type() == typeid(bool))
    //{
    //    xDoc << strField << boost::get<bool>(xValue);
    //}
    else
    {
        std::cout << "Unknow key value type" << std::endl;
        return false;
    }
    return true;
}