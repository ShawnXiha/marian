#pragma once
#include <map>
#include "common/base_best_hyps.h"
#include "array.h"

namespace amunmt {
namespace FPGA {

class BestHyps : public BestHypsBase
{
public:
  BestHyps(const God &god, const OpenCLInfo &openCLInfo);

  void DisAllowUNK(mblas::Matrix& Prob);

  void FindBests(const std::vector<size_t>& beamSizes, mblas::Matrix& Probs,
                 std::vector<float>& outCosts,
                 std::vector<unsigned>& outKeys,
                 const bool isFirst);

  virtual void CalcBeam(
      const God &god,
      const Beam& prevHyps,
      const std::vector<ScorerPtr>& scorers,
      const Words& filterIndices,
      bool returnAlignment,
      std::vector<Beam>& beams,
      std::vector<size_t>& beamSizes
      );

protected:
  Array<unsigned> keys;
  Array<float> Costs;
  const std::map<std::string, float>& weights_;

};

}
}

