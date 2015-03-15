/****************************************************************************
**This file is part of the MotorCar QtWayland Compositor, derived from the
**QtWayland QWindow Reference Compositor
**
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
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
**
****************************************************************************/

#include <qt/qtwaylandmotorcarcompositor.h>
#include <qt/qtwaylandmotorcarsurface.h>
#include <qt/qtwaylandmotorcarseat.h>
#include <sys/time.h>


#include <QtCompositor/private/qwlsurface_p.h>
#include <QtCompositor/private/qwlcompositor_p.h>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QTouchEvent>
#include <QGuiApplication>
#include <QCursor>
#include <QPixmap>
#include <QScreen>
#include <QProcess>

#include <iostream>
#include <vector>

#include <QtCompositor/qwaylandinput.h>

using namespace qtmotorcar;

QtWaylandMotorcarCompositor::QtWaylandMotorcarCompositor(QOpenGLWindow *window, QGuiApplication *app, motorcar::Scene * scene)
    : QWaylandCompositor(window, 0, DefaultExtensions | SubSurfaceExtension)
    , m_scene(scene)
    , m_glData(new OpenGLData(window))//glm::rotate(glm::translate(glm::mat4(1), glm::vec3(0,0,1.5f)), 180.f, glm::vec3(0,1,0)))))
    , m_renderScheduler(this)
    , m_draggingWindow(0)
    , m_dragKeyIsPressed(false)
    , m_cursorSurface(NULL)
//    , m_cursorSurfaceNode(NULL)
//    , m_cursorMotorcarSurface(NULL)
    , m_cursorHotspotX(0)
    , m_cursorHotspotY(0)
    , m_modifiers(Qt::NoModifier)
    , m_app(app)
    , m_defaultSeat(NULL)
    , m_frames(0)

{
    setDisplay(NULL);

    m_renderScheduler.setSingleShot(true);
    //m_renderScheduler.setInterval(16);
    connect(&m_renderScheduler,SIGNAL(timeout()),this,SLOT(render()));


    window->installEventFilter(this);

    setRetainedSelectionEnabled(true);

    setOutputGeometry(QRect(QPoint(0, 0), window->size()));
    setOutputRefreshRate(qRound(qGuiApp->primaryScreen()->refreshRate() * 1000.0));

    m_defaultSeat = new QtWaylandMotorcarSeat(this->defaultInputDevice());


//    motorcar::Display testDisplay(window_context, glm::vec2(1), *m_scene, glm::mat4(1));
//    for(int i = 0; i < 2 ; i++){
//        for(int j = 0; j < 2; j++){
//            motorcar::Geometry::printVector(testDisplay.worldPositionAtDisplayPosition(glm::vec2(i * window->size().width(), j * window->size().height())));
//        }
//    }

    //glClearDepth(0.1f);
}

QtWaylandMotorcarCompositor::~QtWaylandMotorcarCompositor()
{
    delete m_glData;
}

QtWaylandMotorcarCompositor *QtWaylandMotorcarCompositor::create(int argc, char** argv, motorcar::Scene *scene)
{
    // Enable the following to have touch events generated from mouse events.
    // Very handy for testing touch event delivery without a real touch device.
    // QGuiApplication::setAttribute(Qt::AA_SynthesizeTouchForUnhandledMouseEvents, true);

    QGuiApplication *app = new QGuiApplication(argc, argv);
    QScreen *screen = NULL;



    //screen = QGuiApplication::primaryScreen();
    screen = QGuiApplication::screens().back();

    QRect screenGeometry = screen->geometry();


    QSurfaceFormat format;
//    std::cout << "color buffer size: " << format.redBufferSize() << std::endl;
//    format.setRedBufferSize(8);
//    format.setGreenBufferSize(8);
//    format.setBlueBufferSize(8);
//    format.setAlphaBufferSize(8);
    format.setDepthBufferSize(8);
    format.setStencilBufferSize(8);
    format.setSwapInterval(1);
    format.setStencilBufferSize(8);

    //QRect geom = screenGeometry;
//    if (QCoreApplication::arguments().contains(QLatin1String("-nofullscreen")))
//        geom = QRect(screenGeometry.width() / 4, screenGeometry.height() / 4,
//                     screenGeometry.width() / 2, screenGeometry.height() / 2);

    QOpenGLWindow *window = new QOpenGLWindow(format, screenGeometry);
    return  new QtWaylandMotorcarCompositor(window, app, scene);
}

