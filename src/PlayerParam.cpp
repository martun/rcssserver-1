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

#include "ServerParam.h"
#include "PlayerParam.h"
#include "Parser.h"
#include "ActionEffector.h"
#include <fstream>

const char RemotePlayerParam::CONFIG_FILE[] = "./conf/player.conf";
const char RemotePlayerParam::LOG_DIR[] = "Logfiles";
const char RemotePlayerParam::TEAM_NAME[] = "WEBase";
const char RemotePlayerParam::OPPONENT_TEAM_NAME[] = "";
const char RemotePlayerParam::HETERO_TEST_MODEL[] = "./conf/hetero.conf";

const char RemotePlayerParam::TRAIN_DATA_FILE[] = "./train/train.conf";

const int RemotePlayerParam::DEFAULT_PLAYER_TYPES = 18; // [12.0.0] 7 -> 18
const int RemotePlayerParam::DEFAULT_SUBS_MAX = 3;
const int RemotePlayerParam::DEFAULT_PT_MAX = 1; // [12.0.0] 3 -> 1

const double RemotePlayerParam::DEFAULT_PLAYER_SPEED_MAX_DELTA_MIN = 0.0;
const double RemotePlayerParam::DEFAULT_PLAYER_SPEED_MAX_DELTA_MAX = 0.0;
const double RemotePlayerParam::DEFAULT_STAMINA_INC_MAX_DELTA_FACTOR = 0.0;

const double RemotePlayerParam::DEFAULT_PLAYER_DECAY_DELTA_MIN = -0.1; // [12.0.0] 0.0 -> -0.05
const double RemotePlayerParam::DEFAULT_PLAYER_DECAY_DELTA_MAX = 0.1; // [12.0.0] 0.2 -> 0.1
const double RemotePlayerParam::DEFAULT_INERTIA_MOMENT_DELTA_FACTOR = 25.0;

const double RemotePlayerParam::DEFAULT_DASH_POWER_RATE_DELTA_MIN = 0.0;
const double RemotePlayerParam::DEFAULT_DASH_POWER_RATE_DELTA_MAX = 0.0;
const double RemotePlayerParam::DEFAULT_PLAYER_SIZE_DELTA_FACTOR = -100.0;

const double RemotePlayerParam::DEFAULT_KICKABLE_MARGIN_DELTA_MIN = -0.1; // [12.0.0] 0.0 -> -0.1
const double RemotePlayerParam::DEFAULT_KICKABLE_MARGIN_DELTA_MAX = 0.1; // [12.0.0] 0.2 -> 0.1
const double RemotePlayerParam::DEFAULT_KICK_RAND_DELTA_FACTOR = 1.0; // [12.0.0] 0.5 -> 1.0

const double RemotePlayerParam::DEFAULT_EXTRA_STAMINA_DELTA_MIN = 0.0;
const double RemotePlayerParam::DEFAULT_EXTRA_STAMINA_DELTA_MAX = 50.0;
const double RemotePlayerParam::DEFAULT_EFFORT_MAX_DELTA_FACTOR = -0.002;
const double RemotePlayerParam::DEFAULT_EFFORT_MIN_DELTA_FACTOR = -0.002;

const double RemotePlayerParam::DEFAULT_NEW_DASH_POWER_RATE_DELTA_MIN = -0.0012; // [12.0.0] 0 -> -0.0005
const double RemotePlayerParam::DEFAULT_NEW_DASH_POWER_RATE_DELTA_MAX = 0.0008; // [12.0.0] 0.002 -> 0.0015
const double RemotePlayerParam::DEFAULT_NEW_STAMINA_INC_MAX_DELTA_FACTOR = -6000.0; // [12.0.0] -10000.0 -> -6000.0

// v14
const double RemotePlayerParam::DEFAULT_KICK_POWER_RATE_DELTA_MIN = 0.0;
const double RemotePlayerParam::DEFAULT_KICK_POWER_RATE_DELTA_MAX = 0.0;
const double RemotePlayerParam::DEFAULT_FOUL_DETECT_PROBABILITY_DELTA_FACTOR = 0.0;

