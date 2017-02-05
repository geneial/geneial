#pragma once

#include <geneial/namespaces.h>

#include <unordered_map>
#include <chrono>
#include <ctime>
#include <ratio>
#include <functional>
#include <memory>
#include <iostream>

//TODO(bewo): Clean this up.

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
    virtual ~ScopedTrace()
    {
    }
};

struct EventData
{
    virtual ~EventData()
    {
    }
};

class BaseBookkeeper
{
public:
    virtual ~BaseBookkeeper()
    {
    }

    virtual std::unique_ptr<ScopedTrace> scopedEventTrace(const char* traceName) = 0;

    virtual void traceEvent(const char* traceName) = 0;

    virtual void traceEvent(const char* traceName, EventData* data) = 0;
};

template<typename T>
class EventValueData : public EventData
{
    T _value;
private:
    EventValueData(const T value) :
        _value(value)
    {
    }

public:
    virtual ~EventValueData()
    {
    }

    static inline void create(BaseBookkeeper &bookkeeper, const char* name, const T value)
    {
        EventValueData * evd = new EventValueData(value);
        bookkeeper.traceEvent(name,evd);
    }

    T getValue() const
    {
        return _value;
    }
};

class DefaultBookkeeper: public BaseBookkeeper
{
public:
    virtual ~DefaultBookkeeper()
    {
    }

    virtual std::unique_ptr<ScopedTrace> scopedEventTrace(const char*) override
    {
        return std::unique_ptr<ScopedTrace>(nullptr);
    }

    virtual void traceEvent(const char*) override
    {
    }

    virtual void traceEvent(const char*, EventData* evd) override
    {
    	delete evd;
    }
};

class ScopedTimeTrace;


class StatisticBookkeeper: public BaseBookkeeper
{
private:
    friend ScopedTimeTrace;

    //We use an unordered map since we want insert to be as fast as possible.
    //Later when diagnosing on this data-structure we do not care about speed.
    std::unordered_multimap<std::string,
                            std::unique_ptr<EventData>> _events;

    StatisticBookkeeper& operator= ( const StatisticBookkeeper & ) = delete;
    StatisticBookkeeper(StatisticBookkeeper const&) = delete;
protected:
    void traceTime(const char* name, const double duration)
    {
        EventValueData<double>::create(*this,name,duration);
    }


public:
    StatisticBookkeeper()
    {
    }
    virtual std::unique_ptr<ScopedTrace> scopedEventTrace(const char* traceName) override;

    virtual void traceEvent(const char* traceName) override
    {
        _events.insert(std::make_pair(std::string(traceName),std::unique_ptr<EventData>(new EventData())));
    }

    virtual void traceEvent(const char* traceName, EventData* data) override
    {
        _events.insert(std::make_pair(std::string(traceName),std::unique_ptr<EventData>(data)));
    }

    virtual ~StatisticBookkeeper()
    {
    }

    std::unordered_multimap<std::string, std::unique_ptr<EventData> >& getEvents()
    {
        return _events;
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
        const auto duration = std::chrono::duration_cast<std::chrono::duration<double,std::milli>>(end - _start).count();
        //TODO (bewo):string?
        _bookKeeper->traceTime(_name.c_str(), duration);
    }
};


class ScopedEvent
{
    std::unique_ptr<ScopedTrace> _trace;
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