int QtWaylandMotorcarCompositor::start()
{
    this->glData()->m_window->showFullScreen();
    this->cleanupGraphicsResources();
    int result = m_app->exec();
    delete m_app;
    return result;
}

motorcar::OpenGLContext *QtWaylandMotorcarCompositor::getContext()
{
    return new QtWaylandMotorcarOpenGLContext(this->glData()->m_window);
}

wl_display *QtWaylandMotorcarCompositor::wlDisplay()
{
    return QWaylandCompositor::waylandDisplay();
}

motorcar::WaylandSurface *QtWaylandMotorcarCompositor::getSurfaceFromResource(wl_resource *resource)
{
    QWaylandSurface *surface = QtWayland::Surface::fromResource(resource)->waylandSurface();
    std::cout << "got surface from resource: " << surface <<std::endl;

    QtWaylandMotorcarSurface *motorsurface = this->getMotorcarSurface(surface);
    if(motorsurface == NULL){
        std::cout << "Warning: surface has not been created, creating now " << surface <<std::endl;
        motorsurface = new QtWaylandMotorcarSurface(surface, this, motorcar::WaylandSurface::SurfaceType::NA);
    }

    return motorsurface;

}




OpenGLData *QtWaylandMotorcarCompositor::glData() const
{
    return m_glData;
}

void QtWaylandMotorcarCompositor::setGlData(OpenGLData *glData)
{
    m_glData = glData;
}


motorcar::Scene *QtWaylandMotorcarCompositor::scene() const
{
    return m_scene;
}

void QtWaylandMotorcarCompositor::setScene(motorcar::Scene *scene)
{
    m_scene = scene;
}

QtWaylandMotorcarSurface *QtWaylandMotorcarCompositor::getMotorcarSurface(QWaylandSurface *surface) const
{
    //if passed NULL return first Surface
    if(surface == NULL){
        if(!m_surfaceMap.empty()){
            return m_surfaceMap.begin()->second;
        }else{
            return NULL;
        }
    }
    if(!m_surfaceMap.empty() && m_surfaceMap.count(surface)){
        auto it = m_surfaceMap.find(surface);
        if(it != m_surfaceMap.end()){
            return it->second;
        }

    }
    return NULL;

}



//TODO: consider revising to take  MotorcarSurfaceNode as argument depending on call sites
void QtWaylandMotorcarCompositor::ensureKeyboardFocusSurface(QWaylandSurface *oldSurface)
{
//    QWaylandSurface *kbdFocus = defaultInputDevice()->keyboardFocus();
//    windowFocus = topLevelSurfaces.size() - 1;
//    if (kbdFocus == oldSurface || !kbdFocus){
//        motorcar::WaylandSurfaceNode *n = this->getSurfaceNode();
//        // defaultInputDevice()->setKeyboardFocus(m_surfaces.isEmpty() ? 0 : m_surfaces.last());
//        if(n){
//            defaultInputDevice()->setKeyboardFocus(static_cast<QtWaylandMotorcarSurface *>(n->surface())->surface());
//        }else{
//            defaultInputDevice()->setKeyboardFocus(NULL);
//        }

//    }
}



