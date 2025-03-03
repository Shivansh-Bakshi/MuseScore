/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MU_ZERBERUS_MINSTRUMENT_H
#define MU_ZERBERUS_MINSTRUMENT_H

#include <list>
#include <QString>

class MQZipReader;

namespace mu::zerberus {
class Zerberus;
struct Zone;
struct SfzRegion;
class Sample;

//---------------------------------------------------------
//   ZInstrument
//---------------------------------------------------------

class ZInstrument
{
    Zerberus* zerberus;
    int _refCount;
    QString _name;
    int _program;
    QString instrumentPath;
    std::list<Zone*> _zones;
    int _setcc[128];

    bool loadFromFile(const QString&);
    bool loadSfz(const QString&);
    bool loadFromDir(const QString&);
    bool read(const QByteArray&, MQZipReader*, const QString& path);

public:
    ZInstrument(Zerberus*);
    ~ZInstrument();

    int refCount() const { return _refCount; }
    void setRefCount(int val) { _refCount = val; }
    bool load(const QString&);
    int program() const { return _program; }
    QString name() const { return _name; }
    QString path() const { return instrumentPath; }
    const std::list<Zone*>& zones() const { return _zones; }
    std::list<Zone*>& zones() { return _zones; }
    Sample* readSample(const QString& s, MQZipReader* uz);
    void addZone(Zone* z) { _zones.push_back(z); }
    void addRegion(SfzRegion&);
    int getSetCC(int v) { return _setcc[v]; }

    static QByteArray buf;    // used during read of Sample
    static int idx;
};
}

#endif //MU_ZERBERUS_MINSTRUMENT_H
