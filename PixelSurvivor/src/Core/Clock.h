#pragma once
#include <chrono>

namespace Pixel
{

	class Clock
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
		float m_ElaspedTime = 1.0f / 60.0f;

	public:
		inline float GetElapsedSeconds() const { return m_ElaspedTime; }

		inline void StartFrame()
		{
			m_StartTime = std::chrono::high_resolution_clock::now();
		}

		inline void EndFrame()
		{
			auto now = std::chrono::high_resolution_clock::now();
			m_ElaspedTime = std::chrono::duration_cast<std::chrono::microseconds>(now - m_StartTime).count() / 1e6f;
		}
	};

}