const double RemotePlayerParam::DEFAULT_CATCHABLE_AREA_L_STRETCH_MIN = 1.0;
const double RemotePlayerParam::DEFAULT_CATCHABLE_AREA_L_STRETCH_MAX = 1.3;

//private
const double RemotePlayerParam::MAX_SHOOT_DISTANCE = 32.5;
const double RemotePlayerParam::SHOOT_EVALUATION = 5.0;
const double RemotePlayerParam::SHOOT_MAX_CONSIDER_PROB = 0.97;
const double RemotePlayerParam::SHOOT_MIN_PROB = 0.56;
const double RemotePlayerParam::SHOOT_MIN_PROB1 = 0.60;
const double RemotePlayerParam::SHOOT_MIN_PROB2 = 0.60;
const int RemotePlayerParam::SHOOT_MAX_SEARCH_COUNT = 128;
const double RemotePlayerParam::SHOOT_DIR_SEARCH_STEP = 0.6;

const bool RemotePlayerParam::DYNAMIC_DEBUG_MODE = false;
const bool RemotePlayerParam::SAVE_SERVER_MESSAGE = false;
const bool RemotePlayerParam::SAVE_SIGHT_LOG = false;
const bool RemotePlayerParam::SAVE_DEC_LOG = false;
const bool RemotePlayerParam::SAVE_TEXT_LOG = false;
const bool RemotePlayerParam::USE_PLOTTER = false;
const bool RemotePlayerParam::USE_TEAM_GRAPHIC = true;
const bool RemotePlayerParam::TIME_TEST = false;
const bool RemotePlayerParam::NETWORK_TEST = false;
const int RemotePlayerParam::WAIT_SIGHT_BUFFER = 40; // 每周期最多等视觉40毫秒
const int RemotePlayerParam::WAIT_HEAR_BUFFER = 40; // 每周期最多等听觉40毫秒
const int RemotePlayerParam::WAIT_TIME_OUT = 10; // 每场比赛最多等server10秒
const double RemotePlayerParam::ROUTE_ANGLE_DIFF = 1.0;
const double RemotePlayerParam::OPP_TACKLE_THRESHOLD_FORWARD = 0.69;
const double RemotePlayerParam::OPP_TACKLE_THRESHOLD_MID = 0.75;
const double RemotePlayerParam::OPP_TACKLE_THRESHOLD_BACK = 0.85;
const double RemotePlayerParam::TIRED_BUFFER = 10.0;
const double RemotePlayerParam::AT_POINT_BUFFER = 1.0;
const int RemotePlayerParam::KICKER_MODE = 0;
const int RemotePlayerParam::MARKOV_DRIBBLER_MODE = 0;
const int RemotePlayerParam::MARKOV_DRIBBLER_HORIZON = 3;
const int RemotePlayerParam::MARKOV_DRIBBLER_METHOD = 1;
const double RemotePlayerParam::MAX_CONF           = 1.0;
const double RemotePlayerParam::MIN_VALID_CONF       = 0.5;
const double RemotePlayerParam::CONF_DECAY          = 0.98;
const double RemotePlayerParam::PLAYER_CONF_DECAY    = 0.99;
const double RemotePlayerParam::BALL_CONF_DECAY      = 0.95;
const double RemotePlayerParam::MIN_APPEARANCE_POSS = 0.05;
const double RemotePlayerParam::GOALIE_LOW_DELAY = 0.25;
const double RemotePlayerParam::GOALIE_HIGH_DELAY = 0.25;
const int RemotePlayerParam::A_STAR_GO_TO_POINT_MAX_DEPTH = 128;
const int RemotePlayerParam::A_STAR_GO_TO_POINT_MAX_NODES = 1024;
const int RemotePlayerParam::VELOCITY_RANDOMIZED_DIRS = 8;
const int RemotePlayerParam::VELOCITY_RANDOMIZED_SAMPLES = 1;
const double RemotePlayerParam::LOW_STAMINA_POINT_THR = 2600.0;//这个以下dash时就会控制了
const int RemotePlayerParam::COACH_SEND_HETERO_INFO_CONTROL = 6;
const double RemotePlayerParam::SETPLAY_REINFORCE_DIST = 8.0;
const int RemotePlayerParam::SETPLAY_REINFORCE_PLAYERS = 2;

