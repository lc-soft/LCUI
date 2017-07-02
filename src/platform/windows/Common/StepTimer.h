#pragma once

#include <wrl.h>

namespace DX
{
	// 动画和模拟计时的帮助程序类。
	class StepTimer
	{
	public:
		StepTimer() : 
			m_elapsedTicks(0),
			m_totalTicks(0),
			m_leftOverTicks(0),
			m_frameCount(0),
			m_framesPerSecond(0),
			m_framesThisSecond(0),
			m_qpcSecondCounter(0),
			m_isFixedTimeStep(false),
			m_targetElapsedTicks(TicksPerSecond / 60)
		{
			if (!QueryPerformanceFrequency(&m_qpcFrequency))
			{
				throw ref new Platform::FailureException();
			}

			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw ref new Platform::FailureException();
			}

			// 将最大增量初始化为 1/10 秒。
			m_qpcMaxDelta = m_qpcFrequency.QuadPart / 10;
		}

		// 获取上一次 Update 调用后的运行时间。
		uint64 GetElapsedTicks() const						{ return m_elapsedTicks; }
		double GetElapsedSeconds() const					{ return TicksToSeconds(m_elapsedTicks); }

		// 获取程序启动之后的总时间。
		uint64 GetTotalTicks() const						{ return m_totalTicks; }
		double GetTotalSeconds() const						{ return TicksToSeconds(m_totalTicks); }

		// 获取自程序启动之后的更新次数。
		uint32 GetFrameCount() const						{ return m_frameCount; }

		// 获取当前帧速率。
		uint32 GetFramesPerSecond() const					{ return m_framesPerSecond; }

		// 设置是使用固定的还是可变的时间步长模式。
		void SetFixedTimeStep(bool isFixedTimestep)			{ m_isFixedTimeStep = isFixedTimestep; }

		// 当使用固定时间步长模式时，设置调用 Update 的频率。
		void SetTargetElapsedTicks(uint64 targetElapsed)	{ m_targetElapsedTicks = targetElapsed; }
		void SetTargetElapsedSeconds(double targetElapsed)	{ m_targetElapsedTicks = SecondsToTicks(targetElapsed); }

		// 整数格式使用每秒 10,000,000 次滴答来表示时间。
		static const uint64 TicksPerSecond = 10000000;

		static double TicksToSeconds(uint64 ticks)			{ return static_cast<double>(ticks) / TicksPerSecond; }
		static uint64 SecondsToTicks(double seconds)		{ return static_cast<uint64>(seconds * TicksPerSecond); }

		// 在故意中断计时(例如，阻止性 IO 操作)之后
		// 调用此函数以避免固定时间步长逻辑尝试一系列弥补
		// Update 调用。

		void ResetElapsedTime()
		{
			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw ref new Platform::FailureException();
			}

			m_leftOverTicks = 0;
			m_framesPerSecond = 0;
			m_framesThisSecond = 0;
			m_qpcSecondCounter = 0;
		}

		// 更新计时器状态，调用适当次数的指定 Update 函数。
		template<typename TUpdate>
		void Tick(const TUpdate& update)
		{
			// 查询当前时间。
			LARGE_INTEGER currentTime;

			if (!QueryPerformanceCounter(&currentTime))
			{
				throw ref new Platform::FailureException();
			}

			uint64 timeDelta = currentTime.QuadPart - m_qpcLastTime.QuadPart;

			m_qpcLastTime = currentTime;
			m_qpcSecondCounter += timeDelta;

			// 固定过大的时间增量(例如， 在调试器中暂停之后)。
			if (timeDelta > m_qpcMaxDelta)
			{
				timeDelta = m_qpcMaxDelta;
			}

			// 将 QPC 单位转换为规范的计时格式。这会因为上一次固定而无法溢出。
			timeDelta *= TicksPerSecond;
			timeDelta /= m_qpcFrequency.QuadPart;

			uint32 lastFrameCount = m_frameCount;

			if (m_isFixedTimeStep)
			{
				// 固定时间步长更新逻辑

				// 如果应用程序的运行时间十分接近目标运行时间(1/4 毫秒内)，可直接固定
				// 时钟以与目标值完全一致。这将防止微小和无关的错误
				// 随时间累积。在不使用此固定的情况下，请求 60 fps 固定更新的
				// 游戏如果在 59.94 NTSC 显示屏上运行时启用了 vsync，则最终会
				// 累积足够多的小错误而导致掉帧。 最好将
				// 将小偏差圆整到零，保持一切平稳运行。

				if (abs(static_cast<int64>(timeDelta - m_targetElapsedTicks)) < TicksPerSecond / 4000)
				{
					timeDelta = m_targetElapsedTicks;
				}

				m_leftOverTicks += timeDelta;

				while (m_leftOverTicks >= m_targetElapsedTicks)
				{
					m_elapsedTicks = m_targetElapsedTicks;
					m_totalTicks += m_targetElapsedTicks;
					m_leftOverTicks -= m_targetElapsedTicks;
					m_frameCount++;

					update();
				}
			}
			else
			{
				// 可变时间步长更新逻辑。
				m_elapsedTicks = timeDelta;
				m_totalTicks += timeDelta;
				m_leftOverTicks = 0;
				m_frameCount++;

				update();
			}

			// 跟踪当前帧速率。
			if (m_frameCount != lastFrameCount)
			{
				m_framesThisSecond++;
			}

			if (m_qpcSecondCounter >= static_cast<uint64>(m_qpcFrequency.QuadPart))
			{
				m_framesPerSecond = m_framesThisSecond;
				m_framesThisSecond = 0;
				m_qpcSecondCounter %= m_qpcFrequency.QuadPart;
			}
		}

	private:
		// 源计时数据使用 QPC 单位。
		LARGE_INTEGER m_qpcFrequency;
		LARGE_INTEGER m_qpcLastTime;
		uint64 m_qpcMaxDelta;

		// 派生的计时数据使用规范的计时格式。
		uint64 m_elapsedTicks;
		uint64 m_totalTicks;
		uint64 m_leftOverTicks;

		// 用于跟踪帧速率的成员。
		uint32 m_frameCount;
		uint32 m_framesPerSecond;
		uint32 m_framesThisSecond;
		uint64 m_qpcSecondCounter;

		// 用于配置固定时间步长模式的成员。
		bool m_isFixedTimeStep;
		uint64 m_targetElapsedTicks;
	};
}