void QtWaylandMotorcarCompositor::surfaceDestroyed(QObject *object)
{

    QWaylandSurface *surface = static_cast<QWaylandSurface *>(object);
    //m_surfaces.removeOne(surface);
    if(surface != NULL){ //because calling getSurfaceNode with NULL will cause the first surface node to be returned
        motorcar::WaylandSurface *motorsurface = this->getMotorcarSurface(surface); //will return surfaceNode whose destructor will remove it from the scenegraph
        if(motorsurface != NULL){
            this->scene()->windowManager()->destroySurface(motorsurface);
            m_surfaceMap.erase (surface);

            for (unsigned i=0; i<topLevelSurfaces.size(); ++i) {
              if (topLevelSurfaces[i] == surface) {
                topLevelSurfaces.erase(topLevelSurfaces.begin() + i);
              }
            }
        }

    }
//    ensureKeyboardFocusSurface(surface);
//    //m_renderScheduler.start(0);
}

void QtWaylandMotorcarCompositor::surfaceMapped()
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());

    std::cout << "mapped surface: " << surface << std::endl;
    std::cout << "shell surface: " << surface->hasShellSurface() << std::endl;

    QPoint pos;
    //if (!m_surfaces.contains(surface)) {

        //surface->setPos(QPoint(0, 0));
        if (surface->hasShellSurface() || true) {

            motorcar::WaylandSurface::SurfaceType surfaceType;

            int type = static_cast<int>(surface->windowType());
            float popupZOffset = 0.05f;


            if(type == QWaylandSurface::WindowType::Toplevel){
                surfaceType = motorcar::WaylandSurface::SurfaceType::TOPLEVEL;
            }else if(type == QWaylandSurface::WindowType::Popup){
                surfaceType = motorcar::WaylandSurface::SurfaceType::POPUP;
            }else if(type == QWaylandSurface::WindowType::Transient){
                surfaceType = motorcar::WaylandSurface::SurfaceType::TRANSIENT;
            }else{
                surfaceType = motorcar::WaylandSurface::SurfaceType::NA;
            }

            QtWaylandMotorcarSurface *motorsurface = this->getMotorcarSurface(surface);
            if(motorsurface == NULL){
                //if it is not present for some weird reason just go ahead and create it for good measure
                std::cout << "Warning: qwaylandsurface was mapped but motorcar surface does not exist yet, creating now" <<std::endl;
//                surfaceCreated(surface);
//                surfaceNode = this->getSurfaceNode(surface);
                motorsurface = new QtWaylandMotorcarSurface(surface, this, surfaceType);

                 m_surfaceMap.insert(std::pair<QWaylandSurface *, QtWaylandMotorcarSurface *>(surface, motorsurface));

            }
//            if((motorsurface->type() == motorcar::WaylandSurface::SurfaceType::CUBOID ||
//                motorsurface->type() == motorcar::WaylandSurface::SurfaceType::PORTAL)
//                 && surfaceType == motorcar::WaylandSurface::SurfaceType::TOPLEVEL){
//                std::cout << "Warning: ignoring request to remap a 3D surface to a top level surface " <<std::endl;
//            }else{
                this->scene()->windowManager()->mapSurface(motorsurface, surfaceType);
//            }





            //defaultInputDevice()->setKeyboardFocus(surface);

    }




    //m_renderScheduler.start(0);
}

void QtWaylandMotorcarCompositor::surfaceUnmapped()
{

    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
    //    if (m_surfaces.removeOne(surface))
    //        m_surfaces.insert(0, surface);


    if(surface != NULL){
        motorcar::WaylandSurface *motorsurface = this->getMotorcarSurface(surface);
        if(motorsurface != NULL){
            this->scene()->windowManager()->unmapSurface(motorsurface);
        }else{
            std::cout << "Warning: surface unmapped but doesnt have associated surfaceNode" <<std::endl;
        }

    }



    ensureKeyboardFocusSurface(surface);
    //m_renderScheduler.start(0);
}

