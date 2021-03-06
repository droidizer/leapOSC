// leapOSC v0.2 by Morphic Creative

//Includes and namespace for Leap
#include <iostream>
#include "Leap.h"
using namespace Leap;

// Includes and defines for oscpack
#include "include/OscOutboundPacketStream.h"
#include "include/UdpSocket.h"
#define ADDRESS "127.0.0.1"
#define PORT 7000
#define OUTPUT_BUFFER_SIZE 2048
using namespace std;

class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
};

void SampleListener::onInit(const Controller& controller) {
  std::cout << "Device Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
  std::cout << "Device Connected" << std::endl;
}

void SampleListener::onDisconnect(const Controller& controller) {
  std::cout << "Device Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {

  // Get the most recent frame
  const Frame frame = controller.frame();

  // Get the hands
  const Hand handone = frame.hands()[0];
  const Hand handtwo = frame.hands()[1];
  const Hand handthree = frame.hands()[2];
  const Hand handfour = frame.hands()[3];

  // Get the fingers
  const FingerList handonefingers = handone.fingers();
  const FingerList handtwofingers = handtwo.fingers();
  const FingerList handthreefingers = handthree.fingers();
  const FingerList handfourfingers = handfour.fingers();

  // Calculate average finger tip positions
      Vector handonefingersavgPos;
      for (int i = 0; i < handonefingers.count(); ++i) {
        handonefingersavgPos += handonefingers[i].tipPosition();
      }
      handonefingersavgPos /= (float)handonefingers.count();
      Vector handtwofingersavgPos;
      for (int i = 0; i < handtwofingers.count(); ++i) {
        handtwofingersavgPos += handtwofingers[i].tipPosition();
      }
      handtwofingersavgPos /= (float)handtwofingers.count();
      Vector handthreefingersavgPos;
      for (int i = 0; i < handthreefingers.count(); ++i) {
        handthreefingersavgPos += handthreefingers[i].tipPosition();
      }
      handthreefingersavgPos /= (float)handthreefingers.count();
      Vector handfourfingersavgPos;
      for (int i = 0; i < handfourfingers.count(); ++i) {
        handfourfingersavgPos += handfourfingers[i].tipPosition();
      }
      handfourfingersavgPos /= (float)handfourfingers.count();

  // Get the hands normal vectors and directions
  const Vector handonenormalvector = handone.palmNormal();
  const Vector handonedirection = handone.direction();
  const Vector handtwonormalvector = handtwo.palmNormal();
  const Vector handtwodirection = handtwo.direction();
  const Vector handthreenormalvector = handthree.palmNormal();
  const Vector handthreedirection = handthree.direction();
  const Vector handfournormalvector = handfour.palmNormal();
  const Vector handfourdirection = handfour.direction();

  // Output Leap data via OSC
    UdpTransmitSocket transmitSocket( IpEndpointName( ADDRESS, PORT ) );    
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );   
    p << osc::BeginBundleImmediate
        << osc::BeginMessage( "/FrameInfo:" )
		<< frame.id()									//FrameInfo:1
		<< frame.timestamp()							//FrameInfo:2
		<< frame.hands().count()						//FrameInfo:3
		<< frame.fingers().count()						//FrameInfo:4
		<< frame.tools().count()						//FrameInfo:5
		<< osc::EndMessage
        << osc::BeginMessage( "/HandOne:" )
		<< handonefingers.count()						//HandOne:1
		<< handone.palmPosition().x						//HandOne:2 (mm)
		<< handone.palmPosition().y						//HandOne:3 (mm)
		<< handone.palmPosition().z						//HandOne:4 (mm)
		<< handone.palmVelocity().x						//HandOne:5 (mm/s)
		<< handone.palmVelocity().y						//HandOne:6 (mm/s)
		<< handone.palmVelocity().z						//HandOne:7 (mm/s)
		<< handonenormalvector.pitch() * RAD_TO_DEG		//HandOne:8 (deg)
		<< handonenormalvector.roll() * RAD_TO_DEG		//HandOne:9 (deg)
		<< handonenormalvector.yaw() * RAD_TO_DEG		//HandOne:10 (deg)
		<< handonedirection.pitch() * RAD_TO_DEG		//HandOne:11 (deg)
		<< handonedirection.roll() * RAD_TO_DEG			//HandOne:12 (deg)
		<< handonedirection.yaw() * RAD_TO_DEG			//HandOne:13 (deg)
		<< handone.sphereCenter().x						//HandOne:14 (mm)
		<< handone.sphereCenter().y						//HandOne:15 (mm)
		<< handone.sphereCenter().z						//HandOne:16 (mm)
		<< handone.sphereRadius()						//HandOne:17 (mm)
		<< handonefingersavgPos.x						//HandOne:18 (mm)
		<< handonefingersavgPos.y						//HandOne:19 (mm)
		<< handonefingersavgPos.z						//HandOne:20 (mm)
		<< handonefingers[0].length()					//HandOne:21 (mm)
		<< handonefingers[0].width()					//HandOne:22 (mm)
		<< handonefingers[0].direction().pitch() * RAD_TO_DEG		//HandOne:23 (deg)
		<< handonefingers[0].direction().roll()	* RAD_TO_DEG		//HandOne:24 (deg)
		<< handonefingers[0].direction().yaw() * RAD_TO_DEG			//HandOne:25 (deg)
		<< handonefingers[0].tipPosition().x			//HandOne:26 (mm)
		<< handonefingers[0].tipPosition().y			//HandOne:27 (mm)
		<< handonefingers[0].tipPosition().z			//HandOne:28 (mm)
		<< handonefingers[0].tipVelocity().x			//HandOne:29 (mm/s)
		<< handonefingers[0].tipVelocity().y			//HandOne:30 (mm/s)
		<< handonefingers[0].tipVelocity().z			//HandOne:31 (mm/s)
		<< handonefingers[1].length()					//HandOne:32 (mm)
		<< handonefingers[1].width()					//HandOne:33 (mm)
		<< handonefingers[1].direction().pitch() * RAD_TO_DEG		//HandOne:34 (deg)
		<< handonefingers[1].direction().roll()	* RAD_TO_DEG		//HandOne:35 (deg)
		<< handonefingers[1].direction().yaw() * RAD_TO_DEG			//HandOne:36 (deg)
		<< handonefingers[1].tipPosition().x			//HandOne:37 (mm)
		<< handonefingers[1].tipPosition().y			//HandOne:38 (mm)
		<< handonefingers[1].tipPosition().z			//HandOne:39 (mm)
		<< handonefingers[1].tipVelocity().x			//HandOne:40 (mm/s)
		<< handonefingers[1].tipVelocity().y			//HandOne:41 (mm/s)
		<< handonefingers[1].tipVelocity().z			//HandOne:42 (mm/s)
		<< handonefingers[2].length()					//HandOne:43 (mm)
		<< handonefingers[2].width()					//HandOne:44 (mm)
		<< handonefingers[2].direction().pitch() * RAD_TO_DEG		//HandOne:45 (deg)
		<< handonefingers[2].direction().roll()	* RAD_TO_DEG		//HandOne:46 (deg)
		<< handonefingers[2].direction().yaw() * RAD_TO_DEG			//HandOne:47 (deg)
		<< handonefingers[2].tipPosition().x			//HandOne:48 (mm)
		<< handonefingers[2].tipPosition().y			//HandOne:49 (mm)
		<< handonefingers[2].tipPosition().z			//HandOne:50 (mm)
		<< handonefingers[2].tipVelocity().x			//HandOne:51 (mm/s)
		<< handonefingers[2].tipVelocity().y			//HandOne:52 (mm/s)
		<< handonefingers[2].tipVelocity().z			//HandOne:53 (mm/s)
		<< handonefingers[3].length()					//HandOne:54 (mm)
		<< handonefingers[3].width()					//HandOne:55 (mm)
		<< handonefingers[3].direction().pitch() * RAD_TO_DEG		//HandOne:56 (deg)
		<< handonefingers[3].direction().roll()	* RAD_TO_DEG		//HandOne:57 (deg)
		<< handonefingers[3].direction().yaw() * RAD_TO_DEG			//HandOne:58 (deg)
		<< handonefingers[3].tipPosition().x			//HandOne:59 (mm)
		<< handonefingers[3].tipPosition().y			//HandOne:60 (mm)
		<< handonefingers[3].tipPosition().z			//HandOne:61 (mm)
		<< handonefingers[3].tipVelocity().x			//HandOne:62 (mm/s)
		<< handonefingers[3].tipVelocity().y			//HandOne:63 (mm/s)
		<< handonefingers[3].tipVelocity().z			//HandOne:64 (mm/s)
		<< handonefingers[4].length()					//HandOne:65 (mm)
		<< handonefingers[4].width()					//HandOne:66 (mm)
		<< handonefingers[4].direction().pitch() * RAD_TO_DEG		//HandOne:67 (deg)
		<< handonefingers[4].direction().roll()	* RAD_TO_DEG		//HandOne:68 (deg)
		<< handonefingers[4].direction().yaw() * RAD_TO_DEG			//HandOne:69 (deg)
		<< handonefingers[4].tipPosition().x			//HandOne:70 (mm)
		<< handonefingers[4].tipPosition().y			//HandOne:71 (mm)
		<< handonefingers[4].tipPosition().z			//HandOne:72 (mm)
		<< handonefingers[4].tipVelocity().x			//HandOne:73 (mm/s)
		<< handonefingers[4].tipVelocity().y			//HandOne:74 (mm/s)
		<< handonefingers[4].tipVelocity().z			//HandOne:75 (mm/s)
		<< osc::EndMessage
        << osc::BeginMessage( "/HandTwo:" )
		<< handtwofingers.count()						//HandTwo:1
		<< handtwo.palmPosition().x						//HandTwo:2 (mm)
		<< handtwo.palmPosition().y						//HandTwo:3 (mm)
		<< handtwo.palmPosition().z						//HandTwo:4 (mm)
		<< handtwo.palmVelocity().x						//HandTwo:5 (mm/s)
		<< handtwo.palmVelocity().y						//HandTwo:6 (mm/s)
		<< handtwo.palmVelocity().z						//HandTwo:7 (mm/s)
		<< handtwonormalvector.pitch() * RAD_TO_DEG		//HandTwo:8 (deg)
		<< handtwonormalvector.roll() * RAD_TO_DEG		//HandTwo:9 (deg)
		<< handtwonormalvector.yaw() * RAD_TO_DEG		//HandTwo:10 (deg)
		<< handtwodirection.pitch() * RAD_TO_DEG		//HandTwo:11 (deg)
		<< handtwodirection.roll() * RAD_TO_DEG			//HandTwo:12 (deg)
		<< handtwodirection.yaw() * RAD_TO_DEG			//HandTwo:13 (deg)
		<< handtwo.sphereCenter().x						//HandTwo:14 (mm)
		<< handtwo.sphereCenter().y						//HandTwo:15 (mm)
		<< handtwo.sphereCenter().z						//HandTwo:16 (mm)
		<< handtwo.sphereRadius()						//HandTwo:17 (mm)
		<< handtwofingersavgPos.x						//HandTwo:18 (mm)
		<< handtwofingersavgPos.y						//HandTwo:19 (mm)
		<< handtwofingersavgPos.z						//HandTwo:20 (mm)
		<< handtwofingers[0].length()					//HandTwo:21 (mm)
		<< handtwofingers[0].width()					//HandTwo:22 (mm)
		<< handtwofingers[0].direction().pitch() * RAD_TO_DEG		//HandTwo:23 (deg)
		<< handtwofingers[0].direction().roll()	* RAD_TO_DEG		//HandTwo:24 (deg)
		<< handtwofingers[0].direction().yaw() * RAD_TO_DEG			//HandTwo:25 (deg)
		<< handtwofingers[0].tipPosition().x			//HandTwo:26 (mm)
		<< handtwofingers[0].tipPosition().y			//HandTwo:27 (mm)
		<< handtwofingers[0].tipPosition().z			//HandTwo:28 (mm)
		<< handtwofingers[0].tipVelocity().x			//HandTwo:29 (mm/s)
		<< handtwofingers[0].tipVelocity().y			//HandTwo:30 (mm/s)
		<< handtwofingers[0].tipVelocity().z			//HandTwo:31 (mm/s)
		<< handtwofingers[1].length()					//HandTwo:32 (mm)
		<< handtwofingers[1].width()					//HandTwo:33 (mm)
		<< handtwofingers[1].direction().pitch() * RAD_TO_DEG		//HandTwo:34 (deg)
		<< handtwofingers[1].direction().roll()	* RAD_TO_DEG		//HandTwo:35 (deg)
		<< handtwofingers[1].direction().yaw() * RAD_TO_DEG			//HandTwo:36 (deg)
		<< handtwofingers[1].tipPosition().x			//HandTwo:37 (mm)
		<< handtwofingers[1].tipPosition().y			//HandTwo:38 (mm)
		<< handtwofingers[1].tipPosition().z			//HandTwo:39 (mm)
		<< handtwofingers[1].tipVelocity().x			//HandTwo:40 (mm/s)
		<< handtwofingers[1].tipVelocity().y			//HandTwo:41 (mm/s)
		<< handtwofingers[1].tipVelocity().z			//HandTwo:42 (mm/s)
		<< handtwofingers[2].length()					//HandTwo:43 (mm)
		<< handtwofingers[2].width()					//HandTwo:44 (mm)
		<< handtwofingers[2].direction().pitch() * RAD_TO_DEG		//HandTwo:45 (deg)
		<< handtwofingers[2].direction().roll()	* RAD_TO_DEG		//HandTwo:46 (deg)
		<< handtwofingers[2].direction().yaw() * RAD_TO_DEG			//HandTwo:47 (deg)
		<< handtwofingers[2].tipPosition().x			//HandTwo:48 (mm)
		<< handtwofingers[2].tipPosition().y			//HandTwo:49 (mm)
		<< handtwofingers[2].tipPosition().z			//HandTwo:50 (mm)
		<< handtwofingers[2].tipVelocity().x			//HandTwo:51 (mm/s)
		<< handtwofingers[2].tipVelocity().y			//HandTwo:52 (mm/s)
		<< handtwofingers[2].tipVelocity().z			//HandTwo:53 (mm/s)
		<< handtwofingers[3].length()					//HandTwo:54 (mm)
		<< handtwofingers[3].width()					//HandTwo:55 (mm)
		<< handtwofingers[3].direction().pitch() * RAD_TO_DEG		//HandTwo:56 (deg)
		<< handtwofingers[3].direction().roll()	* RAD_TO_DEG		//HandTwo:57 (deg)
		<< handtwofingers[3].direction().yaw() * RAD_TO_DEG			//HandTwo:58 (deg)
		<< handtwofingers[3].tipPosition().x			//HandTwo:59 (mm)
		<< handtwofingers[3].tipPosition().y			//HandTwo:60 (mm)
		<< handtwofingers[3].tipPosition().z			//HandTwo:61 (mm)
		<< handtwofingers[3].tipVelocity().x			//HandTwo:62 (mm/s)
		<< handtwofingers[3].tipVelocity().y			//HandTwo:63 (mm/s)
		<< handtwofingers[3].tipVelocity().z			//HandTwo:64 (mm/s)
		<< handtwofingers[4].length()					//HandTwo:65 (mm)
		<< handtwofingers[4].width()					//HandTwo:66 (mm)
		<< handtwofingers[4].direction().pitch() * RAD_TO_DEG		//HandTwo:67 (deg)
		<< handtwofingers[4].direction().roll()	* RAD_TO_DEG		//HandTwo:68 (deg)
		<< handtwofingers[4].direction().yaw() * RAD_TO_DEG			//HandTwo:69 (deg)
		<< handtwofingers[4].tipPosition().x			//HandTwo:70 (mm)
		<< handtwofingers[4].tipPosition().y			//HandTwo:71 (mm)
		<< handtwofingers[4].tipPosition().z			//HandTwo:72 (mm)
		<< handtwofingers[4].tipVelocity().x			//HandTwo:73 (mm/s)
		<< handtwofingers[4].tipVelocity().y			//HandTwo:74 (mm/s)
		<< handtwofingers[4].tipVelocity().z			//HandTwo:75 (mm/s)
		<< osc::EndMessage
        << osc::BeginMessage( "/HandThree:" )
		<< handthreefingers.count()						//HandThree:1
		<< handthree.palmPosition().x						//HandThree:2 (mm)
		<< handthree.palmPosition().y						//HandThree:3 (mm)
		<< handthree.palmPosition().z						//HandThree:4 (mm)
		<< handthree.palmVelocity().x						//HandThree:5 (mm/s)
		<< handthree.palmVelocity().y						//HandThree:6 (mm/s)
		<< handthree.palmVelocity().z						//HandThree:7 (mm/s)
		<< handthreenormalvector.pitch() * RAD_TO_DEG		//HandThree:8 (deg)
		<< handthreenormalvector.roll() * RAD_TO_DEG		//HandThree:9 (deg)
		<< handthreenormalvector.yaw() * RAD_TO_DEG		//HandThree:10 (deg)
		<< handthreedirection.pitch() * RAD_TO_DEG		//HandThree:11 (deg)
		<< handthreedirection.roll() * RAD_TO_DEG			//HandThree:12 (deg)
		<< handthreedirection.yaw() * RAD_TO_DEG			//HandThree:13 (deg)
		<< handthree.sphereCenter().x						//HandThree:14 (mm)
		<< handthree.sphereCenter().y						//HandThree:15 (mm)
		<< handthree.sphereCenter().z						//HandThree:16 (mm)
		<< handthree.sphereRadius()						//HandThree:17 (mm)
		<< handthreefingersavgPos.x						//HandThree:18 (mm)
		<< handthreefingersavgPos.y						//HandThree:19 (mm)
		<< handthreefingersavgPos.z						//HandThree:20 (mm)
		<< handthreefingers[0].length()					//HandThree:21 (mm)
		<< handthreefingers[0].width()					//HandThree:22 (mm)
		<< handthreefingers[0].direction().pitch() * RAD_TO_DEG		//HandThree:23 (deg)
		<< handthreefingers[0].direction().roll()	* RAD_TO_DEG		//HandThree:24 (deg)
		<< handthreefingers[0].direction().yaw() * RAD_TO_DEG			//HandThree:25 (deg)
		<< handthreefingers[0].tipPosition().x			//HandThree:26 (mm)
		<< handthreefingers[0].tipPosition().y			//HandThree:27 (mm)
		<< handthreefingers[0].tipPosition().z			//HandThree:28 (mm)
		<< handthreefingers[0].tipVelocity().x			//HandThree:29 (mm/s)
		<< handthreefingers[0].tipVelocity().y			//HandThree:30 (mm/s)
		<< handthreefingers[0].tipVelocity().z			//HandThree:31 (mm/s)
		<< handthreefingers[1].length()					//HandThree:32 (mm)
		<< handthreefingers[1].width()					//HandThree:33 (mm)
		<< handthreefingers[1].direction().pitch() * RAD_TO_DEG		//HandThree:34 (deg)
		<< handthreefingers[1].direction().roll()	* RAD_TO_DEG		//HandThree:35 (deg)
		<< handthreefingers[1].direction().yaw() * RAD_TO_DEG			//HandThree:36 (deg)
		<< handthreefingers[1].tipPosition().x			//HandThree:37 (mm)
		<< handthreefingers[1].tipPosition().y			//HandThree:38 (mm)
		<< handthreefingers[1].tipPosition().z			//HandThree:39 (mm)
		<< handthreefingers[1].tipVelocity().x			//HandThree:40 (mm/s)
		<< handthreefingers[1].tipVelocity().y			//HandThree:41 (mm/s)
		<< handthreefingers[1].tipVelocity().z			//HandThree:42 (mm/s)
		<< handthreefingers[2].length()					//HandThree:43 (mm)
		<< handthreefingers[2].width()					//HandThree:44 (mm)
		<< handthreefingers[2].direction().pitch() * RAD_TO_DEG		//HandThree:45 (deg)
		<< handthreefingers[2].direction().roll()	* RAD_TO_DEG		//HandThree:46 (deg)
		<< handthreefingers[2].direction().yaw() * RAD_TO_DEG			//HandThree:47 (deg)
		<< handthreefingers[2].tipPosition().x			//HandThree:48 (mm)
		<< handthreefingers[2].tipPosition().y			//HandThree:49 (mm)
		<< handthreefingers[2].tipPosition().z			//HandThree:50 (mm)
		<< handthreefingers[2].tipVelocity().x			//HandThree:51 (mm/s)
		<< handthreefingers[2].tipVelocity().y			//HandThree:52 (mm/s)
		<< handthreefingers[2].tipVelocity().z			//HandThree:53 (mm/s)
		<< handthreefingers[3].length()					//HandThree:54 (mm)
		<< handthreefingers[3].width()					//HandThree:55 (mm)
		<< handthreefingers[3].direction().pitch() * RAD_TO_DEG		//HandThree:56 (deg)
		<< handthreefingers[3].direction().roll()	* RAD_TO_DEG		//HandThree:57 (deg)
		<< handthreefingers[3].direction().yaw() * RAD_TO_DEG			//HandThree:58 (deg)
		<< handthreefingers[3].tipPosition().x			//HandThree:59 (mm)
		<< handthreefingers[3].tipPosition().y			//HandThree:60 (mm)
		<< handthreefingers[3].tipPosition().z			//HandThree:61 (mm)
		<< handthreefingers[3].tipVelocity().x			//HandThree:62 (mm/s)
		<< handthreefingers[3].tipVelocity().y			//HandThree:63 (mm/s)
		<< handthreefingers[3].tipVelocity().z			//HandThree:64 (mm/s)
		<< handthreefingers[4].length()					//HandThree:65 (mm)
		<< handthreefingers[4].width()					//HandThree:66 (mm)
		<< handthreefingers[4].direction().pitch() * RAD_TO_DEG		//HandThree:67 (deg)
		<< handthreefingers[4].direction().roll()	* RAD_TO_DEG		//HandThree:68 (deg)
		<< handthreefingers[4].direction().yaw() * RAD_TO_DEG			//HandThree:69 (deg)
		<< handthreefingers[4].tipPosition().x			//HandThree:70 (mm)
		<< handthreefingers[4].tipPosition().y			//HandThree:71 (mm)
		<< handthreefingers[4].tipPosition().z			//HandThree:72 (mm)
		<< handthreefingers[4].tipVelocity().x			//HandThree:73 (mm/s)
		<< handthreefingers[4].tipVelocity().y			//HandThree:74 (mm/s)
		<< handthreefingers[4].tipVelocity().z			//HandThree:75 (mm/s)
		<< osc::EndMessage
        << osc::BeginMessage( "/HandFour:" )
		<< handfourfingers.count()						//HandFour:1
		<< handfour.palmPosition().x						//HandFour:2 (mm)
		<< handfour.palmPosition().y						//HandFour:3 (mm)
		<< handfour.palmPosition().z						//HandFour:4 (mm)
		<< handfour.palmVelocity().x						//HandFour:5 (mm/s)
		<< handfour.palmVelocity().y						//HandFour:6 (mm/s)
		<< handfour.palmVelocity().z						//HandFour:7 (mm/s)
		<< handfournormalvector.pitch() * RAD_TO_DEG		//HandFour:8 (deg)
		<< handfournormalvector.roll() * RAD_TO_DEG		//HandFour:9 (deg)
		<< handfournormalvector.yaw() * RAD_TO_DEG		//HandFour:10 (deg)
		<< handfourdirection.pitch() * RAD_TO_DEG		//HandFour:11 (deg)
		<< handfourdirection.roll() * RAD_TO_DEG			//HandFour:12 (deg)
		<< handfourdirection.yaw() * RAD_TO_DEG			//HandFour:13 (deg)
		<< handfour.sphereCenter().x						//HandFour:14 (mm)
		<< handfour.sphereCenter().y						//HandFour:15 (mm)
		<< handfour.sphereCenter().z						//HandFour:16 (mm)
		<< handfour.sphereRadius()						//HandFour:17 (mm)
		<< handfourfingersavgPos.x						//HandFour:18 (mm)
		<< handfourfingersavgPos.y						//HandFour:19 (mm)
		<< handfourfingersavgPos.z						//HandFour:20 (mm)
		<< handfourfingers[0].length()					//HandFour:21 (mm)
		<< handfourfingers[0].width()					//HandFour:22 (mm)
		<< handfourfingers[0].direction().pitch() * RAD_TO_DEG		//HandFour:23 (deg)
		<< handfourfingers[0].direction().roll()	* RAD_TO_DEG		//HandFour:24 (deg)
		<< handfourfingers[0].direction().yaw() * RAD_TO_DEG			//HandFour:25 (deg)
		<< handfourfingers[0].tipPosition().x			//HandFour:26 (mm)
		<< handfourfingers[0].tipPosition().y			//HandFour:27 (mm)
		<< handfourfingers[0].tipPosition().z			//HandFour:28 (mm)
		<< handfourfingers[0].tipVelocity().x			//HandFour:29 (mm/s)
		<< handfourfingers[0].tipVelocity().y			//HandFour:30 (mm/s)
		<< handfourfingers[0].tipVelocity().z			//HandFour:31 (mm/s)
		<< handfourfingers[1].length()					//HandFour:32 (mm)
		<< handfourfingers[1].width()					//HandFour:33 (mm)
		<< handfourfingers[1].direction().pitch() * RAD_TO_DEG		//HandFour:34 (deg)
		<< handfourfingers[1].direction().roll()	* RAD_TO_DEG		//HandFour:35 (deg)
		<< handfourfingers[1].direction().yaw() * RAD_TO_DEG			//HandFour:36 (deg)
		<< handfourfingers[1].tipPosition().x			//HandFour:37 (mm)
		<< handfourfingers[1].tipPosition().y			//HandFour:38 (mm)
		<< handfourfingers[1].tipPosition().z			//HandFour:39 (mm)
		<< handfourfingers[1].tipVelocity().x			//HandFour:40 (mm/s)
		<< handfourfingers[1].tipVelocity().y			//HandFour:41 (mm/s)
		<< handfourfingers[1].tipVelocity().z			//HandFour:42 (mm/s)
		<< handfourfingers[2].length()					//HandFour:43 (mm)
		<< handfourfingers[2].width()					//HandFour:44 (mm)
		<< handfourfingers[2].direction().pitch() * RAD_TO_DEG		//HandFour:45 (deg)
		<< handfourfingers[2].direction().roll()	* RAD_TO_DEG		//HandFour:46 (deg)
		<< handfourfingers[2].direction().yaw() * RAD_TO_DEG			//HandFour:47 (deg)
		<< handfourfingers[2].tipPosition().x			//HandFour:48 (mm)
		<< handfourfingers[2].tipPosition().y			//HandFour:49 (mm)
		<< handfourfingers[2].tipPosition().z			//HandFour:50 (mm)
		<< handfourfingers[2].tipVelocity().x			//HandFour:51 (mm/s)
		<< handfourfingers[2].tipVelocity().y			//HandFour:52 (mm/s)
		<< handfourfingers[2].tipVelocity().z			//HandFour:53 (mm/s)
		<< handfourfingers[3].length()					//HandFour:54 (mm)
		<< handfourfingers[3].width()					//HandFour:55 (mm)
		<< handfourfingers[3].direction().pitch() * RAD_TO_DEG		//HandFour:56 (deg)
		<< handfourfingers[3].direction().roll()	* RAD_TO_DEG		//HandFour:57 (deg)
		<< handfourfingers[3].direction().yaw() * RAD_TO_DEG			//HandFour:58 (deg)
		<< handfourfingers[3].tipPosition().x			//HandFour:59 (mm)
		<< handfourfingers[3].tipPosition().y			//HandFour:60 (mm)
		<< handfourfingers[3].tipPosition().z			//HandFour:61 (mm)
		<< handfourfingers[3].tipVelocity().x			//HandFour:62 (mm/s)
		<< handfourfingers[3].tipVelocity().y			//HandFour:63 (mm/s)
		<< handfourfingers[3].tipVelocity().z			//HandFour:64 (mm/s)
		<< handfourfingers[4].length()					//HandFour:65 (mm)
		<< handfourfingers[4].width()					//HandFour:66 (mm)
		<< handfourfingers[4].direction().pitch() * RAD_TO_DEG		//HandFour:67 (deg)
		<< handfourfingers[4].direction().roll()	* RAD_TO_DEG		//HandFour:68 (deg)
		<< handfourfingers[4].direction().yaw() * RAD_TO_DEG			//HandFour:69 (deg)
		<< handfourfingers[4].tipPosition().x			//HandFour:70 (mm)
		<< handfourfingers[4].tipPosition().y			//HandFour:71 (mm)
		<< handfourfingers[4].tipPosition().z			//HandFour:72 (mm)
		<< handfourfingers[4].tipVelocity().x			//HandFour:73 (mm/s)
		<< handfourfingers[4].tipVelocity().y			//HandFour:74 (mm/s)
		<< handfourfingers[4].tipVelocity().z			//HandFour:75 (mm/s)
		<< osc::EndMessage
		<< osc::EndBundle;  
    transmitSocket.Send( p.Data(), p.Size() );
}

// 'int argc, char* argv[]' added to 'int main' for oscpack
int main(int argc, char* argv[]) {

  //Program title and OSC configuration
  std::cout << "leapOSC v0.2 by Morphic Creative" << std::endl;
  std::cout << "" << std::endl;
  std::cout << "OSC output to: " << ADDRESS << ", port: " << PORT << ", output buffer size: " << OUTPUT_BUFFER_SIZE << std::endl;
  std::cout << "" << std::endl;

  // Create a sample listener and controller
  SampleListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);

  return 0;
}