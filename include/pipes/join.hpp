#ifndef PIPES_join_HPP
#define PIPES_join_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/FWD.hpp"

namespace pipes
{
    struct join_pipe : public pipe_base
    {
        template<typename Collection, typename TailPipeline>
        void onReceive(Collection&& collection, TailPipeline&& tailPipeline)
        {
            std::copy(begin(collection), end(collection), tailPipeline);
        }
    };
    
    auto constexpr join = join_pipe{};

} // namespace pipes

#endif /* PIPES_join_HPP */
