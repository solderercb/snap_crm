function Component()
{
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/snap.exe", "@StartMenuDir@/SNAP CRM.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/snap.exe",
            "iconId=0", "description=Launch SNAP CRM application");
        component.addOperation("CreateShortcut", "@TargetDir@/maintenancetool.exe", "@StartMenuDir@/Uninstall.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/maintenancetool.exe",
            "iconId=0", "description=Uninstall SNAP CRM application");
    }
}
