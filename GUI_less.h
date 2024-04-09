#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "iir1/Iir.h"

const int kNumPresets   = 1;
const int kNumChannels  = 2;

enum EParams {
  kParamTight,
  kParamDrive,
  kParamTone,
  kParamOutput,
  //////////
  kNumParams
};

using namespace iplug;

class GUI_less final: public Plugin {
private:

  // Converted params (dB => real):
  double  m_DriveReal;
  double  m_OutputReal;

  // Filters:
  Iir::Butterworth::HighPass<1> m_LowCut [kNumChannels];  //   6dB/oct low cut tightness filter
  Iir::Butterworth::LowPass <2> m_HighCut[kNumChannels];  //  12dB/oct high cut tone filter

public:
  GUI_less(const InstanceInfo& info);

  void OnReset() override;
  void OnParamChange(int paramIdx) override;
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;

};
