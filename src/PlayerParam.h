/************************************************************************************
 * WrightEagle (Soccer Simulation League 2D)                                        *
 * BASE SOURCE CODE RELEASE 2016                                                    *
 * Copyright (c) 1998-2016 WrightEagle 2D Soccer Simulation Team,                   *
 *                         Multi-Agent Systems Lab.,                                *
 *                         School of Computer Science and Technology,               *
 *                         University of Science and Technology of China            *
 * All rights reserved.                                                             *
 *                                                                                  *
 * Redistribution and use in source and binary forms, with or without               *
 * modification, are permitted provided that the following conditions are met:      *
 *     * Redistributions of source code must retain the above copyright             *
 *       notice, this list of conditions and the following disclaimer.              *
 *     * Redistributions in binary form must reproduce the above copyright          *
 *       notice, this list of conditions and the following disclaimer in the        *
 *       documentation and/or other materials provided with the distribution.       *
 *     * Neither the name of the WrightEagle 2D Soccer Simulation Team nor the      *
 *       names of its contributors may be used to endorse or promote products       *
 *       derived from this software without specific prior written permission.      *
 *                                                                                  *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND  *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED    *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
 * DISCLAIMED. IN NO EVENT SHALL WrightEagle 2D Soccer Simulation Team BE LIABLE    *
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL       *
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR       *
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER       *
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,    *
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF *
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                *
 ************************************************************************************/

#ifndef PLAYERPARAM_H_
#define PLAYERPARAM_H_

#include "ParamEngine.h"
#include "Types.h"
#include "Parser.h"

/**
 * 将dash power离散为100个，存储一些数据
 * Dash power will be divided into 100, some data storage.
 */
#define DASH_POWER_NUM 100

class HeteroParam: public ParamEngine {
	HeteroParam( const HeteroParam & ); // not used
	HeteroParam & operator=( const HeteroParam & ); // not used
	void ParseFromCmdLine(int argc, char **argv);
	bool ParseFromConfigFile(const char *file_name);

public:
	HeteroParam();
	virtual ~HeteroParam(){};

private:
	void AddParams();

private:
	int    M_type;

	double player_speed_max;
	double stamina_inc_max;
	double player_decay;
	double inertia_moment;
	double dash_power_rate;
	double player_size;
	double kickable_margin;
	double kick_rand;
	double extra_stamina;
	double effort_max;
	double effort_min;

	//v14
	double kick_power_rate;
	double catchable_area_l_stretch;
	double foul_detect_probability;

	//private
	double kickable_area;
	double min_catch_area;
	double max_catch_area;
    // 下面4个量都是以effort_max来计算的，使用时要注意
    double effective_speed_max;
    double acceleration_front_rate;
    double acceleration_side_rate;

	Array<double, DASH_POWER_NUM + 1> acceleration_front;
	Array<double, DASH_POWER_NUM + 1> acceleration_side;

public:
	const int & type() const;

	//standard
	const double & playerSpeedMax() const;
	const double & staminaIncMax() const;
	const double & playerDecay() const;
	const double & inertiaMoment() const;
	const double & dashPowerRate() const;
	const double & playerSize() const;
	const double & kickableMargin() const;
	const double & kickRand() const;
	const double & extraStamina() const;
	const double & effortMax() const;
	const double & effortMin() const;

	//v14
	const double & kickPowerRate() const;
	const double & catchableAreaLStretch() const;
	const double & foulDetectProbability() const;

	//private
	const double & kickableArea() const;
	const double & effectiveSpeedMax() const;
	const double & minCatchArea() const;
	const double & maxCatchArea() const;

    const double & accelerationFrontRate() const;
    const double & accelerationSideRate() const;
    const double & accelerationFrontMax() const;
    const double & accelerationSideMax() const;
    const double & accelerationFront(const double & power) const;
    const double & accelerationSide(const double & power) const;

	double accelerationRateOnDir(const double & dir) const;

	/**
	*  一些有依赖关系的变量的初始化和赋值
	*/
	void MaintainConsistency();
};

