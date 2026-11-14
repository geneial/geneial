#pragma once

#include <chrono>
#include <cstddef>
#include <geneial/namespaces.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

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
                    virtual ~ScopedTrace() = default;
                };
                struct EventData
                {
                    virtual ~EventData() = default;
                };

                class BaseBookkeeper
                {
                  public:
                    virtual ~BaseBookkeeper() = default;
                    virtual bool acceptsEventData()
                    {
                        return true;
                    }
                    virtual std::unique_ptr<ScopedTrace> scopedEventTrace(const char *name) = 0;
                    virtual void traceEvent(const char *name) = 0;
                    /** Transfers ownership immediately, including on failure. Custom implementations must adopt before
                     * allocating. */
                    virtual void traceEvent(const char *name, EventData *data) = 0;
                };

                template <typename VALUE> class EventValueData : public EventData
                {
                    VALUE _value;

                  public:
                    explicit EventValueData(VALUE value) : _value(value) {}
                    static void create(BaseBookkeeper &bookkeeper, const char *name, VALUE value)
                    {
                        if (bookkeeper.acceptsEventData())
                            bookkeeper.traceEvent(name, new EventValueData(value));
                    }
                    VALUE getValue() const
                    {
                        return _value;
                    }
                };

                class DefaultBookkeeper : public BaseBookkeeper
                {
                  public:
                    bool acceptsEventData() override
                    {
                        return false;
                    }
                    std::unique_ptr<ScopedTrace> scopedEventTrace(const char *) override
                    {
                        return nullptr;
                    }
                    void traceEvent(const char *) override {}
                    void traceEvent(const char *, EventData *data) override
                    {
                        delete data;
                    }
                };

                class ScopedTimeTrace;

                /** Owner-thread diagnostics. Retains the first capacity events; clear() starts a fresh collection. */
                class StatisticBookkeeper : public BaseBookkeeper
                {
                  public:
                    using event_container = std::unordered_multimap<std::string, std::unique_ptr<EventData>>;
                    static constexpr std::size_t DEFAULT_CAPACITY = 4096;

                  private:
                    friend class ScopedTimeTrace;
                    struct State
                    {
                        event_container events;
                        std::size_t capacity;
                        std::size_t dropped = 0;
                        explicit State(std::size_t limit) : capacity(limit) {}
                    };
                    std::shared_ptr<State> _state;

                    static void record(const std::shared_ptr<State> &state, const char *name,
                                       std::unique_ptr<EventData> data)
                    {
                        if (!name)
                            throw std::invalid_argument("Event name cannot be null");
                        if (state->events.size() >= state->capacity)
                        {
                            ++state->dropped;
                            return;
                        }
                        state->events.emplace(std::string(name), std::move(data));
                    }

                  public:
                    explicit StatisticBookkeeper(std::size_t capacity = DEFAULT_CAPACITY)
                        : _state(std::make_shared<State>(capacity))
                    {
                    }
                    StatisticBookkeeper(const StatisticBookkeeper &) = delete;
                    StatisticBookkeeper &operator=(const StatisticBookkeeper &) = delete;

                    bool acceptsEventData() override
                    {
                        if (_state->events.size() < _state->capacity)
                            return true;
                        ++_state->dropped;
                        return false;
                    }
                    std::unique_ptr<ScopedTrace> scopedEventTrace(const char *name) override;
                    void traceEvent(const char *name) override
                    {
                        if (!name)
                            throw std::invalid_argument("Event name cannot be null");
                        if (acceptsEventData())
                            record(_state, name, std::make_unique<EventData>());
                    }
                    void traceEvent(const char *name, EventData *data) override
                    {
                        std::unique_ptr<EventData> owned(data);
                        record(_state, name, std::move(owned));
                    }
                    const event_container &getEvents() const
                    {
                        return _state->events;
                    }
                    std::size_t getCapacity() const
                    {
                        return _state->capacity;
                    }
                    std::size_t getDroppedEventCount() const
                    {
                        return _state->dropped;
                    }
                    void clear()
                    {
                        _state->events.clear();
                        _state->dropped = 0;
                    }
                    /** Shrinking discards unspecified retained entries; zero disables collection. */
                    void setCapacity(std::size_t capacity)
                    {
                        while (_state->events.size() > capacity)
                        {
                            _state->events.erase(_state->events.begin());
                            ++_state->dropped;
                        }
                        _state->capacity = capacity;
                    }
                };

                class ScopedTimeTrace final : public ScopedTrace
                {
                    friend class StatisticBookkeeper;
                    std::weak_ptr<StatisticBookkeeper::State> _state;
                    std::string _name;
                    std::chrono::steady_clock::time_point _start = std::chrono::steady_clock::now();
                    ScopedTimeTrace(const std::shared_ptr<StatisticBookkeeper::State> &state, const char *name)
                        : _state(state), _name(name)
                    {
                    }

                  public:
                    ~ScopedTimeTrace() override
                    {
                        if (auto state = _state.lock())
                        {
                            if (state->events.size() >= state->capacity)
                            {
                                ++state->dropped;
                                return;
                            }
                            try
                            {
                                const auto elapsed =
                                    std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - _start)
                                        .count();
                                StatisticBookkeeper::record(state, _name.c_str(),
                                                            std::make_unique<EventValueData<double>>(elapsed));
                            }
                            catch (...)
                            {
                                ++state->dropped;
                            }
                        }
                    }
                };

                inline std::unique_ptr<ScopedTrace> StatisticBookkeeper::scopedEventTrace(const char *name)
                {
                    if (!name)
                        throw std::invalid_argument("Event name cannot be null");
                    if (!acceptsEventData())
                        return nullptr;
                    return std::unique_ptr<ScopedTrace>(new ScopedTimeTrace(_state, name));
                }

                class ScopedEvent
                {
                    std::unique_ptr<ScopedTrace> _trace;

                  public:
                    ScopedEvent(const char *name, BaseBookkeeper &bookkeeper)
                        : _trace(bookkeeper.scopedEventTrace(name))
                    {
                    }
                };
            }
        }
    }
}
