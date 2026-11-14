#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <geneial/core/fitness/FunctionFitnessEvaluator.h>
#include <geneial/core/population/builder/MultiValueChromosomeFactory.h>
#include <geneial/core/population/management/BaseManager.h>
#include <geneial/core/population/management/Bookkeeper.h>
#include <geneial/core/population/management/ResourcePool.h>
#include <iostream>
#include <memory>
#include <new>
#include <span>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

namespace
{
std::atomic<std::size_t> allocationCount{0};
std::atomic<bool> countAllocations{false};
thread_local bool failNextAllocation = false;
}

[[gnu::noinline]] void *operator new(std::size_t size)
{
    if (std::exchange(failNextAllocation, false))
        throw std::bad_alloc();
    if (void *memory = std::malloc(size == 0 ? 1 : size))
    {
        if (countAllocations.load(std::memory_order_relaxed))
            allocationCount.fetch_add(1, std::memory_order_relaxed);
        return memory;
    }
    throw std::bad_alloc();
}
[[gnu::noinline]] void *operator new[](std::size_t size)
{
    return ::operator new(size);
}
[[gnu::noinline]] void operator delete(void *memory) noexcept
{
    std::free(memory);
}
[[gnu::noinline]] void operator delete[](void *memory) noexcept
{
    std::free(memory);
}
[[gnu::noinline]] void operator delete(void *memory, std::size_t) noexcept
{
    std::free(memory);
}
[[gnu::noinline]] void operator delete[](void *memory, std::size_t) noexcept
{
    std::free(memory);
}

namespace
{
using namespace geneial::population::management;
struct Payload : EventData
{
    static inline unsigned int live = 0;
    Payload()
    {
        ++live;
    }
    ~Payload() override
    {
        --live;
    }
};
void require(bool condition, const char *message)
{
    if (!condition)
        throw std::runtime_error(message);
}
std::size_t countIgnoredEvents()
{
    DefaultBookkeeper keeper;
    allocationCount = 0;
    countAllocations = true;
    for (unsigned int index = 0; index < 100000; ++index)
        EventValueData<unsigned int>::create(keeper, "ignored", index);
    countAllocations = false;
    return allocationCount;
}
void verifyMemory()
{
    require(countIgnoredEvents() == 0, "No-op diagnostics allocated payloads");
    StatisticBookkeeper keeper(4);
    auto *payload = new Payload;
    failNextAllocation = true;
    bool failed = false;
    try
    {
        keeper.traceEvent("an event name deliberately longer than small-string optimization can hold", payload);
    }
    catch (const std::bad_alloc &)
    {
        failed = true;
    }
    failNextAllocation = false;
    require(failed && Payload::live == 0 && keeper.getEvents().empty(),
            "Event ownership was lost after allocation failure");
    ResourcePool<Payload> pool(1);
    payload = new Payload;
    failNextAllocation = true;
    pool.free(std::move(payload));
    const bool cacheAllocationFailed = !failNextAllocation;
    failNextAllocation = false;
    require(cacheAllocationFailed && !payload && Payload::live == 0 && pool.size() == 0,
            "Cache allocation failure did not release the resource");
    auto timer = keeper.scopedEventTrace("timer");
    failNextAllocation = true;
    timer.reset();
    const bool timerAllocationFailed = !failNextAllocation;
    failNextAllocation = false;
    require(timerAllocationFailed && keeper.getDroppedEventCount() == 1 && keeper.getEvents().empty(),
            "Timer allocation failure escaped its destructor");
}
void benchmark()
{
    auto evaluator = std::make_shared<geneial::FunctionFitnessEvaluator<int>>([](std::span<const int>) { return 1.0; });
    using Factory = geneial::population::chromosome::MultiValueChromosomeFactory<int, double>;
    Factory::Builder builder(evaluator);
    builder.getSettings().setNum(256);
    auto factory = std::dynamic_pointer_cast<Factory>(builder.create());
    auto manager = BaseManager<double>::create(factory);
    auto candidate = factory->createChromosome();
    manager->getPopulation().insertChromosome(candidate);
    std::cout << "{\"noop_event_allocations\":" << countIgnoredEvents() << ",\"deduplication\":[";
    bool first = true;
    for (std::size_t count : {10000, 40000, 80000})
    {
        std::vector<double> samples;
        for (unsigned int repeat = 0; repeat < 5; ++repeat)
        {
            geneial::population::Population<double>::chromosome_container batch(count, candidate);
            const auto start = std::chrono::steady_clock::now();
            manager->getPopulation().removeDuplicates(batch);
            samples.push_back(
                std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count());
            require(batch.empty(), "Duplicate benchmark changed behavior");
        }
        std::sort(samples.begin(), samples.end());
        if (!first)
            std::cout << ',';
        first = false;
        std::cout << "{\"candidates\":" << count << ",\"median_ms\":" << samples[2] << '}';
    }
    std::vector<geneial::population::chromosome::BaseChromosome<double>::ptr> burst;
    for (unsigned int index = 0; index < 10000; ++index)
        burst.push_back(factory->createChromosome());
    burst.clear();
    std::cout << "],\"pool_cached_after_10000\":" << factory->getCachedChromosomeCount()
              << ",\"pool_capacity\":" << factory->getResourcePoolCapacity() << "}\n";
}
}

int main(int argc, char **argv)
{
    try
    {
        if (argc != 2)
            throw std::invalid_argument("Expected --verify-memory or --benchmark");
        const std::string_view mode(argv[1]);
        if (mode == "--verify-memory")
        {
            verifyMemory();
            std::cout << "Allocation and failure-path invariants pass\n";
        }
        else if (mode == "--benchmark")
            benchmark();
        else
            throw std::invalid_argument("Unknown probe mode");
        return 0;
    }
    catch (const std::exception &error)
    {
        failNextAllocation = false;
        countAllocations = false;
        std::cerr << error.what() << '\n';
        return 1;
    }
}
