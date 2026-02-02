#include <iostream>
#include <random>
#include <mutex>
#include <unordered_map>
#include "TrafficLight.h"

void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> uLock(_mutex);
    _queue.emplace_back(std::move(msg));
    _cond.notify_one();
}

template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this]
               { return !_queue.empty(); });

    T msg = std::move(_queue.front());
    _queue.pop_front();

    return msg;
}

void TrafficLight::waitForGreen()
{
    while (_messageQueue.receive() != TrafficLightPhase::green)
    {
    }
}

TrafficLightPhase TrafficLight::nextPhase(TrafficLightPhase current)
{
    static const std::unordered_map<TrafficLightPhase, TrafficLightPhase> transitions{
        {TrafficLightPhase::red, TrafficLightPhase::green},
        {TrafficLightPhase::green, TrafficLightPhase::red}};

    return transitions.at(current);
}

void TrafficLight::cycleThroughPhases()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(4000, 6000);

    int cycleDuration = dist(gen);

    auto lastUpdate = std::chrono::steady_clock::now();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastUpdate).count();

        if (elapsed >= cycleDuration)
        {
            {
                std::lock_guard<std::mutex> lock(_mutex);

                _currentPhase = nextPhase(_currentPhase);
            }

            _messageQueue.send(std::move(_currentPhase));

            lastUpdate = std::chrono::steady_clock::now();

            cycleDuration = dist(gen);
        }
    }
}
