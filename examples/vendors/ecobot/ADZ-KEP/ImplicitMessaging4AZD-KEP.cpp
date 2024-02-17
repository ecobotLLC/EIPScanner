#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#define OS_Windows (1)
#endif

#include <mutex>
#include <sstream>
#include <cip/connectionManager/NetworkConnectionParams.h>
#include "SessionInfo.h"
#include "ConnectionManager.h"
#include "utils/Logger.h"
#include "utils/Buffer.h"

#include"AzdKepController.hpp"

using namespace eipScanner::cip;
using eipScanner::SessionInfo;
using eipScanner::MessageRouter;
using eipScanner::ConnectionManager;
using eipScanner::cip::connectionManager::ConnectionParameters;
using eipScanner::cip::connectionManager::NetworkConnectionParams;
using eipScanner::utils::Buffer;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;
using AzdKepController::AzdKepController; 
std::mutex DataMutex;
AzdKepController::AzdKepController azdKepController;
int main(int argc, char* argv[]) {
  azdKepController = AzdKepController::AzdKepController();
  azdKepController.motion = AzdKepController::Motion::MoveWait;
  double pos_degree = 0;
  double SpeedDegreeParSec = 0;
  double AccelDegreeParSec = 0;
  double BrakeDegreeParSec = 0;
  uint16_t directOperationAmpare = 0;
  for (int i = 1; i < argc; ++i) { // argv[0] はプログラム名なのでスキップ
      std::string arg = argv[i];
      if (arg == "-h" || arg == "--help") {
          Logger(LogLevel::INFO) << "Help message";
          Logger(LogLevel::INFO) << "-v  ,--version: version";
          Logger(LogLevel::INFO) << "-ba , --base :move to base pos";
          Logger(LogLevel::INFO) << "-f , --free :brake free";
          Logger(LogLevel::INFO) << "-s , --stop :stop motion";
          Logger(LogLevel::INFO) << "-c ,-- custom :pos=400[deg] Speed:default:600[deg/s] accel:default:10000[deg/s^2] brake:default:10000[deg/s^2],Ampare default:1000[0.1%]";
          return EXIT_SUCCESS;
      } else if (arg == "-v" || arg == "--version") {
          Logger(LogLevel::INFO) << "Version 1.0";
          return EXIT_SUCCESS;
      } else if (arg == "-ba" || arg == "--base") {
          azdKepController.motion = AzdKepController::Motion::MoveBase;
          Logger(LogLevel::INFO) << "move to base pos";
      } else if (arg == "-t" || arg == "--top") {
          azdKepController.motion = AzdKepController::Motion::MoveTop;
          Logger(LogLevel::INFO) << "move to top pos";
      } else if (arg == "-b" || arg == "--bottom") {
          azdKepController.motion = AzdKepController::Motion::MoveBottom;
          Logger(LogLevel::INFO) << "move to bottom pos";
      } else if (arg == "-f" || arg == "--free") {
          azdKepController.motion = AzdKepController::Motion::MoveFREE;
          Logger(LogLevel::INFO) << "free brake mode";
      } else if (arg == "-s" || arg == "--stop") {
          azdKepController.motion = AzdKepController::Motion::MoveSTOP;
          Logger(LogLevel::INFO) << "motion stop";
      } else if (arg == "-c" || arg == "--custom") {
          azdKepController.motion = AzdKepController::Motion::MoveCustom;
          Logger(LogLevel::INFO) << "move to custom pos";
          if(argc - i == 6)
          {
            try {
                pos_degree = std::stod(argv[i + 1]);
                Logger(LogLevel::INFO) << "pos_degree: " << pos_degree;
            } catch (const std::invalid_argument& e) {
                Logger(LogLevel::ERROR) << "Invalid argument: could not convert to double.";
                return EXIT_FAILURE;
            } catch (const std::out_of_range& e) {
                Logger(LogLevel::ERROR) << "Argument out of range for a double.";
                return EXIT_FAILURE;
            }
            try {
                SpeedDegreeParSec = std::stod(argv[i + 2]);
                Logger(LogLevel::INFO) << "SpeedDegreeParSec: " << SpeedDegreeParSec;
            } catch (const std::invalid_argument& e) {
                Logger(LogLevel::ERROR) << "Invalid argument: could not convert to double.";
                return EXIT_FAILURE;
            } catch (const std::out_of_range& e) {
                Logger(LogLevel::ERROR) << "Argument out of range for a double.";
                return EXIT_FAILURE;
            }
            try {
                AccelDegreeParSec = std::stod(argv[i + 3]);
                Logger(LogLevel::INFO) << "AccelDegreeParSec: " << AccelDegreeParSec;
            } catch (const std::invalid_argument& e) {
                Logger(LogLevel::ERROR) << "Invalid argument: could not convert to double.";
                return EXIT_FAILURE;
            } catch (const std::out_of_range& e) {
                Logger(LogLevel::ERROR) << "Argument out of range for a double.";
                return EXIT_FAILURE;
            }
            try {
                BrakeDegreeParSec = std::stod(argv[i + 4]);
                Logger(LogLevel::INFO) << "BrakeDegreeParSec: " << BrakeDegreeParSec;
            } catch (const std::invalid_argument& e) {
                Logger(LogLevel::ERROR) << "Invalid argument: could not convert to double.";
                return EXIT_FAILURE;
            } catch (const std::out_of_range& e) {
                Logger(LogLevel::ERROR) << "Argument out of range for a double.";
                return EXIT_FAILURE;
            }
            try {
                directOperationAmpare = static_cast<uint16_t>(std::stoi(argv[i + 5]));
                Logger(LogLevel::INFO) << "directOperationAmpare: " << directOperationAmpare;
            } catch (const std::invalid_argument& e) {
                Logger(LogLevel::ERROR) << "Invalid argument: could not convert to double.";
                return EXIT_FAILURE;
            } catch (const std::out_of_range& e) {
                Logger(LogLevel::ERROR) << "Argument out of range for a double.";
                return EXIT_FAILURE;
            }

            break;
          }
      } else {
          Logger(LogLevel::ERROR) << "Unknown argument: " << arg;
          return EXIT_FAILURE;
      }
  }

  Logger::setLogLevel(LogLevel::DEBUG);
#if OS_Windows
  WSADATA wsaData;
  int winsockStart = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (winsockStart != 0) {
    Logger(LogLevel::ERROR) << "Failed to start WinSock - error code: " << winsockStart;
    return EXIT_FAILURE;
  }
#endif
  auto si = std::make_shared<SessionInfo>(AzdKepController::azdAddr, AzdKepController::azdPort);


  MessageRouter::SPtr mr_ptr = std::make_shared<MessageRouter>(MessageRouter::USE_8_BIT_PATH_SEGMENTS);

  ConnectionManager connectionManager = ConnectionManager(mr_ptr);

  std::vector<uint8_t> sendMeessage;
  sendMeessage.emplace_back(1);
  sendMeessage.emplace_back(0);
  for(int i=0;i<38;i++){
    sendMeessage.emplace_back(0);
  }
  auto io = connectionManager.forwardOpen(si, azdKepController.parameters);
  azdKepController.SetConnection(io);
  azdKepController.DirectMoveOff();
  azdKepController.BuildMessage();
  azdKepController.SendMessage();

  if (auto ptr = io.lock()) {
    Logger(LogLevel::INFO) << "io.lock sections ";
    // ptr->setDataToSend(sendMeessage);

    ptr->setReceiveDataListener([](auto realTimeHeader, auto sequence, auto data) {
      std::lock_guard<std::mutex> lock(DataMutex);
      azdKepController.UpDateRecieveData(sequence,data);
    });

    ptr->setCloseListener([]() {
      Logger(LogLevel::INFO) << "Listener Closed ";
    });
  };
  int count = 200000;
  while (connectionManager.hasOpenConnections() && count-- > 0) {

    connectionManager.handleConnections(std::chrono::milliseconds(100));
    {
      std::lock_guard<std::mutex> lock(DataMutex);
      if(!azdKepController.StartMotion())
      {
        if(azdKepController.motion == AzdKepController::Motion::MoveBase)
        {
          azdKepController.GoToBasePos();          
        }
        else if(azdKepController.motion == AzdKepController::Motion::MoveTop)
        {
          azdKepController.GoToTopPos();
        }
        else if(azdKepController.motion == AzdKepController::Motion::MoveBottom)
        {
    			azdKepController.GoToBottomPos();
        }
        else if(azdKepController.motion == AzdKepController::Motion::MoveFREE)
        {
    			azdKepController.FreeBrake();
        }
        else if(azdKepController.motion == AzdKepController::Motion::MoveSTOP)
        {
    			azdKepController.StopMotion();
        }
        else if(azdKepController.motion == AzdKepController::Motion::MoveCustom)
        {
          azdKepController.MoveToPos(pos_degree,SpeedDegreeParSec,AccelDegreeParSec,BrakeDegreeParSec,directOperationAmpare);
        }
      }
      else
      {
        if(azdKepController.FinishMotion())
        {
          azdKepController.BuildMessage();
          azdKepController.SendMessage();
          break;
        }
      }
    }
    azdKepController.BuildMessage();
    azdKepController.SendMessage();
    Logger(LogLevel::INFO) << "sendMeessage";
  }

  connectionManager.forwardClose(si, io);

#if OS_Windows
  WSACleanup();
#endif

  return EXIT_SUCCESS;
}