HeteroParam::HeteroParam()
{
	AddParams();
	MaintainConsistency();
}

void HeteroParam::AddParams()
{
	AddParam( "id", & M_type, 0);
	AddParam( "player_speed_max", & player_speed_max, RemoteServerParam::instance().playerSpeedMax());
	AddParam( "stamina_inc_max", & stamina_inc_max, RemoteServerParam::instance().staminaInc());
	AddParam( "player_decay", & player_decay, RemoteServerParam::instance().playerDecay());
	AddParam( "inertia_moment", & inertia_moment, RemoteServerParam::instance().inertiaMoment());
	AddParam( "dash_power_rate", & dash_power_rate, RemoteServerParam::instance().dashPowerRate());
	AddParam( "player_size", & player_size, RemoteServerParam::instance().playerSize());
	AddParam( "kickable_margin", & kickable_margin, RemoteServerParam::instance().kickableMargin());
	AddParam( "kick_rand", & kick_rand, RemoteServerParam::instance().kickRand());
	AddParam( "extra_stamina", & extra_stamina, RemoteServerParam::instance().extraStamina());
	AddParam( "effort_max", & effort_max, RemoteServerParam::instance().effortInit());
	AddParam( "effort_min", & effort_min, RemoteServerParam::instance().effortMin());

	//v14
	AddParam( "kick_power_rate", & kick_power_rate, RemoteServerParam::instance().kickPowerRate());
	AddParam( "foul_detect_probability", & foul_detect_probability, RemoteServerParam::instance().foulDetectProbability());
	AddParam( "catchable_area_l_stretch", & catchable_area_l_stretch, 1.0);
}

double HeteroParam::accelerationRateOnDir(const double & dir) const
{
	return GetDashDirRate(dir) * dash_power_rate * effort_max;
}

const double & HeteroParam::accelerationFront(const double & power) const
{
    return acceleration_front[(int)Rint(fabs(power))];
}

const double & HeteroParam::accelerationSide(const double & power) const
{
    return acceleration_side[(int)Rint(fabs(power))];
}

void HeteroParam::MaintainConsistency()
{
	kickable_area = player_size + RemoteServerParam::instance().ballSize() + kickable_margin;

    effective_speed_max = RemoteServerParam::instance().maxDashPower() * effort_max * dash_power_rate / (1 - player_decay); // 实际可能达到的最大速度 -- 对dash模型求解可得
    acceleration_front_rate = effort_max * dash_power_rate;
    acceleration_side_rate = acceleration_front_rate * RemoteServerParam::instance().sideDashRate();
    for (int i = 0; i <= DASH_POWER_NUM; ++i)
    {
        acceleration_front[i] = (double)i * acceleration_front_rate;
        acceleration_side[i] = (double)i * acceleration_side_rate;
    }
	min_catch_area = Sqrt( RemoteServerParam::instance().catchAreaLength() * ( 2.0 - catchable_area_l_stretch ) 
		* RemoteServerParam::instance().catchAreaLength() * ( 2.0 - catchable_area_l_stretch ) + RemoteServerParam::instance().catchAreaWidth() * RemoteServerParam::instance().catchAreaWidth() / 4 );
	max_catch_area = Sqrt( RemoteServerParam::instance().catchAreaLength() * catchable_area_l_stretch
		* RemoteServerParam::instance().catchAreaLength() * catchable_area_l_stretch + RemoteServerParam::instance().catchAreaWidth() * RemoteServerParam::instance().catchAreaWidth() / 4 );
}

RemotePlayerParam* RemotePlayerParam::instance()
{
	return new RemotePlayerParam;
}

RemotePlayerParam::RemotePlayerParam()
	: M_log_dir(std::string(LOG_DIR))
	, mDynamicDebugMode(DYNAMIC_DEBUG_MODE)
	, mSaveServerMessage(SAVE_SERVER_MESSAGE)
	, mSaveSightLog(SAVE_SIGHT_LOG)
	, mSaveDecLog(SAVE_DEC_LOG)
	, mUsePlotter(USE_PLOTTER)
	, mNetworkTest(NETWORK_TEST)
	, mWaitTimeOut(WAIT_TIME_OUT)
	// : mHeteroPlayer(nullptr)
{
	AddParams();
	MaintainConsistency();
}

