
#include "AFCMongoDriver.h"
#include <iostream>
/*
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/pool.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/instance.hpp>
*/
#include "mongocxx/exception/exception.hpp"
#include "mongocxx/exception/query_exception.hpp"
#include "mongocxx/exception/bulk_write_exception.hpp"
#include "mongocxx/exception/logic_error.hpp"
#include "mongocxx/exception/write_exception.hpp"

#include "AFIMongoConfig.h"

#define  MONGOCATCH(function, line)     catch(mongocxx::query_exception er)\
    {\
        std::cout<< "Mongo query Error:"<< er.what() << " Function:" << function << " Line:" << line << std::endl;\
        return false;\
    }\
    catch(mongocxx::bulk_write_exception er)\
    {\
        std::cout<< "Mongo bulk_write Error:"<< er.what() << " Function:" << function << " Line:" << line << std::endl;\
        return false;\
    }\
    catch(mongocxx::logic_error er)\
    {\
        std::cout<< "Mongo logic Error:"<< er.what() << " Function:" << function << " Line:" << line << std::endl;\
        return false;\
    }\
    catch(mongocxx::write_exception er)\
    {\
        std::cout<< "Mongo write Error:"<< er.what() << " Function:" << function << " Line:" << line << std::endl;\
        return false;\
    }\
    catch(mongocxx::operation_exception er)\
    {\
        bEnable = false;\
        std::cout<< "Mongo operation Error:"<< er.what() << " Function:" << function << " Line:" << line << std::endl;\
        return false;\
    }\
    catch(mongocxx::exception er)\
    {\
        bEnable = false;\
        std::cout<< "Mongo Error:"<< er.what() << " Function:" << function << " Line:" << line << std::endl;\
        return false;\
    }\
    catch (...)\
    {\
        std::cout<< "Mongo Error: Unknow Exception" << " Function:" << function << " Line:" << line << std::endl;\
        return false;\
    }

bool AFCMongoDriver::Connect(const std::string& strDBName, const std::string& strUri)
{
    if(bEnable == true)
    {
        return true;
    }
    try
    {
        m_strDBName = strDBName;
        m_xMongoPool = new mongocxx::pool{ mongocxx::uri(strUri) };
    }
    MONGOCATCH(__FUNCTION__, __LINE__);

    bEnable = true;
    return true;
}
//
bool AFCMongoDriver::Aggregate(const std::string& strCollection, const mongocxx::pipeline& xPipeline, const mongocxx::options::aggregate& xOptions, std::list<bsoncxx::document::value>& listRusult)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        auto xCursor = xClient->database(m_strDBName)[strCollection].aggregate(xPipeline, xOptions);
        for(auto && doc : xCursor)
        {
            listRusult.push_back(bsoncxx::document::value(doc));
        }
    }
    MONGOCATCH(__FUNCTION__, __LINE__);

    return true;
}
bool AFCMongoDriver::Count(const std::string& strCollection, int64_t &nCount, const bsoncxx::document::view_or_value& xFilter, const mongocxx::options::count& xOptions)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        nCount = xClient->database(m_strDBName)[strCollection].count(xFilter, xOptions);
    }
    MONGOCATCH(__FUNCTION__, __LINE__);

    return true;
}
/*
bool AFCMongoDriver::CreateIndex(const std::string& strCollection, const bsoncxx::document::view_or_value& xKeys, const mongocxx::options::index& xOptions, bsoncxx::document::value& xResult)
{

}
*/
bool AFCMongoDriver::Distinct(const std::string& strCollection, const bsoncxx::string::view_or_value& xDistinctField, const bsoncxx::document::view_or_value& xFilter, const  mongocxx::options::distinct& xOpts, std::list<bsoncxx::document::value>& listRusult)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        auto xCursor = xClient->database(m_strDBName)[strCollection].distinct(xDistinctField, xFilter, xOpts);
        for(auto && doc : xCursor)
        {
            listRusult.push_back(bsoncxx::document::value(doc));
        }
    }
    MONGOCATCH(__FUNCTION__, __LINE__);

    return true;
}
void AFCMongoDriver::Drop(const std::string& strCollection)
{
    if(!bEnable)
    {
        return;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        xClient->database(m_strDBName)[strCollection].drop();
    }
    catch(mongocxx::operation_exception er)
    {
        std::cout << "Mongo Operation Error:" << er.what() << " Function:" << __FUNCTION__ << " Line:" << __LINE__ << std::endl;
        return;
    }
    return;
}

