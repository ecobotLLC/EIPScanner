#ifndef IMPLICIT_RECIEVE_MESSAGE_HPP
#define IMPLICIT_RECIEVE_MESSAGE_HPP
namespace ImplicitRecieveMessage{
	struct ImplicitRecieveMessage{
		std::vector<uint8_t> remoteIO = {0x00,0x00};// #リモートI/O（R-OUT）
		std::vector<uint8_t> operationNo = {0x00,0x00};// #運転データNo.選択_R
		std::vector<uint8_t> fixedIO = {0x00,0x00};// #固定I/O（OUT）
		std::vector<uint8_t> presentAlarm = {0x00,0x00};// #現在アラーム

		std::vector<uint8_t> detectionPositionLower = {0x00,0x00};// #検出位置（下位）
		std::vector<uint8_t> detectionPositionUpper = {0x00,0x00};// #検出位置（上位）

		std::vector<uint8_t> detectionSpeedLower = {0x00,0x00};//検出速度［Hz］（下位）
		std::vector<uint8_t> detectionSpeedUpper = {0x00,0x00};//検出速度［Hz］（上位）

		std::vector<uint8_t> commandPositionLower = {0x00,0x00};// #指令位置（下位）
		std::vector<uint8_t> commandPositionUpper = {0x00,0x00};// #指令位置（上位）

		std::vector<uint8_t> TorqueMonitor = {0x00,0x00};// トルクモニタ

		std::vector<uint8_t> CSToperatingCurrent = {0x00,0x00};// CST運転電流
		std::vector<uint8_t> informationLower = {0x00,0x00};//インフォメーション（下位）
		std::vector<uint8_t> informationUpper = {0x00,0x00};//インフォメーション（上位）


		std::vector<uint8_t> reserved2 = {0x00,0x00};//#予約
		std::vector<uint8_t> readParameterID_R = {0x00,0x00};//リードパラメータID_R

		std::vector<uint8_t> readWriteStatus = {0x00,0x00};// #リード/ライトステータス

		std::vector<uint8_t> writeParameterID_R = {0x00,0x00};// # ライトパラメータID_R

		std::vector<uint8_t> readDataLower = {0x00,0x00};// #リードデータ（下位)
		std::vector<uint8_t> readDataUpper = {0x00,0x00};// #リードデータ（上位)


		std::vector<uint8_t> selectionMonitor0Lower = {0x00,0x00};// #
		std::vector<uint8_t> selectionMonitor0Upper = {0x00,0x00};// #

		std::vector<uint8_t> selectionMonitor1Lower = {0x00,0x00};//#
		std::vector<uint8_t> selectionMonitor1Upper = {0x00,0x00};//#

		std::vector<uint8_t> selectionMonitor2Lower = {0x00,0x00};// # 
		std::vector<uint8_t> selectionMonitor2Upper = {0x00,0x00};// # 

		std::vector<uint8_t> selectionMonitor3Lower = {0x00,0x00};// #
		std::vector<uint8_t> selectionMonitor3Upper = {0x00,0x00};// #

	};
	namespace RemoteIObits{
		typedef uint16_t RemoteIObits;
		enum RemoteIObit : RemoteIObits{
			M0_R = 1,
			M1_R = 1 << 1,
			M2_R = 1 << 2,
			START_R = 1 << 3,
			HOME_END = 1 << 4,
			READY = 1 << 5,
			INFO = 1 << 6,
			ALM_A = 1 << 7,
			SYS_BSY = 1 << 8,
			AREA0 = 1 << 9,
			AREA1 = 1 << 10,
			AREA2 = 1 << 11,
			TIM = 1 << 12,
			MOVE = 1 << 13,
			IN_POS = 1 << 14,
			TLC = 1 << 15
		};
	}
	namespace OperationNobits{
		typedef uint16_t OperationNobits;
		enum OperationNobit: OperationNobits{
			M0_R = 1,
			M1_R = 1 << 1,
			M2_R = 1 << 2,
			M3_R = 1 << 3,
			M4_R = 1 << 4,
			M5_R = 1 << 5,
			M6_R = 1 << 6,
			M7_R = 1 << 7,
			Reserved_0 = 1 << 8,
			Reserved_1 = 1 << 9,
			Reserved_2 = 1 << 10,
			Reserved_3 = 1 << 11,
			Reserved_4 = 1 << 12,
			Reserved_5 = 1 << 13,
			Reserved_6 = 1 << 14,
			Reserved_7 = 1 << 15
		};
	}
	namespace FixedIObits{
		typedef uint16_t FixedIObits;
		enum FixedIObit: FixedIObits{
			SEQ_BSY = 1,
			MOVE = 1 << 1,
			IN_POS = 1 << 2,
			START_R = 1 << 3,
			HOME_END = 1 << 4,
			READY = 1 << 5,
			DCMD_RDY = 1 << 6,
			ALM_A = 1 << 7,
			TRIG_R = 1 << 8,
			TRIG_MODE_R = 1 << 9,
			SET_ERR = 1 << 10,
			EXE_ERR = 1 << 11,
			DCMD_FULL = 1 << 12,
			STOP_R = 1 << 13,
			ETO_MON = 1 << 14,
			TLC = 1 << 15
		};
	}
	namespace ReadWriteStatusbits{
		typedef uint16_t ReadWriteStatusbits;
		enum ReadWriteStatusbit: ReadWriteStatusbits{
			Reserved_0 = 1,
			Reserved_1 = 1 << 1,
			Reserved_2 = 1 << 2,
			Reserved_3 = 1 << 3,
			Reserved_4 = 1 << 4,
			Reserved_5 = 1 << 5,
			Reserved_6 = 1 << 6,
			RD_ERR = 1 << 7,
			WR_END = 1 << 8,
			SYS_BSY = 1 << 9,
			Reserved_7 = 1 << 10,
			WR_SET_ERR = 1 << 11,
			WR_IF_ERR = 1 << 12,
			WR_NV_ERR = 1 << 13,
			WR_EXE_ERR = 1 << 14,
			WR_ERR = 1 << 15
		};
	}

}
#endif