void QtWaylandMotorcarCompositor::surfaceDamaged()
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());

    if(surface != NULL){
        motorcar::WaylandSurface *motorsurface = this->getMotorcarSurface(surface);
        if(motorsurface != NULL){
            //surfaceNode->setDamaged(true);
        }else{
          //  std::cout << "Warning: surface damaged but doesnt have associated surfaceNode" <<std::endl;
        }

    }

    surfaceDamaged(surface);
}

void QtWaylandMotorcarCompositor::surfacePosChanged()
{
    //m_renderScheduler.start(0);
}

void QtWaylandMotorcarCompositor::surfaceDamaged(QWaylandSurface *surface)
{
    Q_UNUSED(surface)
   // Q_UNUSED(rect)
    m_renderScheduler.start(0);
}

void QtWaylandMotorcarCompositor::surfaceCreated(QWaylandSurface *surface)
{
    connect(surface, SIGNAL(destroyed(QObject *)), this, SLOT(surfaceDestroyed(QObject *)));
    connect(surface, SIGNAL(mapped()), this, SLOT(surfaceMapped()));
    connect(surface, SIGNAL(unmapped()), this, SLOT(surfaceUnmapped()));
    connect(surface, SIGNAL(committed()), this, SLOT(surfaceDamaged()));
    connect(surface, SIGNAL(extendedSurfaceReady()), this, SLOT(sendExpose()));
    connect(surface, SIGNAL(posChanged()), this, SLOT(surfacePosChanged()));

    std::cout << "created surface: " << surface << std::endl;

    QtWaylandMotorcarSurface *motorsurface = new QtWaylandMotorcarSurface(surface, this, motorcar::WaylandSurface::SurfaceType::NA);
    m_surfaceMap.insert(std::pair<QWaylandSurface *, QtWaylandMotorcarSurface *>(surface, motorsurface));

    topLevelSurfaces.push_back(surface);
    windowFocus = topLevelSurfaces.size() - 1;

//    if(surface->hasShellSurface()){
//        motorcar::WaylandSurfaceNode *surfaceNode = this->scene()->windowManager()->createSurface();
//        std::cout << "created surfaceNode " << surfaceNode << std::endl;
//        m_surfaceMap.insert(std::pair<QWaylandSurface *, motorcar::WaylandSurfaceNode *>(surface, surfaceNode));
//    }

    //surface->handle()->

    //m_renderScheduler.start(0);
}

void QtWaylandMotorcarCompositor::sendExpose()
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
    surface->sendOnScreenVisibilityChange(true);
}



QPointF QtWaylandMotorcarCompositor::toSurface(QWaylandSurface *surface, const QPointF &point) const
{
//    motorcar::WaylandSurfaceNode *surfaceNode = this->getSurfaceNode(surface);

//    if(surfaceNode != NULL){
//        motorcar::Geometry::Ray ray = display()->worldRayAtDisplayPosition(glm::vec2(point.x(), point.y()));
//        ray = ray.transform(glm::inverse(surfaceNode->worldTransform()));
//        float t;
//        glm::vec2 intersection;
//        bool isIntersected = surfaceNode->computeLocalSurfaceIntersection(ray, intersection, t);
//        if(isIntersected){
//            return QPointF(intersection.x, intersection.y);
//        }else{
//            qDebug() << "ERROR: surface plane does not interesect camera ray through cursor";
//            return QPointF();
//        }

//    }else{
//        qDebug() << "ERROR: could not find SceneGraphNode for the given Surface";
//        return QPointF();
//    }
    return QPointF();
}

motorcar::Seat *QtWaylandMotorcarCompositor::defaultSeat() const
{
    return m_defaultSeat;
}

void QtWaylandMotorcarCompositor::setDefaultSeat(QtWaylandMotorcarSeat *defaultSeat)
{
    m_defaultSeat = defaultSeat;
}


