#pragma once

// BASE CLASS FOR ALL THE COMPONENTS 
class Component
{
public:

	// Having an enum avoid doing some dynamic cast to check types 
	enum ComponentType { TransformCP, RenderCP, TextCP, FPSCP};

	Component(ComponentType type);
	virtual ~Component() = default;

	Component(const Component& other) = delete;
	Component(Component&& other) = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) = delete;

	virtual void Update([[maybe_unused]] const float deltaTime) = 0;

	const ComponentType Type() const;

private:

	ComponentType m_ComponentType;
};

