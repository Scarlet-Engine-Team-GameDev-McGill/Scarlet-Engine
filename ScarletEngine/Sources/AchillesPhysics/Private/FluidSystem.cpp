#include "FluidSystem.h"

namespace ScarletEngine::Achilles
{
	// https://github.com/bwiberg/
	// http://mmacklin.com/pbf_sig_preprint.pdf
	// https://matthias-research.github.io/pages/publications/sca03.pdf

	float  FluidSystem::EuclidianDistance2(glm::vec3 v) const
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}

	float FluidSystem::Poly6Kernel(glm::vec3 Radius, float H) const
	{
		float Dist2 = EuclidianDistance2(Radius);
		if (Dist2 <= H * H && Dist2 > 0)
		{
			return (315.f / (64.f * 3.1415269f * glm::pow(H, 9.f))) * glm::pow((H * H - Dist2), 3.f);
		}
		return 0.f;
	}

	glm::vec3 FluidSystem::GradSpikyKernel(glm::vec3 Radius, float H) const
	{
		float Dist2 = EuclidianDistance2(Radius);
		if (Dist2 <= H * H && Dist2 > 0)
		{
			float Dist = glm::sqrt(Dist2);
			float Constant = -(15.f / (3.1415269f * glm::pow(H, 6.f))) * 3.f * glm::pow(H - Dist, 2.f) / Dist;
			return Constant * Radius;
		}
		return glm::vec3(0.f);
	}

	void FluidSystem::FixedUpdate() const
	{
		const auto& Entities = GetEntities<Transform, FluidComponent, RigidBodyComponent>();
		const size_t Size = Entities.size();

		if (Size == 0)
		{
			return;
		}

		// Update current densities 
		for (size_t i = 0; i < Size; i++)
		{
			const auto& [Entity, Trans, Fluid, Rb] = Entities[i];
			float CurrentDensity = 0;

			for (size_t j = 0; j < Size; j++)
			{
				const auto& [EntityJ, TransJ, FluidJ, RBJ] = Entities[j];
				glm::vec3 Radius = Trans->Position - TransJ->Position;

				if (glm::abs(glm::length(Radius)) <= Fluid->DistanceView)
				{
					CurrentDensity += Poly6Kernel(Radius, Fluid->H);	// assume masses are equal, drop the term
				}
			}

			Fluid->CurrentDensity = CurrentDensity;
		}


		// Calculate lambda
		for (size_t i = 0; i < Size; i++)
		{
			const auto& [Entity, Trans, Fluid, Rb] = Entities[i];
			float GradDensityConstraint = 0.f;

			for (size_t j = 0; j < Size; j++)
			{
				const auto& [EntityJ, TransJ, FluidJ, RBJ] = Entities[j];
				glm::vec3 Radius = Trans->Position - TransJ->Position;

				if (glm::abs(glm::length(Radius)) <= Fluid->DistanceView)
				{
					GradDensityConstraint += EuclidianDistance2(GradSpikyKernel(Radius, Fluid->H));
				}
			}

			GradDensityConstraint /= Fluid->Density;
			float DensityConstraint = Fluid->CurrentDensity / Fluid->Density - 1.f;
			Fluid->Lambda = -DensityConstraint / (GradDensityConstraint + 0.001f);
		}

		// Calculate position change

		for (size_t i = 0; i < Size - 1; i++)
		{
			const auto& [Entity, Trans, Fluid, Rb] = Entities[i];
			glm::vec3 DP = glm::vec3(0.f);

			for (size_t j = i + 1; j < Size; j++)
			{
				const auto& [EntityJ, TransJ, FluidJ, RBJ] = Entities[j];
				glm::vec3 Radius = Trans->Position - TransJ->Position;
				if (glm::abs(glm::length(Radius)) <= Fluid->DistanceView)
				{
					DP += (Fluid->Lambda + FluidJ->Lambda) * GradSpikyKernel(Radius, Fluid->H);
				}
			}			

			DP /= Fluid->Density;
			Trans->Position += DP;
		}
	}
}