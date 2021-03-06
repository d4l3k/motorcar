/****************************************************************************
**This file is part of the Motorcar 3D windowing framework
**
**
**Copyright (C) 2014 Forrest Reiling
**
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**
****************************************************************************/
#ifndef SIXENSECONTROLLERNODE_H
#define SIXENSECONTROLLERNODE_H
#include <scenegraph/input/sixdofpointingdevice.h>
#include <scenegraph/input/singlebonetracker.h>
#include <sixense.h>

namespace motorcar {
class SixenseControllerNode : public PhysicalNode
{
public:
    SixenseControllerNode(int controllerIndex, PhysicalNode *parent, const glm::mat4 &transform = glm::mat4());

    void updateState(sixenseControllerData data);

    int controllerIndex() const;
    void setControllerIndex(int controllerIndex);

    bool enabled() const;
    void setEnabled(bool enabled);


    SixDOFPointingDevice *pointingDevice() const;
    void setPointingDevice(SixDOFPointingDevice *pointingDevice);

    SingleBoneTracker *boneTracker() const;
    void setBoneTracker(SingleBoneTracker *boneTracker);

private:
    SixDOFPointingDevice *m_pointingDevice;
    SingleBoneTracker *m_boneTracker;
    int m_controllerIndex;
    bool m_enabled;
    bool m_bumperDown;

    glm::vec3 m_filteredPos;
    float m_filterConstant;
};
}


#endif // SIXENSECONTROLLERNODE_H
