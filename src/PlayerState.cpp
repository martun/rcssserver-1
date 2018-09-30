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

#include "PlayerState.h"
#include "ActionEffector.h"
#include "Simulator.h"
#include "Dasher.h"

RemotePlayerState::RemotePlayerState(RemotePlayerParam* playerParams)
	: MobileState(RemoteServerParam::instance().playerDecay(), RemotePlayerParam::HeteroPlayer(0).effectiveSpeedMax())
{
	m_playerParams = playerParams;

	mBallCatchable = false;
	mCatchBan = 0;
	mCollideWithBall = false;
	mCollideWithPlayer = false;
	mCollideWithPost = false;
	mIsAlive = false;
	mIsGoalie = false;
    mIsSensed = false;
	mIsKicked = false;
	mIsPointing = false;
	mKickRate = 0.0;
	mIsKickable = false;
	mMaxTurnAngle = 0.0;
	mPlayerType = 0;

	mStamina = RemoteServerParam::instance().staminaMax();
	mEffort  = RemoteServerParam::instance().effortInit();
	mCapacity = RemoteServerParam::instance().staminaCapacity();
	mRecovery = RemoteServerParam::instance().recoverInit();

	mTackleBan = 0;
	mUnderDangerousTackleCondition = false;

	mFoulChargedCycle = 0;

	mUnum = 0;

	mViewWidth = VW_Normal;
	mIsTired = false;
	if(m_playerParams)
	    mMinStamina = GetPlayerParam()->MinStamina();

	mCardType = CR_None;
	mIsBodyDirMayChanged = true;
}

bool RemotePlayerState::IsKickable(const BallState & ball_state, const double & buffer) const
{
	if (ball_state.GetPosConf() < FLOAT_EPS || GetPosConf() < FLOAT_EPS) return false;
	double dis2self = GetPos().Dist(ball_state.GetPos());
	return (dis2self <= GetKickableArea() - buffer);
}

void RemotePlayerState::GetReverseFrom(const RemotePlayerState & o)
{
	SetIsAlive(o.IsAlive());

	UpdateIsGoalie(o.IsGoalie());
	UpdateIsSensed(o.IsSensed());

	UpdatePlayerType(o.GetPlayerType());
	UpdateViewWidth(o.GetViewWidth());

	UpdateBallCatchable(o.IsBallCatchable());
	UpdateCatchBan(o.GetCatchBan());

	UpdateKickable(o.IsKickable());
	UpdateKicked(o.IsKicked());
	UpdateCardType(o.GetCardType());

	UpdateStamina(o.GetStamina());
	UpdateEffort(o.GetEffort());
	UpdateCapacity(o.GetCapacity());
	UpdateRecovery(o.GetRecovery());

	UpdateMaxTurnAngle(o.GetMaxTurnAngle());

	UpdateBodyDir(GetNormalizeAngleDeg(o.GetBodyDir() + 180.0), o.GetBodyDirDelay(), o.GetBodyDirConf());
	UpdateNeckDir(GetNormalizeAngleDeg(o.GetNeckDir() + 180.0), o.GetNeckDirDelay(), o.GetNeckDirConf());

	UpdatePos(o.GetPos().Rotate(180.0), o.GetPosDelay(), o.GetPosConf());
	UpdateVel(o.GetVel().Rotate(180.0), o.GetVelDelay(), o.GetVelConf());

	UpdateTackleBan(o.GetTackleBan());
	UpdateTackleProb(o.GetTackleProb(false), false);
	UpdateTackleProb(o.GetTackleProb(true), true);
	UpdateDangerousTackleCondition(o.UnderDangerousTackleCondition());

	UpdateFoulChargedCycle(o.GetFoulChargedCycle());

	UpdateArmPoint(GetNormalizeAngleDeg(o.GetArmPointDir() + 180.0), o.GetArmPointDelay(), o.GetArmPointConf(), o.GetArmPointDist(), o.GetArmPointMovableBan(), o.GetArmPointExpireBan());
	UpdateFocusOn(o.GetFocusOnSide(), o.GetFocusOnUnum(), o.GetFocusOnDelay(), o.GetFocusOnConf());
}

Vector RemotePlayerState::GetPredictedPosWithDash(int steps, double dash_power, AngleDeg dash_dir) const
{
	Simulator::Player self(*this);
	int dash_dir_idx = dasher.GetDashDirIdx(dash_dir);

	for (int i = 0; i < steps; ++i) {
		self.Dash(dash_power, dash_dir_idx);
	}

	return self.mPos;
}