class RemotePlayerParam: public ParamEngine {
	RemotePlayerParam(); // private
	RemotePlayerParam( const RemotePlayerParam & ); // not used
	RemotePlayerParam & operator=( const RemotePlayerParam & ); // not used

public:
	virtual ~RemotePlayerParam();
	static RemotePlayerParam* instance();
	void init(int argc, char **argv);

private:
	//default values
	static const char CONFIG_FILE[];
	static const char LOG_DIR[];
	static const char TEAM_NAME[];
	static const char OPPONENT_TEAM_NAME[];
	static const char HETERO_TEST_MODEL[];
	
	static const char TRAIN_DATA_FILE[];

	static const int DEFAULT_PLAYER_TYPES;
	static const int DEFAULT_SUBS_MAX;
	static const int DEFAULT_PT_MAX;

	static const double DEFAULT_PLAYER_SPEED_MAX_DELTA_MIN;
	static const double DEFAULT_PLAYER_SPEED_MAX_DELTA_MAX;
	static const double DEFAULT_STAMINA_INC_MAX_DELTA_FACTOR;

	static const double DEFAULT_PLAYER_DECAY_DELTA_MIN;
	static const double DEFAULT_PLAYER_DECAY_DELTA_MAX;
	static const double DEFAULT_INERTIA_MOMENT_DELTA_FACTOR;

	static const double DEFAULT_DASH_POWER_RATE_DELTA_MIN;
	static const double DEFAULT_DASH_POWER_RATE_DELTA_MAX;
	static const double DEFAULT_PLAYER_SIZE_DELTA_FACTOR;

	static const double DEFAULT_KICKABLE_MARGIN_DELTA_MIN;
	static const double DEFAULT_KICKABLE_MARGIN_DELTA_MAX;
	static const double DEFAULT_KICK_RAND_DELTA_FACTOR;

	static const double DEFAULT_EXTRA_STAMINA_DELTA_MIN;
	static const double DEFAULT_EXTRA_STAMINA_DELTA_MAX;
	static const double DEFAULT_EFFORT_MAX_DELTA_FACTOR;
	static const double DEFAULT_EFFORT_MIN_DELTA_FACTOR;

	static const int    DEFAULT_RANDOM_SEED;

	static const double DEFAULT_NEW_DASH_POWER_RATE_DELTA_MIN;
	static const double DEFAULT_NEW_DASH_POWER_RATE_DELTA_MAX;
	static const double DEFAULT_NEW_STAMINA_INC_MAX_DELTA_FACTOR;

	//v 14
    static const double DEFAULT_KICK_POWER_RATE_DELTA_MIN;
    static const double DEFAULT_KICK_POWER_RATE_DELTA_MAX;
    static const double DEFAULT_FOUL_DETECT_PROBABILITY_DELTA_FACTOR;

    static const double DEFAULT_CATCHABLE_AREA_L_STRETCH_MIN;
    static const double DEFAULT_CATCHABLE_AREA_L_STRETCH_MAX;

    //private
	static const double MAX_SHOOT_DISTANCE;
	static const double SHOOT_EVALUATION;
	static const double SHOOT_MAX_CONSIDER_PROB;
	static const double SHOOT_MIN_PROB;
	static const double SHOOT_MIN_PROB1;
	static const double SHOOT_MIN_PROB2;
	static const int SHOOT_MAX_SEARCH_COUNT;
	static const double SHOOT_DIR_SEARCH_STEP;

	//conf的常量 参考we2008
	static const double MAX_CONF;
	static const double MIN_VALID_CONF;
	static const double CONF_DECAY;
	static const double PLAYER_CONF_DECAY;
	static const double BALL_CONF_DECAY;

private:
	static int player_types;
	int subs_max;
	int pt_max;

	bool M_allow_mult_default_type;

	double player_speed_max_delta_min;
	double player_speed_max_delta_max;
	double stamina_inc_max_delta_factor;

	double player_decay_delta_min;
	double player_decay_delta_max;
	double inertia_moment_delta_factor;

	double dash_power_rate_delta_min;
	double dash_power_rate_delta_max;
	double player_size_delta_factor;

