#include "Application.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream

using namespace miapplication;

ApplicationResult miapplication::ApplicationBase::Configure(std::string path)
{
    rapidjson::Document d;
    std::ifstream inFile;
    std::stringstream strStream;
    inFile.open(path.c_str()); //open the input file
    if (!inFile.good())
    {
        inFile.close();
        return ApplicationResult::ErrorConfigFile;
    }
    strStream << inFile.rdbuf(); //read the file
    std::string str = strStream.str(); //str holds the content of the file
    inFile.close();

    d.Parse(str.c_str());
    if (!d.HasMember("miapplicationdescription"))
    {
        return ApplicationResult::ErrorConfig;
    }
    const rapidjson::Value& mimoduldescription = d["miapplicationdescription"];

    if (!mimoduldescription.HasMember("name"))
    {
        return ApplicationResult::ErrorConfig;
    }
    const rapidjson::Value& name = mimoduldescription["name"];
    if (std::string(name.GetString()) != _Name)
    {
        return ApplicationResult::ErrorConfigNotMatch;
    }

    if (!mimoduldescription.HasMember("realtimetimer1priority"))
    {
        return ApplicationResult::ErrorConfig;
    }
    _Configuration.RealTimeTimer1Priority = mimoduldescription["realtimetimer1priority"].GetInt();;

    if (!mimoduldescription.HasMember("realtimetimer1scheduling"))
    {
        return ApplicationResult::ErrorConfig;
    }
    const rapidjson::Value& realtimetimer1scheduling = mimoduldescription["realtimetimer1scheduling"];
    _Configuration.RealTimeTimer1Scheduling = getScheduler(realtimetimer1scheduling.GetString());
    if (_Configuration.RealTimeTimer1Scheduling == miutils::Schedulers::None)
    {
        return ApplicationResult::ErrorConfig;
    }
   
    if (!mimoduldescription.HasMember("realtimetimer1intervall"))
    {
        return ApplicationResult::ErrorConfig;
    }
    const rapidjson::Value& realtimetimer1intervall = mimoduldescription["realtimetimer1intervall"];
    _Configuration.RealTimeTimer1Intervall = realtimetimer1intervall.GetInt();

    if (!mimoduldescription.HasMember("realtimetimer2priority"))
    {
        return ApplicationResult::ErrorConfig;
    }
    _Configuration.RealTimeTimer2Priority = mimoduldescription["realtimetimer1priority"].GetInt();


    if (!mimoduldescription.HasMember("realtimetimer2scheduling"))
    {
        return ApplicationResult::ErrorConfig;
    }
    const rapidjson::Value& realtimetimer2scheduling = mimoduldescription["realtimetimer1scheduling"];
    _Configuration.RealTimeTimer2Scheduling = getScheduler(realtimetimer2scheduling.GetString());
    if (_Configuration.RealTimeTimer2Scheduling == miutils::Schedulers::None)
    {
        return ApplicationResult::ErrorConfig;
    }
    
    if (!mimoduldescription.HasMember("realtimetimer2intervall"))
    {
        return ApplicationResult::ErrorConfig;
    }
    const rapidjson::Value& realtimetimer2intervall = mimoduldescription["realtimetimer2intervall"];
    _Configuration.RealTimeTimer2Intervall = realtimetimer2intervall.GetInt();

    if (!mimoduldescription.HasMember("idletimerpriority"))
    {
        return ApplicationResult::ErrorConfig;
    }
    _Configuration.IdleTimerPriority = mimoduldescription["idletimerpriority"].GetInt();;

    if (!mimoduldescription.HasMember("idletimerscheduling"))
    {
        return ApplicationResult::ErrorConfig;
    }
    const rapidjson::Value& idletimerscheduling = mimoduldescription["idletimerscheduling"];
    _Configuration.IdleTimerScheduling = getScheduler(idletimerscheduling.GetString());
    if (_Configuration.IdleTimerScheduling == miutils::Schedulers::None)
    {
        return ApplicationResult::ErrorConfig;
    }

    if (!mimoduldescription.HasMember("idletimerintervall"))
    {
        return ApplicationResult::ErrorConfig;
    }
    const rapidjson::Value& idletimerintervall = mimoduldescription["idletimerintervall"];
    _Configuration.IdleTimerIntervall = idletimerintervall.GetInt();
    return ApplicationResult();
}

