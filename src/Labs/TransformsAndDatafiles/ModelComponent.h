#pragma once
#include "Model.h"
#include "Component.h"

class ModelComponent : public Component {
public:
	void OnUpdate(float dt) override {}

	void OnMessage(const std::string n) override {}

	ModelComponent();

	ModelComponent(Model*mymodel) {
		themodel = mymodel;
	}

	Model* themodel;
};


