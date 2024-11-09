#pragma once
#include <Component.h>
#include "Observer.h"
#include <string>

class TextComponent;
class PlayerScoreCP final : public engine::Component, public engine::Observer
{
public:
	PlayerScoreCP(engine::GameObject* pOwner, unsigned int playerIdx);
	~PlayerScoreCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void OnNotify(engine::GameObject* gameObject, const engine::Event& event) override;

	std::string GetRatio() const;
	std::string GetHits() const;
	std::string GetShots() const;

private:
	 
	void UpdateScore(int points);
	TextComponent* m_pTextComponent;
	int m_PlayerHits;
	int m_PlayerShots;

};

