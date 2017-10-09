// -------------------------------------------------------------------------
//    @FileName         :
//    @Author           :
//    @Date             :
//    @Module           :
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef AFC_MONGO_MODULE_H
#define AFC_MONGO_MODULE_H

#include "AFMongoClientPlugin/AFInterface/AFIMongoModule.h"
#include <memory>
#include "mongocxx/instance.hpp"
#include "AFIMongoDriver.h"

class AFCMongoModule : public AFIMongoModule
{
public:
public:
    AFCMongoModule(AFIPluginManager* p);
    virtual ~AFCMongoModule();
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

    virtual AFIMongoDriver* GetMongoDriver();
    virtual bool AddMongoDBInfo(const std::string& strDBName, const std::string& strIpPort, const std::string& strUserName, const std::string&strPassword);
    virtual bool FindOne(const std::string& strCollection, const std::pair<std::string, AFCData>& xKeyValue, std::map<std::string, AFCData>& xFiledValue);
    virtual bool Insert(const std::string& strCollection, const std::list<std::pair<std::string, AFCData>>& listFieldValue);
    virtual bool UnInsertSet(const std::string& strCollection, const std::pair<std::string, AFCData>& xKeyValue, const std::list<std::pair<std::string, AFCData>>& listFieldValue);
    virtual bool InsertSet(const std::string& strCollection, const std::pair<std::string, AFCData>& xKeyValue, const std::list<std::pair<std::string, AFCData>>& listFieldValue);
    virtual bool Delete(const std::string& strCollection, const std::pair<std::string, AFCData>& xKeyValue, bool bDelOne = true);

private:
    bool GetValueFromBsonDoc(const bsoncxx::document::view& xDoc, const std::string& strField, AFIData& xValue);
    bool FillValueToDoc(bsoncxx::builder::stream::document& xDoc, const std::string& strField, const AFIData& xValue);

private:
    AFIMongoDriver* m_pMongoDriver;
private:
    static std::unique_ptr<mongocxx::instance> m_pMongoInstance;
};

#endif // AFC_MONGO_MODULE_H