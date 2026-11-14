#include <cmath>
#include <geneial/algorithm/diagnostics/Diagnostics.h>
#include <iomanip>
#include <stdexcept>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(algorithm)
    {
        geneial_export_namespace
        {
            void printTiming(const char *name,
                             const std::unordered_multimap<std::string, std::unique_ptr<EventData>> &events,
                             std::ostream &stream)
            {
                if (!name)
                    throw std::invalid_argument("Timing event name cannot be null");
                const auto range = events.equal_range(name);
                double average = 0;
                double minimum = 0;
                double maximum = 0;
                std::size_t count = 0;
                for (auto entry = range.first; entry != range.second; ++entry)
                {
                    const auto *duration = dynamic_cast<const EventValueData<double> *>(entry->second.get());
                    if (!duration || !std::isfinite(duration->getValue()) || duration->getValue() < 0)
                        continue;
                    const auto value = duration->getValue();
                    minimum = count == 0 ? value : std::min(minimum, value);
                    maximum = std::max(maximum, value);
                    average += (value - average) / static_cast<double>(++count);
                }
                stream << " (" << std::setw(20) << name << ") x " << std::setw(12) << count << ",\t Average  "
                       << std::setprecision(6) << std::fixed << std::setw(12) << average << " [ms],\t Min "
                       << std::setw(12) << minimum << " [ms],\t Max " << std::setw(12) << maximum << " [ms]\n";
            }
        }
    }
}
