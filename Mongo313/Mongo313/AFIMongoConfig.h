// -------------------------------------------------------------------------
//    @FileName         :
//    @Author           :
//    @Date             :
//    @Module           :
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef AFI_MONGO_CONFIG_H
#define AFI_MONGO_CONFIG_H

#include <string>
#include <map>

static std::string NULL_STR;
static std::string TEST_DB_NAME = "ta";

class AFIMongoConfig
{
public:
    enum ClientType
    {
        EClientType_Single       = 0,
        EClientType_Pool         = 1,
    };

    struct ConnectInfo
    {
        bool bLoadFinish;
        int nMinPoolSize;
        int nMaxPoolSize;
        int nClientType;
        int64_t nWriteTimeOut;

        std::string strIPPort;
        std::string strUserName;
        std::string strPassword;
        std::string strDBName;
        std::string strAuthMechanism;
    };

public:
    virtual const std::map<std::string, ConnectInfo>& GetConfigDB() = 0;

    virtual const std::string& GetHostIPPort(const std::string& strDBName) = 0;
    virtual const std::string& GetUserName(const std::string& strDBName) = 0;
    virtual const std::string& GetPassword(const std::string& strDBName) = 0;
    virtual const std::string& GetAuthMechanism(const std::string& strDBName) = 0;
    virtual const std::string GetMongoConnectionUri(const std::string& strDBName) = 0;
    virtual const int GetClientType(const std::string& strDBName) = 0;
    virtual const int64_t GetWriteTimeOut(const std::string& strDBName) = 0;
    virtual bool IsLoadFinish() = 0;

    virtual bool LoadConfig() = 0;
};

#endif // AFI_MONGO_CONFIG_H

