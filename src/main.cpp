/**
 * ==========================================================================================
 * 
 *          Default arduino entry file
 * 
 * =========================================================================================
 *   
 */

#include <dartClient.h>


void setup()
{
    // called once after boot
    dartClient::instance().init();
}

void loop()
{
    // Continuous loop
    dartClient::instance().loop();
}