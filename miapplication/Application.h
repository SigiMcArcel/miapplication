#pragma once
#include <string>
#include <map>
#include <mi/miio/IOImage.h>
#include <mi/miio/IOManager.h>
#include <mi/miutils/Event.h>
#include <mi/miutils/Timer.h>

namespace miapplication
{
	typedef enum ApplicationResult_e
	{
		Ok,
		ErrorConfig,
		ErrorConfigFile,
		ErrorConfigNotMatch,
		ErrorStart,
		ErrorTask
	}ApplicationResult;

	typedef struct ApplicationConfiguration_t
	{
		std::string Name;
		int32_t RealTimeTimer1Priority;
		miutils::Schedulers RealTimeTimer1Scheduling;
		int32_t RealTimeTimer1Intervall;

		int32_t RealTimeTimer2Priority;
		miutils::Schedulers RealTimeTimer2Scheduling;
		int32_t RealTimeTimer2Intervall;

		int32_t IdleTimerPriority;
		miutils::Schedulers IdleTimerScheduling;
		int32_t IdleTimerIntervall;

	}ApplicationConfiguration;

	class ApplicationBase : public miutils::EventListener
	{
	private:
		std::string _Name;
		miutils::Timer _RealTimeTimer1;
		miutils::Timer _RealTimeTimer2;
		miutils::Timer _IdleTimer1;
		miIOManager::IOManager _IoManager;
		ApplicationConfiguration _Configuration;
		std::string _ConfigurationPath;
		ApplicationResult _State;

		ApplicationResult Configure(std::string path);
		miutils::Schedulers getScheduler(const std::string& str);

	public:
		ApplicationBase(const std::string name, const miIOManager::IOManager& ioManager, std::string configurationPath);
		ApplicationBase(const std::string name, const miIOManager::IOManager& ioManager, ApplicationConfiguration _Configuration);
		ApplicationResult Start();
		ApplicationResult Stop();

		virtual ApplicationResult RealtimeTask1(const miIOImage::IOImage& inputImage, const miIOImage::IOImage& outputImage) = 0;
		virtual ApplicationResult RealtimeTask2(const miIOImage::IOImage& inputImage, const miIOImage::IOImage& outputImage) = 0;
		virtual ApplicationResult IdleTask(const miIOImage::IOImage& inputImage, const miIOImage::IOImage& outputImage) = 0;

		// Geerbt über EventListener
		virtual void eventOccured(void* sender, const std::string& name) override;
	};
}
