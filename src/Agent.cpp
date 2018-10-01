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

#include <cstdlib>
#include "Agent.h"
#include "WorldModel.h"

/**
 * Constructor.
 */
Agent::Agent(Unum unum, WorldModel *world_model, bool reverse, RemotePlayerParam* playerParam):
	mSelfUnum( abs(unum) ),
	mReverse( reverse ),
	mpWorldModel( world_model ),
	mpWorldState( &(world_model->World(reverse)) ),
	mpInfoState( new InfoState( mpWorldState)),
	mIsNewSight (false),
	mpStrategy(0),
	mpAnalyser(0),
    mpActionEffector(0),
    mpFormation(0),
    m_playerParam(playerParam)
{
}

/**
 * Destructor.
 */
Agent::~Agent()
{
	SetHistoryActiveBehaviors();

	for (int type = BT_None + 1; type < BT_Max; ++type) {
		delete mLastActiveBehavior[type];
	}

	delete mpInfoState;
    delete mpFormation;
	delete mpActionEffector;
	delete mpStrategy;
	delete mpAnalyser;
}

/**
 * Interface to create an agent which represents a team mate.
 * \param unum positive number represents the uniform number of the team mate.
 * \return an agent created by "new" operator, which should be manually deleted when there will be no
 *         use any more.
 */
Agent * Agent::CreateTeammateAgent(Unum unum, RemotePlayerParam* playerParam) ///反算队友
{
    Assert(unum != 0);
    return new Agent(unum, mpWorldModel, mReverse, playerParam); //reverse属性不变
}

/**
 * Interface to create an agent which represents an opponent.
 * \param unum positive number represents the uniform number of the opponent.
 * \return an agent created by "new" operator, which should be manually deleted when there will be no
 *         use any more.
 */
Agent * Agent::CreateOpponentAgent(Unum unum) ///反算对手
{
	return new Agent(unum, mpWorldModel, !mReverse, nullptr); //reverse属性相反
}


void Agent::SaveActiveBehavior(const ActiveBehavior & beh)
{
	BehaviorType type = beh.GetType();

	Assert(type > BT_None && type < BT_Max);

	if (mActiveBehavior[type] != 0) {
		if (*mActiveBehavior[type] < beh) {
			delete mActiveBehavior[type];
			mActiveBehavior[type] = new ActiveBehavior(beh);
		}
	}
	else {
		mActiveBehavior[type] = new ActiveBehavior(beh);
	}
}

ActiveBehavior *Agent::GetLastActiveBehavior(BehaviorType type) const
{
	Assert(type > BT_None && type < BT_Max);

	return mLastActiveBehavior[type];
}

void Agent::SetActiveBehaviorInAct(BehaviorType type)
{
	Assert(type > BT_None && type < BT_Max);
	Assert(mActiveBehavior[0] == 0);

	mActiveBehavior[0] = mActiveBehavior[type];
}

void Agent::SaveActiveBehaviorList(const std::list<ActiveBehavior> & behavior_list)
{
	for (std::list<ActiveBehavior>::const_iterator it = behavior_list.begin(); it != behavior_list.end(); ++it) {
		SaveActiveBehavior(*it);
	}
}

void Agent::SetHistoryActiveBehaviors()
{
    for (int type = BT_None + 1; type < BT_Max; ++type) {
        delete mLastActiveBehavior[type];

        mLastActiveBehavior[type] = mActiveBehavior[type];
        mActiveBehavior[type] = 0;
    }

    mLastActiveBehavior[0] = mActiveBehavior[0];
    mActiveBehavior[0] = 0;
}

/**
 * Check commands sent to server, based on ActionEffector::CheckCommands. Will update IsNewSight and BallSeenTime.
 */
void Agent::CheckCommands(Observer *observer) 
{ 
	mIsNewSight = observer->IsNewSight();
	mBallSeenTime = observer->Ball().GetDist().time(); 
	GetActionEffector().CheckCommands(observer); 
}

const RemotePlayerState & Agent::GetSelf() const {
	if (!mSelfUnum || mSelfUnum == TRAINER_UNUM) {
		static std::map<RemotePlayerParam*, RemotePlayerState*> coach_map;
		if(coach_map[m_playerParam] == 0)
		{
			coach_map[m_playerParam] = new RemotePlayerState(m_playerParam); //dummy player state for coach
		}
		return *coach_map[m_playerParam];
	}
	RemotePlayerState& state = const_cast<RemotePlayerState&>(GetWorldState().GetTeammate(mSelfUnum));
	state.SetPlayerParam(m_playerParam);
	return const_cast<RemotePlayerState&>(state);
}

RemotePlayerState & Agent::Self() {
	if (!mSelfUnum || mSelfUnum == TRAINER_UNUM) {
		static std::map<RemotePlayerParam*, RemotePlayerState*> coach_map;
		if(coach_map[m_playerParam] == 0)
		{
			coach_map[m_playerParam] = new RemotePlayerState(m_playerParam); //dummy player state for coach
		}
		return *coach_map[m_playerParam];
	}

	RemotePlayerState& state = World().Teammate(mSelfUnum);
	state.SetPlayerParam(m_playerParam);
	return state;
}

/**
 * Interface to create an agent which represents a team mate.
 */
Agent * Agent::CreateTeammateAgent(Unum unum, RemotePlayerParam* playerParam); ///反算队友

/**
 * Interface to create an agent which represents an opponent.
 */
Agent * Agent::CreateOpponentAgent(Unum unum/*no params*/); ///反算对手

/**
 * Interfaces to get the agent's world state.
 */
WorldState       & Agent::World() { return *mpWorldState; }
const WorldState & Agent::GetWorldState() const { return *mpWorldState; }

/**
 * Interfaces to get the agent's info state.
 */
InfoState        & Agent::Info() {	return *mpInfoState; }
const InfoState  & Agent::GetInfoState() const { return *mpInfoState; }

/*
 * 自己相关的接口
 * Interfaces to get information about the agent it self.
 */
AgentID             Agent::GetAgentID() const { return AgentID(mSelfUnum, GetWorldState().CurrentTime(), mReverse); }
Unum                Agent::GetSelfUnum() const { return mSelfUnum; }

Vector Agent::GetSelfPosWithQueuedActions() { return GetActionEffector().GetSelfPosWithQueuedActions(); }
Vector Agent::GetSelfVelWithQueuedActions() { return GetActionEffector().GetSelfVelWithQueuedActions(); }
AngleDeg Agent::GetSelfBodyDirWithQueuedActions() { return GetActionEffector().GetSelfBodyDirWithQueuedActions(); }
Vector Agent::GetBallPosWithQueuedActions() { return GetActionEffector().GetBallPosWithQueuedActions(); }
Vector Agent::GetBallVelWithQueuedActions() { return GetActionEffector().GetBallVelWithQueuedActions(); }