bool AFCMongoDriver::BulkWrite(const std::string& strCollection, const mongocxx::bulk_write& xBulkWrite)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        mongocxx::stdx::optional<mongocxx::result::bulk_write>xResult = xClient->database(m_strDBName)[strCollection].bulk_write(xBulkWrite);
        if(!xResult)
        {
            std::cout << "Mongo bulk_write failed, Function:" << __FUNCTION__ << " Line:" << __LINE__ << std::endl;
            return false;
        }
    }
    MONGOCATCH(__FUNCTION__, __LINE__);

    return true;
}

bool AFCMongoDriver::DeleteMany(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter, const mongocxx::options::delete_options& xOptions)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        mongocxx::stdx::optional<mongocxx::result::delete_result> xResult = xClient->database(m_strDBName)[strCollection].delete_many(xFilter, xOptions);
        if(!xResult)
        {
            std::cout << "Mongo delete_many failed, Function:" << __FUNCTION__ << " Line:" << __LINE__ << std::endl;
            return false;
        }
    }
    MONGOCATCH(__FUNCTION__, __LINE__);
    return true;
}
bool AFCMongoDriver::DeleteOne(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter, const mongocxx::options::delete_options& xOptions)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        mongocxx::stdx::optional<mongocxx::result::delete_result> xResult = xClient->database(m_strDBName)[strCollection].delete_one(xFilter, xOptions);
        if(!xResult)
        {
            std::cout << "Mongo delete_one failed, Function:" << __FUNCTION__ << " Line:" << __LINE__ << std::endl;
            return false;
        }
    }
    MONGOCATCH(__FUNCTION__, __LINE__);
    return true;
}

bool AFCMongoDriver::Query(const std::string& strCollection, const bsoncxx::builder::stream::document& xFilter, const mongocxx::options::find& xOpts, std::list<bsoncxx::document::value>& listRusult)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        auto xCursor = xClient->database(m_strDBName)[strCollection].find(xFilter.view(), xOpts);
        for(auto && doc : xCursor)
        {
            listRusult.push_back(bsoncxx::document::value(doc));
        }
    }
    MONGOCATCH(__FUNCTION__, __LINE__);
    return true;
}

bool AFCMongoDriver::FindOneAndDelete(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                                      const mongocxx::options::find_one_and_delete& xOptions, bsoncxx::document::value& xResult)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        mongocxx::stdx::optional<bsoncxx::document::value> xValue = xClient->database(m_strDBName)[strCollection].find_one_and_delete(xFilter, xOptions);
        if(xValue)
        {
            xResult = xValue.get();
        }
    }
    MONGOCATCH(__FUNCTION__, __LINE__);
    return true;
}

bool AFCMongoDriver::FindOneAndReplace(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                                       const bsoncxx::document::view_or_value& xReplacement, const mongocxx::options::find_one_and_replace& xOptions, bsoncxx::document::value& xResult)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        mongocxx::stdx::optional<bsoncxx::document::value> xValue = xClient->database(m_strDBName)[strCollection].find_one_and_replace(xFilter, xReplacement, xOptions);
        if(xValue)
        {
            xResult = xValue.get();
        }
    }
    MONGOCATCH(__FUNCTION__, __LINE__);
    return true;
}

bool AFCMongoDriver::FindOneAndUpdate(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                                      const bsoncxx::document::view_or_value& xUpdate, const mongocxx::options::find_one_and_update& xOptions, bsoncxx::document::value& xResult)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        mongocxx::stdx::optional<bsoncxx::document::value> xValue = xClient->database(m_strDBName)[strCollection].find_one_and_update(xFilter, xUpdate, xOptions);
        if(xValue)
        {
            xResult = xValue.get();
        }
    }
    MONGOCATCH(__FUNCTION__, __LINE__);
    return true;
}

