#include "genPadSize.h"

namespace o2
{
namespace mch
{
namespace mapping
{
std::array<std::pair<int,int>,18> padSizes() {
 return std::array<std::pair<int,int>,18>{ std::make_pair<int,int>(6300,4200),std::make_pair<int,int>(6300,8400),std::make_pair<int,int>(6300,16800),std::make_pair<int,int>(6300,33600),std::make_pair<int,int>(7143,25000),std::make_pair<int,int>(7143,50000),std::make_pair<int,int>(7143,100000),std::make_pair<int,int>(7500,5000),std::make_pair<int,int>(7500,10000),std::make_pair<int,int>(7500,20000),std::make_pair<int,int>(8400,4200),std::make_pair<int,int>(12600,4200),std::make_pair<int,int>(15000,5000),std::make_pair<int,int>(25000,5000),std::make_pair<int,int>(25200,4200),std::make_pair<int,int>(30000,5000),std::make_pair<int,int>(50000,5000),std::make_pair<int,int>(100000,5000)};
}

} // namespace mapping
} // namespace mch
} // namespace o2
