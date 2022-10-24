#include <cstdio>
#include "ConcertMachine.h"
#include <mi/miio/IOManager.h>

bool exit1 = false;
void signal_callback_handler(int signum) {

    // Terminate program
    exit1 = true;
}

int main()
{
    miIOManager::IOManager iomanager(1024, 1024);//define the iomanager with the ioimagesize
    //add the io manager to your application
    ConcertMachine machine(iomanager);
    //add your iomodul to the iomanager
   miIOManager::IOManagerResult result = iomanager.AddIOModul( "gpio1", "migpiomodul", "");//with manually offset
    if (result != miIOManager::IOManagerResult::Ok)
    {
        printf("Error %d\n",result);
    }
   result = iomanager.AddIOModul("hoerter12", "mipcf8574inmodul", "address=12");//with manually offset
    if (result != miIOManager::IOManagerResult::Ok)
    {
        printf("Error %d\n", result);
    }
    result = iomanager.AddIOModul("hoerter13", "mipcf8574outmodul", "address=13");//with manually offset
    if (result != miIOManager::IOManagerResult::Ok)
    {
        printf("Error %d\n", result);
    }
    //iomanager.AddIOModul("migpiomodul"); //or automatically offset
    
    //start the drivers
    iomanager.StartIOCycle(10);
    //start the application
   // machine.Start();


    printf("start aus %s!\n", "miapplicationTest");
    while (!exit1)
    {


       
        usleep(1000 * 500);
    }
    printf("Ende %s!\n", "miapplicationTest");
    return 0;
}