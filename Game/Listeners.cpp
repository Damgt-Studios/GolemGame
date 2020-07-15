#include "pchgame.h"
#include "Listeners.h"


//void AudioSourceListener::HandleEvent(ADEvents::ADEvent* _event)
//{
//    audioSource.Play();
//}

void RecoveryEmitterListener::HandleEvent(ADEvents::ADEvent* _event)
{
    XMFLOAT3 float4Event = (*static_cast<XMFLOAT3*>(_event->Parameter()));
    emitter.Activate(lifespan, { float4Event.x, float4Event.y, float4Event.z, 0 }, 15.0f);
}

void SmallCloudEmitterListener::HandleEvent(ADEvents::ADEvent* _event)
{
    XMFLOAT3 float4Event = (*static_cast<XMFLOAT3*>(_event->Parameter()));
    emitter.Activate(lifespan, { float4Event.x, float4Event.y, float4Event.z, 0 });
}

void WaterWaveEmitterListener::HandleEvent(ADEvents::ADEvent* _event)
{
    XMFLOAT3 float4Event = (*static_cast<XMFLOAT3*>(_event->Parameter()));
    emitter.Activate(lifespan, { float4Event.x, float4Event.y, float4Event.z, 0 });
}

void IronSkinEmitterListener::HandleEvent(ADEvents::ADEvent* _event)
{
    XMFLOAT3 float4Event = (*static_cast<XMFLOAT3*>(_event->Parameter()));
    emitter.Activate(lifespan, { float4Event.x, float4Event.y, float4Event.z, 0 });
}

void FireballEmitterListener::HandleEvent(ADEvents::ADEvent* _event)
{
    XMFLOAT3 float4Event = (*static_cast<XMFLOAT3*>(_event->Parameter()));
    emitter.Activate(lifespan, { float4Event.x, float4Event.y, float4Event.z, 0 });
}

void CylinderEmitterListener::HandleEvent(ADEvents::ADEvent* _event)
{
    XMFLOAT3 float4Event = (*static_cast<XMFLOAT3*>(_event->Parameter()));
    emitter.Activate(lifespan, { float4Event.x, float4Event.y, float4Event.z, 0 }, 15.0f);
}

//void AudioStopListener::HandleEvent(ADEvents::ADEvent* _event)
//{
//    audioSource.Stop();
//}

//std::string AudioStopListener::ToString()
//{
//    return std::string();
//}

void BigCloudEmitterListener::HandleEvent(ADEvents::ADEvent* _event)
{
    XMFLOAT3 float4Event = *(static_cast<XMFLOAT3*>(_event->Parameter()));
    emitter.Activate(lifespan, { float4Event.x, float4Event.y, float4Event.z, 0 });
}

std::string BigCloudEmitterListener::ToString()
{
    return "Fountain Emitter";
}

void ApplyEffectListener::SetTarget(ADResource::ADGameplay::GameObject* _target, ADResource::ADGameplay::Effect* _effect)
{
    target = _target;
    effect = _effect;
}

void ApplyEffectListener::HandleEvent(ADEvents::ADEvent* _event)
{
    target->effects.push_back(effect->clone());
    target->effects[target->effects.size() - 1].get()->OnApply(target->GetStatSheet());
}

std::string ApplyEffectListener::ToString()
{
    return effect->name;
}

void SetUIVisibilityListener::HandleEvent(ADEvents::ADEvent* _event)
{
    if (isKeyThreshold)
    {
        if (static_cast<int>(reinterpret_cast<intptr_t>(_event->Parameter())) >= keyIndex)
        {
            uiComponent->visible = isOnSwitch;
        }
        else if (switchFailures)
        {
            uiComponent->visible = !isOnSwitch;
        }
    }
    else
    {
        if (static_cast<int>(reinterpret_cast<intptr_t>(_event->Parameter())) == keyIndex)
        {
            uiComponent->visible = isOnSwitch;
        }
        else if (switchFailures)
        {
            uiComponent->visible = !isOnSwitch;
        }
    }
}

std::string SetUIVisibilityListener::ToString()
{
    std::string value = "UI Visibility Listener: ";
    value.append(uiComponent->name);
    value.append(". With key ");
    value.append(to_string(keyIndex));
    return value;
}

void SetUIFocusListener::HandleEvent(ADEvents::ADEvent* _event)
{
    if (isKeyThreshold)
    {
        if (static_cast<int>(reinterpret_cast<intptr_t>(_event->Parameter())) >= keyIndex)
        {
            uiComponent->Focus(isOnSwitch);
        }
        else if (switchFailures)
        {
            uiComponent->Focus(!isOnSwitch);
        }
    }
    else
    {
        if (static_cast<int>(reinterpret_cast<intptr_t>(_event->Parameter())) == keyIndex)
        {
            uiComponent->Focus(isOnSwitch);
        }
        else if (switchFailures)
        {
            uiComponent->Focus(!isOnSwitch);
        }
    }

}

std::string SetUIFocusListener::ToString()
{
    return "";
}

void SetUISetFrameListener::HandleEvent(ADEvents::ADEvent* _event)
{
    //I probably should put a check here...
    image->SetCurrentFrame(static_cast<int>(reinterpret_cast<intptr_t>(_event->Parameter())));
}

std::string SetUISetFrameListener::ToString()
{
    return "";
}

void SetUITilingListener::SetTarget(ADUI::Image2D* _image, UINT _minimum, UINT _maximum, bool _lerped)
{
    image = _image;
    maximum = _maximum;
    minimum = _minimum;
    lerped = _lerped;
}

void SetUITilingListener::HandleEvent(ADEvents::ADEvent* _event)
{
    float cvalue = static_cast<float>(reinterpret_cast<intptr_t>(_event->Parameter()));
    if (lerped)
    {
        cvalue = ceil((cvalue * 100.f) / float(maximum));
        //value = Lerp(minimum, maximum, value);
    }
    if (cvalue < minimum)
        cvalue = minimum;
    if (cvalue > maximum)
        cvalue = maximum;
    image->tiled = cvalue;
}

std::string SetUITilingListener::ToString()
{
    return "";
}

void SetUISelectionListener::HandleEvent(ADEvents::ADEvent* _event)
{
    int value = static_cast<int>(reinterpret_cast<intptr_t>(_event->Parameter()));
    if (value < 0)
        value = 0;
    list->SetSelected(value);
}

std::string SetUISelectionListener::ToString()
{
    return std::string();
}

void SetUITextListener::HandleEvent(ADEvents::ADEvent* _event)
{
    label->output.clear();
    label->output.append(*(static_cast<std::string*>(_event->Parameter())));
}

std::string SetUITextListener::ToString()
{
    return std::string();
}

void MinionCountListener::HandleEvent(ADEvents::ADEvent* _event)
{
    scene->UpdateMinionCounts(static_cast<int>(reinterpret_cast<intptr_t>(_event->Parameter())));
}

std::string MinionCountListener::ToString()
{
    return std::string();
}

void VillagerDeathListener::HandleEvent(ADEvents::ADEvent* _event)
{
    scene->UpdateVillagerCount();
}

std::string VillagerDeathListener::ToString()
{
    return std::string();
}

