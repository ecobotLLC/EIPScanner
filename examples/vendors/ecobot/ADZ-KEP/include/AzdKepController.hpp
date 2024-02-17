#ifndef AZD_KEP_CONTROLLER_HPP
#define AZD_KEP_CONTROLLER_HPP
#include <cip/connectionManager/NetworkConnectionParams.h>
#include "ConnectionManager.h"
#include "ImplicitSendMessage.hpp"
#include "ImplicitRecieveMessage.hpp"
#include "utils/Logger.h"

namespace AzdKepController{
	enum Motion{
		MoveWait,
		MoveBase,
		MoveTop,
		MoveBottom,
		MoveCustom,
		MoveFREE,
		MoveSTOP
	};

	using eipScanner::utils::Logger;
	using eipScanner::utils::LogLevel;

	using eipScanner::cip::connectionManager::ConnectionParameters;
	using eipScanner::cip::connectionManager::NetworkConnectionParams;
	const std::string azdAddr= "192.168.11.200";
	const int azdPort = 0xAF12;
	const int T2O_size = 56;
	const int O2T_size = 40;
	const int electricGearA_ratio = 1;
	const int electricGearB_ratio = 2;
	const int GearRatio = 18;
	const double oneDegreeParStep = 360.0 /((double)GearRatio *1000.0* ((double)electricGearB_ratio / (double)electricGearA_ratio));
	static uint16_t convertToUint16LE(const std::vector<uint8_t>& bytes) {
		if (bytes.size() < 2) {
			// エラー処理
			Logger(LogLevel::ERROR) << "Error: Not enough data to convert.";
			return 0;
		}

		return static_cast<uint16_t>(bytes[1] << 8 | bytes[0]);
	};
	static void convertToUint16VE(uint16_t data, std::vector<uint8_t>& bytes) {
		bytes.clear(); // 既存の内容をクリア
		bytes.push_back(static_cast<uint8_t>(data & 0xFF)); // 下位バイトを追加
		bytes.push_back(static_cast<uint8_t>((data >> 8) & 0xFF));   // 上位バイトを追加
	};

	static int32_t convertDegToStep(double degree)
	{
		return degree / oneDegreeParStep;
	};
	static int32_t convertStepToDeg(double degree)
	{
		return degree * oneDegreeParStep;
	};
	class AzdKepController
	{	
		public:
			AzdKepController();
			~AzdKepController();
			bool SetConnection(eipScanner::IOConnection::WPtr io);
			bool HasError();
			bool IsReady();
			bool IsMoving();
			bool DirectMoveOff();
			bool IsDirectTRIG_R();
			bool MoveToPos(double pos_degree,double SpeedDegreeParSec,double AccelDegreeParSec,double BrakeDegreeParSec,uint16_t directOperationAmpare);
			bool StartMotion();
			bool FinishMotion();
			bool GoToBasePos();
			bool GoToTopPos();
			bool GoToBottomPos();
			bool FreeBrake();
			bool StopMotion();
			bool SendMessage();
			bool BuildMessage();
			bool UpDateRecieveData(uint16_t sequence,const std::vector<uint8_t>& data);
			ConnectionParameters parameters;
			Motion motion = MoveWait;
		private:
			bool _isSetParam = false;
			bool _isReady = true;
			bool _hasError = true;
			bool _isDirectMode = true;
			bool _isDirectTRIG_R = false;
			bool _isMoving = false;
			bool _isMove = false;

			ImplicitSendMessage::ImplicitSendMessage _implicitSendMessage;
			ImplicitRecieveMessage::ImplicitRecieveMessage _implicitRecieveMessage;
			eipScanner::IOConnection::WPtr _io;
			std::vector<uint8_t> _sendMessage;
			std::vector<uint8_t> _buffer;		
	};
};
#endif