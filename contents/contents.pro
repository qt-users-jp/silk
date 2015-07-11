TEMPLATE = aux

include(../silk.pri)
root.source = root
root.target = $${SILK_DATA_PATH}

tasks.source = tasks
tasks.target = $${SILK_DATA_PATH}

DEPLOYMENTFOLDERS = root tasks

silk_platform_linux {
    target.path = $$PREFIX/
}

include(../silkdeployment.pri)
qtcAddDeployment()
