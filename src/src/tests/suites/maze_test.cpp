#ifndef GENEIAL_HEADER_ONLY_TESTS
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE maze
#ifdef GENEIAL_HEADER_ONLY_TESTS
#include <boost/test/included/unit_test.hpp>
#else
#include <boost/test/unit_test.hpp>
#endif
#include "../../../demo/showcase/Maze.h"

using showcase::maze::Maze;

BOOST_AUTO_TEST_CASE(random_mazes_are_connected_symmetric_and_bounded)
{
    for (const auto size : {4u, 12u, 18u, 32u})
        for (const auto seed : {0u, 1u, 42u, UINT32_MAX})
        {
            const Maze maze(size, seed);
            for (unsigned int cell = 0; cell < size * size; ++cell)
            {
                BOOST_REQUIRE_NE(maze.distances()[cell], UINT32_MAX);
                for (unsigned int direction = 0; direction < 4; ++direction)
                {
                    const auto next = maze.advance(cell, direction);
                    BOOST_REQUIRE_LT(next, size * size);
                    if (next != cell)
                    {
                        BOOST_CHECK_EQUAL(maze.advance(next, (direction + 2) % 4), cell);
                        BOOST_CHECK_LE(std::abs(static_cast<int>(maze.distances()[cell]) -
                                                static_cast<int>(maze.distances()[next])),
                                       1);
                    }
                }
            }
            BOOST_CHECK_EQUAL(maze.advance(0, 0), 0);
            BOOST_CHECK_EQUAL(maze.advance(0, 3), 0);
            BOOST_CHECK_EQUAL(maze.advance(size * size - 1, 1), size * size - 1);
            BOOST_CHECK_EQUAL(maze.advance(size * size - 1, 2), size * size - 1);
        }
    BOOST_CHECK_THROW(Maze(0, 42), std::invalid_argument);
    BOOST_CHECK_THROW(Maze(33, 42), std::invalid_argument);
    BOOST_CHECK_THROW(Maze(4, 42).advance(0, 4), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(maze_topology_matches_browser_fixtures)
{
    const std::array<std::pair<std::uint32_t, std::uint32_t>, 4> fixtures{
        {{0, 2753865642u}, {1, 555423777u}, {42, 1714881153u}, {UINT32_MAX, 2238746772u}}};
    for (const auto [seed, expected] : fixtures)
    {
        const Maze maze(18, seed);
        std::uint32_t hash = 2166136261u;
        for (const auto wall : maze.walls())
            hash = (hash ^ wall) * 16777619u;
        BOOST_CHECK_EQUAL(hash, expected);
        BOOST_CHECK(maze.walls() == Maze(18, seed).walls());
    }
    BOOST_CHECK(Maze(18, 1).walls() != Maze(18, 42).walls());
}

BOOST_AUTO_TEST_CASE(route_cost_rewards_exit_and_loop_repair_preserves_reached_cell)
{
    const Maze maze(18, 42);
    std::vector<int> solution;
    unsigned int cell = 0;
    while (maze.distances()[cell] != 0)
        for (unsigned int direction = 0; direction < 4; ++direction)
        {
            const auto next = maze.advance(cell, direction);
            if (maze.distances()[next] + 1 == maze.distances()[cell])
            {
                solution.push_back(static_cast<int>(direction));
                cell = next;
                break;
            }
        }
    BOOST_CHECK_EQUAL(maze.trace(solution).cost, maze.distances()[0]);
    auto wasted = solution;
    wasted.insert(wasted.begin(), 0);
    const auto trace = maze.trace(wasted);
    BOOST_CHECK_EQUAL(trace.remaining, 0);
    BOOST_CHECK_GT(trace.cost, maze.trace(solution).cost);
    const auto repaired = maze.compactPrefix(wasted, trace);
    BOOST_CHECK(repaired == solution);
    BOOST_CHECK_EQUAL(maze.trace(repaired).positions.back(), trace.positions[trace.bestStep]);
    std::vector<int> stationary(wasted.size(), 0);
    BOOST_CHECK_GT(maze.trace(stationary).cost, trace.cost);
}