miutils::Schedulers miapplication::ApplicationBase::getScheduler(const std::string& str)
{
    if (str == std::string("other"))
    {
        return miutils::Schedulers::Other;
    }
    if (str == std::string("fifo"))
    {
        return miutils::Schedulers::Fifo;
    }
    if (str == std::string("roundrobin"))
    {
        return miutils::Schedulers::RoundRobin;
    }
    return miutils::Schedulers::Other;
}

miapplication::ApplicationBase::ApplicationBase(const std::string name,const miIOManager::IOManager& ioManager, std::string configurationPath)
    :_Name(name)
    ,_RealTimeTimer1("RT1",this)
    ,_RealTimeTimer2("RT2", this)
    ,_IdleTimer1("IDLE", this)
    ,_IoManager(ioManager)
    , _Configuration()
    , _ConfigurationPath(configurationPath)
{
    _State = Configure(_ConfigurationPath + std::string("/") + _Name + std::string(".json"));
}

miapplication::ApplicationBase::ApplicationBase(const std::string name, const miIOManager::IOManager& ioManager, ApplicationConfiguration _Configuration)
    :_Name(name)
    , _RealTimeTimer1("RT1", this)
    , _RealTimeTimer2("RT2", this)
    , _IdleTimer1("IDLE", this)
    , _IoManager(ioManager)
    , _Configuration(_Configuration)
    , _ConfigurationPath("")
{
}

ApplicationResult miapplication::ApplicationBase::Start()
{
    if (_RealTimeTimer1.Start(_Configuration.RealTimeTimer1Intervall, this, _Configuration.RealTimeTimer1Priority, _Configuration.RealTimeTimer1Scheduling) != miutils::TimerResults::ErrorOk)
    {
        return miapplication::ApplicationResult::ErrorStart;
    }
    if (_RealTimeTimer2.Start(_Configuration.RealTimeTimer2Intervall, this, _Configuration.RealTimeTimer2Priority, _Configuration.RealTimeTimer2Scheduling) != miutils::TimerResults::ErrorOk)
    {
        return miapplication::ApplicationResult::ErrorStart;
    }
    if (_IdleTimer1.Start(_Configuration.IdleTimerIntervall, this, _Configuration.IdleTimerPriority, _Configuration.IdleTimerScheduling) != miutils::TimerResults::ErrorOk)
    {
        return miapplication::ApplicationResult::ErrorStart;
    }
    return ApplicationResult();
}

ApplicationResult miapplication::ApplicationBase::Stop()
{
    if (_RealTimeTimer1.Stop())
    {
        return miapplication::ApplicationResult::ErrorStart;
    }
    if (_RealTimeTimer2.Stop())
    {
        return miapplication::ApplicationResult::ErrorStart;
    }
    if (_IdleTimer1.Stop())
    {
        return miapplication::ApplicationResult::ErrorStart;
    }
    return ApplicationResult();
}

void miapplication::ApplicationBase::eventOccured(void* sender, const std::string& name)
{
    miutils::Timer* timer = reinterpret_cast<miutils::Timer*>(sender);
    ApplicationBase* app = nullptr;
    if (timer == nullptr)
    {
        _State = ApplicationResult::ErrorTask;
        return;
    }
    app = reinterpret_cast<ApplicationBase*>(timer->GetObject());
    if (app == nullptr)
    {
        _State = ApplicationResult::ErrorTask;
        return;
    }

    if (name == "RT1")
    {
        app->RealtimeTask1(app->_IoManager.InputImage(), app->_IoManager.OutputImage());
    }
    if (name == "RT2")
    {
        app->RealtimeTask2(app->_IoManager.InputImage(), app->_IoManager.OutputImage());
    }
    if (name == "IDLE")
    {
        app->IdleTask(app->_IoManager.InputImage(), app->_IoManager.OutputImage());
    }
    
}
