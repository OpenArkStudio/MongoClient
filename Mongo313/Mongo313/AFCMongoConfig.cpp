
#include <strstream>
#include "AFCMongoConfig.h"
#include "boost/lexical_cast.hpp"

bool AFCMongoConfig::IsLoadFinish()
{
    return m_bLoadFinish;
}

const std::map<std::string, AFIMongoConfig::ConnectInfo>& AFCMongoConfig::GetConfigDB()
{
    return m_mapDBConfig;
}

const std::string& AFCMongoConfig::GetHostIPPort(const std::string& strDBName)
{
    auto it = m_mapDBConfig.find(strDBName);
    if(it == m_mapDBConfig.end())
    {
        return NULL_STR;
    }
    return it->second.strIPPort;
}
const std::string& AFCMongoConfig::GetUserName(const std::string& strDBName)
{
    auto it = m_mapDBConfig.find(strDBName);
    if(it == m_mapDBConfig.end())
    {
        return NULL_STR;
    }
    return it->second.strUserName;
}
const std::string& AFCMongoConfig::GetPassword(const std::string& strDBName)
{
    auto it = m_mapDBConfig.find(strDBName);
    if(it == m_mapDBConfig.end())
    {
        return NULL_STR;
    }
    return it->second.strPassword;
}

//ÔÝÊ±Ã»ÓÃ
const std::string& AFCMongoConfig::GetAuthMechanism(const std::string& strDBName)
{
    auto it = m_mapDBConfig.find(strDBName);
    if(it == m_mapDBConfig.end())
    {
        return NULL_STR;
    }
    return it->second.strAuthMechanism;
}

const std::string AFCMongoConfig::GetMongoConnectionUri(const std::string& strDBName)
{
    auto it = m_mapDBConfig.find(strDBName);
    if(it == m_mapDBConfig.end())
    {
        return NULL_STR;
    }

    std::string strUri;

    if(it->second.strIPPort.empty())
    {
        return NULL_STR;
    }

    strUri += "mongodb://";
    if(!it->second.strUserName.empty() && !it->second.strPassword.empty())
    {
        strUri = strUri + it->second.strUserName + ":" + it->second.strPassword + "@";
    }

    strUri += it->second.strIPPort;

    bool bFirstOption = true;

    if(!it->second.strUserName.empty() && !it->second.strPassword.empty())
    {
        strUri = strUri + "/?authSource=" + it->second.strDBName;
        bFirstOption = false;
    }

    if(it->second.nClientType == AFIMongoConfig::EClientType_Pool)
    {
        strUri = strUri + (bFirstOption ? "/?" : "&") + "minPoolSize=" + boost::lexical_cast<std::string>(it->second.nMinPoolSize) + "&maxPoolSize=" + boost::lexical_cast<std::string>(it->second.nMaxPoolSize);
    }

    return strUri;
}

const int AFCMongoConfig::GetClientType(const std::string& strDBName)
{
    auto it = m_mapDBConfig.find(strDBName);
    if(it == m_mapDBConfig.end())
    {
        return -1;
    }
    return it->second.nClientType;
}

const int64_t AFCMongoConfig::GetWriteTimeOut(const std::string& strDBName)
{
    auto it = m_mapDBConfig.find(strDBName);
    if(it == m_mapDBConfig.end())
    {
        return -1;
    }
    return it->second.nWriteTimeOut;
}


bool AFCMongoConfig::LoadConfig()
{
    ConnectInfo xConfigInfo;
    xConfigInfo.strIPPort = "localhost:27017";
    xConfigInfo.strUserName = "syz";
    xConfigInfo.strPassword = "123";
    xConfigInfo.strDBName = TEST_DB_NAME;
    xConfigInfo.strAuthMechanism = NULL_STR;
    xConfigInfo.nMinPoolSize = 1;
    xConfigInfo.nMaxPoolSize = 4;
    xConfigInfo.nClientType = AFIMongoConfig::EClientType_Pool;

    m_mapDBConfig.insert(std::make_pair(TEST_DB_NAME, xConfigInfo));

    m_bLoadFinish = true;

    return true;
}