	double kickable_margin_delta_min;
	double kickable_margin_delta_max;
	double kick_rand_delta_factor;

	double extra_stamina_delta_min;
	double extra_stamina_delta_max;
	double effort_max_delta_factor;
	double effort_min_delta_factor;

	double new_dash_power_rate_delta_min;
	double new_dash_power_rate_delta_max;
	double new_stamina_inc_max_delta_factor;

	//v14
	double catchable_area_l_stretch_min;
	double catchable_area_l_stretch_max;

	double kick_power_rate_delta_min;
	double kick_power_rate_delta_max;

	double foul_detect_probability_delta_factor;

	//private
	std::string M_player_conf_file;
	std::string M_log_dir;

	std::string M_train_data_file;

	std::string M_team_name;
	int M_team_name_len;
	std::string M_opponent_team_name;
	std::string M_hetero_test_model;

	static int M_our_goalie_unum;
	bool M_is_goalie;
	bool M_is_coach;
	
	bool M_is_trainer;
	
	double M_player_version;
	double M_coach_version;

	double M_say_pos_x_eps;
	double M_say_pos_y_eps;
	double M_say_ball_speed_eps;
	double M_say_player_speed_eps;
	double M_say_dir_eps;

	double M_max_conf;
	static double M_min_valid_conf;
	double M_conf_decay;
	double M_player_conf_decay;
	double M_ball_conf_decay;

	enum
	{
		SIGHTSIZE = 62,
		MARKSIZE = 388
	};
	double mSightChange[SIGHTSIZE][3];
	double mMarkChange[MARKSIZE][3];

	void AddParams();
public:


	//sight dist 主要指视觉中ball player的dist ,其所加的误差加了0.1的截断
    double ConvertSightDist(double dist)
    {
		Assert(dist >= 0);
		int low = 0;
		int middle = 0;
		int high = SIGHTSIZE;

		while(low <=  high)
		{
			middle = (low + high ) / 2;

			if (middle >= SIGHTSIZE || middle < 0)
			{
				break;
			}

			if (fabs(dist - mSightChange[middle][0]) < 0.01)
			{
				return mSightChange[middle][1];
			}

			if (dist < mSightChange[middle][0])
			{
				high = middle;
			}
			else
			{
				low = middle + 1;
			}
		}

		 PRINT_ERROR("can not find this dist " << dist );
		 return dist;
    }

	double GetEpsInSight(double dist)
	{
		 Assert(dist >= 0);
		 int low = 0;
		 int middle = 0;
		 int high = SIGHTSIZE - 1;

		 while(low <= high)
		 {
			middle = (low + high) / 2;

			if (middle + 1 >= SIGHTSIZE)
			{
				return mSightChange[SIGHTSIZE - 1][2];
			}

			if (dist >= mSightChange[middle][0] && dist <= mSightChange[middle + 1][0])
			{
				if (fabs(dist - mSightChange[middle][0]) < fabs (dist - mSightChange[middle + 1][0]))
				{
					return mSightChange[middle][2];
				}
				else
				{
					return mSightChange[middle + 1][2];
				}
			}

			if (middle == 0)
			{
				return mSightChange[middle][2];
			}

			if (dist < mSightChange[middle][0])
			{
				high = middle;
			}
			else
			{
				low = middle + 1;
			}

		 }

		 PRINT_ERROR("can not find this dist " << dist );
		 return 0;
	}

	//mark dist 主要指视觉中marker 的dist的误差 ,其所加的截断为0.01
    double ConvertMarkDist(double dist)
    {
		Assert(dist >= 0);
		int low = 0;
		int middle = 0;
		int high = MARKSIZE;

		while(low <=  high)
		{
			middle = (low + high ) / 2;

			if (middle >= MARKSIZE || middle < 0)
			{
				break;
			}

			if (fabs(dist - mMarkChange[middle][0]) < 0.01)
			{
				return mMarkChange[middle][1];
			}

			if (dist < mMarkChange[middle][0])
			{
				high = middle;
			}
			else
			{
				low = middle + 1;
			}
		}

		 PRINT_ERROR("can not find this dist " << dist );
		 return dist;
    }

