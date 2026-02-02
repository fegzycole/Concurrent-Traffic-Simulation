#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <queue>
#include <condition_variable>
#include "TrafficObject.h"

enum TrafficLightPhase
{
    green,
    red
};

class Vehicle;

template <class T>
class MessageQueue
{
public:
    void send(T &&msg);
    T receive();
    std::deque<T> _queue;
    
    private:
    std::condition_variable _cond;
    std::mutex _mutex;
};

class TrafficLight : public TrafficObject
{
public:
    TrafficLight();
    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase();

private:
    std::condition_variable _condition;
    std::mutex _mutex;
    void cycleThroughPhases();
    TrafficLightPhase nextPhase(TrafficLightPhase _currentPhase);
    TrafficLightPhase _currentPhase;
    MessageQueue<TrafficLightPhase> _messageQueue;
};

#endif
