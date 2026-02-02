#include <iostream>
#include <random>
#include <mutex>
#include <unordered_map>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

/*

*/

/* Implementation of class "TrafficLight" */

/*

// virtual function which is executed in a thread

*/

void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class.
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
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex>
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
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

    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait()
    // to wait for and receive new messages and pull them from the queue using move semantics.
    // The received object should then be returned by the receive function.
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop
    // runs and repeatedly calls the receive function on the message queue.
    // Once it receives TrafficLightPhase::green, the method returns.
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
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles
    // and toggles the current phase of the traffic light between red and green and sends an update method
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds.
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.
}
