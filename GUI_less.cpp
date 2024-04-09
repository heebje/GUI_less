#include "GUI_less.h"
#include "IPlug_include_in_plug_src.h"

GUI_less::GUI_less(const InstanceInfo& info): Plugin(info, MakeConfig(kNumParams, kNumPresets)) {

  GetParam(kParamTight )->InitFrequency ("Tight",  200.0,  20.0,  2000.0, 0.01);
  GetParam(kParamDrive )->InitGain      ("Drive",  +36.0,   0.0,   +72.0, 0.01);
  GetParam(kParamTone  )->InitFrequency ("Tone",  4000.0, 800.0, 20000.0, 0.01);
  GetParam(kParamOutput)->InitGain      ("Output", -18.0, -36.0,     0.0, 0.01);

  // Reset filters:
  OnReset();

}

void GUI_less::OnReset() {

  const double  sr          = GetSampleRate();
  const double  lowCutFreq  = GetParam(kParamTight)->Value();
  const double  highCutFreq = GetParam(kParamTone )->Value();

  for (auto ch = 0; ch < kNumChannels; ch++) {

    m_LowCut [ch].setup(sr, lowCutFreq);
    m_LowCut [ch].reset();

    m_HighCut[ch].setup(sr, highCutFreq);
    m_HighCut[ch].reset();

  }

}

void GUI_less::OnParamChange(int paramIdx) {

  switch (paramIdx) {

    case kParamTight: {
      const double  sr  = GetSampleRate();
      const double  fr  = GetParam(paramIdx)->Value();
      for (auto ch = 0; ch < kNumChannels; ch++) {
        m_LowCut[ch].setup(sr, fr);
      }
      break;
    }

    case kParamDrive: {
      m_DriveReal = GetParam(paramIdx)->DBToAmp();
      break;
    }

    case kParamTone: {
      const double  sr  = GetSampleRate();
      const double  fr  = GetParam(paramIdx)->Value();
      for (auto ch = 0; ch < kNumChannels; ch++) {
        m_HighCut[ch].setup(sr, fr);
      }
      break;
    }

    case kParamOutput: {
      m_OutputReal = GetParam(paramIdx)->DBToAmp();
      break;
    }

    default: {
      assert(!"Not all parameters changes processed");
    }

  }

}

void GUI_less::ProcessBlock(sample** inputs, sample** outputs, int nFrames) {

  for (auto f = 0; f < nFrames; f++) {
    for (auto ch = 0; ch < kNumChannels; ch++) {

      outputs[ch][f] =
        m_OutputReal *
          m_HighCut[ch].filter(
            tanh(
              m_DriveReal *
                m_LowCut[ch].filter(
                  inputs[ch][f])));
    }
  }
}
