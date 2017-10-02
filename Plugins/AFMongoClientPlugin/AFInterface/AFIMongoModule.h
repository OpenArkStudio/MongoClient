// -------------------------------------------------------------------------
//    @FileName         :
//    @Author           :
//    @Date             :
//    @Module           :
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef AFI_MONGO_MODULE_H
#define AFI_MONGO_MODULE_H

#include "boost/variant.hpp"
#include "SDK/Interface/AFIModule.h"
class AFIMongoDriver;
class AFIMongoModule : public AFIModule
{
public:
    typedef boost::variant<int64_t, double, std::string, int> value_type;

public:
    virtual bool Init() = 0;
    virtual AFIMongoDriver* GetMongoDriver() = 0;
    virtual bool AddMongoDBInfo(const std::string& strDBName, const std::string& strIpPort, const std::string& strUserName, const std::string&strPassword) = 0;
    virtual bool Find(const std::string& strCollection, const std::pair<std::string, value_type>& xKeyValue, const std::list<std::string>& listField, std::list<value_type>& listValue) = 0;
    virtual bool Insert(const std::string& strCollection, const std::list<std::pair<std::string, value_type>>& listFieldValue) = 0;
    virtual bool UnInsertSet(const std::string& strCollection, const std::pair<std::string, value_type>& xKeyValue, const std::list<std::pair<std::string, value_type>>& listFieldValue) = 0;
    virtual bool InsertSet(const std::string& strCollection, const std::pair<std::string, value_type>& xKeyValue, const std::list<std::pair<std::string, value_type>>& listFieldValue) = 0;
    virtual bool Delete(const std::string& strCollection, const std::pair<std::string, value_type>& xKeyValue, bool bDelOne = true) = 0;
};

#endif // AFI_MONGO_MODULE_H
