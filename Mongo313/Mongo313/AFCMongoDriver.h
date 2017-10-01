// -------------------------------------------------------------------------
//    @FileName         :
//    @Author           :
//    @Date             :
//    @Module           :
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef AFC_MONGO_DRIVER_H
#define AFC_MONGO_DRIVER_H

#include "AFIMongoDriver.h"

/*
namespace mongocxx
{
    class pool;
}
*/
class AFCMongoDriver: public AFIMongoDriver
{
public:
    AFCMongoDriver()
    {
        bEnable = false;
    }

    virtual mongocxx::pool::entry GetClient();
    virtual bool Connect(const std::string& strDBName, const std::string& strUri);
    //
    virtual bool Aggregate(const std::string& strCollection, const mongocxx::pipeline& xPipeline, const mongocxx::options::aggregate& xOptions, std::list<bsoncxx::document::value>& listRusult);
    virtual bool Count(const std::string& strCollection, int64_t &nCount, const bsoncxx::document::view_or_value& xFilter, const mongocxx::options::count& xOptions);
    //virtual bool CreateIndex(const std::string& strCollection, const bsoncxx::document::view_or_value& xKeys, const mongocxx::options::index& xOptions, bsoncxx::document::value& xResult);
    virtual bool Distinct(const std::string& strCollection, const bsoncxx::string::view_or_value& xDistinctField, const bsoncxx::document::view_or_value& xFilter, const  mongocxx::options::distinct& xOpts, std::list<bsoncxx::document::value>& listRusult);
    virtual void Drop(const std::string& strCollection);

    virtual bool BulkWrite(const std::string& strCollection, const mongocxx::bulk_write& xBulkWrite);

    virtual bool DeleteMany(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter, const mongocxx::options::delete_options& xOptions);
    virtual bool DeleteOne(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter, const mongocxx::options::delete_options& xOptions);

    virtual bool Query(const std::string& strCollection, const bsoncxx::builder::stream::document& xFilter, const mongocxx::options::find& xOpts, std::list<bsoncxx::document::value>& listRusult);

    virtual bool FindOneAndDelete(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                                  const mongocxx::options::find_one_and_delete& xOptions, bsoncxx::document::value& xResult);

    virtual bool FindOneAndReplace(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                                   const bsoncxx::document::view_or_value& xReplacement, const mongocxx::options::find_one_and_replace& xOptions, bsoncxx::document::value& xResult);

    virtual bool FindOneAndUpdate(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                                  const bsoncxx::document::view_or_value& xUpdate, const mongocxx::options::find_one_and_update& xOptions, bsoncxx::document::value& xResult);

    virtual bool InsertOne(const std::string& strCollection, const bsoncxx::document::view_or_value& xDocument,
                           const mongocxx::options::insert& xOptions);
    virtual bool InsertMany(const std::string& strCollection, const std::list<bsoncxx::document::view_or_value> xDocuments, const mongocxx::options::insert& xOptions);
    virtual bool ReplaceOne(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                            const bsoncxx::document::view_or_value& xReplacement, const mongocxx::options::update& xOptions);

    virtual bool UpdateMany(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                            const bsoncxx::document::view_or_value& xUpdate, const mongocxx::options::update& xOptions);

    virtual bool UpdateOne(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                           const bsoncxx::document::view_or_value& xUpdate, const mongocxx::options::update& xOptions);

    //virtual bool ListIndexes(mongocxx::cursor& xResult);
    virtual bool RenameCollection(const std::string& strCollection, const bsoncxx::string::view_or_value& xNewName, bool bDropTargetBeforeRename = false);
    virtual bool SetReadPreference(const std::string& strCollection, const mongocxx::read_preference& xRP);
    virtual bool SetReadConcern(const std::string& strCollection, const mongocxx::read_concern& xRP);
    virtual bool SetWriteConcern(const std::string& strCollection, const mongocxx::write_concern& xWC);
private:
    //mongocxx::pool::entry GetClient();
    //static mongocxx::instance m_xMongoInstance;
private:
    bool bEnable;
    std::string m_strDBName;
    mongocxx::pool* m_xMongoPool;
};

#endif // AFC_MONGO_DRIVER_H
