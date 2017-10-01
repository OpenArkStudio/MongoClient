// -------------------------------------------------------------------------
//    @FileName         :
//    @Author           :
//    @Date             :
//    @Module           :
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef AFC_MONGO_CONFIG_H
#define AFC_MONGO_CONFIG_H

#include "AFIMongoConfig.h"


class AFCMongoConfig : public AFIMongoConfig
{
public:
    AFCMongoConfig()
    {

    }

    virtual bool IsLoadFinish();

    virtual const std::map<std::string, ConnectInfo>& GetConfigDB();

    virtual const std::string& GetHostIPPort(const std::string& strDBName);
    virtual const std::string& GetUserName(const std::string& strDBName);
    virtual const std::string& GetPassword(const std::string& strDBName);
    virtual const std::string& GetAuthMechanism(const std::string& strDBName);
    virtual const std::string GetMongoConnectionUri(const std::string& strDBName);
    virtual const int GetClientType(const std::string& strDBName);
    virtual const int64_t GetWriteTimeOut(const std::string& strDBName);

//private:
    virtual bool LoadConfig();
private:
    std::map<std::string, ConnectInfo> m_mapDBConfig;   //dbName,config
    int m_bLoadFinish;
};

#endif // AFC_MONGO_CONFIG_H