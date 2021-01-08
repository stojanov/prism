#pragma once

namespace glgf::Core
{
	class State
	{
		State();
		virtual ~State() = default;
		virtual void Init() = 0;

		virtual void Pause() = 0;
		virtual void Resume() = 0;

		void Update(float dt) override;
		void Draw(float dt) override;
	protected:

	};
}