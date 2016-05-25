TEMPLATE = aux

include(../silk.pri)
root.source = root
root.target = $${SILK_DATA_PATH}

daemons.source = daemons
daemons.target = $${SILK_DATA_PATH}

DEPLOYMENTFOLDERS = root daemons

silk_platform_linux {
    target.path = $$PREFIX/
}

include(../silkdeployment.pri)
qtcAddDeployment()