RemotePlayerParam::~RemotePlayerParam()
{
	// delete[] mHeteroPlayer;
}

int RemotePlayerParam::player_types = DEFAULT_PLAYER_TYPES;
// std::string RemotePlayerParam::M_log_dir = std::string(LOG_DIR);
int RemotePlayerParam::M_our_goalie_unum = 1;
double RemotePlayerParam::M_min_valid_conf = MIN_VALID_CONF;

// bool RemotePlayerParam::mDynamicDebugMode = DYNAMIC_DEBUG_MODE; 
// bool RemotePlayerParam::mSaveServerMessage = SAVE_SERVER_MESSAGE; 
// bool RemotePlayerParam::mSaveSightLog = SAVE_SIGHT_LOG; 
// bool RemotePlayerParam::mSaveDecLog = SAVE_DEC_LOG; 
bool RemotePlayerParam::mSaveTextLog = SAVE_TEXT_LOG;
// bool RemotePlayerParam::mUsePlotter = USE_PLOTTER;
bool RemotePlayerParam::mTimeTest = TIME_TEST;
// bool RemotePlayerParam::mNetworkTest = NETWORK_TEST;
// int RemotePlayerParam::mWaitTimeOut = WAIT_TIME_OUT;
double RemotePlayerParam::mAtPointBuffer = AT_POINT_BUFFER;
int RemotePlayerParam::mKickerMode = KICKER_MODE;
HeteroParam *RemotePlayerParam::mHeteroPlayer = new HeteroParam[DEFAULT_PLAYER_TYPES];


