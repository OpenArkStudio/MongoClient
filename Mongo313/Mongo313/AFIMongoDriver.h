// -------------------------------------------------------------------------
//    @FileName         :
//    @Author           :
//    @Date             :
//    @Module           :
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef AFI_MONGO_DRIVER_H
#define AFI_MONGO_DRIVER_H

#include <string>
#include <memory>
#include <list>
/*
namespace bsoncxx
{
    namespace builder
    {
        namespace stream
        {
            class document;
        }
    }

    namespace string
    {
        class view_or_value;
    }

    namespace document
    {
        class view_or_value;
        class value;
    }
}
namespace mongocxx
{
    class cursor;
    class pipeline;
    class bulk_write;
    class write_concern;
    class read_concern;
    class read_preference;
    class client;
    namespace options
    {
        class find;
        class distinct;
        class aggregate;
        class count;
        class index;
        class delete_options;
        class update;
        class insert;
        class find_one_and_delete;
        class find_one_and_replace;
        class find_one_and_update;
    }
}
*/
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/pool.hpp>

class AFIMongoDriver
{
public:
    virtual mongocxx::pool::entry GetClient() = 0;
    virtual bool Connect(const std::string& strDBName, const std::string& strUri) = 0;
    //
    virtual bool Aggregate(const std::string& strCollection, const mongocxx::pipeline& xPipeline,
                           const mongocxx::options::aggregate& xOptions, std::list<bsoncxx::document::value>& listRusult) = 0;

    virtual bool Count(const std::string& strCollection, int64_t &nCount, const bsoncxx::document::view_or_value& xFilter, const mongocxx::options::count& xOptions) = 0;
    //virtual bool CreateIndex(const std::string& strCollection, const bsoncxx::document::view_or_value& xKeys, const mongocxx::options::index& xOptions, bsoncxx::document::value& xResult) = 0;

    virtual bool Distinct(const std::string& strCollection, const bsoncxx::string::view_or_value& xDistinctField,
                          const bsoncxx::document::view_or_value& xFilter, const  mongocxx::options::distinct& xOpts,
                          std::list<bsoncxx::document::value>& listRusult) = 0;

    virtual void Drop(const std::string& strCollection) = 0;

    virtual bool BulkWrite(const std::string& strCollection, const mongocxx::bulk_write& xBulkWrite) = 0;

    virtual bool DeleteMany(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter, const mongocxx::options::delete_options& xOptions) = 0;
    virtual bool DeleteOne(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter, const mongocxx::options::delete_options& xOptions) = 0;

    virtual bool Query(const std::string& strCollection, const bsoncxx::builder::stream::document& xFilter, const mongocxx::options::find& xOpts, std::list<bsoncxx::document::value>& listRusult) = 0;

    virtual bool FindOneAndDelete(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                                  const mongocxx::options::find_one_and_delete& xOptions, bsoncxx::document::value& xResult) = 0;

    virtual bool FindOneAndReplace(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                                   const bsoncxx::document::view_or_value& xReplacement, const mongocxx::options::find_one_and_replace& xOptions, bsoncxx::document::value& xResult) = 0;

    virtual bool FindOneAndUpdate(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                                  const bsoncxx::document::view_or_value& xUpdate, const mongocxx::options::find_one_and_update& xOptions, bsoncxx::document::value& xResult) = 0;

    virtual bool InsertOne(const std::string& strCollection, const bsoncxx::document::view_or_value& xDocument,
                           const mongocxx::options::insert& xOptions) = 0;

    virtual bool InsertMany(const std::string& strCollection, const std::list<bsoncxx::document::view_or_value> xDocuments, const mongocxx::options::insert& xOptions) = 0;

    virtual bool ReplaceOne(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                            const bsoncxx::document::view_or_value& xReplacement, const mongocxx::options::update& xOptions) = 0;

    virtual bool UpdateMany(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                            const bsoncxx::document::view_or_value& xUpdate, const mongocxx::options::update& xOptions) = 0;

    virtual bool UpdateOne(const std::string& strCollection, const bsoncxx::document::view_or_value& xFilter,
                           const bsoncxx::document::view_or_value& xUpdate, const mongocxx::options::update& xOptions) = 0;

    //virtual bool ListIndexes(mongocxx::cursor& xResult) = 0;
    virtual bool RenameCollection(const std::string& strCollection, const bsoncxx::string::view_or_value& xNewName, bool bDropTargetBeforeRename = false) = 0;
    virtual bool SetReadPreference(const std::string& strCollection, const mongocxx::read_preference& xRP) = 0;
    virtual bool SetReadConcern(const std::string& strCollection, const mongocxx::read_concern& xRP) = 0;
    virtual bool SetWriteConcern(const std::string& strCollection, const mongocxx::write_concern& xWC) = 0;
};

#endif // AFI_MONGO_DRIVER_H
