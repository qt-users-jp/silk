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

silkrc.files = silkrc.default
silkrc.path = /etc
silkrc.CONFIG = no_check_exist
INSTALLS += silkrc

