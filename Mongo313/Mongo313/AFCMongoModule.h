// -------------------------------------------------------------------------
//    @FileName         :
//    @Author           :
//    @Date             :
//    @Module           :
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef AFC_MONGO_MODULE_H
#define AFC_MONGO_MODULE_H

#include "AFIMongoModule.h"
#include "AFIMongoConfig.h"
#include "MySingleton.h"
#include <memory>
#include "mongocxx/instance.hpp"

class AFCMongoModule : public AFIMongoModule, public MySingleton<AFCMongoModule>
{
public:
    AFCMongoModule();
    virtual ~AFCMongoModule();
    virtual bool Init();
    virtual AFIMongoDriver* GetMongoDriver();
    virtual bool Find(const std::string& strCollection, const std::pair<std::string, value_type>& xKeyValue, const std::list<std::string>& listField, std::list<value_type>& listValue);
    virtual bool Insert(const std::string& strCollection, const std::list<std::pair<std::string, value_type>>& listFieldValue);
    virtual bool UnInsertSet(const std::string& strCollection, const std::pair<std::string, value_type>& xKeyValue, const std::list<std::pair<std::string, value_type>>& listFieldValue);
    virtual bool InsertSet(const std::string& strCollection, const std::pair<std::string, value_type>& xKeyValue, const std::list<std::pair<std::string, value_type>>& listFieldValue);
    virtual bool Delete(const std::string& strCollection, const std::pair<std::string, value_type>& xKeyValue, bool bDelOne = true);
private:
    bool GetValueFromBsonDoc(const bsoncxx::document::view& xDoc, const std::string& strField, value_type& xValue);
    bool FillValueToDoc(bsoncxx::builder::stream::document& xDoc, const std::string& strField, const value_type& xValue);

private:
    AFIMongoConfig* m_pMongoConfig;
    AFIMongoDriver* m_pMongoDriver;
private:
    static std::unique_ptr<mongocxx::instance> m_pMongoInstance;
};

#endif // AFC_MONGO_MODULE_H