void RemotePlayerParam::AddParams()
{
	AddParam( "player_types", & player_types, DEFAULT_PLAYER_TYPES );
	AddParam( "subs_max", & subs_max, DEFAULT_SUBS_MAX );
	AddParam( "pt_max", & pt_max, DEFAULT_PT_MAX );

	AddParam( "player_speed_max_delta_min", & player_speed_max_delta_min, DEFAULT_PLAYER_SPEED_MAX_DELTA_MIN );
	AddParam( "player_speed_max_delta_max", & player_speed_max_delta_max, DEFAULT_PLAYER_SPEED_MAX_DELTA_MAX );
	AddParam( "stamina_inc_max_delta_factor", & stamina_inc_max_delta_factor, DEFAULT_STAMINA_INC_MAX_DELTA_FACTOR );
	AddParam( "player_decay_delta_min", & player_decay_delta_min, DEFAULT_PLAYER_DECAY_DELTA_MIN );
	AddParam( "player_decay_delta_max", & player_decay_delta_max, DEFAULT_PLAYER_DECAY_DELTA_MAX );
	AddParam( "inertia_moment_delta_factor", & inertia_moment_delta_factor, DEFAULT_INERTIA_MOMENT_DELTA_FACTOR );
	AddParam( "dash_power_rate_delta_min", & dash_power_rate_delta_min, DEFAULT_DASH_POWER_RATE_DELTA_MIN );
	AddParam( "dash_power_rate_delta_max", & dash_power_rate_delta_max, DEFAULT_DASH_POWER_RATE_DELTA_MAX );
	AddParam( "player_size_delta_factor", & player_size_delta_factor, DEFAULT_PLAYER_SIZE_DELTA_FACTOR );
	AddParam( "kickable_margin_delta_min", & kickable_margin_delta_min, DEFAULT_KICKABLE_MARGIN_DELTA_MIN );
	AddParam( "kickable_margin_delta_max", & kickable_margin_delta_max, DEFAULT_KICKABLE_MARGIN_DELTA_MAX );
	AddParam( "kick_rand_delta_factor", & kick_rand_delta_factor, DEFAULT_KICK_RAND_DELTA_FACTOR );
	AddParam( "extra_stamina_delta_min", & extra_stamina_delta_min, DEFAULT_EXTRA_STAMINA_DELTA_MIN );
	AddParam( "extra_stamina_delta_max", & extra_stamina_delta_max, DEFAULT_EXTRA_STAMINA_DELTA_MAX );
	AddParam( "effort_max_delta_factor", & effort_max_delta_factor, DEFAULT_EFFORT_MAX_DELTA_FACTOR );
	AddParam( "effort_min_delta_factor", & effort_min_delta_factor, DEFAULT_EFFORT_MIN_DELTA_FACTOR );
	AddParam( "new_dash_power_rate_delta_min", & new_dash_power_rate_delta_min, DEFAULT_NEW_DASH_POWER_RATE_DELTA_MIN );
	AddParam( "new_dash_power_rate_delta_max", & new_dash_power_rate_delta_max, DEFAULT_NEW_DASH_POWER_RATE_DELTA_MAX );
	AddParam( "new_stamina_inc_max_delta_factor", & new_stamina_inc_max_delta_factor, DEFAULT_NEW_STAMINA_INC_MAX_DELTA_FACTOR );

	// v14
	AddParam( "catchable_area_l_stretch_min", & catchable_area_l_stretch_min, DEFAULT_CATCHABLE_AREA_L_STRETCH_MIN);
	AddParam( "catchable_area_l_stretch_max", & catchable_area_l_stretch_max, DEFAULT_CATCHABLE_AREA_L_STRETCH_MAX);
	AddParam( "kick_power_rate_delta_min", & kick_power_rate_delta_min, DEFAULT_KICK_POWER_RATE_DELTA_MIN);
	AddParam( "kick_power_rate_delta_max", & kick_power_rate_delta_max, DEFAULT_KICK_POWER_RATE_DELTA_MAX);
	AddParam( "foul_detect_probability_delta_factor", & foul_detect_probability_delta_factor,DEFAULT_FOUL_DETECT_PROBABILITY_DELTA_FACTOR);

	AddParam( "allow_mult_default_type", & M_allow_mult_default_type, false );

	AddParam( "player_conf", & M_player_conf_file, std::string(CONFIG_FILE));
	AddParam( "log_dir", & M_log_dir, std::string(LOG_DIR));
	
	AddParam( "trainer_data_file", &M_train_data_file, std::string(TRAIN_DATA_FILE));	
	
	AddParam( "team_name", & M_team_name, std::string(TEAM_NAME));
	AddParam( "opponent_team_name", & M_opponent_team_name, std::string(OPPONENT_TEAM_NAME));
	AddParam( "hetero_test_model", & M_hetero_test_model, std::string(HETERO_TEST_MODEL));

	AddParam( "dynamic_debug_mode", & mDynamicDebugMode, DYNAMIC_DEBUG_MODE );
	AddParam( "save_server_message", & mSaveServerMessage, SAVE_SERVER_MESSAGE );
	AddParam( "save_sight_log", & mSaveSightLog, SAVE_SIGHT_LOG );
	AddParam( "save_dec_log", & mSaveDecLog, SAVE_DEC_LOG );
	AddParam( "save_text_log", & mSaveTextLog, SAVE_TEXT_LOG );
	AddParam( "use_plotter", & mUsePlotter, USE_PLOTTER );
    AddParam( "use_team_graphic", & mUseTeamGraphic, USE_TEAM_GRAPHIC );
    AddParam( "time_test", & mTimeTest, TIME_TEST );
    AddParam( "network_test", & mNetworkTest, NETWORK_TEST );
	AddParam( "wait_sight_buffer", & mWaitSightBuffer, WAIT_SIGHT_BUFFER );
    AddParam( "wait_hear_buffer", & mWaitHearBuffer, WAIT_HEAR_BUFFER );
	AddParam( "wait_time_out", & mWaitTimeOut, WAIT_TIME_OUT );

    AddParam( "tired_buffer", & mTiredBuffer, TIRED_BUFFER );
    AddParam( "at_point_buffer", & mAtPointBuffer, AT_POINT_BUFFER );
    AddParam( "kicker_mode", & mKickerMode, KICKER_MODE );

    AddParam( "our_goalie_unum", & M_our_goalie_unum, 1 );
	AddParam( "goalie", & M_is_goalie, false );
	AddParam( "coach", & M_is_coach, false );
	
	AddParam( "trainer", &M_is_trainer, false);
	
	AddParam( "player_version", & M_player_version, 13.1 );
	AddParam( "coach_version", & M_coach_version, 13.1 );

	AddParam( "say_pos_x_eps", & M_say_pos_x_eps, 0.2);
	AddParam( "say_pos_y_eps", & M_say_pos_y_eps, 0.2);
	AddParam( "say_ball_speed_eps", & M_say_ball_speed_eps, 0.1);
	AddParam( "say_player_speed_eps", & M_say_player_speed_eps, 0.1);
	AddParam( "say_dir_eps", & M_say_dir_eps, 1.0);

	AddParam( "max_conf", & M_max_conf, MAX_CONF);
	AddParam( "min_valid_conf", & M_min_valid_conf, MIN_VALID_CONF);
	AddParam( "conf_decay", & M_conf_decay, CONF_DECAY);
	AddParam( "player_conf_decay", & M_player_conf_decay, PLAYER_CONF_DECAY);
	AddParam( "ball_conf_decay", & M_ball_conf_decay, BALL_CONF_DECAY);

    AddParam( "min_appearance_poss", & M_min_appearance_poss, MIN_APPEARANCE_POSS);

	AddParam( "shoot_max_distance", & shoot_max_distance, MAX_SHOOT_DISTANCE );

    AddParam( "low_stamina_point_thr", & mLowStaminaPointThr, LOW_STAMINA_POINT_THR);
}