	double GetEpsInMark(double dist)
	{
		 Assert(dist >= 0);
		 int low = 0;
		 int middle = 0;
		 int high = MARKSIZE - 1;

		 while(low <= high)
		 {
			middle = (low + high) / 2;

			if (middle + 1 >= MARKSIZE)
			{
				return mMarkChange[MARKSIZE - 1][2];
			}

			if (dist >= mMarkChange[middle][0] && dist <= mMarkChange[middle + 1][0])
			{
				if (fabs(dist - mMarkChange[middle][0]) < fabs (dist - mMarkChange[middle + 1][0]))
				{
					return mMarkChange[middle][2];
				}
				else
				{
					return mMarkChange[middle + 1][2];
				}
			}

			if (middle == 0)
			{
				return mMarkChange[middle][2];
			}

			if (dist < mMarkChange[middle][0])
			{
				high = middle;
			}
			else
			{
				low = middle + 1;
			}
		 }

		 PRINT_ERROR("can not find this dist " << dist );
		 return 0;
	}

	double ConvertAngle(double angle)
	{
		if (angle > 0)
		{
			angle += 0.5;
		}
		else	if (angle < 0)
		{
			angle -= 0.5;
		}

		return angle;
	}

	void MaintainConsistency();
	bool SaveParam();

	static int playerTypes();
	int subsMax() const;
	int ptMax() const;

	bool allowMultDefaultType() const;

	const double & maxConf() const;
	static const double & minValidConf(); // ???????
	const double & confDecay() const;
	const double & playerConfDecay() const;
	const double & ballConfDecay() const;

	const double & playerSpeedMaxDeltaMin() const;

	const double & playerSpeedMaxDeltaMax() const;
	const double & staminaIncMaxDeltaFactor() const;

	const double & playerDecayDeltaMin() const;
	const double & playerDecayDeltaMax() const;
	const double & inertiaMomentDeltaFactor() const;

	const double & dashPowerRateDeltaMin() const;
	const double & dashPowerRateDeltaMax() const;
	const double & playerSizeDeltaFactor() const;

	const double & kickableMarginDeltaMin() const;
	const double & kickableMarginDeltaMax() const;
	const double & kickRandDeltaFactor() const;

	const double & extraStaminaDeltaMin() const;
	const double & extraStaminaDeltaMax() const;
	const double & effortMaxDeltaFactor() const;
	const double & effortMinDeltaFactor() const;

    const double & kickPowerRateDeltaMin() const;
    const double & kickPowerRateDeltaMax() const;
    const double & foulDetectProbabilityDeltaFactor() const;

	const double & newDashPowerRateDeltaMin() const;
	const double & newDashPowerRateDeltaMax() const;
	const double & newStaminaIncMaxDeltaFactor() const;

	const double & shootMaxDistance() const;

	const std::string & logDir();
	const std::string & teamName() const;
	void setTeamName(const char *name);
	const int & teamNameLen() const;
	const std::string & opponentTeamName() const;

	const std::string & trainDataFile() const;

	void setOpponentTeamName(const char *name);

	const std::string & heteroTestModel() const;

	static const int & ourGoalieUnum(); //这个量在决策层不应该使用，否则反算对手时会出错
	const bool & isGoalie() const; //这个量在决策层不应该使用，否则反算对手时会出错
	const bool & isCoach() const;
	
	const bool & isTrainer() const;
	
	const double & playerVersion() const;
	const double & coachVersion() const;

	const double & sayPosXEps() const;
	const double & sayPosYEps() const;
	const double & sayBallSpeedEps() const;
	const double & sayPlayerSpeedEps() const;
	const double & sayDirEps() const;

private:
	static HeteroParam *mHeteroPlayer;

public:
	/**
	* add hetero player type from server msg
	* @param line server msg
	*/
	static void AddPlayerType(int type, const char *line);

