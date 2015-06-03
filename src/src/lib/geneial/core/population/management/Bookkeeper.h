#pragma once

#include <geneial/namespaces.h>

#include <unordered_map>
#include <chrono>
#include <ctime>
#include <ratio>

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(management)
{

geneial_export_namespace
{

struct ScopedTrace
{
};

struct EventData
{
};

template<typename T>
struct EventValueData : public EventData
{
    T _value;
public:
    EventValueData(const T value) :
        _value(value)
    {
    }
};

struct TimingEventData: public EventData
{
    double _duration;
public:
    TimingEventData(const double duration) :
            _duration(duration)
    {
    }
};

class BaseBookkeeper
{
public:
    virtual ~BaseBookkeeper()
    {
    }

    virtual ScopedTrace scopedEventTrace(const char* traceName) = 0;

    virtual void traceEvent(const char* traceName) = 0;

    virtual void traceEvent(const char* traceName, EventData data) = 0;
};

class DefaultBookkeeper: public BaseBookkeeper
{
public:
    virtual ~DefaultBookkeeper()
    {
    }

    virtual ScopedTrace scopedEventTrace(const char* traceName) override
    {
        return ScopedTrace();
    }

    virtual void traceEvent(const char* traceName) override
    {
    }

    virtual void traceEvent(const char* traceName, EventData data) override
    {
    }
};
class StatisticBookkeeper;
class ScopedTimeTrace;


class StatisticBookkeeper: public BaseBookkeeper
{
private:
    friend ScopedTimeTrace;

    //We use an unordered map since we want insert to be as fast as possible.
    //Later when diagnosing on this data-structure we do not care about speed.
    std::unordered_multimap<std::string, EventData> _events;

protected:
    void traceTime(const std::string name, const double duration)
    {
        _events.emplace(name,TimingEventData(duration));
    }

public:
    virtual ScopedTrace scopedEventTrace(const char* traceName) override;

    virtual void traceEvent(const char* traceName) override
    {
        _events.emplace(traceName,EventData());
    }

    virtual void traceEvent(const char* traceName, EventData data) override
    {
        _events.emplace(traceName,std::move(data));
    }

    virtual ~StatisticBookkeeper()
    {
    }
};

class ScopedTimeTrace: public ScopedTrace
{
private:
    std::chrono::high_resolution_clock::time_point _start;
    StatisticBookkeeper* _bookKeeper;
    std::string _name;

    friend StatisticBookkeeper;

protected:
    ScopedTimeTrace(StatisticBookkeeper *bookKeeper, const char* traceName) :
            _start(std::chrono::high_resolution_clock::now()),
            _bookKeeper(bookKeeper),
            _name(traceName)
    {
    }

public:
    virtual ~ScopedTimeTrace()
    {
        const std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - _start).count();
        _bookKeeper->traceTime(_name, duration);
    }
};

ScopedTrace StatisticBookkeeper::scopedEventTrace(const char* traceName)
{
        return ScopedTimeTrace(this,traceName);
}

class ScopedEvent
{
    ScopedTrace _trace;
public:
    ScopedEvent(const char* name,BaseBookkeeper &bookkeeper):
        _trace(bookkeeper.scopedEventTrace(name))
    {
    }
};

}
}
}
}

