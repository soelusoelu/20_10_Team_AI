#include "Title.h"
#include "Scene.h"
#include "../Sound/SoundComponent.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"
#include "../../Sound/Effects/SoundEffect.h"
#include "../../Sound/Effects/SoundEffectCollection.h"
#include "../../Sound/Effects/Filter/SoundFilter.h"
#include "../../Sound/Effects/Reverb/Reverb.h"
#include "../../Sound/Effects/Reverb/SimpleReverb.h"
#include "../../Sound/Effects/Reverb/SimpleReverbParam.h"
#include "../../Sound/Player/Frequency.h"
#include "../../Sound/Player/SoundLoop.h"
#include "../../Sound/Player/SoundPlayer.h"
#include "../../Sound/Player/SoundPlayTimer.h"
#include "../../Sound/Voice/IVoice.h"
#include "../../Sound/Voice/VoiceDetails.h"
#include "../../Sound/Voice/Output/OutputVoices.h"
#include "../../Sound/Voice/SourceVoice/SourceVoice.h"
#include "../../Sound/Voice/SubmixVoice/SubmixVoice.h"
#include "../../Sound/Voice/SubmixVoice/SubmixVoiceInitParam.h"
#include "../../Sound/Volume/SoundFade.h"
#include "../../Sound/Volume/SoundPan.h"
#include "../../Sound/Volume/SoundVolume.h"
#include "../../Sound/XAudio2/SoundEngine.h"
#include "../../Utility/StringUtil.h"
#include <complex>
#include <vector>

Title::Title(GameObject& gameObject) :
    Component(gameObject),
    mScene(nullptr),
    mSound(nullptr),
    mWetSubmix(nullptr),
    mDrySubmix(nullptr)
{
}

Title::~Title() = default;

void Title::start() {
    mScene = getComponent<Scene>();
    mSound = getComponent<SoundComponent>();

    if (!mSound || mSound->isNull()) {
        return;
    }

    SubmixVoiceInitParam param;
    const auto& details = mSound->getVoiceDetails();
    param.channels = details.channels;
    param.inputSampleRate = details.sampleRate;
    //mWetSubmix = SoundEngine::instance().createSubmixVoice(param);
    //mDrySubmix = SoundEngine::instance().createSubmixVoice(param);

    //if (!mWetSubmix || !mDrySubmix) {
    //    return;
    //}

    //auto& outputVoice = mSound->getOutputVoices();
    //outputVoice.addOutputVoice(*mWetSubmix);
    //mWetSubmix->getSoundVolume().getPan().pan(1920.f);
    //outputVoice.addOutputVoice(*mDrySubmix, false, false);

    //mSound->getSoundVolume().setVolume(0.f);
    //mSound->getSoundVolume().getFade().settings(0.5f, 2.f);
    //mSound->getSoundPlayer().getFrequency().setFrequencyRatio(4.f);
    //mSound->getSoundEffect().getEffectCollection().getFilter().lowPassFilter(2500.f);
    //mSound->getSoundEffect().getEffectCollection().getFilter().lowPassFilterFromRadianFrequency(0.25f);
    //mSound->getSoundEffect().getEffectCollection().getFilter().highPassFilter(500.f);
    //mSound->getSoundEffect().getEffectCollection().getFilter().bandPassFilter(1000.f);
    //mSound->getSoundEffect().getEffectCollection().getFilter().notchFilter(1000.f);
    //mSound->getSoundEffect().getEffectCollection().getFilter().lowPassOnePoleFilter(0.05f);
    //mSound->getSoundEffect().getEffectCollection().getFilter().highPassOnePoleFilter(0.15f);
    //mSound->getSoundVolume().getPan().panFromPositionX(0.f);
    //mSound->getSoundVolume().getPan().panCenter();

    //サウンドエフェクト
    //int reverbID = mWetSubmix->getSoundEffect().reverb();
    //int echoID = mWetSubmix->getSoundEffect().echo();
    //int reverbID = mWetSubmix->getSoundEffect().simpleReverb();
    //mWetSubmix->getSoundEffect().getFilter().lowPassOnePoleFilter(0.25f);
    //mWetSubmix->getSoundEffect().getFilter().highPassOnePoleFilter(0.1f);
    auto fftID = mSound->getSoundEffect().getEffectCollection().discreteFourierTransform();

    //auto reverbParam = Reverb::getParameters();
    //reverbParam.WetDryMix = 20.f;
    //mWetSubmix->getSoundEffect().setEffectParameters(reverbID, &reverbParam, sizeof(reverbParam));
    //SimpleReverbParam reverbParam;
    //reverbParam.diffusion = 0.2f;
    //reverbParam.roomSize = 0.3f;
    //mWetSubmix->getSoundEffect().setEffectParameters(reverbID, &reverbParam, sizeof(reverbParam));
    //mWetSubmix->getSoundVolume().setVolume(1.f);
    //mDrySubmix->getSoundVolume().setVolume(0.f);

    auto& player = mSound->getSoundPlayer();
    //player.setPlayPoint(30.f);
    //player.getLoop().setLoopPoint(60.f, 90.f);
    player.getLoop().loopAll();
    //player.playStreamingFadeIn(1.f, 2.f);

    //GameObjectCreater::create("Waveform");
    GameObjectCreater::create("ObjTest");
}

void Title::update() {
    //auto isEnd = Input::joyPad().getEnter();
    //if (!isEnd) {
    //    isEnd = Input::keyboard().getEnter();
    //}
    //if (isEnd) {
    //    mScene->next("GamePlay");
    //}

    //if (Input::keyboard().getKeyDown(KeyCode::Alpha1)) {
    //    mSound->getSoundPlayer().setPlayPoint(55.f);
    //}

    Debug::renderLine(Vector3::left * 100.f, Vector3::right * 100.f, ColorPalette::red);
    Debug::renderLine(Vector3::down * 100.f, Vector3::up * 100.f, ColorPalette::green);
    Debug::renderLine(Vector3::back * 100.f, Vector3::forward * 100.f, ColorPalette::blue);
}