void QtWaylandMotorcarCompositor::updateCursor()
{
    if (!m_cursorSurface)
        return;
    QCursor cursor(QPixmap::fromImage(m_cursorSurface->image()), m_cursorHotspotX, m_cursorHotspotY);
    static bool cursorIsSet = false;
    if (cursorIsSet) {
        QGuiApplication::changeOverrideCursor(cursor);
    } else {
        QGuiApplication::setOverrideCursor(cursor);
        cursorIsSet = true;
    }
}




void QtWaylandMotorcarCompositor::setCursorSurface(QWaylandSurface *surface, int hotspotX, int hotspotY)
{

    if(m_defaultSeat->pointer()->cursorNode() == NULL){
        QtWaylandMotorcarSurface *cursorMotorcarSurface =new QtWaylandMotorcarSurface(surface, this, motorcar::WaylandSurface::SurfaceType::CURSOR);
        //m_cursorSurfaceNode =  new motorcar::WaylandSurfaceNode(m_cursorMotorcarSurface, m_scene, glm::rotate(glm::mat4(1), -90.f, glm::vec3(0, 1, 0)));
        motorcar::WaylandSurfaceNode *cursorSurfaceNode = this->scene()->windowManager()->createSurface(cursorMotorcarSurface);
        //cursorSurfaceNode->setTransform(glm::rotate(glm::mat4(1), -90.f, glm::vec3(0, 1, 0)));
        m_surfaceMap.insert(std::pair<QWaylandSurface *, QtWaylandMotorcarSurface *>(surface, cursorMotorcarSurface));
        m_defaultSeat->pointer()->setCursorNode(cursorSurfaceNode);
        std::cout << "created cursor surface node " << cursorSurfaceNode << std::endl;
    }
    if(!surface){
        std::cout << "cursor surface set to NULL" <<std::endl;
        delete m_defaultSeat->pointer()->cursorNode();
         m_defaultSeat->pointer()->setCursorNode(NULL);
    }else{
        (static_cast<QtWaylandMotorcarSurface *>(m_defaultSeat->pointer()->cursorNode()->surface()))->setSurface(surface);
        m_defaultSeat->pointer()->setCursorHotspot(glm::ivec2(hotspotX, hotspotY));
    }

    if ((m_cursorSurface != surface) && surface){
        connect(surface, SIGNAL(committed()), this, SLOT(updateCursor()));
    }

    m_cursorSurface = surface;
    m_cursorHotspotX = hotspotX;
    m_cursorHotspotY = hotspotY;
}


QWaylandSurface *QtWaylandMotorcarCompositor::surfaceAt(const QPointF &point, QPointF *local)
{
    motorcar::Geometry::Ray ray = display()->worldRayAtDisplayPosition(glm::vec2(point.x(), point.y()));
    motorcar::Geometry::RaySurfaceIntersection *intersection = m_scene->intersectWithSurfaces(ray);

    if(intersection){
        qDebug() << "intersection found between cursor ray and scene graph";
        if (local){
            *local = QPointF(intersection->surfaceLocalCoordinates.x, intersection->surfaceLocalCoordinates.y);
        }
        motorcar::WaylandSurface *surface = intersection->surfaceNode->surface();
        delete intersection;

        return static_cast<QtWaylandMotorcarSurface *>(surface)->surface();

    }else{
        return NULL;
    }
}


void QtWaylandMotorcarCompositor::render()
{
    m_glData->m_window->makeCurrent();
    frameStarted();
    cleanupGraphicsResources();


    scene()->drawFrame();
    scene()->finishFrame();

    scene()->prepareForFrame(this->handle()->currentTimeMsecs());
    sendFrameCallbacks(surfaces());


    //frameFinished();

    m_glData->m_window->swapBuffers();

    struct timeval tv;
    static const int32_t benchmark_interval = 5;
    gettimeofday(&tv, NULL);
        uint32_t time = this->handle()->currentTimeMsecs();//tv.tv_sec * 1000 + tv.tv_usec / 1000;
        if (m_frames == 0)
          m_benchmark_time = time;
        if (time - m_benchmark_time > (benchmark_interval * 1000)) {
          std::cout << m_frames << " frames in " << benchmark_interval
                    << " seconds: " << (float)m_frames / benchmark_interval
                    << std::endl;
          m_benchmark_time = time;
          m_frames = 0;
        }

        m_frames++;


//    glFlush();
//    glFinish();

    //if(this->surfaces().empty()){
        m_renderScheduler.start(16);
    //}

    // N.B. Never call glFinish() here as the busylooping with vsync 'feature' of the nvidia binary driver is not desirable.

}

