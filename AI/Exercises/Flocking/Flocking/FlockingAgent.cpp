#include "FlockingAgent.h"

using namespace std;

CFlockingAgent::CFlockingAgent(const vector<CFlockingAgent*>& _flock) :
	m_flock(_flock),
	m_model(NULL),
	m_position()
{
}

vector<CFlockingAgent*> CFlockingAgent::GetAgentsInRange(float _fRange) const
{
	vector<CFlockingAgent*> closeAgents;

	for (unsigned int uiIndex = 0; uiIndex < m_flock.size(); uiIndex++)
	{
		CFlockingAgent* agent = m_flock[uiIndex];

		if (agent != this &&
			agent->GetPosition().proximity(m_position) < _fRange)
		{
			closeAgents.push_back(agent);
		}
	}

	return closeAgents;
}

Vector2 CFlockingAgent::GetAlignmentEffect() const
{
	vector<CFlockingAgent*> closeAgents = GetAgentsInRange(200.0f);
	Vector2 averageHeading;

	for (unsigned int uiIndex = 0; uiIndex < closeAgents.size(); uiIndex++)
	{
		averageHeading += closeAgents[uiIndex]->GetHeading() *
			(200.0f - closeAgents[uiIndex]->GetPosition().proximity(m_position));
	}

	averageHeading /= (float) closeAgents.size();
	averageHeading.normalise();

	return averageHeading;
}

Vector2 CFlockingAgent::GetCohesionEffect() const
{
	vector<CFlockingAgent*> closeAgents = GetAgentsInRange(200.0f);
	Vector2 averagePosition;

	for (unsigned int uiIndex = 0; uiIndex < closeAgents.size(); uiIndex++)
	{
		averagePosition += closeAgents[uiIndex]->GetPosition() *
			(200.0f - closeAgents[uiIndex]->GetPosition().proximity(m_position));
	}

	averagePosition /= (float) closeAgents.size();

	Vector2 toAveragePosition = averagePosition - m_position;
	toAveragePosition.normalise();

	return toAveragePosition;
}

Vector2 CFlockingAgent::GetHeading() const
{
	return m_heading;
}

Circle* CFlockingAgent::GetModel()
{
	return m_model;
}

Vector2 CFlockingAgent::GetPosition() const
{
	return m_position;
}

Vector2 CFlockingAgent::GetSeparationEffect() const
{
	vector<CFlockingAgent*> closeAgents = GetAgentsInRange(50.0f);
	Vector2 separationEffect;

	for (unsigned int uiIndex = 0; uiIndex < closeAgents.size(); uiIndex++)
	{
		Vector2 toCloseAgent = closeAgents[uiIndex]->GetPosition() - m_position;

		float fFractionOfRepulsionZone = 1.0f - (toCloseAgent.length() / 50.0f);
		toCloseAgent *= fFractionOfRepulsionZone;

		separationEffect += toCloseAgent * -1.0f;
	}

	if (!closeAgents.empty())
	{
		separationEffect.normalise();
	}

	return separationEffect;
}

void CFlockingAgent::SetHeading(const Vector2& _heading)
{
	m_heading = _heading;
}

void CFlockingAgent::SetModel(Circle* _model)
{
	m_model = _model;
}

void CFlockingAgent::SetPosition(const Vector2& _position)
{
	m_position = _position;
}

void CFlockingAgent::Update(float _fTimeDelta)
{
	m_heading = Vector2();
	m_heading += GetAlignmentEffect();
	m_heading += GetCohesionEffect();
	m_heading += GetSeparationEffect();

	// Stop them from leaving!
	Vector2 toOrigin = Vector2(400.0f, 300.0f) - m_position;
	if (toOrigin.length() > 200.0f)
	{
		m_heading += toOrigin;
	}
	
	m_heading.normalise();

	m_position += m_heading * 100.0f * _fTimeDelta;
	m_model->setPosition(m_position);
}