Vector RemotePlayerState::GetPredictedVelWithDash(int steps, double dash_power, AngleDeg dash_dir) const
{
	Simulator::Player self(*this);
	int dash_dir_idx = Dasher::GetDashDirIdx(dash_dir);

	for (int i = 0; i < steps; ++i) {
		self.Dash(dash_power, dash_dir_idx);
	}

	return self.mVel;
}

double RemotePlayerState::CorrectDashPowerForStamina(double dash_power) const
{
	double stamina = GetStamina();

	if (IsOutOfStamina()) {
		return dash_power; //已经没有容量了，不需要在控制了
	}

	double new_power;

	if (dash_power >= 0) {
		new_power = Min( dash_power, stamina - GetPlayerParam()->MinStamina() );
		if ( new_power < 0 ) new_power = 0;
	}
	else {
		new_power = Min( -dash_power, (stamina - GetPlayerParam()->MinStamina()) / 2.0);
		if ( new_power < 0 ) new_power = 0;

		new_power = -new_power;
	}

	return new_power;
}

AngleDeg RemotePlayerState::GetEffectiveTurn(AngleDeg moment, double my_speed) const
{
	return GetTurnAngle(moment, GetPlayerType(), my_speed);
}

AngleDeg RemotePlayerState::GetRandAngle( const double & power ,const  double & vel , const BallState & bs ) const
{

    double dir_diff = fabs((bs.GetPos()-GetPos()).Dir() - GetBodyDir());
    Vector tmp = bs.GetPos() - GetPos();
    double dist_ball = ( tmp.Mod() - GetPlayerSize() -
                         - RemoteServerParam::instance().ballSize());

	double pos_rate
	            = 0.5
	            + 0.25 * ( dir_diff/180 + dist_ball/GetKickableMargin());
	        // [0.5, 1.0]
	        double speed_rate
	            = 0.5
	            + 0.5 * ( bs.GetVel().Mod()
	                      / ( RemoteServerParam::instance().ballSpeedMax()
	                          * RemoteServerParam::instance().ballDecay() ) );
	        // [0, 2*kick_rand]
	        double max_rand
	            = GetKickRand()
	            * ( power / RemoteServerParam::instance().maxPower() )
	            * ( pos_rate + speed_rate );

	  return RemoteRad2Deg(max_rand/vel);
}

double RemotePlayerState::GetControlBallProb(const Vector & ball_pos) const
{
	const double dist = GetPos().Dist(ball_pos);

	if (dist < GetKickableArea()) {
		return 1.0; //已经可踢了
	}

	if (IsGoalie()) {
		return Max(GetCatchProb(dist), ::GetTackleProb(ball_pos, GetPos(), GetBodyDir(), false));
	}
	else {
		return ::GetTackleProb(ball_pos, GetPos(), GetBodyDir(), false);
	}
}

double RemotePlayerState::GetCatchProb( const double & dist ) const
{
	if ( dist < GetMinCatchArea() ) {
		return RemoteServerParam::instance().catchProb();
	}
	else {
		double min_length = RemoteServerParam::instance().catchAreaLength() * ( 2.0 - GetCatchAreaLStretch() );
		double max_length = RemoteServerParam::instance().catchAreaLength() * GetCatchAreaLStretch();
		if ( max_length < min_length + FLOAT_EPS ) return 0.0;
		double delt = Sqrt( dist * dist - RemoteServerParam::instance().catchAreaWidth() * RemoteServerParam::instance().catchAreaWidth() / 4 );
		if ( delt > max_length ) return 0.0;
		double dx = delt - min_length;
		return MinMax( 0.0, RemoteServerParam::instance().catchProb() - RemoteServerParam::instance().catchProb() * dx / ( max_length - min_length ), RemoteServerParam::instance().catchProb());
	}
}

//make to be inlined
inline void RemotePlayerState::UpdateStamina(double stamina)
{
	mStamina = stamina;
}

inline void RemotePlayerState::UpdateEffort(double effort)
{
	mEffort = effort;
}

inline void RemotePlayerState::UpdateCapacity(double capacity)
{
	mCapacity = capacity;
}

