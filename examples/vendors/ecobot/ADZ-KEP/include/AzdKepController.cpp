#include "AzdKepController.hpp"
#include <mutex>
#include <sstream>

namespace AzdKepController{

	AzdKepController::AzdKepController()
	{
		parameters.connectionPath = {0x20, 0x04,0x2C, 0x65, 0x2C, 0x64};
		parameters.connectionPath = {0x20, 0x04,0x2C, 0x65, 0x2C, 0x64};  // config Assm151, output Assm150, intput Assm100
		parameters.o2tRealTimeFormat = true;
		parameters.originatorVendorId = 1285;//1285 for codesys sample
		parameters.originatorSerialNumber = 3677099906;//3677099906 codesys sample
		parameters.t2oNetworkConnectionParams |= NetworkConnectionParams::P2P;
		parameters.t2oNetworkConnectionParams |= NetworkConnectionParams::SCHEDULED_PRIORITY;
		parameters.t2oNetworkConnectionParams |= T2O_size;//56 
		parameters.o2tNetworkConnectionParams |= NetworkConnectionParams::P2P;
		parameters.o2tNetworkConnectionParams |= NetworkConnectionParams::SCHEDULED_PRIORITY;
		parameters.o2tNetworkConnectionParams |= O2T_size; //42

		parameters.originatorSerialNumber = 0x12345;
		parameters.o2tRPI = 100000;
		parameters.t2oRPI = 100000;
		parameters.transportTypeTrigger |= NetworkConnectionParams::CLASS1;


	}
	AzdKepController::~AzdKepController() = default;
	bool AzdKepController::SetConnection(eipScanner::IOConnection::WPtr io)
	{
		_io = io;
		return true;
	}
	bool AzdKepController::HasError()
	{
		uint16_t res = 0;
		uint16_t fixedIO = convertToUint16LE(_implicitRecieveMessage.fixedIO);
		Logger(LogLevel::INFO) << "recieve fixedIO=" << fixedIO;
		// res |= fixedIO & ImplicitRecieveMessage::FixedIObits::FixedIObit::SET_ERR;
		res |= fixedIO & ImplicitRecieveMessage::FixedIObits::FixedIObit::EXE_ERR;
		uint16_t presentAlarm = convertToUint16LE(_implicitRecieveMessage.presentAlarm);
		Logger(LogLevel::INFO) << "presentAlarm=" << presentAlarm;
		res |= presentAlarm;
		return (bool)res;
	}
	bool AzdKepController::IsReady()
	{
		uint16_t fixedIO = convertToUint16LE(_implicitRecieveMessage.fixedIO);
		return fixedIO & ImplicitRecieveMessage::FixedIObits::FixedIObit::READY;
	}
	bool AzdKepController::IsMoving()
	{
		uint16_t fixedIO = convertToUint16LE(_implicitRecieveMessage.fixedIO);
		return fixedIO & ImplicitRecieveMessage::FixedIObits::FixedIObit::MOVE;
	}
	bool AzdKepController::IsDirectTRIG_R()
	{
		uint16_t fixedIO = convertToUint16LE(_implicitRecieveMessage.fixedIO);
		return fixedIO & ImplicitRecieveMessage::FixedIObits::FixedIObit::TRIG_R;
	}
	bool AzdKepController::DirectMoveOff()
	{
		uint16_t fixedIO_tmp = convertToUint16LE(_implicitSendMessage.fixedIO);
		fixedIO_tmp = fixedIO_tmp & ~ImplicitSendMessage::FixedIObits::FixedIObit::TRIG_MODE;
		uint16_t fixedIO = fixedIO_tmp & ~ImplicitSendMessage::FixedIObits::FixedIObit::TRIG;
		convertToUint16VE(fixedIO,_implicitSendMessage.fixedIO);
		return true;
	}
	bool AzdKepController::MoveToPos(double pos_degree,double SpeedDegreeParSec,double AccelDegreeParSec,double BrakeDegreeParSec,uint16_t directOperationAmpare)
	{
		Logger(LogLevel::INFO) << "MoveToPos";
		if(!_isReady && !_isSetParam) return false;
		_isDirectMode = true;
		uint16_t fixedIO = convertToUint16LE(_implicitSendMessage.fixedIO);
		fixedIO = fixedIO & ~ImplicitSendMessage::FixedIObits::FixedIObit::TRIG_MODE;
		Logger(LogLevel::INFO) << "fixedIO:" << fixedIO;
		convertToUint16VE(fixedIO,_implicitSendMessage.fixedIO);
		std::ostringstream ss;
		for (auto &byte : _implicitSendMessage.fixedIO) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "fixedIO: " << ss.str();

		uint16_t directOperationMode = ImplicitSendMessage::DirectOperationModebits::DirectOperationModebit::AbsPos;
		Logger(LogLevel::INFO) << "directOperationMode:" << directOperationMode;
		convertToUint16VE(directOperationMode,_implicitSendMessage.directOperationMode);
		ss.str("");
		for (auto &byte : _implicitSendMessage.directOperationMode) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "directOperationMode: " << ss.str();


		uint16_t directOperationPosLower = convertDegToStep(pos_degree) & 0xFFFF;
		Logger(LogLevel::INFO) << "directOperationPosLower:" << directOperationPosLower;
		convertToUint16VE(directOperationPosLower,_implicitSendMessage.directOperationPosLower);
		ss.str("");
		for (auto &byte : _implicitSendMessage.directOperationPosLower) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "directOperationPosLower: " << ss.str();

		uint16_t directOperationPosUpper = (convertDegToStep(pos_degree) >> 16) & 0xFFFF;
		Logger(LogLevel::INFO) << "directOperationPosUpper:" << directOperationPosUpper;
		convertToUint16VE(directOperationPosUpper,_implicitSendMessage.directOperationPosUpper);
		ss.str("");
		for (auto &byte : _implicitSendMessage.directOperationPosUpper) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "directOperationPosUpper: " << ss.str();


		uint16_t directOperationSpeedLower = convertDegToStep(SpeedDegreeParSec) & 0xFFFF;
		Logger(LogLevel::INFO) << "directOperationSpeedLower:" << directOperationSpeedLower;
		convertToUint16VE(directOperationSpeedLower,_implicitSendMessage.directOperationSpeedLower);
		ss.str("");
		for (auto &byte : _implicitSendMessage.directOperationSpeedLower) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "directOperationSpeedLower: " << ss.str();

		uint16_t directOperationSpeedUpper = (convertDegToStep(SpeedDegreeParSec) >> 16) & 0xFFFF;
		Logger(LogLevel::INFO) << "directOperationSpeedUpper:" << directOperationSpeedUpper;
		convertToUint16VE(directOperationSpeedUpper,_implicitSendMessage.directOperationSpeedUpper);
		ss.str("");
		for (auto &byte : _implicitSendMessage.directOperationSpeedUpper) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "directOperationSpeedUpper: " << ss.str();


		uint16_t directOperationStartRateLower = convertDegToStep(AccelDegreeParSec) & 0xFFFF;
		Logger(LogLevel::INFO) << "directOperationStartRateLower:" << directOperationStartRateLower;
		convertToUint16VE(directOperationStartRateLower,_implicitSendMessage.directOperationStartRateLower);
		ss.str("");
		for (auto &byte : _implicitSendMessage.directOperationStartRateLower) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "directOperationStartRateLower: " << ss.str();

		uint16_t directOperationStartRateUpper = (convertDegToStep(AccelDegreeParSec) >> 16) & 0xFFFF;
		Logger(LogLevel::INFO) << "directOperationStartRateUpper:" << directOperationStartRateUpper;
		convertToUint16VE(directOperationStartRateUpper,_implicitSendMessage.directOperationStartRateUpper);
		ss.str("");
		for (auto &byte : _implicitSendMessage.directOperationStartRateUpper) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "directOperationStartRateUpper: " << ss.str();


		uint16_t directOperationStopRateLower = convertDegToStep(BrakeDegreeParSec) & 0xFFFF;
		Logger(LogLevel::INFO) << "directOperationStopRateLower:" << directOperationStopRateLower;
		convertToUint16VE(directOperationStopRateLower,_implicitSendMessage.directOperationStopRateLower);
		ss.str("");
		for (auto &byte : _implicitSendMessage.directOperationStopRateLower) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "directOperationStopRateLower: " << ss.str();

		uint16_t directOperationStoptRateUpper = (convertDegToStep(BrakeDegreeParSec) >> 16) & 0xFFFF;
		Logger(LogLevel::INFO) << "directOperationStoptRateUpper:" << directOperationStoptRateUpper;
		convertToUint16VE(directOperationStoptRateUpper,_implicitSendMessage.directOperationStoptRateUpper);
		ss.str("");
		for (auto &byte : _implicitSendMessage.directOperationStoptRateUpper) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "directOperationStoptRateUpper: " << ss.str();

		Logger(LogLevel::INFO) << "directOperationAmpare:" << directOperationAmpare;
		convertToUint16VE(directOperationAmpare,_implicitSendMessage.directOperationAmpare);
		ss.str("");
		for (auto &byte : _implicitSendMessage.directOperationAmpare) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "directOperationAmpare: " << ss.str();


		ss.str("");
		for (auto &byte : _implicitSendMessage.fixedIO) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "MoveToPos_fixedIO: " << ss.str();

		_isSetParam = true;
		return true;

	}
	bool AzdKepController::StartMotion()
	{
		if(_isMoving){
			_isMove = true;
		}
		bool isStart = _isMove;
		Logger(LogLevel::INFO) << "StartMotion :" << isStart;

		if(_isSetParam && _isReady)
		{
			std::ostringstream ss;

			isStart = true;
			uint16_t fixedIO = convertToUint16LE(_implicitSendMessage.fixedIO);

			fixedIO |= ImplicitSendMessage::FixedIObits::FixedIObit::START;

			if(_isDirectMode)
			{
				fixedIO |= ImplicitSendMessage::FixedIObits::FixedIObit::TRIG;

				if(_isDirectTRIG_R)
				{
					fixedIO = fixedIO & ~ImplicitSendMessage::FixedIObits::FixedIObit::TRIG;
					isStart = true;
				}
				else
				{
					isStart = false;
				}
			}
			if(_isMoving){
				_isMove = true;
			}
			convertToUint16VE(fixedIO,_implicitSendMessage.fixedIO);
			ss.str("");
			for (auto &byte : _implicitSendMessage.fixedIO) {
				ss << "[" << std::hex << (int) byte << "]";
			}
		}
		Logger(LogLevel::INFO) << "StartMotion res:" << isStart;
		return isStart;
	}
	bool AzdKepController::FinishMotion()
	{
		if(_isMove && !_isMoving)
		{
			if(_isDirectMode ){
				uint16_t fixedIO_tmp = convertToUint16LE(_implicitSendMessage.fixedIO);
				fixedIO_tmp = fixedIO_tmp & ~ImplicitSendMessage::FixedIObits::FixedIObit::START;
				fixedIO_tmp = fixedIO_tmp & ~ImplicitSendMessage::FixedIObits::FixedIObit::TRIG_MODE;
				uint16_t fixedIO = fixedIO_tmp & ~ImplicitSendMessage::FixedIObits::FixedIObit::TRIG;

				convertToUint16VE(fixedIO,_implicitSendMessage.fixedIO);
			}
			_isSetParam = false;
			_isMove = false;
			return true;
		}
		return false;
	}
	bool AzdKepController::GoToBasePos()
	{
		Logger(LogLevel::INFO) << "GoToBasePos";
		if(!_isReady && !_isSetParam) return false;
		uint16_t operationNo_uint = ImplicitSendMessage::OperationNobits::OperationNobit::M0;
		Logger(LogLevel::INFO) << "operationNo:" << operationNo_uint;
		convertToUint16VE(operationNo_uint,_implicitSendMessage.operationNo);
		std::ostringstream ss;
		for (auto &byte : _implicitSendMessage.operationNo) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "GoToBasePos: " << ss.str();

		_isSetParam = true;
		return true;
	}
	bool AzdKepController::GoToTopPos()
	{
		Logger(LogLevel::INFO) << "GoToTopPos";
		if(!_isReady && !_isSetParam) return false;
		uint16_t operationNo_uint = ImplicitSendMessage::OperationNobits::OperationNobit::M2;
		Logger(LogLevel::INFO) << "operationNo:" << operationNo_uint;
		convertToUint16VE(operationNo_uint,_implicitSendMessage.operationNo);
		std::ostringstream ss;
		for (auto &byte : _implicitSendMessage.operationNo) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "GoToBasePos: " << ss.str();
		_isSetParam = true;
		return true;

	}
	bool AzdKepController::GoToBottomPos()
	{
		Logger(LogLevel::INFO) << "GoToBottomPos";
		if(!_isReady && !_isSetParam) return false;
		uint16_t operationNo_uint = ImplicitSendMessage::OperationNobits::OperationNobit::M3;
		Logger(LogLevel::INFO) << "GoToBottomPos:" << operationNo_uint;
		convertToUint16VE(operationNo_uint,_implicitSendMessage.operationNo);
		std::ostringstream ss;
		for (auto &byte : _implicitSendMessage.operationNo) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "GoToBottomPos: " << ss.str();
		_isSetParam = true;
		return true;
	}
	bool AzdKepController::FreeBrake()
	{
		Logger(LogLevel::INFO) << "FreeBrake";
		if(!_isReady && !_isSetParam) return false;
		uint16_t fixedIO = ImplicitSendMessage::FixedIObits::FixedIObit::FREE;
		Logger(LogLevel::INFO) << "fixedIO:" << fixedIO;
		convertToUint16VE(fixedIO,_implicitSendMessage.fixedIO);
		std::ostringstream ss;
		for (auto &byte : _implicitSendMessage.fixedIO) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "FreeBrake: " << ss.str();
		_isSetParam = true;
		return true;
	}
	bool AzdKepController::StopMotion()
	{
		Logger(LogLevel::INFO) << "StopMotion";
		if(!_isReady && !_isSetParam) return false;
		uint16_t fixedIO = ImplicitSendMessage::FixedIObits::FixedIObit::STOP;
		Logger(LogLevel::INFO) << "fixedIO:" << fixedIO;
		convertToUint16VE(fixedIO,_implicitSendMessage.fixedIO);
		std::ostringstream ss;
		for (auto &byte : _implicitSendMessage.fixedIO) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "StopMotion: " << ss.str();
		_isSetParam = true;
		return true;
	}
	bool AzdKepController::SendMessage()
	{
		if (auto ptr = _io.lock()) {
			_buffer = std::move(_sendMessage);
			ptr->setDataToSend(_buffer);
		}
		return true;
	}
	bool AzdKepController::UpDateRecieveData(uint16_t sequence,const std::vector<uint8_t>& data)
	{
		size_t dataSize = data.size();
		if(dataSize != T2O_size)
		{
			Logger(LogLevel::ERROR) << "Received data different size" << dataSize << "vs" << sizeof(ImplicitRecieveMessage::ImplicitRecieveMessage);
			return false;
		}
		std::ostringstream ss;
		ss << "secNum=" << sequence << " data=";

		for (auto &byte : data) {
		ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "Received: " << ss.str();

		int offset = 0;
		_implicitRecieveMessage.remoteIO = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.operationNo = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.fixedIO = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.presentAlarm = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.detectionPositionLower = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.detectionPositionUpper = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.detectionSpeedLower = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.detectionSpeedUpper = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.commandPositionLower = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.commandPositionUpper = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.TorqueMonitor = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.CSToperatingCurrent = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.informationLower = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.informationUpper = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.reserved2 = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.readParameterID_R = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.readWriteStatus = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.writeParameterID_R = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.readDataLower = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.readDataUpper = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.selectionMonitor0Lower = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.selectionMonitor0Upper = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.selectionMonitor1Lower = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.selectionMonitor1Upper = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.selectionMonitor2Lower = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.selectionMonitor2Upper = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.selectionMonitor3Lower = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);
		offset += 2;
		_implicitRecieveMessage.selectionMonitor3Upper = std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + 2);

