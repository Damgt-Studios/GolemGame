#pragma once
#include "ADAudio.h"
#include <ADEventSystem.h>
#include <ADParticles.h>



class ParticleEmitterEvent : public ADEvents::Listener
{
private:
    FountainEmitter& emitter;
    XMFLOAT4 position;
public:
    float lifespan;
    ParticleEmitterEvent(FountainEmitter& _emitter) : emitter(_emitter) {};
    void HandleEvent(ADEvents::ADEvent* _event) override;
    std::string ToString() override;
};

class AudioSourceEvent : public ADEvents::Listener
{
private:
    AD_AUDIO::AudioSource& audioSource;
public:
    AudioSourceEvent(AD_AUDIO::AudioSource& _audioSource) : audioSource(_audioSource) {};
    void HandleEvent(ADEvents::ADEvent* _event) override;
    std::string ToString() override;
};

class ApplyEffectEvent : public ADEvents::Listener
{
private:
    ADResource::ADGameplay::Effect* effect;
    ADResource::ADGameplay::GameObject* target;
public:
    ApplyEffectEvent() {};
    void SetTarget(ADResource::ADGameplay::GameObject* _target, ADResource::ADGameplay::Effect* _effect);
    void HandleEvent(ADEvents::ADEvent* _event) override;
    std::string ToString() override;
};

class SetUIVisibilityListener : public ADEvents::Listener
{
private:
    ADUI::UIComponent* uiComponent;
public:
    //This event turns visibility On for a target if the value sent is the same as the keyIndex.  
    //Leave 0 for all singles, UI groups will have a sepeate listener for each responder but only one event call.
    int keyIndex;
    //If we turning visibility on = true, off = false.
    bool isOnSwitch = true;
    //Changes from "is the key equal to the value passed," to "is the key greater than or equal to the value passed."
    bool isKeyThreshold = false;
    //If you are not the target but you listened, do I switch you to the opposite value?  
    //If this is on it'll turn non targets off making a single On setup.  
    //If this is an OffSwitch and switchFailures is true, then all failures will turn ON!
    bool switchFailures = false;
    SetUIVisibilityListener() {};
    void SetTarget(ADUI::UIComponent* _uiComponent)
    {
        uiComponent = _uiComponent;
    };
    void HandleEvent(ADEvents::ADEvent* _event) override;
    std::string ToString() override;
};

class SetUIFocusListener : public ADEvents::Listener
{
private:
    ADUI::UIComponent* uiComponent;
public:
    //This event turns visibility On for a target if the value sent is the same as the keyIndex.  
    //Leave 0 for all singles, UI groups will have a sepeate listener for each responder but only one event call.
    int keyIndex;
    //If we turning visibility on = true, off = false.
    bool isOnSwitch = true;
    //Changes from "is the key equal to the value passed," to "is the key greater than or equal to the value passed."
    bool isKeyThreshold = false;
    //If you are not the target but you listened, do I switch you to the opposite value?  
    //If this is on it'll turn non targets off making a single On setup.  
    //If this is an OffSwitch and switchFailures is true, then all failures will turn ON!
    bool switchFailures = false;
    SetUIFocusListener() {};
    void SetTarget(ADUI::UIComponent* _uiComponent) 
    {
        uiComponent = _uiComponent;
    };
    void HandleEvent(ADEvents::ADEvent* _event) override;
    std::string ToString() override;
};

class SetUISetFrameListener : public ADEvents::Listener
{
private:
    ADUI::Image2D* image;
public:
    SetUISetFrameListener() {};
    void SetTarget(ADUI::Image2D* _image)
    {
        image = _image;
    };
    void HandleEvent(ADEvents::ADEvent* _event) override;
    std::string ToString() override;
};

class SetUITilingListener : public ADEvents::Listener
{
private:
    ADUI::Image2D* image;
public:
    SetUITilingListener() {};
    void SetTarget(ADUI::Image2D* _image)
    {
        image = _image;
    };
    void HandleEvent(ADEvents::ADEvent* _event) override;
    std::string ToString() override;
};

class SetUISelectionListener : public ADEvents::Listener
{
private:
    ADUI::ComponentGrid* list;
public:
    SetUISelectionListener() {};
    void SetTarget(ADUI::ComponentGrid* _list)
    {
        list = _list;
    };
    void HandleEvent(ADEvents::ADEvent* _event) override;
    std::string ToString() override;
};

class SetUITextListener : public ADEvents::Listener
{
private:
    ADUI::Label2D* label;
public:
    SetUITextListener() {};
    void SetTarget(ADUI::Label2D* _label)
    {
        label = _label;
    };
    void HandleEvent(ADEvents::ADEvent* _event) override;
    std::string ToString() override;
};

