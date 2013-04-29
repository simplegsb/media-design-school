#pragma once

#include <vector>

#include <gazengine/math/Vector2.h>
#include <gazengine/model/Circle.h>

class CFlockingAgent
{
	public:
		CFlockingAgent(const std::vector<CFlockingAgent*>& _flock);

		Vector2 GetHeading() const;

		Circle* GetModel();

		Vector2 GetPosition() const;

		void SetHeading(const Vector2& _heading);

		void SetModel(Circle* _model);

		void SetPosition(const Vector2& _position);

		void Update(float _fTimeDelta);

	private:
		const std::vector<CFlockingAgent*>& m_flock;

		Vector2 m_heading;

		Circle* m_model;

		Vector2 m_position;

		CFlockingAgent(const CFlockingAgent& _original);

		std::vector<CFlockingAgent*> GetAgentsInRange(float _fRange) const;

		Vector2 GetAlignmentEffect() const;

		Vector2 GetCohesionEffect() const;

		Vector2 GetSeparationEffect() const;

		CFlockingAgent& operator=(const CFlockingAgent& _original);
};
