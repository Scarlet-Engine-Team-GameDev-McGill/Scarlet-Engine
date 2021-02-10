#pragma once

class Ac_RigidBodyComponent {

	// Fields
	float mass;
	bool uses_gravity;

	inline
	public void SetMass(const& float aMass){ this->mass = aMass }
};