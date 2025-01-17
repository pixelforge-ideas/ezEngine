class ScriptObject :  ezAsScriptComponent
{
    //ezStringView Text = "Test";

    void OnMsgPhysicsJointBroke(ezMsgPhysicsJointBroke@ msg)
    {
        ezLog::Info("Test");
    }
}
