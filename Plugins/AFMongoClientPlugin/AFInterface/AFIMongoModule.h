// -------------------------------------------------------------------------
//    @FileName         :
//    @Author           :
//    @Date             :
//    @Module           :
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef AFI_MONGO_MODULE_H
#define AFI_MONGO_MODULE_H

#include "SDK/Interface/AFIModule.h"
class AFIMongoDriver;
class AFIMongoModule : public AFIModule
{
public:
    virtual bool Init() = 0;
    virtual AFIMongoDriver* GetMongoDriver() = 0;
    virtual bool AddMongoDBInfo(const std::string& strDBName, const std::string& strIpPort, const std::string& strUserName, const std::string&strPassword) = 0;
    virtual bool FindOne(const std::string& strCollection, const std::pair<std::string, AFCData>& xKeyValue, std::map<std::string, AFCData>& xFiledValue) = 0;
    virtual bool Insert(const std::string& strCollection, const std::list<std::pair<std::string, AFCData>>& listFieldValue) = 0;
    virtual bool UnInsertSet(const std::string& strCollection, const std::pair<std::string, AFCData>& xKeyValue, const std::list<std::pair<std::string, AFCData> >& listFieldValue) = 0;
    virtual bool InsertSet(const std::string& strCollection, const std::pair<std::string, AFCData>& xKeyValue, const std::list<std::pair<std::string, AFCData> >& listFieldValue) = 0;
    virtual bool Delete(const std::string& strCollection, const std::pair<std::string, AFCData>& xKeyValue, bool bDelOne = true) = 0;
};

#endif // AFI_MONGO_MODULE_H