bool QtWaylandMotorcarCompositor::eventFilter(QObject *obj, QEvent *event)
{
    if (obj != m_glData->m_window)
        return false;

    QWaylandInputDevice *input = defaultInputDevice();

    switch (event->type()) {
    case QEvent::Expose:
        m_renderScheduler.start(0);
        if (m_glData->m_window->isExposed()) {
            // Alt-tabbing away normally results in the alt remaining in
            // pressed state in the clients xkb state. Prevent this by sending
            // a release. This is not an issue in a "real" compositor but
            // is very annoying when running in a regular window on xcb.
            Qt::KeyboardModifiers mods = QGuiApplication::queryKeyboardModifiers();
            if (m_modifiers != mods && input->keyboardFocus()) {
                Qt::KeyboardModifiers stuckMods = m_modifiers ^ mods;
                if (stuckMods & Qt::AltModifier)
                    input->sendKeyReleaseEvent(64); // native scancode for left alt
                m_modifiers = mods;
            }
        }
        break;
    case QEvent::MouseButtonPress: {
        QPointF local;
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        QWaylandSurface *targetSurface = surfaceAt(me->localPos(), &local);
        m_lastPos = me->localPos();
        if (m_dragKeyIsPressed && targetSurface) {
            m_draggingWindow = targetSurface;
            m_drag_diff = local;
        } else {
            if (targetSurface && input->keyboardFocus() != targetSurface) {
                input->setKeyboardFocus(targetSurface);
                //                m_surfaces.removeOne(targetSurface);
                //                m_surfaces.append(targetSurface);
                //m_renderScheduler.start(0);
            }
            input->sendMousePressEvent(me->button(), local, me->localPos());
        }
        return true;
    }
    case QEvent::MouseButtonRelease: {
        QWaylandSurface *targetSurface = input->mouseFocus();
        if (m_draggingWindow) {
            m_draggingWindow = 0;
            m_drag_diff = QPointF();
        } else {
            QMouseEvent *me = static_cast<QMouseEvent *>(event);
            QPointF localPos;
            if (targetSurface)
                localPos = toSurface(targetSurface, me->localPos());
            input->sendMouseReleaseEvent(me->button(), localPos, me->localPos());
        }
        return true;
    }
    case QEvent::MouseMove: {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        if (m_draggingWindow) {
            m_draggingWindow->setPos(me->localPos() - m_drag_diff);
            //m_renderScheduler.start(0);
        } else if(m_dragKeyIsPressed && m_lastPos.x() != 0) {
            double x = me->localPos().x() - m_lastPos.x();
            double y = me->localPos().y() - m_lastPos.y();
            motorcar::Display *d = this->scene()->compositor()->display();
            float angleX = -(float)(x)/200.0f;
            float angleY = -(float)(y)/200.0f;
            glm::mat4 controllerTransform = glm::rotate(glm::rotate(d->transform(), angleX, glm::vec3(0, 1, 0)), angleY, glm::vec3(1, 0, 0));
            d->setTransform(controllerTransform);
        } else {
            QPointF local;
            QWaylandSurface *targetSurface = surfaceAt(me->localPos(), &local);
            input->sendMouseMoveEvent(targetSurface, local, me->localPos());
        }
        m_lastPos = me->localPos();
        break;
    }
    case QEvent::Wheel: {
        QWheelEvent *we = static_cast<QWheelEvent *>(event);
        input->sendMouseWheelEvent(we->orientation(), we->delta());
        break;
    }
    case QEvent::KeyPress: {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Meta || ke->key() == Qt::Key_Super_L) {
            std::cout << "DRAG KEY" << std::endl;
            m_dragKeyIsPressed = true;
        } else if (ke->key() == Qt::Key_Return && m_dragKeyIsPressed) { // Mod - Enter to launch terminal
            QProcess *process = new QProcess(this);
            QString program = "weston-terminal";
            process->startDetached(program, QStringList());
            break;
        } else if (ke->key() == Qt::Key_Tab && m_dragKeyIsPressed) { // Mod - Tab to change focus
            std::vector<motorcar::WaylandSurface *> surfaces = this->scene()->windowManager()->getSurfaces();
            int size = surfaces.size();
            std::cout << "WINDOW COUNT " << size << " i " << windowFocus << std::endl;

            windowFocus += 1;
            if (windowFocus >= size) {
              windowFocus = 0;
            }
            if (size > 0) {
              //input->setKeyboardFocus(this->scene()->childNodes()[windowFocus]);
              //input->setMouseFocus(this->scene()->childNodes()[windowFocus], m_lastPos, m_lastPos);

              //this->defaultSeat()->setPointerFocus(surfaces[windowFocus], glm::vec2());
              this->defaultSeat()->setKeyboardFocus(surfaces[windowFocus]);
              //this->scene()->windowManager()->ensureKeyboardFocusIsValid(surfaces[windowFocus]);
            }
            break;
        }/* else if(ke->key() == Qt::Key_Up){
            m_glData->m_cameraNode->setTransform(glm::translate(glm::mat4(1), glm::vec3(0,0,0.001f)) * m_glData->m_cameraNode->transform());
        }else if(ke->key() == Qt::Key_Down){
            m_glData->m_cameraNode->setTransform(glm::translate(glm::mat4(1), glm::vec3(0,0,-0.001f)) * m_glData->m_cameraNode->transform());
        }*/
        m_modifiers = ke->modifiers();
        QWaylandSurface *targetSurface = input->keyboardFocus();
        if (targetSurface)
           // input->sendKeyPressEvent(ke->nativeScanCode());
          this->scene()->windowManager()->sendEvent(motorcar::KeyboardEvent(motorcar::KeyboardEvent::Event::KEY_PRESS, ke->nativeScanCode(), defaultSeat()));
          break;
    }
    case QEvent::KeyRelease: {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Meta || ke->key() == Qt::Key_Super_L) {
            m_dragKeyIsPressed = false;
        }
        m_modifiers = ke->modifiers();
        QWaylandSurface *targetSurface = input->keyboardFocus();
        if (targetSurface)
           // input->sendKeyReleaseEvent(ke->nativeScanCode());
        this->scene()->windowManager()->sendEvent(motorcar::KeyboardEvent(motorcar::KeyboardEvent::Event::KEY_RELEASE, ke->nativeScanCode(), defaultSeat()));
        break;
    }
        //    case QEvent::TouchBegin:
        //    case QEvent::TouchUpdate:
        //    case QEvent::TouchEnd:
        //    {
        //        QWaylandSurface *targetSurface = 0;
        //        QTouchEvent *te = static_cast<QTouchEvent *>(event);
        //        QList<QTouchEvent::TouchPoint> points = te->touchPoints();
        //        QPoint pointPos;
        //        if (!points.isEmpty()) {
        //            pointPos = points.at(0).pos().toPoint();
        //            targetSurface = surfaceAt(pointPos);
        //        }
        //        if (targetSurface && targetSurface != input->mouseFocus())
        //            input->setMouseFocus(targetSurface, pointPos, pointPos);
        //        if (input->mouseFocus())
        //            input->sendFullTouchEvent(te);
        //        break;
        //    }
    default:
        break;
    }
    return false;
}