void RemotePlayerParam::init(int argc, char **argv)
{
	ParseFromCmdLine(argc, argv);                       //首先分析命令行，因为有可能命令行里面改了配置文件的路径
	ParseFromConfigFile(M_player_conf_file.c_str());    //分析配置文件
	ParseFromCmdLine(argc, argv);                       //再次分析命令行，因为命令行有权更改配置文件里面的默认设置
	MaintainConsistency();

    std::ifstream file("data/eps0.1");

	memset(mSightChange , 0 , sizeof(mSightChange));
    if (!file)  {
        PRINT_ERROR("date/eps0.1 not exist");
        return;
    }

    int i = 0;
    while(!file.eof())
    {
        if (i >= SIGHTSIZE)
        {
         //    PRINT_ERROR("mSightChange not big enough!!!!!!!");
             break;
        }
        file >> mSightChange[i][0] >> mSightChange[i][1] >> mSightChange[i][2];
        i++;
     }
     file.close();




	 std::ifstream file1("data/eps0.01");
	 memset(mMarkChange ,0 , sizeof(mMarkChange));
	 if (!file1)
	 {
		 PRINT_ERROR("file data/eps0.01 not exist");
		 return;
	 }
	 i = 0;
	 while(!file1.eof())
	 {

		 if (i >= MARKSIZE)
		 {
		//	 PRINT_ERROR("mMarkChange not big enough!!!!!!!!");
			 break;
		 }
		 file1 >> mMarkChange[i][0] >> mMarkChange[i][1] >> mMarkChange[i][2];
		 i++;
	 }
	 file1.close();

}

bool RemotePlayerParam::SaveParam()
{
	std::cout << "Saving Player Param To Config File " << M_player_conf_file.c_str() << "..." << std::endl;
	return SaveToConfigFile(M_player_conf_file.c_str());
}

void RemotePlayerParam::AddPlayerType(int type, const char *line)
{
	Assert (type >= 0 && type < RemotePlayerParam.playerTypes());
	if(mHeteroPlayer == nullptr)
	{
		mHeteroPlayer = new HeteroParam[DEFAULT_PLAYER_TYPES];
	}
	mHeteroPlayer[type].ParseFromServerMsg(line);
	mHeteroPlayer[type].MaintainConsistency();
}

void RemotePlayerParam::MaintainConsistency()
{
	M_team_name_len = M_team_name.length();
    mMinStamina = RemoteServerParam::instance().recoverDecStamina() + TiredBuffer();
}

const int & HeteroParam::type() const { return M_type; }