bool AFCMongoDriver::InsertOne(const std::string& strCollection, const bsoncxx::document::view_or_value& xDocument,
                               const mongocxx::options::insert& xOptions)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        mongocxx::stdx::optional<mongocxx::result::insert_one> xValue = xClient->database(m_strDBName)[strCollection].insert_one(xDocument, xOptions);
        if(!xValue)
        {
            std::cout << "insert one failed, Function:" << __FUNCTION__ << " Line:" << __LINE__ << std::endl;
            return false;
        }
    }
    MONGOCATCH(__FUNCTION__, __LINE__);
    return true;
}

bool AFCMongoDriver::InsertMany(const std::string& strCollection, const std::list<bsoncxx::document::view_or_value> xDocuments, const mongocxx::options::insert& xOptions)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        mongocxx::stdx::optional<mongocxx::result::insert_many> xValue = xClient->database(m_strDBName)[strCollection].insert_many(xDocuments, xOptions);
        if(!xValue)
        {
            std::cout << "insert_many failed, Function:" << __FUNCTION__ << " Line:" << __LINE__ << std::endl;
            return false;
        }
    }
    MONGOCATCH(__FUNCTION__, __LINE__);
    return true;
}

bool AFCMongoDriver::ReplaceOne(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                                const bsoncxx::document::view_or_value& xReplacement, const mongocxx::options::update& xOptions)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        mongocxx::stdx::optional<mongocxx::result::replace_one> xValue = xClient->database(m_strDBName)[strCollection].replace_one(xFilter, xReplacement, xOptions);
        if(!xValue)
        {
            std::cout << "replace one failed, Function:" << __FUNCTION__ << " Line:" << __LINE__ << std::endl;
            return false;
        }
    }
    MONGOCATCH(__FUNCTION__, __LINE__);
    return true;
}

bool AFCMongoDriver::UpdateMany(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                                const bsoncxx::document::view_or_value& xUpdate, const mongocxx::options::update& xOptions)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        xClient->database(m_strDBName)[strCollection].update_many(xFilter, xUpdate, xOptions);
    }
    MONGOCATCH(__FUNCTION__, __LINE__);
    return true;
}

bool AFCMongoDriver::UpdateOne(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                               const bsoncxx::document::view_or_value& xUpdate, const mongocxx::options::update& xOptions)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        xClient->database(m_strDBName)[strCollection].update_one(xFilter, xUpdate, xOptions);
    }
    MONGOCATCH(__FUNCTION__, __LINE__);
    return true;
}
/*
bool AFCMongoDriver::ListIndexes(mongocxx::cursor& xResult)
{

}
*/
bool AFCMongoDriver::RenameCollection(const std::string& strCollection, const bsoncxx::string::view_or_value& xNewName, bool bDropTargetBeforeRename/* = false*/)
{
    if(!bEnable)
    {
        return false;
    }
    try
    {
        auto xClient = m_xMongoPool->acquire();
        xClient->database(m_strDBName)[strCollection].rename(xNewName, bDropTargetBeforeRename);
    }
    MONGOCATCH(__FUNCTION__, __LINE__);
    return true;
}
bool AFCMongoDriver::SetReadPreference(const std::string& strCollection, const mongocxx::read_preference& xRP)
{
    if(!bEnable)
    {
        return false;
    }

    auto xClient = m_xMongoPool->acquire();
    xClient->database(m_strDBName)[strCollection].read_preference(xRP);
    return true;
}
bool AFCMongoDriver::SetReadConcern(const std::string& strCollection, const mongocxx::read_concern& xRP)
{
    if(!bEnable)
    {
        return false;
    }

    auto xClient = m_xMongoPool->acquire();
    xClient->database(m_strDBName)[strCollection].read_concern(xRP);
    return true;
}
bool AFCMongoDriver::SetWriteConcern(const std::string& strCollection, const mongocxx::write_concern& xWC)
{
    if(!bEnable)
    {
        return false;
    }

    auto xClient = m_xMongoPool->acquire();
    xClient->database(m_strDBName)[strCollection].write_concern(xWC);
    return true;
}

mongocxx::pool::entry AFCMongoDriver::GetClient()
{
    return m_xMongoPool->acquire();
}