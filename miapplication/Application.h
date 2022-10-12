#pragma once
#include <mi/miio/IOImage.h>
#include <mi/miutils/Event.h>
#include <mi/miutils/Timer.h>

namespace miapplication
{
	typedef enum ApplicationResult_e
	{
		Ok,
		Error
	}ApplicationResult;

	class ApplicationTask
	{
	public:
		virtual ApplicationResult Task(const miIOImage::IOImage& inputImage, const miIOImage::IOImage& outputImage) = 0;
	};
	class Application : public miutils::EventListener
	{
	private:
	public:
		ApplicationResult AddTask(const ApplicationTask& task,miutils::Schedulers scheduler,int32_t priority);
		ApplicationResult Start();
		ApplicationResult Stop();
	};
}
