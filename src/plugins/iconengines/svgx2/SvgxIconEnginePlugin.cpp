#include "SvgxIconEnginePlugin.h"

#include "SvgxIconEngine.h"

QIconEngine *SvgxIconEnginePlugin::create(const QString &fileName) {
    auto engine = new SvgxIconEngine();
    if (fileName == ".svgx") {
        return engine;
    }

    // TODO

    return engine;
}