class HealthPickup :  ezAsScriptComponent
{
    ezGameObject@ GetOwner()
    {
        return GetScriptOwnerObject(@this);
    }

    void OnMsgGenericEvent(ezMsgGenericEvent@ msg)
    {
        if (msg.Message != "Animation Cue 1")
           return;

        ezGameObject@ pfxObj = GetOwner().FindChildByName("Particle", true);

        if (@pfxObj !is null)
        {
            ezParticleComponent@ pfxComp;
            if (pfxObj.TryGetComponentOfBaseType(@pfxComp))
            {
                pfxComp.StartEffect();
            }
        }
    }

    void OnMsgTriggerTriggered(ezMsgTriggerTriggered@ msg)
    {
        if (msg.TriggerState == ezTriggerState::Activated && msg.Message == "Pickup")
        {
            ezGameObject@ owner = GetOwner();

            ezParticleComponent@ pfxComp;
            if (owner.TryGetComponentOfBaseType(@pfxComp))
            {
                pfxComp.StartEffect();
            }

            ezMsgDeleteGameObject msgDel;
            owner.PostMessage(msgDel, ezTime_MakeFromSeconds(0.1));
        }
    }
} 