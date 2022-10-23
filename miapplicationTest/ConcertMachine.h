#pragma once
#include <mi/miio/IOImage.h>
#include <mi/miio/IOManager.h>
#include <mi/miapplication/Application.h>

//inherit the apllication base
class ConcertMachine : public miapplication::ApplicationBase
{
public:
	
	//initalize the new application with a iomanager a name and a configuration path
	ConcertMachine(const miIOManager::IOManager& ioManager)
		:ApplicationBase("concertmachine", ioManager, "/etc/mi")
	{

	}
	/*
	//or with a configuration 
	ConcertMachine(const miIOManager::IOManager& ioManager,const miapplication::ApplicationConfiguration& conf)
		:ApplicationBase("concertmachine", ioManager, conf)
	{

	}
	*/

	//implement your Programm
	// Geerbt über ApplicationBase
	virtual miapplication::ApplicationResult RealtimeTask1(const miIOImage::IOImage& inputImage, const miIOImage::IOImage& outputImage) override;

	virtual miapplication::ApplicationResult RealtimeTask2(const miIOImage::IOImage& inputImage, const miIOImage::IOImage& outputImage) override;

	virtual miapplication::ApplicationResult IdleTask(const miIOImage::IOImage& inputImage, const miIOImage::IOImage& outputImage) override;

};

