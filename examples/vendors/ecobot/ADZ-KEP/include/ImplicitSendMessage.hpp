#ifndef IMPLICIT_SEND_MESSAGE_HPP
#define IMPLICIT_SEND_MESSAGE_HPP

namespace ImplicitSendMessage{
	struct ImplicitSendMessage{
		std::vector<uint8_t> remoteIO = {0x00,0x00};// #リモートI/O（R-IN）
		std::vector<uint8_t> operationNo = {0x00,0x00};// #運転データNo.選択
		std::vector<uint8_t> fixedIO = {0x00,0x00};// #固定I/O（IN）
		std::vector<uint8_t> directOperationMode = {0x00,0x00};// #ダイレクトデータ運転	運転方式
		std::vector<uint8_t> directOperationPosLower = {0x00,0x00};// #ダイレクトデータ運転	位置
		std::vector<uint8_t> directOperationPosUpper = {0x00,0x00};//
		std::vector<uint8_t> directOperationSpeedLower = {0x00,0x00};// #ダイレクトデータ運転	速度
		std::vector<uint8_t> directOperationSpeedUpper = {0x00,0x00};// 

		std::vector<uint8_t> directOperationStartRateLower = {0x00,0x00};//#ダイレクトデータ運転	起動・変速レート
		std::vector<uint8_t> directOperationStartRateUpper = {0x00,0x00};//

		std::vector<uint8_t> directOperationStopRateLower = {0x00,0x00};//#ダイレクトデータ運転	停止レート
		std::vector<uint8_t> directOperationStoptRateUpper = {0x00,0x00};//

		std::vector<uint8_t> directOperationAmpare = {0x00,0x00};// #ダイレクトデータ運転	運転電流

		std::vector<uint8_t> directOperationTransfar = {0x00,0x00};// # ダイレクトデータ運転	転送先

		std::vector<uint8_t> reservedID = {0x00,0x00};// #予約されている

		std::vector<uint8_t> readParameterID = {0x00,0x00};// #リードパラメータID 

		std::vector<uint8_t> writeRequest = {0x00,0x00};//#ライトリクエスト

		std::vector<uint8_t> writeParameterID = {0x00,0x00};// # ライトパラメータID

		std::vector<uint8_t> writeDataLower = {0x00,0x00};// #ライトデータ
		std::vector<uint8_t> writeDataUpper = {0x00,0x00};//
	};
	namespace RemoteIObits{
		typedef uint16_t RemoteIObits;
		enum RemoteIObit: RemoteIObits{
			R_IN0 = 1,
			R_IN1 = 1 << 1,
			R_IN2 = 1 << 2,
			R_IN3 = 1 << 3,
			R_IN4 = 1 << 4,
			R_IN5 = 1 << 5,
			R_IN6 = 1 << 6,
			R_IN7 = 1 << 7,
			R_IN8 = 1 << 8,
			R_IN9 = 1 << 9,
			R_IN10 = 1 << 10,
			R_IN11 = 1 << 11,
			R_IN12 = 1 << 12,
			R_IN13 = 1 << 13,
			R_IN14 = 1 << 14,
			R_IN15 = 1 << 15
		};
	};
	namespace OperationNobits{
		typedef uint16_t OperationNobits;
		// M0 は運転データNo1を示す。
		// 運転データNo0はOperationNobits=0
		enum OperationNobit: OperationNobits{
			M0 = 0,
			M1 = 1,
			M2 = 2,
			M3 = 3,
			M4 = 4,
			M5 = 5,
			M6 = 6,
			M7 = 7,
			Reserved_0 = 8,
			Reserved_1 = 9,
			Reserved_2 = 10,
			Reserved_3 = 11,
			Reserved_4 = 12,
			Reserved_5 = 13,
			Reserved_6 = 14,
			Reserved_7 = 15
		};
	};
	namespace FixedIObits{
		typedef uint16_t FixedIObits;
		enum FixedIObit: FixedIObits{
			FW_JOG = 1,
			RV_JOG = 1 << 1,
			Reserved_0 = 1 << 2,
			START = 1 << 3,
			ZHOME = 1 << 4,
			STOP = 1 << 5,
			FREE = 1 << 6,
			ALM_RST = 1 << 7,
			TRIG = 1 << 8,
			TRIG_MODE = 1 << 9,
			ETO_CLR = 1 << 10,
			Reserved_1 = 1 << 11,
			FW_JOG_P = 1 << 12,
			RV_JOG_P= 1 << 13,
			FW_POS = 1 << 14,
			RV_POS = 1 << 15
		};
	};
	namespace DirectOperationModebits{
		typedef uint16_t DirectOperationModebits;	
		enum DirectOperationModebit: DirectOperationModebits{
			Nothing = 0,
			AbsPos = 1,
			IncPosByCommand = 2,
			IncPosByDetection = 3,
			Reserved_0 = 4,
			Reserved_1 = 5,
			Reserved_2 = 6,
			ContinueOperationByPos = 7,
			RoundAbsPos = 8,
			RoundNearPos = 9,
			RoundFWDAbsPos = 10,
			RoundRVSAbsPos = 11,
			RoundAbsPush = 12,
			RoundNearPush = 13,
			RoundFWDPush = 14,
			RoundRVSPush = 15,

			ContinueOperationBySpeed = 16,
			ContinueOperationByPush = 17,
			ContinueOperationByTorque = 18,

			AbsPosPush = 20,
			IncPosPushByCommand = 21,
			IncPosPushByDetection = 22
		};
	};	
}
#endif