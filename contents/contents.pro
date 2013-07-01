TEMPLATE = app
TARGET = phony_target
CONFIG -= qt
QT =
LIBS =

CONFIG += console
PHONY_DEPS = .
phony_src.input = PHONY_DEPS
phony_src.output = phony.c
phony_src.variable_out = GENERATED_SOURCES
phony_src.commands = echo int main\\(\\) { return 0\\; } > phony.c
phony_src.name = CREATE phony.c
phony_src.CONFIG += combine
QMAKE_EXTRA_COMPILERS += phony_src

include(../silk.pri)
root.source = root
root.target = $${SILK_DATA_PATH}

tasks.source = tasks
tasks.target = $${SILK_DATA_PATH}

DEPLOYMENTFOLDERS = root tasks

include(../silkdeployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    root/modules.qml \
    root/getting-started.qml \
    root/scaffolding/HtmlExample.qml \
    root/scaffolding/html.qml
