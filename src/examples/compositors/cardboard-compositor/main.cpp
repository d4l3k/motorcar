/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Compositor.
**
** $QT_BEGIN_LICENSE:BSD$
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
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
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
** $QT_END_LICENSE$
**
****************************************************************************/

#include <motorcar.h>


int main(int argc, char *argv[])
{

    motorcar::Scene *scene = new motorcar::Scene();

    motorcar::Compositor *compositor = motorcar::Compositor::createCompositor(argc, argv, scene) ;
    scene->setCompositor(compositor);

    scene->setWindowManager( new motorcar::WindowManager(scene, compositor->defaultSeat()));

    motorcar::OpenGLContext *context = compositor->getContext();

    motorcar::Skeleton *skeleton = new motorcar::Skeleton(scene);


    std::cout << "Using Default Display" << std::endl;

    int HResolution = 1280;
    int VResolution  = 800 ;
    float HScreenSize = .14976;
    float VScreenSize = .0936;
    float VScreenCenter = VScreenSize/2;
    float EyeToScreenDistance = 0.041f;
    float LensSeparationDistance = .064;
    float InterpupillaryDistance = .0647;
    glm::vec4 DistortionK = glm::vec4(1, .22, .24, 0);
    float scaleFactor = 1.25;

    float near = .01f, far = 10.0f;

    float h_meters = HScreenSize / 4.0f - LensSeparationDistance / 2.0f;
    float h = (4.0f * h_meters) / HScreenSize ;

    motorcar::Display *display = new motorcar::Display(context, glm::vec2(0.325f, 0.1f), scene, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, .0f, 1.25f)));
    /*display->addViewpoint(new motorcar::ViewPoint( .01f, 1000.0f, display, display, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, camToDisplayDistance))));
    compositor->setDisplay(display);
    scene->addDisplay(compositor->display());*/

    motorcar::ViewPoint *lCam = new motorcar::ViewPoint(near, far, display, display,
                                     glm::translate(glm::mat4(), glm::vec3(-InterpupillaryDistance/2, VScreenSize/2 - VScreenCenter, EyeToScreenDistance)),
                                     glm::vec4(0.0f,0.0f,.5f,1.0f), glm::vec3(h, 0.0f, 0.0f));

    motorcar::ViewPoint *rCam = new motorcar::ViewPoint(near, far, display, display,
                                     glm::translate(glm::mat4(), glm::vec3(InterpupillaryDistance/2.0f, VScreenSize/2 - VScreenCenter, EyeToScreenDistance)),
                                     glm::vec4(.5f,0.0f,.5f,1.0f), glm::vec3(-h, 0.0f, 0.0f));

      display->addViewpoint(lCam);
      display->addViewpoint(rCam);


    std::cout << "Starting Compositor "<<std::endl;


    int result = compositor->start();

    delete scene;

    return result;
}