//standard
const double & HeteroParam::playerSpeedMax() const { return player_speed_max; }
const double & HeteroParam::staminaIncMax() const { return stamina_inc_max; }
const double & HeteroParam::playerDecay() const { return player_decay; }
const double & HeteroParam::inertiaMoment() const { return inertia_moment; }
const double & HeteroParam::dashPowerRate() const { return dash_power_rate; }
const double & HeteroParam::playerSize() const { return player_size; }
const double & HeteroParam::kickableMargin() const { return kickable_margin; }
const double & HeteroParam::kickRand() const { return kick_rand; }
const double & HeteroParam::extraStamina() const { return extra_stamina; }
const double & HeteroParam::effortMax() const { return effort_max; }
const double & HeteroParam::effortMin() const { return effort_min; }

//v14
const double & HeteroParam::kickPowerRate() const { return kick_power_rate; }
const double & HeteroParam::catchableAreaLStretch() const { return catchable_area_l_stretch; }
const double & HeteroParam::foulDetectProbability() const { return foul_detect_probability; }

//private
const double & HeteroParam::kickableArea() const { return kickable_area; }
const double & HeteroParam::effectiveSpeedMax() const { return effective_speed_max; }
const double & HeteroParam::minCatchArea() const { return min_catch_area; }
const double & HeteroParam::maxCatchArea() const { return max_catch_area; }

const double & HeteroParam::accelerationFrontRate() const { return acceleration_front_rate; }
const double & HeteroParam::accelerationSideRate() const { return acceleration_side_rate; }
const double & HeteroParam::accelerationFrontMax() const { return acceleration_front[DASH_POWER_NUM]; }
const double & HeteroParam::accelerationSideMax() const { return acceleration_side[DASH_POWER_NUM]; }




int RemotePlayerParam::playerTypes() { return player_types; }
int RemotePlayerParam::subsMax() const { return subs_max; }
int RemotePlayerParam::ptMax() const { return pt_max; }

bool RemotePlayerParam::allowMultDefaultType() const { return M_allow_mult_default_type; }

const double & RemotePlayerParam::maxConf() const { return M_max_conf; }
const double & RemotePlayerParam::minValidConf() { return M_min_valid_conf; } // ???????
const double & RemotePlayerParam::confDecay() const { return M_conf_decay; }
const double & RemotePlayerParam::playerConfDecay() const { return M_player_conf_decay; }
const double & RemotePlayerParam::ballConfDecay() const { return M_ball_conf_decay; }

const double & RemotePlayerParam::playerSpeedMaxDeltaMin() const { return player_speed_max_delta_min; }

const double & RemotePlayerParam::playerSpeedMaxDeltaMax() const { return player_speed_max_delta_max; }
const double & RemotePlayerParam::staminaIncMaxDeltaFactor() const { return stamina_inc_max_delta_factor; }

const double & RemotePlayerParam::playerDecayDeltaMin() const { return player_decay_delta_min; }
const double & RemotePlayerParam::playerDecayDeltaMax() const { return player_decay_delta_max; }
const double & RemotePlayerParam::inertiaMomentDeltaFactor() const { return inertia_moment_delta_factor; }

const double & RemotePlayerParam::dashPowerRateDeltaMin() const { return dash_power_rate_delta_min; }
const double & RemotePlayerParam::dashPowerRateDeltaMax() const { return dash_power_rate_delta_max; }
const double & RemotePlayerParam::playerSizeDeltaFactor() const { return player_size_delta_factor; }

const double & RemotePlayerParam::kickableMarginDeltaMin() const { return kickable_margin_delta_min; }
const double & RemotePlayerParam::kickableMarginDeltaMax() const { return kickable_margin_delta_max; }
const double & RemotePlayerParam::kickRandDeltaFactor() const { return kick_rand_delta_factor; }

const double & RemotePlayerParam::extraStaminaDeltaMin() const { return extra_stamina_delta_min; }
const double & RemotePlayerParam::extraStaminaDeltaMax() const { return extra_stamina_delta_max; }
const double & RemotePlayerParam::effortMaxDeltaFactor() const { return effort_max_delta_factor; }
const double & RemotePlayerParam::effortMinDeltaFactor() const { return effort_min_delta_factor; }

const double & RemotePlayerParam::kickPowerRateDeltaMin() const { return kick_power_rate_delta_min; }
const double & RemotePlayerParam::kickPowerRateDeltaMax() const { return kick_power_rate_delta_max; }
const double & RemotePlayerParam::foulDetectProbabilityDeltaFactor() const { return foul_detect_probability_delta_factor; }

