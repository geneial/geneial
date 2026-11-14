#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <queue>
#include <span>
#include <stdexcept>
#include <vector>

namespace showcase::maze
{
class Generator
{
    std::uint32_t _state;

  public:
    explicit Generator(std::uint32_t seed) : _state(seed ? seed : 0x9e3779b9u) {}
    std::uint32_t next()
    {
        _state ^= _state << 13;
        _state ^= _state >> 17;
        _state ^= _state << 5;
        return _state;
    }
};

struct Trace
{
    std::vector<unsigned int> positions;
    unsigned int remaining;
    unsigned int bestStep = 0;
    unsigned int collisions = 0;
    double cost = 0;
};

class Maze
{
    unsigned int _size;
    std::vector<unsigned int> _walls;
    std::vector<unsigned int> _distance;

    unsigned int neighbor(unsigned int cell, unsigned int direction) const
    {
        const auto row = cell / _size, column = cell % _size;
        if (direction == 0 && row > 0)
            return cell - _size;
        if (direction == 1 && column + 1 < _size)
            return cell + 1;
        if (direction == 2 && row + 1 < _size)
            return cell + _size;
        if (direction == 3 && column > 0)
            return cell - 1;
        return cell;
    }

    void open(unsigned int cell, unsigned int direction)
    {
        const auto next = neighbor(cell, direction);
        _walls[cell] &= ~(1u << direction);
        _walls[next] &= ~(1u << ((direction + 2) % 4));
    }

  public:
    Maze(unsigned int size, std::uint32_t seed) : _size(size)
    {
        if (size < 4 || size > 32)
            throw std::invalid_argument("Maze size must be 4..32");
        _walls.assign(size * size, 15);
        Generator random(seed);
        std::vector<bool> visited(_walls.size(), false);
        std::vector<unsigned int> stack{0};
        visited[0] = true;
        while (!stack.empty())
        {
            const auto cell = stack.back();
            std::vector<unsigned int> choices;
            for (unsigned int direction = 0; direction < 4; ++direction)
            {
                const auto next = neighbor(cell, direction);
                if (next != cell && !visited[next])
                    choices.push_back(direction);
            }
            if (choices.empty())
                stack.pop_back();
            else
            {
                const auto direction = choices[random.next() % choices.size()];
                const auto next = neighbor(cell, direction);
                open(cell, direction);
                visited[next] = true;
                stack.push_back(next);
            }
        }
        for (unsigned int cell = 0; cell < _walls.size(); ++cell)
            for (const auto direction : {1u, 2u})
                if (neighbor(cell, direction) != cell && random.next() % 64 == 0)
                    open(cell, direction);
        _distance.assign(_walls.size(), std::numeric_limits<unsigned int>::max());
        std::queue<unsigned int> pending;
        _distance.back() = 0;
        pending.push(static_cast<unsigned int>(_walls.size() - 1));
        while (!pending.empty())
        {
            const auto cell = pending.front();
            pending.pop();
            for (unsigned int direction = 0; direction < 4; ++direction)
            {
                const auto next = advance(cell, direction);
                if (_distance[next] == std::numeric_limits<unsigned int>::max())
                {
                    _distance[next] = _distance[cell] + 1;
                    pending.push(next);
                }
            }
        }
    }

    unsigned int size() const
    {
        return _size;
    }
    const std::vector<unsigned int> &walls() const
    {
        return _walls;
    }
    const std::vector<unsigned int> &distances() const
    {
        return _distance;
    }

    unsigned int advance(unsigned int cell, unsigned int direction) const
    {
        if (cell >= _walls.size() || direction > 3)
            throw std::invalid_argument("Invalid maze move");
        return _walls[cell] & (1u << direction) ? cell : neighbor(cell, direction);
    }

    Trace trace(std::span<const int> genome) const
    {
        Trace result{{0}, _distance[0]};
        unsigned int hits = 0;
        for (std::size_t step = 0; step < genome.size() && result.remaining != 0; ++step)
        {
            const auto previous = result.positions.back();
            const auto cell = advance(previous, static_cast<unsigned int>(genome[step]));
            hits += cell == previous;
            result.positions.push_back(cell);
            if (_distance[cell] < result.remaining)
            {
                result.remaining = _distance[cell];
                result.bestStep = static_cast<unsigned int>(step + 1);
                result.collisions = hits;
            }
        }
        result.cost = result.remaining * (genome.size() + 1.0) + result.bestStep +
                      static_cast<double>(result.collisions) / (genome.size() + 1.0);
        return result;
    }

    std::vector<int> compactPrefix(std::span<const int> genome, const Trace &trace) const
    {
        std::vector<int> route;
        std::vector<unsigned int> positions{0};
        std::vector<int> seen(_walls.size(), -1);
        seen[0] = 0;
        for (unsigned int step = 1; step <= trace.bestStep; ++step)
        {
            const auto cell = trace.positions[step];
            if (cell == positions.back())
                continue;
            if (seen[cell] >= 0)
            {
                const auto end = static_cast<std::size_t>(seen[cell]);
                for (auto index = end + 1; index < positions.size(); ++index)
                    seen[positions[index]] = -1;
                positions.resize(end + 1);
                route.resize(end);
            }
            else
            {
                route.push_back(genome[step - 1]);
                seen[cell] = static_cast<int>(positions.size());
                positions.push_back(cell);
            }
        }
        return route;
    }
};
}
