#ifndef PIPES_PARTITION_HPP
#define PIPES_PARTITION_HPP

#include "pipes/operator.hpp"

#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/FWD.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/pipeline_base.hpp"

namespace pipes
{

template<typename OutputPipeTrue, typename OutputPipeFalse, typename Predicate>
class partition_pipe : public pipeline_base<partition_pipe<OutputPipeTrue, OutputPipeFalse, Predicate>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        if (predicate_(FWD(value)))
        {
            send(outputPipeTrue_, FWD(value));
        }
        else
        {
            send(outputPipeFalse_, FWD(value));
        }
    }

    explicit partition_pipe(OutputPipeTrue iteratorTrue, OutputPipeFalse iteratorFalse, Predicate predicate) : outputPipeTrue_(iteratorTrue), outputPipeFalse_(iteratorFalse), predicate_(predicate) {}
    
private:
    OutputPipeTrue outputPipeTrue_;
    OutputPipeFalse outputPipeFalse_;
    detail::assignable<Predicate> predicate_;
};

template<typename Predicate, typename OutputPipeTrue, typename OutputPipeFalse>
partition_pipe<OutputPipeTrue, OutputPipeFalse, Predicate> partition(Predicate predicate, OutputPipeTrue&& outputPipeTrue, OutputPipeFalse&& outputPipeFalse)
{
    return partition_pipe<OutputPipeTrue, OutputPipeFalse, Predicate>(FWD(outputPipeTrue), FWD(outputPipeFalse), predicate);
}

} // namespace pipes

#endif /* PIPES_PARTITION_HPP */