	static HeteroParam & HeteroPlayer(const int & type)
	{
		Assert (type >= 0 && type < m_playerParams.playerTypes());
		// Assert(type == 0 || Parser::IsPlayerTypesReady());
		Assert(mHeteroPlayer);
		auto& obj = mHeteroPlayer[type];
		return obj;
	}

private:
	static const bool DYNAMIC_DEBUG_MODE;
	static const bool SAVE_SERVER_MESSAGE;
	static const bool SAVE_SIGHT_LOG;
	static const bool SAVE_DEC_LOG;
	static const bool SAVE_TEXT_LOG;
	static const bool SAVE_STAT_LOG;
	static const bool USE_PLOTTER;
    static const bool USE_TEAM_GRAPHIC;
	static const bool TIME_TEST;
	static const bool NETWORK_TEST;
	static const int WAIT_SIGHT_BUFFER;
	static const int WAIT_HEAR_BUFFER;
	static const int WAIT_TIME_OUT;
	static const double ROUTE_ANGLE_DIFF;
    static const double OPP_TACKLE_THRESHOLD_FORWARD;
    static const double OPP_TACKLE_THRESHOLD_MID;
    static const double OPP_TACKLE_THRESHOLD_BACK;
    static const double TIRED_BUFFER;
    static const double AT_POINT_BUFFER;
    static const int KICKER_MODE;
    static const int MARKOV_DRIBBLER_MODE;
    static const int MARKOV_DRIBBLER_HORIZON;
    static const int MARKOV_DRIBBLER_METHOD;
	static const double MIN_APPEARANCE_POSS;
	static const double GOALIE_LOW_DELAY;
	static const double GOALIE_HIGH_DELAY;
	static const int A_STAR_GO_TO_POINT_MAX_DEPTH;
	static const int A_STAR_GO_TO_POINT_MAX_NODES;
	static const int VELOCITY_RANDOMIZED_DIRS;
	static const int VELOCITY_RANDOMIZED_SAMPLES;
	static const double LOW_STAMINA_POINT_THR;
	static const int COACH_SEND_HETERO_INFO_CONTROL;
	static const double SETPLAY_REINFORCE_DIST;
	static const int SETPLAY_REINFORCE_PLAYERS;

	bool mDynamicDebugMode; // DynamicDebug模式
	bool mForcePenaltyMode; //利用trainer强制进入penalty模式
	bool mSaveServerMessage; // 是否保存server的信息，用于动态调试
	bool mSaveSightLog; // 是否保存sight_log
	bool mSaveDecLog; // 是否保存dec_log
	static bool mSaveTextLog;
	bool mUsePlotter;
    bool mUseTeamGraphic;
	static bool mTimeTest;
	bool mNetworkTest;
	int mWaitSightBuffer; // 等待视觉到来的最大buffer
	int mWaitHearBuffer; // 等待听觉到来的最大buffer
	int mWaitTimeOut; // 等待server的最大时间

    double mTiredBuffer;
    double mMinStamina;
    static double mAtPointBuffer;

    //射门相关
	double shoot_max_distance;

	/**
     * 对应Kicker的几种状态
     *
     * 0表示读取值函数文件，进行正常比赛
     * 1表示离线计算值函数，需要关掉所有log
     */
    static int mKickerMode;

    /**
     * 如果视觉部分导致超时严重，就调大这个变量，最大为1
     */
	double M_min_appearance_poss;

    double mLowStaminaPointThr;

public:
	const bool & DynamicDebugMode();
	const bool & ForcePenaltyMode() const;
	const bool & SaveServerMessage();
	const bool & SaveSightLog();
	const bool & SaveDecLog();
	static const bool & SaveTextLog();
	static const bool & TimeTest();
	const bool & NetworkTest();
	const bool & UsePlotter();
    const bool & UseTeamGraphic() const;
	const int & WaitSightBuffer() const;
	const int & WaitHearBuffer() const;
	const int & WaitTimeOut();

	const double & minAppearancePoss() const;

    const double & TiredBuffer() const;
    const double & MinStamina() const;
    static const double & AtPointBuffer(); // ????
    static const int & KickerMode(); // ????

	const double & LowStaminaPointThr() const;
};

#endif /* PLAYERPARAM_H_ */
