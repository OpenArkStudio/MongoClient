// -------------------------------------------------------------------------
//    @FileName         :
//    @Author           :
//    @Date             :
//    @Module           :
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef AFC_MONGO_TUTORIAL_MODULE_H
#define AFC_MONGO_TUTORIAL_MODULE_H
#include "AFMongoClientPlugin/AFInterface/AFIMongoTutorialModule.h"
#include "AFMongoClientPlugin/AFInterface/AFIMongoModule.h"
class AFCMongoTutorialModule : public AFIMongoTutorialModule
{
public:
public:
    AFCMongoTutorialModule(AFIPluginManager* p);
    virtual ~AFCMongoTutorialModule();
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

private:
    AFIMongoModule * m_pMongoModule;
};

#endif // AFC_MONGO_MODULE_H