		uint32_t step = convertToUint16LE(_implicitRecieveMessage.detectionPositionLower) + (convertToUint16LE(_implicitRecieveMessage.detectionPositionUpper) << 16);
		Logger(LogLevel::INFO) << "pos: " << (double)step * oneDegreeParStep << "[deg]";
		_hasError = HasError();
		_isReady = IsReady();
		_isMoving = IsMoving();
		_isDirectTRIG_R = IsDirectTRIG_R();
		Logger(LogLevel::INFO) << "_hasError: " << _hasError << ",_isReady:" << _isReady;

		return true;
	}
	bool AzdKepController::BuildMessage()
	{
		Logger(LogLevel::INFO) << "BuildMessage";
		_sendMessage.clear();
		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.remoteIO.begin(), _implicitSendMessage.remoteIO.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.operationNo.begin(), _implicitSendMessage.operationNo.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.fixedIO.begin(), _implicitSendMessage.fixedIO.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.directOperationMode.begin(), _implicitSendMessage.directOperationMode.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.directOperationPosLower.begin(), _implicitSendMessage.directOperationPosLower.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.directOperationPosUpper.begin(), _implicitSendMessage.directOperationPosUpper.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.directOperationSpeedLower.begin(), _implicitSendMessage.directOperationSpeedLower.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.directOperationSpeedUpper.begin(), _implicitSendMessage.directOperationSpeedUpper.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.directOperationStartRateLower.begin(), _implicitSendMessage.directOperationStartRateLower.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.directOperationStartRateUpper.begin(), _implicitSendMessage.directOperationStartRateUpper.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.directOperationStopRateLower.begin(), _implicitSendMessage.directOperationStopRateLower.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.directOperationStoptRateUpper.begin(), _implicitSendMessage.directOperationStoptRateUpper.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.directOperationAmpare.begin(), _implicitSendMessage.directOperationAmpare.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.directOperationTransfar.begin(), _implicitSendMessage.directOperationTransfar.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.reservedID.begin(), _implicitSendMessage.reservedID.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.readParameterID.begin(), _implicitSendMessage.readParameterID.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.writeRequest.begin(), _implicitSendMessage.writeRequest.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.writeParameterID.begin(), _implicitSendMessage.writeParameterID.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.writeDataLower.begin(), _implicitSendMessage.writeDataLower.end());

		_sendMessage.insert(_sendMessage.end(), _implicitSendMessage.writeDataUpper.begin(), _implicitSendMessage.writeDataUpper.end());
		std::ostringstream ss;
		for (auto &byte : _implicitSendMessage.fixedIO) {
			ss << "[" << std::hex << (int) byte << "]";
		}
		Logger(LogLevel::INFO) << "sendfixedIO: " << ss.str();

		return true;
	}
}