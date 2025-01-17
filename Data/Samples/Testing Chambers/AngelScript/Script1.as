class Object : ezAsScriptComponent
{
    private int value = 0;
    float LightIntensity = 100;

    ezGameObject@ GetOwner()
    {
        return GetScriptOwnerObject(@this);
    }

    void OnMsgTriggerTriggered(ezMsgTriggerTriggered@ msg)
    {
        ezLog::Info("Trigger Message Handler!!");

        ezPointLightComponent@ comp;
        if (GetOwner().TryGetComponentOfBaseType(@comp))
        {
            comp.LightColor = ezColor::Red;
            comp.Intensity = LightIntensity;
        }
    }

    void Update()
    {
        // ezLog::Info("Update !");

        ezComponent@ ownerComp = GetScriptOwnerComponent(@this);
        ezGameObject@ ownerObj = GetScriptOwnerObject(@this);
        ezVec3 localPos = ownerObj.GetLocalPosition();
        ezWorld@ world = ownerObj.GetWorld();

        ezScriptComponent@ ownerScript = cast<ezScriptComponent@>(ownerComp);

        ++value;
        // ezLog::Info("Update: " + value + " - " + ownerComp.GetUniqueID());

        ownerObj.SetLocalPosition(localPos + ezVec3(0, 0, 0.01));

        if (value == 10)
        {
            ezLog::Info("10 !");
        }

        if (value == 20)
        {
            ezLog::Info("20 !");

            const ezRTTI@ pRtti = ezRTTI::GetType("ezPointLightComponent");

            ezComponent@ comp;
            if (ownerObj.TryGetComponentOfBaseType(pRtti, comp))
            {
                ezLog::Info("Found ezPointLightComponent!");
                auto derivedComp = cast<ezPointLightComponent@>(comp);

                ezColor col = ezColor::CornflowerBlue;
                derivedComp.LightColor = col;
                derivedComp.Intensity = 500;
            }
        }
    }
}