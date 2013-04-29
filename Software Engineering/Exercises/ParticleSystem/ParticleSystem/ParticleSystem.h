#pragma once

#include <list>

#include <gazengine/gdi/model/GDIMesh.h>
#include <gazengine/math/Vector2.h>

#include "ObjectPool.h"

class CParticleSystem : public Model
{
	public:
		CParticleSystem(unsigned int _uiParticleCount, float _fAverageLifetime, float _fAverageSpeed,
				const Vector2& _vPosition);

		void advance(float _fDeltaTime);

		Texture* getNormalMap() const;

		PrimitiveType getPrimitiveType() const;

		Texture* getTexture() const;

		void render(Renderer& renderer) const;

		void setNormalMap(Texture* texture);

		void setTexture(Texture* texture);

	private:
		struct Particle
		{
			Particle() :
				age(0.0f),
				birthTime(0.0f),
				lifetime(0.0f),
				mesh(std::vector<Vector2>(), RGB(255, 0, 0)),
				velocity()
			{
				mesh.getVertices().resize(4);
			}

			float age;

			float birthTime;

			float lifetime;

			GDIMesh mesh;

			Vector2 velocity;
		};

		float m_fAverageLifetime;

		float m_fAverageSpeed;

		float m_fTime;

		CObjectPool<Particle> m_particlePool;

		std::list<Particle*> m_particles;

		Vector2 m_vPosition;

		void addParticle();
};