inline void RemotePlayerState::UpdateNeckDir(double dir, int delay, double conf)
{
	mNeckDir.mValue = GetNormalizeAngleDeg(dir);
	mNeckDir.mCycleDelay = delay;
	mNeckDir.mConf  = conf;
}

inline void RemotePlayerState::UpdateBodyDir(double dir, int delay, double conf)
{
	mBodyDir.mValue = GetNormalizeAngleDeg(dir);
	mBodyDir.mCycleDelay = delay;
	mBodyDir.mConf = conf;
}

inline void RemotePlayerState::UpdateTackleBan(int ban)
{
	mTackleBan = ban;
}

inline void RemotePlayerState::UpdateCatchBan(int ban)
{
	mCatchBan = ban;
}

inline void RemotePlayerState::UpdateArmPoint(AngleDeg dir, int delay, double conf, double dist, int move_ban , int expire_ban )
{
	ArmPoint arm;
	arm.mTargetDir = GetNormalizeAngleDeg(dir);
	arm.mTargetDist = dist;
	arm.mMovableBan = move_ban;
	arm.mExpireBan  = expire_ban;

	mArmPoint.mValue = arm;
	mArmPoint.mCycleDelay = delay;
	mArmPoint.mConf  = conf;
}

inline void RemotePlayerState::UpdateFocusOn(char side, Unum num, int delay, double conf)
{
	FocusOn focus;
	focus.mFocusSide = side;
	focus.mFocusNum  = num;

	mFocusOn.mValue = focus;
	mFocusOn.mCycleDelay = delay;
	mFocusOn.mConf  = conf;
}

inline void RemotePlayerState::UpdateKicked(bool is_kicked)
{
	mIsKicked = is_kicked;
}

inline void RemotePlayerState::AutoUpdate(int delay_add, double conf_dec_factor)
{
	MobileState::AutoUpdate(delay_add , conf_dec_factor);

	mNeckDir.AutoUpdate(delay_add , conf_dec_factor);
	mBodyDir.AutoUpdate(delay_add , conf_dec_factor);
	mArmPoint.AutoUpdate(delay_add , conf_dec_factor);
	mFocusOn.AutoUpdate(delay_add , conf_dec_factor);

	mCollideWithPost = false;
	mCollideWithPlayer = false;
	mCollideWithBall = false;
	mIsKicked    = false;
	mIsTired     = false;

	if (mTackleBan > 0) {
		mTackleBan -= delay_add;
		mTackleBan = Max(mTackleBan, 0);
	}

	if (mFoulChargedCycle > 0) {
		mFoulChargedCycle -= delay_add;
		mFoulChargedCycle = Max(mFoulChargedCycle, 0);
	}

	if (mCatchBan > 0) {
		mCatchBan -= delay_add;
		mCatchBan = Max(mCatchBan, 0);
	}

	if (mArmPoint.mValue.mExpireBan > 0) {
		mArmPoint.mValue.mExpireBan -= delay_add;
		mArmPoint.mValue.mExpireBan = Max(mArmPoint.mValue.mExpireBan, 0);
	}

	if (mArmPoint.mValue.mMovableBan > 0) {
		mArmPoint.mValue.mMovableBan -= delay_add;
		mArmPoint.mValue.mMovableBan = Max(mArmPoint.mValue.mMovableBan, 0);
	}

	if (GetPosDelay() > delay_add) {
		mIsBodyDirMayChanged = true;
	}
}

inline void RemotePlayerState::UpdatePlayerType(int type)
{
	if (mPlayerType != type) {
		mPlayerType = type;

		SetDecay(RemotePlayerParam::HeteroPlayer(mPlayerType).playerDecay());
		SetEffectiveSpeedMax(RemotePlayerParam::HeteroPlayer(mPlayerType).effectiveSpeedMax());
	}
}

inline void RemotePlayerState::UpdateViewWidth(ViewWidth width)
{
	mViewWidth = width;
}

inline void RemotePlayerState::UpdateUnum(Unum num)
{
	mUnum = num;
}

inline void RemotePlayerState::SetIsAlive(bool alive)
{
	mIsAlive = alive;

	if (!mIsAlive) {
		UpdatePos(GetPos(), GetPosDelay(), 0.0);
		UpdateVel(GetVel(), GetVelDelay(), 0.0);

		mNeckDir.mConf = 0.0;
		mBodyDir.mConf = 0.0;
	}
}
//end of file pLayerState.cpp


