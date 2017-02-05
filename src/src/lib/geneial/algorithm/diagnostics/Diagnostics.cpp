#include <geneial/algorithm/diagnostics/Diagnostics.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(algorithm)
{

void printTiming(const char* string, std::unordered_multimap<std::string, std::unique_ptr<EventData> > &events,
        std::ostream& os)
{
    auto range = events.equal_range(string);
    //Calculate cumulative moving average:
    double cma = 0;
    int n = 0;
    double min = -1;
    double max = 0;

    std::for_each(range.first, //first
            range.second, //last
            [&os,&cma,&n,&min,&max](
                    std::pair<const std::string, std::unique_ptr<EventData>> &event)
            {
			//static int n = 0;
            EventValueData<double> *derivedPointer = dynamic_cast<EventValueData<double>*>(event.second.get());
            const double time = derivedPointer->getValue();
            max = std::max(max,time);
            if(min == -1)
            {
                min = derivedPointer->getValue();
            }
            else
            {
                min = std::min(min,time);
            }
            cma = (n * cma + derivedPointer->getValue()) / (n + 1);
            //cma += derivedPointer->getValue();
            n++;
        });
    //cma = (cma ) / n;
    os << " (" << std::setw(20) << string << ") x " << std::setw(12) << n << ",\t Average  " << std::setprecision(6)
            << std::fixed << std::setw(12) << cma << " [ms],\t Min " << std::setprecision(6) << std::fixed
            << std::setw(12) << min << " [ms],\t Max " << std::setprecision(6) << std::fixed << std::setw(12) << max
            << " [ms]" << std::endl;
}

geneial_export_namespace
{
}

}
}