const double & RemotePlayerParam::newDashPowerRateDeltaMin() const { return new_dash_power_rate_delta_min; }
const double & RemotePlayerParam::newDashPowerRateDeltaMax() const { return new_dash_power_rate_delta_max; }
const double & RemotePlayerParam::newStaminaIncMaxDeltaFactor() const { return new_stamina_inc_max_delta_factor; }

const double & RemotePlayerParam::shootMaxDistance() const { return shoot_max_distance; }

const std::string & RemotePlayerParam::logDir() { return M_log_dir; }
const std::string & RemotePlayerParam::teamName() const { return M_team_name; }
void RemotePlayerParam::setTeamName(const char *name) { M_team_name = std::string(name); }
const int & RemotePlayerParam::teamNameLen() const { return M_team_name_len; }
const std::string & RemotePlayerParam::opponentTeamName() const { return M_opponent_team_name; }

const std::string & RemotePlayerParam::trainDataFile() const { return M_train_data_file; }

void RemotePlayerParam::setOpponentTeamName(const char *name) 
{ 
	M_opponent_team_name = std::string(name);
}

const std::string & RemotePlayerParam::heteroTestModel() const { return M_hetero_test_model; }

const int & RemotePlayerParam::ourGoalieUnum() { return M_our_goalie_unum; } //这个量在决策层不应该使用，否则反算对手时会出错
const bool & RemotePlayerParam::isGoalie() const { return M_is_goalie; } //这个量在决策层不应该使用，否则反算对手时会出错
const bool & RemotePlayerParam::isCoach() const { return M_is_coach; }

const bool & RemotePlayerParam::isTrainer() const { return M_is_trainer;}

const double & RemotePlayerParam::playerVersion() const { return M_player_version; }
const double & RemotePlayerParam::coachVersion() const { return M_coach_version; }

const double & RemotePlayerParam::sayPosXEps() const { return M_say_pos_x_eps; }
const double & RemotePlayerParam::sayPosYEps() const { return M_say_pos_y_eps; }
const double & RemotePlayerParam::sayBallSpeedEps() const { return M_say_ball_speed_eps; }
const double & RemotePlayerParam::sayPlayerSpeedEps() const { return M_say_player_speed_eps; }
const double & RemotePlayerParam::sayDirEps() const { return M_say_dir_eps; }



const bool & RemotePlayerParam::DynamicDebugMode() { return mDynamicDebugMode; }
const bool & RemotePlayerParam::ForcePenaltyMode() const { return mForcePenaltyMode; }
const bool & RemotePlayerParam::SaveServerMessage() { return mSaveServerMessage; }
const bool & RemotePlayerParam::SaveSightLog() { return mSaveSightLog; }
const bool & RemotePlayerParam::SaveDecLog() { return mSaveDecLog; }
const bool & RemotePlayerParam::SaveTextLog() { return mSaveTextLog; }
const bool & RemotePlayerParam::TimeTest() { return mTimeTest; }
const bool & RemotePlayerParam::NetworkTest() { return mNetworkTest; }
const bool & RemotePlayerParam::UsePlotter() { return mUsePlotter; }
const bool & RemotePlayerParam::UseTeamGraphic() const { return mUseTeamGraphic; }
const int & RemotePlayerParam::WaitSightBuffer() const { return mWaitSightBuffer; }
const int & RemotePlayerParam::WaitHearBuffer() const { return mWaitHearBuffer; }
const int & RemotePlayerParam::WaitTimeOut() { return mWaitTimeOut; }

const double & RemotePlayerParam::minAppearancePoss() const { return M_min_appearance_poss; }

const double & RemotePlayerParam::TiredBuffer() const { return mTiredBuffer; }
const double & RemotePlayerParam::MinStamina() const { return mMinStamina; }
const double & RemotePlayerParam::AtPointBuffer() { return mAtPointBuffer; } // ????
const int & RemotePlayerParam::KickerMode() { return mKickerMode; } // ????

const double & RemotePlayerParam::LowStaminaPointThr() const { return mLowStaminaPointThr; }
